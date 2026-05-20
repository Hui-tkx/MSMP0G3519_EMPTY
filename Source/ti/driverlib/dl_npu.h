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
 * 本软件由版权持有人和贡献者按“原样”提供，对任何明示或暗示的担保
 * （包括但不限于适销性和特定用途适用性的默示担保）不作任何保证。
 * 在任何情况下，版权持有人或贡献者均不对因使用本软件而导致的任何
 * 直接、间接、偶发、特殊、惩戒性或后果性损害（包括但不限于采购替代
 * 商品或服务；使用损失、数据丢失或利润损失；业务中断等）承担责任，
 * 无论其是基于合同、严格责任还是侵权行为（包括疏忽或其他原因），
 * 即使已被告知可能会发生此类损害。
 */
/*!****************************************************************************
 *  @file       dl_npu.h
 *  @brief      神经网络处理单元 (NPU) 驱动库
 *  @defgroup   神经网络处理单元 (NPU)
 *
 *  @anchor ti_dl_dl_npu_Overview
 *  # 概述
 *
 *  本文件提供 NPU 外设的底层驱动接口，用于：
 *    - 控制 NPU 外设电源开关（PWREN）
 *    - 管理 NPU 中断（使能、查询、清除等）
 *    - 获取 NPU 控制寄存器、累加器寄存器以及指令/参数存储区的物理地址
 *
 *  其中，电源控制和中断管理相关 API 通常由用户应用直接调用；
 *  控制寄存器和 NPU 内存地址访问函数一般由上层 NPU 算法库内部使用。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup NPU
 * @{
 */
#ifndef ti_dl_dl_npu__include
#define ti_dl_dl_npu__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>

