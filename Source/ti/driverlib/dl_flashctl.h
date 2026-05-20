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
 *  @file       dl_flashctl.h
 *  @brief      Flash控制器驱动程序库
 *  @defgroup   FLASHCTL Flash控制器 (FLASHCTL)
 *
 *  @anchor ti_dl_dl_flashctl_Overview
 *  # 概述
 *
 *  Flash控制器驱动程序库允许完全配置MSPM0非易失性存储器系统。
 *  非易失性存储器(NVM)系统提供用于存储可执行代码和数据的非易失性闪存。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup FLASHCTL
 * @{
 */
#ifndef ti_dl_dl_flashctl__include
#define ti_dl_dl_flashctl__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/m0p/dl_factoryregion.h>
#include <ti/driverlib/m0p/dl_sysctl.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (FLASHCTL_SYS_DATAWIDTH == 128)
/*!
 * @brief 设备具有128位Flash字宽
 */
#define DEVICE_HAS_FLASH_128_BIT_WORD
#endif

#if (FLASHCTL_SYS_WEPROTAWIDTH == 0)
/*!
 * @brief 设备没有CMDWEPROTA寄存器
 */
#define DEVICE_HAS_NO_CMDWEPROTA
#endif

/* clang-format off */

/*!
 * @brief Bank 0的地址
 */
#define FLASHCTL_BANK0_ADDRESS                                      (0x00000000)

/*!
 * @brief MAIN内存区域的地址
 */
#define FLASHCTL_MAIN_ADDRESS                                       (0x00000000)

/*!
 * @brief NONMAIN内存区域的地址
 */
#define FLASHCTL_NONMAIN_ADDRESS                                    (0x41c00000)

/*!
 * @brief NONMAIN扇区数量
 */
#define NUMBER_OF_NONMAIN_SECTORS                                            (1)

/*!
 * @brief DATA内存区域的地址
 */
#define FLASHCTL_DATA_ADDRESS                                       (0x41d00000)

/*!
 * @brief 使能8位编程(无ECC)
 */
#define DL_FLASHCTL_PROGRAM_8_WITHOUT_ECC                           (0x00000001)

/*!
 * @brief 使能16位编程(无ECC)
 */
#define DL_FLASHCTL_PROGRAM_16_WITHOUT_ECC                          (0x00000003)

/*!
 * @brief 使能32位编程(无ECC)
 */
#define DL_FLASHCTL_PROGRAM_32_WITHOUT_ECC                          (0x0000000F)

/*!
 * @brief 使能64位编程(无ECC)
 */
#define DL_FLASHCTL_PROGRAM_64_WITHOUT_ECC                          (0x000000FF)

/*!
 * @brief 使能8位编程(带ECC)
 */
#define DL_FLASHCTL_PROGRAM_8_WITH_ECC                              (0x00000101)

/*!
 * @brief 使能16位编程(带ECC)
 */
#define DL_FLASHCTL_PROGRAM_16_WITH_ECC                             (0x00000103)

/*!
 * @brief 使能32位编程(带ECC)
 */
#define DL_FLASHCTL_PROGRAM_32_WITH_ECC                             (0x0000010F)

/*!
 * @brief 使能64位编程(带ECC)
 */
#define DL_FLASHCTL_PROGRAM_64_WITH_ECC                             (0x000001FF)

/*!
 * @brief 使能8位读取验证(无ECC)
 */
#define DL_FLASHCTL_READ_VERIFY_8_WITHOUT_ECC                       (0x00000001)

/*!
 * @brief 使能16位读取验证(无ECC)
 */
#define DL_FLASHCTL_READ_VERIFY_16_WITHOUT_ECC                      (0x00000003)

/*!
 * @brief 使能32位读取验证(无ECC)
 */
#define DL_FLASHCTL_READ_VERIFY_32_WITHOUT_ECC                      (0x0000000F)

/*!
 * @brief 使能64位读取验证(无ECC)
 */
#define DL_FLASHCTL_READ_VERIFY_64_WITHOUT_ECC                      (0x000000FF)

/*!
 * @brief 使能8位读取验证(带ECC)
 */
#define DL_FLASHCTL_READ_VERIFY_8_WITH_ECC                          (0x00000101)

/*!
 * @brief 使能16位读取验证(带ECC)
 */
#define DL_FLASHCTL_READ_VERIFY_16_WITH_ECC                         (0x00000103)

/*!
 * @brief 使能32位读取验证(带ECC)
 */
#define DL_FLASHCTL_READ_VERIFY_32_WITH_ECC                         (0x0000010F)

/*!
 * @brief 使能64位读取验证(带ECC)
 */
#define DL_FLASHCTL_READ_VERIFY_64_WITH_ECC                         (0x000001FF)

/** @addtogroup DL_FLASHCTL_INTERRUPT
 *  @{
 */
/*!
 * @brief Flash命令执行完成中断
 */
#define DL_FLASHCTL_INTERRUPT_DONE                       (FLASHCTL_MIS_DONE_SET)
/** @}*/

/** @addtogroup DL_FLASHCTL_COMMAND_TYPE
 *  @{
 */
/*!
 * @brief 无操作Flash命令
 */
#define DL_FLASHCTL_COMMAND_TYPE_NO_OPERATION    (FLASHCTL_CMDTYPE_COMMAND_NOOP)

/*!
 * @brief 编程命令类型
 */
#define DL_FLASHCTL_COMMAND_TYPE_PROGRAM      (FLASHCTL_CMDTYPE_COMMAND_PROGRAM)

/*!
 * @brief 擦除命令类型
 */
#define DL_FLASHCTL_COMMAND_TYPE_ERASE          (FLASHCTL_CMDTYPE_COMMAND_ERASE)

/*!
 * @brief 读取验证命令类型
 */
#define DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY                                   \
                                           (FLASHCTL_CMDTYPE_COMMAND_READVERIFY)

/*!
 * @brief 空白验证命令类型
 */
#define DL_FLASHCTL_COMMAND_TYPE_BLANK_VERIFY                                  \
                                          (FLASHCTL_CMDTYPE_COMMAND_BLANKVERIFY)

/*!
 * @brief 模式切换命令类型
 */
#define DL_FLASHCTL_COMMAND_TYPE_MODE_CHANGE                                   \
                                           (FLASHCTL_CMDTYPE_COMMAND_MODECHANGE)

/*!
 * @brief 清除状态命令类型
 */
#define DL_FLASHCTL_COMMAND_TYPE_CLEAR_STATUS                                  \
                                          (FLASHCTL_CMDTYPE_COMMAND_CLEARSTATUS)
/** @}*/

/*!
 * @brief 一个Flash扇区的大小(字节)
 */
#define DL_FLASHCTL_SECTOR_SIZE                                           (1024)

/* clang-format on */

/*! @enum DL_FLASHCTL_COMMAND_SIZE */
typedef enum {
    /*! Flash 命令大小为一个字 */
    DL_FLASHCTL_COMMAND_SIZE_ONE_WORD = FLASHCTL_CMDTYPE_SIZE_ONEWORD,
    /*! Flash 命令大小为两个字 */
    DL_FLASHCTL_COMMAND_SIZE_TWO_WORDS = FLASHCTL_CMDTYPE_SIZE_TWOWORD,
    /*! Flash 命令大小为四个字 */
    DL_FLASHCTL_COMMAND_SIZE_FOUR_WORDS = FLASHCTL_CMDTYPE_SIZE_FOURWORD,
    /*! Flash 命令大小为八个字 */
    DL_FLASHCTL_COMMAND_SIZE_EIGHT_WORDS = FLASHCTL_CMDTYPE_SIZE_EIGHTWORD,
    /*! Flash 命令大小为一个扇区 */
    DL_FLASHCTL_COMMAND_SIZE_SECTOR = FLASHCTL_CMDTYPE_SIZE_SECTOR,
    /*! Flash 命令大小为一个存储库 */
    DL_FLASHCTL_COMMAND_SIZE_BANK = FLASHCTL_CMDTYPE_SIZE_BANK

} DL_FLASHCTL_COMMAND_SIZE;

/*! @enum DL_FLASHCTL_REGION_SELECT */
typedef enum {
    /*! Flash 区域选择主区域 */
    DL_FLASHCTL_REGION_SELECT_MAIN = FLASHCTL_CMDCTL_REGIONSEL_MAIN,
    /*! Flash 区域选择非主区域 */
    DL_FLASHCTL_REGION_SELECT_NONMAIN = FLASHCTL_CMDCTL_REGIONSEL_NONMAIN

} DL_FLASHCTL_REGION_SELECT;

/*! @enum DL_FLASHCTL_BANK_SELECT */
typedef enum {
    /*! 存储库 0 */
    DL_FLASHCTL_BANK_SELECT_0 = FLASHCTL_CMDCTL_BANKSEL_BANK0,
    /*! 存储库 1 */
    DL_FLASHCTL_BANK_SELECT_1 = FLASHCTL_CMDCTL_BANKSEL_BANK1,
    /*! 存储库 2 */
    DL_FLASHCTL_BANK_SELECT_2 = FLASHCTL_CMDCTL_BANKSEL_BANK2,
    /*! 存储库 3 */
    DL_FLASHCTL_BANK_SELECT_3 = FLASHCTL_CMDCTL_BANKSEL_BANK3,
    /*! 存储库 4 */
    DL_FLASHCTL_BANK_SELECT_4 = FLASHCTL_CMDCTL_BANKSEL_BANK4
} DL_FLASHCTL_BANK_SELECT;

/*! @enum DL_FLASHCTL_FAIL_TYPE */
typedef enum {
    /*! Flash 命令在执行期间未失败 */
    DL_FLASHCTL_FAIL_TYPE_NO_FAILURE = 0x0,
    /*! Flash 命令因写入/擦除保护扇区违规而失败 */
    DL_FLASHCTL_FAIL_TYPE_WRITE_ERASE_PROTECT =
        (FLASHCTL_STATCMD_FAILWEPROT_STATFAIL),
    /*! Flash 命令因验证错误而失败 */
    DL_FLASHCTL_FAIL_TYPE_VERIFY_ERROR = FLASHCTL_STATCMD_FAILVERIFY_STATFAIL,
    /*! Flash 命令因使用非法地址而失败 */
    DL_FLASHCTL_FAIL_TYPE_ILLEGAL_ADDRESS =
        FLASHCTL_STATCMD_FAILILLADDR_STATFAIL,
    /*! Flash 命令因存储库被设置为非读取模式而失败 */
    DL_FLASHCTL_FAIL_TYPE_WRONG_BANK_MODE = FLASHCTL_STATCMD_FAILMODE_STATFAIL,
    /*! Flash 命令因未定义错误而失败 */
    DL_FLASHCTL_FAIL_TYPE_MISCELLANEOUS = FLASHCTL_STATCMD_FAILMISC_STATFAIL,
} DL_FLASHCTL_FAIL_TYPE;

/*! @enum DL_FLASHCTL_COMMAND_STATUS */
typedef enum {
    /*! Flash 命令通过 */
    DL_FLASHCTL_COMMAND_STATUS_PASSED = (FLASHCTL_STATCMD_CMDDONE_STATDONE |
                                         FLASHCTL_STATCMD_CMDPASS_STATPASS),
    /*! Flash 命令失败 */
    DL_FLASHCTL_COMMAND_STATUS_FAILED = (FLASHCTL_STATCMD_CMDDONE_STATDONE |
                                         FLASHCTL_STATCMD_CMDPASS_STATFAIL),
    /*! Flash 命令仍在进行中 */
    DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS =
        (FLASHCTL_STATCMD_CMDINPROGRESS_STATINPROGRESS)

} DL_FLASHCTL_COMMAND_STATUS;

/*! @enum DL_FLASHCTL_IIDX */
typedef enum {
    /*! 完成中断挂起 */
    DL_FLASHCTL_IIDX_DONE = FLASHCTL_IIDX_STAT_DONE
} DL_FLASHCTL_IIDX;

/**
 * @brief 使能Flash中断
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
__STATIC_INLINE void DL_FlashCTL_enableInterrupt(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.IMASK = FLASHCTL_IMASK_DONE_ENABLED;
}

/**
 * @brief 禁用Flash中断
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
__STATIC_INLINE void DL_FlashCTL_disableInterrupt(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.IMASK = FLASHCTL_IMASK_DONE_DISABLED;
}

/**
 * @brief 检查Flash中断是否已使能
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return Flash中断是否已使能
 *
 * @retval false 中断已禁用
 * @retval true  中断已使能
 */
__STATIC_INLINE bool DL_FlashCTL_isInterruptEnabled(
    const FLASHCTL_Regs *flashctl)
{
    return (flashctl->GEN.IMASK == FLASHCTL_IMASK_DONE_ENABLED);
}

/**
 * @brief 检查已使能Flash中断的标志
 *
 * 检查之前使能的Flash中断是否待处理。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return Flash中断是否待处理
 *
 * @retval 0  无中断使能
 * @retval @ref DL_FLASHCTL_INTERRUPT 的值
 */
__STATIC_INLINE uint32_t DL_FlashCTL_getEnabledInterruptStatus(
    const FLASHCTL_Regs *flashctl)
{
    return (flashctl->GEN.MIS);
}

/**
 * @brief 检查Flash中断标志
 *
 * 检查Flash中断是否待处理。不必先前使能该中断。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return Flash中断是否待处理
 *
 * @retval 0  无中断待处理
 * @retval @ref DL_FLASHCTL_INTERRUPT 的值
 */
__STATIC_INLINE uint32_t DL_FlashCTL_getRawInterruptStatus(
    const FLASHCTL_Regs *flashctl)
{
    return (flashctl->GEN.RIS);
}

/**
 * @brief 获取最高优先级待处理Flash中断
 *
 *  Checks if the flash interrupt is pending. Interrupts do not have to be
 *  previously enabled.
 *
 *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
 *
 *  @return     The highest priority pending FLASH interrupt
 *
 */
__STATIC_INLINE uint32_t DL_FlashCTL_getPendingInterrupt(
    const FLASHCTL_Regs *flashctl)
{
    return (flashctl->GEN.IIDX);
}

/**
 * @brief 清除待处理的Flash中断
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
__STATIC_INLINE void DL_FlashCTL_clearInterruptStatus(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.ICLR = FLASHCTL_ICLR_DONE_CLR;
}

/**
 * @brief 检查命令是否已开始执行
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return 命令是否已设置为执行
 */
__STATIC_INLINE bool DL_FlashCTL_isCommandExecuted(
    const FLASHCTL_Regs *flashctl)
{
    return (flashctl->GEN.CMDEXEC == FLASHCTL_CMDEXEC_VAL_EXECUTE);
}

/**
 * @brief 启动命令执行
 *
 * 执行先前配置的Flash命令(擦除、编程等)
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
__STATIC_INLINE void DL_FlashCTL_setCommandExecute(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 禁用阶梯式擦除
 *
 * 所有擦除脉冲将使用默认VHV校准电压设置。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
__STATIC_INLINE void DL_FlashCTL_disbleStairStepErase(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDCTL |= FLASHCTL_CMDCTL_SSERASEDIS_DISABLE;
}

/**
 * @brief 检查阶梯式擦除是否已禁用
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return 阶梯式擦除是否已禁用
 */
__STATIC_INLINE bool DL_FlashCTL_isStairStepEraseDisabled(
    const FLASHCTL_Regs *flashctl)
{
    return ((flashctl->GEN.CMDCTL & FLASHCTL_CMDCTL_SSERASEDIS_MASK) ==
            FLASHCTL_CMDCTL_SSERASEDIS_DISABLE);
}

/**
 * @brief 使能阶梯式擦除
 *
 * 在连续的擦除脉冲期间,VHV电压将逐步变化。
 * 步进计数、步进电压、起始和结束电压都是硬连接的。此功能默认使能。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
__STATIC_INLINE void DL_FlashCTL_enableStairStepErase(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDCTL &= ~(FLASHCTL_CMDCTL_SSERASEDIS_MASK);
}

/**
 * @brief 使能地址覆盖模式
 *
 * 此函数覆盖CMDADDR中从系统地址到银行地址和银行ID的硬件地址转换。
 * 直接使用写入CMDADDR的数据作为银行地址。
 * 直接使用写入CMDCTL.BANKSEL的值作为银行ID。
 * 直接使用写入CMDCTL.REGIONSEL的值作为区域ID。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @sa DL_FlashCTL_setCommandAddress
 * @sa DL_FlashCTL_setBankSelect
 * @sa DL_FlashCTL_setRegionSelect
 */
__STATIC_INLINE void DL_FlashCTL_enableAddressOverrideMode(
    FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDCTL |= FLASHCTL_CMDCTL_ADDRXLATEOVR_OVERRIDE;
}

/**
 * @brief 检查地址覆盖模式是否已使能
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return 地址覆盖模式是否已使能
 */
__STATIC_INLINE bool DL_FlashCTL_isAddressOverrideModeEnabled(
    const FLASHCTL_Regs *flashctl)
{
    return ((flashctl->GEN.CMDCTL & FLASHCTL_CMDCTL_ADDRXLATEOVR_MASK) ==
            FLASHCTL_CMDCTL_ADDRXLATEOVR_OVERRIDE);
}

/**
 * @brief 禁用地址覆盖模式
 *
 * 禁用地址覆盖模式并返回系统寻址模式。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
__STATIC_INLINE void DL_FlashCTL_disableAddressOverrideMode(
    FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDCTL &= ~(FLASHCTL_CMDCTL_ADDRXLATEOVR_MASK);
}

/**
 * @brief 使能覆盖硬件生成的ECC码
 *
 * 覆盖Flash控制器的硬件ECC码生成功能。启用后,
 * 用户可以通过调用@ref DL_FlashCTL_setCommandDataECC手动设置要编程的ECC码。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @sa DL_FlashCTL_setCommandDataECC
 */
__STATIC_INLINE void DL_FlashCTL_enableOverrideHardwareGeneratedECC(
    FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDCTL |= FLASHCTL_CMDCTL_ECCGENOVR_OVERRIDE;
}

/**
 * @brief 检查是否已使能覆盖硬件生成的ECC码
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return 是否已使能覆盖硬件生成的ECC码
 */
__STATIC_INLINE bool DL_FlashCTL_isOverrideHardwareGeneratedECCEnabled(
    const FLASHCTL_Regs *flashctl)
{
    return ((flashctl->GEN.CMDCTL & FLASHCTL_CMDCTL_ECCGENOVR_MASK) ==
            FLASHCTL_CMDCTL_ECCGENOVR_OVERRIDE);
}

/**
 * @brief 禁用覆盖硬件生成的ECC码
 *
 * 禁用覆盖硬件生成的ECC码,这样Flash控制器将负责从数据生成所需的ECC位。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
__STATIC_INLINE void DL_FlashCTL_disableOverrideHardwareGeneratedECC(
    FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDCTL &= ~(FLASHCTL_CMDCTL_ECCGENOVR_MASK);
}

/**
 * @brief 设置区域选择
 *
 * 可以向此字段写入特定的区域ID,以指示如果设置了CMDCTL.ADDRXLATEOVR,
 * 操作应应用于哪个区域。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param region      要选择的区域。@ref DL_FLASHCTL_REGION_SELECT之一
 *
 * @sa DL_FlashCTL_enableAddressOverrideMode
 */
__STATIC_INLINE void DL_FlashCTL_setRegionSelect(
    FLASHCTL_Regs *flashctl, DL_FLASHCTL_REGION_SELECT region)
{
    DL_Common_updateReg(&flashctl->GEN.CMDCTL, (uint32_t) region,
        (uint32_t) FLASHCTL_CMDCTL_REGIONSEL_MASK);
}

/**
 * @brief 获取区域选择
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 *
 * @return     所选区域
 *
 * @retval     @ref DL_FLASHCTL_REGION_SELECT 之一
 */
__STATIC_INLINE DL_FLASHCTL_REGION_SELECT DL_FlashCTL_getRegionSelect(
    const FLASHCTL_Regs *flashctl)
{
    uint32_t region = flashctl->GEN.CMDCTL & FLASHCTL_CMDCTL_REGIONSEL_MASK;

    return (DL_FLASHCTL_REGION_SELECT)(region);
}

/**
 * @brief 设置bank选择
 *
 * 如果CMDCTL.ADDRXLATEOVR被设置，可以向此字段写入特定的bank ID，以指示操作应应用于哪个bank。
 *
 * @note 请参考设备数据手册了解设备上可用的bank数量。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 * @param bank      要选择的bank。@ref DL_FLASHCTL_BANK_SELECT 之一
 *
 * @sa DL_FlashCTL_enableAddressOverrideMode
 */
__STATIC_INLINE void DL_FlashCTL_setBankSelect(
    FLASHCTL_Regs *flashctl, DL_FLASHCTL_BANK_SELECT bank)
{
    DL_Common_updateReg(&flashctl->GEN.CMDCTL, (uint32_t) bank,
        (uint32_t) FLASHCTL_CMDCTL_BANKSEL_MASK);
}

/**
 * @brief 获取bank选择
 *
 * @note 请参考设备数据手册了解设备上可用的bank数量。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 *
 * @return     所选bank
 *
 * @retval     @ref DL_FLASHCTL_BANK_SELECT 之一
 */
__STATIC_INLINE DL_FLASHCTL_BANK_SELECT DL_FlashCTL_getBankSelect(
    const FLASHCTL_Regs *flashctl)
{
    uint32_t bank = flashctl->GEN.CMDCTL & FLASHCTL_CMDCTL_BANKSEL_MASK;

    return (DL_FLASHCTL_BANK_SELECT)(bank);
}

/**
 * @brief 设置用于编程数据的字节使能
 *
 * 此寄存器形成编程数据的每字节使能。CMDBYTEN中的每一位对应于要编程的寻址Flash字中的一个字节，包括ECC字节。
 * 如果需要，这允许子字编程（编程少于完整的64或72位Flash字）。
 *
 * ECC数据字节由该寄存器中的MSB位保护，具体取决于ECC的存在和Flash字数据宽度。
 * 注意：请参考TRM了解如何正确处理子字编程的ECC。如果数据字的全部64位未被编程，
 * 则无法正确编程ECC位，因此在Flash字的所有64位都被编程之前，不应启用ECC字节。
 *
 * 所有FLASHCTL命令完成后，此寄存器将被写入全0。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param byteMask   要启用的字节掩码。范围[0,0x1FF]。
 *                  例如：
 *                  - byteMask = 0x0，禁用所有字节编程
 *                  - byteMask = 0x1，启用8位编程
 *                  - byteMask = 0x3，启用16位编程
 *                  - byteMask = 0xF，启用32位编程
 *                  - byteMask = 0xFF，启用64位编程
 *                  - byteMask = 0x1FF，启用带ECC的64位编程
 */
__STATIC_INLINE void DL_FlashCTL_setCommandByteEnable(
    FLASHCTL_Regs *flashctl, uint32_t byteMask)
{
    flashctl->GEN.CMDBYTEN = byteMask;
}

/**
 * @brief 获取已启用编程数据的字节
 *
 * 此寄存器形成编程数据的每字节使能。CMDBYTEN中的每一位对应于要编程的寻址Flash字中的一个字节，包括ECC字节。
 * 如果需要，这允许子字编程（编程少于完整的64或72位Flash字）。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 *
 * @return     已启用的字节
 *
 * @retval     范围[0,0x1FF]的值。
 */
__STATIC_INLINE uint32_t DL_FlashCTL_getCommandByteEnable(
    const FLASHCTL_Regs *flashctl)
{
    return ((uint32_t) flashctl->GEN.CMDCTL &
            (uint32_t)(FLASHCTL_CMDBYTEN_VAL_MASK));
}

/**
 * @brief 设置命令数据寄存器的数据
 *
 * 命令数据寄存器CMDDATAx用于形成命令的数据。对于仅支持单字编程的设备，
 * 仅使用CMDDATA0和CMDDATA1寄存器加载要编程到Flash内存的数据。
 * CMDDATA0始终加载目标数据的BIT31-BIT0，CMDDATA1始终加载目标数据的BIT63-BIT32。
 * 如果编程少于64位数据，请参阅设备TRM中关于编程少于一个Flash字的特殊处理要求部分。
 *
 * @note 确保使用@ref DL_FlashCTL_setCommandByteEnable启用了正确的编程字节。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param index      选择要写入的命令数据寄存器。
 *                   请参阅设备TRM了解CMDDATA寄存器的数量。例如：
 *                   - index = 0，选择CMDDATA0
 *                   - index = 1，选择CMDDATA1
 * @param data       要写入命令数据寄存器的数据。
 *                   范围[0,0xFFFFFFFF]的值
 */
__STATIC_INLINE void DL_FlashCTL_setCommandData(
    FLASHCTL_Regs *flashctl, uint8_t index, const uint32_t *data)
{
    volatile uint32_t *pReg = &(flashctl->GEN.CMDDATA0) + index;

    *pReg = *data;
}

/**
 * @brief 从命令数据寄存器获取数据
 *
 * 对于仅支持单字编程的设备，仅使用CMDDATA0和CMDDATA1寄存器加载要编程到Flash内存的数据。
 * CMDDATA0始终加载目标数据的BIT31-BIT0，CMDDATA1始终加载目标数据的BIT63-BIT32。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param index      选择要读取的命令数据寄存器。
 *                   请参阅设备TRM了解CMDDATA寄存器的数量。例如：
 *                   - index = 0，选择CMDDATA0
 *                   - index = 1，选择CMDDATA1
 *
 * @return     命令数据寄存器中的数据
 *
 * @retval     范围[0,0xFFFFFFFF]的值。
 */
__STATIC_INLINE uint32_t DL_FlashCTL_getCommandData(
    FLASHCTL_Regs *flashctl, uint8_t index)
{
    volatile uint32_t *pReg = &(flashctl->GEN.CMDDATA0);

    return (
        (uint32_t)(*(pReg + index) & (uint32_t) FLASHCTL_CMDDATA0_VAL_MASK));
}

/**
 * @brief 在命令数据ECC寄存器中设置ECC码
 *
 * 此寄存器形成命令数据的ECC部分。
 *
 * @note 只有当使用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC禁用硬件ECC码生成时，
 * 才能手动设置ECC数据
 *
 * @note 确保使用@ref DL_FlashCTL_setCommandByteEnable启用了正确的ECC编程字节
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param index      选择要写入的命令数据ECC寄存器。
 *                   请参阅设备TRM了解CMDDATAECC寄存器的数量。例如：
 *                   - index = 0，选择CMDDATAECC0
 *                   - index = 1，选择CMDDATAECC1
 * @param data       要写入命令数据ECC寄存器的数据。
 *                   范围[0, 0xFF]的值
 */
__STATIC_INLINE void DL_FlashCTL_setCommandDataECC(
    FLASHCTL_Regs *flashctl, uint8_t index, const uint8_t *data)
{
    volatile uint32_t *pReg = &(flashctl->GEN.CMDDATAECC0) + index;

    *pReg = *data;
}

/**
 * @brief 从命令数据ECC寄存器获取ECC码
 *
 * 此寄存器形成命令数据的ECC部分。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param index      选择要读取的命令数据ECC寄存器。
 *                   请参阅设备TRM了解CMDDATAECC寄存器的数量。例如：
 *                   - index = 0，选择CMDDATAECC0
 *                   - index = 1，选择CMDDATAECC1
 *
 * @return     命令数据ECC寄存器中的数据
 *
 * @retval     范围[0,0xFF]的值。
 */
__STATIC_INLINE uint8_t DL_FlashCTL_getCommandDataECC(
    FLASHCTL_Regs *flashctl, uint8_t index)
{
    volatile uint32_t *pReg = &(flashctl->GEN.CMDDATAECC0);

    return ((uint8_t)(
        *(pReg + index) & (uint32_t) FLASHCTL_CMDDATAECC0_VAL0_MASK));
}

/**
 * @brief 设置命令的目标地址
 *
 * 命令的目标地址必须是Flash字(64位)对齐的。这意味着目标地址必须对齐到0b000边界
 * (例如,地址的低3位必须为零)。
 *
 * 1) 对于单字编程,此地址表示要编程的Flash银行字。
 * 2) 对于多字编程,此地址表示编程的第一个Flash银行地址。地址将自动递增。
 * 3) 对于扇区和银行擦除,此地址表示要擦除的扇区或银行。
 * 4) 对于读取验证,此地址形成验证操作的起始地址。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 * @param address   编程/擦除/读取的目标地址
 *
 */
__STATIC_INLINE void DL_FlashCTL_setCommandAddress(
    FLASHCTL_Regs *flashctl, uint32_t address)
{
    flashctl->GEN.CMDADDR = address;
}

/**
 * @brief 返回当前命令的状态
 *
 * 检查命令是否仍在进行中或已完成,以及是通过还是失败
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return     当前命令的状态
 *
 * @retval     @ref DL_FLASHCTL_COMMAND_STATUS 值之一
 */
__STATIC_INLINE DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_getCommandStatus(
    const FLASHCTL_Regs *flashctl)
{
    uint32_t commandStatus =
        flashctl->GEN.STATCMD &
        (FLASHCTL_STATCMD_CMDDONE_MASK | FLASHCTL_STATCMD_CMDPASS_MASK |
            FLASHCTL_STATCMD_CMDINPROGRESS_MASK |
            FLASHCTL_STATCMD_CMDPASS_STATFAIL);

    return (DL_FLASHCTL_COMMAND_STATUS)(commandStatus);
}

/**
 * @brief 返回命令失败的原因
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return     命令失败的类型
 *
 * @retval     @ref DL_FLASHCTL_FAIL_TYPE 值之一
 */
__STATIC_INLINE DL_FLASHCTL_FAIL_TYPE DL_FlashCTL_getFailureStatus(
    const FLASHCTL_Regs *flashctl)
{
    uint32_t commandFailureType =
        flashctl->GEN.STATCMD &
        (FLASHCTL_STATCMD_FAILWEPROT_MASK | FLASHCTL_STATCMD_FAILVERIFY_MASK |
            FLASHCTL_STATCMD_FAILILLADDR_MASK |
            FLASHCTL_STATCMD_FAILMODE_MASK | FLASHCTL_STATCMD_FAILMISC_MASK);

    return (DL_FLASHCTL_FAIL_TYPE)(commandFailureType);
}

/**
 * @brief 阻塞函数,等待命令执行完成
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return     命令是否成功
 *
 * @retval     false 命令未成功
 * @retval     true  命令成功
 */
__STATIC_INLINE bool DL_FlashCTL_waitForCmdDone(const FLASHCTL_Regs *flashctl)
{
    /* 等待命令完成 */
    while ((flashctl->GEN.STATCMD & FLASHCTL_STATCMD_CMDDONE_MASK) !=
           FLASHCTL_STATCMD_CMDDONE_STATDONE) {
        ;
    }

    return ((flashctl->GEN.STATCMD & FLASHCTL_STATCMD_CMDPASS_MASK) ==
            FLASHCTL_STATCMD_CMDPASS_STATPASS);
}

/**
 * @brief 设置清除状态位并执行命令
 *
 * 这将清除STATCMD寄存器并重新对CMDWEPROTx寄存器应用最大保护
 *
 * @pre        此命令应在@ref DL_FlashCTL_unprotectSector之前调用,
 *             以确保解除保护后内存不会被重新保护。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
__STATIC_INLINE void DL_FlashCTL_executeClearStatus(FLASHCTL_Regs *flashctl)
{
    /* 在CMDTYPE寄存器中设置命令位以清除状态 */
    flashctl->GEN.CMDTYPE = DL_FLASHCTL_COMMAND_TYPE_CLEAR_STATUS;
    /* 设置执行位 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;

    /* 等待清除状态命令完成 */
    while ((FLASHCTL->GEN.STATCMD & FLASHCTL_STATCMD_CMDINPROGRESS_MASK) ==
           FLASHCTL_STATCMD_CMDINPROGRESS_STATINPROGRESS) {
        ;
    }
}

