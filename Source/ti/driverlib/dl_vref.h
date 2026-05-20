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
 *  @file       dl_vref.h
 *  @brief      电压基准（VREF）驱动库
 *  @defgroup   VREF 电压基准（VREF）
 *
 *  @anchor ti_dl_dl_vref_Overview
 *  # 概述
 *
 *  电压基准驱动库用于对 MSPM0 的 VREF 模块进行完整配置。
 *  VREF 模块包含一个可配置的电压基准缓冲器，用于为片上模拟外设
 *  提供稳定的内部参考电压。同时也支持接入外部参考电压，
 *  适用于需要更高精度的应用。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup VREF
 * @{
 */
#ifndef ti_dl_dl_m0p_vref__include
#define ti_dl_dl_m0p_vref__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_VREF__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_VREF_CTL1_READY
 * @{
 */

/*!
* @brief CTL1 中指示模块就绪的 VREF 位未置位（未就绪）
*/
#define DL_VREF_CTL1_READY_NOTRDY    (VREF_CTL1_READY_NOTRDY)

/*!
* @brief CTL1 中指示模块就绪的 VREF 位置位（已就绪）
*/
#define DL_VREF_CTL1_READY_RDY    (VREF_CTL1_READY_RDY)


/** @}*/

/** @addtogroup DL_VREF_SH
 * @{
 */

/*!
* @brief VREF 采样保持（若启用）时的最小取值
*/
#define DL_VREF_SH_MIN             (VREF_CTL2_SHCYCLE_MINIMUM)

/*!
* @brief VREF 采样保持（若启用）时的最大取值
*/
#define DL_VREF_SH_MAX             (VREF_CTL2_SHCYCLE_MAXIMUM)

/** @}*/

/** @addtogroup DL_VREF_HOLD
 * @{
 */

/*!
* @brief VREF 保持时间（若启用）时的最小取值
*/
#define DL_VREF_HOLD_MIN             (0x0000U)

/*!
* @brief VREF 保持时间（若启用）时的最大取值
*/
#define DL_VREF_HOLD_MAX             (0xFFFFU)

/** @}*/

/* clang-format on */

/*! @enum DL_VREF_CLOCK
 *  @brief VREF 时钟源选择
 */
typedef enum {
    /*! 将 VREF 时钟源设置为 LFCLK  */
    DL_VREF_CLOCK_LFCLK = VREF_CLKSEL_LFCLK_SEL_MASK,
    /*! 将 VREF 时钟源设置为 MFCLK  */
    DL_VREF_CLOCK_MFCLK = VREF_CLKSEL_MFCLK_SEL_MASK,
    /*! 将 VREF 时钟源设置为 BUSCLK  */
    DL_VREF_CLOCK_BUSCLK = VREF_CLKSEL_BUSCLK_SEL_MASK,
} DL_VREF_CLOCK;

/*! @enum DL_VREF_CLOCK_DIVIDE
 *  @brief VREF 时钟分频配置
 */
typedef enum {
    /*! 时钟分频为 1 */
    DL_VREF_CLOCK_DIVIDE_1 = 0,
    /*! 时钟分频为 2 */
    DL_VREF_CLOCK_DIVIDE_2 = 1,
    /*! 时钟分频为 4 */
    DL_VREF_CLOCK_DIVIDE_4 = 3,
    /*! 时钟分频为 6 */
    DL_VREF_CLOCK_DIVIDE_6 = 5,
    /*! 时钟分频为 8 */
    DL_VREF_CLOCK_DIVIDE_8 = 7,
} DL_VREF_CLOCK_DIVIDE;

/*! @enum DL_VREF_ENABLE
 *  @brief VREF 使能配置
 */
typedef enum {
    /*! VREF 控制设置为禁用 */
    DL_VREF_ENABLE_DISABLE = VREF_CTL0_ENABLE_DISABLE,
    /*! VREF 控制设置为启用 */
    DL_VREF_ENABLE_ENABLE = VREF_CTL0_ENABLE_ENABLE,
} DL_VREF_ENABLE;

/*! @enum DL_VREF_BUFCONFIG
 *  @brief VREF 输出缓冲配置
 */
typedef enum {
    /*! 将 VREF 输出缓冲配置为 2.5V */
    DL_VREF_BUFCONFIG_OUTPUT_2_5V = VREF_CTL0_BUFCONFIG_OUTPUT2P5V,
    /*! 将 VREF 输出缓冲配置为 1.4V */
    DL_VREF_BUFCONFIG_OUTPUT_1_4V = VREF_CTL0_BUFCONFIG_OUTPUT1P4V,
} DL_VREF_BUFCONFIG;

/*! @enum DL_VREF_SHMODE
 *  @brief VREF 采样保持模式配置
 */
typedef enum {
    /*! 将 VREF 采样保持模式设置为禁用 */
    DL_VREF_SHMODE_DISABLE = VREF_CTL0_SHMODE_DISABLE,
    /*! 将 VREF 采样保持模式设置为启用 */
    DL_VREF_SHMODE_ENABLE = VREF_CTL0_SHMODE_ENABLE,
} DL_VREF_SHMODE;

/*!
 *  @brief  @ref DL_VREF_setClockConfig 的配置结构体。
 */
typedef struct {
    /*! 选择 VREF 模块时钟源 @ref DL_VREF_CLOCK */
    DL_VREF_CLOCK clockSel;
    /*! 选择分频系数，取值为 @ref DL_VREF_CLOCK_DIVIDE 之一 */
    DL_VREF_CLOCK_DIVIDE divideRatio;
} DL_VREF_ClockConfig;

/**
 * @brief @ref DL_VREF_Config 的配置结构体。
 */
typedef struct {
    /*! VREF 使能设置。取值为 @ref DL_VREF_ENABLE_DISABLE 或 @ref DL_VREF_ENABLE_ENABLE */
    DL_VREF_ENABLE vrefEnable;
    /*! VREF 缓冲配置设置。取值为 @ref DL_VREF_BUFCONFIG_OUTPUT_1_4V 或 @ref DL_VREF_BUFCONFIG_OUTPUT_2_5V */
    DL_VREF_BUFCONFIG bufConfig;
    /*! VREF 采样保持使能配置。取值为 @ref DL_VREF_SHMODE_DISABLE 或 @ref DL_VREF_SHMODE_ENABLE */
    DL_VREF_SHMODE shModeEnable;
    /*! 若启用采样保持模式，用于采样保持的时钟周期数。最小值 @ref DL_VREF_SH_MIN，最大值 @ref DL_VREF_SH_MAX */
    uint16_t shCycleCount;
    /*! 若启用采样保持模式，用于保持的时钟周期数。最小值 @ref DL_VREF_HOLD_MIN，最大值 @ref DL_VREF_HOLD_MAX */
    uint16_t holdCycleCount;
} DL_VREF_Config;

/**
 * @brief 使能 VREF 外设写使能（PWREN）寄存器
 *
 *  在软件能够配置任意外设寄存器之前，必须先向外设的 PWREN 寄存器中
 *  写入正确的 KEY 值并同时置位 ENABLE 位，以此使能该外设。
 *
 *  @note 若需节省功耗，请参考 @ref DL_VREF_enableInternalRef
 *
 * @param vref       指向该外设寄存器映射结构的指针
 */
__STATIC_INLINE void DL_VREF_enablePower(VREF_Regs *vref)
{
    vref->GPRCM.PWREN = VREF_PWREN_KEY_UNLOCK_W | VREF_PWREN_ENABLE_ENABLE;
}

/**
 * @brief 禁用 VREF 外设写使能（PWREN）寄存器
 *
 *  当 PWREN.ENABLE 位被清除时，该外设的寄存器将无法进行读写访问。
 *
 *  @note 此 API 本身并不能带来很大的功耗节省。若需节省功耗，
 *        请参考 @ref DL_VREF_disableInternalRef
 *
 * @param vref       指向该外设寄存器映射结构的指针
 */
__STATIC_INLINE void DL_VREF_disablePower(VREF_Regs *vref)
{
    vref->GPRCM.PWREN = VREF_PWREN_KEY_UNLOCK_W | VREF_PWREN_ENABLE_DISABLE;
}
/**
 * @brief 返回 VREF 外设写使能（PWREN）寄存器是否被使能
 *
 *  在软件能够配置任意外设寄存器之前，必须先向外设的 PWREN 寄存器中
 *  写入正确的 KEY 值并同时置位 ENABLE 位，以此使能该外设。
 *
 *  当 PWREN.ENABLE 位被清除时，该外设的寄存器将无法进行读写访问。
 *
 * @param vref        指向该外设寄存器映射结构的指针
 *
 * @return true  当外设寄存器访问被使能时返回
 * @return false 当外设寄存器访问被禁用时返回
 */
__STATIC_INLINE bool DL_VREF_isPowerEnabled(VREF_Regs *vref)
{
    return ((VREF->GPRCM.PWREN & VREF_PWREN_ENABLE_MASK) ==
            VREF_PWREN_ENABLE_ENABLE);
}

/**
 *  @brief      使能内部 VREF
 *
 *  此函数只使能内部 VREF，不会配置所有相关设置。
 *  推荐使用 @ref DL_VREF_configReference 对 VREF 进行完整配置并启用。
 *
 *  @param vref       指向该外设寄存器映射结构的指针
 *
 */
__STATIC_INLINE void DL_VREF_enableInternalRef(VREF_Regs *vref)
{
    vref->CTL0 |= VREF_CTL0_ENABLE_ENABLE;
}

/**
 *  @brief      禁用内部 VREF，以便使用外部 VREF
 *
 *  在使用外部 VREF 输入时，必须先禁用内部 VREF。
 *
 *  @param vref       指向该外设寄存器映射结构的指针
 *
 */
__STATIC_INLINE void DL_VREF_disableInternalRef(VREF_Regs *vref)
{
    vref->CTL0 &= ~VREF_CTL0_ENABLE_MASK;
}

/**
 *  @brief      检查内部 VREF 是否已被使能
 *
 *  @param vref       指向该外设寄存器映射结构的指针
 *
 *  @return     返回 VREF 外设的使能状态
 *
 *  @retval     true  内部 VREF 已使能
 *  @retval     false 内部 VREF 已禁用，可使用外部 VREF
 */
__STATIC_INLINE bool DL_VREF_isEnabled(const VREF_Regs *vref)
{
    return ((vref->CTL0 & VREF_CTL0_ENABLE_MASK) == VREF_CTL0_ENABLE_ENABLE);
}

#if defined(VREF_SYS_INT_COMP_REF) && (VREF_SYS_INT_COMP_REF == 1)
/**
 *  @brief      使能 VREF 缓冲器作为比较器的内部参考输入
 *
 * @param vref       指向该外设寄存器映射结构的指针
 */
__STATIC_INLINE void DL_VREF_enableInternalRefCOMP(VREF_Regs *vref)
{
    vref->CTL0 |= VREF_CTL0_COMP_VREF_ENABLE_ENABLE;
}

/**
 *  @brief      检查用于比较器的 VREF 缓冲器是否已使能
 *
 * @param vref       指向该外设寄存器映射结构的指针
 *
 *  @return     返回比较器 VREF 缓冲器是否已使能
 *
 *  @retval     true  用于比较器的 VREF 缓冲器已使能
 *  @retval     false 用于比较器的 VREF 缓冲器已禁用
 */
__STATIC_INLINE bool DL_VREF_isInternalRefCOMPEnabled(const VREF_Regs *vref)
{
    return ((vref->CTL0 & VREF_CTL0_COMP_VREF_ENABLE_MASK) ==
            VREF_CTL0_COMP_VREF_ENABLE_ENABLE);
}

/**
 *  @brief      禁用 VREF 缓冲器作为比较器的内部参考输入
 *
 * @param vref       指向该外设寄存器映射结构的指针
 */
__STATIC_INLINE void DL_VREF_disableInternalRefCOMP(VREF_Regs *vref)
{
    vref->CTL0 &= ~(VREF_CTL0_COMP_VREF_ENABLE_MASK);
}
#endif

#if defined(VREF_SYS_INT_ADC_REF) && (VREF_SYS_INT_ADC_REF == 1)
/**
 *  @brief      使能 VREF 缓冲器作为 ADC 的内部参考输入
 *
 * @param vref       指向该外设寄存器映射结构的指针
 */
__STATIC_INLINE void DL_VREF_enableInternalRefADC(VREF_Regs *vref)
{
    vref->CTL0 |= VREF_CTL0_ADC_VREF_ENABLE_ENABLE;
}

/**
 *  @brief      检查用于 ADC 的 VREF 缓冲器是否已使能
 *
 * @param vref       指向该外设寄存器映射结构的指针
 *
 *  @return     返回 ADC VREF 缓冲器是否已使能
 *
 *  @retval     true  用于 ADC 的 VREF 缓冲器已使能
 *  @retval     false 用于 ADC 的 VREF 缓冲器已禁用
 */
__STATIC_INLINE bool DL_VREF_isInternalRefADCEnabled(const VREF_Regs *vref)
{
    return ((vref->CTL0 & VREF_CTL0_ADC_VREF_ENABLE_MASK) ==
            VREF_CTL0_ADC_VREF_ENABLE_ENABLE);
}

/**
 *  @brief      禁用 VREF 缓冲器作为 ADC 的内部参考输入
 *
 * @param vref       指向该外设寄存器映射结构的指针
 */
__STATIC_INLINE void DL_VREF_disableInternalRefADC(VREF_Regs *vref)
{
    vref->CTL0 &= ~(VREF_CTL0_ADC_VREF_ENABLE_MASK);
}
#endif

/**
 * @brief 设置 VREF 中的时钟源选择和时钟分频字段
 *
 * @param vref       指向该外设寄存器映射结构的指针
 *
 * @param config     指向配置结构体的指针
 */
void DL_VREF_setClockConfig(
    VREF_Regs *vref, const DL_VREF_ClockConfig *config);

/**
 * @brief 将 VREF 中的时钟源选择和时钟分频字段拷贝到配置结构体中
 *
 * @param vref       指向该外设寄存器映射结构的指针
 *
 * @param config     指向配置结构体的指针
 */
void DL_VREF_getClockConfig(
    const VREF_Regs *vref, DL_VREF_ClockConfig *config);

/**
 * @brief 复位 VREF 模块
 *
 * @param vref       指向该外设寄存器映射结构的指针
 */
__STATIC_INLINE void DL_VREF_reset(VREF_Regs *vref)
{
    vref->GPRCM.RSTCTL =
        VREF_RSTCTL_RESETASSERT_MASK | VREF_RSTCTL_KEY_UNLOCK_W;
}

/**
 * @brief 返回 VREF 外设是否曾被复位
 *
 * @param vref        指向 VREF 外设寄存器映射结构的指针
 *
 * @return true  外设发生过复位
 * @return false 外设未发生复位
 *
 */
__STATIC_INLINE bool DL_VREF_isReset(const VREF_Regs *vref)
{
    return (vref->GPRCM.STAT & VREF_STAT_RESETSTKY_MASK) ==
           VREF_STAT_RESETSTKY_RESET;
}

/**
 * @brief 返回 VREF 寄存器状态
 *
 * @param vref        指向 VREF 外设寄存器映射结构的指针
 *
 * @return VREF 状态位，取值为 @ref DL_VREF_CTL1_READY_NOTRDY 或 @ref DL_VREF_CTL1_READY_RDY
 *
 */
__STATIC_INLINE uint32_t DL_VREF_getStatus(const VREF_Regs *vref)
{
    return vref->CTL1 & VREF_CTL1_READY_MASK;
}

/**
 * @brief 配置 VREF 使能与控制寄存器
 *
 * @param vref        指向 VREF 外设寄存器映射结构的指针
 *
 * @param config      指向 @ref DL_VREF_Config 结构体的指针，用于配置外设
 *
 */
void DL_VREF_configReference(VREF_Regs *vref, const DL_VREF_Config *config);

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_VREF__ */

#endif /* ti_dl_dl_m0p_vref__include */
/** @}*/
