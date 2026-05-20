/**
 * @file dac.h
 * @brief DAC 数模转换器驱动库 (码值/电压输出 + 软件波形)
 */
#ifndef _DAC_H_
#define _DAC_H_

#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdbool.h>

/* ========== 硬件常量 ========== */
#define DAC_INST            DAC0            /*!< DAC 外设实例            */
#define DAC_RESOLUTION      12U             /*!< 分辨率 12-bit           */
#define DAC_MAX_VALUE       4095U           /*!< 满量程值                */
#define DAC_REF_VOLTAGE     3.3f            /*!< 参考电压 3.3V           */

/* ========== API 函数声明 ========== */

void        DAC_init(void);                                         /*!< 初始化 DAC 运行态 */
void        DAC_setValue(uint16_t value);                           /*!< 写码值 (0~4095, 自动截断) */
void        DAC_setVoltage(float voltage);                          /*!< 写电压 (V, 自动截断) */
void        DAC_enableOutput(void);                                 /*!< 使能输出引脚 */
void        DAC_disableOutput(void);                                /*!< 禁用输出引脚 */
bool        DAC_isOutputEnabled(void);                              /*!< 查询输出是否使能 */

/* --- FIFO 模式 (SysConfig 已配置, 运行时按需调用) --- */
void        DAC_enableFIFO(DL_DAC12_FIFO_THRESHOLD threshold);      /*!< 启用 FIFO + 设阈值 */
void        DAC_disableFIFO(void);                                  /*!< 禁用 FIFO */
void        DAC_enableSampleTimer(DL_DAC12_SAMPLES_PER_SECOND rate);/*!< 启用采样定时器 */
void        DAC_disableSampleTimer(void);                           /*!< 禁用采样定时器 */
uint32_t    DAC_fillFIFO(const uint16_t *buffer, uint32_t count);   /*!< 批量填充 FIFO, 返回写入数 */

/* --- 软件波形生成 (阻塞) --- */
void DAC_generateTriangleWave(uint16_t minVal, uint16_t maxVal,     /*!< 三角波 */
                              uint16_t step, uint32_t delayMs);
void DAC_generateSineWave(uint16_t amplitude, uint16_t offset,      /*!< 正弦波 */
                          uint32_t frequency, uint32_t durationMs);

#endif