/**
 * @brief 获取输入地址在整个内存映射中的扇区编号
 *
 * 如果设备有两个128KB的bank，输入地址为0x20000，那么此API将返回扇区编号为128。
 * 要获取设备整个内存映射的扇区编号，请参考@ref DL_FlashCTL_getFlashSectorNumberInBank。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 * @param addr      所需扇区内的内存地址
 *
 * @return     包含输入地址的扇区编号
 */
__STATIC_INLINE uint32_t DL_FlashCTL_getFlashSectorNumber(
    FLASHCTL_Regs *flashctl, uint32_t addr)
{
    return (addr >> (uint32_t) 10);
}

/**
 * @brief 获取输入地址相对于其所在bank的扇区编号
 *
 * 如果设备有两个128KB的bank，输入地址为0x20000，那么此API将返回扇区编号为0。这是因为0x20000位于Bank 1的Sector 0中。
 * 要获取设备整个内存映射的扇区编号，请参考@ref DL_FlashCTL_getFlashSectorNumber。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 * @param addr      所需扇区内的内存地址
 *
 * @return     包含该地址的当前bank中的扇区编号
 */
__STATIC_INLINE uint32_t DL_FlashCTL_getFlashSectorNumberInBank(
    FLASHCTL_Regs *flashctl, uint32_t addr)
{
    uint8_t numBanks       = DL_FactoryRegion_getNumBanks();
    uint32_t mainFlashSize = DL_FactoryRegion_getMAINFlashSize();
    uint32_t sector, bankSectors, sectorInBank;

    /* Current sector over the whole memory map */
    sector = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
    if (numBanks > 1) {
        /* Number of sectors per bank, considered max sector
         * count for a bank-adjusted sector. Assume banks are
         * evenly distributed */
        bankSectors = (mainFlashSize / numBanks);
        /* We will not assume that the maximum number of bank sectors is a
         * multiple of 2. Which does mean incurring a modulo operation. */
        sectorInBank = sector % bankSectors;
    } else {
        sectorInBank = sector;
    }

    return sectorInBank;
}

