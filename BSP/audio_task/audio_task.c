/**
 * @file audio_task.c
 * @brief 音频采集/回放任务实现。
 * @details 实现 ADC+DMA 录音、DAC+DMA 播放、W25Q64 非阻塞读写状态机。
 * @note 大白话：这里管理"录音→存 Flash"和"Flash→播放"的全过程。
 */

#include "audio_task.h"
#include "w25q64.h"
#include "led.h"

/*==============================================================================
 * 缓冲区
 *============================================================================*/

static uint16_t adc_buf_0[DMA_CHUNK_SIZE];
static uint16_t adc_buf_1[DMA_CHUNK_SIZE];
static uint16_t adc_buf_2[DMA_CHUNK_SIZE];
static uint16_t adc_buf_3[DMA_CHUNK_SIZE];

static uint16_t *const adc_buf_table[ADC_BUF_COUNT] = {
    adc_buf_0, adc_buf_1, adc_buf_2, adc_buf_3
};

static uint16_t dac_buf_0[DMA_CHUNK_SIZE];
static uint16_t dac_buf_1[DMA_CHUNK_SIZE];
static uint16_t dac_buf_2[DMA_CHUNK_SIZE];
static uint16_t dac_buf_3[DMA_CHUNK_SIZE];

static uint16_t *const dac_buf_table[DAC_BUF_COUNT] = {
    dac_buf_0, dac_buf_1, dac_buf_2, dac_buf_3
};

uint16_t loopback_buf_a[LOOPBACK_SIZE];
uint16_t loopback_buf_b[LOOPBACK_SIZE];

/*==============================================================================
 * 内部变量
 *============================================================================*/

/* ADC 缓冲区空闲标志 */
static volatile bool adc_buf_free[ADC_BUF_COUNT] = { true };

/* ADC 完成队列 */
static volatile uint8_t  full_queue[ADC_BUF_COUNT];
static volatile uint32_t full_start[ADC_BUF_COUNT];
static volatile uint32_t full_len[ADC_BUF_COUNT];
static volatile uint8_t  full_head = 0;
static volatile uint8_t  full_tail = 0;
static volatile uint8_t  full_count = 0;

static volatile uint8_t  adc_active_idx = 0;
static volatile uint32_t adc_active_start = 0;
static volatile uint32_t adc_active_len = 0;

/* DAC 缓冲就绪标志 */
static volatile bool dac_buf_ready[DAC_BUF_COUNT] = { false };
static volatile uint8_t dac_play_idx = 0;

/* 录音/播放计数器 */
static volatile uint32_t record_started_samples = 0;
static volatile uint32_t record_idx = 0;
static volatile uint32_t play_idx = 0;
static volatile uint32_t actual_recorded_samples = 0;

/* Flash 写入状态机 */
static volatile bool     flash_write_busy = false;
static volatile uint8_t  flash_write_idx = 0;
static volatile uint32_t flash_write_addr = 0;
static volatile uint32_t flash_write_len = 0;

/* DMA 标志 */
static volatile bool record_dma_running = false;
static volatile bool record_overrun_flag = false;
volatile bool adc_dma_done = false;
volatile bool dac_dma_done = false;

/* 直通模式 */
static volatile uint8_t loopback_buf_idx = 0;

/* 音量 */
static volatile uint8_t volume_gain = 2;

/*==============================================================================
 * 内部函数声明
 *============================================================================*/

static uint8_t  ADC_FindFreeBuffer(void);
static bool     ADC_QueueFullBuffer(uint8_t idx, uint32_t start, uint32_t len);
static bool     ADC_StartNextDMA(void);
static void     ADC_Reset(void);
static void     Flash_StartWrite(uint8_t buf_idx, uint32_t sample_start, uint32_t sample_len);
static void     Flash_CheckDone(void);
static void     DAC_PreloadAll(void);

/*==============================================================================
 * 内部实现
 *============================================================================*/

