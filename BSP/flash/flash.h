/**
 * @file flash.h
 * @brief 片上 Flash 读写接口。
 * @note
 * 1. 这个库只处理主 Flash 区域。
 * 2. 写入地址必须 8 字节对齐，这是芯片硬件要求，不是我故意找你麻烦。
 * 3. 写新数据前，通常要先擦除所在扇区，否则大概率写不进去或者结果不对。
 *
 * 调用说明：
 * 1. FLASH_init()：初始化 Flash 控制器。一般开机后调一次。
 * 2. FLASH_write(address, data, size)：往 Flash 写数据。address 是起始地址，data 是数据地址，size 是字节数。
 * 3. FLASH_read(address, data, size)：从 Flash 读数据。address 是起始地址，data 是接收缓冲区，size 是读取字节数。
 * 4. FLASH_eraseSector(address)：擦除某个扇区。address 只要落在目标扇区里就行，不一定非得是扇区起始地址。
 * 5. FLASH_isAddressValid(address)：检查这个地址能不能用于写操作。
 * 6. FLASH_runSelfTest()：跑 Flash 自测。
 */
#ifndef _FLASH_H_
#define _FLASH_H_

#include "ti_msp_dl_config.h"
#include <stdbool.h>
#include <stdint.h>

#define FLASH_INST                FLASHCTL
#define FLASH_REGION_MAIN         DL_FLASHCTL_REGION_SELECT_MAIN
#define FLASH_SECTOR_SIZE         DL_FLASHCTL_SECTOR_SIZE
#define FLASH_PROGRAM_UNIT        (8U)
/**
 * @brief 初始化 Flash 控制器
 * @note
 * 这里只负责清状态，不负责系统级 wait state 配置。
 */
void FLASH_init(void);

/**
 * @brief 写入数据到 Flash
 * @param address Flash 起始地址，必须 8 字节对齐
 * @param data    要写入的数据首地址
 * @param size    要写入的字节数
 * @return true   写入成功
 * @return false  写入失败
 * @note
 * 底层实际按 8 字节为一个单位写入。
 * 如果最后一块不足 8 字节，函数会自动补 0xFF 再写。
 */
bool FLASH_write(uint32_t address, const void *data, uint32_t size);

/**
 * @brief 从 Flash 读取数据
 * @param address Flash 起始地址
 * @param data    用来接收数据的缓冲区
 * @param size    要读取的字节数
 * @return true   读取成功
 * @return false  读取失败
 * @note 读取不要求 8 字节对齐，只要求地址范围合法。
 */
bool FLASH_read(uint32_t address, void *data, uint32_t size);

/**
 * @brief 擦除包含指定地址的扇区
 * @param address 扇区内任意一个地址
 * @return true   擦除成功
 * @return false  擦除失败
 * @note
 * 你传入的不需要是扇区起始地址，只要这个地址落在目标扇区内即可。
 * 函数会让底层擦掉这个地址所在的整个扇区。
 */
bool FLASH_eraseSector(uint32_t address);

/**
 * @brief 检查写入地址是否有效
 * @param address Flash 地址
 * @return true   地址有效，且满足 8 字节对齐要求
 * @return false  地址无效
 * @note
 * 这个函数主要给写操作做前置检查。
 */
bool FLASH_isAddressValid(uint32_t address);


#endif