#ifdef __MSPM0_HAS_NPU__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_NPU_INTERRUPT
 *  @{
 */

/*!
 * @brief NPU 计算完成中断
 */
#define DL_NPU_INTERRUPT_DONE (NPU_IMASK_DONE_SET)

/** @}*/

/* clang-format on */

/*! @enum DL_NPU_IIDX
 *  @brief NPU 中断索引枚举（读取 IIDX 寄存器时使用）
 */
typedef enum {
    /*! 无中断挂起 */
    DL_NPU_IIDX_NO_INTERRUPT = NPU_IIDX_STAT_NO_INTR,
    /*! NPU 计算完成中断挂起 */
    DL_NPU_IIDX_OUTPUT_READY = NPU_IIDX_STAT_DONE
} DL_NPU_IIDX;

/**
 * @brief 使能 NPU 外设写使能（PWREN），打开 NPU 电源访问
 *
 * 在软件配置任意 NPU 寄存器之前，必须先向 NPU 的 PWREN 寄存器写入
 * 正确的 KEY 与 ENABLE 位以打开访问权限。
 *
 * @param npu        指向 NPU 寄存器结构体的指针
 */
__STATIC_INLINE void DL_NPU_enablePower(NPU_Regs *npu)
{
    npu->GPRCM.PWREN = (NPU_PWREN_KEY_UNLOCK_W | NPU_PWREN_ENABLE_ENABLE);
}

/**
 * @brief 关闭 NPU 外设写使能（PWREN），禁止访问 NPU 寄存器和内存
 *
 * 清除 PWREN.ENABLE 位后，不能再读写 NPU 寄存器和 NPU 内存。
 *
 * @note 注意：此 API 对整体功耗的影响有限。
 *
 * @param npu        指向 NPU 寄存器结构体的指针
 */
__STATIC_INLINE void DL_NPU_disablePower(NPU_Regs *npu)
{
    npu->GPRCM.PWREN = (NPU_PWREN_KEY_UNLOCK_W | NPU_PWREN_ENABLE_DISABLE);
}

/**
 * @brief 查询 NPU 外设写使能（PWREN）当前是否已打开
 *
 * 在配置 NPU 之前必须保证 PWREN 已经被正确使能。
 *
 * 当 PWREN.ENABLE 位被清除时，NPU 寄存器将无法进行读写操作。
 *
 * @param npu        指向 NPU 寄存器结构体的指针
 *
 * @return true  如果允许访问 NPU 寄存器
 * @return false 如果禁止访问 NPU 寄存器
 */
__STATIC_INLINE bool DL_NPU_isPowerEnabled(const NPU_Regs *npu)
{
    return (
        (npu->GPRCM.PWREN & NPU_PWREN_ENABLE_MASK) == NPU_PWREN_ENABLE_ENABLE);
}

/**
 * @brief 对 NPU 外设进行复位
 *
 * @param npu        指向 NPU 寄存器结构体的指针
 */
__STATIC_INLINE void DL_NPU_reset(NPU_Regs *npu)
{
    npu->GPRCM.RSTCTL =
        (NPU_RSTCTL_KEY_UNLOCK_W | NPU_RSTCTL_RESETSTKYCLR_CLR |
            NPU_RSTCTL_RESETASSERT_ASSERT);
}

/**
 * @brief 查询 NPU 是否发生过复位（复位标志）
 *
 * @param npu        指向 NPU 寄存器结构体的指针
 *
 * @return true  如果 NPU 曾被复位
 * @return false 如果 NPU 未被复位
 */
__STATIC_INLINE bool DL_NPU_isReset(const NPU_Regs *npu)
{
    return ((npu->GPRCM.STAT & NPU_STAT_RESETSTKY_MASK) ==
            NPU_STAT_RESETSTKY_RESET);
}

/**
 *  @brief      使能 NPU 中断
 *
 *  @param[in]  npu            指向 NPU 寄存器结构体的指针
 *
 *  @param[in]  interruptMask  要使能的中断位掩码，可按位或组合 @ref DL_NPU_INTERRUPT
 */
__STATIC_INLINE void DL_NPU_enableInterrupt(
    NPU_Regs *npu, uint32_t interruptMask)
{
    npu->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      关闭 NPU 中断
 *
 *  @param[in]  npu            指向 NPU 寄存器结构体的指针
 *
 *  @param[in]  interruptMask  要关闭的中断位掩码，可按位或组合 @ref DL_NPU_INTERRUPT
 */
__STATIC_INLINE void DL_NPU_disableInterrupt(
    NPU_Regs *npu, uint32_t interruptMask)
{
    npu->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      查询哪些 NPU 中断已经被使能
 *
 *  @param[in]  npu            指向 NPU 寄存器覆盖结构的指针
 *
 *  @param[in]  interruptMask  要检查的中断位掩码，可按位或组合 @ref DL_NPU_INTERRUPT
 *
 *  @return     传入掩码中，哪些位当前处于“已使能”状态
 *
 *  @retval     @ref DL_NPU_INTERRUPT 按位或的结果
 */
__STATIC_INLINE uint32_t DL_NPU_getEnabledInterrupts(
    NPU_Regs *npu, uint32_t interruptMask)
{
    return (npu->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      查询“已使能”的 NPU 中断中，哪些当前处于挂起状态
 *
 *  仅检查先前已使能的中断，并返回其中处于挂起状态的中断位。
 *
 *  @param[in]  npu            指向 NPU 寄存器覆盖结构的指针
 *
 *  @param[in]  interruptMask  要检查的中断位掩码，可按位或组合
 *                             @ref DL_NPU_INTERRUPT.
 *
 *  @return     传入掩码中，哪些位当前挂起
 *
 *  @retval     @ref DL_NPU_INTERRUPT 按位或的结果
 *
 *  @sa         DL_NPU_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_NPU_getEnabledInterruptStatus(
    NPU_Regs *npu, uint32_t interruptMask)
{
    return (npu->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      查询任意 NPU 中断的原始挂起状态
 *
 *  检查所有 NPU 中断是否挂起，与是否已使能无关。
 *
 *  @param[in]  npu            指向 NPU 寄存器覆盖结构的指针
 *
 *  @param[in]  interruptMask  要检查的中断位掩码，可按位或组合
 *                             @ref DL_NPU_INTERRUPT.
 *
 *  @return     传入掩码中，哪些位当前挂起
 *
 *  @retval     @ref DL_NPU_INTERRUPT 按位或的结果
 */
__STATIC_INLINE uint32_t DL_NPU_getRawInterruptStatus(
    NPU_Regs *npu, uint32_t interruptMask)
{
    return (npu->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      获取当前优先级最高的挂起 NPU 中断
 *
 *  读取 IIDX 寄存器，返回当前优先级最高的挂起中断索引。
 *  注意：读取该寄存器通常会清除对应的挂起标志。
 *
 *  @param[in]  npu            指向 NPU 寄存器覆盖结构的指针
 *
 *  @return     一个 @ref DL_NPU_IIDX 枚举值
 */
__STATIC_INLINE DL_NPU_IIDX DL_NPU_getPendingInterrupt(NPU_Regs *npu)
{
    return (DL_NPU_IIDX)(npu->CPU_INT.IIDX);
}

/**
 *  @brief      清除挂起的 NPU 中断标志
 *
 *  @param[in]  npu            指向 NPU 寄存器覆盖结构的指针
 *
 *  @param[in]  interruptMask  要清除的中断位掩码，可按位或组合
 *                             @ref DL_NPU_INTERRUPT.
 */
__STATIC_INLINE void DL_NPU_clearInterruptStatus(
    NPU_Regs *npu, uint32_t interruptMask)
{
    npu->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief      获取 NPU 指令存储区的物理起始地址
 *
 *  @param[in]  npu            指向 NPU 寄存器覆盖结构的指针
 *
 *  @return     指令存储区起始物理地址
 */
__STATIC_INLINE uint32_t DL_NPU_getInstrMemAddress(const NPU_Regs *npu)
{
    return ((uint32_t)(&npu->DREG20));
}

/**
 *  @brief      获取 NPU 参数存储区的物理起始地址
 *
 *  @param[in]  npu            指向 NPU 寄存器覆盖结构的指针
 *
 *  @return     参数存储区起始物理地址
 */
__STATIC_INLINE uint32_t DL_NPU_getParamMemAddress(const NPU_Regs *npu)
{
    return ((uint32_t)(&npu->DREG21));
}

/**
 *  @brief      获取 NPU 控制寄存器（CTL）的物理起始地址
 *
 *  @param[in]  npu            指向 NPU 寄存器覆盖结构的指针
 *
 *  @return     NPU CTL 寄存器物理地址
 */
__STATIC_INLINE uint32_t DL_NPU_getCtlRegAddress(const NPU_Regs *npu)
{
    return ((uint32_t)(&npu->DREG0));
}

/**
 *  @brief      获取 NPU 累加器寄存器（ACCVAL）的物理地址
 *
 *  @param[in]  npu            指向 NPU 寄存器覆盖结构的指针
 *
 *  @return     ACCVAL 寄存器物理地址
 */
__STATIC_INLINE uint32_t DL_NPU_getAccRegAddress(const NPU_Regs *npu)
{
    return ((uint32_t)(&npu->DREG19));
}

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_NPU__ */

#endif /* ti_dl_dl_npu__include */
/** @}*/
