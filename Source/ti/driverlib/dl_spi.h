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
 *  @file       dl_spi.h
 *  @brief      SPI驱动库
 *  @defgroup   SPI 串行外设接口 (SPI)
 *
 *  @anchor ti_dl_dl_spi_Overview
 *  # 概述
 *
 *  串行外设接口驱动库允许对MSPM0 SPI模块进行完整配置。
 *  串行外设接口（SPI）模块提供标准化的串行接口，用于在MSPM0设备和其他具有SPI接口的外部设备之间传输数据。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup SPI
 * @{
 */
#ifndef ti_dl_dl_spi__include
#define ti_dl_dl_spi__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_SPI__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_SPI_CD_MODE
 *  @{
 */
/*!
 * @brief 数据模式
 */
#define DL_SPI_CD_MODE_DATA        (SPI_CTL1_CDMODE_DATA >> SPI_CTL1_CDMODE_OFS)

/*!
 * @brief 命令模式
 */
#define DL_SPI_CD_MODE_COMMAND  (SPI_CTL1_CDMODE_COMMAND >> SPI_CTL1_CDMODE_OFS)


/** @}*/

/** @addtogroup DL_SPI_INTERRUPT
 *  @{
 */

/*!
 * @brief 发送的DMA完成1事件中断
 */
#define DL_SPI_INTERRUPT_DMA_DONE_TX    (SPI_CPU_INT_IMASK_DMA_DONE_TX_SET)

/*!
 * @brief 接收的DMA完成1事件中断
 */
#define DL_SPI_INTERRUPT_DMA_DONE_RX    (SPI_CPU_INT_IMASK_DMA_DONE_RX_SET)
/*!
 * @brief SPI完成传输并进入空闲模式中断
 */
#define DL_SPI_INTERRUPT_IDLE                   (SPI_CPU_INT_IMASK_IDLE_SET)

/*!
 * @brief 发送FIFO为空中断
 */
#define DL_SPI_INTERRUPT_TX_EMPTY            (SPI_CPU_INT_IMASK_TXEMPTY_SET)

/*!
 * @brief 发送FIFO中断
 */
#define DL_SPI_INTERRUPT_TX                       (SPI_CPU_INT_IMASK_TX_SET)

/*!
 * @brief 接收FIFO中断
 */
#define DL_SPI_INTERRUPT_RX                       (SPI_CPU_INT_IMASK_RX_SET)

/*!
 * @brief 接收超时中断
 */
#define DL_SPI_INTERRUPT_RX_TIMEOUT            (SPI_CPU_INT_IMASK_RTOUT_SET)

/*!
 * @brief 接收FIFO满中断
 */
#define DL_SPI_INTERRUPT_RX_FULL              (SPI_CPU_INT_IMASK_RXFULL_SET)

/*!
 * @brief 发送FIFO下溢中断
 */
#define DL_SPI_INTERRUPT_TX_UNDERFLOW     (SPI_CPU_INT_IMASK_TXFIFO_UNF_SET)

/*!
 * @brief 奇偶校验错误
 */
#define DL_SPI_INTERRUPT_PARITY_ERROR            (SPI_CPU_INT_IMASK_PER_SET)

/*!
 * @brief 接收FIFO溢出中断
 */
#define DL_SPI_INTERRUPT_RX_OVERFLOW      (SPI_CPU_INT_IMASK_RXFIFO_OVF_SET)

/** @}*/

/*! @enum DL_SPI_DMA_IIDX_RX */
typedef enum {
    /*! 用于启用SPI接收作为DMA触发器的SPI中断索引 */
    DL_SPI_DMA_IIDX_RX_TRIGGER = SPI_DMA_TRIG_RX_IIDX_STAT_RX_EVT,
    /*! 用于启用SPI接收超时作为DMA触发器的SPI中断索引 */
    DL_SPI_DMA_IIDX_RX_TIMEOUT_TRIGGER = SPI_DMA_TRIG_RX_IIDX_STAT_RTOUT_EVT
} DL_SPI_DMA_IIDX_RX;

/*! @enum DL_SPI_DMA_IIDX_TX */
typedef enum {
    /*! 用于启用SPI发送作为DMA触发器的SPI中断索引 */
    DL_SPI_DMA_IIDX_TX_TRIGGER = SPI_DMA_TRIG_TX_IIDX_STAT_TX_EVT
} DL_SPI_DMA_IIDX_TX;

/** @addtogroup DL_SPI_DMA_INTERRUPT_RX
 *  @{
 */
/*!
 * @brief 用于启用SPI接收作为DMA触发器的SPI中断
 */
#define DL_SPI_DMA_INTERRUPT_RX               (SPI_DMA_TRIG_RX_IMASK_RX_SET)

/*!
 * @brief 用于启用SPI接收超时作为DMA触发器的SPI中断
 */
#define DL_SPI_DMA_INTERRUPT_RX_TIMEOUT       (SPI_DMA_TRIG_RX_IMASK_RTOUT_SET)

/** @}*/

/*!
 * @brief 用于启用SPI发送作为DMA触发器的SPI中断
 */
#define DL_SPI_DMA_INTERRUPT_TX               (SPI_DMA_TRIG_TX_IMASK_TX_SET)

/* clang-format on */

/*! @enum DL_SPI_PARITY */
typedef enum {
    /*! 使用偶校验，启用发送和接收校验 */
    DL_SPI_PARITY_EVEN =
        (SPI_CTL1_PES_ENABLE | SPI_CTL1_PREN_ENABLE | SPI_CTL1_PTEN_ENABLE),
    /*! 使用奇校验，启用发送和接收校验 */
    DL_SPI_PARITY_ODD =
        (SPI_CTL1_PES_DISABLE | SPI_CTL1_PREN_ENABLE | SPI_CTL1_PTEN_ENABLE),
    /*! 禁用接收和发送校验 */
    DL_SPI_PARITY_NONE = (SPI_CTL1_PREN_DISABLE | SPI_CTL1_PTEN_DISABLE)
} DL_SPI_PARITY;

/*! @enum DL_SPI_FRAME_FORMAT */
typedef enum {
    /*! Motorola 3线模式，极性0，相位0 */
    DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA0 =
        (SPI_CTL0_SPO_LOW | SPI_CTL0_SPH_FIRST | SPI_CTL0_FRF_MOTOROLA_3WIRE),
    /*! Motorola 3线模式，极性0，相位1 */
    DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA1 =
        (SPI_CTL0_SPO_LOW | SPI_CTL0_SPH_SECOND | SPI_CTL0_FRF_MOTOROLA_3WIRE),
    /*! Motorola 3线模式，极性1，相位0 */
    DL_SPI_FRAME_FORMAT_MOTO3_POL1_PHA0 =
        (SPI_CTL0_SPO_HIGH | SPI_CTL0_SPH_FIRST | SPI_CTL0_FRF_MOTOROLA_3WIRE),
    /*! Motorola 3线模式，极性1，相位1 */
    DL_SPI_FRAME_FORMAT_MOTO3_POL1_PHA1 =
        (SPI_CTL0_SPO_HIGH | SPI_CTL0_SPH_SECOND |
            SPI_CTL0_FRF_MOTOROLA_3WIRE),
    /*! Motorola 4线模式，极性0，相位0 */
    DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA0 =
        (SPI_CTL0_SPO_LOW | SPI_CTL0_SPH_FIRST | SPI_CTL0_FRF_MOTOROLA_4WIRE),
    /*! Motorola 4线模式，极性0，相位1 */
    DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA1 =
        (SPI_CTL0_SPO_LOW | SPI_CTL0_SPH_SECOND | SPI_CTL0_FRF_MOTOROLA_4WIRE),
    /*! Motorola 4线模式，极性1，相位0 */
    DL_SPI_FRAME_FORMAT_MOTO4_POL1_PHA0 =
        (SPI_CTL0_SPO_HIGH | SPI_CTL0_SPH_FIRST | SPI_CTL0_FRF_MOTOROLA_4WIRE),
    /*! Motorola 4线模式，极性1，相位1 */
    DL_SPI_FRAME_FORMAT_MOTO4_POL1_PHA1 =
        (SPI_CTL0_SPO_HIGH | SPI_CTL0_SPH_SECOND |
            SPI_CTL0_FRF_MOTOROLA_4WIRE),
    /*! TI同步帧格式 */
    DL_SPI_FRAME_FORMAT_TI_SYNC = (SPI_CTL0_FRF_TI_SYNC),
} DL_SPI_FRAME_FORMAT;

/*! @enum DL_SPI_MODE */
typedef enum {
    /*! 控制器模式 */
    DL_SPI_MODE_CONTROLLER = (SPI_CTL1_CP_ENABLE),
    /*! 外设模式 */
    DL_SPI_MODE_PERIPHERAL = (SPI_CTL1_CP_DISABLE)
} DL_SPI_MODE;

/*! @enum DL_SPI_BIT_ORDER */
typedef enum {
    /*! 最高位优先 */
    DL_SPI_BIT_ORDER_MSB_FIRST = (SPI_CTL1_MSB_ENABLE),
    /*! 最低位优先 */
    DL_SPI_BIT_ORDER_LSB_FIRST = (SPI_CTL1_MSB_DISABLE)
} DL_SPI_BIT_ORDER;

/*! @enum DL_SPI_DATA_SIZE */
typedef enum {
    /*! 数据大小 4位 */
    DL_SPI_DATA_SIZE_4 = (SPI_CTL0_DSS_DSS_4),
    /*! 数据大小 5位 */
    DL_SPI_DATA_SIZE_5 = (SPI_CTL0_DSS_DSS_5),
    /*! 数据大小 6位 */
    DL_SPI_DATA_SIZE_6 = (SPI_CTL0_DSS_DSS_6),
    /*! 数据大小 7位 */
    DL_SPI_DATA_SIZE_7 = (SPI_CTL0_DSS_DSS_7),
    /*! 数据大小 8位 */
    DL_SPI_DATA_SIZE_8 = (SPI_CTL0_DSS_DSS_8),
    /*! 数据大小 9位 */
    DL_SPI_DATA_SIZE_9 = (SPI_CTL0_DSS_DSS_9),
    /*! 数据大小 10位 */
    DL_SPI_DATA_SIZE_10 = (SPI_CTL0_DSS_DSS_10),
    /*! 数据大小 11位 */
    DL_SPI_DATA_SIZE_11 = (SPI_CTL0_DSS_DSS_11),
    /*! 数据大小 12位 */
    DL_SPI_DATA_SIZE_12 = (SPI_CTL0_DSS_DSS_12),
    /*! 数据大小 13位 */
    DL_SPI_DATA_SIZE_13 = (SPI_CTL0_DSS_DSS_13),
    /*! 数据大小 14位 */
    DL_SPI_DATA_SIZE_14 = (SPI_CTL0_DSS_DSS_14),
    /*! 数据大小 15位 */
    DL_SPI_DATA_SIZE_15 = (SPI_CTL0_DSS_DSS_15),
    /*! 数据大小 16位 */
    DL_SPI_DATA_SIZE_16 = (SPI_CTL0_DSS_DSS_16),
} DL_SPI_DATA_SIZE;

/*! @enum DL_SPI_CHIP_SELECT */
typedef enum {
    /*! 片选 0 */
    DL_SPI_CHIP_SELECT_0 = (SPI_CTL0_CSSEL_CSSEL_0),
    /*! 片选 1 */
    DL_SPI_CHIP_SELECT_1 = (SPI_CTL0_CSSEL_CSSEL_1),
    /*! 片选 2 */
    DL_SPI_CHIP_SELECT_2 = (SPI_CTL0_CSSEL_CSSEL_2),
    /*! 片选 3 */
    DL_SPI_CHIP_SELECT_3 = (SPI_CTL0_CSSEL_CSSEL_3),
    /*! 无片选 */
    DL_SPI_CHIP_SELECT_NONE = (0)
} DL_SPI_CHIP_SELECT;

/*! @enum DL_SPI_TX_FIFO_LEVEL */
typedef enum {
    /*! 当TX FIFO <= 3/4为空时触发中断 */
    DL_SPI_TX_FIFO_LEVEL_3_4_EMPTY = SPI_IFLS_TXIFLSEL_LVL_3_4,
    /*! 当TX FIFO <= 1/2为空时触发中断（默认） */
    DL_SPI_TX_FIFO_LEVEL_1_2_EMPTY = SPI_IFLS_TXIFLSEL_LVL_1_2,
    /*! 当TX FIFO <= 1/4为空时触发中断 */
    DL_SPI_TX_FIFO_LEVEL_1_4_EMPTY = SPI_IFLS_TXIFLSEL_LVL_1_4,
    /*! 当TX FIFO为空时触发中断 */
    DL_SPI_TX_FIFO_LEVEL_EMPTY = SPI_IFLS_TXIFLSEL_LVL_EMPTY,
    /*! 当TX FIFO有 >= 1帧空闲时触发中断。应与DMA一起使用 */
    DL_SPI_TX_FIFO_LEVEL_ONE_FRAME = SPI_IFLS_TXIFLSEL_LEVEL_1
} DL_SPI_TX_FIFO_LEVEL;

/*! @enum DL_SPI_RX_FIFO_LEVEL */
typedef enum {
    /*! 当RX FIFO包含 >= 1帧时触发中断。应与DMA一起使用 */
    DL_SPI_RX_FIFO_LEVEL_ONE_FRAME = SPI_IFLS_RXIFLSEL_LEVEL_1,
    /*! 当RX FIFO满时触发中断 */
    DL_SPI_RX_FIFO_LEVEL_FULL = SPI_IFLS_RXIFLSEL_LVL_FULL,
    /*! 当RX FIFO >= 3/4满时触发中断 */
    DL_SPI_RX_FIFO_LEVEL_3_4_FULL = SPI_IFLS_RXIFLSEL_LVL_3_4,
    /*! 当RX FIFO >= 1/2满时触发中断（默认） */
    DL_SPI_RX_FIFO_LEVEL_1_2_FULL = SPI_IFLS_RXIFLSEL_LVL_1_2,
    /*! 当RX FIFO >= 1/4满时触发中断 */
    DL_SPI_RX_FIFO_LEVEL_1_4_FULL = SPI_IFLS_RXIFLSEL_LVL_1_4,
} DL_SPI_RX_FIFO_LEVEL;

/*! @enum DL_SPI_IIDX */
typedef enum {

    /*! 用于发送的DMA完成1事件的SPI中断索引 */
    DL_SPI_IIDX_DMA_DONE_TX = SPI_CPU_INT_IIDX_STAT_DMA_DONE_TX_EVT,
    /*! 用于接收的DMA完成1事件的SPI中断索引 */
    DL_SPI_IIDX_DMA_DONE_RX = SPI_CPU_INT_IIDX_STAT_DMA_DONE_RX_EVT,
    /*! 用于SPI信号完成传输并进入空闲模式的SPI中断索引 */

    DL_SPI_IIDX_IDLE = SPI_CPU_INT_IIDX_STAT_IDLE_EVT,
    /*! 用于发送FIFO为空的SPI中断索引 */
    DL_SPI_IIDX_TX_EMPTY = SPI_CPU_INT_IIDX_STAT_TX_EMPTY,
    /*! 用于发送FIFO的SPI中断索引 */
    DL_SPI_IIDX_TX = SPI_CPU_INT_IIDX_STAT_TX_EVT,
    /*! 用于接收FIFO的SPI中断索引 */
    DL_SPI_IIDX_RX = SPI_CPU_INT_IIDX_STAT_RX_EVT,
    /*! 用于接收超时的SPI中断索引 */
    DL_SPI_IIDX_RX_TIMEOUT = SPI_CPU_INT_IIDX_STAT_RTOUT_EVT,

    /*! 用于接收FIFO满的SPI中断索引 */
    DL_SPI_IIDX_RX_FULL = SPI_CPU_INT_IIDX_STAT_RXFULL_EVT,
    /*! 用于发送FIFO下溢的SPI中断索引 */
    DL_SPI_IIDX_TX_UNDERFLOW = SPI_CPU_INT_IIDX_STAT_TXFIFO_UNF_EVT,

    /*! 用于奇偶校验错误的SPI中断索引 */
    DL_SPI_IIDX_PARITY_ERROR = SPI_CPU_INT_IIDX_STAT_PER_EVT,
    /*! 用于接收FIFO溢出的SPI中断索引 */
    DL_SPI_IIDX_RX_OVERFLOW = SPI_CPU_INT_IIDX_STAT_RXFIFO_OFV_EVT
} DL_SPI_IIDX;

/*! @enum DL_SPI_CLOCK_DIVIDE_RATIO */
typedef enum {
    /*! SPI源时钟分频比设置为1 */
    DL_SPI_CLOCK_DIVIDE_RATIO_1 = SPI_CLKDIV_RATIO_DIV_BY_1,
    /*! SPI源时钟分频比设置为2 */
    DL_SPI_CLOCK_DIVIDE_RATIO_2 = SPI_CLKDIV_RATIO_DIV_BY_2,
    /*! SPI源时钟分频比设置为3 */
    DL_SPI_CLOCK_DIVIDE_RATIO_3 = SPI_CLKDIV_RATIO_DIV_BY_3,
    /*! SPI源时钟分频比设置为4 */
    DL_SPI_CLOCK_DIVIDE_RATIO_4 = SPI_CLKDIV_RATIO_DIV_BY_4,
    /*! SPI源时钟分频比设置为5 */
    DL_SPI_CLOCK_DIVIDE_RATIO_5 = SPI_CLKDIV_RATIO_DIV_BY_5,
    /*! SPI源时钟分频比设置为6 */
    DL_SPI_CLOCK_DIVIDE_RATIO_6 = SPI_CLKDIV_RATIO_DIV_BY_6,
    /*! SPI源时钟分频比设置为7 */
    DL_SPI_CLOCK_DIVIDE_RATIO_7 = SPI_CLKDIV_RATIO_DIV_BY_7,
    /*! SPI源时钟分频比设置为8 */
    DL_SPI_CLOCK_DIVIDE_RATIO_8 = SPI_CLKDIV_RATIO_DIV_BY_8
} DL_SPI_CLOCK_DIVIDE_RATIO;

/*! @enum DL_SPI_CLOCK */
typedef enum {
    /*! 选择BUSCLK作为时钟源 */
    DL_SPI_CLOCK_BUSCLK = SPI_CLKSEL_SYSCLK_SEL_ENABLE,
    /*! 选择MFCLK作为时钟源 */
    DL_SPI_CLOCK_MFCLK = SPI_CLKSEL_MFCLK_SEL_ENABLE,
    /*! 选择LFCLK作为时钟源 */
    DL_SPI_CLOCK_LFCLK = SPI_CLKSEL_LFCLK_SEL_ENABLE
} DL_SPI_CLOCK;

/*!
 *  @brief  Configuration struct for @ref DL_SPI_init.
 */
typedef struct {
    /*! The controller/peripheral mode configuration. One of @ref DL_SPI_MODE */
    DL_SPI_MODE mode;

    /*!
     *  The frame format to use for data transfer. One of @ref
     *  DL_SPI_FRAME_FORMAT
     */
    DL_SPI_FRAME_FORMAT frameFormat;

    /*!
     *  The parity configuration to use for data transfer. One of @ref
     *  DL_SPI_PARITY.
     */
    DL_SPI_PARITY parity;

    /*! The size of the data transfer. One of @ref DL_SPI_DATA_SIZE */
    DL_SPI_DATA_SIZE dataSize;

    /*! The order of bits during data transfer. One of @ref DL_SPI_BIT_ORDER */
    DL_SPI_BIT_ORDER bitOrder;

    /*! The pin to use for chip select. Used in Controller or Peripheral modes
     *  with Motorola 4-Wire or TI Sync frame formats. One of
     *  @ref DL_SPI_CHIP_SELECT.
     */
    DL_SPI_CHIP_SELECT chipSelectPin;

} DL_SPI_Config;

/*!
 *  @brief  Configuration struct for @ref DL_SPI_setClockConfig.
 */
typedef struct {
    /*! Selects SPI module clock source @ref DL_SPI_CLOCK */
    DL_SPI_CLOCK clockSel;

    /*! Selects the divide ratio. One of @ref DL_SPI_CLOCK_DIVIDE_RATIO */
    DL_SPI_CLOCK_DIVIDE_RATIO divideRatio;

} DL_SPI_ClockConfig;

/**
 * @brief Configuration structure to backup SPI peripheral state before going
 *        to STOP/STANDBY mode. Used by @ref DL_SPI_saveConfiguration and
 *        @ref DL_SPI_restoreConfiguration
 */
typedef struct {
    /*! Combination of basic SPI control configurations that are
     *  compressed to a single word as they are stored in the SPI
     *  registers See @ref DL_SPI_init for how the peripheral control word 0
     *  is created. */
    uint32_t controlWord0;

    /*! Combination of basic SPI control configurations that are
     *  compressed to a single word as they are stored in the SPI
     *  registers. See @ref DL_SPI_init for how the peripheral control word 1
     *  is created. */
    uint32_t controlWord1;

    /*! Combination of serial clock divider and delayed sampling settings
     *  compressed to a single word as they are stored in the SPI registers. */
    uint32_t clockControl;

    /*! SPI module clock source. One of  @ref DL_SPI_CLOCK */
    uint32_t clockSel;

    /*! SPI clock divider. One of @ref DL_SPI_CLOCK_DIVIDE_RATIO */
    uint32_t divideRatio;

    /*! Combination of SPI interrupt FIFO level select configurations that are
     *  compressed to a single word as they are stored in the SPI
     *  registers. */
    uint32_t interruptFifoLevelSelectWord;

    /*! SPI interrupt status for EVENT0.
     *  Bitwise OR of @ref DL_SPI_INTERRUPT */
    uint32_t interruptMask0;

    /*! SPI interrupt status for EVENT1.
     *  Bitwise OR of @ref DL_SPI_DMA_INTERRUPT_RX */
    uint32_t interruptMask1;

    /*! SPI interrupt status for EVENT2.
     *  Bitwise OR of @ref DL_SPI_DMA_INTERRUPT_TX */
    uint32_t interruptMask2;

    /*! Boolean flag indicating whether or not a valid configuration structure
     *  exists. Should not be modified by the user. */
    bool backupRdy;
} DL_SPI_backupConfig;

/**
 *  @brief      初始化SPI外设
 *
 *  初始化SPI外设的所有常用可配置选项。调用此API后可以进行其他自定义配置。此API中不会启用SPI。
 *
 *  @param[in]  spi     指向外设寄存器覆盖的指针
 *  @param[in]  config  SPI外设的配置
 */
void DL_SPI_init(SPI_Regs *spi, const DL_SPI_Config *config);

/**
 * @brief 启用SPI的外设写使能（PWREN）寄存器
 *
 *  在软件可以配置任何外设寄存器之前，必须通过向外设的PWREN寄存器写入ENABLE位和适当的KEY值来启用外设本身。
 *
 *  @note 关于功耗节省，请参考 @ref DL_SPI_enable
 *
 * @param spi        指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_enablePower(SPI_Regs *spi)
{
    spi->GPRCM.PWREN = (SPI_PWREN_KEY_UNLOCK_W | SPI_PWREN_ENABLE_ENABLE);
}

/**
 * @brief 禁用SPI的外设写使能（PWREN）寄存器
 *
 *  当PWREN.ENABLE位被清除时，外设的寄存器不可用于读/写操作。
 *
 *  @note 此API不提供大量功耗节省。关于功耗节省，请参考 @ref DL_SPI_enable
 *
 * @param spi        指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_disablePower(SPI_Regs *spi)
{
    spi->GPRCM.PWREN = (SPI_PWREN_KEY_UNLOCK_W | SPI_PWREN_ENABLE_DISABLE);
}

/**
 * @brief 返回SPI的外设写使能（PWREN）寄存器是否已启用
 *
 *  在软件可以配置任何外设寄存器之前，必须通过向外设的PWREN寄存器写入ENABLE位和适当的KEY值来启用外设本身。
 *
 *  当PWREN.ENABLE位被清除时，外设的寄存器不可用于读/写操作。
 *
 * @param spi        指向外设寄存器覆盖的指针
 *
 * @return 如果外设寄存器访问已启用则返回true
 * @return 如果外设寄存器访问已禁用则返回false
 */
__STATIC_INLINE bool DL_SPI_isPowerEnabled(const SPI_Regs *spi)
{
    return (
        (spi->GPRCM.PWREN & SPI_PWREN_ENABLE_MASK) == SPI_PWREN_ENABLE_ENABLE);
}

/**
 * @brief 重置SPI外设
 *
 * @param spi        指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_reset(SPI_Regs *spi)
{
    spi->GPRCM.RSTCTL =
        (SPI_RSTCTL_KEY_UNLOCK_W | SPI_RSTCTL_RESETSTKYCLR_CLR |
            SPI_RSTCTL_RESETASSERT_ASSERT);
}

/**
 * @brief 返回SPI外设是否已重置
 *
 * @param spi        指向外设寄存器覆盖的指针
 *
 * @return 如果外设已重置则返回true
 * @return 如果外设未重置则返回false
 *
 */
__STATIC_INLINE bool DL_SPI_isReset(const SPI_Regs *spi)
{
    return ((spi->GPRCM.STAT & SPI_GPRCM_STAT_RESETSTKY_MASK) ==
            SPI_GPRCM_STAT_RESETSTKY_RESET);
}

/**
 *  @brief      启用SPI外设
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_enable(SPI_Regs *spi)
{
    spi->CTL1 |= SPI_CTL1_ENABLE_ENABLE;
}

/**
 *  @brief      检查SPI外设是否已启用
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     返回SPI的启用状态
 *
 *  @retval     true  SPI外设已启用
 *  @retval     false SPI外设已禁用
 */
__STATIC_INLINE bool DL_SPI_isEnabled(const SPI_Regs *spi)
{
    return ((spi->CTL1 & SPI_CTL1_ENABLE_MASK) == SPI_CTL1_ENABLE_ENABLE);
}

/**
 *  @brief      禁用SPI外设
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_disable(SPI_Regs *spi)
{
    spi->CTL1 &= ~(SPI_CTL1_ENABLE_MASK);
}

/**
 *  @brief      配置SPI源时钟
 *
 *  @param[in]  spi    指向外设寄存器覆盖的指针
 *  @param[in]  config 指向时钟配置结构体的指针
 *                       @ref DL_SPI_ClockConfig.
 */
void DL_SPI_setClockConfig(SPI_Regs *spi, const DL_SPI_ClockConfig *config);

/**
 *  @brief      获取SPI源时钟配置
 *
 *  @param[in]  spi    指向外设寄存器覆盖的指针
 *  @param[in]  config 指向时钟配置结构体的指针
 *                      @ref DL_SPI_ClockConfig.
 */
void DL_SPI_getClockConfig(const SPI_Regs *spi, DL_SPI_ClockConfig *config);

/**
 *  @brief      检查SPI是否正在传输
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     返回SPI的忙状态
 *
 *  @retval     true  SPI正在传输
 *  @retval     false SPI处于空闲状态
 */
__STATIC_INLINE bool DL_SPI_isBusy(const SPI_Regs *spi)
{
    return ((spi->STAT & SPI_STAT_BUSY_MASK) == SPI_STAT_BUSY_ACTIVE);
}

/**
 *  @brief      检查TX FIFO是否为空
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     返回TX FIFO的空状态
 *
 *  @retval     true  TX FIFO为空
 *  @retval     false TX FIFO不为空
 */
__STATIC_INLINE bool DL_SPI_isTXFIFOEmpty(const SPI_Regs *spi)
{
    return ((spi->STAT & SPI_STAT_TFE_MASK) == SPI_STAT_TFE_EMPTY);
}

/**
 *  @brief      检查TX FIFO是否已满
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     返回TX FIFO的满状态
 *
 *  @retval     true  TX FIFO已满
 *  @retval     false TX FIFO未满
 */
__STATIC_INLINE bool DL_SPI_isTXFIFOFull(const SPI_Regs *spi)
{
    return ((spi->STAT & SPI_STAT_TNF_MASK) == SPI_STAT_TNF_FULL);
}

/**
 *  @brief      检查RX FIFO是否为空
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     返回RX FIFO的空状态
 *
 *  @retval     true  RX FIFO为空
 *  @retval     false RX FIFO不为空
 */
__STATIC_INLINE bool DL_SPI_isRXFIFOEmpty(const SPI_Regs *spi)
{
    return ((spi->STAT & SPI_STAT_RFE_MASK) == SPI_STAT_RFE_EMPTY);
}

/**
 *  @brief      检查RX FIFO是否已满
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     返回RX FIFO的满状态
 *
 *  @retval     true  RX FIFO已满
 *  @retval     false RX FIFO未满
 */
__STATIC_INLINE bool DL_SPI_isRXFIFOFull(const SPI_Regs *spi)
{
    return ((spi->STAT & SPI_STAT_RNF_MASK) == SPI_STAT_RNF_FULL);
}

/**
 *  @brief      设置用于事务的奇偶校验配置
 *
 *  此API设置接收校验和发送校验的配置。包括使用哪个位，以及是否设置偶校验或奇校验。
 *
 *  要单独启用或禁用接收或发送校验，请参考下面列出的API。
 *
 *  @param[in]  spi     指向外设寄存器覆盖的指针
 *  @param[in]  parity  要使用的校验配置。@ref DL_SPI_PARITY之一。
 *
 *  @sa         DL_SPI_init
 *  @sa         DL_SPI_enableReceiveParity
 *  @sa         DL_SPI_disableReceiveParity
 *  @sa         DL_SPI_enableTransmitParity
 *  @sa         DL_SPI_disableTransmitParity
 */
__STATIC_INLINE void DL_SPI_setParity(SPI_Regs *spi, DL_SPI_PARITY parity)
{
    DL_Common_updateReg(&spi->CTL1, (uint32_t) parity,
        (SPI_CTL1_PREN_MASK | SPI_CTL1_PTEN_MASK | SPI_CTL1_PES_MASK));
}

/**
 *  @brief      获取当前的接收和发送校验配置
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     当前使用的校验配置
 *
 *  @retval     @ref DL_SPI_PARITY之一
 */
__STATIC_INLINE DL_SPI_PARITY DL_SPI_getParity(const SPI_Regs *spi)
{
    uint32_t parity = spi->CTL1 & (SPI_CTL1_PES_MASK | SPI_CTL1_PREN_MASK |
                                      SPI_CTL1_PTEN_MASK);

    return (DL_SPI_PARITY)(parity);
}

/**
 *  @brief      启用接收校验
 *
 *  此API仅启用接收校验，不配置使用的校验模式。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @sa         DL_SPI_setParity
 */
__STATIC_INLINE void DL_SPI_enableReceiveParity(SPI_Regs *spi)
{
    spi->CTL1 |= SPI_CTL1_PREN_ENABLE;
}

/**
 *  @brief      禁用接收校验
 *
 *  此API仅禁用接收校验，不配置使用的校验模式。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @sa         DL_SPI_setParity
 */
__STATIC_INLINE void DL_SPI_disableReceiveParity(SPI_Regs *spi)
{
    spi->CTL1 &= ~(SPI_CTL1_PREN_MASK);
}

/**
 *  @brief      检查接收校验是否已启用
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     接收校验是否已启用
 *
 *  @retval     true   接收校验已启用
 *  @retval     false  接收校验已禁用
 */
__STATIC_INLINE bool DL_SPI_isReceiveParityEnabled(const SPI_Regs *spi)
{
    return ((spi->CTL1 & SPI_CTL1_PREN_MASK) == SPI_CTL1_PREN_ENABLE);
}

/**
 *  @brief      启用发送校验
 *
 *  此API仅启用发送校验，不配置使用的校验模式。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @sa         DL_SPI_setParity
 */
__STATIC_INLINE void DL_SPI_enableTransmitParity(SPI_Regs *spi)
{
    spi->CTL1 |= SPI_CTL1_PTEN_ENABLE;
}

/**
 *  @brief      禁用发送校验
 *
 *  此API仅禁用发送校验，不配置使用的校验模式。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @sa         DL_SPI_setParity
 */
__STATIC_INLINE void DL_SPI_disableTransmitParity(SPI_Regs *spi)
{
    spi->CTL1 &= ~(SPI_CTL1_PTEN_MASK);
}

/**
 *  @brief      检查发送校验是否已启用
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     发送校验是否已启用
 *
 *  @retval     true   发送校验已启用
 *  @retval     false  发送校验已禁用
 */
__STATIC_INLINE bool DL_SPI_isTransmitParityEnabled(const SPI_Regs *spi)
{
    return ((spi->CTL1 & SPI_CTL1_PTEN_MASK) == SPI_CTL1_PTEN_ENABLE);
}

/**
 *  @brief      设置要使用的帧格式
 *
 *  配置用于事务的帧格式。如果使用片选，必须使用Motorola 4线帧格式之一。
 *
 *  @param[in]  spi          指向外设寄存器覆盖的指针
 *  @param[in]  frameFormat  要使用的帧格式。@ref DL_SPI_FRAME_FORMAT之一。
 *
 *  @sa         DL_SPI_init
 */
__STATIC_INLINE void DL_SPI_setFrameFormat(
    SPI_Regs *spi, DL_SPI_FRAME_FORMAT frameFormat)
{
    DL_Common_updateReg(&spi->CTL0, (uint32_t) frameFormat,
        (SPI_CTL0_FRF_MASK | SPI_CTL0_SPO_MASK | SPI_CTL0_SPH_MASK));
}

/**
 *  @brief      获取帧格式配置
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     当前使用的帧格式
 *
 *  @retval     @ref DL_SPI_FRAME_FORMAT之一
 */
__STATIC_INLINE DL_SPI_FRAME_FORMAT DL_SPI_getFrameFormat(const SPI_Regs *spi)
{
    uint32_t frameFormat = spi->CTL0 & (SPI_CTL0_FRF_MASK | SPI_CTL0_SPO_MASK |
                                           SPI_CTL0_SPH_MASK);

    return (DL_SPI_FRAME_FORMAT)(frameFormat);
}

/**
 *  @brief      设置传输的数据大小
 *
 *  @param[in]  spi       指向外设寄存器覆盖的指针
 *  @param[in]  dataSize  传输中使用的位数。@ref DL_SPI_DATA_SIZE之一
 *
 *  @sa         DL_SPI_init
 */
__STATIC_INLINE void DL_SPI_setDataSize(
    SPI_Regs *spi, DL_SPI_DATA_SIZE dataSize)
{
    DL_Common_updateReg(&spi->CTL0, (uint32_t) dataSize, SPI_CTL0_DSS_MASK);
}

/**
 *  @brief      获取配置的传输数据大小
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     当前配置的传输数据大小
 *
 *  @retval     @ref DL_SPI_DATA_SIZE之一
 */
__STATIC_INLINE DL_SPI_DATA_SIZE DL_SPI_getDataSize(const SPI_Regs *spi)
{
    uint32_t dataSize = spi->CTL0 & SPI_CTL0_DSS_MASK;

    return (DL_SPI_DATA_SIZE)(dataSize);
}

/**
 *  @brief      设置设备是否应处于控制器/外设模式
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *  @param[in]  mode  要配置SPI的模式。@ref DL_SPI_MODE之一。
 *
 *  @sa         DL_SPI_init
 */
__STATIC_INLINE void DL_SPI_setMode(SPI_Regs *spi, DL_SPI_MODE mode)
{
    DL_Common_updateReg(&spi->CTL1, (uint32_t) mode, SPI_CTL1_CP_MASK);
}

/**
 *  @brief      获取SPI的当前模式（控制器/外设）
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     SPI的当前配置模式（控制器/外设）
 *
 *  @retval     @ref DL_SPI_MODE之一。
 */
__STATIC_INLINE DL_SPI_MODE DL_SPI_getMode(const SPI_Regs *spi)
{
    uint32_t mode = spi->CTL1 & SPI_CTL1_CP_MASK;

    return (DL_SPI_MODE)(mode);
}

/**
 *  @brief      设置传输时使用的位顺序
 *
 *  @param[in]  spi       指向外设寄存器覆盖的指针
 *  @param[in]  bitOrder  传输期间位发送的顺序。@ref DL_SPI_BIT_ORDER之一。
 *
 *  @sa         DL_SPI_init
 */
__STATIC_INLINE void DL_SPI_setBitOrder(
    SPI_Regs *spi, DL_SPI_BIT_ORDER bitOrder)
{
    DL_Common_updateReg(&spi->CTL1, (uint32_t) bitOrder, SPI_CTL1_MSB_MASK);
}

/**
 *  @brief      获取当前用于传输的位顺序
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     当前配置的位顺序
 *
 *  @retval     @ref DL_SPI_BIT_ORDER之一。
 */
__STATIC_INLINE DL_SPI_BIT_ORDER DL_SPI_getBitOrder(const SPI_Regs *spi)
{
    uint32_t bitOrder = spi->CTL1 & SPI_CTL1_MSB_MASK;

    return (DL_SPI_BIT_ORDER)(bitOrder);
}

/**
 *  @brief      启用回环模式
 *
 *  启用回环模式。启用后，发送串行移位器的输出在内部连接到接收串行移位器的输入。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_enableLoopbackMode(SPI_Regs *spi)
{
    spi->CTL1 |= SPI_CTL1_LBM_ENABLE;
}

/**
 *  @brief      禁用回环模式
 *
 *  禁用回环模式。禁用后，发送串行移位器和接收串行移位器在内部不连接。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_disableLoopbackMode(SPI_Regs *spi)
{
    spi->CTL1 &= ~(SPI_CTL1_LBM_MASK);
}

/**
 *  @brief      检查回环模式是否已启用
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     回环模式的状态
 *
 *  @retval     true 如果回环模式已启用
 *  @retval     false 如果回环模式已禁用
 */
__STATIC_INLINE bool DL_SPI_isLoopbackModeEnabled(const SPI_Regs *spi)
{
    return ((spi->CTL1 & SPI_CTL1_LBM_MASK) == SPI_CTL1_LBM_ENABLE);
}

/**
 *  @brief      设置重复发送的计数器
 *
 *  重复发送允许您多次发送相同的数据。这本质上与将数据重复写入发送缓冲区相同。
 *
 *  @param[in]  spi         指向外设寄存器覆盖的指针
 *  @param[in]  numRepeats  重复传输的次数。应为0-255之间的值。
 *                          @arg 0 禁用重复传输
 *                          @arg 1-255 重复该次数。因此总共会发送numRepeats + 1次。
 */
__STATIC_INLINE void DL_SPI_setRepeatTransmit(
    SPI_Regs *spi, uint32_t numRepeats)
{
    DL_Common_updateReg(&spi->CTL1, numRepeats << SPI_CTL1_REPEATTX_OFS,
        SPI_CTL1_REPEATTX_MASK);
}

/**
 *  @brief      获取重复发送的计数器
 *
 *  重复发送允许您多次发送相同的数据。这本质上与将数据重复写入发送缓冲区相同。
 *
 *  @param[in]  spi    指向外设寄存器覆盖的指针
 *
 *  @return     用于传输的重复次数
 *
 *  @retval     0      重复传输已禁用
 *  @retval     1-255  重复该次数。
 */
__STATIC_INLINE uint32_t DL_SPI_getRepeatTransmit(const SPI_Regs *spi)
{
    return ((spi->CTL1 & SPI_CTL1_REPEATTX_MASK) >> SPI_CTL1_REPEATTX_OFS);
}

/**
 *  @brief      启用外设的片选数据对齐
 *
 *  启用后，当片选变为非活动状态时，接收位计数器会自动清除。这有助于外设在时钟线上有干扰或故障时，或在初始化期间重新与控制器同步。
 *
 *  这仅在外设模式下相关。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_enablePeripheralAlignDataOnChipSelect(
    SPI_Regs *spi)
{
    spi->CTL0 |= SPI_CTL0_CSCLR_ENABLE;
}

/**
 *  @brief      禁用外设的片选数据对齐
 *
 *  禁用后，当片选变为非活动状态时，接收位计数器不会自动清除。
 *
 *  这仅在外设模式下相关。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_disablePeripheralAlignDataOnChipSelect(
    SPI_Regs *spi)
{
    spi->CTL0 &= ~(SPI_CTL0_CSCLR_MASK);
}

/**
 *  @brief      检查外设的片选数据对齐是否已启用
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     片选数据对齐的状态
 *
 *  @retval     true   片选数据对齐已启用
 *  @retval     false  片选数据对齐已禁用
 */
__STATIC_INLINE bool DL_SPI_isPeripheralAlignDataOnChipSelectEnabled(
    const SPI_Regs *spi)
{
    return ((spi->CTL0 & SPI_CTL0_CSCLR_MASK) == SPI_CTL0_CSCLR_ENABLE);
}

/**
 *  @brief      启用打包功能
 *
 *  启用后，RX FIFO的两个条目会作为32位值返回。当读取TX FIFO时，如果上次对该字段的写入是32位写入，将返回这些32位。当写入TX FIFO时，32位写入会被写为一个FIFO条目。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_enablePacking(SPI_Regs *spi)
{
    spi->CTL0 |= SPI_CTL0_PACKEN_ENABLED;
}

/**
 *  @brief      禁用打包功能
 *
 *  禁用后，RX FIFO的1个条目会作为16位值返回。当读取TX FIFO时，如果上次对该字段的写入是16位写入，将返回这些16位。当写入TX FIFO时，32位写入会被写为两个FIFO条目。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_disablePacking(SPI_Regs *spi)
{
    spi->CTL0 &= ~(SPI_CTL0_PACKEN_MASK);
}

/**
 *  @brief      检查打包功能是否已启用
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     打包功能是否已启用
 *
 *  @retval     true   打包功能已启用
 *  @retval     false  打包功能已禁用
 */
__STATIC_INLINE bool DL_SPI_isPackingEnabled(const SPI_Regs *spi)
{
    return ((spi->CTL0 & SPI_CTL0_PACKEN_MASK) == SPI_CTL0_PACKEN_ENABLED);
}

/**
 *  @brief      设置用于控制器或外设模式的片选
 *
 *  选择用于数据传输的片选。用户必须确保使用@ref DL_SPI_init或@ref DL_SPI_setFrameFormat选择了4线帧格式之一。
 *
 *  此API适用于控制器和外设模式，应用程序可以通过调用此API更改片选。
 *
 *  @param[in]  spi         指向外设寄存器覆盖的指针
 *  @param[in]  chipSelect  要使用的片选引脚。@ref DL_SPI_CHIP_SELECT之一。
 */
__STATIC_INLINE void DL_SPI_setChipSelect(
    SPI_Regs *spi, DL_SPI_CHIP_SELECT chipSelect)
{
    DL_Common_updateReg(
        &spi->CTL0, (uint32_t) chipSelect, SPI_CTL0_CSSEL_MASK);
}

/**
 *  @brief      获取用于控制器或外设模式的片选
 *
 *  此API适用于控制器和外设模式。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     当前用于数据传输的片选
 *
 *  @retval     @ref DL_SPI_CHIP_SELECT之一。
 */
__STATIC_INLINE DL_SPI_CHIP_SELECT DL_SPI_getChipSelect(const SPI_Regs *spi)
{
    uint32_t chipSelect = spi->CTL0 & SPI_CTL0_CSSEL_MASK;

    return (DL_SPI_CHIP_SELECT)(chipSelect);
}

/**
 *  @brief      设置外设接收超时
 *
 *  设置接收超时发生前的时钟周期数。
 *
 *  这仅在外设模式下相关。
 *
 *  @param[in]  spi      指向外设寄存器覆盖的指针
 *  @param[in]  timeout  接收超时发生前的时钟周期数。必须在0-63之间。
 *                       @arg 0 禁用接收超时
 *                       @arg 1-63 超时前的时钟周期数
 */
__STATIC_INLINE void DL_SPI_setPeripheralReceiveTimeout(
    SPI_Regs *spi, uint32_t timeout)
{
    DL_Common_updateReg(&spi->CTL1, timeout << SPI_CTL1_RXTIMEOUT_OFS,
        SPI_CTL1_RXTIMEOUT_MASK);
}

/**
 *  @brief      获取外设接收超时
 *
 *  这仅在外设模式下相关。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *
 *  @return     用于外设超时的当前周期数
 *
 *  @retval     0     表示接收超时已禁用
 *  @retval     1-63  接收超时发生前的时钟周期数
 */
__STATIC_INLINE uint32_t DL_SPI_getPeripheralReceiveTimeout(
    const SPI_Regs *spi)
{
    return ((spi->CTL1 & SPI_CTL1_RXTIMEOUT_MASK) >> SPI_CTL1_RXTIMEOUT_OFS);
}

/**
 *  @brief      配置命令/数据模式
 *
 *  命令/数据模式允许硬件控制C/D引脚以指示正在传输的数据是命令还是仅数据。C/D引脚对于命令为低电平，对于数据为高电平。
 *
 *  您可以通过传入@ref DL_SPI_CD_MODE_DATA或@ref DL_SPI_CD_MODE_COMMAND来手动设置C/D引脚。您还可以通过传入命令传输的字节数来让C/D引脚自动切换。C/D引脚会保持低电平，直到发送完指定数量的字节，然后自动切换到高电平。
 *
 *  这仅在控制器模式下相关。
 *
 *  @param[in]  spi    指向外设寄存器覆盖的指针
 *  @param[in]  config 命令/数据模式的配置。以下之一：
 *                     @arg DL_SPI_CD_MODE_DATA
 *                     @arg DL_SPI_CD_MODE_COMMAND
 *                     @arg 1-14之间的值，表示应该作为命令发送的字节数
 */
__STATIC_INLINE void DL_SPI_setControllerCommandDataModeConfig(
    SPI_Regs *spi, uint32_t config)
{
    DL_Common_updateReg(
        &spi->CTL1, config << SPI_CTL1_CDMODE_OFS, SPI_CTL1_CDMODE_MASK);
}

/**
 *  @brief      获取命令/数据模式配置
 *
 *  返回命令/数据模式的当前配置。当使用自动C/D引脚时，读回的值将是剩余要发送的命令字节数。
 *
 *  @param[in]  spi    指向外设寄存器覆盖的指针
 *
 *  @return     命令/数据模式的当前配置
 *
 *  @retval     DL_SPI_CD_MODE_DATA    如果处于手动数据模式
 *  @retval     DL_SPI_CD_MODE_COMMAND 如果处于手动命令模式
 *  @retval     1-14 表示仍需要发送的命令字节数
 */
__STATIC_INLINE uint32_t DL_SPI_getControllerCommandDataModeConfig(
    const SPI_Regs *spi)
{
    return ((spi->CTL1 & SPI_CTL1_CDMODE_MASK) >> SPI_CTL1_CDMODE_OFS);
}

/**
 *  @brief      启用命令/数据模式
 *
 *  当命令/数据模式启用时，C/D引脚用于在事务期间区分命令和数据。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @sa         DL_SPI_setControllerCommandDataModeConfig
 */
__STATIC_INLINE void DL_SPI_enableControllerCommandDataMode(SPI_Regs *spi)
{
    spi->CTL1 |= SPI_CTL1_CDENABLE_ENABLE;
}

/**
 *  @brief      禁用命令/数据模式
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_disableControllerCommandDataMode(SPI_Regs *spi)
{
    spi->CTL1 &= ~(SPI_CTL1_CDENABLE_MASK);
}

/**
 *  @brief      检查命令/数据模式是否已启用
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     命令/数据模式的状态
 *
 *  @retval     true  命令/数据模式已启用
 *  @retval     false 命令/数据模式已禁用
 */
__STATIC_INLINE bool DL_SPI_isControllerCommandDataModeEnabled(
    const SPI_Regs *spi)
{
    return ((spi->CTL1 & SPI_CTL1_CDENABLE_MASK) == SPI_CTL1_CDENABLE_ENABLE);
}

/**
 *  @brief      启用外设数据输出
 *
 *  当外设数据输出启用时，外设可以驱动POCI输出引脚。如果所有外设的RXD线都连接在一起，而控制器试图向所有外设广播消息，同时确保只有一个外设将数据驱动到其串行输出线上，这会导致问题。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_enablePeripheralDataOutput(SPI_Regs *spi)
{
    spi->CTL1 &= ~(SPI_CTL1_POD_MASK);
}

/**
 *  @brief      禁用外设数据输出
 *
 *  当外设数据输出禁用时，外设不能驱动POCI输出引脚。这允许多个RXD线连接在一起的外设接收来自控制器的广播消息。
 *
 *  这仅在外设模式下相关。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_disablePeripheralDataOutput(SPI_Regs *spi)
{
    spi->CTL1 |= SPI_CTL1_POD_ENABLE;
}

/**
 *  @brief      检查外设数据输出是否已启用
 *
 *  这仅在外设模式下相关。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     外设数据输出是否已启用
 *
 *  @retval     true 如果外设数据输出已启用
 *  @retval     false 如果外设数据输出已禁用
 */
__STATIC_INLINE bool DL_SPI_isPeripheralDataOutputEnabled(const SPI_Regs *spi)
{
    return ((spi->CTL1 & SPI_CTL1_POD_MASK) == SPI_CTL1_POD_DISABLE);
}

/**
 *  @brief      设置延迟采样
 *
 *  仅在控制器模式下，输入引脚上的数据将被定义的SPI时钟周期延迟采样。延迟可以以SPI输入时钟步骤为单位进行调整。允许的最大延迟不应超过一个数据帧的长度。
 *
 *  @param[in]  spi     指向外设寄存器覆盖的指针
 *  @param[in]  delay   输入引脚上延迟采样的SPI时钟周期数。值在0-15之间。
 */
__STATIC_INLINE void DL_SPI_setDelayedSampling(SPI_Regs *spi, uint32_t delay)
{
    DL_Common_updateReg(&spi->CLKCTL, delay << SPI_CLKCTL_DSAMPLE_OFS,
        SPI_CLKCTL_DSAMPLE_MASK);
}

/**
 *  @brief      获取延迟采样
 *
 *  仅在控制器模式下，输入引脚上的数据将被定义的SPI时钟周期延迟采样。延迟可以以SPI输入时钟步骤为单位进行调整。允许的最大延迟不应超过一个数据帧的长度。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     输入引脚上延迟采样的SPI时钟周期数。
 *
 *  @retval     0 - 15. 延迟采样的SPI时钟周期数。
 */
__STATIC_INLINE uint32_t DL_SPI_getDelayedSampling(const SPI_Regs *spi)
{
    return ((spi->CLKCTL & SPI_CLKCTL_DSAMPLE_MASK) >> SPI_CLKCTL_DSAMPLE_OFS);
}

/**
 *  @brief      设置RX和TX FIFO中断阈值级别
 *
 *  选择接收和发送FIFO中断的阈值。中断是基于通过某个级别的转换生成的，而不是基于级别本身。也就是说，当填充级别通过触发级别时会生成中断。
 *
 *  例如，如果触发级别设置为中间标记，当FIFO变为半空/半满时会触发中断。
 *
 *  复位后，FIFO被触发在中间标记处中断。
 *
 *  @param[in]  spi             指向外设寄存器覆盖的指针
 *  @param[in]  rxThreshold     @ref DL_SPI_RX_FIFO_LEVEL之一
 *  @param[in]  txThreshold     @ref DL_SPI_TX_FIFO_LEVEL之一
 *
 */
__STATIC_INLINE void DL_SPI_setFIFOThreshold(SPI_Regs *spi,
    DL_SPI_RX_FIFO_LEVEL rxThreshold, DL_SPI_TX_FIFO_LEVEL txThreshold)
{
    DL_Common_updateReg(&spi->IFLS,
        (uint32_t) rxThreshold | (uint32_t) txThreshold,
        SPI_IFLS_RXIFLSEL_MASK | SPI_IFLS_TXIFLSEL_MASK);
}

/**
 *  @brief      获取TX FIFO中断阈值级别
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     TX FIFO中断阈值级别
 *
 *  @retval     @ref DL_SPI_TX_FIFO_LEVEL之一
 */
__STATIC_INLINE DL_SPI_TX_FIFO_LEVEL DL_SPI_getTXFIFOThreshold(
    const SPI_Regs *spi)
{
    uint32_t txThreshold = spi->IFLS & SPI_IFLS_TXIFLSEL_MASK;

    return (DL_SPI_TX_FIFO_LEVEL)(txThreshold);
}

/**
 *  @brief      获取RX FIFO中断阈值级别
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     RX FIFO中断阈值级别
 *
 *  @retval     @ref DL_SPI_RX_FIFO_LEVEL之一
 */
__STATIC_INLINE DL_SPI_RX_FIFO_LEVEL DL_SPI_getRXFIFOThreshold(
    const SPI_Regs *spi)
{
    uint32_t rxThreshold = spi->IFLS & SPI_IFLS_RXIFLSEL_MASK;

    return (DL_SPI_RX_FIFO_LEVEL)(rxThreshold);
}

/**
 *  @brief      设置SPI比特率串行时钟分频器（SCR）
 *
 * SPI包含一个可编程的比特率时钟分频器和预分频器，用于生成串行输出时钟。支持的比特率最高可达FUNCCLK/2。功能时钟选择取决于特定设备，请参考设备数据手册和PMU/时钟部分。
 *
 * SPI输出比特率通过以下公式计算：
 * SPI比特率 = (SPI功能时钟) / ((1 + SCR)*2)
 * 根据此公式，SCR可计算为：
 * SCR = (SPI功能时钟) / ((2 * SPI比特率) - 1)
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *  @param[in]  SCR  SPI串行时钟分频器。值在0-1023之间。
 */
__STATIC_INLINE void DL_SPI_setBitRateSerialClockDivider(
    SPI_Regs *spi, uint32_t SCR)
{
    DL_Common_updateReg(&spi->CLKCTL, SCR, SPI_CLKCTL_SCR_MASK);
}

/**
 *  @brief      获取SPI比特率串行时钟分频器（SCR）
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @return     当前的比特率串行时钟分频器
 *
 *  @retval     SPI SCR。值从0-1023
 */
__STATIC_INLINE uint32_t DL_SPI_getBitRateSerialClockDivider(
    const SPI_Regs *spi)
{
    return (spi->CLKCTL & SPI_CLKCTL_SCR_MASK);
}

/**
 *  @brief      将8位数据写入TX FIFO进行发送
 *
 *  将数据放入TX FIFO而不检查其状态。如果已经确定TX FIFO有空间进行写入，则使用此函数。有关其他发送选项，请参阅相关API。
 *
 *  可用于任何小于或等于8位的数据传输。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *  @param[in]  data  要发送的数据
 *
 *  @sa         DL_SPI_transmitDataBlocking8
 *  @sa         DL_SPI_transmitDataCheck8
 */
__STATIC_INLINE void DL_SPI_transmitData8(SPI_Regs *spi, uint8_t data)
{
    spi->TXDATA = data;
}

/**
 *  @brief      将16位数据写入TX FIFO进行发送
 *
 *  将数据放入TX FIFO而不检查其状态。如果已经确定TX FIFO有空间进行写入，则使用此函数。有关其他发送选项，请参阅相关API。
 *
 *  可用于任何小于或等于16位的数据传输。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *  @param[in]  data  要发送的数据
 *
 *  @sa         DL_SPI_transmitDataBlocking16
 *  @sa         DL_SPI_transmitDataCheck16
 */
__STATIC_INLINE void DL_SPI_transmitData16(SPI_Regs *spi, uint16_t data)
{
    spi->TXDATA = data;
}

/**
 *  @brief      将32位数据写入TX FIFO进行发送
 *
 *  将数据放入TX FIFO而不检查其状态。如果已经确定TX FIFO有空间进行写入，则使用此函数。有关其他发送选项，请参阅相关API。
 *
 *  可用于任何小于或等于32位的数据传输。
 *
 *  注意：如果在调用此API之前通过调用@ref DL_SPI_enablePacking启用了打包功能，则32位写入将被写为一个FIFO条目。如果打包功能被禁用，则32位写入将被写为两个FIFO条目。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *  @param[in]  data  要发送的数据
 *
 *  @sa         DL_SPI_enablePacking
 *  @sa         DL_SPI_transmitDataBlocking32
 *  @sa         DL_SPI_transmitDataCheck32
 */
__STATIC_INLINE void DL_SPI_transmitData32(SPI_Regs *spi, uint32_t data)
{
    spi->TXDATA = data;
}

/**
 *  @brief      从RX FIFO读取8位数据
 *
 *  从RX FIFO读取数据而不检查其状态。如果已经确定RX FIFO有可用数据，则使用此函数。有关其他接收选项，请参阅相关API。
 *
 *  可用于任何小于或等于8位的数据传输。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *
 *  @return     RX FIFO中的数据
 *
 *  @sa         DL_SPI_receiveDataBlocking8
 *  @sa         DL_SPI_receiveDataCheck8
 */
__STATIC_INLINE uint8_t DL_SPI_receiveData8(const SPI_Regs *spi)
{
    return ((uint8_t)(spi->RXDATA));
}

/**
 *  @brief      从RX FIFO读取16位数据
 *
 *  从RX FIFO读取数据而不检查其状态。如果已经确定RX FIFO有可用数据，则使用此函数。有关其他接收选项，请参阅相关API。
 *
 *  可用于任何小于或等于16位的数据传输。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *
 *  @return     RX FIFO中的数据
 *
 *  @sa         DL_SPI_receiveDataBlocking16
 *  @sa         DL_SPI_receiveDataCheck16
 */
__STATIC_INLINE uint16_t DL_SPI_receiveData16(const SPI_Regs *spi)
{
    return ((uint16_t)(spi->RXDATA));
}

/**
 *  @brief      从RX FIFO读取32位数据
 *
 *  从RX FIFO读取数据而不检查其状态。如果已经确定RX FIFO有可用数据，则使用此函数。有关其他接收选项，请参阅相关API。
 *
 *  可用于任何小于或等于32位的数据传输。
 *
 *  注意：需要在调用此API之前通过调用@ref DL_SPI_enablePacking启用打包功能。当打包功能启用时，RX FIFO的两个条目会作为32位值返回。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *
 *  @return     RX FIFO中的数据
 *
 *  @pre         DL_SPI_enablePacking
 *
 *  @sa         DL_SPI_receiveDataBlocking32
 *  @sa         DL_SPI_receiveDataCheck32
 */
__STATIC_INLINE uint32_t DL_SPI_receiveData32(const SPI_Regs *spi)
{
    return spi->RXDATA;
}

/**
 *  @brief      启用SPI中断
 *
 *  @param[in]  spi            指向外设寄存器覆盖的指针
 *  @param[in]  interruptMask  要启用的中断的位掩码。@ref DL_SPI_INTERRUPT的按位或。
 */
__STATIC_INLINE void DL_SPI_enableInterrupt(
    SPI_Regs *spi, uint32_t interruptMask)
{
    spi->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      禁用SPI中断
 *
 *  @param[in]  spi            指向外设寄存器覆盖的指针
 *  @param[in]  interruptMask  要禁用的中断的位掩码。@ref DL_SPI_INTERRUPT的按位或。
 */
__STATIC_INLINE void DL_SPI_disableInterrupt(
    SPI_Regs *spi, uint32_t interruptMask)
{
    spi->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      检查哪些SPI中断已启用
 *
 *  @param[in]  spi            指向外设寄存器覆盖的指针
 *  @param[in]  interruptMask  要检查的中断的位掩码。@ref DL_SPI_INTERRUPT的按位或。
 *
 *  @return     请求的哪些SPI中断已启用
 *
 *  @retval     @ref DL_SPI_INTERRUPT值的按位或
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledInterrupts(
    const SPI_Regs *spi, uint32_t interruptMask)
{
    return (spi->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      检查已启用的SPI中断的中断标志
 *
 *  检查之前启用的任何SPI中断是否待处理。
 *
 *  @param[in]  spi            指向外设寄存器覆盖的指针
 *  @param[in]  interruptMask  要检查的中断的位掩码。@ref DL_SPI_INTERRUPT的按位或。
 *
 *  @return     请求的哪些SPI中断待处理
 *
 *  @retval     @ref DL_SPI_INTERRUPT值的按位或
 *
 *  @sa         DL_SPI_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledInterruptStatus(
    const SPI_Regs *spi, uint32_t interruptMask)
{
    return (spi->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      检查任何SPI中断的中断标志
 *
 *  检查任何SPI中断是否待处理。中断不必事先启用。
 *
 *  @param[in]  spi            指向外设寄存器覆盖的指针
 *  @param[in]  interruptMask  要检查的中断的位掩码。@ref DL_SPI_INTERRUPT的按位或。
 *
 *  @return     请求的哪些SPI中断待处理
 *
 *  @retval     @ref DL_SPI_INTERRUPT值的按位或
 */
__STATIC_INLINE uint32_t DL_SPI_getRawInterruptStatus(
    const SPI_Regs *spi, uint32_t interruptMask)
{
    return (spi->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      获取最高优先级的待处理SPI中断
 *
 *  检查任何SPI中断是否待处理。中断不必事先启用。
 *
 *  @param[in]  spi            指向外设寄存器覆盖的指针
 *
 *  @return     最高优先级的待处理SPI中断。@ref DL_SPI_IIDX之一
 */
__STATIC_INLINE DL_SPI_IIDX DL_SPI_getPendingInterrupt(const SPI_Regs *spi)
{
    return ((DL_SPI_IIDX) spi->CPU_INT.IIDX);
}

/**
 *  @brief      清除待处理的SPI中断
 *
 *  @param[in]  spi            指向外设寄存器覆盖的指针
 *  @param[in]  interruptMask  要清除的中断的位掩码。@ref DL_SPI_INTERRUPT的按位或。
 */
__STATIC_INLINE void DL_SPI_clearInterruptStatus(
    SPI_Regs *spi, uint32_t interruptMask)
{
    spi->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief      阻塞以确保在发送数据前发送准备就绪
 *
 *  将数据放入TX FIFO，无限期等待直到TX FIFO中有空间。放置数据后，无限期等待直到数据发送完毕且SPI不再忙。请注意，如果在调用此API时TX FIFO中已存在数据，它将阻塞直到所有数据发送完毕。有关其他发送选项，请参阅相关API。
 *
 *  可用于任何小于或等于8位的数据传输。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *  @param[in]  data  要发送的数据
 *
 *  @sa         DL_SPI_transmitData8
 *  @sa         DL_SPI_transmitDataCheck8
 */
void DL_SPI_transmitDataBlocking8(SPI_Regs *spi, uint8_t data);

/**
 *  @brief      阻塞以确保在发送数据前发送准备就绪
 *
 *  将数据放入TX FIFO，无限期等待直到TX FIFO中有空间。放置数据后，无限期等待直到数据发送完毕且SPI不再忙。请注意，如果在调用此API时TX FIFO中已存在数据，它将阻塞直到所有数据发送完毕。有关其他发送选项，请参阅相关API。
 *
 *  可用于任何小于或等于16位的数据传输。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *  @param[in]  data  要发送的数据
 *
 *  @sa         DL_SPI_transmitData16
 *  @sa         DL_SPI_transmitDataCheck16
 */
void DL_SPI_transmitDataBlocking16(SPI_Regs *spi, uint16_t data);

/**
 *  @brief      阻塞以确保在发送数据前发送准备就绪
 *
 *  将数据放入TX FIFO，无限期等待直到TX FIFO中有空间。放置数据后，无限期等待直到数据发送完毕且SPI不再忙。请注意，如果在调用此API时TX FIFO中已存在数据，它将阻塞直到所有数据发送完毕。有关其他发送选项，请参阅相关API。
 *
 *  可用于任何小于或等于32位的数据传输。
 *
 *  注意：如果在调用此API之前通过调用@ref DL_SPI_enablePacking启用了打包功能，则32位写入将被写为一个FIFO条目。如果打包功能被禁用，则32位写入将被写为两个FIFO条目。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *  @param[in]  data  要发送的数据
 *
 *  @sa         DL_SPI_enablePacking
 *  @sa         DL_SPI_transmitData32
 *  @sa         DL_SPI_transmitDataCheck32
 */
void DL_SPI_transmitDataBlocking32(SPI_Regs *spi, uint32_t data);

/**
 *  @brief      阻塞以确保在读取数据前接收准备就绪
 *
 *  在阻塞确保RX FIFO非空后从RX FIFO读取数据。将无限期等待直到RX FIFO中有数据。有关其他接收选项，请参阅相关API。
 *
 *  可用于任何小于或等于8位的数据传输。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *
 *  @return     RX FIFO中的数据
 *
 *  @sa         DL_SPI_transmitData8
 *  @sa         DL_SPI_transmitDataCheck8
 */
uint8_t DL_SPI_receiveDataBlocking8(const SPI_Regs *spi);

/**
 *  @brief      阻塞以确保在读取数据前接收准备就绪
 *
 *  在阻塞确保RX FIFO非空后从RX FIFO读取数据。将无限期等待直到RX FIFO中有数据。有关其他接收选项，请参阅相关API。
 *
 *  可用于任何小于或等于16位的数据传输。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *
 *  @return     RX FIFO中的数据
 *
 *  @sa         DL_SPI_transmitData16
 *  @sa         DL_SPI_transmitDataCheck16
 */
uint16_t DL_SPI_receiveDataBlocking16(const SPI_Regs *spi);

/**
 *  @brief      阻塞以确保在读取数据前接收准备就绪
 *
 *  在阻塞确保RX FIFO非空后从RX FIFO读取数据。将无限期等待直到RX FIFO中有数据。有关其他接收选项，请参阅相关API。
 *
 *  可用于任何小于或等于32位的数据传输。
 *
 *  注意：需要在调用此API之前通过调用@ref DL_SPI_enablePacking启用打包功能。当打包功能启用时，RX FIFO的两个条目会作为32位值返回。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *
 *  @return     RX FIFO中的数据
 *
 *  @pre         DL_SPI_enablePacking
 *
 *  @sa         DL_SPI_transmitData32
 *  @sa         DL_SPI_transmitDataCheck32
 */
uint32_t DL_SPI_receiveDataBlocking32(const SPI_Regs *spi);

/**
 *  @brief      在尝试传输数据前检查TX FIFO
 *
 *  在尝试向FIFO添加新数据之前，检查TX FIFO是否已满。如果已满则立即退出，而不是尝试阻塞。有关其他发送选项，请参阅相关API。
 *
 *  可用于任何小于或等于8位的数据传输。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *  @param[in]  data  要发送的数据
 *
 *  @return     传输是否发生
 *
 *  @retval     true  如果数据已添加到TX FIFO
 *  @retval     false 如果TX FIFO已满且数据未添加
 *
 *  @sa         DL_SPI_transmitData8
 *  @sa         DL_SPI_transmitDataBlocking8
 */
bool DL_SPI_transmitDataCheck8(SPI_Regs *spi, uint8_t data);

/**
 *  @brief      在尝试传输数据前检查TX FIFO
 *
 *  在尝试向FIFO添加新数据之前，检查TX FIFO是否已满。如果已满则立即退出，而不是尝试阻塞。有关其他发送选项，请参阅相关API。
 *
 *  可用于任何小于或等于16位的数据传输。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *  @param[in]  data  要发送的数据
 *
 *  @return     传输是否发生
 *
 *  @retval     true  如果数据已添加到TX FIFO
 *  @retval     false 如果TX FIFO已满且数据未添加
 *
 *  @sa         DL_SPI_transmitData16
 *  @sa         DL_SPI_transmitDataBlocking16
 */
bool DL_SPI_transmitDataCheck16(SPI_Regs *spi, uint16_t data);

/**
 *  @brief      在尝试传输数据前检查TX FIFO
 *
 *  在尝试向FIFO添加新数据之前，检查TX FIFO是否已满。如果已满则立即退出，而不是尝试阻塞。有关其他发送选项，请参阅相关API。
 *
 *  可用于任何小于或等于32位的数据传输。
 *
 *  注意：如果在调用此API之前通过调用@ref DL_SPI_enablePacking启用了打包功能，则32位写入将被写为一个FIFO条目。如果打包功能被禁用，则32位写入将被写为两个FIFO条目。
 *
 *  @param[in]  spi   指向外设寄存器覆盖的指针
 *  @param[in]  data  要发送的数据
 *
 *  @return     传输是否发生
 *
 *  @retval     true  如果数据已添加到TX FIFO
 *  @retval     false 如果TX FIFO已满且数据未添加
 *
 *  @sa         DL_SPI_enablePacking
 *  @sa         DL_SPI_transmitData32
 *  @sa         DL_SPI_transmitDataBlocking32
 */
bool DL_SPI_transmitDataCheck32(SPI_Regs *spi, uint32_t data);

/**
 *  @brief      在尝试读取数据前检查RX FIFO
 *
 *  在尝试从FIFO读取新数据之前，检查RX FIFO是否为空。如果为空则立即退出，而不是尝试阻塞。有关其他接收选项，请参阅相关API。
 *
 *  可用于任何小于或等于8位的数据传输。
 *
 *  @param[in]  spi    指向外设寄存器覆盖的指针
 *  @param[in]  buffer 用于写入接收数据的缓冲区
 *
 *  @return     接收是否发生
 *
 *  @retval     true  如果从RX FIFO读取了数据
 *  @retval     false 如果RX FIFO为空且未读取数据
 *
 *  @sa         DL_SPI_receiveData8
 *  @sa         DL_SPI_receiveDataBlocking8
 */
bool DL_SPI_receiveDataCheck8(const SPI_Regs *spi, uint8_t *buffer);

/**
 *  @brief      在尝试读取数据前检查RX FIFO
 *
 *  在尝试从FIFO读取新数据之前，检查RX FIFO是否为空。如果为空则立即退出，而不是尝试阻塞。有关其他接收选项，请参阅相关API。
 *
 *  可用于任何小于或等于16位的数据传输。
 *
 *  @param[in]  spi    指向外设寄存器覆盖的指针
 *  @param[in]  buffer 用于写入接收数据的缓冲区
 *
 *  @return     接收是否发生
 *
 *  @retval     true  如果从RX FIFO读取了数据
 *  @retval     false 如果RX FIFO为空且未读取数据
 *
 *  @sa         DL_SPI_receiveData16
 *  @sa         DL_SPI_receiveDataBlocking16
 */
bool DL_SPI_receiveDataCheck16(const SPI_Regs *spi, uint16_t *buffer);

/**
 *  @brief      在尝试传输数据前检查 RX FIFO
 *
 *  在尝试从 FIFO 读取新数据前检查 RX FIFO 是否已为空。如果为空则立即退出，而不是尝试阻塞。
 *  有关其他接收选项，请参阅相关 API。
 *
 *  可用于任何小于或等于 32 位的数据传输。
 *
 *  注意：需要在调用此 API 之前通过调用 @ref DL_SPI_enablePacking 启用打包功能。
 *  启用打包后，RX FIFO 的两个条目将作为 32 位值返回。
 *
 *  @param[in]  spi    指向外设寄存器覆盖的指针
 *  @param[in]  buffer 用于写入接收数据的缓冲区
 *
 *  @return     接收是否发生
 *
 *  @retval     true  如果从 RX FIFO 读取了数据
 *  @retval     false 如果 RX FIFO 为空且未读取数据
 *
 *  @pre         DL_SPI_enablePacking
 *
 *  @sa         DL_SPI_receiveData32
 *  @sa         DL_SPI_receiveDataBlocking32
 */
bool DL_SPI_receiveDataCheck32(const SPI_Regs *spi, uint32_t *buffer);

/**
 *  @brief       使用 8 位访问读取 RX FIFO 中的所有可用数据
 *
 *  @param[in]   spi       指向外设寄存器覆盖的指针
 *  @param[out]  buffer    用于写入接收数据的缓冲区
 *  @param[in]   maxCount  从 RX FIFO 读取的最大字节数
 *
 *  @return      从 RX FIFO 读取的字节数
 */
uint32_t DL_SPI_drainRXFIFO8(
    const SPI_Regs *spi, uint8_t *buffer, uint32_t maxCount);

/**
 *  @brief       使用 16 位访问读取 RX FIFO 中的所有可用数据
 *
 *  @param[in]   spi       指向外设寄存器覆盖的指针
 *  @param[out]  buffer    用于写入接收数据的缓冲区
 *  @param[in]   maxCount  从 RX FIFO 读取的最大半字数
 *
 *  @return      从 RX FIFO 读取的半字数
 */
uint32_t DL_SPI_drainRXFIFO16(
    const SPI_Regs *spi, uint16_t *buffer, uint32_t maxCount);

/**
 *  @brief       使用 32 位访问读取 RX FIFO 中的所有可用数据
 *
 *  注意：需要在调用此 API 之前通过调用 @ref DL_SPI_enablePacking 启用打包功能。
 *  启用打包后，RX FIFO 的两个条目将作为 32 位值返回。
 *
 *  @param[in]   spi       指向外设寄存器覆盖的指针
 *  @param[out]  buffer    用于写入接收数据的缓冲区
 *  @param[in]   maxCount  从 RX FIFO 读取的最大字数
 *
 *  @return      从 RX FIFO 读取的字数
 *
 *  @pre         DL_SPI_enablePacking
 *
 */
uint32_t DL_SPI_drainRXFIFO32(
    const SPI_Regs *spi, uint32_t *buffer, uint32_t maxCount);

/**
 *  @brief      使用 8 位访问填充 TX FIFO
 *
 *  持续向 TX FIFO 写入数据，直到它被填满或达到计数。
 *
 *  @param[in]  spi     指向外设寄存器覆盖的指针
 *  @param[in]  buffer  要写入 TX FIFO 的数据缓冲区
 *  @param[in]  count   要写入 TX FIFO 的最大字节数
 *
 *  @return     写入 TX FIFO 的字节数
 */
uint32_t DL_SPI_fillTXFIFO8(
    SPI_Regs *spi, const uint8_t *buffer, uint32_t count);

/**
 *  @brief      使用 16 位访问填充 TX FIFO
 *
 *  持续向 TX FIFO 写入数据，直到它被填满或达到计数。
 *
 *  @param[in]  spi     指向外设寄存器覆盖的指针
 *  @param[in]  buffer  要写入 TX FIFO 的数据缓冲区
 *  @param[in]  count   要写入 TX FIFO 的最大半字数
 *
 *  @return     写入 TX FIFO 的半字数
 */
uint32_t DL_SPI_fillTXFIFO16(
    SPI_Regs *spi, const uint16_t *buffer, uint32_t count);

/**
 *  @brief      使用 32 位访问填充 TX FIFO
 *
 *  持续向 TX FIFO 写入数据，直到它被填满或达到计数。
 *
 *  注意：如果在调用此 API 之前通过调用 @ref DL_SPI_enablePacking 启用了打包功能，
 *  则 32 位写入将作为一个 FIFO 条目写入。如果禁用了打包功能，则 32 位写入将作为两个 FIFO 条目写入。
 *
 *  @param[in]  spi     指向外设寄存器覆盖的指针
 *  @param[in]  buffer  要写入 TX FIFO 的数据缓冲区
 *  @param[in]  count   要写入 TX FIFO 的最大字数
 *
 *  @return     写入 TX FIFO 的字数
 *
 *  @sa         DL_SPI_enablePacking
 */
uint32_t DL_SPI_fillTXFIFO32(
    SPI_Regs *spi, const uint32_t *buffer, uint32_t count);

/**
 *  @brief      启用用于触发 DMA 接收事件的 SPI 中断
 *
 *  启用 SPI 中断作为生成事件的条件，以直接触发 DMA。此 API 配置 DMA_TRIG_RX 寄存器，
 *  该寄存器是用于触发 DMA 执行接收数据传输的事件发布者。
 *
 *  @note 一次只能启用一个中断源。
 *
 *  @param[in]  spi       指向外设寄存器覆盖的指针
 *  @param[in]  interrupt  要启用作为 DMA 触发条件的中断。@ref DL_SPI_DMA_INTERRUPT_RX 之一。
 */
__STATIC_INLINE void DL_SPI_enableDMAReceiveEvent(
    SPI_Regs *spi, uint32_t interrupt)
{
    spi->DMA_TRIG_RX.IMASK = interrupt;
}

/**
 *  @brief      启用用于触发 DMA 传输事件的 SPI 中断
 *
 *  启用 SPI 中断作为生成事件的条件，以直接触发 DMA。此 API 配置 DMA_TRIG_TX 寄存器，
 *  该寄存器是用于触发 DMA 执行传输数据传输的事件发布者。
 *
 *  @note DMA_TRIG_TX 只有一个传输中断源
 *
 *  @param[in]  spi       指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_enableDMATransmitEvent(SPI_Regs *spi)
{
    spi->DMA_TRIG_TX.IMASK = SPI_DMA_TRIG_TX_IMASK_TX_SET;
}

/**
 *  @brief      禁用用于触发 DMA 接收事件的 SPI 中断
 *
 *  禁用 SPI 中断作为生成事件的条件，以直接触发 DMA。此 API 配置 DMA_TRIG_RX 寄存器，
 *  该寄存器是用于触发 DMA 执行接收数据传输的事件发布者。
 *
 *  @param[in]  spi       指向外设寄存器覆盖的指针
 *  @param[in]  interrupt  要禁用作为 DMA 触发条件的中断。@ref DL_SPI_DMA_INTERRUPT_RX 之一。
 */
__STATIC_INLINE void DL_SPI_disableDMAReceiveEvent(
    SPI_Regs *spi, uint32_t interrupt)
{
    spi->DMA_TRIG_RX.IMASK &= ~(interrupt);
}

/**
 *  @brief      禁用用于触发 DMA 传输事件的 SPI 中断
 *
 *  禁用 SPI 中断作为生成事件的条件，以直接触发 DMA。此 API 配置 DMA_TRIG_TX 寄存器，
 *  该寄存器是用于触发 DMA 执行传输数据传输的事件发布者。
 *
 *  @note DMA_TRIG_TX 只有一个传输中断源
 *
 *  @param[in]  spi       指向外设寄存器覆盖的指针
 */
__STATIC_INLINE void DL_SPI_disableDMATransmitEvent(SPI_Regs *spi)
{
    spi->DMA_TRIG_TX.IMASK = SPI_DMA_TRIG_TX_IMASK_TX_CLR;
}

/**
 *  @brief      检查哪些用于 DMA 接收事件的 SPI 中断已启用
 *
 *  此 API 检查 DMA_TRIG_RX 寄存器，该寄存器是用于触发 DMA 执行接收数据传输的事件发布者。
 *
 *  @param[in]  spi            指向外设寄存器覆盖的指针
 *  @param[in]  interruptMask  要检查的中断的位掩码。@ref DL_SPI_DMA_INTERRUPT_RX 的按位或。
 *
 *  @note 一次只能启用一个中断源。
 *
 *  @return     哪些请求的 SPI 中断已启用
 *
 *  @retval     @ref DL_SPI_DMA_INTERRUPT_RX 之一
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledDMAReceiveEvent(
    const SPI_Regs *spi, uint32_t interruptMask)
{
    return (spi->DMA_TRIG_RX.IMASK & interruptMask);
}

/**
 *  @brief      检查用于 DMA 传输事件的 SPI 中断是否已启用
 *
 *  此 API 检查 DMA_TRIG_TX 寄存器，该寄存器是用于触发 DMA 执行传输数据传输的事件发布者。
 *
 *  @param[in]  spi           指向外设寄存器覆盖的指针
 *
 *  @return     请求的 SPI 中断状态
 *
 *  @retval     如果已启用，则为 DL_SPI_DMA_INTERRUPT_TX，否则为 0
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledDMATransmitEvent(const SPI_Regs *spi)
{
    return (spi->DMA_TRIG_TX.IMASK & SPI_DMA_TRIG_TX_IMASK_TX_MASK);
}

/**
 *  @brief      检查已启用的用于 DMA 接收事件的 SPI 中断的中断标志
 *
 *  检查之前启用的用于 DMA 接收事件的任何 SPI 中断是否处于待处理状态。
 *  此 API 检查 DMA_TRIG_RX 寄存器，该寄存器是用于触发 DMA 执行接收数据传输的事件发布者。
 *
 *  @param[in]  spi            指向外设寄存器覆盖的指针
 *  @param[in]  interruptMask  要检查的中断的位掩码。@ref DL_SPI_DMA_INTERRUPT_RX 的按位或。
 *
 *  @note 一次只能启用一个中断源。
 *
 *  @return     请求的 SPI 中断状态
 *
 *  @retval     @ref DL_SPI_DMA_INTERRUPT_RX 之一
 *
 *  @sa         DL_SPI_enableDMAReceiveEvent
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledDMAReceiveEventStatus(
    const SPI_Regs *spi, uint32_t interruptMask)
{
    return (spi->DMA_TRIG_RX.MIS & interruptMask);
}

/**
 *  @brief      检查已启用的用于 DMA 传输事件的 SPI 中断的中断标志
 *
 *  检查之前启用的用于 DMA 传输事件的 SPI 中断是否处于待处理状态。
 *  此 API 检查 DMA_TRIG_TX 寄存器，该寄存器是用于触发 DMA 执行传输数据传输的事件发布者。
 *
 *  @param[in]  spi           指向外设寄存器覆盖的指针
 *
 *  @return     请求的 SPI 中断状态
 *
 *  @retval     如果已启用，则为 DL_SPI_DMA_INTERRUPT_TX，否则为 0
 *
 *  @sa         DL_SPI_enableDMATransmitEvent
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledDMATransmitEventStatus(
    const SPI_Regs *spi)
{
    return (spi->DMA_TRIG_TX.MIS & SPI_DMA_TRIG_TX_MIS_TX_MASK);
}

/**
 *  @brief      检查任何用于 DMA 接收事件的 SPI 中断的中断标志
 *
 *  检查任何用于 DMA 接收事件的 SPI 中断是否处于待处理状态。
 *  中断不必之前已启用。
 *  此 API 检查 DMA_TRIG_RX 寄存器，该寄存器是用于触发 DMA 执行接收数据传输的事件发布者。
 *
 *  @param[in]  spi            指向外设寄存器覆盖的指针
 *  @param[in]  interruptMask  要检查的中断的位掩码。@ref DL_SPI_DMA_INTERRUPT_RX 的按位或。
 *
 *  @return     哪些请求的 SPI 中断处于待处理状态
 *
 *  @retval     @ref DL_SPI_DMA_INTERRUPT_RX 值的按位或
 */
__STATIC_INLINE uint32_t DL_SPI_getRawDMAReceiveEventStatus(
    const SPI_Regs *spi, uint32_t interruptMask)
{
    return (spi->DMA_TRIG_RX.RIS & interruptMask);
}

/**
 *  @brief      检查任何用于 DMA 传输事件的 SPI 中断的中断标志
 *
 *  检查任何用于 DMA 传输事件的 SPI 中断是否处于待处理状态。
 *  中断不必之前已启用。
 *  此 API 检查 DMA_TRIG_TX 寄存器，该寄存器是用于触发 DMA 执行传输数据传输的事件发布者。
 *
 *  @param[in]  spi           指向外设寄存器覆盖的指针
 *
 *  @return     请求的 SPI 中断状态
 *
 *  @retval     如果已启用，则为 DL_SPI_DMA_INTERRUPT_TX，否则为 0
 */
__STATIC_INLINE uint32_t DL_SPI_getRawDMATransmitEventStatus(
    const SPI_Regs *spi)
{
    return (spi->DMA_TRIG_TX.RIS & SPI_DMA_TRIG_TX_RIS_TX_MASK);
}

/**
 *  @brief      获取用于 DMA 接收事件的最高优先级待处理 SPI 中断
 *
 *  检查任何用于 DMA 接收事件的 SPI 中断是否处于待处理状态。
 *  中断不必之前已启用。
 *  此 API 检查 DMA_TRIG_RX 寄存器，该寄存器是用于触发 DMA 执行接收数据传输的事件发布者。
 *
 *  @param[in]  spi           指向外设寄存器覆盖的指针
 *
 *  @return     最高优先级的待处理 SPI 中断
 *
 *  @retval     @ref DL_SPI_DMA_IIDX_RX 之一
 */
__STATIC_INLINE DL_SPI_DMA_IIDX_RX DL_SPI_getPendingDMAReceiveEvent(
    const SPI_Regs *spi)
{
    return (DL_SPI_DMA_IIDX_RX)(spi->DMA_TRIG_RX.IIDX);
}

/**
 *  @brief      获取用于 DMA 传输事件的最高优先级待处理 SPI 中断
 *
 *  检查用于 DMA 传输事件的 SPI 中断是否处于待处理状态。
 *  中断不必之前已启用。
 *  此 API 检查 DMA_TRIG_TX 寄存器，该寄存器是用于触发 DMA 执行传输数据传输的事件发布者。
 *
 *  @param[in]  spi           指向外设寄存器覆盖的指针
 *
 *  @return     最高优先级的待处理 SPI 中断
 *
 *  @retval     如果待处理，则为 DL_SPI_DMA_IIDX_TX，否则为 0
 */
__STATIC_INLINE DL_SPI_DMA_IIDX_TX DL_SPI_getPendingDMATransmitEvent(
    const SPI_Regs *spi)
{
    return (DL_SPI_DMA_IIDX_TX)(spi->DMA_TRIG_TX.IIDX);
}

/**
 *  @brief      清除用于 DMA 接收事件的待处理 SPI 中断
 *
 *  此 API 检查 DMA_TRIG_RX 寄存器，该寄存器是用于触发 DMA 执行接收数据传输的事件发布者。
 *
 *  @param[in]  spi            指向外设寄存器覆盖的指针
 *  @param[in]  interruptMask  要清除的中断的位掩码。@ref DL_SPI_DMA_INTERRUPT_RX 的按位或。
 */
__STATIC_INLINE void DL_SPI_clearDMAReceiveEventStatus(
    SPI_Regs *spi, uint32_t interruptMask)
{
    spi->DMA_TRIG_RX.ICLR = interruptMask;
}

/**
 *  @brief      清除用于 DMA 传输事件的待处理 SPI 中断
 *
 *  此 API 检查 DMA_TRIG_TX 寄存器，该寄存器是用于触发 DMA 执行传输数据传输的事件发布者。
 *
 *  @param[in]  spi           指向外设寄存器覆盖的指针
 *  @note DMA_TRIG_TX 只有一个传输中断源
 */
__STATIC_INLINE void DL_SPI_clearDMATransmitEventStatus(SPI_Regs *spi)
{
    spi->DMA_TRIG_TX.ICLR = SPI_DMA_TRIG_TX_ICLR_TX_CLR;
}

/**
 *  @brief      在进入功耗状态前保存 SPI 配置
 *
 *  一些位于 PD1 域的 MSPM0G 外设在进入 STOP 或 STANDBY 模式时不会保留寄存器内容。
 *  有关表现出此行为的外设实例的完整列表，请参阅数据手册。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @param[in]  ptr  配置备份设置结构。请参阅 @ref DL_SPI_backupConfig。
 *
 *  @retval     如果 ptr 中已存在配置（将不会被覆盖），则为 FALSE。如果配置已成功保存，则为 TRUE
 *
 *  @sa         DL_SPI_restoreConfiguration
 */
bool DL_SPI_saveConfiguration(const SPI_Regs *spi, DL_SPI_backupConfig *ptr);

/**
 *  @brief      在离开功耗状态后恢复 SPI 配置
 *
 *  一些位于 PD1 域的 MSPM0G 外设在进入 STOP 或 STANDBY 模式时不会保留寄存器内容。
 *  有关表现出此行为的外设实例的完整列表，请参阅数据手册。
 *
 *  @param[in]  spi  指向外设寄存器覆盖的指针
 *
 *  @param[in]  ptr  配置备份设置结构。请参阅 @ref DL_SPI_backupConfig。
 *
 *  @retval     如果 ptr 中不存在配置（将不会加载），则为 FALSE。如果配置已成功加载，则为 TRUE
 *
 *  @sa         DL_SPI_saveConfiguration
 */
bool DL_SPI_restoreConfiguration(SPI_Regs *spi, DL_SPI_backupConfig *ptr);

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_SPI__ */

#endif /* ti_dl_dl_spi__include */
/** @}*/