/**
 * @brief 设置Flash使用的等待状态数
 *
 * 更改Flash控制器使用的等待状态数。当更改时钟频率时，可能需要更新等待状态数。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param waitState  要设置的等待状态数。推荐值基于系统时钟速度：
 *                   - 0MHz-24MHz: 0个等待状态
 *                   - 24MHz-48MHz: 1个等待状态
 *                   - 48MHz-72MHz: 2个等待状态
 *                   - 72MHz-80MHz: 3个等待状态
 */
__STATIC_INLINE void DL_FlashCTL_setWaitState(
    FLASHCTL_Regs *flashctl, uint32_t waitState)
{
    flashctl->GEN.CFGCMD = waitState;
}

/**
 *  @brief      获取Flash使用的等待状态数
 *
 *  @param[in]  flashctl   指向Flash寄存器结构体的指针
 *
 *  @return     Flash使用的等待状态数
 */
__STATIC_INLINE uint32_t DL_FlashCTL_getWaitState(
    const FLASHCTL_Regs *flashctl)
{
    return (flashctl->GEN.CFGCMD & FLASHCTL_CFGCMD_WAITSTATE_MASK);
}

/**
 * @brief 在未保护内存上执行擦除
 *
 * 在包含输入内存地址的扇区或银行内的未保护内存上执行擦除。
 *
 * 注意:调用此API之前,用户负责解除内存区域的保护。
 *      命令执行后,内存将自动被保护。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address       要擦除的扇区或银行内的内存地址
 * @param memorySize    要擦除的内存块大小:
 *                      @arg DL_FLASHCTL_COMMAND_SIZE_BANK
 *                      @arg DL_FLASHCTL_COMMAND_SIZE_SECTOR
 *
 * @post        此API仅启动擦除过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:擦除操作完成后,Flash控制器会将所有内存配置为保护状态。
 */
