/**
 * @file uart.c
 * @brief UART 驱动库实现
 *
 * ============================================================================
 * 实现说明
 * ============================================================================
 * 基于 TI DriverLib 的 UART 外设封装，提供三种工作模式。
 *
 * 核心特性:
 * - 轮询模式: 简单直接，适合低频通信
 * - 中断模式: 收到数据自动回调，不阻塞 CPU
 * - DMA 模式: CPU 零开销，适合高速数据流
 *
 * 重要约定:
 * - timeout 参数单位为毫秒
 * - 中断优先级固定为 0x02
 *
 * ============================================================================
 * 函数示例
 * ============================================================================
 *
 * 示例 1: 调试打印
 * @code
 * // 初始化后直接打印信息
 * UART_init(UART_0_INST);
 * UART_sendString(UART_0_INST, "System started\r\n");
 * UART_printf(UART_0_INST, "ADC value: %d\r\n", adcValue);
 * @endcode
 *
 * 示例 2: 命令交互 (中断模式)
 * @code
 * char cmdBuffer[64];
 * uint32_t cmdIndex = 0;
 *
 * void onRx(uint8_t data, void *userData) {
 *     if (data == '\n' || data == '\r') {
 *         cmdBuffer[cmdIndex] = '\0';
 *         // 解析命令
 *         if (strcmp(cmdBuffer, "STATUS") == 0) {
 *             UART_sendString(UART_0_INST, "OK\r\n");
 *         }
 *         cmdIndex = 0;
 *     } else if (cmdIndex < sizeof(cmdBuffer) - 1) {
 *         cmdBuffer[cmdIndex++] = data;
 *     }
 * }
 *
 * UART_init(UART_0_INST);
 * UART_setRxCallback(UART_0_INST, onRx, NULL);
 * // 之后每收到字符都会自动调用 onRx
 * @endcode
 *
 * 示例 3: 回显功能
 * @code
 * void echoCallback(uint8_t data, void *userData) {
 *     UART_sendByte(UART_0_INST, data);  // 收到什么发回什么
 * }
 *
 * UART_init(UART_0_INST);
 * UART_setRxCallback(UART_0_INST, echoCallback, NULL);
 * @endcode
 *
 * 示例 4: 数据记录 (DMA 模式)
 * @code
 * uint8_t rxBuf[1024];
 *
 * void onRxOverflow(uint32_t avail, void *userData) {
 *     UART_printf(UART_0_INST, "RX overflow! %lu bytes\r\n", avail);
 * }
 *
 * UART_init(UART_0_INST);
 * UART_DMA_init(UART_0_INST, rxBuf, sizeof(rxBuf),
 *               onRxOverflow, NULL, NULL, NULL);
 *
 * // 主循环中处理数据
 * while (1) {
 *     uint32_t avail = UART_DMA_available(UART_0_INST);
 *     if (avail > 0) {
 *         uint8_t tmp[64];
 *         uint32_t len = UART_DMA_read(UART_0_INST, tmp, sizeof(tmp));
 *         // 处理 tmp[0..len-1]
 *     }
 * }
 * @endcode
 *
 * 示例 5: 等待发送完成
 * @code
 * volatile bool txDone = false;
 *
 * void onTxComplete(void *userData) {
 *     txDone = true;
 * }
 *
 * UART_init(UART_0_INST);
 * UART_setTxCallback(UART_0_INST, onTxComplete, NULL);
 *
 * UART_sendString(UART_0_INST, "Start\r\n");
 * while (!txDone) { }  // 等待发送完成
 * txDone = false;
 *
 * UART_sendString(UART_0_INST, "Next\r\n");  // 发送下一条
 * @endcode
 *
 * 示例 6: 动态波特率切换
 * @code
 * UART_init(UART_0_INST);
 *
 * // 高速模式
 * UART_setBaudRate(UART_0_INST, 921600);
 * UART_sendString(UART_0_INST, "High speed mode\r\n");
 *
 * delay_ms(1000);
 *
 * // 低速模式
 * UART_setBaudRate(UART_0_INST, 9600);
 * UART_sendString(UART_0_INST, "Low speed mode\r\n");
 *
 * // 恢复默认
 * UART_setBaudRate(UART_0_INST, 115200);
 * @endcode
 *
 * 示例 7: 非阻塞数据接收
 * @code
 * void pollUart(void) {
 *     while (UART_isDataAvailable(UART_0_INST)) {
 *         uint8_t byte;
 *         UART_receiveByte(UART_0_INST, 0, &byte);  // 非阻塞
 *         // 处理 byte
 *     }
 * }
 * @endcode
 *
 * 示例 8: 串口协议解析
 * @code
 * // 假设协议格式: $CMD,VALUE\r\n
 * #define CMD_BUFFER_SIZE 32
 * static char g_cmdBuf[CMD_BUFFER_SIZE];
 * static uint32_t g_cmdLen = 0;
 *
 * void protocolRx(uint8_t data, void *userData) {
 *     if (data == '\n') {
 *         g_cmdBuf[g_cmdLen] = '\0';
 *         // 解析协议
 *         if (strncmp(g_cmdBuf, "$ADC", 4) == 0) {
 *             uint16_t value = atoi(&g_cmdBuf[5]);
 *             UART_printf(UART_0_INST, "ADC=%u\r\n", value);
 *         }
 *         g_cmdLen = 0;
 *     } else if (g_cmdLen < CMD_BUFFER_SIZE - 1) {
 *         g_cmdBuf[g_cmdLen++] = data;
 *     }
 * }
 *
 * UART_init(UART_0_INST);
 * UART_setRxCallback(UART_0_INST, protocolRx, NULL);
 * @endcode
 */

