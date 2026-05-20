/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file       dl_uart.h
 *  @brief      UART 驱动库
 *  @defgroup   UART 通用异步收发器 (UART)
 *
 *  @anchor ti_dl_dl_uart_Overview
 *  # 概述
 *
 *  通用异步收发器驱动库允许对 MSPM0 UART 模块进行完整配置。
 *  此模块为 UART-Main 和 UART-Extend 提供通用功能，但开发人员应直接使用相应的 dl_uart_extend 或 dl_uart_main API。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup UART
 * @{
 */
#ifndef ti_dl_dl_uart__include
#define ti_dl_dl_uart__include

#if defined(ti_dl_dl_uart_main__include) || \
    defined(ti_dl_dl_uart_extend__include) || defined(DOXYGEN__INCLUDE)

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#if defined(__MSPM0_HAS_UART_MAIN__) || defined(__MSPM0_HAS_UART_EXTD__)

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_UART_INTERRUPT
 *  @{
 */
/*!
 * @brief 传输完成的 DMA 中断
 */
#define DL_UART_INTERRUPT_DMA_DONE_TX   (UART_CPU_INT_IMASK_DMA_DONE_TX_SET)

/*!
 * @brief 接收完成的 DMA 中断
 */
#define DL_UART_INTERRUPT_DMA_DONE_RX   (UART_CPU_INT_IMASK_DMA_DONE_RX_SET)

/*!
 * @brief 清除发送中断
 */
#define DL_UART_INTERRUPT_CTS_DONE              (UART_CPU_INT_IMASK_CTS_SET)

/*!
 * @brief 9 位模式地址匹配中断
 */
#define DL_UART_INTERRUPT_ADDRESS_MATCH  (UART_CPU_INT_IMASK_ADDR_MATCH_SET)

/*!
 * @brief LINC0 匹配中断
 */
#define DL_UART_INTERRUPT_LINC0_MATCH         (UART_CPU_INT_IMASK_LINC0_SET)

/*!
 * @brief 传输结束中断
 */
#define DL_UART_INTERRUPT_EOT_DONE              (UART_CPU_INT_IMASK_EOT_SET)

/*!
 * @brief UART 传输中断
 */
#define DL_UART_INTERRUPT_TX                  (UART_CPU_INT_IMASK_TXINT_SET)

/*!
 * @brief UART 接收中断
 */
#define DL_UART_INTERRUPT_RX                  (UART_CPU_INT_IMASK_RXINT_SET)

/*!
 * @brief LIN 硬件计数器溢出中断
 */
#define DL_UART_INTERRUPT_LIN_COUNTER_OVERFLOW                                \
                                             (UART_CPU_INT_IMASK_LINOVF_SET)

/*!
 * @brief LIN 上升沿 LINC1 中断
 */
#define DL_UART_INTERRUPT_LIN_RISING_EDGE                                     \
                                              (UART_CPU_INT_IMASK_LINC1_SET)

/*!
 * @brief LIN 下降沿 LINC0 中断
 */
#define DL_UART_INTERRUPT_LIN_FALLING_EDGE                                    \
                                              (UART_CPU_INT_IMASK_LINC0_SET)

/*!
 * @brief UARTxRXD 上的正边沿中断
 */
#define DL_UART_INTERRUPT_RXD_POS_EDGE         (UART_CPU_INT_IMASK_RXPE_SET)

/*!
 * @brief UARTxRXD 上的负边沿中断
 */
#define DL_UART_INTERRUPT_RXD_NEG_EDGE         (UART_CPU_INT_IMASK_RXNE_SET)

/*!
 * @brief 溢出错误中断
 */
#define DL_UART_INTERRUPT_OVERRUN_ERROR      (UART_CPU_INT_IMASK_OVRERR_SET)

/*!
 * @brief 中断错误中断
 */
#define DL_UART_INTERRUPT_BREAK_ERROR        (UART_CPU_INT_IMASK_BRKERR_SET)

/*!
 * @brief 奇偶校验错误中断
 */
#define DL_UART_INTERRUPT_PARITY_ERROR       (UART_CPU_INT_IMASK_PARERR_SET)

/*!
 * @brief 帧错误中断
 */
#define DL_UART_INTERRUPT_FRAMING_ERROR      (UART_CPU_INT_IMASK_FRMERR_SET)

/*!
 * @brief 接收超时中断
 */
#define DL_UART_INTERRUPT_RX_TIMEOUT_ERROR    (UART_CPU_INT_IMASK_RTOUT_SET)


/*!
 * @brief 噪声错误中断
 */
#define DL_UART_INTERRUPT_NOISE_ERROR          (UART_CPU_INT_IMASK_NERR_SET)


/** @}*/

/*! @enum DL_UART_IIDX */
typedef enum {
    /*! 传输完成的 DMA 的 UART 中断索引 */
    DL_UART_IIDX_DMA_DONE_TX = UART_CPU_INT_IIDX_STAT_DMA_DONE_TX,
    /*! 接收完成的 DMA 的 UART 中断索引 */
    DL_UART_IIDX_DMA_DONE_RX = UART_CPU_INT_IIDX_STAT_DMA_DONE_RX,
    /*! 清除发送的 UART 中断索引 */
    DL_UART_IIDX_CTS_DONE = UART_CPU_INT_IIDX_STAT_CTS,
    /*! 9 位模式地址匹配的 UART 中断索引 */
    DL_UART_IIDX_ADDRESS_MATCH = UART_CPU_INT_IIDX_STAT_MODE_9B,
    /*! 传输结束的 UART 中断索引 */
    DL_UART_IIDX_EOT_DONE = UART_CPU_INT_IIDX_STAT_EOT,
    /*! UART 传输的 UART 中断索引 */
    DL_UART_IIDX_TX = UART_CPU_INT_IIDX_STAT_TXIFG,
    /*! UART 接收的 UART 中断索引 */
    DL_UART_IIDX_RX = UART_CPU_INT_IIDX_STAT_RXIFG,
    /*! LIN 硬件计数器溢出的 UART 中断索引 */
    DL_UART_IIDX_LIN_COUNTER_OVERFLOW = UART_CPU_INT_IIDX_STAT_LINOVF,
    /*! LIN 上升沿 LINC1 的 UART 中断索引 */
    DL_UART_IIDX_LIN_RISING_EDGE = UART_CPU_INT_IIDX_STAT_LINC1,
    /*! LIN 下降沿 LINC0 的 UART 中断索引 */
    DL_UART_IIDX_LIN_FALLING_EDGE = UART_CPU_INT_IIDX_STAT_LINC0,
    /*! UARTxRXD 上正边沿的 UART 中断索引 */
    DL_UART_IIDX_RXD_POS_EDGE = UART_CPU_INT_IIDX_STAT_RXPE,
    /*! UARTxRXD 上负边沿的 UART 中断索引 */
    DL_UART_IIDX_RXD_NEG_EDGE = UART_CPU_INT_IIDX_STAT_RXNE,
    /*! 溢出错误的 UART 中断索引 */
    DL_UART_IIDX_OVERRUN_ERROR = UART_CPU_INT_IIDX_STAT_OEFG,
    /*! 中断错误的 UART 中断索引 */
    DL_UART_IIDX_BREAK_ERROR = UART_CPU_INT_IIDX_STAT_BEFG,
    /*! 奇偶校验错误的 UART 中断索引 */
    DL_UART_IIDX_PARITY_ERROR = UART_CPU_INT_IIDX_STAT_PEFG,
    /*! 帧错误的 UART 中断索引 */
    DL_UART_IIDX_FRAMING_ERROR = UART_CPU_INT_IIDX_STAT_FEFG,
    /*! 接收超时的 UART 中断索引 */
    DL_UART_IIDX_RX_TIMEOUT_ERROR = UART_CPU_INT_IIDX_STAT_RTFG,


    /*! 噪声错误的 UART 中断索引 */
    DL_UART_IIDX_NOISE_ERROR = UART_CPU_INT_IIDX_STAT_NERR_EVT,


    /*! 无中断的 UART 中断索引 */
    DL_UART_IIDX_NO_INTERRUPT = UART_CPU_INT_IIDX_STAT_NO_INTR
} DL_UART_IIDX;

/*! @enum DL_UART_DMA_IIDX_RX */
typedef enum {
    /*! 用于启用 UART 接收作为 DMA 触发的 UART 中断索引 */
    DL_UART_DMA_IIDX_RX_TRIGGER = UART_DMA_TRIG_RX_IIDX_STAT_RXIFG,
    /*! 用于启用 UART 接收超时作为 DMA 触发的 UART 中断索引 */
    DL_UART_DMA_IIDX_RX_TIMEOUT_TRIGGER = UART_DMA_TRIG_RX_IIDX_STAT_RTFG
} DL_UART_DMA_IIDX_RX;

/*! @enum DL_UART_DMA_IIDX_TX */
typedef enum {
    /*! 用于启用 UART 传输作为 DMA 触发的 UART 中断索引 */
    DL_UART_DMA_IIDX_TX_TRIGGER = UART_DMA_TRIG_TX_IIDX_STAT_TXIFG
} DL_UART_DMA_IIDX_TX;

/** @addtogroup DL_UART_DMA_INTERRUPT_RX
 *  @{
 */
/*!
 * @brief 用于启用 UART 接收作为 DMA 触发的 UART 中断
 */
#define DL_UART_DMA_INTERRUPT_RX              (UART_DMA_TRIG_RX_IMASK_RXINT_SET)

/*!
 * @brief 指示 RX 的 DMA 完成的 UART 中断
 */
#define DL_UART_DMA_DONE_INTERRUPT_RX         (UART_CPU_INT_IMASK_DMA_DONE_RX_SET)

/*!
 * @brief 用于启用 UART 接收超时作为 DMA 触发的 UART 中断
 */
#define DL_UART_DMA_INTERRUPT_RX_TIMEOUT      (UART_DMA_TRIG_RX_IMASK_RTOUT_SET)

/** @}*/

/*!
 * @brief 用于启用 UART 传输作为 DMA 触发的 UART 中断
 */
#define DL_UART_DMA_INTERRUPT_TX              (UART_DMA_TRIG_TX_IMASK_TXINT_SET)

/*!
 * @brief 指示 TX 的 DMA 完成的 UART 中断
 */
#define DL_UART_DMA_DONE_INTERRUPT_TX         (UART_CPU_INT_IMASK_DMA_DONE_TX_SET)

/** @addtogroup DL_UART_ERROR
 *  @{
 */
/*!
 * @brief 发生溢出错误
 */
#define DL_UART_ERROR_OVERRUN                          (UART_RXDATA_OVRERR_SET)

/*!
 * @brief 发生中断错误
 */
#define DL_UART_ERROR_BREAK                            (UART_RXDATA_BRKERR_SET)

/*!
 * @brief 发生奇偶校验错误
 */
#define DL_UART_ERROR_PARITY                           (UART_RXDATA_PARERR_SET)

/*!
 * @brief 发生帧错误
 */
#define DL_UART_ERROR_FRAMING                          (UART_RXDATA_FRMERR_SET)

/** @}*/

/*! @enum DL_UART_PULSE_WIDTH */
typedef enum {
    /*! 长度短于 5ns 的脉冲被过滤 */
    DL_UART_PULSE_WIDTH_5_NS = UART_GFCTL_AGFSEL_AGLIT_5,
    /*! 长度短于 10ns 的脉冲被过滤 */
    DL_UART_PULSE_WIDTH_10_NS = UART_GFCTL_AGFSEL_AGLIT_10,
    /*! 长度短于 25ns 的脉冲被过滤 */
    DL_UART_PULSE_WIDTH_25_NS = UART_GFCTL_AGFSEL_AGLIT_25,
    /*! 长度短于 50ns 的脉冲被过滤 */
    DL_UART_PULSE_WIDTH_50_NS = UART_GFCTL_AGFSEL_AGLIT_50
} DL_UART_PULSE_WIDTH;

/*! @enum DL_UART_OVERSAMPLING_RATE */
typedef enum {
    /*! 将过采样率设置为 16x */
    DL_UART_OVERSAMPLING_RATE_16X = UART_CTL0_HSE_OVS16,
    /*! 将过采样率设置为 8x */
    DL_UART_OVERSAMPLING_RATE_8X = UART_CTL0_HSE_OVS8,
    /*! 将过采样率设置为 3x。
     * 当启用 3x 过采样时，不支持 IrDA、Manchester 和 DALI。 */
    DL_UART_OVERSAMPLING_RATE_3X = UART_CTL0_HSE_OVS3
} DL_UART_OVERSAMPLING_RATE;

/*! @enum DL_UART_PARITY */
typedef enum {
    /*! 启用偶校验生成，检查 1 的数量是否为偶数 */
    DL_UART_PARITY_EVEN = (UART_LCRH_PEN_ENABLE | UART_LCRH_EPS_EVEN),
    /*! 启用奇校验生成，检查 1 的数量是否为奇数 */
    DL_UART_PARITY_ODD = (UART_LCRH_PEN_ENABLE | UART_LCRH_EPS_ODD),
    /*! 启用带有奇偶校验位 '1' 的粘性校验
     * 启用时，传输时将永久的 '1' 设为校验位，接收数据时检查是否为 '1'。 */
    DL_UART_PARITY_STICK_ONE = (UART_LCRH_PEN_ENABLE | UART_LCRH_SPS_ENABLE | UART_LCRH_EPS_ODD),
    /*! 启用带有奇偶校验位 '0' 的粘性校验
     * 启用时，传输时将永久的 '0' 设为校验位，接收数据时检查是否为 '0'。 */
    DL_UART_PARITY_STICK_ZERO = (UART_LCRH_PEN_ENABLE | UART_LCRH_SPS_ENABLE | UART_LCRH_EPS_EVEN),
    /*! 禁用校验检查和生成 */
    DL_UART_PARITY_NONE = UART_LCRH_PEN_DISABLE
} DL_UART_PARITY;

/*! @enum DL_UART_WORD_LENGTH */
typedef enum {
    /*! 字长为 5 位 */
    DL_UART_WORD_LENGTH_5_BITS = UART_LCRH_WLEN_DATABIT5,
    /*! 字长为 6 位 */
    DL_UART_WORD_LENGTH_6_BITS = UART_LCRH_WLEN_DATABIT6,
    /*! 字长为 7 位 */
    DL_UART_WORD_LENGTH_7_BITS = UART_LCRH_WLEN_DATABIT7,
    /*! 字长为 8 位 */
    DL_UART_WORD_LENGTH_8_BITS = UART_LCRH_WLEN_DATABIT8
} DL_UART_WORD_LENGTH;

/*! @enum DL_UART_MODE */
typedef enum {
    /*! 正常操作 */
    DL_UART_MODE_NORMAL = UART_CTL0_MODE_UART,
    /*! 以 RS485 模式操作 */
    DL_UART_MODE_RS485 = UART_CTL0_MODE_RS485,
    /*! 以空闲线模式操作 */
    DL_UART_MODE_IDLE_LINE = UART_CTL0_MODE_IDLELINE,
    /*! 以 9 位地址模式操作 */
    DL_UART_MODE_ADDR_9_BIT = UART_CTL0_MODE_ADDR9BIT,
    /*! 以 ISO7816 智能卡支持模式操作 */
    DL_UART_MODE_SMART_CARD = UART_CTL0_MODE_SMART,
    /*! 以 DALI 模式操作 */
    DL_UART_MODE_DALI = UART_CTL0_MODE_DALI
} DL_UART_MODE;

/*! @enum DL_UART_DIRECTION */
typedef enum {
    /*! 启用 UART 发送器 */
    DL_UART_DIRECTION_TX = UART_CTL0_TXE_ENABLE,
    /*! 启用 UART 接收器 */
    DL_UART_DIRECTION_RX = UART_CTL0_RXE_ENABLE,
    /*! 启用 UART 发送器和接收器 */
    DL_UART_DIRECTION_TX_RX = (UART_CTL0_RXE_ENABLE | UART_CTL0_TXE_ENABLE),
    /*! 禁用 UART 发送器和接收器 */
    DL_UART_DIRECTION_NONE = (UART_CTL0_RXE_DISABLE | UART_CTL0_TXE_DISABLE)
} DL_UART_DIRECTION;

/*! @enum DL_UART_CLOCK */
typedef enum {
    /*! 选择 BUSCLK 作为时钟源 */
    DL_UART_CLOCK_BUSCLK = UART_CLKSEL_BUSCLK_SEL_ENABLE,
    /*! 选择 MFCLK 作为时钟源 */
    DL_UART_CLOCK_MFCLK = UART_CLKSEL_MFCLK_SEL_ENABLE,
    /*! 选择 LFCLK 作为时钟源 */
    DL_UART_CLOCK_LFCLK = UART_CLKSEL_LFCLK_SEL_ENABLE
} DL_UART_CLOCK;

/*! @enum DL_UART_FLOW_CONTROL */
typedef enum {
    /*! 启用请求发送 */
    DL_UART_FLOW_CONTROL_RTS = UART_CTL0_RTSEN_ENABLE,
    /*! 启用清除发送 */
    DL_UART_FLOW_CONTROL_CTS = UART_CTL0_CTSEN_ENABLE,
    /*! 启用请求发送和清除发送 */
    DL_UART_FLOW_CONTROL_RTS_CTS = (UART_CTL0_RTSEN_ENABLE | UART_CTL0_CTSEN_ENABLE),
    /*! 禁用流控制 */
    DL_UART_FLOW_CONTROL_NONE = (UART_CTL0_CTSEN_DISABLE | UART_CTL0_RTSEN_DISABLE)
} DL_UART_FLOW_CONTROL;

/*! @enum DL_UART_RTS */
typedef enum {
    /*! RTS 被断言，表示 RX FIFO 中的数据低于阈值 */
    DL_UART_RTS_ASSERT = UART_CTL0_RTS_SET,
    /*! RTS 被取消断言，表示 RX FIFO 中的数据达到或超过阈值 */
    DL_UART_RTS_DEASSERT = UART_CTL0_RTS_CLR
} DL_UART_RTS;

/*! @enum DL_UART_STOP_BITS */
typedef enum {
    /*! 在帧末尾传输一个停止位 */
    DL_UART_STOP_BITS_ONE = UART_LCRH_STP2_DISABLE,
    /*! 在帧末尾传输两个停止位 */
    DL_UART_STOP_BITS_TWO = UART_LCRH_STP2_ENABLE
} DL_UART_STOP_BITS;

/*! @enum DL_UART_TXD_OUT */
typedef enum {
    /*! TXD 引脚为低电平 */
    DL_UART_TXD_OUT_LOW = UART_CTL0_TXD_OUT_LOW,
    /*! TXD 引脚为高电平 */
    DL_UART_TXD_OUT_HIGH = UART_CTL0_TXD_OUT_HIGH
} DL_UART_TXD_OUT;

/*! @enum DL_UART_TX_FIFO_LEVEL */
typedef enum {
    /*! 当 FIFO <= 3/4 空时触发中断 */
    DL_UART_TX_FIFO_LEVEL_3_4_EMPTY = UART_IFLS_TXIFLSEL_LVL_3_4,
    /*! 当 FIFO <= 1/2 空时触发中断 */
    DL_UART_TX_FIFO_LEVEL_1_2_EMPTY = UART_IFLS_TXIFLSEL_LVL_1_2,
    /*! 当 FIFO <= 1/4 空时触发中断 */
    DL_UART_TX_FIFO_LEVEL_1_4_EMPTY = UART_IFLS_TXIFLSEL_LVL_1_4,
    /*! 当 FIFO 为空时触发中断 */
    DL_UART_TX_FIFO_LEVEL_EMPTY = UART_IFLS_TXIFLSEL_LVL_EMPTY,
    /*! 当 FIFO >= 1 个条目时触发中断 */
    DL_UART_TX_FIFO_LEVEL_ONE_ENTRY = UART_IFLS_TXIFLSEL_LVL_1
} DL_UART_TX_FIFO_LEVEL;

/*! @enum DL_UART_RX_FIFO_LEVEL */
typedef enum {
    /*! 当 FIFO >= 1 个可用条目时触发中断。DMA 触发所需 */
    DL_UART_RX_FIFO_LEVEL_ONE_ENTRY = UART_IFLS_RXIFLSEL_LVL_1,
    /*! 当 FIFO 已满时触发中断 */
    DL_UART_RX_FIFO_LEVEL_FULL = UART_IFLS_RXIFLSEL_LVL_FULL,
    /*! 当 FIFO >= 3/4 满时触发中断 */
    DL_UART_RX_FIFO_LEVEL_3_4_FULL = UART_IFLS_RXIFLSEL_LVL_3_4,
    /*! 当 FIFO >= 1/2 满时触发中断 */
    DL_UART_RX_FIFO_LEVEL_1_2_FULL = UART_IFLS_RXIFLSEL_LVL_1_2,
    /*! 当 FIFO >= 1/4 满时触发中断 */
    DL_UART_RX_FIFO_LEVEL_1_4_FULL = UART_IFLS_RXIFLSEL_LVL_1_4,
} DL_UART_RX_FIFO_LEVEL;

/*! @enum DL_UART_IRDA_CLOCK */
typedef enum {
    /*! IrDA 编码数据基于波特率时钟 */
    DL_UART_IRDA_CLOCK_BAUD_RATE = UART_IRCTL_IRTXCLK_BRCLK,
    /*! IrDA 编码数据基于功能时钟 */
    DL_UART_IRDA_CLOCK_FUNCTIONAL = UART_IRCTL_IRTXCLK_BITCLK
} DL_UART_IRDA_CLOCK;

/*! @enum DL_UART_IRDA_POLARITY */
typedef enum {
    /*! 当检测到光脉冲时，IrDA 收发器输出低脉冲 */
    DL_UART_IRDA_POLARITY_LOW = UART_IRCTL_IRRXPL_LOW,
    /*! 当检测到光脉冲时，IrDA 收发器输出高脉冲 */
    DL_UART_IRDA_POLARITY_HIGH = UART_IRCTL_IRRXPL_HIGH
} DL_UART_IRDA_POLARITY;

/*!
 * @brief 当使用 BITCLK16 时，将 IrDA 脉冲宽度设置为 3/16 位周期
 */
#define DL_UART_PULSE_WIDTH_3_16_BIT_PERIOD           ((uint32_t) 0x00000000U)

/*! @enum DL_UART_CLOCK_DIVIDE_RATIO */
typedef enum {
    /*! UART 源时钟分频比设置为 1 */
    DL_UART_CLOCK_DIVIDE_RATIO_1 = UART_CLKDIV_RATIO_DIV_BY_1,
    /*! UART 源时钟分频比设置为 2 */
    DL_UART_CLOCK_DIVIDE_RATIO_2 = UART_CLKDIV_RATIO_DIV_BY_2,
    /*! UART 源时钟分频比设置为 3 */
    DL_UART_CLOCK_DIVIDE_RATIO_3 = UART_CLKDIV_RATIO_DIV_BY_3,
    /*! UART 源时钟分频比设置为 4 */
    DL_UART_CLOCK_DIVIDE_RATIO_4 = UART_CLKDIV_RATIO_DIV_BY_4,
    /*! UART 源时钟分频比设置为 5 */
    DL_UART_CLOCK_DIVIDE_RATIO_5 = UART_CLKDIV_RATIO_DIV_BY_5,
    /*! UART 源时钟分频比设置为 6 */
    DL_UART_CLOCK_DIVIDE_RATIO_6 = UART_CLKDIV_RATIO_DIV_BY_6,
    /*! UART 源时钟分频比设置为 7 */
    DL_UART_CLOCK_DIVIDE_RATIO_7 = UART_CLKDIV_RATIO_DIV_BY_7,
    /*! UART 源时钟分频比设置为 8 */
    DL_UART_CLOCK_DIVIDE_RATIO_8 = UART_CLKDIV_RATIO_DIV_BY_8
} DL_UART_CLOCK_DIVIDE_RATIO;

/*! @enum DL_UART_CLOCK_DIVIDE2_RATIO */
typedef enum {
    /*! UART 源时钟分频比 2 设置为 1 */
    DL_UART_CLOCK_DIVIDE2_RATIO_1 = UART_CLKDIV2_RATIO_DIV_BY_1,
    /*! UART 源时钟分频比 2 设置为 2 */
    DL_UART_CLOCK_DIVIDE2_RATIO_2 = UART_CLKDIV2_RATIO_DIV_BY_2,
    /*! UART 源时钟分频比 2 设置为 3 */
    DL_UART_CLOCK_DIVIDE2_RATIO_3 = UART_CLKDIV2_RATIO_DIV_BY_3,
    /*! UART 源时钟分频比 2 设置为 4 */
    DL_UART_CLOCK_DIVIDE2_RATIO_4 = UART_CLKDIV2_RATIO_DIV_BY_4,
    /*! UART 源时钟分频比 2 设置为 5 */
    DL_UART_CLOCK_DIVIDE2_RATIO_5 = UART_CLKDIV2_RATIO_DIV_BY_5,
    /*! UART 源时钟分频比 2 设置为 6 */
    DL_UART_CLOCK_DIVIDE2_RATIO_6 = UART_CLKDIV2_RATIO_DIV_BY_6,
    /*! UART 源时钟分频比 2 设置为 7 */
    DL_UART_CLOCK_DIVIDE2_RATIO_7 = UART_CLKDIV2_RATIO_DIV_BY_7,
    /*! UART 源时钟分频比 2 设置为 8 */
    DL_UART_CLOCK_DIVIDE2_RATIO_8 = UART_CLKDIV2_RATIO_DIV_BY_8
} DL_UART_CLOCK_DIVIDE2_RATIO;

/* clang-format on */

/*!
 *  @brief  @ref DL_UART_init 的配置结构体
 */
typedef struct {
    /*! 使用的通信模式和协议。@ref DL_UART_MODE 之一 */
    DL_UART_MODE mode;

    /*! 通信方向。@ref DL_UART_DIRECTION 之一。 */
    DL_UART_DIRECTION direction;

    /*! 流控制配置。@ref DL_UART_FLOW_CONTROL 之一 */
    DL_UART_FLOW_CONTROL flowControl;

    /*! 奇偶校验配置。@ref DL_UART_PARITY 之一 */
    DL_UART_PARITY parity;

    /*! 数据传输大小。@ref DL_UART_WORD_LENGTH 之一 */
    DL_UART_WORD_LENGTH wordLength;

    /*! @ref DL_UART_STOP_BITS 之一  */
    DL_UART_STOP_BITS stopBits;

} DL_UART_Config;

/*!
 *  @brief  @ref DL_UART_setClockConfig 的配置结构体。
 */
typedef struct {
    /*! 选择 UART 模块时钟源 @ref DL_UART_CLOCK */
    DL_UART_CLOCK clockSel;

    /*! 选择分频比。@ref DL_UART_CLOCK_DIVIDE_RATIO 之一 */
    DL_UART_CLOCK_DIVIDE_RATIO divideRatio;

} DL_UART_ClockConfig;

#ifdef __MSPM0_HAS_UART_MAIN__

/**
 * @brief 在进入 STOP/STANDBY 模式前备份 UART Main 外设状态的配置结构。由
 *        @ref DL_UART_Main_saveConfiguration 和
 *        @ref DL_UART_Main_restoreConfiguration 使用
 */
typedef struct {
    /*! 基本 UART 控制配置的组合，它们被压缩为一个字，因为它们存储在 UART
     *  寄存器中。有关外设控制字的创建方式，请参阅 @ref DL_UART_init。 */
    uint32_t controlWord;

    /*! UART 模块时钟源。@ref DL_UART_CLOCK 之一 */
    uint32_t clockSel;

    /*! UART 时钟分频器。@ref DL_UART_CLOCK_DIVIDE_RATIO 之一 */
    uint32_t divideRatio;

    /*! UART 中断 FIFO 级别选择配置的组合，它们被压缩为一个字，因为它们存储在 UART
     * 寄存器中。 */
    uint32_t interruptFifoLevelSelectWord;

    /*! UART 整数波特率除数。值在 0 - 65535 之间。 */
    uint32_t ibrd;

    /*! UART 小数波特率除数。值在 0 - 63 之间。 */
    uint32_t fbrd;

    /*! UART 线路控制寄存器配置的组合，它们被压缩为一个字，因为它们存储在 UART
     *  寄存器中。有关外设控制字的创建方式，请参阅 @ref DL_UART_init。 */
    uint32_t lineControlRegisterWord;

    /*! UART  glitch 过滤器配置的组合，它们被压缩为一个字，因为它们存储在 UART
     *  寄存器中。 */
    uint32_t glitchFilterControlWord;

    /*! EVENT0 的 UART 中断掩码。
     * @ref DL_UART_INTERRUPT 的按位或 */
    uint32_t interruptMask0;

    /*! EVENT1 的 UART 中断掩码。
     * @ref DL_UART_DMA_INTERRUPT_RX 的按位或 */
    uint32_t interruptMask1;

    /*! EVENT2 的 UART 中断掩码。
     * @ref DL_UART_DMA_INTERRUPT_TX 的按位或 */
    uint32_t interruptMask2;

    /*! 指示是否存在有效配置结构的布尔标志。用户不应修改。 */
    bool backupRdy;
} DL_UART_Main_backupConfig;

#endif /* __MSPM0_HAS_UART_MAIN__ */

#ifdef __MSPM0_HAS_UART_EXTD__

/**
 * @brief 在进入 STOP/STANDBY 模式前备份 UART Extend 外设状态的配置结构。由
 *        @ref DL_UART_Extend_saveConfiguration 和
 *        @ref DL_UART_Extend_restoreConfiguration 使用
 */
typedef struct {
    /*! 基本 UART 控制配置的组合，它们被压缩为一个字，因为它们存储在 UART
     *  寄存器中。有关外设控制字的创建方式，请参阅 @ref DL_UART_init。 */
    uint32_t controlWord;

    /*! UART 模块时钟源。@ref DL_UART_CLOCK 之一 */
    uint32_t clockSel;

    /*! UART 时钟分频器。@ref DL_UART_CLOCK_DIVIDE_RATIO 之一 */
    uint32_t divideRatio;

    /*! UART 线路控制寄存器配置的组合，它们被压缩为一个字，因为它们存储在 UART
     *  寄存器中。有关外设控制字的创建方式，请参阅 @ref DL_UART_init。 */
    uint32_t lineControlRegisterWord;

    /*! UART 中断 FIFO 级别选择配置的组合，它们被压缩为一个字，因为它们存储在 UART
     * 寄存器中。 */
    uint32_t interruptFifoLevelSelectWord;

    /*! UART 整数波特率除数。值在 0 - 65535 之间。 */
    uint32_t ibrd;

    /*! UART 小数波特率除数。值在 0 - 63 之间。 */
    uint32_t fbrd;

    /*! UART  glitch 过滤器配置的组合，它们被压缩为一个字，因为它们存储在 UART
     *  寄存器中。 */
    uint32_t glitchFilterControlWord;

    /*! UART LIN 基本配置的组合，它们被压缩为一个字，因为它们存储在 UART
     *  寄存器中。 */
    uint32_t linControlWord;

    /*! UART IrDA 基本配置的组合，它们被压缩为一个字，因为它们存储在 UART
     *  寄存器中。 */
    uint32_t irdaControlWord;

    /*! 9 位或空闲模式的 UART 地址掩码。 */
    uint32_t addressMask;

    /*! 9 位或空闲模式应匹配的 UART 地址。 */
    uint32_t address;

    /*! EVENT0 的 UART 中断状态。
     * @ref DL_UART_INTERRUPT 的按位或 */
    uint32_t interruptMask0;

    /*! EVENT1 的 UART 中断状态。
     * @ref DL_UART_DMA_INTERRUPT_RX 的按位或 */
    uint32_t interruptMask1;

    /*! EVENT2 的 UART 中断状态。
     * @ref DL_UART_DMA_INTERRUPT_TX 的按位或 */
    uint32_t interruptMask2;

    /*! 指示是否存在有效配置结构的布尔标志。用户不应修改。 */
    bool backupRdy;
} DL_UART_Extend_backupConfig;

#endif /* __MSPM0_HAS_UART_EXTD__ */

/**
 *  @brief      初始化 UART 外设
 *
 *  初始化 UART 外设的所有通用可配置选项。任何其他自定义配置都可以在调用此 API 后完成。
 *  此 API 中不启用 UART。
 *
 *  @param[in]  uart    指向外设寄存器覆盖的指针
 *  @param[in]  config  UART 外设的配置
 */
void DL_UART_init(UART_Regs *uart, const DL_UART_Config *config);

/**
 * @brief 启用 UART 的外设写入使能 (PWREN) 寄存器
 *
 *  在软件可以配置任何外设寄存器之前，必须通过将 ENABLE 位与适当的 KEY 值一起写入
 *  外设的 PWREN 寄存器来启用外设本身。
 *
 *  @note 有关省电，请参阅 @ref DL_UART_enable
 *
 * @param uart        指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_enablePower(UART_Regs *uart)
{
    uart->GPRCM.PWREN = (UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_ENABLE);
}

/**
 * @brief 禁用 UART 的外设写入使能 (PWREN) 寄存器
 *
 *  当 PWREN.ENABLE 位被清除时，外设的寄存器不可用于读/写操作。
 *
 *  @note 此 API 不提供大量省电。有关省电，请参阅 @ref DL_UART_enable
 *
 * @param uart        指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_disablePower(UART_Regs *uart)
{
    uart->GPRCM.PWREN = (UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_DISABLE);
}

/**
 * @brief 返回 UART 的外设写入使能 (PWREN) 寄存器是否已启用
 *
 *  在软件可以配置任何外设寄存器之前，必须通过将 ENABLE 位与适当的 KEY 值一起写入
 *  外设的 PWREN 寄存器来启用外设本身。
 *
 *  当 PWREN.ENABLE 位被清除时，外设的寄存器不可用于读/写操作。
 *
 * @param uart        指向外设寄存器覆盖的指针
 *
 * @return 如果外设寄存器访问已启用，则为 true
 * @return 如果外设寄存器访问已禁用，则为 false
 */
__STATIC_INLINE bool DL_UART_isPowerEnabled(const UART_Regs *uart)
{
    return ((uart->GPRCM.PWREN & UART_PWREN_ENABLE_MASK) ==
            UART_PWREN_ENABLE_ENABLE);
}

/**
 * @brief 重置 UART 外设
 *
 * @param uart        指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_reset(UART_Regs *uart)
{
    uart->GPRCM.RSTCTL =
        (UART_RSTCTL_KEY_UNLOCK_W | UART_RSTCTL_RESETSTKYCLR_CLR |
            UART_RSTCTL_RESETASSERT_ASSERT);
}

/**
 * @brief 返回 UART 外设是否已重置
 *
 * @param uart        指向外设寄存器覆盖的指针
 *
 * @return 如果外设已重置，则为 true
 * @return 如果外设未重置，则为 false
 *
 */
__STATIC_INLINE bool DL_UART_isReset(const UART_Regs *uart)
{
    return ((uart->GPRCM.STAT & UART_GPRCM_STAT_RESETSTKY_MASK) ==
            UART_GPRCM_STAT_RESETSTKY_RESET);
}

/**
 *  @brief      启用 UART 外设
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_enable(UART_Regs *uart)
{
    uart->CTL0 |= UART_CTL0_ENABLE_ENABLE;
}

/**
 *  @brief      检查 UART 外设是否已启用
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     返回 UART 的启用状态
 *
 *  @retval     true  UART 外设已启用
 *  @retval     false UART 外设已禁用

 */
__STATIC_INLINE bool DL_UART_isEnabled(const UART_Regs *uart)
{
    return ((uart->CTL0 & UART_CTL0_ENABLE_MASK) == UART_CTL0_ENABLE_ENABLE);
}

/**
 *  @brief      禁用 UART 外设
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_disable(UART_Regs *uart)
{
    uart->CTL0 &= ~(UART_CTL0_ENABLE_MASK);
}

/**
 *  @brief      配置 UART 源时钟
 *
 *  @param[in]  uart    指向外设寄存器覆盖的指针
 *  @param[in]  config  指向时钟配置结构体的指针
 *                       @ref DL_UART_ClockConfig.
 */
void DL_UART_setClockConfig(
    UART_Regs *uart, const DL_UART_ClockConfig *config);

/**
 *  @brief      获取 UART 源时钟配置
 *
 *  @param[in]  uart    指向外设寄存器覆盖的指针
 *  @param[in]  config  指向时钟配置结构体的指针
 *                      @ref DL_UART_ClockConfig.
 */
void DL_UART_getClockConfig(
    const UART_Regs *uart, DL_UART_ClockConfig *config);

/**
 *  @brief      配置波特率
 *
 *  给定目标波特率和 UART 时钟源的频率，此 API 确定并设置推荐的过采样设置，然后
 *  计算并设置所需的波特率除数。
 *
 *  将设置的过采样率是给定目标波特率和 UART 时钟频率下可能的最高过采样率。
 *
 *  如果用户希望避免让 CPU 计算波特率除数或不使用推荐的计算值，用户可以直接调用
 *  @ref DL_UART_setOversampling 和 @ref DL_UART_setBaudRateDivisor。
 *
 *  @param[in]  uart       指向外设寄存器覆盖的指针
 *  @param[in]  clockFreq  UART 时钟源的时钟频率（以 Hz 为单位）
 *  @param[in]  baudRate   目标波特率
 *
 * @sa          DL_UART_setOversampling
 * @sa          DL_UART_setBaudRateDivisor
 */
void DL_UART_configBaudRate(
    UART_Regs *uart, uint32_t clockFreq, uint32_t baudRate);

/**
 *  @brief      设置过采样率
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *  @param[in]  rate  要使用的过采样率。
 *                    @ref DL_UART_OVERSAMPLING_RATE 之一
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 */
__STATIC_INLINE void DL_UART_setOversampling(
    UART_Regs *uart, DL_UART_OVERSAMPLING_RATE rate)
{
    DL_Common_updateReg(&uart->CTL0, (uint32_t) rate, UART_CTL0_HSE_MASK);
}

/**
 *  @brief      获取过采样率
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     当前过采样率
 *
 *  @retval     @ref DL_UART_OVERSAMPLING_RATE 之一
 *
 */
__STATIC_INLINE DL_UART_OVERSAMPLING_RATE DL_UART_getOversampling(
    const UART_Regs *uart)
{
    uint32_t rate = uart->CTL0 & UART_CTL0_HSE_MASK;

    return (DL_UART_OVERSAMPLING_RATE)(rate);
}

/**
 *  @brief      启用回环模式
 *
 *  启用回环模式。启用后，UARTxTXD 路径通过 UARTxRXD 路径馈送。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 */
__STATIC_INLINE void DL_UART_enableLoopbackMode(UART_Regs *uart)
{
    uart->CTL0 |= UART_CTL0_LBE_ENABLE;
}

/**
 *  @brief      检查回环模式是否已启用
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     回环模式的状态
 *
 *  @retval     true  回环模式已启用
 *  @retval     false 回环模式已禁用
 */
__STATIC_INLINE bool DL_UART_isLoopbackModeEnabled(const UART_Regs *uart)
{
    return ((uart->CTL0 & UART_CTL0_LBE_MASK) == UART_CTL0_LBE_ENABLE);
}

/**
 *  @brief      禁用回环模式
 *
 *  禁用回环模式。禁用后，UARTxTXD 路径不通过 UARTxRXD 路径馈送。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 */
__STATIC_INLINE void DL_UART_disableLoopbackMode(UART_Regs *uart)
{
    uart->CTL0 &= ~(UART_CTL0_LBE_MASK);
}

/**
 *  @brief      设置 UART 通信的方向
 *
 *  如果 UART 在传输或接收过程中被禁用，它会在停止前完成当前字符。
 *
 *  @param[in]  uart       指向外设寄存器覆盖的指针
 *  @param[in]  direction  要设置 UART 通信的方向。
 *                         @ref DL_UART_DIRECTION 之一。
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 *
 */
__STATIC_INLINE void DL_UART_setDirection(
    UART_Regs *uart, DL_UART_DIRECTION direction)
{
    DL_Common_updateReg(&uart->CTL0, (uint32_t) direction,
        UART_CTL0_TXE_MASK | UART_CTL0_RXE_MASK);
}

/**
 *  @brief      获取 UART 通信的方向
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     UART 通信的方向。
 *
 *  @retval     @ref DL_UART_DIRECTION 之一
 */
__STATIC_INLINE DL_UART_DIRECTION DL_UART_getDirection(const UART_Regs *uart)
{
    uint32_t direction =
        uart->CTL0 & (UART_CTL0_TXE_MASK | UART_CTL0_RXE_MASK);

    return (DL_UART_DIRECTION)(direction);
}

/**
 *  @brief      启用多数投票控制
 *
 *  启用时，使用三个中心位来确定接收的样本值。对应于至少两个样本的值被视为接收值。
 *  如果出现错误（即所有 3 位都不相同），则检测到噪声错误并设置位 RIS.NERR 和寄存器 RXDATA.NERR。
 *
 *  当启用 16 倍过采样时，对样本 7、8 和 9 进行多数投票以决定采样位值。
 *
 *  当启用 8 倍过采样时，对样本 3、4 和 5 进行多数投票以决定采样位值。对应于至少 2 个样本的值被视为接收值。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 *
 *  @sa          DL_UART_setOversampling
 */
__STATIC_INLINE void DL_UART_enableMajorityVoting(UART_Regs *uart)
{
    uart->CTL0 |= UART_CTL0_MAJVOTE_ENABLE;
}

/**
 *  @brief      检查多数投票是否已启用
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 *
 *  @return     多数控制功能的状态
 *
 *  @retval     true  多数投票已启用
 *  @retval     false 多数投票已禁用
 */
__STATIC_INLINE bool DL_UART_isMajorityVotingEnabled(const UART_Regs *uart)
{
    return ((uart->CTL0 & UART_CTL0_MAJVOTE_MASK) == UART_CTL0_MAJVOTE_ENABLE);
}

/**
 *  @brief      禁用多数投票控制
 *
 *  禁用时，只对接收位进行单次采样。
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 */
__STATIC_INLINE void DL_UART_disableMajorityVoting(UART_Regs *uart)
{
    uart->CTL0 &= ~(UART_CTL0_MAJVOTE_MASK);
}

/**
 *  @brief      启用最高有效位 (MSB) 优先
 *
 *  启用时，最高有效位 (MSB) 在协议数据包中首先发送。此位对协议字节的传输和接收方式都有影响。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 */
__STATIC_INLINE void DL_UART_enableMSBFirst(UART_Regs *uart)
{
    uart->CTL0 |= UART_CTL0_MSBFIRST_ENABLE;
}

/**
 *  @brief      检查最高有效位 (MSB) 优先是否已启用
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 *
 *  @return     MSB 优先功能的状态
 *
 *  @retval     true  MSB 优先已启用
 *  @retval     false MSB 优先已禁用
 */
__STATIC_INLINE bool DL_UART_isMSBFirstEnabled(const UART_Regs *uart)
{
    return (
        (uart->CTL0 & UART_CTL0_MSBFIRST_MASK) == UART_CTL0_MSBFIRST_ENABLE);
}

/**
 *  @brief      禁用最高有效位 (MSB) 优先
 *
 *  禁用时，最低有效位 (LSB) 在协议数据包中首先发送。此位对协议字节的传输和接收方式都有影响。
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 */
__STATIC_INLINE void DL_UART_disableMSBFirst(UART_Regs *uart)
{
    uart->CTL0 &= ~(UART_CTL0_MSBFIRST_MASK);
}

/**
 *  @brief      启用 TXD 引脚的控制
 *
 *  启用时，TXD 引脚可以由 TXD_OUT 位控制。UART 传输部分必须首先被禁用。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 *
 *  @sa          DL_UART_setDirection
 *  @sa          DL_UART_setTXDPin
 */
__STATIC_INLINE void DL_UART_enableTransmitPinManualControl(UART_Regs *uart)
{
    uart->CTL0 |= UART_CTL0_TXD_OUT_EN_ENABLE;
}

/**
 *  @brief      检查 TXD 引脚的控制是否已启用
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 *
 *  @return     TXD 引脚控制的状态
 *
 *  @retval     true  TXD 引脚的控制已启用
 *  @retval     false TXD 引脚的控制已禁用
 */
__STATIC_INLINE bool DL_UART_isTransmitPinManualControlEnabled(
    const UART_Regs *uart)
{
    return ((uart->CTL0 & UART_CTL0_TXD_OUT_EN_MASK) ==
            UART_CTL0_TXD_OUT_EN_ENABLE);
}

/**
 *  @brief      禁用 TXD 引脚的控制
 *
 *  禁用时，TXD 引脚不能由 TXD_OUT 位控制
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 */
__STATIC_INLINE void DL_UART_disableTransmitPinManualControl(UART_Regs *uart)
{
    uart->CTL0 &= ~(UART_CTL0_TXD_OUT_EN_MASK);
}

/**
 *  @brief      设置 TXD 引脚的输出
 *
 *  仅当 TXD_OUT_EN 启用且 TXE 禁用时，控制输出传输数据引脚。
 *
 *  如果 TXD 引脚之前未设置为手动控制，则将其设置为手动控制。
 *
 *  @param[in]  uart       指向外设寄存器覆盖的指针
 *  @param[in]  txdOutVal  要设置 TXD 引脚输出的值。
 *                         @ref DL_UART_TXD_OUT 之一
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 *
 *  @sa          DL_UART_disableTransmitPinManualControl
 *  @sa          DL_UART_enableTransmitPinManualControl
 */
__STATIC_INLINE void DL_UART_setTransmitPinManualOutput(
    UART_Regs *uart, DL_UART_TXD_OUT txdOutVal)
{
    DL_Common_updateReg(&uart->CTL0,
        UART_CTL0_TXD_OUT_EN_ENABLE | (uint32_t) txdOutVal,
        UART_CTL0_TXD_OUT_EN_MASK | UART_CTL0_TXD_OUT_MASK);
}

/**
 *  @brief      获取 TXD 引脚的输出值
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     TXD 引脚的值
 *
 *  @retval     @ref DL_UART_TXD_OUT 之一
 */
__STATIC_INLINE DL_UART_TXD_OUT DL_UART_getTransmitPinManualOutput(
    const UART_Regs *uart)
{
    uint32_t txdOutVal = uart->CTL0 & UART_CTL0_TXD_OUT_MASK;

    return (DL_UART_TXD_OUT)(txdOutVal);
}

/**
 *  @brief      启用曼彻斯特编码
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_enableManchesterEncoding(UART_Regs *uart)
{
    uart->CTL0 |= UART_CTL0_MENC_ENABLE;
}

/**
 *  @brief      禁用曼彻斯特编码
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_disableManchesterEncoding(UART_Regs *uart)
{
    uart->CTL0 &= ~(UART_CTL0_MENC_MASK);
}

/**
 *  @brief      检查曼彻斯特编码是否已启用
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     曼彻斯特编码的状态
 *
 *  @retval     true  曼彻斯特编码已启用
 *  @retval     false 曼彻斯特编码已禁用
 */
__STATIC_INLINE bool DL_UART_isManchesterEncodingEnabled(const UART_Regs *uart)
{
    return ((uart->CTL0 & UART_CTL0_MENC_MASK) == UART_CTL0_MENC_ENABLE);
}

/**
 *  @brief      设置要使用的通信模式/协议
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *  @param[in]  mode  要设置 UART 通信协议的值。
 *                     @ref DL_UART_MODE 之一
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 */
__STATIC_INLINE void DL_UART_setCommunicationMode(
    UART_Regs *uart, DL_UART_MODE mode)
{
    DL_Common_updateReg(&uart->CTL0, (uint32_t) mode, UART_CTL0_MODE_MASK);
}

/**
 *  @brief      获取正在使用的通信模式/协议
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     UART 正在使用的通信模式/协议
 *
 *  @retval     @ref DL_UART_MODE 之一
 */
__STATIC_INLINE DL_UART_MODE DL_UART_getCommunicationMode(
    const UART_Regs *uart)
{
    uint32_t mode = uart->CTL0 & UART_CTL0_MODE_MASK;

    return (DL_UART_MODE)(mode);
}

/**
 *  @brief      设置流控制配置
 *
 *  @param[in]  uart    指向外设寄存器覆盖的指针
 *  @param[in]  config  要使用的流控制配置。
 *                      @ref DL_UART_FLOW_CONTROL 之一。
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 */
__STATIC_INLINE void DL_UART_setFlowControl(
    UART_Regs *uart, DL_UART_FLOW_CONTROL config)
{
    DL_Common_updateReg(&uart->CTL0, (uint32_t) config,
        UART_CTL0_RTSEN_MASK | UART_CTL0_CTSEN_MASK);
}

/**
 *  @brief      检查流控制配置
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     流控制配置
 *
 *  @retval     @ref DL_UART_FLOW_CONTROL 值之一
 */
__STATIC_INLINE DL_UART_FLOW_CONTROL DL_UART_getFlowControl(
    const UART_Regs *uart)
{
    uint32_t config =
        uart->CTL0 & (UART_CTL0_RTSEN_MASK | UART_CTL0_CTSEN_MASK);

    return (DL_UART_FLOW_CONTROL)(config);
}

/**
 *  @brief      设置请求发送输出信号
 *
 *  RTS 输出信号指示 RX FIFO 的状态，并与可编程接收 FIFO 阈值水平相关联。当启用 RTS 流控制时，
 *  当 RX FIFO 中的数据低于阈值水平时，RTS 信号被断言（低电平）。当达到 RX FIFO 阈值水平时，
 *  RTS 信号被取消断言（高电平）。当数据从 RX FIFO 中读出，使其低于阈值时，RTS 信号被重新断言（低电平）。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *  @param[in]  val   RTS 输出信号。@ref DL_UART_RTS 之一
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 *
 *  @sa         DL_UART_setTXFIFOThreshold
 */
__STATIC_INLINE void DL_UART_setRTSOutput(UART_Regs *uart, DL_UART_RTS val)
{
    DL_Common_updateReg(&uart->CTL0, (uint32_t) val, UART_CTL0_RTS_MASK);
}

/**
 *  @brief      获取请求发送输出信号
 *
 *  RTS 输出信号指示 RX FIFO 的状态，并与可编程接收 FIFO 阈值水平相关联。当启用 RTS 流控制时，
 *  当 RX FIFO 中的数据低于阈值水平时，RTS 信号被断言（低电平）。当达到 RX FIFO 阈值水平时，
 *  RTS 信号被取消断言（高电平）。当数据从 RX FIFO 中读出，使其低于阈值时，RTS 信号被重新断言（低电平）。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     RTS 信号状态
 *
 *  @retval     @ref DL_UART_RTS 之一
 *
 *  @sa         DL_UART_setTXFIFOThreshold
 */
__STATIC_INLINE DL_UART_RTS DL_UART_getRTSOutput(const UART_Regs *uart)
{
    uint32_t val = uart->CTL0 & UART_CTL0_RTS_MASK;

    return (DL_UART_RTS)(val);
}

/**
 *  @brief      启用 FIFO
 *
 *  启用传输和接收 FIFO 缓冲区。
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 *
 *  @sa         DL_UART_init
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 */
__STATIC_INLINE void DL_UART_enableFIFOs(UART_Regs *uart)
{
    uart->CTL0 |= UART_CTL0_FEN_ENABLE;
}

/**
 *  @brief      禁用 FIFO
 *
 *  禁用传输和接收 FIFO 缓冲区。接收器现在只能保存 1 字节的数据。
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 *
 *  @pre  如果 UART 已经启用，则必须先调用 @ref DL_UART_changeConfig 使其准备好进行配置
 *  @post 如果调用了 @ref DL_UART_changeConfig，则必须通过调用 @ref DL_UART_enable 重新启用 UART
 */
__STATIC_INLINE void DL_UART_disableFIFOs(UART_Regs *uart)
{
    uart->CTL0 &= ~(UART_CTL0_FEN_MASK);
}

/**
 *  @brief      检查 FIFO 是否已启用
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     FIFO 的状态
 *
 *  @retval     true  FIFO 已启用
 *  @retval     false FIFO 已禁用
 */
__STATIC_INLINE bool DL_UART_isFIFOsEnabled(const UART_Regs *uart)
{
    return ((uart->CTL0 & UART_CTL0_FEN_MASK) == UART_CTL0_FEN_ENABLE);
}

/**
 *  @brief      启用发送中断（用于 LIN 协议）
 *
 *  启用时，在完成当前字符的传输后，TXD 信号上会持续输出低电平。为了正确执行中断命令，
 *  软件必须将此位设置至少两个帧（字符周期）。
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_enableLINSendBreak(UART_Regs *uart)
{
    uart->LCRH |= UART_LCRH_BRK_ENABLE;
}

/**
 *  @brief      禁用发送中断
 *
 *  禁用时，在完成当前字符的传输后，TXD 信号上不会持续输出低电平。
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_disableLINSendBreak(UART_Regs *uart)
{
    uart->LCRH &= ~(UART_LCRH_BRK_MASK);
}

/**
 *  @brief      检查发送中断是否已启用
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     发送中断的状态
 *
 *  @retval     true  发送中断已启用
 *  @retval     false 发送中断已禁用
 */
__STATIC_INLINE bool DL_UART_isLINSendBreakEnabled(const UART_Regs *uart)
{
    return ((uart->LCRH & UART_LCRH_BRK_MASK) == UART_LCRH_BRK_ENABLE);
}

/**
 *  @brief      检查奇偶校验是否已启用
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     奇偶校验的状态
 *
 *  @retval     true  奇偶校验已启用
 *  @retval     false 奇偶校验已禁用
 */
__STATIC_INLINE bool DL_UART_isParityEnabled(const UART_Regs *uart)
{
    return ((uart->LCRH & UART_LCRH_PEN_MASK) == UART_LCRH_PEN_ENABLE);
}

/**
 *  @brief      设置奇偶校验模式
 *
 *  对于 9 位 UART 模式传输，奇偶校验模式会影响地址字节和数据字节指示（第 9 位）。如果启用了 DL_UART_PARITY_EVEN 或
 *  DL_UART_PARITY_STICK_ZERO，则传输的字节是奇偶校验位为 '1' 的地址字节。如果未启用 DL_UART_PARITY_EVEN 或
 *  DL_UART_PARITY_STICK_ZERO，则传输的字节是奇偶校验位为 '0' 的地址字节。
 *
 *  @param[in]  uart    指向外设寄存器覆盖的指针
 *  @param[in]  parity  要设置 UART 的奇偶校验模式。
 *                      @ref DL_UART_PARITY 之一
 */
__STATIC_INLINE void DL_UART_setParityMode(
    UART_Regs *uart, DL_UART_PARITY parity)
{
    DL_Common_updateReg(&uart->LCRH, (uint32_t) parity,
        (UART_LCRH_PEN_MASK | UART_LCRH_EPS_MASK | UART_LCRH_SPS_MASK));
}

/**
 *  @brief      获取奇偶校验模式
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     当前使用的奇偶校验模式
 *
 *  @retval     @ref DL_UART_PARITY 之一
 *
 */
__STATIC_INLINE DL_UART_PARITY DL_UART_getParityMode(const UART_Regs *uart)
{
    uint32_t parity = uart->LCRH & (UART_LCRH_PEN_MASK | UART_LCRH_EPS_MASK |
                                       UART_LCRH_SPS_MASK);

    return (DL_UART_PARITY)(parity);
}

/**
 *  @brief      设置停止位数量
 *
 *  在 7816 智能卡模式（DL_UART_MODE_SMART_CARD 模式）下，停止位数量被强制设置为 2
 *
 *  @param[in]  uart         指向外设寄存器覆盖的指针
 *  @param[in]  numStopBits  传输的停止位数量。
 *                           @ref DL_UART_STOP_BITS 之一
 */
__STATIC_INLINE void DL_UART_setStopBits(
    UART_Regs *uart, DL_UART_STOP_BITS numStopBits)
{
    DL_Common_updateReg(
        &uart->LCRH, (uint32_t) numStopBits, UART_LCRH_STP2_MASK);
}

/**
 *  @brief      获取停止位数量
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     传输的停止位数量
 *
 *  @retval     @ref DL_UART_STOP_BITS 之一
 */
__STATIC_INLINE DL_UART_STOP_BITS DL_UART_getStopBits(const UART_Regs *uart)
{
    uint32_t numStopBits = uart->LCRH & UART_LCRH_STP2_MASK;

    return (DL_UART_STOP_BITS)(numStopBits);
}

/**
 *  @brief      设置字长
 *
 *  @param[in]  uart        指向外设寄存器覆盖的指针
 *  @param[in]  wordLength  一帧中传输或接收的数据位数。@ref DL_UART_WORD_LENGTH 之一
 */
__STATIC_INLINE void DL_UART_setWordLength(
    UART_Regs *uart, DL_UART_WORD_LENGTH wordLength)
{
    DL_Common_updateReg(
        &uart->LCRH, (uint32_t) wordLength, UART_LCRH_WLEN_MASK);
}

/**
 *  @brief      获取字长
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     一帧中传输或接收的数据位长度
 *
 *  @retval     @ref DL_UART_WORD_LENGTH 之一
 */
__STATIC_INLINE DL_UART_WORD_LENGTH DL_UART_getWordLength(
    const UART_Regs *uart)
{
    uint32_t wordLength = uart->LCRH & UART_LCRH_WLEN_MASK;

    return (DL_UART_WORD_LENGTH)(wordLength);
}

/**
 *  @brief      发送空闲模式
 *
 *  启用时，TX 线上会发送 11 位时间的 SENDIDLE 周期。之后该位会被硬件清除。
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_enableSendIdlePattern(UART_Regs *uart)
{
    uart->LCRH |= UART_LCRH_SENDIDLE_ENABLE;
}

/**
 *  @brief      禁用发送空闲模式
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_disableSendIdlePattern(UART_Regs *uart)
{
    uart->LCRH &= ~(UART_LCRH_SENDIDLE_MASK);
}

/**
 *  @brief      检查发送空闲模式是否已启用
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     发送空闲模式的状态
 *
 *  @retval     true  发送空闲模式已启用
 *  @retval     false 发送空闲模式已禁用
 */
__STATIC_INLINE bool DL_UART_isSendIdlePatternEnabled(const UART_Regs *uart)
{
    return (
        (uart->LCRH & UART_LCRH_SENDIDLE_MASK) == UART_LCRH_SENDIDLE_ENABLE);
}

/**
 *  @brief      设置外部驱动器设置值
 *
 *  定义在发送 START 位之前，控制 RS485 外部驱动器的信号将设置的 UARTclk 周期数
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *  @param[in]  val   RS485 设置前信号的 UARTclk 周期数。值范围 0 - 31。
 */
__STATIC_INLINE void DL_UART_setExternalDriverSetup(
    UART_Regs *uart, uint32_t val)
{
    DL_Common_updateReg(&uart->LCRH, val << UART_LCRH_EXTDIR_SETUP_OFS,
        UART_LCRH_EXTDIR_SETUP_MASK);
}

/**
 *  @brief      获取外部驱动器设置值
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     控制外部驱动器的信号的 UARTclk 周期数
 *
 *  @retval     0 - 31 UARTclk 周期数
 */
__STATIC_INLINE uint32_t DL_UART_getExternalDriverSetup(const UART_Regs *uart)
{
    return ((uart->LCRH &
             UART_LCRH_EXTDIR_SETUP_MASK) >> UART_LCRH_EXTDIR_SETUP_OFS);
}

/**
 *  @brief      设置外部驱动器保持时间
 *
 *  定义在停止位开始后，控制 RS485 外部驱动器的信号将被重置的 UARTclk 周期数。
 *  如果启用了 2 个停止位，RS485 将在第 2 个停止位开始时被重置。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *  @param[in]  val   在 RS485 被重置前保持外部驱动器的信号的 UARTclk 周期数。
 *                    值范围 0 - 31。
 */
__STATIC_INLINE void DL_UART_setExternalDriverHold(
    UART_Regs *uart, uint32_t val)
{
    DL_Common_updateReg(&uart->LCRH, val << UART_LCRH_EXTDIR_HOLD_OFS,
        UART_LCRH_EXTDIR_HOLD_MASK);
}

/**
 *  @brief      获取外部驱动器保持时间
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 *
 *  @return     在 RS485 被重置前保持外部驱动器的信号的 UARTclk 周期数
 *
 *  @retval     0 - 31 UARTclk 周期数
 */
__STATIC_INLINE uint32_t DL_UART_getExternalDriverHold(const UART_Regs *uart)
{
    return ((
        uart->LCRH & UART_LCRH_EXTDIR_HOLD_MASK) >> UART_LCRH_EXTDIR_HOLD_OFS);
}

/**
 *  @brief      检查 UART 是否忙
 *
 *  当发送 FIFO 或 TXDATA 寄存器变为非空（无论 UART 是否已启用）或当前正在接收数据（从检测到起始沿到移位寄存器接收到完整字节，包括所有停止位）时，此位被设置。
 *
 *  在 IDLE 线路模式下，忙信号在空闲时间生成期间也保持设置状态。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     UART 忙位的状态
 *
 *  @retval     true   UART 忙
 *  @retval     false  UART 不忙
 *
 */
__STATIC_INLINE bool DL_UART_isBusy(const UART_Regs *uart)
{
    return ((uart->STAT & UART_STAT_BUSY_MASK) == UART_STAT_BUSY_SET);
}

/**
 *  @brief      检查 RX FIFO 是否为空
 *
 *  此位的含义取决于 FIFO 是否已启用。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     返回 RX FIFO 的空状态
 *
 *  @retval     true   如果 FIFO 已启用，接收 FIFO 为空。
 *                     如果 FIFO 已禁用，接收器无数据。
 *  @retval     false  接收器不为空
 *
 *  @sa         DL_UART_enableFIFOs
 */
__STATIC_INLINE bool DL_UART_isRXFIFOEmpty(const UART_Regs *uart)
{
    return ((uart->STAT & UART_STAT_RXFE_MASK) == UART_STAT_RXFE_SET);
}

/**
 *  @brief      检查 RX FIFO 是否已满
 *
 *  此位的含义取决于 FIFO 是否已启用。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     返回 RX FIFO 的满状态
 *
 *  @retval     true   如果 FIFO 已启用，接收 FIFO 已满。
 *                     如果 FIFO 已禁用，接收器有 1 字节数据。
 *  @retval     false  接收器未满
 *
 *  @sa         DL_UART_enableFIFOs
 */
__STATIC_INLINE bool DL_UART_isRXFIFOFull(const UART_Regs *uart)
{
    return ((uart->STAT & UART_STAT_RXFF_MASK) == UART_STAT_RXFF_SET);
}

/**
 *  @brief      检查 TX FIFO 是否为空
 *
 *  此位的含义取决于 FIFO 是否已启用。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     返回 TX FIFO 的空状态
 *
 *  @retval     true   如果 FIFO 已启用，发送 FIFO 为空。
 *                     如果 FIFO 已禁用，发送器无数据。
 *  @retval     false  发送器不为空
 *
 *  @sa         DL_UART_enableFIFOs
 */
__STATIC_INLINE bool DL_UART_isTXFIFOEmpty(const UART_Regs *uart)
{
    return ((uart->STAT & UART_STAT_TXFE_MASK) == UART_STAT_TXFE_SET);
}

/**
 *  @brief      检查 TX FIFO 是否已满
 *
 *  此位的含义取决于 FIFO 是否已启用。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     返回 TX FIFO 的满状态
 *
 *  @retval     true   如果 FIFO 已启用，发送 FIFO 已满。
 *                     如果 FIFO 已禁用，发送器有 1 字节数据。
 *  @retval     false  发送器未满
 *
 *  @sa         DL_UART_enableFIFOs
 */
__STATIC_INLINE bool DL_UART_isTXFIFOFull(const UART_Regs *uart)
{
    return ((uart->STAT & UART_STAT_TXFF_MASK) == UART_STAT_TXFF_SET);
}

/**
 *  @brief      检查 UART 是否可以发送
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     返回 CTS 信号的状态
 *
 *  @retval     true   CTS 信号已断言（低电平）
 *  @retval     false  CTS 信号未断言（高电平）
 *
 *  @sa         DL_UART_isClearToSendEnabled
 */
__STATIC_INLINE bool DL_UART_isClearToSend(const UART_Regs *uart)
{
    return ((uart->STAT & UART_STAT_CTS_MASK) == UART_STAT_CTS_SET);
}

/**
 *  @brief      检查是否检测到空闲模式
 *
 *  在空闲线-多处理器模式下检测到空闲模式。IDLE 位用作每个字符块的地址标记。在空闲线多处理器格式中，当接收到的字符是地址时，此位被设置。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     返回是否检测到空闲模式的状态
 *
 *  @retval     true  在前一个接收的字符之前检测到空闲
 *  @retval     false 在前一个接收的字符之前未检测到空闲
 *
 */
__STATIC_INLINE bool DL_UART_isIdleModeDetected(const UART_Regs *uart)
{
    return ((uart->STAT & UART_STAT_IDLE_MASK) == UART_STAT_IDLE_SET);
}

/**
 *  @brief      设置 TX FIFO 中断阈值水平
 *
 *  选择发送 FIFO 中断的阈值。中断是基于通过一个水平的过渡而不是基于水平本身生成的。也就是说，当中断填充水平通过触发水平时生成中断。例如，如果发送触发水平设置为中间标记，则当发送 FIFO 变为半空时触发中断。换句话说，如果发送 FIFO 填充了四个字符，则在传输后剩下两个或更少字符时触发中断。
 *
 *  @param[in]  uart       指向外设寄存器覆盖的指针
 *  @param[in]  threshold  @ref DL_UART_TX_FIFO_LEVEL 之一
 *
 */
__STATIC_INLINE void DL_UART_setTXFIFOThreshold(
    UART_Regs *uart, DL_UART_TX_FIFO_LEVEL threshold)
{
    DL_Common_updateReg(
        &uart->IFLS, (uint32_t) threshold, UART_IFLS_TXIFLSEL_MASK);
}

/**
 *  @brief      获取 TX FIFO 中断阈值水平
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     TX FIFO 中断阈值水平
 *
 *  @retval     @ref DL_UART_TX_FIFO_LEVEL 之一
 */
__STATIC_INLINE DL_UART_TX_FIFO_LEVEL DL_UART_getTXFIFOThreshold(
    const UART_Regs *uart)
{
    uint32_t threshold = uart->IFLS & UART_IFLS_TXIFLSEL_MASK;

    return (DL_UART_TX_FIFO_LEVEL)(threshold);
}

/**
 *  @brief      设置 RX FIFO 中断阈值水平。中断是基于通过一个水平的过渡而不是基于水平本身生成的。也就是说，当中断填充水平通过触发水平时生成中断。例如，如果接收触发水平设置为中间标记，则当接收 FIFO 变为半满时触发中断。换句话说，在接收 FIFO 填充了两个或更多字符后触发中断。
 *
 *  @param[in]  uart       指向外设寄存器覆盖的指针
 *  @param[in]  threshold  @ref DL_UART_RX_FIFO_LEVEL 之一
 */
__STATIC_INLINE void DL_UART_setRXFIFOThreshold(
    UART_Regs *uart, DL_UART_RX_FIFO_LEVEL threshold)
{
    DL_Common_updateReg(
        &uart->IFLS, (uint32_t) threshold, UART_IFLS_RXIFLSEL_MASK);
}

/**
 *  @brief      获取 RX FIFO 中断阈值水平
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     RX FIFO 中断阈值水平
 *
 *  @retval     @ref DL_UART_RX_FIFO_LEVEL 之一
 */
__STATIC_INLINE DL_UART_RX_FIFO_LEVEL DL_UART_getRXFIFOThreshold(
    const UART_Regs *uart)
{
    uint32_t threshold = uart->IFLS & UART_IFLS_RXIFLSEL_MASK;

    return (DL_UART_RX_FIFO_LEVEL)(threshold);
}

/**
 *  @brief      设置 RX 中断超时
 *
 *  当在设定的超时时间内未接收到额外字符时，即使 FIFO 水平未达到，RX 中断仍会触发。值为 0 时禁用此功能。
 *
 *  @param[in]  uart     指向外设寄存器覆盖的指针
 *  @param[in]  timeout  要设置的 RX 中断超时时间。
 *                       值范围 0 - 15
 */
__STATIC_INLINE void DL_UART_setRXInterruptTimeout(
    UART_Regs *uart, uint32_t timeout)
{
    DL_Common_updateReg(
        &uart->IFLS, timeout << UART_IFLS_RXTOSEL_OFS, UART_IFLS_RXTOSEL_MASK);
}

/**
 *  @brief      获取 RX 中断超时
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     RX 中断超时值
 *
 *  @retval     0 - 15  RX 中断超时值
 *
 */
__STATIC_INLINE uint32_t DL_UART_getRXInterruptTimeout(const UART_Regs *uart)
{
    return ((uart->IFLS & UART_IFLS_RXTOSEL_MASK) >> UART_IFLS_RXTOSEL_OFS);
}

/**
 *  @brief      获取整数波特率除数
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     波特率除数的整数部分
 *
 *  @retval     0 - 65535 整数波特率除数
 */
__STATIC_INLINE uint32_t DL_UART_getIntegerBaudRateDivisor(
    const UART_Regs *uart)
{
    return (uart->IBRD & UART_IBRD_DIVINT_MASK);
}

/**
 *  @brief      获取小数波特率除数
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     波特率除数的小数部分
 *
 *  @retval     0 - 63  小数波特率除数
 */
__STATIC_INLINE uint32_t DL_UART_getFractionalBaudRateDivisor(
    const UART_Regs *uart)
{
    return (uart->FBRD & UART_FBRD_DIVFRAC_MASK);
}

/**
 *  @brief      设置波特率除数
 *
 *  设置波特率除数的整数部分和小数部分
 *
 *  @param[in]  uart               指向外设寄存器覆盖的指针
 *  @param[in]  integerDivisor     波特率除数的整数部分
 *  @param[in]  fractionalDivisor  波特率除数的小数部分
 */
__STATIC_INLINE void DL_UART_setBaudRateDivisor(
    UART_Regs *uart, uint32_t integerDivisor, uint32_t fractionalDivisor)
{
    DL_Common_updateReg(&uart->IBRD, integerDivisor, UART_IBRD_DIVINT_MASK);
    DL_Common_updateReg(
        &uart->FBRD, fractionalDivisor, UART_FBRD_DIVFRAC_MASK);

    // When updating the baud-rate divisor (UARTIBRD or UARTIFRD),
    // the LCRH register must also be written to (any bit in LCRH can
    // be written to for updating the baud-rate divisor).
    DL_Common_updateReg(
        &uart->LCRH, (uart->LCRH & UART_LCRH_BRK_MASK), UART_LCRH_BRK_MASK);
}

/**
 *  @brief      为 IrDA 模式设置波特率除数
 *
 *  设置波特率除数的整数部分和小数部分
 *  将 integerDivisor 除以 clkDivisor2 + 1，以降低 UART 时钟频率，从而根据 IrDA 标准进一步减少波特率除数
 *
 *  @param[in]  uart               指向外设寄存器覆盖的指针
 *  @param[in]  integerDivisor     波特率除数的整数部分
 *  @param[in]  fractionalDivisor  波特率除数的小数部分
 *  @param[in]  clkDivisor2        用于分频时钟的附加因子，
 *                                 @ref DL_UART_CLOCK_DIVIDE2_RATIO 之一
 *
 */
__STATIC_INLINE void DL_UART_setIrDABaudRateDivisor(UART_Regs *uart,
    uint32_t integerDivisor, uint32_t fractionalDivisor,
    DL_UART_CLOCK_DIVIDE2_RATIO clkDivisor2)
{
    DL_Common_updateReg(&uart->IBRD,
        (integerDivisor / ((uint32_t) clkDivisor2 + 1)),
        UART_IBRD_DIVINT_MASK);
    DL_Common_updateReg(
        &uart->FBRD, fractionalDivisor, UART_FBRD_DIVFRAC_MASK);

    // When updating the baud-rate divisor (UARTIBRD or UARTIFRD),
    // the LCRH register must also be written to (any bit in LCRH can
    // be written to for updating the baud-rate divisor).
    DL_Common_updateReg(
        &uart->LCRH, (uart->LCRH & UART_LCRH_BRK_MASK), UART_LCRH_BRK_MASK);
}

/**
 *  @brief      设置数字 glitch 抑制的脉冲宽度选择
 *
 *  控制 RX 线上 glitch 抑制的脉冲宽度选择。glitch 抑制值以功能时钟为单位。
 *
 *  在 IrDA 模式下，这设置接收滤波器长度。接收的最小脉冲长度由以下公式给出：t(MIN) = (DGFSEL) / f(IRTXCLK)
 *
 *  @param[in]  uart        指向外设寄存器覆盖的指针
 *  @param[in]  pulseWidth  glitch 抑制的脉冲宽度选择。
 *                          值范围 0 - 63。
 */
__STATIC_INLINE void DL_UART_setDigitalPulseWidth(
    UART_Regs *uart, uint32_t pulseWidth)
{
    DL_Common_updateReg(&uart->GFCTL, pulseWidth, UART_GFCTL_DGFSEL_MASK);
}

/**
 *  @brief      获取数字 glitch 抑制的脉冲宽度选择
 *
 *  获取 RX 线上 glitch 抑制的脉冲宽度选择。glitch 抑制值以功能时钟为单位。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     数字 glitch 抑制脉冲宽度
 *
 *  @retval     0 表示数字 glitch 抑制已禁用
 *  @retval     1 - 63 数字 glitch 抑制脉冲宽度
 */
__STATIC_INLINE uint32_t DL_UART_getDigitalPulseWidth(const UART_Regs *uart)
{
    return (uart->GFCTL & UART_GFCTL_DGFSEL_MASK);
}

/**
 *  @brief      向 TX FIFO 写入数据以进行传输
 *
 *  将数据放入 TX FIFO 而不检查其状态。如果已经确定 TX FIFO 有空间进行写入，则使用此函数。有关其他传输选项，请参阅相关 API。
 *
 *  对于传输的数据，如果 FIFO 已启用，写入此位置的数据将被推送到发送 FIFO。如果 FIFO 已禁用，数据将存储在 1 字节深的发送器中。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *  @param[in]  data  要传输的数据
 *
 *
 *  @sa         DL_UART_transmitDataBlocking
 *  @sa         DL_UART_transmitDataCheck
 */
__STATIC_INLINE void DL_UART_transmitData(UART_Regs *uart, uint8_t data)
{
    uart->TXDATA = data;
}

/**
 *  @brief      从 RX FIFO 读取数据
 *
 *  从 RX FIFO 读取数据而不检查其状态。如果已经确定 RX FIFO 有可用数据，则使用此函数。有关其他接收选项，请参阅相关 API。
 *
 *  @note:      读取 RX FIFO 数据的结果是，RXDATA 寄存器中的相应错误状态（OVRERR、BRKERR、PARERR、FRMERR 位）将被丢弃。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     RX FIFO 中的数据
 *
 *  @sa         DL_UART_receiveDataBlocking
 *  @sa         DL_UART_receiveDataCheck
 */
__STATIC_INLINE uint8_t DL_UART_receiveData(const UART_Regs *uart)
{
    return ((uint8_t)(uart->RXDATA & UART_RXDATA_DATA_MASK));
}

/**
 *  @brief      获取接收数据的错误标志状态
 *
 *  @note:      读取错误状态的结果是，RXDATA.DATA 位字段中的相应 RX FIFO 数据将被丢弃。
 *
 *  @param[in]  uart       指向外设寄存器覆盖的指针
 *  @param[in]  errorMask  要检查的错误标志的位掩码。@ref DL_UART_ERROR 的按位或。
 *
 *  @return     请求的 UART 错误标志的状态
 *
 *  @retval     @ref DL_UART_ERROR 值的按位或
 */
__STATIC_INLINE uint32_t DL_UART_getErrorStatus(
    const UART_Regs *uart, uint32_t errorMask)
{
    return (uart->RXDATA & errorMask);
}

/**
 *  @brief      设置 LIN 计数器值
 *
 *  LIN 计数器是一个 16 位向上计数器，由 UART 的功能时钟驱动
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 *  @param[in]  value  要设置的 LIN 计数器值
 */
__STATIC_INLINE void DL_UART_setLINCounterValue(
    UART_Regs *uart, uint16_t value)
{
    DL_Common_updateReg(&uart->LINCNT, value, UART_LINCNT_VALUE_MASK);
}

/**
 *  @brief      获取 LIN 计数器值
 *
 *  LIN 计数器是一个 16 位向上计数器，由 UART 的模块时钟驱动
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     时钟计数器值
 *
 *  @retval     0 - 65535  时钟计数器值
 *
 */
__STATIC_INLINE uint16_t DL_UART_getLINCounterValue(const UART_Regs *uart)
{
    return ((uint16_t)(uart->LINCNT & UART_LINCNT_VALUE_MASK));
}

/**
 *  @brief      启用 LIN 计数器
 *
 *  LIN 计数器仅在启用时计数。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_enableLINCounter(UART_Regs *uart)
{
    uart->LINCTL |= UART_LINCTL_CTRENA_ENABLE;
}

/**
 *  @brief      检查 LIN 计数器是否已启用
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     LIN 计数器的状态
 *
 *  @retval     true   LIN 计数器已启用
 *  @retval     false  LIN 计数器已禁用
 */
__STATIC_INLINE bool DL_UART_isLINCounterEnabled(const UART_Regs *uart)
{
    return (
        (uart->LINCTL & UART_LINCTL_CTRENA_MASK) == UART_LINCTL_CTRENA_ENABLE);
}

/**
 *  @brief      禁用 LIN 计数器
 *
 *  LIN 计数器仅在启用时计数。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_disableLINCounter(UART_Regs *uart)
{
    uart->LINCTL &= ~(UART_LINCTL_CTRENA_MASK);
}

/**
 *  @brief   启用 LIN 计数器清除并在 RXD 的下降沿开始计数
 *
 *  启用时，计数器被设置为 0 并在 RXD 的下降沿开始 LIN 计数器计数。
 *
 *  LIN 计数器仅在启用时计数。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @sa  DL_UART_enableLINCounter
 */
__STATIC_INLINE void DL_UART_enableLINCounterClearOnFallingEdge(
    UART_Regs *uart)
{
    uart->LINCTL |= UART_LINCTL_ZERONE_ENABLE;
}

/**
 *  @brief      检查 RXD 下降沿计数是否已启用
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     RXD 下降沿计数的状态
 *
 *  @retval     true   下降沿计数已启用
 *  @retval     false  下降沿计数已禁用
 */
__STATIC_INLINE bool DL_UART_isLINCounterClearOnFallingEdge(
    const UART_Regs *uart)
{
    return (
        (uart->LINCTL & UART_LINCTL_ZERONE_MASK) == UART_LINCTL_ZERONE_ENABLE);
}

/**
 *  @brief      禁用 RXD 下降沿计数
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_disableLINCounterClearOnFallingEdge(
    UART_Regs *uart)
{
    uart->LINCTL &= ~(UART_LINCTL_ZERONE_MASK);
}

/**
 *  @brief      启用 RXD 信号为低电平时的 LIN 计数器递增
 *
 *  当 LIN 计数器启用且 RXD 上的信号为低电平时，计数器递增
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @sa  DL_UART_enableLINCounter
 */
__STATIC_INLINE void DL_UART_enableLINCountWhileLow(UART_Regs *uart)
{
    uart->LINCTL |= UART_LINCTL_CNTRXLOW_ENABLE;
}

/**
 *  @brief   检查 RXD 信号为低电平时的 LIN 计数器递增是否已启用
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     RXD 信号为低电平时的计数器递增状态
 *
 *  @retval     true   RXD 信号为低电平时的计数器递增已启用
 *  @retval     false  RXD 信号为低电平时的计数器递增已禁用
 */
__STATIC_INLINE bool DL_UART_isLINCountWhileLowEnabled(const UART_Regs *uart)
{
    return ((uart->LINCTL & UART_LINCTL_CNTRXLOW_MASK) ==
            UART_LINCTL_CNTRXLOW_ENABLE);
}

/**
 *  @brief      禁用 RXD 信号为低电平时的 LIN 计数器递增
 *
 *  当 RXD 信号为低电平时，LIN 计数器不会递增
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_disableLINCountWhileLow(UART_Regs *uart)
{
    uart->LINCTL &= ~(UART_LINCTL_CNTRXLOW_MASK);
}

/**
 *  @brief      启用下降沿 LIN 计数器捕获
 *
 *  启用时，LIN 计数器值在每个 RXD 下降沿被捕获到 LINC0 寄存器。启用时会触发 LINC0 中断。
 *  如果启用了计数器比较匹配模式，则禁用该模式。
 *
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @sa  DL_UART_configLINMode
 */
__STATIC_INLINE void DL_UART_enableLINFallingEdgeCapture(UART_Regs *uart)
{
    DL_Common_updateReg(&uart->LINCTL,
        UART_LINCTL_LINC0CAP_ENABLE | UART_LINCTL_LINC0_MATCH_DISABLE,
        UART_LINCTL_LINC0CAP_MASK | UART_LINCTL_LINC0_MATCH_MASK);
}

/**
 *  @brief      检查下降沿 LIN 计数器捕获的状态
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     下降沿 LIN 计数器捕获的状态
 *
 *  @retval     true   RXD 下降沿捕获到 LINC0 已启用
 *  @retval     false  RXD 下降沿捕获到 LINC0 已禁用
 */
__STATIC_INLINE bool DL_UART_isLINFallingEdgeCaptureEnabled(
    const UART_Regs *uart)
{
    return ((uart->LINCTL & UART_LINCTL_LINC0CAP_MASK) ==
            UART_LINCTL_LINC0CAP_ENABLE);
}

/**
 *  @brief      禁用下降沿 LIN 计数器捕获
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_disableLINFallingEdgeCapture(UART_Regs *uart)
{
    uart->LINCTL &= ~(UART_LINCTL_LINC0CAP_MASK);
}

/**
 *  @brief      启用上升沿 LIN 计数器捕获
 *
 *  启用时，LIN 计数器值在每个 RXD 上升沿被捕获到 LINC1 寄存器。启用时会触发 LINC1 中断。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 */
__STATIC_INLINE void DL_UART_enableLINRisingEdgeCapture(UART_Regs *uart)
{
    uart->LINCTL |= UART_LINCTL_LINC1CAP_ENABLE;
}

/**
 *  @brief      检查上升沿 LIN 计数器捕获的状态
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     上升沿 LIN 计数器捕获的状态
 *
 *  @retval     true   RXD 上升沿捕获到 LINC1 已启用
 *  @retval     false  RXD 上升沿捕获到 LINC1 已禁用
 */
__STATIC_INLINE bool DL_UART_isLINRisingEdgeCaptureEnabled(
    const UART_Regs *uart)
{
    return ((uart->LINCTL & UART_LINCTL_LINC1CAP_MASK) ==
            UART_LINCTL_LINC1CAP_ENABLE);
}

/**
 *  @brief      禁用上升沿 LIN 计数器捕获
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_disableLINRisingEdgeCapture(UART_Regs *uart)
{
    uart->LINCTL &= ~(UART_LINCTL_LINC1CAP_MASK);
}

/**
 *  @brief      启用 LIN 计数器比较匹配模式
 *
 *  启用时，LINC0 中的值与 LIN 计数器之间的匹配可以触发 LINC0 中断。如果启用了下降沿捕获，则禁用该模式。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_enableLINCounterCompareMatch(UART_Regs *uart)
{
    DL_Common_updateReg(&uart->LINCTL,
        UART_LINCTL_LINC0_MATCH_ENABLE | UART_LINCTL_LINC0CAP_DISABLE,
        UART_LINCTL_LINC0CAP_MASK | UART_LINCTL_LINC0_MATCH_MASK);
}

/**
 *  @brief      设置用于同步字段验证的 LIN 计数器控制
 *
 *  启用 RX 上升沿 LIN 计数器捕获。启用 RX 下降沿 LIN 计数器捕获。
 *  启用 RX 下降沿 LIN 计数器清除。启用 LIN 计数器。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_enableLINSyncFieldValidationCounterControl(
    UART_Regs *uart)
{
    DL_Common_updateReg(&uart->LINCTL,
        UART_LINCTL_LINC0CAP_ENABLE | UART_LINCTL_LINC1CAP_ENABLE |
            UART_LINCTL_ZERONE_ENABLE | UART_LINCTL_CTRENA_ENABLE,
        UART_LINCTL_LINC0CAP_MASK | UART_LINCTL_LINC1CAP_MASK |
            UART_LINCTL_ZERONE_MASK | UART_LINCTL_CTRENA_MASK);
}

/**
 *  @brief      设置用于 LIN 接收的 LIN 计数器控制
 *
 *  启用 RXD 低信号时计数。启用 RXD 下降沿 LIN 计数器清除。
 *  启用 LIN 计数器。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_enableLINReceptionCountControl(UART_Regs *uart)
{
    DL_Common_updateReg(&uart->LINCTL,
        UART_LINCTL_CNTRXLOW_ENABLE | UART_LINCTL_ZERONE_ENABLE |
            UART_LINCTL_CTRENA_ENABLE,
        UART_LINCTL_CNTRXLOW_MASK | UART_LINCTL_ZERONE_MASK |
            UART_LINCTL_CTRENA_MASK);
}

/**
 *  @brief      检查 LIN 计数器比较匹配模式是否已启用
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     计数器比较匹配模式的状态
 *
 *  @retval     true   LIN 计数器比较匹配模式已启用
 *  @retval     false  LIN 计数器比较匹配模式已禁用
 */
__STATIC_INLINE bool DL_UART_isLINCounterCompareMatchEnabled(
    const UART_Regs *uart)
{
    return ((uart->LINCTL & UART_LINCTL_LINC0_MATCH_MASK) ==
            UART_LINCTL_LINC0_MATCH_ENABLE);
}

/**
 *  @brief      禁用 LIN 计数器比较匹配模式
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_UART_disableLINCounterCompareMatch(UART_Regs *uart)
{
    uart->LINCTL &= ~(UART_LINCTL_LINC0_MATCH_MASK);
}

/**
 *  @brief      设置要与 LIN 计数器比较的值
 *
 *  设置 LINC0 的值，用于与 LIN 计数器进行比较。用于启用 LIN 计数器比较匹配模式时。
 *
 *  @param[in]  uart   指向外设寄存器覆盖的指针
 *  @param[in]  value  要与 LIN 计数器比较以进行匹配的值。
 *                     值范围 0 - 65535。
 *
 *  @sa  DL_UART_enableLINCounterCompareMatch
 */
__STATIC_INLINE void DL_UART_setLINCounterCompareValue(
    UART_Regs *uart, uint16_t value)
{
    DL_Common_updateReg(&uart->LINC0, value, UART_LINC0_DATA_MASK);
}

/**
 *  @brief      获取 LINC0 计数器值
 *
 *  在 RXD 下降沿捕获当前 LINCTR 值。
 *
 *  如果使用 @ref DL_UART_enableLINFallingEdgeCapture 启用了捕获，则捕获可以生成 LINC0 中断。
 *  如果使用 @ref DL_UART_enableLINCounterCompareMatch 启用了比较模式，则计数器匹配可以生成 LINC0 中断。
 *
 *  @param[in]  uart  指向外设寄存器覆盖的指针
 *
 *  @return     LINC0 计数器值
 *
 *  @retval     0 - 65535  LINC0 计数器值
 *
 *  @sa  DL_UART_enableLINFallingEdgeCapture
 *  @sa  DL_UART_enableLINCounterCompareMatch
 */
__STATIC_INLINE uint16_t DL_UART_getLINFallingEdgeCaptureValue(
    const UART_Regs *uart)
{
    return ((uint16_t)(uart->LINC0 & UART_LINC0_DATA_MASK));
}

/**
 *  @brief      Get the LINC1 counter value
 *
 *  Captures current LINCTR value on RXD rising edge. For use when LIN rising
 *  edge capture is enabled. It can generate a LINC1 interrupt on capture.
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @return     The LINC1 counter value
 *
 *  @retval     0 - 65535  The LINC1 counter value
 *
 *  @sa  DL_UART_enableLINRisingEdgeCapture
 */
__STATIC_INLINE uint16_t DL_UART_getLINRisingEdgeCaptureValue(
    const UART_Regs *uart)
{
    return ((uint16_t)(uart->LINC1 & UART_LINC1_DATA_MASK));
}

/**
 *  @brief      Enable the IrDA encoder/decoder
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_UART_enableIrDAMode(UART_Regs *uart)
{
    uart->IRCTL |= UART_IRCTL_IREN_ENABLE;
}

/**
 *  @brief      Check if the IrDA encoder/decoder is enabled
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @return     The status of IrDA mode
 *
 *  @retval     true   IrDA mode is enabled
 *  @retval     false  IrDA mode is disabled
 */
__STATIC_INLINE bool DL_UART_isIrDAModeEnabled(const UART_Regs *uart)
{
    return ((uart->IRCTL & UART_IRCTL_IREN_MASK) == UART_IRCTL_IREN_ENABLE);
}

/**
 *  @brief      Disable the IrDA encoder/decoder
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_UART_disableIrDAMode(UART_Regs *uart)
{
    uart->IRCTL &= ~(UART_IRCTL_IREN_MASK);
}

/**
 *  @brief      Set the IrDA transmit pulse clock select
 *
 *  @param[in]  uart   Pointer to the register overlay for the peripheral
 *  @param[in]  uartClock  The clock select for the IrDA pulse data to be based on.
 *                     One of @ref DL_UART_IRDA_CLOCK
 */
__STATIC_INLINE void DL_UART_setIrDATXPulseClockSelect(
    UART_Regs *uart, DL_UART_IRDA_CLOCK uartClock)
{
    DL_Common_updateReg(
        &uart->IRCTL, (uint32_t) uartClock, UART_IRCTL_IRTXCLK_MASK);
}

/**
 *  @brief      Get the IrDA transmit pulse clock select
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @return     The clock select that the IrDA pulse data is based on
 *
 *  @retval     One of @ref DL_UART_IRDA_CLOCK
 *
 */
__STATIC_INLINE DL_UART_IRDA_CLOCK DL_UART_getIrDATXPulseClockSelect(
    const UART_Regs *uart)
{
    uint32_t uartClock = uart->IRCTL & UART_IRCTL_IRTXCLK_MASK;

    return (DL_UART_IRDA_CLOCK)(uartClock);
}

/**
 *  @brief      Set the IrDA configurations
 *
 *  Calculates the baud rate divisor given the clock output of the UART clock
 *  source and the target baud rate. This API also enables IrDA mode.
 *
 *  @param[in]  uart         Pointer to the register overlay for the peripheral
 *  @param[in]  polarity     The receive input polarity.
 *                           One of @ref DL_UART_IRDA_POLARITY.
 *  @param[in]  pulseLength  The length of the IrDA transmit pulse.
 *  @param[in]  irdaClk      The clock used for the transmit pulse.
 *                           One of @ref DL_UART_IRDA_CLOCK.
 */
void DL_UART_configIrDAMode(UART_Regs *uart, DL_UART_IRDA_POLARITY polarity,
    uint32_t pulseLength, DL_UART_IRDA_CLOCK irdaClk);

/**
 *  @brief      Set the IrDA transmit pulse length
 *
 *  The pulse length can be calculated with the following equation:
 *      IRTXPLx = pulseLength * 2 * irdaClk - 1
 *      (IRTXCLK = functional clock of the UART)
 *
 *  To set the pulse time of 3/16 bit period required by the IrDA period,
 *  16-bit oversampling is selected with HSE = 0, the baud rate clock is
 *  selected with IRTXCLK = 1, and the pulse length is set to six one-half
 *  clock cycles with IRTXPLx = 6 - 1 - 5.
 *
 *  @param[in]  uart         Pointer to the register overlay for the peripheral
 *  @param[in]  pulseLength  The length of the IrDA transmit pulse.
 *  @param[in]  irdaClk      The clock used for the transmit pulse.
 *                           One of @ref DL_UART_IRDA_CLOCK.
 */
void DL_UART_setIrDAPulseLength(
    UART_Regs *uart, uint32_t pulseLength, DL_UART_IRDA_CLOCK irdaClk);

/**
 *  @brief      Get the IrDA transmit pulse length
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @return     The IrDA transmit pulse length is set to
 *
 *  @retval     0 - 63  The IrDA transmit pulse length
 */
__STATIC_INLINE uint32_t DL_UART_getIrDATXPulseLength(const UART_Regs *uart)
{
    return (uart->IRCTL & UART_IRCTL_IRTXPL_MASK);
}

/**
 *  @brief      Set the IrDA receive input UCAxRXD polarity
 *
 *  @param[in]  uart      Pointer to the register overlay for the peripheral
 *  @param[in]  polarity  The value to set the IrDA RX pulse polarity to.
 *                        One of @ref DL_UART_IRDA_POLARITY
 */
__STATIC_INLINE void DL_UART_setIrDARXPulsePolarity(
    UART_Regs *uart, DL_UART_IRDA_POLARITY polarity)
{
    DL_Common_updateReg(
        &uart->IRCTL, (uint32_t) polarity, UART_IRCTL_IRRXPL_MASK);
}

/**
 *  @brief      Get the IrDA receive input UCAxRXD polarity
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @return     The IrDA RX pulse polarity
 *
 *  @retval     One of @ref DL_UART_IRDA_POLARITY
 */
__STATIC_INLINE DL_UART_IRDA_POLARITY DL_UART_getIrDARXPulsePolarity(
    const UART_Regs *uart)
{
    uint32_t polarity = uart->IRCTL & UART_IRCTL_IRRXPL_MASK;

    return (DL_UART_IRDA_POLARITY)(polarity);
}

/**
 *  @brief      Set the address mask for DALI, 9-bit, or Idle-Line mode
 *
 *  The address bits are masked to create a set of addresses to be matched
 *  with the received address byte.
 *
 *  A 0 bit in the MSK bitfield configures that the corresponding bit in the
 *  ADDR bitfield of the ADDR register is don't care.
 *  A 1 bit in the MSK bitfield configures, that the corresponding bit in the
 *  ADDR bitfield of the ADDR register must match.
 *
 *  Used in DALI, UART 9-Bit or Idle-Line mode.
 *
 *  @param[in]  uart         Pointer to the register overlay for the peripheral
 *  @param[in]  addressMask  The address mask to set
 */
__STATIC_INLINE void DL_UART_setAddressMask(
    UART_Regs *uart, uint32_t addressMask)
{
    DL_Common_updateReg(&uart->AMASK, addressMask, UART_AMASK_VALUE_MASK);
}

/**
 *  @brief      Get the address mask being used
 *
 *  The address bits are masked to create a set of addresses to be matched
 *  with the received address byte.
 *
 *  A 0 bit in the MSK bitfield configures that the corresponding bit in the
 *  ADDR bitfield of the ADDR register is don't care.
 *  A 1 bit in the MSK bitfield configures, that the corresponding bit in the
 *  ADDR bitfield of the ADDR register must match.
 *
 *  Used in DALI, UART 9-Bit or Idle-Line mode.
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @return     The address mask being used
 *
 *  @retval     0-255  The address mask
 *
 */
__STATIC_INLINE uint32_t DL_UART_getAddressMask(const UART_Regs *uart)
{
    return (uart->AMASK & UART_AMASK_VALUE_MASK);
}

/**
 *  @brief      Set the address
 *
 *  Used to write the specific address that should be matched with the
 *  receiving byte when the Address Mask (AMASK) is set to FFh. This register
 *  is used in conjunction with AMASK to form a match for address-byte
 *  received.
 *
 *  Used in DALI, UART 9-Bit or Idle-Line mode.
 *
 *  @param[in]  uart     Pointer to the register overlay for the peripheral
 *  @param[in]  address  The address to set
 */
__STATIC_INLINE void DL_UART_setAddress(UART_Regs *uart, uint32_t address)
{
    DL_Common_updateReg(&uart->ADDR, address, UART_ADDR_VALUE_MASK);
}

/**
 *  @brief      Get the address being used
 *
 *  Used to write the specific address that should be matched with the
 *  receiving byte when the Address Mask (AMASK) is set to FFh. This register
 *  is used in conjunction with AMASK to form a match for address-byte
 *  received.
 *
 *  Used in DALI, UART 9-Bit or Idle-Line mode.
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @return     The address being used
 *
 *  @retval     0-255  The address being used
 *
 */
__STATIC_INLINE uint32_t DL_UART_getAddress(const UART_Regs *uart)
{
    return (uart->ADDR & UART_ADDR_VALUE_MASK);
}

/**
 *  @brief      Enable UART interrupts
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_UART_INTERRUPT.
 */
__STATIC_INLINE void DL_UART_enableInterrupt(
    UART_Regs *uart, uint32_t interruptMask)
{
    uart->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      Disable UART interrupts
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of
 *                             @ref DL_UART_INTERRUPT.
 */
__STATIC_INLINE void DL_UART_disableInterrupt(
    UART_Regs *uart, uint32_t interruptMask)
{
    uart->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      Check which UART interrupts are enabled
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_UART_INTERRUPT.
 *
 *  @return     Which of the requested UART interrupts are enabled
 *
 *  @retval     Bitwise OR of @ref DL_UART_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_UART_getEnabledInterrupts(
    const UART_Regs *uart, uint32_t interruptMask)
{
    return (uart->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled UART interrupts
 *
 *  Checks if any of the UART interrupts that were previously enabled are
 *  pending.
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_UART_INTERRUPT.
 *
 *  @return     Which of the requested UART interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_UART_INTERRUPT values
 *
 *  @sa         DL_UART_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_UART_getEnabledInterruptStatus(
    const UART_Regs *uart, uint32_t interruptMask)
{
    return (uart->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any UART interrupt
 *
 *  Checks if any of the UART interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_UART_INTERRUPT.
 *
 *  @return     Which of the requested UART interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_UART_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_UART_getRawInterruptStatus(
    const UART_Regs *uart, uint32_t interruptMask)
{
    return (uart->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      Get highest priority pending UART interrupt
 *
 *  Checks if any of the UART interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *
 *  @return     The highest priority pending UART interrupt
 *
 *  @retval     TBD
 */
__STATIC_INLINE DL_UART_IIDX DL_UART_getPendingInterrupt(const UART_Regs *uart)
{
    return (DL_UART_IIDX)(uart->CPU_INT.IIDX);
}

/**
 *  @brief      Clear pending UART interrupts
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
 *                             @ref DL_UART_INTERRUPT.
 */
__STATIC_INLINE void DL_UART_clearInterruptStatus(
    UART_Regs *uart, uint32_t interruptMask)
{
    uart->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief      Prepares the UART to change the configuration
 *
 *  If the UART has already been enabled, then it is recommended to call this
 *  function before calling other APIs that make changes to the CTL0 register.
 *  If changes are made to the CTL0 register without disabling the UART, then
 *  results are unpredictable. This API performs the following:
 *  1. Disable the UART.
 *  2. Wait for the end of transmission or reception of the current character.
 *  3. Flush the transmit FIFO by clearing bit FEN in the UART control
 *  register CTL0.
 *
 *  @post After calling this API, the user must be re-enabled by calling
 *        @ref DL_UART_enable.
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_UART_changeConfig(UART_Regs *uart)
{
    DL_UART_disable(uart);
    while (DL_UART_isBusy(uart)) {
        ;
    }
    DL_UART_disableFIFOs(uart);
}

/**
 *  @brief      Enable the analog glitch filter on the RX input
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_UART_enableAnalogGlitchFilter(UART_Regs *uart)
{
    uart->GFCTL |= UART_GFCTL_AGFEN_ENABLE;
}

/**
 *  @brief      Disable the analog glitch filter on the RX input
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_UART_disableAnalogGlitchFilter(UART_Regs *uart)
{
    uart->GFCTL &= ~(UART_GFCTL_AGFEN_MASK);
}

/**
 * @brief      Returns if analog glitch filter is enabled
 *
 * @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 * @return true   if analog glitch filter is enabled
 * @return false  if analog glitch filter is disabled
 */
__STATIC_INLINE bool DL_UART_isAnalogGlitchFilterEnabled(const UART_Regs *uart)
{
    return ((uart->GFCTL & UART_GFCTL_AGFEN_MASK) == UART_GFCTL_AGFEN_ENABLE);
}

/**
 *  @brief      Enable analog and digital noise glitch filter chaining
 *
 * When enabled, analog and digital glitch filters are chained and the output
 * of the combination is made available to the IP logic for sampling.
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_UART_enableGlitchFilterChaining(UART_Regs *uart)
{
    uart->GFCTL |= UART_GFCTL_CHAIN_ENABLED;
}

/**
 *  @brief      Disable analog and digital noise glitch filter chaining
 *
 * When disabled, only digital filter output is available to the IP logic
 * for sampling.
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_UART_disableGlitchFilterChaining(UART_Regs *uart)
{
    uart->GFCTL &= ~(UART_GFCTL_CHAIN_MASK);
}

/**
 * @brief      Returns if glitch filter chaining enabled
 *
 * @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 * @return true   if glitch filter chaining is enabled
 * @return false  if glitch filter chaining is disabled
 */
__STATIC_INLINE bool DL_UART_isGlitchFilterChainingEnabled(
    const UART_Regs *uart)
{
    return ((uart->GFCTL & UART_GFCTL_CHAIN_MASK) == UART_GFCTL_CHAIN_ENABLED);
}

/**
 *  @brief      设置 RX 线上模拟毛刺抑制的脉宽选择
 *
 *  Sets the pulse width select for the analog glitch
 *  suppression on the RX line. See device datasheet for exact values.
 *
 *  This only applies to Power Domain 0 (PD0).
 *
 *  @param[in]  uart        Pointer to the register overlay for the peripheral
 *  @param[in]  pulseWidth  Pulse width select for the glitch suppresion.
                            One of @ref DL_UART_PULSE_WIDTH
 */
__STATIC_INLINE void DL_UART_setAnalogPulseWidth(
    UART_Regs *uart, DL_UART_PULSE_WIDTH pulseWidth)
{
    DL_Common_updateReg(
        &uart->GFCTL, (uint32_t) pulseWidth, UART_GFCTL_AGFSEL_MASK);
}

/**
 *  @brief      获取用于毛刺抑制的脉宽选择
 *
 *  Gets the pulse width select for the analog glitch
 *  suppression on the RX line. See device datasheet for exact values.
 *
 *  This only applies to Power Domain 0 (PD0).
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @return     The pulse width select for the glitch suppression
 *
 *  @retval     One of @ref DL_UART_PULSE_WIDTH

 */
__STATIC_INLINE DL_UART_PULSE_WIDTH DL_UART_getAnalogPulseWidth(
    const UART_Regs *uart)
{
    uint32_t pulseWidth = uart->GFCTL & UART_GFCTL_AGFSEL_MASK;

    return (DL_UART_PULSE_WIDTH)(pulseWidth);
}

/**
 *  @brief      Blocks to ensure transmit is ready before sending data
 *
 *  Puts data into the TX FIFO, waiting indefintely until there is space in the
 *  the TX FIFO. After placing data, waits indefintely until data is sent and
 *  the entire TX FIFO is empty. Note that if data is already present in the TX
 *  FIFO when this API is called, it will block until all data is sent.
 *  See related APIs for additional transmit options.
 *
 *  Can be used for any data transfers that are less than or equal to 8 bits.
 *
 *  @param[in]  uart  pointer to the register overlay for the peripheral
 *  @param[in]  data  data to send
 *
 *  @sa         DL_UART_transmitData
 *  @sa         DL_UART_transmitDataCheck
 */
void DL_UART_transmitDataBlocking(UART_Regs *uart, uint8_t data);

/**
 *  @brief      Blocks to ensure receive is ready before reading data
 *
 *  Reads the data from the RX FIFO after blocking to ensure the RX FIFO is not
 *  empty. Will wait indefintely until there is data in the RX FIFO. See
 *  related APIs for additional receive options.
 *
 *  Can be used for any data transfers that are less than or equal to 8 bits.
 *
 *  @note:      As a result of reading the RX FIFO data, the corresponding
 *              error status in the RXDATA register (OVRERR, BRKERR, PARERR,
 *              FRMERR bits) will be dropped.
 *
 *  @param[in]  uart  pointer to the register overlay for the peripheral
 *
 *  @return     The data in the RX FIFO
 *
 *  @sa         DL_UART_transmitData
 *  @sa         DL_UART_transmitDataCheck
 */
uint8_t DL_UART_receiveDataBlocking(const UART_Regs *uart);

/**
 *  @brief      Checks the TX FIFO before trying to transmit data
 *
 *  Checks if the TX FIFO is already full before trying to add new data to the
 *  FIFO. Exits immediately if full rather than trying to block. See related
 *  APIs for additional transmit options.
 *
 *  Can be used for any data transfers that are less than or equal to 8 bits.
 *
 *  @param[in]  uart  pointer to the register overlay for the peripheral
 *  @param[in]  data  data to send
 *
 *  @return     If the transmit occurred
 *
 *  @retval     true  if data was added to the TX FIFO
 *  @retval     false if the TX FIFO was full and data was not added
 *
 *  @sa         DL_UART_transmitData
 *  @sa         DL_UART_transmitDataBlocking
 */
bool DL_UART_transmitDataCheck(UART_Regs *uart, uint8_t data);

/**
 *  @brief      Checks the RX FIFO before trying to transmit data
 *
 *  Checks if the RX FIFO is already empty before trying to read new data from
 *  the FIFO. Exits immediately if empty rather than trying to block. See
 *  related APIs for additional receive options.
 *
 *  Can be used for any data transfers that are less than or equal to 8 bits.
 *
 *  @note:      As a result of reading the RX FIFO data, the corresponding
 *              error status in the RXDATA register (OVRERR, BRKERR, PARERR,
 *              FRMERR bits) will be dropped.
 *
 *  @param[in]  uart   pointer to the register overlay for the peripheral
 *  @param[in]  buffer a buffer to write the received data into
 *
 *  @return     If the receive occurred
 *
 *  @retval     true  if data was read from the RX FIFO
 *  @retval     false if the RX FIFO was empty and data was not read
 *
 *  @sa         DL_UART_receiveData
 *  @sa         DL_UART_receiveDataBlocking
 */
bool DL_UART_receiveDataCheck(const UART_Regs *uart, uint8_t *buffer);

/**
 *  @brief       Read all available data out of the RX FIFO using 8 bit access
 *
 *  @param[in]   uart      Pointer to the register overlay for the peripheral
 *  @param[out]  buffer    Buffer to write received data into
 *  @param[in]   maxCount  Max number of bytes to read from the RX FIFO
 *
 *  @return      Number of bytes read from the RX FIFO
 */
uint32_t DL_UART_drainRXFIFO(
    const UART_Regs *uart, uint8_t *buffer, uint32_t maxCount);

/**
 *  @brief      Fill the TX FIFO until full using 8 bit access
 *
 *  Continuously write data into the TX FIFO until it is filled up or count has
 *  been reached.
 *
 *  @param[in]  uart    Pointer to the register overlay for the peripheral
 *  @param[in]  buffer  Buffer of data to write to the TX FIFO
 *  @param[in]  count   Max number of bytes to write to the TX FIFO
 *
 *  @return     Number of bytes written to the TX FIFO
 */
uint32_t DL_UART_fillTXFIFO(
    UART_Regs *uart, const uint8_t *buffer, uint32_t count);

/**
 *  @brief      Enable UART interrupt for triggering the DMA receive event
 *
 * Enables the UART interrupt to be used as the condition to generate an
 * event to directly trigger the DMA. This API configures the DMA_TRIG_RX
 * register, which is the event publisher used for triggering the DMA to do
 * a receive data transfer.
 *
 * @note Only one interrupt source should be enabled at a time.
 *
 *  @param[in]  uart       Pointer to the register overlay for the
 *                         peripheral
 *  @param[in]  interrupt  Interrupt to enable as the trigger condition for
 *                         the DMA. One of @ref DL_UART_DMA_INTERRUPT_RX.
 */
__STATIC_INLINE void DL_UART_enableDMAReceiveEvent(
    UART_Regs *uart, uint32_t interrupt)
{
    uart->DMA_TRIG_RX.IMASK = interrupt;
}

/**
 *  @brief      Enable UART interrupt for triggering the DMA transmit event
 *
 * Enables the UART interrupt to be used as the condition to generate an
 * event to directly trigger the DMA. This API configures the DMA_TRIG_TX
 * register, which is the event publisher used for triggering the DMA to do
 * a transmit data transfer.
 *
 * @note DMA_TRIG_TX only has one transmit interrupt source
 *
 *  @param[in]  uart       Pointer to the register overlay for the
 *                         peripheral
 */
__STATIC_INLINE void DL_UART_enableDMATransmitEvent(UART_Regs *uart)
{
    uart->DMA_TRIG_TX.IMASK = UART_DMA_TRIG_TX_IMASK_TXINT_SET;
}

/**
 *  @brief      Disables UART interrupt from triggering the DMA receive event
 *
 * Disables the UART interrupt as the condition to generate an event to
 * directly trigger the DMA. This API configures the DMA_TRIG_RX
 * register, which is the event publisher used for triggering the DMA to do
 * a receive data transfer.
 *
 *  @param[in]  uart       Pointer to the register overlay for the
 *                         peripheral
 *  @param[in]  interrupt  Interrupt to disable as the trigger condition for
 *                         the DMA. One of @ref DL_UART_DMA_INTERRUPT_RX.
 */
__STATIC_INLINE void DL_UART_disableDMAReceiveEvent(
    UART_Regs *uart, uint32_t interrupt)
{
    uart->DMA_TRIG_RX.IMASK &= ~(interrupt);
}

/**
 *  @brief      Disables UART interrupt from triggering the DMA transmit event
 *
 * Disables the UART interrupt as the condition to generate an event to
 * directly trigger the DMA. This API configures the DMA_TRIG_TX
 * register, which is the event publisher used for triggering the DMA to do
 * a transmit data transfer.
 *
 * @note DMA_TRIG_TX only has one transmit interrupt source
 *
 *  @param[in]  uart       Pointer to the register overlay for the
 *                         peripheral
 */
__STATIC_INLINE void DL_UART_disableDMATransmitEvent(UART_Regs *uart)
{
    uart->DMA_TRIG_TX.IMASK = UART_DMA_TRIG_TX_IMASK_TXINT_CLR;
}

/**
 *  @brief      Check which UART interrupt for DMA receive events is enabled
 *
 *  This API checks the DMA_TRIG_RX register, which is the event publisher used
 *  for triggering the DMA to do a receive data transfer.
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check Bitwise OR of
 *                             @ref DL_UART_DMA_INTERRUPT_RX.
 *
 *  @return     Which of the requested UART interrupts is enabled
 *
 *  @retval     One of @ref DL_UART_DMA_INTERRUPT_RX
 */
__STATIC_INLINE uint32_t DL_UART_getEnabledDMAReceiveEvent(
    const UART_Regs *uart, uint32_t interruptMask)
{
    return (uart->DMA_TRIG_RX.IMASK & interruptMask);
}

/**
 *  @brief      Check if UART interrupt for DMA transmit event is enabled
 *
 *  This API checks the DMA_TRIG_TX register, which is the event publisher used
 *  for triggering the DMA to do a transmit data transfer.
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *
 *  @return     The requested UART interrupt status
 *
 *  @retval     DL_UART_DMA_INTERRUPT_TX if enabled, 0 if not enabled
 */
__STATIC_INLINE uint32_t DL_UART_getEnabledDMATransmitEvent(
    const UART_Regs *uart)
{
    return (uart->DMA_TRIG_TX.IMASK & UART_DMA_TRIG_TX_IMASK_TXINT_MASK);
}

/**
 *  @brief      Check interrupt flag of enabled UART interrupt for DMA receive event
 *
 * Checks if any of the UART interrupts for the DMA receive event that were
 * previously enabled are pending.
 * This API checks the DMA_TRIG_RX register, which is the event publisher used
 * for triggering the DMA to do a receive data transfer.
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_UART_DMA_INTERRUPT_RX.
 *
 *  @return     The requested UART interrupt status
 *
 *  @retval     One of @ref DL_UART_DMA_INTERRUPT_RX
 *
 *  @sa         DL_UART_enableDMAReceiveEvent
 */
__STATIC_INLINE uint32_t DL_UART_getEnabledDMAReceiveEventStatus(
    const UART_Regs *uart, uint32_t interruptMask)
{
    return (uart->DMA_TRIG_RX.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled UART interrupt for DMA transmit event
 *
 * Checks if the UART interrupt for the DMA transmit event that was
 * previously enabled is pending.
 * This API checks the DMA_TRIG_TX register, which is the event publisher used
 * for triggering the DMA to do a transmit data transfer.
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *
 *  @return     The requested UART interrupt status
 *
 *  @retval     DL_UART_DMA_INTERRUPT_TX if enabled, 0 if not enabled
 *
 *  @sa         DL_UART_enableDMATransmitEvent
 */
__STATIC_INLINE uint32_t DL_UART_getEnabledDMATransmitEventStatus(
    const UART_Regs *uart)
{
    return (uart->DMA_TRIG_TX.MIS & UART_DMA_TRIG_TX_MIS_TXINT_MASK);
}

/**
 *  @brief      Check interrupt flag of any UART interrupt for DMA receive event
 *
 *  Checks if any of the UART interrupts for DMA receive event are pending.
 *  Interrupts do not have to be previously enabled.
 *  This API checks the DMA_TRIG_RX register, which is the event publisher used
 *  for triggering the DMA to do a receive data transfer.
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_UART_DMA_INTERRUPT_RX.
 *
 *  @return     Which of the requested UART interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_UART_DMA_INTERRUPT_RX values
 */
__STATIC_INLINE uint32_t DL_UART_getRawDMAReceiveEventStatus(
    const UART_Regs *uart, uint32_t interruptMask)
{
    return (uart->DMA_TRIG_RX.RIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any UART interrupt for DMA transmit event
 *
 *  Checks if any of the UART interrupts for DMA transmit event are pending.
 *  Interrupts do not have to be previously enabled.
 *  This API checks the DMA_TRIG_TX register, which is the event publisher used
 *  for triggering the DMA to do a transmit data transfer.
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *
 *  @return     The requested UART interrupt status
 *
 *  @retval     DL_UART_DMA_INTERRUPT_TX if enabled, 0 if not enabled
 */
__STATIC_INLINE uint32_t DL_UART_getRawDMATransmitEventStatus(
    const UART_Regs *uart)
{
    return (uart->DMA_TRIG_TX.RIS & UART_DMA_TRIG_TX_RIS_TXINT_MASK);
}

/**
 *  @brief      Get highest priority pending UART interrupt for DMA receive event
 *
 *  Checks if any of the UART interrupts for DMA receive event are pending.
 *  Interrupts do not have to be previously enabled.
 *  This API checks the DMA_TRIG_RX register, which is the event publisher used
 *  for triggering the DMA to do a receive data transfer.
 *
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *
 *  @return     The highest priority pending UART interrupt
 *
 *  @retval     One of @ref DL_UART_DMA_IIDX_RX
 */
__STATIC_INLINE DL_UART_DMA_IIDX_RX DL_UART_getPendingDMAReceiveEvent(
    const UART_Regs *uart)
{
    return (DL_UART_DMA_IIDX_RX)(uart->DMA_TRIG_RX.IIDX);
}

/**
 *  @brief      Get highest priority pending UART interrupt for DMA transmit event
 *
 *  Checks if the UART interrupt for DMA transmit event is pending.
 *  Interrupts do not have to be previously enabled.
 *  This API checks the DMA_TRIG_TX register, which is the event publisher used
 *  for triggering the DMA to do a transmit data transfer.
 *
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *
 *  @return     The highest priority pending UART interrupt
 *
 *  @retval     DL_UART_DMA_IIDX_TX if pending, 0 if not pending
 */
__STATIC_INLINE DL_UART_DMA_IIDX_TX DL_UART_getPendingDMATransmitEvent(
    const UART_Regs *uart)
{
    return (DL_UART_DMA_IIDX_TX)(uart->DMA_TRIG_TX.IIDX);
}

/**
 *  @brief      Clear pending UART interrupts for DMA receive event
 *
 *  This API checks the DMA_TRIG_RX register, which is the event publisher used
 *  for triggering the DMA to do a receive data transfer.
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
 *                             @ref DL_UART_DMA_INTERRUPT_RX.
 */
__STATIC_INLINE void DL_UART_clearDMAReceiveEventStatus(
    UART_Regs *uart, uint32_t interruptMask)
{
    uart->DMA_TRIG_RX.ICLR = interruptMask;
}

/**
 *  @brief      Clear pending UART interrupt for DMA transmit event
 *
 *  This API checks the DMA_TRIG_TX register, which is the event publisher used
 *  for triggering the DMA to do a transmit data transfer.
 *
 *  @param[in]  uart           Pointer to the register overlay for the
 *                             peripheral
 * @note DMA_TRIG_TX only has one transmit interrupt source
 */
__STATIC_INLINE void DL_UART_clearDMATransmitEventStatus(UART_Regs *uart)
{
    uart->DMA_TRIG_TX.ICLR = UART_DMA_TRIG_TX_ICLR_TXINT_CLR;
}

/*!
 *  @brief Sets the second clock divider ratio
 *
 *  This API sets the CLKDIV2 register, which stores the clock divider ratio
 *  used to further divide the UART function clock in IrDA UART mode
 *
 *  @param[in]  uart    Pointer to the register overlay for the peripheral
 *  @param[in]  ratio   The CLKDIV2 value.  One of @ref DL_UART_CLOCK_DIVIDE_RATIO
 */
__STATIC_INLINE void DL_UART_setClockDivider2(
    UART_Regs *uart, DL_UART_CLOCK_DIVIDE2_RATIO ratio)
{
    uart->CLKDIV2 = (uint32_t) ratio;
}

/*!
 *  @brief Gets the value of CLKDIV2
 *
 *  This API gets the value stored in the CLKDIV2 register, which stores the
 *  clock divider ratio to further divide the UART function clock in IrDA Mode
 *
 *  @param[in]  uart    Pointer to the register overlay for the peripheral
 *
 *  @return     The clock divider ratio stored in the CLKDIV2 register
 *
 *  @retval     The CLKDIV2 value.  One of @ref DL_UART_CLOCK_DIVIDE2_RATIO
 */

__STATIC_INLINE DL_UART_CLOCK_DIVIDE2_RATIO DL_UART_getClockDivider2(
    const UART_Regs *uart)
{
    uint32_t ratio = uart->CLKDIV2;
    return (DL_UART_CLOCK_DIVIDE2_RATIO) ratio;
}
#ifdef __MSPM0_HAS_UART_MAIN__

/**
 *  @brief      Save UART Main configuration before entering a power loss state.
 *
 *  Some MSPM0G peripherals residing in PD1 domain do not retain register
 *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
 *  for the full list of peripheral instances that exhibit this behavior.
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @param[in]  ptr  Configuration backup setup structure. See
 *                  @ref DL_UART_Main_backupConfig.
 *
 *  @retval     FALSE if a configuration already exists in ptr (will not be
 *              overwritten). TRUE if a configuration was successfully saved
 *
 */
bool DL_UART_Main_saveConfiguration(
    const UART_Regs *uart, DL_UART_Main_backupConfig *ptr);

/**
 *  @brief      Restore UART Main configuration after leaving a power loss state.
 *
 *  Some MSPM0G peripherals residing in PD1 domain do not retain register
 *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
 *  for the full list of peripheral instances that exhibit this behavior.
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @param[in]  ptr   Configuration backup setup structure. See
 *                    @ref DL_UART_Main_backupConfig.
 *
 *  @retval     FALSE if a configuration does not exist in ptr (will not be
 *              loaded). TRUE if a configuration successfully loaded
 *
 */
bool DL_UART_Main_restoreConfiguration(
    UART_Regs *uart, DL_UART_Main_backupConfig *ptr);

#endif /* __MSPM0_HAS_UART_MAIN__ */

#ifdef __MSPM0_HAS_UART_EXTD__

/**
 *  @brief      Save UART Extend configuration before entering a power loss
 *              state.
 *
 *  Some MSPM0G peripherals residing in PD1 domain do not retain register
 *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
 *  for the full list of peripheral instances that exhibit this behavior.
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @param[in]  ptr   Configuration backup setup structure. See
 *                    @ref DL_UART_Extend_backupConfig.
 *
 *  @retval     FALSE if a configuration already exists in ptr (will not be
 *              overwritten). TRUE if a configuration was successfully saved
 *
 */
bool DL_UART_Extend_saveConfiguration(
    const UART_Regs *uart, DL_UART_Extend_backupConfig *ptr);

/**
 *  @brief      Restore UART Extend configuration after leaving a power loss
 *              state.
 *
 *  Some MSPM0G peripherals residing in PD1 domain do not retain register
 *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
 *  for the full list of peripheral instances that exhibit this behavior.
 *
 *  @param[in]  uart  Pointer to the register overlay for the peripheral
 *
 *  @param[in]  ptr   Configuration backup setup structure. See
 *                    @ref DL_UART_Extend_backupConfig.
 *
 *  @retval     FALSE if a configuration does not exist in ptr (will not be
 *              loaded). TRUE if a configuration successfully loaded
 *
 */
bool DL_UART_Extend_restoreConfiguration(
    UART_Regs *uart, DL_UART_Extend_backupConfig *ptr);

#endif /* __MSPM0_HAS_UART_EXTD__ */

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_UART_MAIN__ || __MSPM0_HAS_UART_EXTD__ */

#else
#warning \
    "TI highly recommends accessing uart with dl_uart_main, dl_uart_extend.h only."
#endif /* ti_dl_dl_uart_main__include ti_dl_dl_uart_extend__include */

#endif /* ti_dl_dl_uart__include */
/** @}*/
