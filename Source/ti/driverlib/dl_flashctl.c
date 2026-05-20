/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 * 版权所有 (c) 2020，德州仪器公司 (Texas Instruments Incorporated)
 *
 * 允许在源代码和二进制形式下进行再分发和使用（无论是否经过修改），
 * 但必须满足以下条件：
 *
 * - 源代码的再分发必须保留上述版权声明、本条件列表以及以下免责声明。
 * - 二进制形式的再分发必须在提供的文档和/或其他材料中复制上述版权声明、
 *   本条件列表以及以下免责声明。
 * - 未经事先书面许可，不得使用 Texas Instruments Incorporated 或其
 *   贡献者的名称来支持或推广源自本软件的产品。
 *
 * 本软件由版权持有人和贡献者按“原样”提供，对任何明示或暗示的担保
 * （包括但不限于适销性和特定用途适用性的默示担保）不作任何保证。
 * 在任何情况下，版权持有人或贡献者均不对因使用本软件而导致的任何
 * 直接、间接、偶发、特殊、惩戒性或后果性损害（包括但不限于采购替代
 * 商品或服务；使用损失、数据丢失或利润损失；业务中断等）承担责任，
 * 无论其是基于合同、严格责任还是侵权行为（包括疏忽或其他原因），
 * 即使已被告知可能会发生此类损害。
 */

/**
 * @file  dl_flashctl.c
 * @brief Flash控制器驱动程序库实现
 *
 * 本文件实现了Flash控制器驱动程序库,用于操作MSPM0系列芯片的
 * 非易失性闪存存储器,包括擦除、编程、读取验证等功能。
 */

#include <stdbool.h>
#include <stdint.h>
#include <ti/devices/DeviceFamily.h>

#include <ti/driverlib/dl_flashctl.h>

static void DL_FlashCTL_programMemoryConfig(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t cmd);
static DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_executeCommandFromRAM(
    FLASHCTL_Regs *flashctl);
static void DL_FlashCTL_programMemory8Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint8_t *data);
static void DL_FlashCTL_programMemory16Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint16_t *data);
static void DL_FlashCTL_programMemory32Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint32_t *data);
static void DL_FlashCTL_programMemory64Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint32_t *data);

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
/**
 * @brief RAM函数属性定义 - TI编译器版本
 * 用于将函数放置在RAM中执行,避免Flash编程时可能发生的冲突
 */
#define RAMFUNC \
    __attribute__((section(".TI.ramfunc"))) __attribute__((noinline))
#elif defined(__GNUC__)
/**
 * @brief RAM函数属性定义 - GCC编译器版本
 */
#define RAMFUNC __attribute__((section(".ramfunc"))) __attribute__((noinline))
#elif defined(__IAR_SYSTEMS_ICC__)
/**
 * @brief RAM函数属性定义 - IAR编译器版本
 */
#define RAMFUNC __ramfunc __attribute__((noinline))
#else
#error "当前编译器不支持该函数"
#endif
/**
 * @brief 从RAM执行Flash命令并等待完成
 *
 * 此函数在RAM中执行以避免在Flash编程/擦除过程中从Flash读取指令的问题。
 * 函数会轮询STATCMD寄存器直到命令完成或失败。
 *
 * @param flashctl Flash控制器寄存器基址
 * @return 命令执行状态 (PASSED/FAILED)
 */
RAMFUNC static DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_executeCommandFromRAM(
    FLASHCTL_Regs *flashctl)
{
    volatile uint32_t status;
    /* 设置执行位以启动命令 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;

    /*
     * 执行Flash操作后,将进入do-while循环并使用status变量读取STATCMD寄存器。
     * 在循环中会轮询直到STATCMD寄存器返回DL_FLASHCTL_COMMAND_STATUS_PASSED
     * 或DL_FLASHCTL_COMMAND_STATUS_FAILED。这样可以确保在CPU以最高速度运行时
     * 也能正确轮询。
     */
    do {
        status =
            flashctl->GEN.STATCMD &
            (FLASHCTL_STATCMD_CMDDONE_MASK | FLASHCTL_STATCMD_CMDPASS_MASK |
                FLASHCTL_STATCMD_CMDINPROGRESS_MASK |
                FLASHCTL_STATCMD_CMDPASS_STATFAIL);
    } while ((DL_FLASHCTL_COMMAND_STATUS) status !=
                 (DL_FLASHCTL_COMMAND_STATUS_PASSED) &&
             (DL_FLASHCTL_COMMAND_STATUS) status !=
                 (DL_FLASHCTL_COMMAND_STATUS_FAILED));

    return ((DL_FLASHCTL_COMMAND_STATUS) status);
}

/**
 * @brief 擦除指定地址的Flash内存
 *
 * @param flashctl Flash控制器寄存器基址
 * @param address 要擦除的地址(必须在要擦除的扇区或银行内)
 * @param memorySize 擦除大小(扇区/银行)
 */