#include "uart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define UART_CALLBACK_SLOT_COUNT    (1U)
#define UART_TX_TIMEOUT_MS          (100U)
#define UART_LOOPS_PER_MS           ((CPUCLK_FREQ + 4000U) / 8000U)

/* ========== 内部数据结构 ========== */

/*! @brief 中断回调槽 */
typedef struct {
    UART_Regs      *uart;          /*!< UART 实例         */
    UART_RxCallback rxCallback;    /*!< RX 回调           */
    void           *rxUserData;    /*!< RX 回调参数       */
    UART_TxCallback txCallback;    /*!< TX 完成回调       */
    void           *txUserData;    /*!< TX 回调参数       */
} UART_CallbackSlot;

/*! @brief DMA 上下文 (环形缓冲) */
typedef struct {
    UART_Regs        *uart;        /*!< UART 实例         */
    uint8_t          *rxBuffer;    /*!< RX 环形缓冲区     */
    uint32_t          rxSize;      /*!< 缓冲区大小        */
    volatile uint32_t rxTail;      /*!< 读取位置          */
    volatile bool     rxOverflow;  /*!< 溢出标志          */
    UART_DmaRxCallback rxCallback; /*!< RX 回调           */
    void             *rxUserData;  /*!< RX 回调参数       */
    UART_TxCallback   txCallback;  /*!< TX 完成回调       */
    void             *txUserData;  /*!< TX 回调参数       */
    volatile bool     txBusy;      /*!< TX 忙标志         */
    bool              initialized; /*!< 初始化标志        */
} UART_DmaContext;

static UART_CallbackSlot g_uartSlots[UART_CALLBACK_SLOT_COUNT] = {
    {UART_0_INST, NULL, NULL, NULL, NULL}
};

static UART_DmaContext g_uartDma = {
    UART_0_INST, NULL, 0U, 0U, false, NULL, NULL, NULL, NULL, false, false
};