void DL_FlashCTL_eraseMemory(FLASHCTL_Regs *flashctl, uint32_t address,
    DL_FLASHCTL_COMMAND_SIZE memorySize);

/**
 * @brief 在未保护内存上执行擦除,并从RAM执行命令
 *
 * 在包含输入内存地址的扇区或银行内的未保护内存上执行擦除。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * 注意:调用此API之前,用户负责解除内存区域的保护。
 *      命令执行后,内存将自动被保护。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address       要擦除的扇区或银行内的内存地址
 * @param memorySize    要擦除的内存块大小:
 *                      @arg DL_FLASHCTL_COMMAND_SIZE_BANK
 *                      @arg DL_FLASHCTL_COMMAND_SIZE_SECTOR
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @post        此API仅启动擦除过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:擦除操作完成后,Flash控制器会将所有内存配置为保护状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_eraseMemoryFromRAM(
    FLASHCTL_Regs *flashctl, uint32_t address,
    DL_FLASHCTL_COMMAND_SIZE memorySize);

/**
 * @brief 在一个银行上执行主内存整片擦除
 *
 * 仅在Bank 0上执行主内存整片擦除。此API应用于单银行设备。
 *
 * 注意:此API会将所有主内存设置为解除擦除/编程保护
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return     擦除是否成功
 *
 * @retval     false 擦除未成功
 * @retval     true  擦除成功
 *
 * 注意:擦除操作完成后,Flash控制器会将所有内存配置为保护状态。
 */