void DL_FlashCTL_eraseMemory(FLASHCTL_Regs *flashctl, uint32_t address,
    DL_FLASHCTL_COMMAND_SIZE memorySize)
{
    /* 设置命令类型和大小 */
    flashctl->GEN.CMDTYPE =
        (uint32_t) memorySize | DL_FLASHCTL_COMMAND_TYPE_ERASE;

    /* 设置地址,地址应在要擦除的所需扇区或银行内 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 设置执行位以启动命令 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 从RAM擦除指定地址的Flash内存
 *
 * @param flashctl Flash控制器寄存器基址
 * @param address 要擦除的地址
 * @param memorySize 擦除大小
 * @return 命令执行状态
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_eraseMemoryFromRAM(
    FLASHCTL_Regs *flashctl, uint32_t address,
    DL_FLASHCTL_COMMAND_SIZE memorySize)
{
    /* 设置命令类型和大小 */
    flashctl->GEN.CMDTYPE =
        (uint32_t) memorySize | DL_FLASHCTL_COMMAND_TYPE_ERASE;

    /* 设置地址,地址应在要擦除的所需扇区或银行内 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 跳转到RAM执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 整片擦除 (Mass Erase)
 *
 * 擦除主内存和数据Flash(如果存在)。此操作会解除主内存保护并保护非主内存。
 *
 * @param flashctl Flash控制器寄存器基址
 * @return 擦除是否成功
 */
bool DL_FlashCTL_massErase(FLASHCTL_Regs *flashctl)
{
    bool status;

    /* 解除主内存保护 */
    DL_FlashCTL_unprotectMainMemory(flashctl);
    /* 保护非主内存 */
    DL_FlashCTL_protectNonMainMemory(flashctl);

    /* 擦除Bank 0 */
    DL_FlashCTL_eraseMemory(
        flashctl, FLASHCTL_BANK0_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_BANK);
    status = DL_FlashCTL_waitForCmdDone(flashctl);

    /* 如果存在数据Flash,则擦除数据银行 */
    if (DL_FactoryRegion_getDATAFlashSize() && (status == true)) {
        status = DL_FlashCTL_eraseDataBank(flashctl);
    }
    return (status);
}

/**
 * @brief 从RAM执行整片擦除
 *
 * @param flashctl Flash控制器寄存器基址
 * @return 命令执行状态
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_massEraseFromRAM(
    FLASHCTL_Regs *flashctl)
{
    DL_FLASHCTL_COMMAND_STATUS status;

    /* 解除主内存保护 */
    DL_FlashCTL_unprotectMainMemory(flashctl);
    /* 保护非主内存 */
    DL_FlashCTL_protectNonMainMemory(flashctl);

    status = DL_FlashCTL_eraseMemoryFromRAM(
        flashctl, FLASHCTL_BANK0_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_BANK);

    /* 如果存在数据Flash且命令未失败,则擦除数据银行 */
    if (DL_FactoryRegion_getDATAFlashSize() &&
        (status != DL_FLASHCTL_COMMAND_STATUS_FAILED)) {
        status = DL_FlashCTL_eraseDataBankFromRAM(flashctl);
    }

    return (status);
}

bool DL_FlashCTL_massEraseMultiBank(FLASHCTL_Regs *flashctl)
{
    bool status            = true;
    uint32_t bankStartAddr = 0x0;
    uint8_t numBanks       = DL_FactoryRegion_getNumBanks();
    uint32_t flashSize     = DL_FactoryRegion_getMAINFlashSize();
    /* Assuming a sector size of 1KB */
    uint32_t bankSize = (((uint32_t) flashSize / (uint32_t) numBanks) * 1024U);

    DL_FLASHCTL_BANK_SELECT bankSelect = DL_FLASHCTL_BANK_SELECT_0;

    bool eraseFlag    = true;
    uint8_t bankIndex = 0;
    while (bankIndex < numBanks && status != false) {
        /* If flash bank swap policy is enabled, the primary bank will be write
         * and erase protected. Thus, we will not attempt an erase of the
         * primary bank.
         */
        if (DL_SYSCTL_isFlashBankSwapEnabled()) {
            eraseFlag = (bankIndex < (numBanks / (uint8_t) 2)) ? false : true;
        } else {
            eraseFlag = true;
        }
        switch (bankIndex) {
            case 0:
                bankSelect = DL_FLASHCTL_BANK_SELECT_0;
                break;
            case 1:
                bankSelect = DL_FLASHCTL_BANK_SELECT_1;
                break;
            case 2:
                bankSelect = DL_FLASHCTL_BANK_SELECT_2;
                break;
            case 3:
                bankSelect = DL_FLASHCTL_BANK_SELECT_3;
                break;
            default:
                break;
        }

        bankStartAddr = (bankSize * bankIndex);

        if (eraseFlag == true) {
            DL_FlashCTL_enableAddressOverrideMode(flashctl);
            DL_FlashCTL_setBankSelect(flashctl, bankSelect);
            DL_FlashCTL_setRegionSelect(
                flashctl, DL_FLASHCTL_REGION_SELECT_MAIN);

            DL_FlashCTL_unprotectMainMemory(flashctl);
            DL_FlashCTL_protectNonMainMemory(flashctl);
            DL_FlashCTL_eraseMemory(
                flashctl, bankStartAddr, DL_FLASHCTL_COMMAND_SIZE_BANK);
            status = DL_FlashCTL_waitForCmdDone(flashctl);
        }
        bankIndex++;
    }
    DL_FlashCTL_disableAddressOverrideMode(flashctl);

    if (DL_FactoryRegion_getDATAFlashSize() && status == true) {
        status = DL_FlashCTL_eraseDataBank(flashctl);
    }

    return (status);
}