static int32_t UART_getSlotIndex(UART_Regs *uart);
static uint32_t UART_getInputClock(UART_Regs *uart);
static UART_DmaContext *UART_getDmaContext(UART_Regs *uart);
static uint32_t UART_DMA_getHead(const UART_DmaContext *ctx);
static bool UART_waitTxReady(UART_Regs *uart);
static void UART_dispatchInterrupt(UART_Regs *uart);

/*! @brief 初始化 UART (NVIC + 默认禁用 RX/TX 中断) */
UART_Error UART_init(UART_Regs *uart)
{
    if (UART_getSlotIndex(uart) < 0) {
        return UART_ERR_INVALID_PARAM;
    }

    if (uart == UART_0_INST) {
        NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
        NVIC_SetPriority(UART_0_INST_INT_IRQN, 0x02U);
        NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

        DL_UART_Main_disableInterrupt(UART_0_INST,
                               DL_UART_MAIN_INTERRUPT_RX |
                               DL_UART_MAIN_INTERRUPT_TX);
    }

    return UART_OK;
}

/*! @brief 发送 1 字节 (轮询, 阻塞等待 TX FIFO) */
UART_Error UART_sendByte(UART_Regs *uart, uint8_t data)
{
    if (UART_getSlotIndex(uart) < 0) {
        return UART_ERR_INVALID_PARAM;
    }

    if (!UART_waitTxReady(uart)) {
        return UART_ERR_TIMEOUT;
    }

    DL_UART_transmitData(uart, data);
    return UART_OK;
}

/*! @brief 发送字符串 (轮询, 逐字节调用 sendByte) */
UART_Error UART_sendString(UART_Regs *uart, const char *str)
{
    UART_Error status;

    if ((UART_getSlotIndex(uart) < 0) || (str == NULL)) {
        return UART_ERR_INVALID_PARAM;
    }

    while (*str != '\0') {
        status = UART_sendByte(uart, (uint8_t) *str);
        if (status != UART_OK) {
            return status;
        }
        str++;
    }

    return UART_OK;
}

/*! @brief 发送二进制数据 (轮询, 可含 \0) */
UART_Error UART_sendBuffer(UART_Regs *uart, const uint8_t *buffer, uint32_t length)
{
    uint32_t i;
    UART_Error status;

    if ((UART_getSlotIndex(uart) < 0) || ((buffer == NULL) && (length > 0U))) {
        return UART_ERR_INVALID_PARAM;
    }

    for (i = 0U; i < length; i++) {
        status = UART_sendByte(uart, buffer[i]);
        if (status != UART_OK) {
            return status;
        }
    }

    return UART_OK;
}

/*! @brief 接收 1 字节 (轮询, timeout=0 无限等) */
UART_Error UART_receiveByte(UART_Regs *uart, uint32_t timeout, uint8_t *data)
{
    uint32_t loopsRemaining;

    if ((UART_getSlotIndex(uart) < 0) || (data == NULL)) {
        return UART_ERR_INVALID_PARAM;
    }

    while (!DL_UART_isRXFIFOEmpty(uart)) {
        (void) DL_UART_receiveData(uart);
    }

    if (timeout == 0U) {
        while (DL_UART_isRXFIFOEmpty(uart)) {
        }
        *data = DL_UART_receiveData(uart);
        return UART_OK;
    }

    if (timeout > (UINT32_MAX / UART_LOOPS_PER_MS)) {
        return UART_ERR_INVALID_PARAM;
    }
    loopsRemaining = timeout * UART_LOOPS_PER_MS;
    do {
        if (loopsRemaining == 0U) {
            return UART_ERR_TIMEOUT;
        }
        if (!DL_UART_isRXFIFOEmpty(uart)) {
            break;
        }
        loopsRemaining--;
    } while (1U);

    *data = DL_UART_receiveData(uart);
    return UART_OK;
}