bool DL_FlashCTL_massErase(FLASHCTL_Regs *flashctl);

/**
 * @brief 执行主内存整片擦除,并从RAM执行命令
 *
 * 仅在Bank 0上执行主内存整片擦除。此API应用于单银行设备。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * 注意:此API会将所有主内存设置为解除擦除/编程保护
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * 注意:擦除操作完成后,Flash控制器会将所有内存配置为保护状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_massEraseFromRAM(
    FLASHCTL_Regs *flashctl);

/**
 * @brief 执行主内存整片擦除,并擦除所有Flash银行
 *
 * 在每个Flash银行内执行主内存整片擦除。
 *
 * 注意:此API会将所有主内存设置为解除擦除/编程保护
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return     擦除是否成功
 *
 * @retval     false 擦除未成功
 * @retval     true  擦除成功
 *
 * 注意:擦除操作完成后,Flash控制器会将所有内存配置为保护状态。
 */
bool DL_FlashCTL_massEraseMultiBank(FLASHCTL_Regs *flashctl);

/**
 * @brief 在主内存和非主内存上执行工厂复位擦除
 *
 * 在设备的Bank 0主内存和非主内存上执行擦除。此API应用于单银行设备。
 *
 * 注意:此API会在成功后将所有主内存设置为擦除/编程保护。
 *      如果不成功,所有内存将被解除保护。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return     擦除是否成功
 *
 * @retval     false 擦除未成功
 * @retval     true  擦除成功
 *
 * 注意:擦除操作完成后,Flash控制器会将所有内存配置为保护状态。
 */
bool DL_FlashCTL_factoryReset(FLASHCTL_Regs *flashctl);