bool DL_FlashCTL_factoryReset(FLASHCTL_Regs *flashctl)
{
    bool status;

    /* Erase Main Memory */
    status = DL_FlashCTL_massErase(flashctl);

    if (status == true) {
        DL_FlashCTL_unprotectNonMainMemory(flashctl);
        DL_FlashCTL_eraseMemory(flashctl, FLASHCTL_NONMAIN_ADDRESS,
            DL_FLASHCTL_COMMAND_SIZE_SECTOR);

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }
    return (status);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_factoryResetFromRAM(
    FLASHCTL_Regs *flashctl)
{
    DL_FLASHCTL_COMMAND_STATUS status;

    /* Erase Main Memory */
    status = DL_FlashCTL_massEraseFromRAM(flashctl);

    if (status == DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        DL_FlashCTL_unprotectNonMainMemory(flashctl);
        status = DL_FlashCTL_eraseMemoryFromRAM(flashctl,
            FLASHCTL_NONMAIN_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
    }
    return (status);
}

bool DL_FlashCTL_factoryResetMultiBank(FLASHCTL_Regs *flashctl)
{
    bool status;

    /* Erase Main Memory */
    status = DL_FlashCTL_massEraseMultiBank(flashctl);

    if (status == true) {
        DL_FlashCTL_unprotectNonMainMemory(flashctl);
        DL_FlashCTL_eraseMemory(flashctl, FLASHCTL_NONMAIN_ADDRESS,
            DL_FLASHCTL_COMMAND_SIZE_SECTOR);

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }
    return (status);
}

/**
 * @brief 编程Flash内存配置 (内部函数)
 *
 * @param flashctl Flash控制器寄存器基址
 * @param address 编程地址
 * @param cmd 编程命令
 */
static void DL_FlashCTL_programMemoryConfig(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t cmd)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            DL_FLASHCTL_COMMAND_TYPE_PROGRAM;

    flashctl->GEN.CMDBYTEN = cmd;

    /* 设置地址,地址应在要编程的扇区内 */
    DL_FlashCTL_setCommandAddress(flashctl, address);
}

static void DL_FlashCTL_programMemory8Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint8_t *data)
{
    DL_FlashCTL_programMemoryConfig(flashctl, address, cmd);

    /* Set data registers */
    flashctl->GEN.CMDDATA0 = *data;
}

static void DL_FlashCTL_programMemory16Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint16_t *data)
{
    DL_FlashCTL_programMemoryConfig(flashctl, address, cmd);

    /* Set data registers */
    flashctl->GEN.CMDDATA0 = *data;
}

static void DL_FlashCTL_programMemory32Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint32_t *data)
{
    DL_FlashCTL_programMemoryConfig(flashctl, address, cmd);

    /* Set data registers */
    flashctl->GEN.CMDDATA0 = *data;
}

static void DL_FlashCTL_programMemory64Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint32_t *data)
{
    DL_FlashCTL_programMemoryConfig(flashctl, address, cmd);

    /* Set data registers */
    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATA1 = *(data + 1);
}

/**
 * @brief 编程8位数据到Flash (无ECC)
 *
 * @param flashctl Flash控制器寄存器基址
 * @param address 编程地址
 * @param data 要编程的数据指针
 */
void DL_FlashCTL_programMemory8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    /* 仅使能低8位用于编程 */
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITHOUT_ECC, data);

    /* 设置执行位以启动命令 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 从RAM编程8位数据到Flash
 *
 * @param flashctl Flash控制器寄存器基址
 * @param address 编程地址
 * @param data 要编程的数据指针
 * @return 命令执行状态
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    /* 仅使能低8位用于编程 */
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITHOUT_ECC, data);
    /* 跳转到RAM执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 编程16位数据到Flash (无ECC)
 *
 * @param flashctl Flash控制器寄存器基址
 * @param address 编程地址
 * @param data 要编程的数据指针
 */
void DL_FlashCTL_programMemory16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    /* 使能每个数据寄存器16位用于编程 */
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITHOUT_ECC, data);

    /* 设置执行位以启动命令 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 从RAM编程16位数据到Flash
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    /* 使能每个数据寄存器16位用于编程 */
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITHOUT_ECC, data);

    /* 跳转到RAM执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 编程32位数据到Flash (无ECC)
 */
void DL_FlashCTL_programMemory32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能每个数据寄存器32位用于编程 */
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITHOUT_ECC, data);

    /* 设置执行位以启动命令 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 从RAM编程32位数据到Flash
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能每个数据寄存器32位用于编程 */
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITHOUT_ECC, data);

    /* 跳转到RAM执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 编程64位数据到Flash (无ECC)
 */
void DL_FlashCTL_programMemory64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能每个数据寄存器64位用于编程 */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITHOUT_ECC, data);

    /* 设置执行位以启动命令 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 从RAM编程64位数据到Flash
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能每个数据寄存器64位用于编程 */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITHOUT_ECC, data);

    /* 跳转到RAM执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 编程8位数据到Flash (自动生成ECC)
 */
void DL_FlashCTL_programMemory8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    /* 仅使能低8位用于编程 */
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITH_ECC, data);

    /* 设置执行位以启动命令 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    /* Only enable the bottom 8 bits for programming*/
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITH_ECC, data);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

void DL_FlashCTL_programMemory16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    /* Enable 16 bits per data register for programming*/
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITH_ECC, data);

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    /* Enable 16 bits per data register for programming*/
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITH_ECC, data);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

