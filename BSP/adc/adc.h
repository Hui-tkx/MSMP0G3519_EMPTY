/**
 * @file adc.h
 * @brief ADC 模数转换器驱动库 (阻塞轮询 + 硬件平均 + DMA)
 */
#ifndef _ADC_H_
#define _ADC_H_

#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdbool.h>

/* ========== 硬件常量 ========== */
#define ADC_INST_0            ADC12_0_INST           /*!< ADC 外设实例           */
#define ADC_MEM_IDX_0         DL_ADC12_MEM_IDX_0     /*!< 转换内存索引 0         */
#define ADC_MAX_VALUE         (4095U)                /*!< 12-bit 满量程值        */
#define ADC_REFERENCE_VOLTAGE (3.3f)                 /*!< 参考电压 3.3V          */
#define ADC_DEFAULT_CHANNEL   (0)                    /*!< 默认通道 (SysConfig)   */

/* ========== 枚举定义 ========== */

/*! @enum  ADC 操作状态 */
typedef enum {
    ADC_STATUS_OK               = 0,    /*!< 操作成功                    */
    ADC_STATUS_INVALID_CHANNEL  = 1,    /*!< 无效通道号                  */
    ADC_STATUS_TIMEOUT          = 2,    /*!< 转换超时                    */
    ADC_STATUS_INVALID_PARAM    = 3,    /*!< 无效参数                    */
    ADC_STATUS_DMA_NOT_CONFIGURED = 4,  /*!< DMA 未配置                  */
    ADC_STATUS_MODE_CONFLICT    = 5,    /*!< 模式冲突                    */
    ADC_STATUS_DMA_RUNNING      = 6,    /*!< DMA 运行中                  */
    ADC_STATUS_DMA_ERROR        = 7,    /*!< DMA 错误                    */
} ADC_Status;

/*! @enum  ADC 工作模式: POLLING=轮询, DMA=硬件传输 */
typedef enum {
    ADC_MODE_POLLING = 0,   /*!< 轮询模式 (阻塞读) */
    ADC_MODE_DMA     = 1,   /*!< DMA 模式 (连续采集) */
} ADC_Mode;

/*! @enum  ADC DMA 传输状态 */
typedef enum {
    ADC_DMA_IDLE           = 0,    /*!< DMA 空闲          */
    ADC_DMA_RUNNING        = 1,    /*!< DMA 传输中        */
    ADC_DMA_HALF_COMPLETE  = 2,    /*!< 半缓冲完成        */
    ADC_DMA_FULL_COMPLETE  = 3,    /*!< 全传输完成        */
    ADC_DMA_ERROR          = 4,    /*!< DMA 错误          */
} ADC_DMA_Status;

/*! @enum  ADC 模拟输入通道 (与 SysConfig 对应) */
typedef enum {
    ADC_CHANNEL_0 = 0,  /*!< 通道 0 (PA27) */
    ADC_CHANNEL_1 = 1,  /*!< 通道 1 (预留) */
} ADC_CHANNEL;

/* ========== DMA 回调 & 配置 ========== */

/*! @brief DMA 传输完成回调 (channel, buffer, length, arg) */
typedef void (*ADC_DMA_Callback)(ADC_CHANNEL channel, uint16_t *buffer,
                                  uint32_t length, void *arg);

/*! @brief DMA 配置参数 */
typedef struct {
    uint16_t        *buffer;         /*!< 数据缓冲区              */
    uint32_t         bufferSize;     /*!< 缓冲区大小 (样本数)     */
    bool             useDoubleBuffer;/*!< 是否双缓冲              */
    uint16_t        *buffer2;        /*!< 第二缓冲区 (双缓冲用)   */
    ADC_DMA_Callback callback;       /*!< 传输完成回调            */
    void            *callbackArg;    /*!< 回调参数                */
    bool             autoRestart;    /*!< 完成后自动重启          */
} ADC_DMA_Config;

/* ========== 硬件平均配置辅助宏 ========== */

/*! @brief 将采样次数转为 DL_ADC12_HW_AVG_NUM 宏 */
#define ADC_HW_AVG_NUM(n) \
    ((n) <= 2  ? DL_ADC12_HW_AVG_NUM_ACC_2   : \
     (n) <= 4  ? DL_ADC12_HW_AVG_NUM_ACC_4   : \
     (n) <= 8  ? DL_ADC12_HW_AVG_NUM_ACC_8   : \
     (n) <= 16 ? DL_ADC12_HW_AVG_NUM_ACC_16  : \
     (n) <= 32 ? DL_ADC12_HW_AVG_NUM_ACC_32  : \
     (n) <= 64 ? DL_ADC12_HW_AVG_NUM_ACC_64  : \
                 DL_ADC12_HW_AVG_NUM_ACC_128)

/*! @brief 将采样次数转为 DL_ADC12_HW_AVG_DEN 宏 (分母=分子) */
#define ADC_HW_AVG_DEN(n) \
    ((n) <= 2  ? DL_ADC12_HW_AVG_DEN_DIV_BY_2   : \
     (n) <= 4  ? DL_ADC12_HW_AVG_DEN_DIV_BY_4   : \
     (n) <= 8  ? DL_ADC12_HW_AVG_DEN_DIV_BY_8   : \
     (n) <= 16 ? DL_ADC12_HW_AVG_DEN_DIV_BY_16  : \
     (n) <= 32 ? DL_ADC12_HW_AVG_DEN_DIV_BY_32  : \
     (n) <= 64 ? DL_ADC12_HW_AVG_DEN_DIV_BY_64  : \
                 DL_ADC12_HW_AVG_DEN_DIV_BY_128)

/* ========== API 函数声明 ========== */

void        ADC_init(void);                                         /*!< 初始化 ADC 运行态 */
uint16_t    ADC_read(ADC_CHANNEL channel);                          /*!< 阻塞读取一次原始值 (0~4095) */
float       ADC_readVoltage(ADC_CHANNEL channel);                   /*!< 读取电压值 (V) */
uint16_t    ADC_readAverage(ADC_CHANNEL channel, uint32_t samples); /*!< 硬件平均采样 (samples=2/4/8/16/32/64/128) */
ADC_Status  ADC_getLastStatus(void);                                /*!< 获取上次操作状态 */

/* --- DMA 模式 API --- */
ADC_Status      ADC_configDMA(ADC_CHANNEL channel, const ADC_DMA_Config *config); /*!< 配置 DMA */
ADC_Status      ADC_startDMA(ADC_CHANNEL channel);                                 /*!< 启动 DMA 采集 */
ADC_Status      ADC_stopDMA(ADC_CHANNEL channel);                                  /*!< 停止 DMA 采集 */
ADC_DMA_Status  ADC_getDMAStatus(ADC_CHANNEL channel);                             /*!< 查询 DMA 状态 */
ADC_Status      ADC_setMode(ADC_CHANNEL channel, ADC_Mode mode);                   /*!< 切换模式 */
ADC_Mode        ADC_getMode(ADC_CHANNEL channel);                                  /*!< 查询当前模式 */
bool            ADC_isDMAMode(ADC_CHANNEL channel);                                 /*!< 是否 DMA 模式 */
uint32_t        ADC_getDMADataCount(ADC_CHANNEL channel);                          /*!< DMA 已采样本数 */
ADC_Status      ADC_resetDMABuffer(ADC_CHANNEL channel);                           /*!< 重置 DMA 缓冲区 */

#endif
