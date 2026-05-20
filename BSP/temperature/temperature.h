/**
 * @file temperature.h
 * @brief 温度传感器驱动库 (ADC CH11 + 内部 1.4V VREF)
 *
 * 前置条件:
 *   SYSCFG_DL_init() 必须先调用, 其中 VREF 配置为:
 *   - 使能内部 1.4V 基准 (DL_VREF_BUFCONFIG_OUTPUT_1_4V)
 *   - 禁用采样保持 (DL_VREF_SHMODE_DISABLE)
 *
 * 公式 (TRM SLAU846D):
 *   VSAMPLE = VREF / 2^RES × (ADCCODE - 0.5)
 *   TSAMPLE = (1 / TSc) × (VSAMPLE - VTRIM) + TSTRIM
 *
 * 参数:
 *   VREF=1.4V, RES=12-bit, TSc=-1.9mV/℃, TSTRIM=30℃
 */
#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

#include <stdint.h>
#include <stdbool.h>

/* ========== 温度传感器常量 ========== */
#define TEMP_ADC_CHANNEL        (11)            /*!< 温度传感器 ADC 通道号   */
#define TEMP_VREF_MV            (1400)          /*!< 内部 VREF 电压 (mV)     */
#define TEMP_ADC_RESOLUTION     (4095)          /*!< 12-bit ADC 满量程       */
#define TEMP_TSTRIM             (30)            /*!< 出厂修整温度 (℃)        */
#define TEMP_TC_MV_PER_C        (-19)           /*!< 温度系数 (×10, 即 -1.9) */
#define TEMP_SAMPLE_TIME_CLKS   (416)           /*!< 采样时钟数 (≥12.5µs)    */

/* ========== API 声明 ========== */

void     TEMP_init(void);                               /*!< 初始化 VREF + ADC 温度测量 */
void     TEMP_deinit(void);                             /*!< 停用温度测量, 恢复 ADC 原配置 */
int32_t  TEMP_read_x10(void);                           /*!< 读温度 (×10, 如 256=25.6℃) */
float    TEMP_read(void);                               /*!< 读温度 (℃, 浮点) */
int32_t  TEMP_getCalibration_x10(void);                 /*!< 获取出厂校准温度 (×10) */
uint32_t TEMP_getCalCode(void);                         /*!< 获取出厂校准 ADC 码值 (调试用) */
uint16_t TEMP_readRaw(void);                            /*!< 阻塞读一次 ADC CH11 (调试用) */

/* 调试: 注册打印函数, 在关键步骤输出状态 */
void TEMP_setDebugPrint(void (*fn)(const char *));      /*!< 设调试打印回调 */

#endif /* _TEMPERATURE_H_ */