void DL_FlashCTL_programMemory32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* Enable 32 bits per data register for programming*/
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITH_ECC, data);
    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* Enable 32 bits per data register for programming*/
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITH_ECC, data);
    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

void DL_FlashCTL_programMemory64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* Enable 64 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITH_ECC, data);

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* Enable 64 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITH_ECC, data);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

void DL_FlashCTL_programMemory8WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint8_t *data, const uint8_t *eccCode)
{
    /* Enable 8 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data,
    const uint8_t *eccCode)
{
    /* Enable 8 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

void DL_FlashCTL_programMemory16WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint16_t *data, const uint8_t *eccCode)
{
    /* Enable 16 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data,
    const uint8_t *eccCode)
{
    /* Enable 16 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

void DL_FlashCTL_programMemory32WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint32_t *data, const uint8_t *eccCode)
{
    /* Enable 32 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode)
{
    /* Enable 32 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

void DL_FlashCTL_programMemory64WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint32_t *data, const uint8_t *eccCode)
{
    /* Enable 64 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode)
{
    /* Enable 64 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

bool DL_FlashCTL_programMemoryBlocking64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect)
{
    bool status = true;

    /* Check for valid data size */
    if (dataSize == (uint32_t) 0 ||
        ((dataSize & (uint32_t) 1) == (uint32_t) 1)) {
        status = false;
    }

    while ((dataSize != (uint32_t) 0) && status) {
        /* Clear STATCMD register before executing a flash operation */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* Unprotect sector before every write */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        DL_FlashCTL_programMemory64WithECCGenerated(flashctl, address, data);
        dataSize = dataSize - (uint32_t) 2;
        data     = data + 2;
        address  = address + (uint32_t) 8;

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }

    return (status);
}

DL_FLASHCTL_COMMAND_STATUS
DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect)
{
    DL_FLASHCTL_COMMAND_STATUS status = DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS;

    /* Check for valid data size */
    if (dataSize == (uint32_t) 0 ||
        ((dataSize & (uint32_t) 1) == (uint32_t) 1)) {
        status = DL_FLASHCTL_COMMAND_STATUS_FAILED;
    }

    while ((dataSize != (uint32_t) 0) &&
           (status != DL_FLASHCTL_COMMAND_STATUS_FAILED)) {
        /* Clear STATCMD register before executing a flash operation */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* Unprotect sector before every write */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        status = DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
            flashctl, address, data);
        dataSize = dataSize - (uint32_t) 2;
        data     = data + 2;
        address  = address + (uint32_t) 8;
    }

    return (status);
}

bool DL_FlashCTL_programMemoryBlocking64WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t *data, uint8_t *eccCode, uint32_t dataSize,
    DL_FLASHCTL_REGION_SELECT regionSelect)
{
    bool status = true;

    /* Check for valid data size */
    if (dataSize == (uint32_t) 0 ||
        ((dataSize & (uint32_t) 1) == (uint32_t) 1)) {
        status = false;
    }

    while ((dataSize != (uint32_t) 0) && status) {
        /* Clear STATCMD register before executing a flash operation */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* Unprotect sector before every write */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        DL_FlashCTL_programMemory64WithECCManual(
            flashctl, address, data, eccCode);
        dataSize = dataSize - (uint32_t) 2;
        data     = data + 2;
        eccCode  = eccCode + 1;
        address  = address + (uint32_t) 8;

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }

    return (status);
}

DL_FLASHCTL_COMMAND_STATUS
DL_FlashCTL_programMemoryBlockingFromRAM64WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint8_t *eccCode, uint32_t dataSize,
    DL_FLASHCTL_REGION_SELECT regionSelect)
{
    DL_FLASHCTL_COMMAND_STATUS status = DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS;

    /* Check for valid data size */
    if (dataSize == (uint32_t) 0 ||
        ((dataSize & (uint32_t) 1) == (uint32_t) 1)) {
        status = DL_FLASHCTL_COMMAND_STATUS_FAILED;
    }

    while ((dataSize != (uint32_t) 0) &&
           (status != DL_FLASHCTL_COMMAND_STATUS_FAILED)) {
        /* Clear STATCMD register before executing a flash operation */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* Unprotect sector before every write */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        status = DL_FlashCTL_programMemoryFromRAM64WithECCManual(
            flashctl, address, data, eccCode);
        dataSize = dataSize - (uint32_t) 2;
        data     = data + 2;
        eccCode  = eccCode + 1;
        address  = address + (uint32_t) 8;
    }

    return (status);
}