/**
 * @brief 执行工厂复位擦除主内存和非主内存,并从RAM执行命令
 *
 * 在设备的Bank 0主内存和非主内存上执行擦除。此API应用于单银行设备。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * 注意:此API会在成功后将所有主内存设置为擦除/编程保护。
 *      如果不成功,所有内存将被解除保护。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * 注意:擦除操作完成后,Flash控制器会将所有内存配置为保护状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_factoryResetFromRAM(
    FLASHCTL_Regs *flashctl);

/**
 * @brief 执行工厂复位擦除主内存和非主内存
 *
 * 在设备的所有Flash银行主内存和非主内存上执行擦除。
 *
 * 注意:此API会在成功后将所有主内存设置为擦除/编程保护。
 *      如果不成功,所有内存将被解除保护。
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return     擦除是否成功
 *
 * @retval     false 擦除未成功
 * @retval     true  擦除成功
 *
 * 注意:擦除操作完成后,Flash控制器会将所有内存配置为保护状态。
 */
bool DL_FlashCTL_factoryResetMultiBank(FLASHCTL_Regs *flashctl);

/**
 * @brief 编程8位数据到指定地址的未保护内存
 *
 * 设备数据手册规定了每个字线(word line)在需要擦除包含该字线的扇区之前的
 * 最大编程操作次数。超过此最大值可能导致字线内的数据损坏。
 * 如果执行8位(字节)编程操作,或对同一内存位置进行多次编程,
 * 则必须考虑并不得超过每个字线的最大编程限制。因此,最好缓冲数据并使用
 * @ref DL_FlashCTL_programMemory16、@ref DL_FashCTL_programMemory32或
 * @ref DL_FlashCTL_programMemory64 API来编程内存,以减少字线中的编程操作次数。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向8位源数据的指针
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:此API不使能ECC码编程。
 */
void DL_FlashCTL_programMemory8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

/**
 * @brief 编程8位数据到指定地址的未保护内存,并从RAM执行命令
 *
 * 设备数据手册规定了每个字线(word line)在需要擦除包含该字线的扇区之前的
 * 最大编程操作次数。超过此最大值可能导致字线内的数据损坏。
 * 如果执行8位(字节)编程操作,或对同一内存位置进行多次编程,
 * 则必须考虑并不得超过每个字线的最大编程限制。因此,最好缓冲数据并使用
 * @ref DL_FlashCTL_programMemory16、@ref DL_FlashCTL_programMemory32或
 * @ref DL_FlashCTL_programMemory64 API来编程内存,以减少字线中的编程操作次数。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,
 *                  即对齐到0b000边界。
 * @param data       指向8位源数据的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:此API不使能ECC码编程。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

/**
 * @brief 编程16位数据到指定地址的未保护内存
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向16位源数据的指针
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:此API不使能ECC码编程。
 */
void DL_FlashCTL_programMemory16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

/**
 * @brief 编程16位数据到指定地址的未保护内存,并从RAM执行命令
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向16位源数据的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 *  @pre         Before programming memory, the user must unprotect the region
 *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
 *
 *  NOTE: After completion of a program operation, the flash controller will
 *  configure all memory to a protected state.
 *  NOTE: This API does not enable programming the ECC code.
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

/**
 * @brief 编程32位数据到指定地址的未保护内存
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 *      此API不使能ECC码编程。
 */
void DL_FlashCTL_programMemory32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 编程32位数据到指定地址的未保护内存,并从RAM执行命令
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 *      此API不使能ECC码编程。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 编程64位数据到指定地址的未保护内存
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针(64位数据)
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *  @post        This API just starts the program process. Check the result of it
 *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
 *
 *  NOTE: After completion of a program operation, the flash controller will
 *  configure all memory to a protected state.
 *  NOTE: This API does not enable programming the ECC code.
 */
void DL_FlashCTL_programMemory64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 编程64位数据到指定地址的未保护内存,并从RAM执行命令
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针(64位数据)
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:此API不使能ECC码编程。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 编程8位数据并使用硬件生成的ECC码
 *
 * 编程8位数据,以及与8位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向8位源数据的指针
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_programMemory8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

/**
 * @brief 编程8位数据并使用硬件生成的ECC码,从RAM执行命令
 *
 * 编程8位数据,以及与8位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向8位源数据的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

/**
 * @brief 编程16位数据并使用硬件生成的ECC码
 *
 * 编程16位数据,以及与16位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向16位源数据的指针
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_programMemory16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

/**
 * @brief 编程16位数据并使用硬件生成的ECC码,从RAM执行命令
 *
 * 编程16位数据,以及与16位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向16位源数据的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

/**
 * @brief 编程32位数据并使用硬件生成的ECC码
 *
 * 编程32位数据,以及与32位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_programMemory32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 编程32位数据并使用硬件生成的ECC码,从RAM执行命令
 *
 * 编程32位数据,以及与32位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 编程64位数据并使用硬件生成的ECC码
 *
 * 编程64位数据,以及与64位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针(64位数据)
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_programMemory64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 编程64位数据并使用硬件生成的ECC码,从RAM执行命令
 *
 * 编程64位数据,以及与64位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针(64位数据)
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 编程8位数据并使用用户提供的ECC码
 *
 * 编程8位数据,以及与8位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向8位源数据的指针
 * @param eccCode    指向要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_programMemory8WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint8_t *data, const uint8_t *eccCode);

/**
 * @brief 编程8位数据并使用用户提供的ECC码,从RAM执行命令
 *
 * 编程8位数据,以及与8位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向8位源数据的指针
 * @param eccCode    指向要编程的ECC码的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data,
    const uint8_t *eccCode);

/**
 * @brief 编程16位数据并使用用户提供的ECC码
 *
 * 编程16位数据,以及与16位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向16位源数据的指针
 * @param eccCode    指向要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_programMemory16WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint16_t *data, const uint8_t *eccCode);

/**
 * @brief 编程16位数据并使用用户提供的ECC码,从RAM执行命令
 *
 * 编程16位数据,以及与16位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向16位源数据的指针
 * @param eccCode    指向要编程的ECC码的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data,
    const uint8_t *eccCode);

/**
 * @brief 编程32位数据并使用用户提供的ECC码
 *
 * 编程32位数据,以及与32位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针
 * @param eccCode    指向要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_programMemory32WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint32_t *data, const uint8_t *eccCode);

/**
 * @brief 编程32位数据并使用用户提供的ECC码,从RAM执行命令
 *
 * 编程32位数据,以及与32位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针
 * @param eccCode    指向要编程的ECC码的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode);

/**
 * @brief 编程64位数据并使用用户提供的ECC码
 *
 * 编程64位数据,以及与64位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针(64位数据)
 * @param eccCode    指向要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_programMemory64WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint32_t *data, const uint8_t *eccCode);

/**
 * @brief 编程64位数据并使用用户提供的ECC码,从RAM执行命令
 *
 * 编程64位数据,以及与64位数据字对应的8位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针(64位数据)
 * @param eccCode    指向要编程的ECC码的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode);

/**
 * @brief 编程提供的数据并使用硬件生成的ECC码
 *
 * 阻塞函数,编程一组数据,以及与指定地址数据对应的8位ECC码。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向要编程到Flash的数据源的指针
 * @param dataSize   要编程的32位字的数量。值应是偶数,以确保数据以64位为单位编程。
 * @param regionSelect  要擦除的内存区域。@ref DL_FLASHCTL_REGION_SELECT之一
 *
 * @return     编程是否成功
 *
 * @retval     false   编程未成功,或dataSize无效
 * @retval     true    编程成功
 *
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
bool DL_FlashCTL_programMemoryBlocking64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect);

/**
 * @brief 编程提供的数据并使用硬件生成的ECC码,从RAM执行命令
 *
 * 阻塞函数,编程一组数据,以及与指定地址数据对应的8位ECC码。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向要编程到Flash的数据源的指针
 * @param dataSize   要编程的32位字的数量。值应是偶数,以确保数据以64位为单位编程。
 * @param regionSelect  要擦除的内存区域。@ref DL_FLASHCTL_REGION_SELECT之一
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
DL_FLASHCTL_COMMAND_STATUS
DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect);

/**
 *  @brief      编程提供的数据并使用用户提供的ECC码
 *
 *  阻塞函数,编程一组数据,以及与指定地址数据对应的8位ECC码。
 *  此API假定硬件生成的ECC码已被禁用,因此用户
 *  必须提供要编程的ECC码。
 *
 *  @param[in]  flashctl   指向Flash寄存器结构体的指针
 *  @param[in]  address    要编程数据的目标内存地址。
 *                         地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 *  @param[in]  dataSize      要编程的32位字的数量。值应是偶数,以确保数据
 *                            以64位为单位编程。
 *  @param[in]  data          指向要编程到Flash的数据源的指针
 *  @param[in]  eccCode       指向与数据对应的要编程的ECC码的指针
 *  @param[in]  regionSelect  要擦除的内存区域。@ref DL_FLASHCTL_REGION_SELECT之一
 *
 *  @return     编程是否成功
 *
 *  @retval     false   编程未成功,或dataSize无效
 *  @retval     true    编程成功
 *
 *  @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *               来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *               此覆盖设置将在多个编程操作中保持有效,直到调用
 *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 *  @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 *  注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 *  注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
bool DL_FlashCTL_programMemoryBlocking64WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t *data, uint8_t *eccCode, uint32_t dataSize,
    DL_FLASHCTL_REGION_SELECT regionSelect);

/**
 * @brief 编程提供的数据并使用用户提供的ECC码,从RAM执行命令
 *
 * 阻塞函数,编程一组数据,以及与指定地址数据对应的8位ECC码。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向要编程到Flash的数据源的指针
 * @param eccCode    指向与数据对应的要编程的ECC码的指针
 * @param dataSize   要编程的32位字的数量。值应是偶数,以确保数据以64位为单位编程。
 * @param regionSelect  要擦除的内存区域。@ref DL_FLASHCTL_REGION_SELECT之一
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
DL_FLASHCTL_COMMAND_STATUS
DL_FlashCTL_programMemoryBlockingFromRAM64WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint8_t *eccCode, uint32_t dataSize,
    DL_FLASHCTL_REGION_SELECT regionSelect);

/**
 * @brief 编程提供的数据到指定地址的未保护内存
 *
 * 阻塞函数,编程一组数据。数据将被编程为Flash字(64位)。如果dataSize是奇数,则最后一个字将被编程为32位数据。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向要编程到Flash的数据源的指针
 * @param dataSize   要编程的32位字的数量
 * @param regionSelect  要擦除的内存区域。@ref DL_FLASHCTL_REGION_SELECT之一
 *
 * @return     编程是否成功
 *
 * @retval     false   编程未成功
 * @retval     true    编程成功
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:此API不使能ECC码编程。
 */
