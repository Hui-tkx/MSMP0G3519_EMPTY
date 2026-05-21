/**
 * @file dac.c
 * @brief DAC 驱动库 — Sample Timer + FIFO + DMA 波形播放
 *
 * SysConfig 已配置:
 *   DAC12: FIFO=EN, SAMPLETIMER=EN(8kHz), DMA_TRIG=EN, threshold=TWO_QTRS_EMPTY
 *   DMA CH4: FULL_CH_REPEAT_BLOCK, src=INC, dst=UNCHANGED
 *
 * 数据流(全部 DAC 内部完成):
 *   Sample Timer @8kHz → FIFO触发 → FIFO弹出 → DAC输出
 *                                       ↓
 *                                FIFO空位≥阈值 → DMA触发 → 搬运波形数据
 */
#include "dac.h"
#include <ti/driverlib/dl_dac12.h>
#include "delay.h"
#include <math.h>

extern int UART_printf(void *uart, const char *format, ...);
static volatile bool g_wavePlaying = false;

/* ========== 基础 API ========== */

void DAC_init(void)
{
    /* SysConfig 已配好全部参数. 这里只关 DMA 通道, 待命. */
    DL_DMA_disableChannel(DMA, DMA_DAC_CHAN_ID);
    DAC_enableOutput();
}

void DAC_setValue(uint16_t value)
{
    if (value > DAC_MAX_VALUE) value = DAC_MAX_VALUE;
    DAC0->DATA0 = value;
}

void DAC_setVoltage(float voltage)
{
    if (voltage < 0.0f) voltage = 0.0f;
    if (voltage > DAC_REF_VOLTAGE) voltage = DAC_REF_VOLTAGE;
    DAC0->DATA0 = (uint16_t)((voltage / DAC_REF_VOLTAGE) * DAC_MAX_VALUE);
}

void DAC_enableOutput(void)  { DL_DAC12_enableOutputPin(DAC_INST); }
void DAC_disableOutput(void) { DL_DAC12_disableOutputPin(DAC_INST); }
bool DAC_isOutputEnabled(void) { return DL_DAC12_isOutputPinEnabled(DAC_INST); }

/* ========== Sample Timer + FIFO + DMA 波形播放 ========== */

void DAC_waveStart(const uint16_t *buf, uint32_t len)
{
    UART_printf(UART_0_INST, "\r\n[DAC] START buf=0x%08X len=%u\r\n",
                (uint32_t)buf, len);

    /* 1. 设置 DMA 参数 */
    DL_DMA_setSrcAddr(DMA, DMA_DAC_CHAN_ID, (uint32_t)buf);
    DL_DMA_setDestAddr(DMA, DMA_DAC_CHAN_ID, (uint32_t)&(DAC0->DATA0));
    DL_DMA_setTransferSize(DMA, DMA_DAC_CHAN_ID, (uint16_t)len);
    DL_DMA_enableChannel(DMA, DMA_DAC_CHAN_ID);
    DAC_enableOutput();

    /* 2. 确保 FIFO 触发源为 SAMPLETIMER */
    DL_DAC12_setFIFOTriggerSource(DAC_INST, DL_DAC12_FIFO_TRIGGER_SAMPLETIMER);

    /* 3. 预填充 FIFO 前 4 个数据, 避免初始 underrun */
    for (uint32_t k = 0; k < 4 && k < len; k++) {
        DAC0->DATA0 = buf[k];
    }

    g_wavePlaying = true;
    UART_printf(UART_0_INST, "[DAC] SA=0x%08X SZ=%u DMACTL=0x%08X\r\n",
                DMA->DMACHAN[DMA_DAC_CHAN_ID].DMASA,
                DMA->DMACHAN[DMA_DAC_CHAN_ID].DMASZ,
                DMA->DMACHAN[DMA_DAC_CHAN_ID].DMACTL);
    UART_printf(UART_0_INST, "[DAC] CTL0=0x%08X CTL2=0x%08X CTL3=0x%08X\r\n",
                DAC0->CTL0, DAC0->CTL2, DAC0->CTL3);
    UART_printf(UART_0_INST, "[DAC] DACen=%d FIFOen=%d STIMen=%d DMATRIGEN=%d\r\n",
                (DAC0->CTL0 >> 0) & 1,
                DAC0->CTL2 & 1,
                (DAC0->CTL3 >> 0) & 1,
                (DAC0->CTL2 >> 24) & 1);
    UART_printf(UART_0_INST, "[DAC] === PLAYING ===\r\n");
}