bool DL_FlashCTL_programMemoryBlocking(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t *data, uint32_t dataSize,
    DL_FLASHCTL_REGION_SELECT regionSelect)
{
    bool status = true;

    /* Check for valid data size */
    if (dataSize == (uint32_t) 0) {
        status = false;
    }

    while ((dataSize != (uint32_t) 0) && status) {
        /* Clear STATCMD register before executing a flash operation */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* Unprotect sector before every write */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        /* 32-bit case */
        if (dataSize == (uint32_t) 1) {
            DL_FlashCTL_programMemory32(flashctl, address, data);

            dataSize = dataSize - (uint32_t) 1;
            data     = data + 1;
            address  = address + (uint32_t) 4;
        } else {
            /* 64-bit case */
            DL_FlashCTL_programMemory64(flashctl, address, data);
            dataSize = dataSize - (uint32_t) 2;
            data     = data + 2;
            address  = address + (uint32_t) 8;
        }

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }

    return (status);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect)
{
    DL_FLASHCTL_COMMAND_STATUS status = DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS;

    /* Check for valid data size */
    if (dataSize == (uint32_t) 0) {
        status = DL_FLASHCTL_COMMAND_STATUS_FAILED;
    }

    while ((dataSize != (uint32_t) 0) &&
           (status != DL_FLASHCTL_COMMAND_STATUS_FAILED)) {
        /* Unprotect sector before every write */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        /* 32-bit case */
        if (dataSize == (uint32_t) 1) {
            status =
                DL_FlashCTL_programMemoryFromRAM32(flashctl, address, data);

            dataSize = dataSize - (uint32_t) 1;
            data     = data + 1;
            address  = address + (uint32_t) 4;
        } else {
            /* 64-bit case */
            status =
                DL_FlashCTL_programMemoryFromRAM64(flashctl, address, data);
            dataSize = dataSize - (uint32_t) 2;
            data     = data + 2;
            address  = address + (uint32_t) 8;
        }
    }

    return (status);
}

/**
 * @brief 解除主内存的写保护和擦除保护
 */
void DL_FlashCTL_unprotectMainMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTA = 0;
    flashctl->GEN.CMDWEPROTB = 0;
    flashctl->GEN.CMDWEPROTC = 0;
}

/**
 * @brief 解除数据内存的写保护和擦除保护
 */
void DL_FlashCTL_unprotectDataMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTA = 0;
    flashctl->GEN.CMDWEPROTB = 0;
    flashctl->GEN.CMDWEPROTC = 0;
}

/**
 * @brief 设置主内存的写保护和擦除保护
 */
void DL_FlashCTL_protectMainMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTA = FLASHCTL_CMDWEPROTA_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTB = FLASHCTL_CMDWEPROTB_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTC = FLASHCTL_CMDWEPROTC_VAL_MAXIMUM;
}

/**
 * @brief 解除非主内存的写保护和擦除保护
 */
void DL_FlashCTL_unprotectNonMainMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTNM = 0;
}

/**
 * @brief 设置非主内存的写保护和擦除保护
 */
void DL_FlashCTL_protectNonMainMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTNM = FLASHCTL_CMDWEPROTNM_VAL_MAXIMUM;
}

/**
 * @brief 解除所有内存的写保护和擦除保护
 */
void DL_FlashCTL_unprotectAllMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTA  = 0;
    flashctl->GEN.CMDWEPROTB  = 0;
    flashctl->GEN.CMDWEPROTC  = 0;
    flashctl->GEN.CMDWEPROTNM = 0;
    flashctl->GEN.CMDWEPROTTR = 0;
    flashctl->GEN.CMDWEPROTEN = 0;
}

/**
 * @brief 设置所有内存的写保护和擦除保护
 */
void DL_FlashCTL_protectAllMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTA  = FLASHCTL_CMDWEPROTA_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTB  = FLASHCTL_CMDWEPROTB_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTC  = FLASHCTL_CMDWEPROTC_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTNM = FLASHCTL_CMDWEPROTNM_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTTR = FLASHCTL_CMDWEPROTTR_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTEN = FLASHCTL_CMDWEPROTEN_VAL_MAXIMUM;
}

#ifdef DEVICE_HAS_NO_CMDWEPROTA
void DL_FlashCTL_unprotectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
    DL_FLASHCTL_REGION_SELECT regionSelect)
{
    uint32_t sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
    uint32_t sectorInBank =
        DL_FlashCTL_getFlashSectorNumberInBank(flashctl, addr);
    uint32_t sectorMask;

    /*
     * Devices without CMDWEPROTA will use CMDWEPROTB to unprotect all sectors of MAIN memory
     */

    if ((uint32_t) regionSelect == FLASHCTL_CMDCTL_REGIONSEL_MAIN) {
        sectorMask = (uint32_t) 1
                     << ((sectorInBank / (uint32_t) 8) % (uint32_t) 32);
        flashctl->GEN.CMDWEPROTB &= ~sectorMask;
    } else if ((uint32_t) regionSelect == FLASHCTL_CMDCTL_REGIONSEL_NONMAIN) {
        sectorMask = (uint32_t) 1 << (sectorNumber % (uint32_t) 32);
        flashctl->GEN.CMDWEPROTNM &= ~sectorMask;
    } else {
        ; /* Not expected to reach this else statement */
    }
}
#else
void DL_FlashCTL_unprotectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
    DL_FLASHCTL_REGION_SELECT regionSelect)
{
    uint32_t sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
    uint32_t sectorInBank =
        DL_FlashCTL_getFlashSectorNumberInBank(flashctl, addr);
    uint8_t numBanks              = DL_FactoryRegion_getNumBanks();
    uint32_t mainFlashSize        = DL_FactoryRegion_getMAINFlashSize();
    uint32_t physicalSectorNumber = 0;
    uint32_t sectorMask;

    if ((uint32_t) regionSelect == FLASHCTL_CMDCTL_REGIONSEL_MAIN) {
        /* If the banks have been swapped, CMDWEPROTA only protects physical
         * bank 0 (logical bank 1 in a swap), so if the address points to this
         * region we must protect it using CMDWEPROTA instead of CMDWEPROTB
         */
        if (DL_SYSCTL_isExecuteFromUpperFlashBank() && numBanks > 1) {
            /* physical sectors are swapped. Calculate physical sector to
             * determine use of CMDWEPROTA */
            if (sectorNumber >= (mainFlashSize / 2)) {
                physicalSectorNumber = sectorNumber - (mainFlashSize / 2);
            } else {
                physicalSectorNumber = sectorNumber + (mainFlashSize / 2);
            }
        } else {
            physicalSectorNumber = sectorNumber;
        }

        if (physicalSectorNumber < (uint32_t) 32) {
            /* Use CMDWEPROTA */
            sectorMask = (uint32_t) 1 << physicalSectorNumber;
            flashctl->GEN.CMDWEPROTA &= ~sectorMask;
        } else {
            /* Use CMDWEPROTB */
            if (sectorInBank < (uint32_t) 256) {
                /* Single bank system */
                if (DL_FactoryRegion_getNumBanks() == (uint32_t) 1) {
                    sectorMask =
                        (uint32_t) 1
                        << ((sectorInBank - (uint32_t) 32) / (uint32_t) 8);
                    flashctl->GEN.CMDWEPROTB &= ~sectorMask;
                } else { /* Multi bank system */
                    sectorMask = (uint32_t) 1 << (sectorInBank / (uint32_t) 8);
                    flashctl->GEN.CMDWEPROTB &= ~sectorMask;
                }
            }
            /* Use CMDWEPROTC */
            else if (sectorInBank < (uint32_t) 511) {
                sectorMask =
                    ((uint32_t) 1
                        << ((sectorInBank - (uint32_t) 256) / (uint32_t) 8));
                flashctl->GEN.CMDWEPROTC &= ~sectorMask;
            } else {
                ; /* Not expected to reach this else statement */
            }
        }
    } else if ((uint32_t) regionSelect == FLASHCTL_CMDCTL_REGIONSEL_NONMAIN) {
        sectorMask = (uint32_t) 1 << (sectorNumber % (uint32_t) 32);
        flashctl->GEN.CMDWEPROTNM &= ~sectorMask;
    } else {
        ; /* Not expected to reach this else statement */
    }
}
#endif

