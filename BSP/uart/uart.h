/**
 * @file uart.h
 * @brief UART 驱动库 (轮询 + 中断 + DMA)
 */
#ifndef _UART_H_
#define _UART_H_

#include "ti_msp_dl_config.h"
#include <stdbool.h>
#include <stdint.h>

/* ========== 枚举 ========== */

/*! @enum  UART 操作错误码 */
typedef enum {
    UART_OK                = 0,    /*!< 成功                  */
    UART_ERR_TIMEOUT       = 1,    /*!< 超时                  */
    UART_ERR_BUFFER_FULL   = 2,    /*!< 缓冲区满              */
    UART_ERR_INVALID_PARAM = 3,    /*!< 无效参数              */
    UART_ERR_HARDWARE      = 4,    /*!< 硬件错误              */
    UART_ERR_BUSY          = 5,    /*!< 忙                    */
} UART_Error;

/* ========== 回调类型 ========== */
typedef void (*UART_RxCallback)(uint8_t data, void *userData);       /*!< RX 中断回调 */
typedef void (*UART_TxCallback)(void *userData);                     /*!< TX 完成回调 */
typedef void (*UART_DmaRxCallback)(uint32_t available, void *arg);   /*!< DMA RX 回调 */

/* ========== API 声明 ========== */

/* --- 基础 (轮询) --- */
UART_Error UART_init(UART_Regs *uart);                                          /*!< 初始化 UART */
UART_Error UART_sendByte(UART_Regs *uart, uint8_t data);                        /*!< 发送 1 字节 */
UART_Error UART_sendString(UART_Regs *uart, const char *str);                   /*!< 发送字符串 */
UART_Error UART_sendBuffer(UART_Regs *uart, const uint8_t *buf, uint32_t len);  /*!< 发送二进制 */
UART_Error UART_receiveByte(UART_Regs *uart, uint32_t timeout, uint8_t *data);  /*!< 接收 1 字节 */
UART_Error UART_receiveString(UART_Regs *uart, char *buf, uint32_t maxLen,      /*!< 接收一行 */
                              uint32_t timeout, uint32_t *count);
bool       UART_isDataAvailable(UART_Regs *uart);                                /*!< 是否有数据 */
UART_Error UART_clearRxBuffer(UART_Regs *uart);                                  /*!< 清空 RX FIFO */
int        UART_printf(UART_Regs *uart, const char *format, ...);                /*!< 格式化打印 */

/* --- 中断模式 --- */
UART_Error UART_setRxCallback(UART_Regs *uart, UART_RxCallback cb, void *arg);   /*!< 注册 RX 回调 */
UART_Error UART_setTxCallback(UART_Regs *uart, UART_TxCallback cb, void *arg);   /*!< 注册 TX 回调 */

/* --- 波特率 --- */
UART_Error UART_setBaudRate(UART_Regs *uart, uint32_t baudRate);                 /*!< 设置波特率 */
UART_Error UART_getBaudRate(UART_Regs *uart, uint32_t *baudRate);                /*!< 获取波特率 */

/* --- DMA 模式 --- */
UART_Error UART_DMA_init(UART_Regs *uart, uint8_t *rxBuf, uint32_t rxSize,       /*!< 初始化 DMA */
                         UART_DmaRxCallback rxCb, void *rxArg,
                         UART_TxCallback txCb, void *txArg);
UART_Error UART_DMA_send(UART_Regs *uart, const uint8_t *buf, uint32_t len);     /*!< DMA 发送 */
UART_Error UART_DMA_sendString(UART_Regs *uart, const char *str);                /*!< DMA 发字符串 */
uint32_t   UART_DMA_available(UART_Regs *uart);                                   /*!< 可读字节数 */
uint32_t   UART_DMA_read(UART_Regs *uart, uint8_t *buf, uint32_t len);            /*!< 读取 DMA 数据 */
bool       UART_DMA_isTxBusy(UART_Regs *uart);                                    /*!< TX 是否忙 */
bool       UART_DMA_isRxOverflowed(UART_Regs *uart);                              /*!< RX 是否溢出 */
void       UART_DMA_clearRxOverflow(UART_Regs *uart);                             /*!< 清除溢出标志 */

/* --- 中断处理 (在启动文件中引用) --- */
void UART0_IRQHandler(void);
void UART_DMA_IRQHandler(void);

/* --- 测试 (声明在 uart_test.h 中更合适, 为兼容保留) --- */
void UART_TEST_runAll(UART_Regs *uart);
void UART_TEST_runPollingTests(UART_Regs *uart);
void UART_TEST_runDmaTests(UART_Regs *uart);

#endif