/*! @brief 接收一行 (遇 \n/\r/满/超时停止) */
UART_Error UART_receiveString(UART_Regs *uart, char *buffer, uint32_t maxLength,
                              uint32_t timeout, uint32_t *count)
{
    uint8_t data;
    uint32_t totalLoops;
    uint32_t loopsRemaining;

    if ((UART_getSlotIndex(uart) < 0) || (buffer == NULL) || (count == NULL) || (maxLength == 0U)) {
        return UART_ERR_INVALID_PARAM;
    }

    if (timeout > (UINT32_MAX / UART_LOOPS_PER_MS)) {
        return UART_ERR_INVALID_PARAM;
    }

    while (!DL_UART_isRXFIFOEmpty(uart)) {
        (void) DL_UART_receiveData(uart);
    }

    *count = 0U;
    totalLoops = timeout * UART_LOOPS_PER_MS;
    loopsRemaining = totalLoops;

    while (*count < (maxLength - 1U)) {
        if (loopsRemaining == 0U) {
            break;
        }

        if (!DL_UART_isRXFIFOEmpty(uart)) {
            data = DL_UART_receiveData(uart);

            if (*count == 0 && (data == '\n' || data == '\r')) {
                continue;
            }

            buffer[*count] = (char) data;
            (*count)++;
            loopsRemaining = totalLoops;

            if ((data == '\n') || (data == '\r')) {
                break;
            }
        } else {
            loopsRemaining--;
        }
    }

    buffer[*count] = '\0';
    return UART_OK;
}

/*! @brief 检查 RX FIFO 是否有数据 */
bool UART_isDataAvailable(UART_Regs *uart)
{
    if (UART_getSlotIndex(uart) < 0) {
        return false;
    }

    return !DL_UART_isRXFIFOEmpty(uart);
}

/*! @brief 清空 RX FIFO (丢弃所有残留) */
UART_Error UART_clearRxBuffer(UART_Regs *uart)
{
    if (UART_getSlotIndex(uart) < 0) {
        return UART_ERR_INVALID_PARAM;
    }

    while (!DL_UART_isRXFIFOEmpty(uart)) {
        (void) DL_UART_receiveData(uart);
    }

    return UART_OK;
}

/*! @brief 格式化打印 (vsnprintf + sendBuffer, 缓冲 256B) */
int UART_printf(UART_Regs *uart, const char *format, ...)
{
    char buffer[256];
    va_list args;
    int length;
    uint32_t sendLength;

    if ((UART_getSlotIndex(uart) < 0) || (format == NULL)) {
        return -1;
    }

    va_start(args, format);
    length = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (length < 0) {
        return -1;
    }

    sendLength = (length >= (int) sizeof(buffer)) ? ((uint32_t) sizeof(buffer) - 1U) : (uint32_t) length;

    if (UART_sendBuffer(uart, (const uint8_t *) buffer, sendLength) != UART_OK) {
        return -1;
    }

    return (int) sendLength;
}

/*! @brief 注册 RX 中断回调 (NULL=禁用) */
UART_Error UART_setRxCallback(UART_Regs *uart, UART_RxCallback callback, void *userData)
{
    int32_t slotIndex = UART_getSlotIndex(uart);

    if (slotIndex < 0) {
        return UART_ERR_INVALID_PARAM;
    }

    g_uartSlots[slotIndex].rxCallback = callback;
    g_uartSlots[slotIndex].rxUserData = userData;

    if (callback != NULL) {
        DL_UART_Main_enableInterrupt(uart, DL_UART_MAIN_INTERRUPT_RX);
    } else {
        DL_UART_Main_disableInterrupt(uart, DL_UART_MAIN_INTERRUPT_RX);
    }

    return UART_OK;
}

/*! @brief 注册 TX 完成回调 (NULL=禁用, 触发后自动关中断) */
UART_Error UART_setTxCallback(UART_Regs *uart, UART_TxCallback callback, void *userData)
{
    int32_t slotIndex = UART_getSlotIndex(uart);

    if (slotIndex < 0) {
        return UART_ERR_INVALID_PARAM;
    }

    g_uartSlots[slotIndex].txCallback = callback;
    g_uartSlots[slotIndex].txUserData = userData;

    if (callback != NULL) {
        DL_UART_Main_enableInterrupt(uart, DL_UART_MAIN_INTERRUPT_TX);
    } else {
        DL_UART_Main_disableInterrupt(uart, DL_UART_MAIN_INTERRUPT_TX);
    }

    return UART_OK;
}