static uint8_t ADC_FindFreeBuffer(void)
{
    for (uint8_t i = 0; i < ADC_BUF_COUNT; i++) {
        if (adc_buf_free[i]) return i;
    }
    return 0xFF;
}

static bool ADC_QueueFullBuffer(uint8_t idx, uint32_t start, uint32_t len)
{
    if (full_count >= ADC_BUF_COUNT) return false;

    full_queue[full_tail] = idx;
    full_start[full_tail] = start;
    full_len[full_tail]   = len;
    full_tail = (full_tail + 1) % ADC_BUF_COUNT;
    full_count++;
    return true;
}

static bool ADC_StartNextDMA(void)
{
    if (record_started_samples >= TOTAL_SAMPLES) {
        record_dma_running = false;
        DL_DMA_disableChannel(DMA, ADC_DMA_CHAN_ID);
        DL_ADC12_disableConversions(ADC12_0_INST);
        return false;
    }

    uint8_t next = ADC_FindFreeBuffer();
    if (next == 0xFF) {
        record_overrun_flag = true;
        record_dma_running = false;
        DL_DMA_disableChannel(DMA, ADC_DMA_CHAN_ID);
        DL_ADC12_disableConversions(ADC12_0_INST);
        return false;
    }

    uint32_t remain = TOTAL_SAMPLES - record_started_samples;
    uint32_t len    = (remain > DMA_CHUNK_SIZE) ? DMA_CHUNK_SIZE : remain;

    adc_buf_free[next] = false;
    adc_active_idx      = next;
    adc_active_start    = record_started_samples;
    adc_active_len      = len;
    record_started_samples += len;
    record_dma_running  = true;

    DL_DMA_disableChannel(DMA, ADC_DMA_CHAN_ID);
    DL_DMA_setTransferSize(DMA, ADC_DMA_CHAN_ID, len);
    DL_DMA_setSrcAddr(DMA, ADC_DMA_CHAN_ID,
        (uint32_t)DL_ADC12_getMemResultAddress(ADC12_0_INST, DL_ADC12_MEM_IDX_0));
    DL_DMA_setDestAddr(DMA, ADC_DMA_CHAN_ID, (uint32_t)adc_buf_table[next]);
    DL_DMA_clearInterruptStatus(DMA, (1UL << ADC_DMA_CHAN_ID));
    DL_DMA_enableInterrupt(DMA, (1UL << ADC_DMA_CHAN_ID));
    DL_DMA_enableChannel(DMA, ADC_DMA_CHAN_ID);
    return true;
}

static void ADC_Reset(void)
{
    for (uint8_t i = 0; i < ADC_BUF_COUNT; i++) adc_buf_free[i] = true;

    full_head = full_tail = full_count = 0;
    adc_active_idx = adc_active_start = adc_active_len = 0;
    record_started_samples = record_idx = play_idx = actual_recorded_samples = 0;
    record_dma_running = record_overrun_flag = false;
    adc_dma_done = dac_dma_done = false;
    flash_write_busy = false;
    flash_write_idx = 0;
}

static void Flash_StartWrite(uint8_t buf_idx, uint32_t sample_start, uint32_t sample_len)
{
    flash_write_idx  = buf_idx;
    flash_write_addr = sample_start * 2;
    flash_write_len  = sample_len;
    flash_write_busy = true;

    W25Q64_write16((uint16_t *)adc_buf_table[buf_idx], flash_write_addr, flash_write_len);
}

static void Flash_CheckDone(void)
{
    if (!flash_write_busy) return;

    if (W25Q64_Process() == W25Q64_NB_DONE) {
        flash_write_busy = false;
        adc_buf_free[flash_write_idx] = true;
        record_idx = flash_write_addr / 2 + flash_write_len;
        if (record_idx > actual_recorded_samples) {
            actual_recorded_samples = record_idx;
        }
    }
}