bool DL_FlashCTL_programMemoryBlocking(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t *data, uint32_t dataSize,
    DL_FLASHCTL_REGION_SELECT regionSelect);

/**
 * @brief 编程提供的数据到指定地址的未保护内存,并从RAM执行命令
 *
 * 阻塞函数,编程一组数据。数据将被编程为Flash字(64位)。如果dataSize是奇数,则最后一个字将被编程为32位数据。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向要编程到Flash的数据源的指针
 * @param dataSize   要编程的32位字的数量
 * @param regionSelect  要擦除的内存区域。@ref DL_FLASHCTL_REGION_SELECT之一
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:此API不使能ECC码编程。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect);

/**
 * @brief 解除所有主内存的擦除/编程保护
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
void DL_FlashCTL_unprotectMainMemory(FLASHCTL_Regs *flashctl);

/**
 * @brief 解除所有数据内存的擦除/编程保护
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
void DL_FlashCTL_unprotectDataMemory(FLASHCTL_Regs *flashctl);

/**
 * @brief 保护所有主内存免受擦除/编程
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
void DL_FlashCTL_protectMainMemory(FLASHCTL_Regs *flashctl);

/**
 * @brief 解除所有非主内存的擦除/编程保护
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
void DL_FlashCTL_unprotectNonMainMemory(FLASHCTL_Regs *flashctl);

/**
 * @brief 保护所有非主内存免受擦除/编程
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
void DL_FlashCTL_protectNonMainMemory(FLASHCTL_Regs *flashctl);

/**
 * @brief 解除所有用户内存的擦除/编程保护
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
void DL_FlashCTL_unprotectAllMemory(FLASHCTL_Regs *flashctl);

/**
 * @brief 保护所有用户内存免受擦除/编程
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 */
void DL_FlashCTL_protectAllMemory(FLASHCTL_Regs *flashctl);

/**
 * @brief 将指定扇区设置为可擦除/编程（解除保护）
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param addr       要解除保护的扇区地址
 * @param regionSelect  要解除保护的内存区域。@ref DL_FLASHCTL_REGION_SELECT之一
 */
void DL_FlashCTL_unprotectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
    DL_FLASHCTL_REGION_SELECT regionSelect);

/**
 * @brief 将指定扇区设置为不可擦除/编程（保护）
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param addr       要保护的扇区地址
 * @param regionSelect  要保护的内存区域。@ref DL_FLASHCTL_REGION_SELECT之一
 */
void DL_FlashCTL_protectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
    DL_FLASHCTL_REGION_SELECT regionSelect);

/**
 * @brief 验证指定地址的8位数据
 *
 * READVERIFY命令可用于读取Flash位置并与要验证的数据进行比较。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 */
void DL_FlashCTL_readVerify8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

/**
 * @brief 验证指定地址的8位数据,并从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并与要验证的数据进行比较。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

/**
 * @brief 验证指定地址的16位数据
 *
 * READVERIFY命令可用于读取Flash位置并与要验证的数据进行比较。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 */
void DL_FlashCTL_readVerify16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

/**
 * @brief 验证指定地址的16位数据,并从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并与要验证的数据进行比较。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

/**
 * @brief 验证指定地址的32位数据
 *
 * READVERIFY命令可用于读取Flash位置并与要验证的数据进行比较。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 */
void DL_FlashCTL_readVerify32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 验证指定地址的32位数据,并从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并与要验证的数据进行比较。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 验证指定地址的64位数据
 *
 * READVERIFY命令可用于读取Flash位置并与要验证的数据进行比较。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址。
 * @param data        指向要验证的数据源的指针
 */
void DL_FlashCTL_readVerify64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 验证指定地址的64位数据,并从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并与要验证的数据进行比较。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址。
 * @param data        指向要验证的数据源的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 验证指定地址的8位数据并使用硬件生成的ECC码
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_readVerify8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