/*! @brief 设置波特率 (动态修改) */
UART_Error UART_setBaudRate(UART_Regs *uart, uint32_t baudRate)
{
    uint32_t inputClock;
    uint64_t divisorScaled;
    uint32_t integerDivisor;
    uint32_t fractionalDivisor;

    if ((UART_getSlotIndex(uart) < 0) || (baudRate == 0U)) {
        return UART_ERR_INVALID_PARAM;
    }

    inputClock = UART_getInputClock(uart);
    if (inputClock == 0U) {
        return UART_ERR_HARDWARE;
    }

    divisorScaled = ((((uint64_t) inputClock) * 4ULL) + ((uint64_t) baudRate / 2ULL)) /
        (uint64_t) baudRate;
    integerDivisor = (uint32_t) (divisorScaled / 64ULL);
    fractionalDivisor = (uint32_t) (divisorScaled % 64ULL);

    DL_UART_Main_setOversampling(uart, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_setBaudRateDivisor(uart, integerDivisor, fractionalDivisor);
    return UART_OK;
}

/*! @brief 获取当前波特率 */
UART_Error UART_getBaudRate(UART_Regs *uart, uint32_t *baudRate)
{
    uint32_t inputClock;
    uint32_t integerDivisor;
    uint32_t fractionalDivisor;
    uint32_t divisorScaled;

    if ((UART_getSlotIndex(uart) < 0) || (baudRate == NULL)) {
        return UART_ERR_INVALID_PARAM;
    }

    inputClock = UART_getInputClock(uart);
    if (inputClock == 0U) {
        return UART_ERR_HARDWARE;
    }

    integerDivisor = DL_UART_getIntegerBaudRateDivisor(uart);
    fractionalDivisor = DL_UART_getFractionalBaudRateDivisor(uart);
    divisorScaled = (integerDivisor * 64U) + fractionalDivisor;
    if (divisorScaled == 0U) {
        return UART_ERR_HARDWARE;
    }

    *baudRate = (uint32_t) ((((uint64_t) inputClock) * 4ULL + (divisorScaled / 2U)) /
        (uint64_t) divisorScaled);
    return UART_OK;
}

void UART0_IRQHandler(void)
{
    UART_dispatchInterrupt(UART_0_INST);
}

/*! @brief 初始化 DMA 模式 (RX 环形缓冲 + TX 单次 DMA) 
 *   - <1KB/s   → 256 字节
 *   - 1-10KB/s → 512 字节
 *   - >10KB/s  → 1024+ 字节
 *
 * 用法:
 * @code
 * uint8_t g_dmaRxBuf[512];
 *
 * void onDmaRx(uint32_t avail, void *userData) {
 *     uint8_t tmp[64];
 *     while (UART_DMA_available(UART_0_INST) > 0) {
 *         uint32_t len = UART_DMA_read(UART_0_INST, tmp, sizeof(tmp));
 *     }
 * }
 *
 * UART_init(UART_0_INST);
 * UART_DMA_init(UART_0_INST, g_dmaRxBuf, sizeof(g_dmaRxBuf),
 *               onDmaRx, NULL, NULL, NULL);
 * @endcode
 *
 * 注意事项:
 * - 必须在 UART_init() 之后调用
 * - RX 使用循环模式，TX 使用单次模式
 */
UART_Error UART_DMA_init(UART_Regs *uart, uint8_t *rxBuffer, uint32_t rxBufferSize,
                         UART_DmaRxCallback rxCallback, void *rxUserData,
                         UART_TxCallback txCallback, void *txUserData)
{
    UART_DmaContext *ctx;
    uint32_t mask;

    if ((uart == NULL) || (rxBuffer == NULL) || (rxBufferSize == 0U) ||
        (rxBufferSize > 0xFFFFU)) {
        return UART_ERR_INVALID_PARAM;
    }

    ctx = UART_getDmaContext(uart);
    if (ctx == NULL) {
        return UART_ERR_INVALID_PARAM;
    }

    ctx->rxBuffer = rxBuffer;
    ctx->rxSize = rxBufferSize;
    ctx->rxTail = 0U;
    ctx->rxOverflow = false;
    ctx->rxCallback = rxCallback;
    ctx->rxUserData = rxUserData;
    ctx->txCallback = txCallback;
    ctx->txUserData = txUserData;
    ctx->txBusy = false;
    ctx->initialized = true;

    DL_UART_Main_disableInterrupt(uart, DL_UART_MAIN_INTERRUPT_RX | DL_UART_MAIN_INTERRUPT_TX);
    DL_UART_Main_enableDMAReceiveEvent(uart, DL_UART_DMA_INTERRUPT_RX);
    DL_UART_Main_enableDMATransmitEvent(uart);

    mask = (1U << DMA_UART_RX_CHAN_ID) | (1U << DMA_UART_TX_CHAN_ID);
    DL_DMA_disableChannel(DMA, DMA_UART_RX_CHAN_ID);
    DL_DMA_disableChannel(DMA, DMA_UART_TX_CHAN_ID);
    DL_DMA_clearInterruptStatus(DMA, mask);
    DL_DMA_enableInterrupt(DMA, mask);

    DL_DMA_setTransferMode(DMA, DMA_UART_RX_CHAN_ID, DL_DMA_FULL_CH_REPEAT_BLOCK_TRANSFER_MODE);
    DL_DMA_setSrcAddr(DMA, DMA_UART_RX_CHAN_ID, (uint32_t) (&uart->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_UART_RX_CHAN_ID, (uint32_t) rxBuffer);
    DL_DMA_setTransferSize(DMA, DMA_UART_RX_CHAN_ID, (uint16_t) rxBufferSize);
    DL_DMA_setSrcIncrement(DMA, DMA_UART_RX_CHAN_ID, DL_DMA_ADDR_UNCHANGED);
    DL_DMA_setDestIncrement(DMA, DMA_UART_RX_CHAN_ID, DL_DMA_ADDR_INCREMENT);
    DL_DMA_enableChannel(DMA, DMA_UART_RX_CHAN_ID);

    DL_DMA_setTransferMode(DMA, DMA_UART_TX_CHAN_ID, DL_DMA_SINGLE_BLOCK_TRANSFER_MODE);
    DL_DMA_setSrcIncrement(DMA, DMA_UART_TX_CHAN_ID, DL_DMA_ADDR_INCREMENT);
    DL_DMA_setDestIncrement(DMA, DMA_UART_TX_CHAN_ID, DL_DMA_ADDR_UNCHANGED);

    NVIC_ClearPendingIRQ(DMA_INT_IRQn);
    NVIC_SetPriority(DMA_INT_IRQn, 0x02U);
    NVIC_EnableIRQ(DMA_INT_IRQn);

    return UART_OK;
}

/*! @brief DMA 发送数据 (非阻塞) */
UART_Error UART_DMA_send(UART_Regs *uart, const uint8_t *buffer, uint32_t length)
{
    UART_DmaContext *ctx;
    uint32_t mask;

    if ((buffer == NULL) || (length == 0U) || (length > 0xFFFFU)) {
        return UART_ERR_INVALID_PARAM;
    }

    ctx = UART_getDmaContext(uart);
    if ((ctx == NULL) || (!ctx->initialized)) {
        return UART_ERR_INVALID_PARAM;
    }

    if (ctx->txBusy) {
        return UART_ERR_BUSY;
    }

    ctx->txBusy = true;

    mask = (1U << DMA_UART_TX_CHAN_ID);
    DL_DMA_disableChannel(DMA, DMA_UART_TX_CHAN_ID);
    DL_DMA_clearInterruptStatus(DMA, mask);
    DL_DMA_setSrcAddr(DMA, DMA_UART_TX_CHAN_ID, (uint32_t) buffer);
    DL_DMA_setDestAddr(DMA, DMA_UART_TX_CHAN_ID, (uint32_t) (&uart->TXDATA));
    DL_DMA_setTransferSize(DMA, DMA_UART_TX_CHAN_ID, (uint16_t) length);
    DL_DMA_enableChannel(DMA, DMA_UART_TX_CHAN_ID);

    return UART_OK;
}

/*! @brief DMA 发送字符串 (非阻塞) */
UART_Error UART_DMA_sendString(UART_Regs *uart, const char *str)
{
    uint32_t length;

    if (str == NULL) {
        return UART_ERR_INVALID_PARAM;
    }

    length = (uint32_t) strlen(str);
    return UART_DMA_send(uart, (const uint8_t *) str, length);
}

/*! @brief DMA RX 可读字节数 */
uint32_t UART_DMA_available(UART_Regs *uart)
{
    UART_DmaContext *ctx;
    uint32_t head;
    uint32_t tail;

    ctx = UART_getDmaContext(uart);
    if ((ctx == NULL) || (!ctx->initialized) || (ctx->rxSize == 0U)) {
        return 0U;
    }

    head = UART_DMA_getHead(ctx);
    tail = ctx->rxTail;

    if (head >= tail) {
        return head - tail;
    }

    return (ctx->rxSize - tail + head);
}

/*! @brief 从 DMA 环形缓冲读取 (非阻塞) */
uint32_t UART_DMA_read(UART_Regs *uart, uint8_t *buffer, uint32_t length)
{
    UART_DmaContext *ctx;
    uint32_t head;
    uint32_t tail;
    uint32_t available;
    uint32_t toRead;
    uint32_t firstChunk;

    if ((buffer == NULL) || (length == 0U)) {
        return 0U;
    }

    ctx = UART_getDmaContext(uart);
    if ((ctx == NULL) || (!ctx->initialized) || (ctx->rxSize == 0U)) {
        return 0U;
    }

    head = UART_DMA_getHead(ctx);
    tail = ctx->rxTail;

    if (head >= tail) {
        available = head - tail;
    } else {
        available = ctx->rxSize - tail + head;
    }

    if (available == 0U) {
        return 0U;
    }

    toRead = (available < length) ? available : length;
    firstChunk = ctx->rxSize - tail;
    if (firstChunk > toRead) {
        firstChunk = toRead;
    }

    (void) memcpy(buffer, &ctx->rxBuffer[tail], firstChunk);
    if (toRead > firstChunk) {
        (void) memcpy(buffer + firstChunk, ctx->rxBuffer, toRead - firstChunk);
    }

    ctx->rxTail = (tail + toRead) % ctx->rxSize;
    if (ctx->rxOverflow) {
        ctx->rxOverflow = false;
    }

    return toRead;
}

/*! @brief DMA TX 是否忙 */
bool UART_DMA_isTxBusy(UART_Regs *uart)
{
    UART_DmaContext *ctx = UART_getDmaContext(uart);

    if ((ctx == NULL) || (!ctx->initialized)) {
        return false;
    }

    return ctx->txBusy;
}

/*! @brief DMA RX 是否溢出 */
bool UART_DMA_isRxOverflowed(UART_Regs *uart)
{
    UART_DmaContext *ctx = UART_getDmaContext(uart);

    if ((ctx == NULL) || (!ctx->initialized)) {
        return false;
    }

    return ctx->rxOverflow;
}

/*! @brief 清除 DMA RX 溢出标志 */
void UART_DMA_clearRxOverflow(UART_Regs *uart)
{
    UART_DmaContext *ctx = UART_getDmaContext(uart);

    if ((ctx == NULL) || (!ctx->initialized)) {
        return;
    }

    ctx->rxOverflow = false;
}

void UART_DMA_IRQHandler(void)
{
    UART_DmaContext *ctx = &g_uartDma;
    uint32_t mask = (1U << DMA_UART_RX_CHAN_ID) | (1U << DMA_UART_TX_CHAN_ID);
    uint32_t status;

    while ((status = DL_DMA_getRawInterruptStatus(DMA, mask)) != 0U) {
        DL_DMA_clearInterruptStatus(DMA, status);

        if ((status & (1U << DMA_UART_RX_CHAN_ID)) != 0U) {
            ctx->rxOverflow = true;
            if (ctx->rxCallback != NULL) {
                ctx->rxCallback(UART_DMA_available(ctx->uart), ctx->rxUserData);
            }
        }

        if ((status & (1U << DMA_UART_TX_CHAN_ID)) != 0U) {
            ctx->txBusy = false;
            if (ctx->txCallback != NULL) {
                ctx->txCallback(ctx->txUserData);
            }
        }
    }
}

static int32_t UART_getSlotIndex(UART_Regs *uart)
{
    uint32_t i;

    for (i = 0U; i < UART_CALLBACK_SLOT_COUNT; i++) {
        if (g_uartSlots[i].uart == uart) {
            return (int32_t) i;
        }
    }

    return -1;
}

static uint32_t UART_getInputClock(UART_Regs *uart)
{
    if (uart == UART_0_INST) {
        return UART_0_INST_FREQUENCY;
    }
    return 0U;
}

static UART_DmaContext *UART_getDmaContext(UART_Regs *uart)
{
    if (g_uartDma.uart == uart) {
        return &g_uartDma;
    }

    return NULL;
}

static uint32_t UART_DMA_getHead(const UART_DmaContext *ctx)
{
    uint32_t dest;
    uint32_t base;
    uint32_t offset;

    if ((ctx == NULL) || (ctx->rxBuffer == NULL) || (ctx->rxSize == 0U)) {
        return 0U;
    }

    dest = DL_DMA_getDestAddr(DMA, DMA_UART_RX_CHAN_ID);
    base = (uint32_t) ctx->rxBuffer;
    if (dest < base) {
        return 0U;
    }

    offset = dest - base;
    if (offset >= ctx->rxSize) {
        offset %= ctx->rxSize;
    }

    return offset;
}

static bool UART_waitTxReady(UART_Regs *uart)
{
    uint32_t loopsRemaining = UART_TX_TIMEOUT_MS * UART_LOOPS_PER_MS;

    while (DL_UART_isTXFIFOFull(uart)) {
        if (loopsRemaining == 0U) {
            return false;
        }
        loopsRemaining--;
    }

    return true;
}

static void UART_dispatchInterrupt(UART_Regs *uart)
{
    int32_t slotIndex = UART_getSlotIndex(uart);
    DL_UART_IIDX pending;

    if (slotIndex < 0) {
        return;
    }

    while ((pending = DL_UART_getPendingInterrupt(uart)) != DL_UART_IIDX_NO_INTERRUPT) {
        if (pending == DL_UART_IIDX_RX) {
            uint8_t data = DL_UART_receiveData(uart);
            if (g_uartSlots[slotIndex].rxCallback != NULL) {
                g_uartSlots[slotIndex].rxCallback(data, g_uartSlots[slotIndex].rxUserData);
            }
        } else if (pending == DL_UART_IIDX_TX) {
            if (g_uartSlots[slotIndex].txCallback != NULL) {
                g_uartSlots[slotIndex].txCallback(g_uartSlots[slotIndex].txUserData);
            }
            DL_UART_Main_disableInterrupt(uart, DL_UART_MAIN_INTERRUPT_TX);
        }
    }
}