static void DAC_PreloadAll(void)
{
    for (uint8_t i = 0; i < DAC_BUF_COUNT; i++) dac_buf_ready[i] = false;
    dac_play_idx = 0;

    uint32_t total = actual_recorded_samples;
    for (uint8_t i = 0; i < DAC_BUF_COUNT && play_idx < total; i++) {
        uint32_t chunk = total - play_idx;
        if (chunk > DMA_CHUNK_SIZE) chunk = DMA_CHUNK_SIZE;

        W25Q64_read16((uint16_t *)dac_buf_table[i], play_idx * 2, chunk);
        while (W25Q64_Process() != W25Q64_NB_DONE);

        for (uint32_t j = chunk; j < DMA_CHUNK_SIZE; j++) {
            dac_buf_table[i][j] = 0;
        }
        dac_buf_ready[i] = true;
        play_idx += chunk;
    }
}

/*==============================================================================
 * 中断入口（由 DMA_IRQHandler 调用）
 *============================================================================*/

void Audio_DMA_ADC_Done(void)
{
    if (record_dma_running) {
        uint8_t done_idx = adc_active_idx;
        if (!ADC_QueueFullBuffer(done_idx, adc_active_start, adc_active_len)) {
            record_overrun_flag = true;
            record_dma_running = false;
            DL_DMA_disableChannel(DMA, ADC_DMA_CHAN_ID);
            DL_ADC12_disableConversions(ADC12_0_INST);
        } else {
            ADC_StartNextDMA();
        }
        LED_Toggle(LED_L1);
    } else {
        adc_dma_done = true;
    }
}

void Audio_DMA_DAC_Done(void)
{
    dac_dma_done = true;
}

/*==============================================================================
 * 对外 API
 *============================================================================*/

void Audio_Init(void)
{
    W25Q64_InitConfig();
    ADC_Reset();
}

bool Audio_StartRecord(void)
{
    uint32_t total_bytes = TOTAL_SAMPLES * 2;
    uint32_t addr = 0;

    while (addr < total_bytes) {
        W25Q64_erase_sector(addr);
        while (W25Q64_ReadState() & W25Q64_BUSY);
        addr += W25Q64_SECTOR_SIZE;
    }

    ADC_Reset();
    DL_ADC12_enableConversions(ADC12_0_INST);
    return ADC_StartNextDMA();
}

void Audio_StopRecord(void)
{
    DL_DMA_disableChannel(DMA, ADC_DMA_CHAN_ID);
    DL_ADC12_disableConversions(ADC12_0_INST);
    record_dma_running = false;

    while (full_count > 0) {
        uint8_t bi = full_queue[full_head];
        uint32_t ss = full_start[full_head];
        uint32_t sl = full_len[full_head];
        full_head = (full_head + 1) % ADC_BUF_COUNT;
        if (full_count > 0) full_count--;

        Flash_StartWrite(bi, ss, sl);
        while (flash_write_busy) {
            W25Q64_Process();
            if (!flash_write_busy) {
                adc_buf_free[bi] = true;
            }
        }
        record_idx = ss + sl;
    }
    actual_recorded_samples = record_idx;
}

bool Audio_StartPlayback(void)
{
    if (actual_recorded_samples == 0) return false;

    DL_ADC12_disableConversions(ADC12_0_INST);
    DL_DMA_disableChannel(DMA, ADC_DMA_CHAN_ID);

    play_idx = 0;
    dac_dma_done = false;

    DAC_PreloadAll();

    DL_DMA_disableChannel(DMA, DMA_DAC_CHAN_ID);
    DL_DMA_setTransferSize(DMA, DMA_DAC_CHAN_ID, DMA_CHUNK_SIZE);
    DL_DMA_setSrcAddr(DMA, DMA_DAC_CHAN_ID, (uint32_t)dac_buf_table[dac_play_idx]);
    DL_DMA_setDestAddr(DMA, DMA_DAC_CHAN_ID, (uint32_t)&DAC0->DATA0);
    DL_DMA_clearInterruptStatus(DMA, (1UL << DMA_DAC_CHAN_ID));
    DL_DMA_enableInterrupt(DMA, (1UL << DMA_DAC_CHAN_ID));
    DL_DMA_enableChannel(DMA, DMA_DAC_CHAN_ID);
    return true;
}