/**
 * @brief 验证指定地址的8位数据并使用硬件生成的ECC码,从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

/**
 * @brief 验证指定地址的16位数据并使用硬件生成的ECC码
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_readVerify16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

/**
 * @brief 验证指定地址的16位数据并使用硬件生成的ECC码,从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

/**
 * @brief 验证指定地址的32位数据并使用硬件生成的ECC码
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_readVerify32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 验证指定地址的32位数据并使用硬件生成的ECC码,从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 验证指定地址的64位数据并使用硬件生成的ECC码
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_readVerify64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 验证指定地址的64位数据并使用硬件生成的ECC码,从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 验证指定地址的8位数据并使用用户提供的ECC码
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 * @param eccCode     指向与数据对应的要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_readVerify8WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint8_t *data, const uint8_t *eccCode);

/**
 * @brief 验证指定地址的8位数据并使用用户提供的ECC码,从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 * @param eccCode     指向与数据对应的要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data,
    const uint8_t *eccCode);

/**
 * @brief 验证指定地址的16位数据并使用用户提供的ECC码
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 * @param eccCode     指向与数据对应的要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_readVerify16WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint16_t *data, const uint8_t *eccCode);

/**
 * @brief 验证指定地址的16位数据并使用用户提供的ECC码,从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 * @param eccCode     指向与数据对应的要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data,
    const uint8_t *eccCode);

/**
 * @brief 验证指定地址的32位数据并使用用户提供的ECC码
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 * @param eccCode     指向与数据对应的要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_readVerify32WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint32_t *data, const uint8_t *eccCode);

/**
 * @brief 验证指定地址的32位数据并使用用户提供的ECC码,从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 * @param eccCode     指向与数据对应的要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode);

/**
 * @brief 验证指定地址的64位数据并使用用户提供的ECC码
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 * @param eccCode     指向与数据对应的要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_readVerify64WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint32_t *data, const uint8_t *eccCode);

/**
 * @brief 验证指定地址的64位数据并使用用户提供的ECC码,从RAM执行命令
 *
 * READVERIFY命令可用于读取Flash位置并将其与要验证的数据以及与数据对应的8位ECC码进行比较。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 * @param data        指向要验证的数据源的指针
 * @param eccCode     指向与数据对应的要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 *
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode);

/**
 * @brief 验证Flash字是否为空白
 *
 * 空白Flash字定义为已成功使用ERASE命令擦除且尚未使用PROGRAM命令从该非擦除状态编程的Flash字。
 * BLANKVERIFY命令可用于测试Flash字是否处于空白状态,表示它尚未从擦除状态编程。
 * BLANKVERIFY命令一次只能应用于单个Flash字。
 *
 * @post        此API仅启动命令过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查命令是否完成执行。
 * @post        应调用@ref DL_FlashCTL_getFailureStatus来确定BLANKVERIFY命令的结果。
 *              @ref DL_FlashCTL_getFailureStatus API将在Flash位置未被擦除时返回
 *              @ref DL_FLASHCTL_FAIL_TYPE_VERIFY_ERROR,在Flash位置为空白时返回
 *              @ref DL_FLASHCTL_FAIL_TYPE_NO_FAILURE。
 *
 * 注意:执行此命令后,Flash控制器将所有内存配置为保护状态。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址

 */
void DL_FlashCTL_blankVerify(FLASHCTL_Regs *flashctl, uint32_t address);

/**
 * @brief 验证Flash字是否为空白,并从RAM执行命令
 *
 * 空白Flash字定义为已成功使用ERASE命令擦除且尚未使用PROGRAM命令从该非擦除状态编程的Flash字。
 * BLANKVERIFY命令可用于测试Flash字是否处于空白状态,表示它尚未从擦除状态编程。
 * BLANKVERIFY命令一次只能应用于单个Flash字。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * @post        应调用@ref DL_FlashCTL_getFailureStatus来确定BLANKVERIFY命令的结果。
 *              @ref DL_FlashCTL_getFailureStatus API将在Flash位置未被擦除时返回
 *              @ref DL_FLASHCTL_FAIL_TYPE_VERIFY_ERROR,在Flash位置为空白时返回
 *              @ref DL_FLASHCTL_FAIL_TYPE_NO_FAILURE。
 *
 * 注意:执行此命令后,Flash控制器将所有内存配置为保护状态。
 *
 * @param flashctl    指向Flash寄存器结构体的指针
 * @param address     要验证的Flash内存地址
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_blankVerifyFromRAM(
    FLASHCTL_Regs *flashctl, uint32_t address);

/**
 * @brief 编程128位数据到指定地址的未保护内存
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:此API不使能ECC码编程。
 */
void DL_FlashCTL_programMemory128(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

/**
 * @brief 编程128位数据并使用硬件生成的ECC码
 *
 * 编程128位数据,以及与128位数据字对应的16位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码未被禁用,因此Flash控制器将生成ECC位。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针
 *
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_programMemory128WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

// /**
//  *  @brief      Programs 128 bit data with hardware generated ECC code
//  *
//  *  Programs 128 bit data, along with the 16 ECC bits which correspond to the
//  *  128-bit data word, to unprotected memory at the given address. This API
//  *  assumes that hardware generation of the ECC code has NOT been disabled,
//  *  and so the flash controller will generate the ECC bits.
//  *
//  *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
//  *  @param[in]  address    Destination memory address to program data. The
//  *                         address must be flash word (64-bit) aligned i.e.
//  *                         aligned to a 0b000 boundary.
//  *  @param[in]  data       Pointer to the 32-bit source data
//  *
//  *  @pre         Before programming memory, the user must unprotect the region
//  *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
//  *  @post        This API just starts the program process. Check the result of it
//  *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
//  *
//  *  NOTE: After completion of a program operation, the flash controller will
//  *  configure all memory to a protected state.
//  *  NOTE: After completion of a program operation, the flash controller will
//  *  disable programming of the ECC code (regardless of whether ECC code is
//  *  hardware generated or manually provided).
//  */
// void DL_FlashCTL_programMemory128WithECCGenerated(
//     FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data);

/**
 * @brief 编程128位数据并使用用户提供的ECC码
 *
 * 编程128位数据,以及与128位数据字对应的16位ECC码,到指定地址的未保护内存。
 * 此API假定硬件生成的ECC码已被禁用,因此用户必须提供要编程的ECC码。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向32位源数据的指针
 * @param eccCode    指向要编程的ECC码的指针
 *
 * @pre         用户必须调用@ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
 *              来禁用ECC码的硬件生成,以便可以手动提供ECC码进行编程。
 *              此覆盖设置将在多个编程操作中保持有效,直到调用
 *              @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC
 * @pre         编程内存之前,用户必须解除要编程的内存区域的保护。
 *              参见@ref DL_FlashCTL_unprotectSector
 * @post        此API仅启动编程过程。使用中断或@ref DL_FlashCTL_waitForCmdDone API检查结果
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:编程操作完成后,Flash控制器将禁用ECC码编程(无论ECC码是硬件生成还是手动提供)。
 */
void DL_FlashCTL_programMemory128WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint32_t *data, const uint8_t *eccCode);

/**
 * @brief 编程最多128位数据并生成ECC,同时在写入之间阻塞
 *
 * 阻塞函数,使用多字编程对最多2个Flash字进行编程。如果设备支持多字编程,请参考设备数据手册。
 * 尽可能将数据编程为64位数据或32位数据。
 *
 * @param flashctl   指向Flash寄存器结构体的指针
 * @param address    要编程数据的目标内存地址。地址必须是Flash字(64位)对齐,即对齐到0b000边界。
 * @param data       指向要编程到Flash的数据源的指针
 * @param dataSize   要编程的32位字的数量
 * @param regionSelect  要擦除的内存区域。@ref DL_FLASHCTL_REGION_SELECT之一
 *
 * @return     编程是否成功
 *
 * @retval     false   编程未成功
 * @retval     true    编程成功
 *
 * 注意:编程操作完成后,Flash控制器会将所有内存配置为保护状态。
 * 注意:此API不使能ECC码编程。
 */
bool DL_FlashCTL_programMemoryBlocking128WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect);

/**
 * @brief 对DATA bank执行擦除
 *
 * 仅对DATA bank执行擦除。此API应在具有DATA bank的设备上使用。
 * 要确定设备是否具有DATA bank,请使用@ref DL_FactoryRegion_getDATAFlashSize API。
 *
 * 注意:此API会擦除整个DATA bank
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return     擦除是否成功
 *
 * @retval     false 擦除未成功
 * @retval     true  擦除成功
 *
 */
bool DL_FlashCTL_eraseDataBank(FLASHCTL_Regs *flashctl);

/**
 * @brief 对DATA bank执行擦除,并从RAM执行命令
 *
 * 仅对DATA bank执行擦除。此API应在具有DATA bank的设备上使用。
 * 要确定设备是否具有DATA bank,请使用@ref DL_FactoryRegion_getDATAFlashSize API。
 *
 * 命令从RAM执行,阻塞直到命令完成。
 *
 * 注意:此API会擦除整个DATA bank
 *
 * @param flashctl  指向Flash寄存器结构体的指针
 *
 * @return     命令是否成功。@ref DL_FLASHCTL_COMMAND_STATUS之一
 *
 * @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   命令未成功
 * @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   命令成功
 *
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_eraseDataBankFromRAM(
    FLASHCTL_Regs *flashctl);

#ifdef __cplusplus
}
#endif

#endif /* ti_dl_dl_flashctl__include */
/** @}*/
