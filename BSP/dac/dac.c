/**
 * @file dac.c
 * @brief DAC 数模转换器驱动库实现 (官方 API + 软件波形)
 *
 * 核心设计:
 * - 基础输出: DAC_setValue/setVoltage → DL_DAC12_output12()
 * - FIFO 模式: 由 SysConfig 初始化 (FIFO 使能, HWTRIG0 触发, DMA 使能)
 * - 波形生成: 软件循环 + delay_ms, 低频调试用
 */
#include "dac.h"
#include <ti/driverlib/dl_dac12.h>
#include "delay.h"

/* ========== 基础 API ========== */

void DAC_init(void) {}

void DAC_setValue(uint16_t value)
{
    if (value > DAC_MAX_VALUE) value = DAC_MAX_VALUE;              // 上限截断
    DL_DAC12_output12(DAC_INST, value);
}

void DAC_setVoltage(float voltage)
{
    if (voltage < 0.0f) voltage = 0.0f;                            // 下限截断
    if (voltage > DAC_REF_VOLTAGE) voltage = DAC_REF_VOLTAGE;      // 上限截断
    uint16_t value = (uint16_t)((voltage / DAC_REF_VOLTAGE) * DAC_MAX_VALUE);
    DL_DAC12_output12(DAC_INST, value);
}

void DAC_enableOutput(void)  { DL_DAC12_enableOutputPin(DAC_INST); }
void DAC_disableOutput(void) { DL_DAC12_disableOutputPin(DAC_INST); }

bool DAC_isOutputEnabled(void) { return DL_DAC12_isOutputPinEnabled(DAC_INST); }

/* ========== FIFO 模式 API ========== */

void DAC_enableFIFO(DL_DAC12_FIFO_THRESHOLD threshold)
{
    DL_DAC12_enableFIFO(DAC_INST);
    DL_DAC12_setFIFOThreshold(DAC_INST, threshold);
    DL_DAC12_setFIFOTriggerSource(DAC_INST, DL_DAC12_FIFO_TRIGGER_SAMPLETIMER);
}

void DAC_disableFIFO(void) { DL_DAC12_disableFIFO(DAC_INST); }

void DAC_enableSampleTimer(DL_DAC12_SAMPLES_PER_SECOND rate)
{
    DL_DAC12_enableSampleTimeGenerator(DAC_INST);
    DL_DAC12_setSampleRate(DAC_INST, rate);
}

void DAC_disableSampleTimer(void) { DL_DAC12_disableSampleTimeGenerator(DAC_INST); }

uint32_t DAC_fillFIFO(const uint16_t *buffer, uint32_t count)
{
    return DL_DAC12_fillFIFO12(DAC_INST, buffer, count);
}

/* ========== 软件波形生成 (阻塞) ========== */

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
        uint16_t v = offset + (uint16_t)((float)amplitude * sinf(rad));
        DAC_setValue(v);
        delay_ms(1);
    }
}
