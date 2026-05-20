/**
 * @file adc.c
 * @brief ADC 模数转换器驱动库实现 (阻塞轮询 + 硬件平均 + DMA)
 *
 * 核心设计:
 * - 阻塞轮询: ADC_read() 启动转换 → 等待完成 → 读结果
 * - 硬件平均: ADC_readAverage() 使用 DL_ADC12_configHwAverage 硬件累加/移位
 * - DMA 模式: 通过 MEM 结果寄存器 + DMA 连续采集，中断通知完成
 *
 * 使用官方 DriverLib API 替代手写寄存器操作，保持同类型等价替换。
 */

#include "adc.h"
#include <string.h>

/* ========== 内部常量 ========== */
#define ADC_RESULT_TIMEOUT_LOOPS    (200000U)   /*!< 轮询超时上限        */
#define ADC_DMA_CHAN_ID             (5)          /*!< DMA 通道号 (SysConfig) */
#define ADC_CHANNEL_COUNT           (16)         /*!< 最大通道数          */

/* ========== 内部全局状态 ========== */
static ADC_Status g_adcLastStatus = ADC_STATUS_OK;  /*!< 上次操作状态 */

/*! @brief 每个通道的运行状态 */
typedef struct {
    ADC_Mode        mode;           /*!< 工作模式               */
    ADC_DMA_Status  dmaStatus;      /*!< DMA 状态               */
    ADC_DMA_Config  config;         /*!< DMA 配置参数            */
    volatile bool   dmaConfigured;  /*!< DMA 是否已配置          */
    volatile bool   active;         /*!< 是否处于活跃状态        */
} ADC_ChannelState;

static ADC_ChannelState g_adcChannelStates[ADC_CHANNEL_COUNT]; /*!< 通道状态数组 */

/* ========== 内部辅助函数 ========== */

/*! @brief 获取 ADC 寄存器基址 (当前仅单实例) */
static ADC12_Regs *ADC_getInstance(ADC_CHANNEL channel)
{
    (void)channel;
    return ADC_INST_0;
}

/*! @brief 初始化单通道状态 */
static void ADC_ChannelState_Init(ADC_ChannelState *state)
{
    state->mode          = ADC_MODE_POLLING;
    state->dmaStatus     = ADC_DMA_IDLE;
    state->dmaConfigured = false;
    state->active        = false;
    memset(&state->config, 0, sizeof(ADC_DMA_Config));
}

/*! @brief 初始化所有通道状态 */
static void ADC_InitChannelStates(void)
{
    for (int i = 0; i < ADC_CHANNEL_COUNT; i++) {
        ADC_ChannelState_Init(&g_adcChannelStates[i]);
    }
}

/* ========== 公开 API 实现 ========== */

void ADC_init(void)
{
    g_adcLastStatus = ADC_STATUS_OK;
    ADC_InitChannelStates();
}

