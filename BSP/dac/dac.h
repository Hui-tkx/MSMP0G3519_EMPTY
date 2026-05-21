/**
 * @file dac.h
 * @brief DAC 数模转换器驱动库
 *
 * 方案: DMA + 双缓冲, 不用 FIFO
 *   - TIMER_8K Zero事件 → FPUB_0 → 事件通道1 → DMA Generic SUB0 → DMA CH4
 *   - DMA CH4: REPEAT_BLOCK, src=INC, dst=UNCHANGED → 直接写 DAC0->DATA0
 *   - 双缓冲 ping-pong: 播放 bufA 时 bufB 可被填充新波形
 */
#ifndef _DAC_H_
#define _DAC_H_

#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdbool.h>

/* ========== 硬件常量 ========== */
#define DAC_INST            DAC0
#define DAC_RESOLUTION      12U
#define DAC_MAX_VALUE       4095U
#define DAC_REF_VOLTAGE     3.3f

#define DAC_SAMPLE_RATE_HZ  8000UL
#define DAC_WAVE_BUF_SIZE   256

/* ========== API ========== */

void DAC_init(void);
void DAC_setValue(uint16_t value);
void DAC_setVoltage(float voltage);
void DAC_enableOutput(void);
void DAC_disableOutput(void);
bool DAC_isOutputEnabled(void);

/* DMA + 双缓冲波形播放 */
void DAC_waveStart(const uint16_t *buf, uint32_t len);
void DAC_waveStop(void);
bool DAC_waveIsPlaying(void);

/* 波形表构建 */
typedef enum {
    DAC_WAVE_SINE     = 0,
    DAC_WAVE_SQUARE   = 1,
    DAC_WAVE_TRIANGLE = 2,
    DAC_WAVE_SAWTOOTH = 3,
} DAC_WaveType;

uint32_t DAC_buildWaveform(uint16_t *buf, uint32_t len,
                           DAC_WaveType type,
                           uint16_t offset, uint16_t amplitude);

void DAC_playWaveform(uint16_t *buf, uint32_t len,
                      DAC_WaveType type,
                      uint16_t offset, uint16_t amplitude);

/* 软件波形 (阻塞, 调试用) */
void DAC_generateTriangleWave(uint16_t minVal, uint16_t maxVal,
                              uint16_t step, uint32_t delayMs);
void DAC_generateSineWave(uint16_t amplitude, uint16_t offset,
                          uint32_t frequency, uint32_t durationMs);

#endif