void Audio_StopPlayback(void)
{
    DL_DMA_disableChannel(DMA, DMA_DAC_CHAN_ID);
    dac_dma_done = false;
}

void Audio_Process(void)
{
    Flash_CheckDone();

    if (!flash_write_busy && full_count > 0) {
        uint8_t  bi = full_queue[full_head];
        uint32_t ss = full_start[full_head];
        uint32_t sl = full_len[full_head];
        full_head = (full_head + 1) % ADC_BUF_COUNT;
        if (full_count > 0) full_count--;

        Flash_StartWrite(bi, ss, sl);
        LED_Toggle(LED_L2);
    }
}

bool Audio_IsRecordDone(void)
{
    return (!record_dma_running && record_started_samples >= TOTAL_SAMPLES
            && full_count == 0 && !flash_write_busy);
}

bool Audio_IsRecordOverrun(void)
{
    return record_overrun_flag;
}

uint32_t Audio_GetRecordedSamples(void)
{
    return actual_recorded_samples;
}

bool Audio_IsDacDmaDone(void)
{
    return dac_dma_done;
}

bool Audio_IsAdcDmaDone(void)
{
    return adc_dma_done;
}

void Audio_SetVolume(uint8_t gain)
{
    if (gain == 0) gain = 1;
    volume_gain = gain;
}

void Audio_InitLoopback(void)
{
    loopback_buf_idx = 0;
    adc_dma_done = false;
    dac_dma_done = false;
}

bool Audio_RunLoopback(void)
{
    uint8_t filled = loopback_buf_idx;
    uint8_t next   = loopback_buf_idx ^ 1;

    uint16_t *fbuf = (filled == 0) ? loopback_buf_a : loopback_buf_b;
    uint16_t *nbuf = (next == 0)   ? loopback_buf_a : loopback_buf_b;

    DL_DMA_disableChannel(DMA, DMA_DAC_CHAN_ID);
    DL_DMA_setTransferSize(DMA, DMA_DAC_CHAN_ID, LOOPBACK_SIZE);
    DL_DMA_setSrcAddr(DMA, DMA_DAC_CHAN_ID, (uint32_t)fbuf);
    DL_DMA_setDestAddr(DMA, DMA_DAC_CHAN_ID, (uint32_t)&DAC0->DATA0);
    DL_DMA_clearInterruptStatus(DMA, (1UL << DMA_DAC_CHAN_ID));
    DL_DMA_enableInterrupt(DMA, (1UL << DMA_DAC_CHAN_ID));
    DL_DMA_enableChannel(DMA, DMA_DAC_CHAN_ID);

    DL_DMA_disableChannel(DMA, ADC_DMA_CHAN_ID);
    DL_DMA_setTransferSize(DMA, ADC_DMA_CHAN_ID, LOOPBACK_SIZE);
    DL_DMA_setSrcAddr(DMA, ADC_DMA_CHAN_ID,
        (uint32_t)DL_ADC12_getMemResultAddress(ADC12_0_INST, DL_ADC12_MEM_IDX_0));
    DL_DMA_setDestAddr(DMA, ADC_DMA_CHAN_ID, (uint32_t)nbuf);
    DL_DMA_clearInterruptStatus(DMA, (1UL << ADC_DMA_CHAN_ID));
    DL_DMA_enableInterrupt(DMA, (1UL << ADC_DMA_CHAN_ID));
    DL_DMA_enableChannel(DMA, ADC_DMA_CHAN_ID);

    loopback_buf_idx = next;
    return true;
}