uint16_t ADC_read(ADC_CHANNEL channel)
{
    ADC12_Regs *adcInst = ADC_getInstance(channel);
    uint32_t timeoutLoops = ADC_RESULT_TIMEOUT_LOOPS;

    if (channel >= ADC_CHANNEL_COUNT) {                             // 通道合法性
        g_adcLastStatus = ADC_STATUS_INVALID_CHANNEL;
        return 0U;
    }
    if (g_adcChannelStates[channel].active) {                       // DMA 冲突
        g_adcLastStatus = ADC_STATUS_MODE_CONFLICT;
        return 0U;
    }

    /*
     * SysConfig 将 TRIGSRC 初始化为 EVENT (硬件事件触发),
     * 此时 SC 位无法启动转换, 必须临时切到 SOFTWARE 触发.
     * 修改 CTL1 前需先关 ENC (ADC12 手册要求).
     */
    DL_ADC12_disableConversions(adcInst);
    DL_Common_updateReg(&adcInst->ULLMEM.CTL1,
        DL_ADC12_TRIG_SRC_SOFTWARE, ADC12_CTL1_TRIGSRC_MASK);
    DL_ADC12_enableConversions(adcInst);

    /* 关闭硬件平均 (单次采样无需平均) */
    DL_ADC12_configHwAverage(adcInst,
        DL_ADC12_HW_AVG_NUM_ACC_DISABLED, DL_ADC12_HW_AVG_DEN_DIV_BY_1);

    /* 配置 MEM0: 指定通道、VDDA 参考、SCOMP0 采样时间、禁用平均 */
    DL_ADC12_configConversionMem(adcInst, ADC_MEM_IDX_0,
        (uint32_t)channel,
        DL_ADC12_REFERENCE_VOLTAGE_VDDA_VSSA,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0,
        DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED,
        DL_ADC12_TRIGGER_MODE_AUTO_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

    DL_ADC12_startConversion(adcInst);                              // 启动转换

    /* 轮询等待 MEM0 结果加载中断标志 */
    while ((DL_ADC12_getRawInterruptStatus(adcInst,
            DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED) == 0U)) {
        if (timeoutLoops-- == 0U) {
            /* 恢复 EVENT 触发后返回 */
            DL_ADC12_disableConversions(adcInst);
            DL_Common_updateReg(&adcInst->ULLMEM.CTL1,
                DL_ADC12_TRIG_SRC_EVENT, ADC12_CTL1_TRIGSRC_MASK);
            DL_ADC12_enableConversions(adcInst);
            g_adcLastStatus = ADC_STATUS_TIMEOUT;
            return 0U;
        }
    }

    DL_ADC12_clearInterruptStatus(adcInst, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);

    /* 恢复 EVENT 触发 (SysConfig 默认, DMA 等依赖) */
    DL_ADC12_disableConversions(adcInst);
    DL_Common_updateReg(&adcInst->ULLMEM.CTL1,
        DL_ADC12_TRIG_SRC_EVENT, ADC12_CTL1_TRIGSRC_MASK);
    DL_ADC12_enableConversions(adcInst);

    g_adcLastStatus = ADC_STATUS_OK;
    return DL_ADC12_getMemResult(adcInst, ADC_MEM_IDX_0);
}

float ADC_readVoltage(ADC_CHANNEL channel)
{
    uint16_t raw = ADC_read(channel);                               // 复用阻塞读
    return ((float)raw * ADC_REFERENCE_VOLTAGE) / (float)ADC_MAX_VALUE;
}

uint16_t ADC_readAverage(ADC_CHANNEL channel, uint32_t samples)
{
    ADC12_Regs *adcInst = ADC_getInstance(channel);
    uint32_t timeoutLoops = ADC_RESULT_TIMEOUT_LOOPS;

    if (channel >= ADC_CHANNEL_COUNT) {                             // 通道合法性
        g_adcLastStatus = ADC_STATUS_INVALID_CHANNEL;
        return 0U;
    }
    if (samples < 2) {                                              // 最少 2 次
        g_adcLastStatus = ADC_STATUS_INVALID_PARAM;
        return 0U;
    }
    if (g_adcChannelStates[channel].active) {
        g_adcLastStatus = ADC_STATUS_MODE_CONFLICT;
        return 0U;
    }

    /*
     * SysConfig 将 TRIGSRC 初始化为 EVENT (硬件事件触发),
     * 此时 SC 位无法启动转换, 必须临时切到 SOFTWARE 触发.
     * 修改 CTL1 前需先关 ENC (ADC12 手册要求).
     */
    DL_ADC12_disableConversions(adcInst);
    DL_Common_updateReg(&adcInst->ULLMEM.CTL1,
        DL_ADC12_TRIG_SRC_SOFTWARE, ADC12_CTL1_TRIGSRC_MASK);
    DL_ADC12_enableConversions(adcInst);

    /* 配置硬件平均: 累加 samples 次, 右移 (除 samples) */
    DL_ADC12_configHwAverage(adcInst, ADC_HW_AVG_NUM(samples), ADC_HW_AVG_DEN(samples));

    /* 配置 MEM0: 开启平均模式 */
    DL_ADC12_configConversionMem(adcInst, ADC_MEM_IDX_0,
        (uint32_t)channel,
        DL_ADC12_REFERENCE_VOLTAGE_VDDA_VSSA,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0,
        DL_ADC12_AVERAGING_MODE_ENABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED,
        DL_ADC12_TRIGGER_MODE_AUTO_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

    DL_ADC12_startConversion(adcInst);                              // 启动转换

    /* 轮询等待硬件平均完成 (单次结果即平均后的值) */
    while ((DL_ADC12_getRawInterruptStatus(adcInst,
            DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED) == 0U)) {
        if (timeoutLoops-- == 0U) {
            DL_ADC12_configHwAverage(adcInst,                      // 恢复禁用
                DL_ADC12_HW_AVG_NUM_ACC_DISABLED, DL_ADC12_HW_AVG_DEN_DIV_BY_1);
            /* 恢复 EVENT 触发后返回 */
            DL_ADC12_disableConversions(adcInst);
            DL_Common_updateReg(&adcInst->ULLMEM.CTL1,
                DL_ADC12_TRIG_SRC_EVENT, ADC12_CTL1_TRIGSRC_MASK);
            DL_ADC12_enableConversions(adcInst);
            g_adcLastStatus = ADC_STATUS_TIMEOUT;
            return 0U;
        }
    }

    DL_ADC12_clearInterruptStatus(adcInst, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);

    /* 恢复禁用硬件平均 (不影响后续单次读取) */
    DL_ADC12_configHwAverage(adcInst,
        DL_ADC12_HW_AVG_NUM_ACC_DISABLED, DL_ADC12_HW_AVG_DEN_DIV_BY_1);

    /* 恢复 EVENT 触发 (SysConfig 默认, DMA 等依赖) */
    DL_ADC12_disableConversions(adcInst);
    DL_Common_updateReg(&adcInst->ULLMEM.CTL1,
        DL_ADC12_TRIG_SRC_EVENT, ADC12_CTL1_TRIGSRC_MASK);
    DL_ADC12_enableConversions(adcInst);

    g_adcLastStatus = ADC_STATUS_OK;
    return DL_ADC12_getMemResult(adcInst, ADC_MEM_IDX_0);
}

ADC_Status ADC_getLastStatus(void)
{
    return g_adcLastStatus;
}

/* ========== DMA 内部辅助 ========== */

/*! @brief 配置 DMA 通道 (源=MEM0结果寄存器, 目标=用户缓冲区) */
static void ADC_DMASetupChannel(ADC_CHANNEL channel)
{
    ADC_ChannelState *state = &g_adcChannelStates[channel];
    ADC12_Regs *adcInst = ADC_getInstance(channel);

    if (adcInst == NULL || state->config.buffer == NULL) return;

    DL_DMA_disableChannel(DMA, ADC_DMA_CHAN_ID);

    DL_DMA_setTransferSize(DMA, ADC_DMA_CHAN_ID, state->config.bufferSize);
    DL_DMA_setSrcAddr(DMA, ADC_DMA_CHAN_ID,
        DL_ADC12_getMemResultAddress(adcInst, ADC_MEM_IDX_0));
    DL_DMA_setDestAddr(DMA, ADC_DMA_CHAN_ID, (uint32_t)(state->config.buffer));
    DL_DMA_setSrcIncrement(DMA, ADC_DMA_CHAN_ID, DL_DMA_ADDR_UNCHANGED);
    DL_DMA_setDestIncrement(DMA, ADC_DMA_CHAN_ID, DL_DMA_ADDR_INCREMENT);
    DL_DMA_setTransferMode(DMA, ADC_DMA_CHAN_ID,
        DL_DMA_FULL_CH_REPEAT_BLOCK_TRANSFER_MODE);

    DL_DMA_clearInterruptStatus(DMA, DL_DMA_INTERRUPT_CHANNEL5);
    DL_DMA_enableInterrupt(DMA, DL_DMA_INTERRUPT_CHANNEL5);
}

/*! @brief DMA 传输完成后的重启处理 */
static void ADC_DMARestartTransfer(ADC_CHANNEL channel)
{
    ADC_ChannelState *state = &g_adcChannelStates[channel];
    if (!state->config.autoRestart) {
        state->dmaStatus = ADC_DMA_FULL_COMPLETE;
        return;
    }
    ADC_DMASetupChannel(channel);
    DL_DMA_enableChannel(DMA, ADC_DMA_CHAN_ID);
    state->dmaStatus = ADC_DMA_RUNNING;
}

/* ========== DMA 中断处理 ========== */

void ADC_DMA_IRQHandler(void)
{
    uint32_t status = DL_DMA_getRawInterruptStatus(DMA, DL_DMA_INTERRUPT_CHANNEL5);
    DL_DMA_clearInterruptStatus(DMA, status);

    if ((status & DL_DMA_INTERRUPT_CHANNEL5) != 0U) {
        for (uint8_t ch = 0; ch < ADC_CHANNEL_COUNT; ch++) {
            ADC_ChannelState *state = &g_adcChannelStates[ch];
            if (state->mode == ADC_MODE_DMA && state->active) {
                state->dmaStatus = ADC_DMA_FULL_COMPLETE;
                if (state->config.callback != NULL) {
                    state->config.callback((ADC_CHANNEL)ch,
                        state->config.buffer, state->config.bufferSize,
                        state->config.callbackArg);
                }
                ADC_DMARestartTransfer(ch);
                break;
            }
        }
    }
}

/* ========== DMA 公开 API ========== */

ADC_Status ADC_configDMA(ADC_CHANNEL channel, const ADC_DMA_Config *config)
{
    if (channel >= ADC_CHANNEL_COUNT) {
        g_adcLastStatus = ADC_STATUS_INVALID_CHANNEL;
        return ADC_STATUS_INVALID_CHANNEL;
    }
    if (config == NULL || config->buffer == NULL) {
        g_adcLastStatus = ADC_STATUS_INVALID_PARAM;
        return ADC_STATUS_INVALID_PARAM;
    }
    if (config->useDoubleBuffer && config->buffer2 == NULL) {
        g_adcLastStatus = ADC_STATUS_INVALID_PARAM;
        return ADC_STATUS_INVALID_PARAM;
    }

    ADC_ChannelState *state = &g_adcChannelStates[channel];
    state->config = *config;
    state->dmaConfigured = true;
    g_adcLastStatus = ADC_STATUS_OK;
    return ADC_STATUS_OK;
}

ADC_Status ADC_startDMA(ADC_CHANNEL channel)
{
    if (channel >= ADC_CHANNEL_COUNT) {
        g_adcLastStatus = ADC_STATUS_INVALID_CHANNEL;
        return ADC_STATUS_INVALID_CHANNEL;
    }

    ADC_ChannelState *state = &g_adcChannelStates[channel];
    if (!state->dmaConfigured) {
        g_adcLastStatus = ADC_STATUS_DMA_NOT_CONFIGURED;
        return ADC_STATUS_DMA_NOT_CONFIGURED;
    }

    ADC12_Regs *adcInst = ADC_getInstance(channel);

    ADC_DMASetupChannel(channel);

    /* 配置 MEM0 转换通道 (重复模式由 SysConfig 初始化保证) */
    DL_ADC12_configConversionMem(adcInst, ADC_MEM_IDX_0,
        (uint32_t)channel,
        DL_ADC12_REFERENCE_VOLTAGE_VDDA_VSSA,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0,
        DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED,
        DL_ADC12_TRIGGER_MODE_AUTO_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

    DL_DMA_enableChannel(DMA, ADC_DMA_CHAN_ID);

    state->active    = true;
    state->mode      = ADC_MODE_DMA;
    state->dmaStatus = ADC_DMA_RUNNING;
    g_adcLastStatus  = ADC_STATUS_OK;
    return ADC_STATUS_OK;
}

ADC_Status ADC_stopDMA(ADC_CHANNEL channel)
{
    if (channel >= ADC_CHANNEL_COUNT) {
        g_adcLastStatus = ADC_STATUS_INVALID_CHANNEL;
        return ADC_STATUS_INVALID_CHANNEL;
    }

    ADC_ChannelState *state = &g_adcChannelStates[channel];
    DL_DMA_disableChannel(DMA, ADC_DMA_CHAN_ID);

    state->active    = false;
    state->dmaStatus = ADC_DMA_IDLE;
    g_adcLastStatus  = ADC_STATUS_OK;
    return ADC_STATUS_OK;
}

ADC_DMA_Status ADC_getDMAStatus(ADC_CHANNEL channel)
{
    if (channel >= ADC_CHANNEL_COUNT) return ADC_DMA_IDLE;
    return g_adcChannelStates[channel].dmaStatus;
}

ADC_Status ADC_setMode(ADC_CHANNEL channel, ADC_Mode mode)
{
    if (channel >= ADC_CHANNEL_COUNT) {
        g_adcLastStatus = ADC_STATUS_INVALID_CHANNEL;
        return ADC_STATUS_INVALID_CHANNEL;
    }
    ADC_ChannelState *state = &g_adcChannelStates[channel];
    if (state->active) ADC_stopDMA(channel);
    state->mode = mode;
    g_adcLastStatus = ADC_STATUS_OK;
    return ADC_STATUS_OK;
}

ADC_Mode ADC_getMode(ADC_CHANNEL channel)
{
    if (channel >= ADC_CHANNEL_COUNT) return ADC_MODE_POLLING;
    return g_adcChannelStates[channel].mode;
}

bool ADC_isDMAMode(ADC_CHANNEL channel)
{
    if (channel >= ADC_CHANNEL_COUNT) return false;
    return (g_adcChannelStates[channel].mode == ADC_MODE_DMA);
}

uint32_t ADC_getDMADataCount(ADC_CHANNEL channel)
{
    if (channel >= ADC_CHANNEL_COUNT) return 0;
    /* 通过 DMA 传输大小寄存器获取已传输次数 */
    return DL_DMA_getTransferSize(DMA, ADC_DMA_CHAN_ID);
}

ADC_Status ADC_resetDMABuffer(ADC_CHANNEL channel)
{
    if (channel >= ADC_CHANNEL_COUNT) {
        g_adcLastStatus = ADC_STATUS_INVALID_CHANNEL;
        return ADC_STATUS_INVALID_CHANNEL;
    }
    g_adcLastStatus = ADC_STATUS_OK;
    return ADC_STATUS_OK;
}