void DAC_waveStop(void)
{
    UART_printf(UART_0_INST, "[DAC] stop DMASZ=%u\r\n",
                DMA->DMACHAN[DMA_DAC_CHAN_ID].DMASZ);
    DL_DMA_disableChannel(DMA, DMA_DAC_CHAN_ID);
    g_wavePlaying = false;
    UART_printf(UART_0_INST, "[DAC] stop done\r\n");
}

bool DAC_waveIsPlaying(void) { return g_wavePlaying; }

/* ========== 波形表构建 ========== */

static uint16_t clampU12(int32_t v)
{
    if (v < 0) return 0U;
    if (v > (int32_t)DAC_MAX_VALUE) return DAC_MAX_VALUE;
    return (uint16_t)v;
}

uint32_t DAC_buildWaveform(uint16_t *buf, uint32_t len,
                           DAC_WaveType type,
                           uint16_t offset, uint16_t amplitude)
{
    if (buf == NULL || len == 0) return 0U;
    uint32_t i;
    int32_t lo, hi;

    switch (type) {
    case DAC_WAVE_SINE:
        for (i = 0; i < len; i++) {
            float rad = 2.0f * 3.14159265f * (float)i / (float)len;
            float v = (float)offset + (float)amplitude * sinf(rad);
            buf[i] = clampU12((int32_t)(v + 0.5f));
        }
        break;

    case DAC_WAVE_SQUARE: {
        uint32_t half = len / 2U;
        hi = (int32_t)offset + (int32_t)amplitude;
        lo = (int32_t)offset - (int32_t)amplitude;
        if (hi > (int32_t)DAC_MAX_VALUE) hi = DAC_MAX_VALUE;
        if (lo < 0) lo = 0;
        for (i = 0; i < half; i++) buf[i] = (uint16_t)hi;
        for (i = half; i < len; i++) buf[i] = (uint16_t)lo;
        break;
    }
    case DAC_WAVE_TRIANGLE: {
        uint32_t half = len / 2U;
        hi = (int32_t)offset + (int32_t)amplitude;
        lo = (int32_t)offset - (int32_t)amplitude;
        if (hi > (int32_t)DAC_MAX_VALUE) hi = DAC_MAX_VALUE;
        if (lo < 0) lo = 0;
        for (i = 0; i < half; i++)
            buf[i] = (uint16_t)(lo + ((hi - lo) * (int32_t)i / (int32_t)(half - 1)));
        for (i = half; i < len; i++)
            buf[i] = (uint16_t)(hi - ((hi - lo) * (int32_t)(i - half) / (int32_t)(half - 1)));
        break;
    }
    case DAC_WAVE_SAWTOOTH: {
        lo = (int32_t)offset - (int32_t)amplitude;
        hi = (int32_t)offset + (int32_t)amplitude;
        if (hi > (int32_t)DAC_MAX_VALUE) hi = DAC_MAX_VALUE;
        if (lo < 0) lo = 0;
        for (i = 0; i < len; i++)
            buf[i] = (uint16_t)(lo + ((hi - lo) * (int32_t)i / (int32_t)(len - 1)));
        break;
    }
    default: return 0U;
    }
    return len;
}

void DAC_playWaveform(uint16_t *buf, uint32_t len,
                      DAC_WaveType type,
                      uint16_t offset, uint16_t amplitude)
{
    DAC_buildWaveform(buf, len, type, offset, amplitude);
    DAC_waveStart(buf, len);
}

/* ========== 软件波形 (阻塞) ========== */

void DAC_generateTriangleWave(uint16_t minVal, uint16_t maxVal,
                              uint16_t step, uint32_t delayMs)
{
    if (minVal > maxVal) { uint16_t t = minVal; minVal = maxVal; maxVal = t; }
    if (step == 0) step = 1;
    for (uint16_t v = minVal; v <= maxVal; v += step) { DAC_setValue(v); delay_ms(delayMs); }
    for (uint16_t v = maxVal; v >= minVal; v -= step) { DAC_setValue(v); delay_ms(delayMs); }
}

void DAC_generateSineWave(uint16_t amplitude, uint16_t offset,
                          uint32_t frequency, uint32_t durationMs)
{
    if (amplitude > offset) amplitude = offset;
    if ((offset + amplitude) > DAC_MAX_VALUE) amplitude = DAC_MAX_VALUE - offset;
    uint32_t samples = (frequency > 0) ? frequency : 1;
    uint32_t iters   = (durationMs > 0) ? durationMs : 1;
    for (uint32_t t = 0; t < iters; t++) {
        float rad = 2.0f * 3.14159265f * (float)t / (float)samples;
        uint16_t v = (uint16_t)((int32_t)offset + (int32_t)((float)amplitude * sinf(rad)));
        DAC_setValue(v);
        delay_ms(1);
    }
}