#ifdef DEVICE_HAS_NO_CMDWEPROTA
void DL_FlashCTL_protectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
    DL_FLASHCTL_REGION_SELECT regionSelect)
{
    uint32_t sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
    uint32_t sectorInBank =
        DL_FlashCTL_getFlashSectorNumberInBank(flashctl, addr);
    uint32_t sectorMask;

    /*
     * Devices without CMDWEPROTA will use CMDWEPROTB to protect all sectors of MAIN memory
     */

    if ((uint32_t) regionSelect == FLASHCTL_CMDCTL_REGIONSEL_MAIN) {
        sectorMask = ((uint32_t) 1 << (sectorInBank / (uint32_t) 8));
        flashctl->GEN.CMDWEPROTB |= sectorMask;
    } else if ((uint32_t) regionSelect == FLASHCTL_CMDCTL_REGIONSEL_NONMAIN) {
        sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
        sectorMask   = (uint32_t) 1 << (sectorNumber % (uint32_t) 32);
        flashctl->GEN.CMDWEPROTNM |= sectorMask;
    } else {
        ; /* Not expected to reach this else statement */
    }
}
#else
void DL_FlashCTL_protectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
    DL_FLASHCTL_REGION_SELECT regionSelect)
{
    uint32_t sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
    uint32_t sectorInBank =
        DL_FlashCTL_getFlashSectorNumberInBank(flashctl, addr);
    uint8_t numBanks              = DL_FactoryRegion_getNumBanks();
    uint32_t mainFlashSize        = DL_FactoryRegion_getMAINFlashSize();
    uint32_t physicalSectorNumber = 0;
    uint32_t sectorMask;

    if ((uint32_t) regionSelect == FLASHCTL_CMDCTL_REGIONSEL_MAIN) {
        /* If the banks have been swapped, CMDWEPROTA only protects physical
         * bank 0 (logical bank 1 in a swap), so if the address points to this
         * region we must protect it using CMDWEPROTA instead of CMDWEPROTB
         */
        if (DL_SYSCTL_isExecuteFromUpperFlashBank() && numBanks > 1) {
            /* physical sectors are swapped. Calculate physical sector to
             * determine use of CMDWEPROTA */
            if (sectorNumber >= (mainFlashSize / 2)) {
                physicalSectorNumber = sectorNumber - (mainFlashSize / 2);
            } else {
                physicalSectorNumber = sectorNumber + (mainFlashSize / 2);
            }
        } else {
            physicalSectorNumber = sectorNumber;
        }

        if (physicalSectorNumber < (uint32_t) 32) {
            /* Use CMDWEPROTA */
            sectorMask = (uint32_t) 1 << physicalSectorNumber;
            flashctl->GEN.CMDWEPROTA |= sectorMask;
        } else {
            /* Use CMDWEPROTB */
            if (sectorNumber < (uint32_t) 256) {
                /* Single bank system */
                if (DL_FactoryRegion_getNumBanks() == (uint32_t) 1) {
                    sectorMask =
                        (uint32_t) 1
                        << ((sectorInBank - (uint32_t) 32) / (uint32_t) 8);
                    flashctl->GEN.CMDWEPROTB |= sectorMask;
                } else { /* Multi bank system */
                    sectorMask =
                        ((uint32_t) 1 << (sectorInBank / (uint32_t) 8));
                    flashctl->GEN.CMDWEPROTB |= sectorMask;
                }
            }
            /* Use CMDWEPROTC */
            else if (sectorNumber < (uint32_t) 511) {
                sectorMask =
                    ((uint32_t) 1
                        << ((sectorInBank - (uint32_t) 256) / (uint32_t) 8));
                flashctl->GEN.CMDWEPROTC |= sectorMask;
            } else {
                ; /* Not expected to reach this else statement */
            }
        }
    } else if ((uint32_t) regionSelect == FLASHCTL_CMDCTL_REGIONSEL_NONMAIN) {
        sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
        sectorMask   = (uint32_t) 1 << (sectorNumber % (uint32_t) 32);
        flashctl->GEN.CMDWEPROTNM |= sectorMask;
    } else {
        ; /* Not expected to reach this else statement */
    }
}
#endif

