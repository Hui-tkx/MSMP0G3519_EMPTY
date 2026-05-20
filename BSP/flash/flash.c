/**
  * @file         flash.c
    * @details      Flash 存储控制实现文件
  */

#include "flash.h"
#include "../uart/uart.h"
#include <ti/driverlib/m0p/dl_factoryregion.h>
#include <stddef.h>
#include <string.h>

static uint32_t FLASH_getMainSizeBytes(void)
{
    return ((uint32_t) DL_FactoryRegion_getMAINFlashSize()) * 1024U;
}

static bool FLASH_isRangeInMain(uint32_t address, uint32_t size)
{
    uint32_t endAddress;
    uint32_t mainSizeBytes = FLASH_getMainSizeBytes();

    /* 长度为 0 没有意义，直接判失败。 */
    if (size == 0U) {
        return false;
    }

    /* 起始地址超出主 Flash 范围。 */
    if ((mainSizeBytes == 0U) || (address >= mainSizeBytes)) {
        return false;
    }

    endAddress = address + size;
    /* 防止 address + size 发生无符号溢出。 */
    if (endAddress < address) {
        return false;
    }

    /* 要求整段数据都位于主 Flash 范围内。 */
    return (endAddress <= mainSizeBytes);
}

static bool FLASH_programWord(uint32_t address, const uint8_t *data, uint32_t size)
{
    uint32_t programData[2] = {0xFFFFFFFFU, 0xFFFFFFFFU};
    DL_FLASHCTL_COMMAND_STATUS status;

    /*
     * 一次固定写 8 字节。
     * 如果本次数据不足 8 字节，剩余部分保留为 0xFF。
     */
    memcpy((uint8_t *) programData, data, size);

    /*
     * 每次编程前都先清状态，再对当前地址所在扇区解除保护。
     * TI 的 Flash 控制器在一次操作结束后会重新进入保护状态。
     */
    DL_FlashCTL_executeClearStatus(FLASH_INST);
    DL_FlashCTL_unprotectSector(FLASH_INST, address, FLASH_REGION_MAIN);

    /*
     * 主 Flash 正常写入时需要同时生成 ECC。
     * 这里使用带 ECC 自动生成的版本，避免只写数据不写 ECC 导致后续异常。
     */
    status = DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
        FLASH_INST, address, programData);

    /* 底层命令执行失败。 */
    if (status != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        return false;
    }

    /*
     * 再读回这 8 字节做一次校验。
     * 这样能避免“函数返回成功，但实际内容没写对”的情况。
     */
    return (memcmp((const void *) (uintptr_t) address, programData,
        FLASH_PROGRAM_UNIT) == 0);
}

/**
 * @brief 初始化 Flash
 */
void FLASH_init(void)
{
    /* 清除上一次擦写可能留下的状态标志。 */
    DL_FlashCTL_executeClearStatus(FLASH_INST);
}

/**
 * @brief 写入数据到 Flash
 */
bool FLASH_write(uint32_t address, const void *data, uint32_t size)
{
    const uint8_t *dataPtr = (const uint8_t *) data;
    uint32_t currentAddress = address;
    uint32_t remainingSize = size;
    uint32_t chunkSize;

    /* 先做最基本的参数检查。 */
    if ((data == NULL) || (size == 0U) || !FLASH_isAddressValid(address)) {
        return false;
    }

    /* 要求整段待写区域都在主 Flash 合法范围内。 */
    if (!FLASH_isRangeInMain(address, size)) {
        return false;
    }

    /*
     * 循环按 8 字节写入。
     * 这里 currentAddress 每次加 8，而不是按 1/2/4 增长，
     * 是因为底层接口要求目标地址始终满足 8 字节对齐。
     */
    while (remainingSize > 0U) {
        chunkSize = (remainingSize >= FLASH_PROGRAM_UNIT) ?
            FLASH_PROGRAM_UNIT : remainingSize;

        /* 当前 8 字节块写失败时，整个写操作直接返回失败。 */
        if (!FLASH_programWord(currentAddress, dataPtr, chunkSize)) {
            return false;
        }

        currentAddress += FLASH_PROGRAM_UNIT;
        dataPtr += chunkSize;
        remainingSize -= chunkSize;
    }

    return true;
}

/**
 * @brief 从 Flash 读取数据
 */
bool FLASH_read(uint32_t address, void *data, uint32_t size)
{
    uint8_t *dataPtr;
    const uint8_t *flashPtr;
    uint32_t i;

    /* 读取只要求目标范围合法，不要求地址 8 字节对齐。 */
    if ((data == NULL) || !FLASH_isRangeInMain(address, size)) {
        return false;
    }

    /*
     * 这里故意不用库函数 memcpy，直接逐字节读。
     * 这样在裸机环境里更直观，也更方便排查问题。
     */
    dataPtr = (uint8_t *) data;
    flashPtr = (const uint8_t *) (uintptr_t) address;

    for (i = 0U; i < size; i++) {
        dataPtr[i] = flashPtr[i];
    }

    return true;
}

/**
 * @brief 擦除 Flash 扇区
 */
bool FLASH_eraseSector(uint32_t address)
{
    DL_FLASHCTL_COMMAND_STATUS status;

    /* 只要地址落在主 Flash 内，就允许擦除其所在扇区。 */
    if (!FLASH_isRangeInMain(address, 1U)) {
        return false;
    }

    /* 擦除流程与写入类似：先清状态，再解除扇区保护。 */
    DL_FlashCTL_executeClearStatus(FLASH_INST);
    DL_FlashCTL_unprotectSector(FLASH_INST, address, FLASH_REGION_MAIN);

    status = DL_FlashCTL_eraseMemoryFromRAM(
        FLASH_INST, address, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

    /* 擦除命令成功返回 true。 */
    return (status == DL_FLASHCTL_COMMAND_STATUS_PASSED);
}

/**
 * @brief 检查写地址是否合法
 */
bool FLASH_isAddressValid(uint32_t address)
{
    /* 写地址必须 8 字节对齐。 */
    if ((address & (FLASH_PROGRAM_UNIT - 1U)) != 0U) {
        return false;
    }

    /* 还要保证从该地址开始的 8 字节都在主 Flash 范围内。 */
    return FLASH_isRangeInMain(address, FLASH_PROGRAM_UNIT);
}
