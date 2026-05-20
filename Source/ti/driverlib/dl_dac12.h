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
 *  @file       dl_dac12.h
 *  @brief      12位DAC (数模转换器) 驱动程序库
 *  @defgroup   DAC12 数模转换器 (DAC12)
 *
 *  @anchor ti_dl_dl_dac12_Overview
 *  # 概述
 *
 *  数模转换器驱动程序库允许完全配置 MSPM0 DAC12 模块。
 *  DAC 模块是一个12位电压输出数模转换器 (DAC)。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup DAC12
 * @{
 */
#ifndef ti_dl_dl_dac12__include
#define ti_dl_dl_dac12__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_DAC12__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @enum DL_DAC12_OUTPUT */
typedef enum {
    /*! DAC输出与输出引脚断开 */
    DL_DAC12_OUTPUT_DISABLED = DAC12_CTL1_OPS_NOC0,
    /*! DAC输出连接到输出引脚 */
    DL_DAC12_OUTPUT_ENABLED = DAC12_CTL1_OPS_OUT0,
} DL_DAC12_OUTPUT;

/** @enum DL_DAC12_REPRESENTATION */
typedef enum {
    /*! 二进制表示 */
    DL_DAC12_REPRESENTATION_BINARY = DAC12_CTL0_DFM_BINARY,
    /*! 二进制补码表示 */
    DL_DAC12_REPRESENTATION_TWOS_COMPLEMENT = DAC12_CTL0_DFM_TWOS_COMP,
} DL_DAC12_REPRESENTATION;

/** @enum DL_DAC12_RESOLUTION */
typedef enum {
    /*! 12位输入分辨率 */
    DL_DAC12_RESOLUTION_12BIT = DAC12_CTL0_RES__12BITS,
    /*! 8位输入分辨率 */
    DL_DAC12_RESOLUTION_8BIT = DAC12_CTL0_RES__8BITS,
} DL_DAC12_RESOLUTION;

/** @enum DL_DAC12_AMP */
typedef enum {
    /*! 输出放大器关闭，高阻抗三态输出 */
    DL_DAC12_AMP_OFF_TRISTATE = DAC12_CTL1_AMPHIZ_HIZ,
    /*! 输出放大器关闭，输出下拉至地 */
    DL_DAC12_AMP_OFF_0V = DAC12_CTL1_AMPHIZ_PULLDOWN,
    /*! 输出放大器使能 */
    DL_DAC12_AMP_ON = DAC12_CTL1_AMPEN_ENABLE,
} DL_DAC12_AMP;

/** @enum DL_DAC12_VREF_SOURCE */
typedef enum {
    /*! 正参考: VDDA, 负参考: VeREF- */
    DL_DAC12_VREF_SOURCE_VDDA_VEREFN = (DAC12_CTL1_REFSP_VDDA |
                                        DAC12_CTL1_REFSN_VEREFN) ,
    /*! 正参考: VeREF+, 负参考: VeREF- */
    DL_DAC12_VREF_SOURCE_VEREFP_VEREFN = (DAC12_CTL1_REFSP_VEREFP |
                                          DAC12_CTL1_REFSN_VEREFN),
    /*! 正参考: VDDA, 负参考: VSSA */
    DL_DAC12_VREF_SOURCE_VDDA_VSSA = (DAC12_CTL1_REFSP_VDDA |
                                      DAC12_CTL1_REFSN_VSSA),
    /*! 正参考: VeREF+, 负参考: VSSA */
    DL_DAC12_VREF_SOURCE_VEREFP_VSSA = (DAC12_CTL1_REFSP_VEREFP |
                                        DAC12_CTL1_REFSN_VSSA),
} DL_DAC12_VREF_SOURCE;

/** @enum DL_DAC12_SAMPLETIMER */
typedef enum {
    /*! DAC采样定时器发生器禁用 */
    DL_DAC12_SAMPLETIMER_DISABLE = DAC12_CTL3_STIMEN_CLR,
    /*! DAC采样定时器发生器使能 */
    DL_DAC12_SAMPLETIMER_ENABLE = DAC12_CTL3_STIMEN_SET,
} DL_DAC12_SAMPLETIMER;


/** @enum DL_DAC12_SAMPLES_PER_SECOND */
typedef enum {
    /*! 采样触发速率: 500 次/秒 */
    DL_DAC12_SAMPLES_PER_SECOND_500 = DAC12_CTL3_STIMCONFIG__500SPS,
    /*! 采样触发速率: 1,000 次/秒 */
    DL_DAC12_SAMPLES_PER_SECOND_1K = DAC12_CTL3_STIMCONFIG__1KSPS,
    /*! 采样触发速率: 2,000 次/秒 */
    DL_DAC12_SAMPLES_PER_SECOND_2K = DAC12_CTL3_STIMCONFIG__2KSPS,
    /*! 采样触发速率: 4,000 次/秒 */
    DL_DAC12_SAMPLES_PER_SECOND_4K = DAC12_CTL3_STIMCONFIG__4KSPS,
    /*! 采样触发速率: 8,000 次/秒 */
    DL_DAC12_SAMPLES_PER_SECOND_8K = DAC12_CTL3_STIMCONFIG__8KSPS,
    /*! 采样触发速率: 16,000 次/秒 */
    DL_DAC12_SAMPLES_PER_SECOND_16K = DAC12_CTL3_STIMCONFIG__16KSPS,
    /*! 采样触发速率: 100,000 次/秒 */
    DL_DAC12_SAMPLES_PER_SECOND_100K = DAC12_CTL3_STIMCONFIG__100KSPS,
    /*! 采样触发速率: 200,000 次/秒 */
    DL_DAC12_SAMPLES_PER_SECOND_200K = DAC12_CTL3_STIMCONFIG__200KSPS,
    /*! 采样触发速率: 500,000 次/秒 */
    DL_DAC12_SAMPLES_PER_SECOND_500K = DAC12_CTL3_STIMCONFIG__500KSPS,
    /*! 采样触发速率: 1,000,000 次/秒 */
    DL_DAC12_SAMPLES_PER_SECOND_1M = DAC12_CTL3_STIMCONFIG__1MSPS,
} DL_DAC12_SAMPLES_PER_SECOND;

/** @enum DL_DAC12_FIFO */
typedef enum {
    /*! DAC FIFO禁用 */
    DL_DAC12_FIFO_DISABLED = DAC12_CTL2_FIFOEN_CLR,
    /*! DAC FIFO使能 */
    DL_DAC12_FIFO_ENABLED = DAC12_CTL2_FIFOEN_SET,
} DL_DAC12_FIFO;


/** @enum DL_DAC12_FIFO_THRESHOLD */
typedef enum {
    /*! DMA触发FIFO阈值: 1/4空 */
    DL_DAC12_FIFO_THRESHOLD_ONE_QTR_EMPTY = DAC12_CTL2_FIFOTH_LOW,
    /*! DMA触发FIFO阈值: 2/4空 */
    DL_DAC12_FIFO_THRESHOLD_TWO_QTRS_EMPTY = DAC12_CTL2_FIFOTH_MED,
    /*! DMA触发FIFO阈值: 3/4空 */
    DL_DAC12_FIFO_THRESHOLD_THREE_QTRS_EMPTY = DAC12_CTL2_FIFOTH_HIGH,
} DL_DAC12_FIFO_THRESHOLD;

/** @enum DL_DAC12_FIFO_TRIGGER */
typedef enum {
    /*! FIFO读触发源: 采样定时器 */
    DL_DAC12_FIFO_TRIGGER_SAMPLETIMER = DAC12_CTL2_FIFOTRIGSEL_STIM,
    /*! FIFO读触发源: 硬件触发0事件 */
    DL_DAC12_FIFO_TRIGGER_HWTRIG0 = DAC12_CTL2_FIFOTRIGSEL_TRIG0,
} DL_DAC12_FIFO_TRIGGER;

/** @enum DL_DAC12_DMA_TRIGGER */
typedef enum {
    /*! DAC DMA触发机制禁用 */
    DL_DAC12_DMA_TRIGGER_DISABLED = DAC12_CTL2_DMATRIGEN_CLR,
    /*! DAC DMA触发机制使能 */
    DL_DAC12_DMA_TRIGGER_ENABLED = DAC12_CTL2_DMATRIGEN_SET,
} DL_DAC12_DMA_TRIGGER;

/** @enum DL_DAC12_CALIBRATION */
typedef enum {

    /*! 工厂校准 */
    DL_DAC12_CALIBRATION_FACTORY = DAC12_CALCTL_CALSEL_FACTORYTRIM,
    /*! 自校准 (上次校准结果) */
    DL_DAC12_CALIBRATION_SELF = DAC12_CALCTL_CALSEL_SELFCALIBRATIONTRIM,

} DL_DAC12_CALIBRATION;

/** @addtogroup DL_DAC12_INTERRUPT
 *  @{
 */

/*!
 *  @brief DAC模块就绪事件中断
 */
#define DL_DAC12_INTERRUPT_MODULE_READY             (DAC12_GEN_EVENT_IMASK_MODRDYIFG_SET)

/*!
 *  @brief FIFO空中断
 */
#define DL_DAC12_INTERRUPT_FIFO_EMPTY            (DAC12_GEN_EVENT_IMASK_FIFOEMPTYIFG_SET)

/*!
 *  @brief FIFO 3/4空中断
 */
#define DL_DAC12_INTERRUPT_FIFO_THREE_QTRS_EMPTY   (DAC12_GEN_EVENT_IMASK_FIFO3B4IFG_SET)

/*!
 *  @brief FIFO 2/4空中断
 */
#define DL_DAC12_INTERRUPT_FIFO_TWO_QTRS_EMPTY     (DAC12_GEN_EVENT_IMASK_FIFO1B2IFG_SET)

/*!
 *  @brief FIFO 1/4空中断
 */
#define DL_DAC12_INTERRUPT_FIFO_ONE_QTR_EMPTY      (DAC12_GEN_EVENT_IMASK_FIFO1B4IFG_SET)

/*!
 *  @brief FIFO满中断
 */
#define DL_DAC12_INTERRUPT_FIFO_FULL              (DAC12_GEN_EVENT_IMASK_FIFOFULLIFG_SET)

/*!
 *  @brief FIFO下溢中断 (尝试从空的FIFO读取)
 */
#define DL_DAC12_INTERRUPT_FIFO_UNDERRUN          (DAC12_GEN_EVENT_IMASK_FIFOURUNIFG_SET)

/*!
 *  @brief DMA完成传输中断确认
 *
 *  仅在DMA操作期间，当DMA完成向FIFO传输指定数量的数据后，
 *  DMA将发出DMA done事件。DMA还会发送状态信号，如果还有更多数据要发送，
 *  状态为0；如果DMA已完成发送信息且状态非零，则DMATrigger将被禁用，
 *  并触发此中断。
 */
#define DL_DAC12_INTERRUPT_DMA_DONE                (DAC12_GEN_EVENT_IMASK_DMADONEIFG_SET)

/** @}*/

/** @addtogroup DL_DAC12_EVENT
 *  @{
 */

/*!
 *  @brief DAC模块就绪事件
 */
#define DL_DAC12_EVENT_MODULE_READY             (DAC12_GEN_EVENT_IMASK_MODRDYIFG_SET)

/*!
 *  @brief FIFO空事件
 */
#define DL_DAC12_EVENT_FIFO_EMPTY            (DAC12_GEN_EVENT_IMASK_FIFOEMPTYIFG_SET)

/*!
 *  @brief FIFO 3/4空事件
 */
#define DL_DAC12_EVENT_FIFO_THREE_QTRS_EMPTY   (DAC12_GEN_EVENT_IMASK_FIFO3B4IFG_SET)

/*!
 *  @brief FIFO 2/4空事件
 */
#define DL_DAC12_EVENT_FIFO_TWO_QTRS_EMPTY     (DAC12_GEN_EVENT_IMASK_FIFO1B2IFG_SET)

/*!
 *  @brief FIFO 1/4空事件
 */
#define DL_DAC12_EVENT_FIFO_ONE_QTR_EMPTY      (DAC12_GEN_EVENT_IMASK_FIFO1B4IFG_SET)

/*!
 *  @brief FIFO满事件
 */
#define DL_DAC12_EVENT_FIFO_FULL              (DAC12_GEN_EVENT_IMASK_FIFOFULLIFG_SET)

/*!
 *  @brief FIFO下溢事件 (尝试从空的FIFO读取)
 */
#define DL_DAC12_EVENT_FIFO_UNDERRUN          (DAC12_GEN_EVENT_IMASK_FIFOURUNIFG_SET)

/*!
 *  @brief DMA完成传输事件确认
 *
 *  仅在DMA操作期间，当DMA完成向FIFO传输指定数量的数据后，
 *  DMA将发出DMA done事件。
 */
#define DL_DAC12_EVENT_DMA_DONE                (DAC12_GEN_EVENT_IMASK_DMADONEIFG_SET)

/** @}*/

/* clang-format on */

/*! @enum DL_DAC12_IIDX */
typedef enum {
    /*! DAC无待处理中断 */
    DL_DAC12_IIDX_NO_INT = DAC12_CPU_INT_IIDX_STAT_NO_INTR,
    /*! DAC模块就绪中断 */
    DL_DAC12_IIDX_MODULE_READY = DAC12_CPU_INT_IIDX_STAT_MODRDYIFG,
    /*! DAC FIFO满中断 */
    DL_DAC12_IIDX_FIFO_FULL = DAC12_CPU_INT_IIDX_STAT_FIFOFULLIFG,
    /*! DAC FIFO 1/4空中断 */
    DL_DAC12_IIDX_FIFO_1_4_EMPTY = DAC12_CPU_INT_IIDX_STAT_FIFO1B4IFG,
    /*! DAC FIFO 1/2空中断 */
    DL_DAC12_IIDX_FIFO_1_2_EMPTY = DAC12_CPU_INT_IIDX_STAT_FIFO1B2IFG,
    /*! DAC FIFO 3/4空中断 */
    DL_DAC12_IIDX_FIFO_3_4_EMPTY = DAC12_CPU_INT_IIDX_STAT_FIFO3B4IFG,
    /*! DAC FIFO空中断 */
    DL_DAC12_IIDX_FIFO_EMPTY = DAC12_CPU_INT_IIDX_STAT_FIFOEMPTYIFG,
    /*! DAC FIFO下溢中断 */
    DL_DAC12_IIDX_FIFO_UNDERRUN = DAC12_CPU_INT_IIDX_STAT_FIFOURUNIFG,
    /*! DAC DMA传输完成中断 */
    DL_DAC12_IIDX_DMA_DONE = DAC12_CPU_INT_IIDX_STAT_DMADONEIFG
} DL_DAC12_IIDX;

/*! @enum DL_DAC12_SUBSCRIBER_INDEX */
typedef enum {
    /*! DAC12订阅者索引0 */
    DL_DAC12_SUBSCRIBER_INDEX_0 = 0,
} DL_DAC12_SUBSCRIBER_INDEX;

/*! @enum DL_DAC12_EVENT_ROUTE */
typedef enum {
    /*! DAC12事件路由1 */
    DL_DAC12_EVENT_ROUTE_1 = 0,
} DL_DAC12_EVENT_ROUTE;

/**
 * @brief  DAC12初始化配置结构体
 *
 * 使用示例:
 * @code
 * DL_DAC12_Config dacConfig = {
 *     .outputEnable = DL_DAC12_OUTPUT_ENABLED,              // 使能DAC输出
 *     .resolution = DL_DAC12_RESOLUTION_12BIT,            // 12位分辨率
 *     .representation = DL_DAC12_REPRESENTATION_BINARY,    // 二进制格式
 *     .voltageReferenceSource = DL_DAC12_VREF_SOURCE_VDDA_VSSA, // VDDA参考
 *     .amplifierSetting = DL_DAC12_AMP_ON,                // 放大器使能
 *     .fifoEnable = DL_DAC12_FIFO_DISABLED,               // 禁用FIFO
 *     .fifoTriggerSource = DL_DAC12_FIFO_TRIGGER_SAMPLETIMER,
 *     .dmaTriggerEnable = DL_DAC12_DMA_TRIGGER_DISABLED,
 *     .dmaTriggerThreshold = DL_DAC12_FIFO_THRESHOLD_ONE_QTR_EMPTY,
 *     .sampleTimeGeneratorEnable = DL_DAC12_SAMPLETIMER_DISABLE,
 *     .sampleRate = DL_DAC12_SAMPLES_PER_SECOND_1K
 * };
 * DL_DAC12_init(DAC12, &dacConfig);
 * @endcode
 */
typedef struct {
    /*! DAC输出使能 (连接到OUT引脚)。可选 @ref DL_DAC12_OUTPUT */
    DL_DAC12_OUTPUT outputEnable;

    /*! DAC分辨率。可选 @ref DL_DAC12_RESOLUTION */
    DL_DAC12_RESOLUTION resolution;

    /*! 输入数据表示格式。可选 @ref DL_DAC12_REPRESENTATION */
    DL_DAC12_REPRESENTATION representation;

    /*! 参考电压源。可选 @ref DL_DAC12_VREF_SOURCE */
    DL_DAC12_VREF_SOURCE voltageReferenceSource;

    /*! 输出放大器设置。可选 @ref DL_DAC12_AMP */
    DL_DAC12_AMP amplifierSetting;

    /*! FIFO使能。可选 @ref DL_DAC12_FIFO */
    DL_DAC12_FIFO fifoEnable;

    /*! FIFO读触发源。可选 @ref DL_DAC12_FIFO_TRIGGER */
    DL_DAC12_FIFO_TRIGGER fifoTriggerSource;

    /*! DMA触发使能 (使用DMA代替CPU输入数据)。
     *  可选 @ref DL_DAC12_DMA_TRIGGER
     */
    DL_DAC12_DMA_TRIGGER dmaTriggerEnable;

    /*! DMA触发时的FIFO阈值。
     *  可选 @ref DL_DAC12_FIFO_THRESHOLD
     */
    DL_DAC12_FIFO_THRESHOLD dmaTriggerThreshold;

    /*! 采样定时器使能作为FIFO触发。
     *  可选 @ref DL_DAC12_SAMPLETIMER
     */
    DL_DAC12_SAMPLETIMER sampleTimeGeneratorEnable;

    /*! 采样定时器速率。可选
     *  @ref DL_DAC12_SAMPLES_PER_SECOND
     */
    DL_DAC12_SAMPLES_PER_SECOND sampleRate;
} DL_DAC12_Config;

/**
 * @brief 初始化DAC模块
 *
 * 初始化DAC模块的所有常用可配置选项。任何其他自定义配置可以在此初始化之后进行。
 * @post DAC未使能
 *
 * @param[in] dac12   指向DAC12寄存器结构体的指针
 * @param[in] config  DAC12外设配置结构体
 *
 * @par 使用示例
 * @code
 * DL_DAC12_Config dacConfig = {
 *     .outputEnable = DL_DAC12_OUTPUT_ENABLED,
 *     .resolution = DL_DAC12_RESOLUTION_12BIT,
 *     .representation = DL_DAC12_REPRESENTATION_BINARY,
 *     .voltageReferenceSource = DL_DAC12_VREF_SOURCE_VDDA_VSSA,
 *     .amplifierSetting = DL_DAC12_AMP_ON,
 *     .fifoEnable = DL_DAC12_FIFO_DISABLED,
 *     .fifoTriggerSource = DL_DAC12_FIFO_TRIGGER_SAMPLETIMER,
 *     .dmaTriggerEnable = DL_DAC12_DMA_TRIGGER_DISABLED,
 *     .dmaTriggerThreshold = DL_DAC12_FIFO_THRESHOLD_ONE_QTR_EMPTY,
 *     .sampleTimeGeneratorEnable = DL_DAC12_SAMPLETIMER_DISABLE,
 *     .sampleRate = DL_DAC12_SAMPLES_PER_SECOND_1K
 * };
 * DL_DAC12_init(DAC12, &dacConfig);
 * @endcode
 */
void DL_DAC12_init(DAC12_Regs *dac12, const DL_DAC12_Config *config);

/**
 * @brief 启用DAC12外设的电源使能(PWREN)寄存器
 *
 * 在软件可以配置任何外设寄存器之前，必须通过向外设的PWREN寄存器
 * 写入ENABLE位和适当的KEY值来启用外设本身。
 *
 * @note 欲了解更多省电信息，请参阅 @ref DL_DAC12_enable
 *
 * @param dac12        指向外设寄存器映射结构的指针
 *
 * @par 使用示例
 * @code
 * DL_DAC12_enablePower(DAC12);
 * @endcode
 */
__STATIC_INLINE void DL_DAC12_enablePower(DAC12_Regs *dac12)
{
    dac12->GPRCM.PWREN =
        (DAC12_PWREN_KEY_UNLOCK_W | DAC12_PWREN_ENABLE_ENABLE);
}

/**
 * @brief 禁用DAC12外设的电源使能(PWREN)寄存器
 *
 * 当PWREN.ENABLE位被清除时，外设的寄存器不可进行读/写操作。
 *
 * @note 此API不能提供大量省电效果，欲了解更多省电信息，
 *       请参阅 @ref DL_DAC12_enable
 *
 * @param dac12        指向外设寄存器映射结构的指针
 *
 * @par 使用示例
 * @code
 * DL_DAC12_disablePower(DAC12);
 * @endcode
 */
__STATIC_INLINE void DL_DAC12_disablePower(DAC12_Regs *dac12)
{
    dac12->GPRCM.PWREN =
        (DAC12_PWREN_KEY_UNLOCK_W | DAC12_PWREN_ENABLE_DISABLE);
}

/**
 * @brief Returns if the Peripheral Write Enable (PWREN) register for the DAC12
 *        is enabled
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *
 *
 * @param dac12        Pointer to the register overlay for the peripheral
 *
 * @return true if peripheral register access is enabled
 * @return false if peripheral register access is disabled
 */
__STATIC_INLINE bool DL_DAC12_isPowerEnabled(const DAC12_Regs *dac12)
{
    return ((dac12->GPRCM.PWREN & DAC12_PWREN_ENABLE_MASK) ==
            DAC12_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Resets dac12 peripheral
 *
 * @param dac12        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_DAC12_reset(DAC12_Regs *dac12)
{
    dac12->GPRCM.RSTCTL =
        (DAC12_RSTCTL_KEY_UNLOCK_W | DAC12_RSTCTL_RESETSTKYCLR_CLR |
            DAC12_RSTCTL_RESETASSERT_ASSERT);
}

/**
 * @brief Returns if dac12 peripheral was reset
 *
 * @param dac12        Pointer to the register overlay for the peripheral
 *
 * @return true if peripheral was reset
 * @return false if peripheral wasn't reset
 *
 */
__STATIC_INLINE bool DL_DAC12_isReset(const DAC12_Regs *dac12)
{
    return ((dac12->GPRCM.STAT & DAC12_STAT_RESETSTKY_MASK) ==
            DAC12_STAT_RESETSTKY_RESET);
}

/**
 * @brief 使能DAC模块
 *
 * @param[in] dac12    指向DAC12寄存器结构体的指针
 *
 * @par 使用示例
 * @code
 * DL_DAC12_enable(DAC12);
 * @endcode
 */
__STATIC_INLINE void DL_DAC12_enable(DAC12_Regs *dac12)
{
    dac12->CTL0 |= DAC12_CTL0_ENABLE_SET;
}

/**
 * @brief 禁用DAC模块
 *
 * @param[in] dac12    指向DAC12寄存器结构体的指针
 *
 * @par 使用示例
 * @code
 * DL_DAC12_disable(DAC12);
 * @endcode
 */
__STATIC_INLINE void DL_DAC12_disable(DAC12_Regs *dac12)
{
    dac12->CTL0 &= ~DAC12_CTL0_ENABLE_MASK;
}

/**
 * @brief 检查DAC模块是否使能
 *
 * @param[in] dac12    指向DAC12寄存器结构体的指针
 *
 * @return     DAC模块是否使能
 *
 * @retval     true    模块已使能
 * @retval     false   模块已禁用
 *
 * @par 使用示例
 * @code
 * if (DL_DAC12_isEnabled(DAC12)) {
 *     // DAC已使能
 * }
 * @endcode
 */
__STATIC_INLINE bool DL_DAC12_isEnabled(const DAC12_Regs *dac12)
{
    uint32_t t = (dac12->CTL0 & DAC12_CTL0_ENABLE_MASK);
    return (t == DAC12_CTL0_ENABLE_SET);
}

/**
 * @brief 配置DAC数据格式 (分辨率和数据表示)
 *
 * @param[in] dac12    指向DAC12寄存器结构体的指针
 * @param[in] rep      数据表示格式。可选 @ref DL_DAC12_REPRESENTATION
 * @param[in] res      数据分辨率。可选 @ref DL_DAC12_RESOLUTION
 *
 * @par 使用示例
 * @code
 * // 配置为12位分辨率，二进制格式
 * DL_DAC12_configDataFormat(DAC12,
 *     DL_DAC12_REPRESENTATION_BINARY,
 *     DL_DAC12_RESOLUTION_12BIT);
 * @endcode
 */
__STATIC_INLINE void DL_DAC12_configDataFormat(
    DAC12_Regs *dac12, DL_DAC12_REPRESENTATION rep, DL_DAC12_RESOLUTION res)
{
    DL_Common_updateReg(&dac12->CTL0, ((uint32_t) rep | (uint32_t) res),
        DAC12_CTL0_RES_MASK | DAC12_CTL0_DFM_MASK);
}

/**
 *  @brief      Gets the currently configured amplifier setting
 *
 *  @param[in]  dac12    pointer to the register overlay of the peripheral
 *
 *  @return     Currently configured amplifier setting
 *
 *  @retval     One of @ref DL_DAC12_AMP
 */
__STATIC_INLINE DL_DAC12_AMP DL_DAC12_getAmplifier(const DAC12_Regs *dac12)
{
    uint32_t ampVal =
        (dac12->CTL1 & (DAC12_CTL1_AMPEN_MASK | DAC12_CTL1_AMPHIZ_MASK));

    return (DL_DAC12_AMP)(ampVal);
}

/**
 *  @brief      Sets the DAC and output amplifer setting
 *
 *  @param[in]  dac12    pointer to the register overlay of the peripheral
 *  @param[in]  ampVal   amplifier configuration value. One of @ref DL_DAC12_AMP
 */
__STATIC_INLINE void DL_DAC12_setAmplifier(
    DAC12_Regs *dac12, DL_DAC12_AMP ampVal)
{
    DL_Common_updateReg(&dac12->CTL1, (uint32_t) ampVal,
        (DAC12_CTL1_AMPEN_MASK | DAC12_CTL1_AMPHIZ_MASK));
}

/**
 * @brief 获取参考电压源
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @return  当前配置的参考电压
 *
 * @retval  @ref DL_DAC12_VREF_SOURCE
 */
__STATIC_INLINE DL_DAC12_VREF_SOURCE DL_DAC12_getReferenceVoltageSource(
    const DAC12_Regs *dac12)
{
    uint32_t refsVal =
        (dac12->CTL1 & (DAC12_CTL1_REFSP_MASK | DAC12_CTL1_REFSN_MASK));

    return (DL_DAC12_VREF_SOURCE)(refsVal);
}

/**
 * @brief 设置DAC参考电压源
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 * @param refsVal 参考电压源。可选 @ref DL_DAC12_VREF_SOURCE
 */
__STATIC_INLINE void DL_DAC12_setReferenceVoltageSource(
    DAC12_Regs *dac12, DL_DAC12_VREF_SOURCE refsVal)
{
    DL_Common_updateReg(&dac12->CTL1, (uint32_t) refsVal,
        (DAC12_CTL1_REFSP_MASK | DAC12_CTL1_REFSN_MASK));
}

/**
 * @brief 使能DAC输出 (连接到OUT0引脚)
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @par 使用示例
 * @code
 * DL_DAC12_enableOutputPin(DAC12);
 * @endcode
 */
__STATIC_INLINE void DL_DAC12_enableOutputPin(DAC12_Regs *dac12)
{
    dac12->CTL1 |= DAC12_CTL1_OPS_OUT0;
}

/**
 * @brief 禁用DAC输出 (从OUT0引脚断开)
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @par 使用示例
 * @code
 * DL_DAC12_disableOutputPin(DAC12);
 * @endcode
 */
__STATIC_INLINE void DL_DAC12_disableOutputPin(DAC12_Regs *dac12)
{
    dac12->CTL1 &= ~DAC12_CTL1_OPS_MASK;
}

/**
 * @brief 检查输出是否已连接
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @return  输出连接状态
 *
 * @retval  true   输出已连接
 * @retval  false  输出未连接
 */
__STATIC_INLINE bool DL_DAC12_isOutputPinEnabled(const DAC12_Regs *dac12)
{
    return ((dac12->CTL1 & DAC12_CTL1_OPS_MASK) == DAC12_CTL1_OPS_OUT0);
}

/**
 * @brief 使能FIFO模块
 *
 * 使能FIFO和FIFO硬件控制状态机
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @par 使用示例
 * @code
 * DL_DAC12_enableFIFO(DAC12);
 * @endcode
 */
__STATIC_INLINE void DL_DAC12_enableFIFO(DAC12_Regs *dac12)
{
    /* Insert value */
    dac12->CTL2 |= DAC12_CTL2_FIFOEN_SET;
}

/**
 * @brief 禁用FIFO
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @par 使用示例
 * @code
 * DL_DAC12_disableFIFO(DAC12);
 * @endcode
 */
__STATIC_INLINE void DL_DAC12_disableFIFO(DAC12_Regs *dac12)
{
    /* Clear out the bit */
    dac12->CTL2 &= ~DAC12_CTL2_FIFOEN_MASK;
}

/**
 * @brief 检查FIFO是否已使能
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @return  FIFO使能状态
 *
 * @retval  true    FIFO已使能
 * @retval  false   FIFO未使能
 */
__STATIC_INLINE bool DL_DAC12_isFIFOEnabled(const DAC12_Regs *dac12)
{
    uint32_t t = (dac12->CTL2 & DAC12_CTL2_FIFOEN_MASK);
    return (t == DAC12_CTL2_FIFOEN_SET);
}

/**
 * @brief 获取FIFO阈值
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @return  当前配置的FIFO阈值
 *
 * @retval  @ref DL_DAC12_FIFO_THRESHOLD
 *
 * @see DL_DAC12_setFIFOThreshold
 */
__STATIC_INLINE DL_DAC12_FIFO_THRESHOLD DL_DAC12_getFIFOThreshold(
    const DAC12_Regs *dac12)
{
    uint32_t fifoThreshold = (dac12->CTL2 & DAC12_CTL2_FIFOTH_MASK);

    return (DL_DAC12_FIFO_THRESHOLD)(fifoThreshold);
}

/**
 * @brief 设置FIFO阈值
 *
 * 确定FIFO阈值。在基于DMA的操作中，当FIFO中的空位置数量
 * 与所选FIFO阈值级别匹配时，DAC会生成新的DMA触发。
 * 在基于CPU的操作中，FIFO阈值位无效，FIFO级别通过
 * RIS寄存器中的相应位直接指示。
 *
 * @param dac12          指向DAC12寄存器结构体的指针
 * @param fifoThreshold  阈值。可选 @ref DL_DAC12_FIFO_THRESHOLD
 */
__STATIC_INLINE void DL_DAC12_setFIFOThreshold(
    DAC12_Regs *dac12, DL_DAC12_FIFO_THRESHOLD fifoThreshold)
{
    DL_Common_updateReg(
        &dac12->CTL2, (uint32_t) fifoThreshold, DAC12_CTL2_FIFOTH_MASK);
}

/**
 * @brief 获取FIFO读触发源
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @return  当前配置的触发源
 *
 * @retval  @ref DL_DAC12_FIFO_TRIGGER
 *
 * @see DL_DAC12_setFIFOTriggerSource
 */
__STATIC_INLINE DL_DAC12_FIFO_TRIGGER DL_DAC12_getFIFOTriggerSource(
    const DAC12_Regs *dac12)
{
    uint32_t fifoTrig = (dac12->CTL2 & DAC12_CTL2_FIFOTRIGSEL_MASK);

    return (DL_DAC12_FIFO_TRIGGER)(fifoTrig);
}

/**
 * @brief 设置FIFO读触发源
 *
 * 选择DAC的FIFO读触发源。当所选的FIFO读触发被置有效时，
 * FIFO中的数据(由读指针指示)被移动到内部DAC数据寄存器。
 *
 * @param dac12     指向DAC12寄存器结构体的指针
 * @param fifoTrig 读触发源。可选 @ref DL_DAC12_FIFO_TRIGGER
 */
__STATIC_INLINE void DL_DAC12_setFIFOTriggerSource(
    DAC12_Regs *dac12, DL_DAC12_FIFO_TRIGGER fifoTrig)
{
    DL_Common_updateReg(
        &dac12->CTL2, (uint32_t) fifoTrig, DAC12_CTL2_FIFOTRIGSEL_MASK);
}

/**
 * @brief 使能DMA触发器
 *
 * 当FIFO使能时，DMA触发会根据FIFO空位置数量和所选FIFO阈值设置生成。
 * 当DMA done事件被置有效且DMA状态信号值非零时，此位由硬件自动清除。
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @see DL_DAC12_enableFIFO
 * @see DL_DAC12_setFIFOThreshold
 */
__STATIC_INLINE void DL_DAC12_enableDMATrigger(DAC12_Regs *dac12)
{
    /* Insert value */
    dac12->CTL2 |= DAC12_CTL2_DMATRIGEN_SET;
}

/**
 * @brief 禁用DMA触发器
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 */
__STATIC_INLINE void DL_DAC12_disableDMATrigger(DAC12_Regs *dac12)
{
    /* Clear out the bit */
    dac12->CTL2 &= ~DAC12_CTL2_DMATRIGEN_MASK;
}

/**
 * @brief 检查DMA触发器是否使能
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @return  DMA触发器状态
 *
 * @retval  true   DMA触发已使能
 * @retval  false  DMA触发未使能
 */
__STATIC_INLINE bool DL_DAC12_isDMATriggerEnabled(const DAC12_Regs *dac12)
{
    uint32_t t = (dac12->CTL2 & DAC12_CTL2_DMATRIGEN_MASK);
    return (t == DAC12_CTL2_DMATRIGEN_SET);
}

/**
 * @brief 使能采样时间生成器
 *
 * 采样时间生成器可作为FIFO触发源 (@ref DL_DAC12_setFIFOTriggerSource)
 * 控制DAC从FIFO消耗输入的速率。
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 */
__STATIC_INLINE void DL_DAC12_enableSampleTimeGenerator(DAC12_Regs *dac12)
{
    /* Insert value */
    dac12->CTL3 |= DAC12_CTL3_STIMEN_SET;
}

/**
 * @brief 禁用采样时间生成器
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @see DL_DAC12_enableSampleTimeGenerator
 */
__STATIC_INLINE void DL_DAC12_disableSampleTimeGenerator(DAC12_Regs *dac12)
{
    /* Clear out the bit */
    dac12->CTL3 &= ~DAC12_CTL3_STIMEN_MASK;
}

/**
 * @brief 检查采样时间触发器是否使能
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @return  采样时间生成器状态
 *
 * @retval  true   采样时间生成器已使能
 * @retval  false  采样时间生成器未使能
 */
__STATIC_INLINE bool DL_DAC12_isSampleTimeGeneratorEnabled(
    const DAC12_Regs *dac12)
{
    uint32_t t = (dac12->CTL3 & DAC12_CTL3_STIMEN_MASK);
    return (t == DAC12_CTL3_STIMEN_SET);
}

/**
 * @brief 获取采样率
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @return  当前配置的采样率
 *
 * @retval  @ref DL_DAC12_SAMPLES_PER_SECOND
 */
__STATIC_INLINE DL_DAC12_SAMPLES_PER_SECOND DL_DAC12_getSampleRate(
    const DAC12_Regs *dac12)
{
    uint32_t sampleRate = (dac12->CTL3 & DAC12_CTL3_STIMCONFIG_MASK);

    return (DL_DAC12_SAMPLES_PER_SECOND)(sampleRate);
}

/**
 * @brief 设置采样率
 *
 * 设置采样时间生成器的采样触发速率。
 * 采样时间生成器可作为FIFO触发源 (@ref DL_DAC12_setFIFOTriggerSource)
 * 控制DAC从FIFO消耗输入的速率。
 *
 * @param dac12     指向DAC12寄存器结构体的指针
 * @param sampleRate 期望的采样率。可选 @ref DL_DAC12_SAMPLES_PER_SECOND
 */
__STATIC_INLINE void DL_DAC12_setSampleRate(
    DAC12_Regs *dac12, DL_DAC12_SAMPLES_PER_SECOND sampleRate)
{
    DL_Common_updateReg(
        &dac12->CTL3, (uint32_t) sampleRate, DAC12_CTL3_STIMCONFIG_MASK);
}

/**
 * @brief 检查校准序列是否正在运行
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @return  当前活动校准序列状态
 *
 * @retval  true   校准序列正在运行
 * @retval  false  校准序列未运行
 *
 * @see DL_DAC12_startCalibration
 */
__STATIC_INLINE bool DL_DAC12_isCalibrationRunning(const DAC12_Regs *dac12)
{
    uint32_t t = (dac12->CALCTL & DAC12_CALCTL_CALON_MASK);
    return (t == DAC12_CALCTL_CALON_ACTIVE);
}

/**
 * @brief 启动DAC偏移误差校准序列
 *
 * 启动DAC偏移误差校准序列，当偏移误差校准完成时，
 * CALON_ACTIVE位自动复位。完成后，正确的校准值将被写入CALDATA寄存器。
 * 此序列可以在模块就绪后立即启动(动态启动)，也可以在设置期间启动
 * (在使能DAC后直接调用)。
 *
 * 为了使校准成功，AMP必须已配置，且LOCK位必须被清除才能成功写入值。
 * 校准期间输出也为三态。
 *
 * 负偏移意味着低输入值将输出为0。
 * 正偏移意味着输入为0将导致非零输出。
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 */
__STATIC_INLINE void DL_DAC12_startCalibration(DAC12_Regs *dac12)
{
    dac12->CALCTL =
        (DAC12_CALCTL_CALON_ACTIVE | DAC12_CALCTL_CALSEL_SELFCALIBRATIONTRIM);
}

/**
 * @brief 获取DAC校准偏移量
 *
 * 只能在校准序列完成后才能读取CALDATA寄存器。
 * 校准期间，此寄存器被连续写入，可能获取不准确的值。
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @return  当前误差校准偏移量
 *
 * @retval  -64 到 +63 (二进制补码)
 */
__STATIC_INLINE uint32_t DL_DAC12_getCalibrationData(const DAC12_Regs *dac12)
{
    return (dac12->CALDATA & DAC12_CALDATA_DATA_MASK);
}

/**
 * @brief 执行校准序列
 *
 * 使能写入，然后启动校准序列。只应在DAC模块使能后调用。
 * 此函数只在校准序列完成后才返回。
 *
 * @pre 应先配置放大器设置 @ref DL_DAC12_setAmplifier
 * @post 校准完成，CALDATA被锁定以防止写入
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 */
void DL_DAC12_performSelfCalibrationBlocking(DAC12_Regs *dac12);

/**
 * @brief 输出8位数据值
 *
 * 使用CPU控制DAC，当FIFO禁用时可以写入此寄存器以获得固定输出电压
 * (如直流生成)，或通过使能FIFO并写入此寄存器来实现可变输出(如交流生成)。
 * 值将被内部写入FIFO。使能FIFO后 (@ref DL_DAC12_enableFIFO)，
 * DAC内部会产生FIFO特定的中断。
 *
 * 使用CPU向DAC加载数据时，必须保持DMA触发生成机制禁用
 *
 * @param dac12       指向DAC12寄存器结构体的指针
 * @param dataValue   要写入的值，范围0x00到0xFF。可以是二进制或二进制补码
 */
__STATIC_INLINE void DL_DAC12_output8(DAC12_Regs *dac12, uint8_t dataValue)
{
    dac12->DATA0 = dataValue;
}

/**
 * @brief 输出12位数据值
 *
 * 使用CPU控制DAC，当FIFO禁用时可以写入此寄存器以获得固定输出电压
 * (如直流生成)，或通过使能FIFO并写入此寄存器来实现可变输出(如交流生成)。
 * 值将被内部写入FIFO。使能FIFO后 (@ref DL_DAC12_enableFIFO)，
 * DAC内部会产生FIFO特定的中断。
 *
 * 使用CPU向DAC加载数据时，必须保持DMA触发生成机制禁用
 *
 * @param dac12       指向DAC12寄存器结构体的指针
 * @param dataValue   要写入的值，范围0x000到0xFFF。可以是二进制或二进制补码
 */
__STATIC_INLINE void DL_DAC12_output12(DAC12_Regs *dac12, uint32_t dataValue)
{
    dac12->DATA0 = (dataValue & DAC12_DATA0_DATA_VALUE_MASK);
}

/**
 * @brief 用8位数据填充DAC FIFO
 *
 * 从数据缓冲区尽可能多地向FIFO写入值，然后在计数匹配或FIFO满时退出
 *
 * @param dac12       指向DAC12寄存器结构体的指针
 * @param buffer      包含要写入数据值 (0x00 - 0xFF) 的数组
 * @param count       缓冲区长度，或最大期望传输数量
 *
 * @return 成功传输到FIFO的计数
 *
 * @retval 0 - min(空FIFO槽数量, count)
 */
uint32_t DL_DAC12_fillFIFO8(
    DAC12_Regs *dac12, const uint8_t *buffer, uint32_t count);

/**
 * @brief 用12位数据填充DAC FIFO
 *
 * 从数据缓冲区尽可能多地向FIFO写入值，然后在计数匹配或FIFO满时退出
 *
 * @param dac12       指向DAC12寄存器结构体的指针
 * @param buffer      包含要写入数据值 (0x00 - 0xFFF) 的数组
 * @param count       缓冲区长度，或最大期望传输数量
 *
 * @return 成功传输到FIFO的计数
 *
 * @retval 0 - min(空FIFO槽数量, count)
 */
uint32_t DL_DAC12_fillFIFO12(
    DAC12_Regs *dac12, const uint16_t *buffer, uint32_t count);

/**
 *  @brief      Blocking 8-bit output to the DAC FIFO
 *
 *  Waits until the FIFO is not full, then writes a single value
 *
 *  @param[in]  dac12        pointer to the register overlay of the peripheral
 *  @param[in]  data         data value (0x00 - 0xFF) to be written
 */
void DL_DAC12_outputBlocking8(DAC12_Regs *dac12, uint8_t data);

/**
 *  @brief      Blocking 12-bit output to the DAC FIFO
 *
 *  Waits until the FIFO is not full, then writes a single value
 *
 *  @param[in]  dac12        pointer to the register overlay of the peripheral
 *  @param[in]  data         data value (0x000 - 0xFFF) to be written
 */
void DL_DAC12_outputBlocking12(DAC12_Regs *dac12, uint16_t data);

/**
 *  @brief      Checks the raw interrupt status of one or more interrupts
 *
 *  The raw interrupt status is independent of whether a specific interrupt is
 *  enabled.
 *
 *  @param[in]  dac12         pointer to the register overlay of the peripheral
 *  @param[in]  interruptMask the desired interrupt(s). One or more of
 *                            @ref DL_DAC12_INTERRUPT
 *
 *  @return     currently asserted interrupts (whether enabled or not) bit-wise
 *              AND with the inputted interrupts
 *
 *  @retval     0       no interrupts that were inputted are currently asserted
 *  @retval     not 0   a bit-wise OR of the inputted @ref DL_DAC12_INTERRUPT
 *                      registers that are currently asserted.
 */
__STATIC_INLINE uint32_t DL_DAC12_getInterruptStatus(
    const DAC12_Regs *dac12, uint32_t interruptMask)
{
    return (dac12->CPU_INT.RIS & interruptMask);
}

/**
 * @brief 清除一个或多个中断的状态标志
 *
 * 访问仅写的中断清除(ICLR)寄存器并向指定位写入1。
 * 任何写入1的位将清除当前待处理的中断。
 *
 * @param dac12         指向DAC12寄存器结构体的指针
 * @param interruptMask 期望的中断(s)。可以是 @ref DL_DAC12_INTERRUPT 的一个或多个
 */
__STATIC_INLINE void DL_DAC12_clearInterruptStatus(
    DAC12_Regs *dac12, uint32_t interruptMask)
{
    dac12->CPU_INT.ICLR = interruptMask;
}

/**
 * @brief 使能一个或多个中断
 *
 * 访问并写入IMASK寄存器以使能指定的中断。
 *
 * @param dac12         指向DAC12寄存器结构体的指针
 * @param interruptMask 期望的中断(s)。可以是 @ref DL_DAC12_INTERRUPT 的一个或多个
 */
__STATIC_INLINE void DL_DAC12_enableInterrupt(
    DAC12_Regs *dac12, uint32_t interruptMask)
{
    dac12->CPU_INT.IMASK |= interruptMask;
}

/**
 * @brief 禁用一个或多个中断
 *
 * 访问并写入IMASK寄存器以禁用指定的中断。
 *
 * @param dac12         指向DAC12寄存器结构体的指针
 * @param interruptMask 期望的中断(s)。可以是 @ref DL_DAC12_INTERRUPT 的一个或多个
 */
__STATIC_INLINE void DL_DAC12_disableInterrupt(
    DAC12_Regs *dac12, uint32_t interruptMask)
{
    dac12->CPU_INT.IMASK &= ~interruptMask;
}

/**
 * @brief 获取最高优先级待处理中断
 *
 * 访问并读取IIDX寄存器以获取当前待处理的最高优先级中断。
 * 偏移量较小的中断(值较小)比偏移量较大的中断(值较大)优先级更高。
 * 要使中断处于待处理状态，必须已启用该中断并已触发。
 * 清除中断将取消待处理状态。
 *
 * @param dac12        指向DAC12寄存器结构体的指针
 *
 * @return 最高优先级待处理中断
 *
 * @retval @ref DL_DAC12_IIDX
 */
__STATIC_INLINE DL_DAC12_IIDX DL_DAC12_getPendingInterrupt(
    const DAC12_Regs *dac12)
{
    return ((DL_DAC12_IIDX) dac12->CPU_INT.IIDX);
}

/**
 * @brief 检查DAC FIFO是否已满
 *
 * 访问原始中断状态(RIS)寄存器以获取FIFO满中断标志的当前状态。
 *
 * @param dac12        指向DAC12寄存器结构体的指针
 *
 * @return FIFO满中断标志状态
 *
 * @retval  true   FIFO当前已满
 * @retval  false  FIFO未满
 */
__STATIC_INLINE bool DL_DAC12_isFIFOFull(const DAC12_Regs *dac12)
{
    uint32_t t =
        DL_DAC12_getInterruptStatus(dac12, DL_DAC12_INTERRUPT_FIFO_FULL);
    return (t == DL_DAC12_INTERRUPT_FIFO_FULL);
}

/**
 * @brief 设置事件发布者通道ID
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 * @param chanID  通道ID号。有效范围0-15。如果ChanID==0，
 *                发布者已断开连接。
 */
__STATIC_INLINE void DL_DAC12_setPublisherChanID(
    DAC12_Regs *dac12, uint8_t chanID)
{
    dac12->FPUB_1 = (chanID & DAC12_FPUB_1_CHANID_MAXIMUM);
}

/**
 * @brief 获取事件发布者通道ID
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 *
 * @return 事件发布者通道ID
 */
__STATIC_INLINE uint8_t DL_DAC12_getPublisherChanID(const DAC12_Regs *dac12)
{
    return ((uint8_t)((dac12->FPUB_1) & DAC12_FPUB_1_CHANID_MASK));
}

/**
 * @brief 设置事件订阅者通道ID
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 * @param index   指定要配置的寄存器事件索引
 * @param chanID  通道ID号。有效范围0-15。如果ChanID==0，
 *                订阅者已断开连接。
 */
__STATIC_INLINE void DL_DAC12_setSubscriberChanID(
    DAC12_Regs *dac12, DL_DAC12_SUBSCRIBER_INDEX index, uint8_t chanID)
{
    volatile uint32_t *pReg = &dac12->FSUB_0;

    *(pReg + (uint32_t) index) = (chanID & DAC12_FSUB_0_CHANID_MAXIMUM);
}

/**
 * @brief 获取事件订阅者通道ID
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 * @param index   指定要读取的寄存器事件索引
 *
 * @return 事件订阅者通道ID
 */
__STATIC_INLINE uint8_t DL_DAC12_getSubscriberChanID(
    DAC12_Regs *dac12, DL_DAC12_SUBSCRIBER_INDEX index)
{
    volatile uint32_t *pReg = &dac12->FSUB_0;

    return ((uint8_t)(*(pReg + (uint32_t) index) & DAC12_FSUB_0_CHANID_MASK));
}

/**
 * @brief 使能DAC事件
 *
 * @param dac12        指向DAC12寄存器结构体的指针
 * @param eventMask    要使能的事件位掩码。
 *                    可以是 @ref DL_DAC12_EVENT 的位或组合。
 */
__STATIC_INLINE void DL_DAC12_enableEvent(
    DAC12_Regs *dac12, uint32_t eventMask)
{
    dac12->GEN_EVENT.IMASK |= (eventMask);
}

/**
 * @brief 禁用DAC事件
 *
 * @param dac12        指向DAC12寄存器结构体的指针
 * @param eventMask    要禁用的事件位掩码。
 *                    可以是 @ref DL_DAC12_EVENT 的位或组合。
 */
__STATIC_INLINE void DL_DAC12_disableEvent(
    DAC12_Regs *dac12, uint32_t eventMask)
{
    dac12->GEN_EVENT.IMASK &= ~(eventMask);
}

/**
 * @brief 检查哪些DAC事件已使能
 *
 * @param dac12        指向DAC12寄存器结构体的指针
 * @param eventMask    要检查的事件位掩码。
 *                    可以是 @ref DL_DAC12_EVENT 的位或组合。
 *
 * @return 哪些请求的DAC中断已使能
 *
 * @retval @ref DL_DAC12_EVENT 值的位或组合
 */
__STATIC_INLINE uint32_t DL_DAC12_getEnabledEvents(
    const DAC12_Regs *dac12, uint32_t eventMask)
{
    return ((dac12->GEN_EVENT.IMASK) & (eventMask));
}

/**
 * @brief 检查已使能的DAC事件标志
 *
 * 检查之前使能的任何DAC事件是否待处理。
 *
 * @param dac12        指向DAC12寄存器结构体的指针
 * @param eventMask    要检查的事件位掩码。
 *                    可以是 @ref DL_DAC12_EVENT 的位或组合。
 *
 * @return 哪些请求的DAC中断待处理
 *
 * @retval @ref DL_DAC12_EVENT 值的位或组合
 *
 * @see DL_DAC12_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_DAC12_getEnabledEventStatus(
    const DAC12_Regs *dac12, uint32_t eventMask)
{
    return ((dac12->GEN_EVENT.MIS) & eventMask);
}

/**
 * @brief 检查任意DAC事件的中断标志
 *
 * 检查是否有任何事件待处理。事件不必先前已使能。
 *
 * @param dac12        指向DAC12寄存器结构体的指针
 * @param eventMask    要检查的事件位掩码。
 *                    可以是 @ref DL_DAC12_EVENT 的位或组合。
 *
 * @return 哪些请求的DAC中断待处理
 *
 * @retval @ref DL_DAC12_EVENT 值的位或组合
 */
__STATIC_INLINE uint32_t DL_DAC12_getRawEventsStatus(
    const DAC12_Regs *dac12, uint32_t eventMask)
{
    return ((dac12->GEN_EVENT.RIS) & eventMask);
}

/**
 * @brief 清除待处理的DAC事件
 *
 * @param dac12        指向DAC12寄存器结构体的指针
 * @param eventMask    要清除的事件位掩码。
 *                    可以是 @ref DL_DAC12_EVENT 的位或组合。
 */
__STATIC_INLINE void DL_DAC12_clearEventsStatus(
    DAC12_Regs *dac12, uint32_t eventMask)
{
    dac12->GEN_EVENT.ICLR |= (eventMask);
}

#ifdef __cplusplus
}
#endif
#endif /* __MSPM0_HAS_DAC12__ */

#endif /* ti_dl_dl_dac12__include */
/** @}*/