/**
 * @brief 读取验证配置 (内部函数)
 */
static void DL_FlashCTL_readVerifyConfig(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t cmd)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    flashctl->GEN.CMDBYTEN = cmd;

    /* 设置地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);
}

/**
 * @brief 读取验证8位配置 (内部函数)
 */
static void DL_FlashCTL_readVerify8Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint8_t *data)
{
    DL_FlashCTL_readVerifyConfig(flashctl, address, cmd);

    flashctl->GEN.CMDDATA0 = *data;
}

/**
 * @brief 读取验证16位配置 (内部函数)
 */
static void DL_FlashCTL_readVerify16Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint16_t *data)
{
    DL_FlashCTL_readVerifyConfig(flashctl, address, cmd);

    flashctl->GEN.CMDDATA0 = *data;
}

/**
 * @brief 读取验证32位配置 (内部函数)
 */
static void DL_FlashCTL_readVerify32Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint32_t *data)
{
    DL_FlashCTL_readVerifyConfig(flashctl, address, cmd);

    flashctl->GEN.CMDDATA0 = *data;
}

/**
 * @brief 读取验证64位配置 (内部函数)
 */
static void DL_FlashCTL_readVerify64Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint32_t *data)
{
    DL_FlashCTL_readVerifyConfig(flashctl, address, cmd);

    /* 设置数据寄存器 */
    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATA1 = *(data + 1);
}

void DL_FlashCTL_readVerify8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_8_WITHOUT_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_readVerify16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_16_WITHOUT_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_readVerify32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_32_WITHOUT_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_readVerify64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_64_WITHOUT_ECC;

    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATA1 = *(data + 1);

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    DL_FlashCTL_readVerify8Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_8_WITHOUT_ECC, data);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    DL_FlashCTL_readVerify16Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_16_WITHOUT_ECC, data);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    DL_FlashCTL_readVerify32Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_32_WITHOUT_ECC, data);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    DL_FlashCTL_readVerify64Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_64_WITHOUT_ECC, data);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    DL_FlashCTL_readVerify8Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_8_WITH_ECC, data);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    DL_FlashCTL_readVerify16Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_16_WITH_ECC, data);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    DL_FlashCTL_readVerify32Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_32_WITH_ECC, data);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    DL_FlashCTL_readVerify64Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_64_WITH_ECC, data);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data,
    const uint8_t *eccCode)
{
    DL_FlashCTL_readVerify8Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_8_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data,
    const uint8_t *eccCode)
{
    DL_FlashCTL_readVerify16Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_16_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode)
{
    DL_FlashCTL_readVerify32Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_32_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode)
{
    DL_FlashCTL_readVerify64Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_64_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

void DL_FlashCTL_readVerify8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison, with ECC enabled */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_8_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_readVerify16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison, with ECC enabled */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_16_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_readVerify32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison, with ECC enabled */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_32_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_readVerify64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison, with ECC enabled */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_64_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATA1 = *(data + 1);

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_readVerify8WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint8_t *data, const uint8_t *eccCode)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison, with ECC enabled */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_8_WITH_ECC;

    flashctl->GEN.CMDDATA0    = *data;
    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_readVerify16WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint16_t *data, const uint8_t *eccCode)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison, with ECC enabled */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_16_WITH_ECC;

    flashctl->GEN.CMDDATA0    = *data;
    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_readVerify32WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint32_t *data, const uint8_t *eccCode)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison, with ECC enabled */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_32_WITH_ECC;

    flashctl->GEN.CMDDATA0    = *data;
    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_readVerify64WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint32_t *data, const uint8_t *eccCode)
{
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Configure bytes to use for comparison, with ECC enabled */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_64_WITH_ECC;

    flashctl->GEN.CMDDATA0    = *data;
    flashctl->GEN.CMDDATA1    = *(data + 1);
    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_blankVerify(FLASHCTL_Regs *flashctl, uint32_t address)
{
    /* Set command and word size. BLANKVERIFY can only be applied to a single
     * flash word at a time */
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_BLANK_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_blankVerifyFromRAM(
    FLASHCTL_Regs *flashctl, uint32_t address)
{
    /* Set command and word size. BLANKVERIFY can only be applied to a single
     * flash word at a time */
    flashctl->GEN.CMDTYPE = (uint32_t) DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t) DL_FLASHCTL_COMMAND_TYPE_BLANK_VERIFY;

    /* Set the address we want to verify */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* Jump to RAM to execute command and wait for completion */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

bool DL_FlashCTL_eraseDataBank(FLASHCTL_Regs *flashctl)
{
    bool status              = true;
    uint32_t address         = FLASHCTL_DATA_ADDRESS;
    uint8_t dataFlashSectors = DL_FactoryRegion_getDATAFlashSize();

    while ((dataFlashSectors != (uint8_t) 0) && (status == true)) {
        DL_FlashCTL_executeClearStatus(flashctl);
        DL_FlashCTL_unprotectSector(
            flashctl, address, DL_FLASHCTL_REGION_SELECT_MAIN);
        DL_FlashCTL_eraseMemory(
            flashctl, address, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
        status = DL_FlashCTL_waitForCmdDone(flashctl);

        address          = address + (uint32_t) DL_FLASHCTL_SECTOR_SIZE;
        dataFlashSectors = dataFlashSectors - (uint8_t) 1;
    }

    return (status);
}

DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_eraseDataBankFromRAM(
    FLASHCTL_Regs *flashctl)
{
    DL_FLASHCTL_COMMAND_STATUS status = DL_FLASHCTL_COMMAND_STATUS_PASSED;
    uint32_t address                  = FLASHCTL_DATA_ADDRESS;
    uint8_t dataFlashSectors          = DL_FactoryRegion_getDATAFlashSize();

    while ((dataFlashSectors != (uint8_t) 0) &&
           (status == DL_FLASHCTL_COMMAND_STATUS_PASSED)) {
        DL_FlashCTL_executeClearStatus(flashctl);
        DL_FlashCTL_unprotectSector(
            flashctl, address, DL_FLASHCTL_REGION_SELECT_MAIN);
        status = DL_FlashCTL_eraseMemoryFromRAM(
            flashctl, address, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

        address          = address + (uint32_t) DL_FLASHCTL_SECTOR_SIZE;
        dataFlashSectors = dataFlashSectors - (uint8_t) 1;
    }

    return (status);
}

#ifdef DEVICE_HAS_FLASH_128_BIT_WORD

static void DL_FlashCTL_programMemory128Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint32_t *data);
static void DL_FlashCTL_programMemoryConfigMultiWord(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, DL_FLASHCTL_COMMAND_SIZE cmdSize);

/*!
 * @brief Enable programming 128 bits without ECC enabled
 */
#define DL_FLASHCTL_PROGRAM_128_WITHOUT_ECC (0x0000FFFF)

/*!
 * @brief Enable programming 128 bits with ECC enabled
 */
#define DL_FLASHCTL_PROGRAM_128_WITH_ECC (0x0001FFFF)

static void DL_FlashCTL_programMemoryConfigMultiWord(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, DL_FLASHCTL_COMMAND_SIZE cmdSize)
{
    flashctl->GEN.CMDTYPE =
        (uint32_t) cmdSize | DL_FLASHCTL_COMMAND_TYPE_PROGRAM;

    flashctl->GEN.CMDBYTEN = cmd;

    /* Set address, address should be in the sector that we want to erase */
    DL_FlashCTL_setCommandAddress(flashctl, address);
}

static void DL_FlashCTL_programMemory128Config(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t cmd, const uint32_t *data)
{
    DL_FlashCTL_programMemoryConfigMultiWord(
        flashctl, address, cmd, DL_FLASHCTL_COMMAND_SIZE_TWO_WORDS);

    /* Set data registers */
    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATA1 = *(data + 1);
    flashctl->GEN.CMDDATA2 = *(data + 2);
    flashctl->GEN.CMDDATA3 = *(data + 3);
}

void DL_FlashCTL_programMemory128(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* Enable 128 bits per data register for programming*/
    DL_FlashCTL_programMemory128Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_128_WITHOUT_ECC, data);

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_programMemory128WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* Enable 128 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory128Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_128_WITH_ECC, data);

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

void DL_FlashCTL_programMemory128WithECCManual(FLASHCTL_Regs *flashctl,
    uint32_t address, const uint32_t *data, const uint8_t *eccCode)
{
    /* Enable 128 bits per data register for programming, with ECC enabled */
    DL_FlashCTL_programMemory128Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_128_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;
    flashctl->GEN.CMDDATAECC1 = *(eccCode + 1);

    /* Set bit to execute command */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

bool DL_FlashCTL_programMemoryBlocking128WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect)
{
    bool status = true;

    /* Check for valid data size */
    if ((dataSize == (uint32_t) 0) || dataSize < (uint32_t) 4) {
        status = false;
    }

    while ((dataSize != (uint32_t) 0) && status) {
        /* Clear STATCMD register before executing a flash operation */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* Unprotect sector before every write */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        /* 32-bit case */
        if (dataSize == (uint32_t) 1) {
            DL_FlashCTL_programMemory32WithECCGenerated(
                flashctl, address, data);

            dataSize = dataSize - (uint32_t) 1;
            data     = data + 1;
            address  = address + (uint32_t) 4;
        } else if (dataSize < (uint32_t) 4) {
            /* 64-bit case */
            DL_FlashCTL_programMemory64WithECCGenerated(
                flashctl, address, data);
            dataSize = dataSize - (uint32_t) 2;
            data     = data + 2;
            address  = address + (uint32_t) 8;
        } else {
            /* 128-bit case */
            DL_FlashCTL_programMemory128WithECCGenerated(
                flashctl, address, data);
            dataSize = dataSize - (uint32_t) 4;
            data     = data + 4;
            address  = address + (uint32_t) 16;
        }

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }

    return (status);
}

#endif /* DEVICE_HAS_FLASH_128_BIT_WORD */
