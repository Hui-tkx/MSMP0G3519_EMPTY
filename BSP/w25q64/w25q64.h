/**
 * @file w25q64.h
 * @brief W25Q64 SPI Flash 驱动 (阻塞 + DMA 非阻塞读写)
 *
 * 芯片: W25Q64 (8MB/64Mbit), 地址 0x000000~0x7FFFFF
 * 结构: 页 256B, 扇区 4KB, 块 64KB
 * 写入: 必须先擦除(置1)再写入(置0), 擦除最小单位=扇区
 */
#ifndef _W25Q64_H_
#define _W25Q64_H_

#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdbool.h>

/* ========== 存储结构常量 ========== */
#define W25Q64_PAGE_SIZE    256     /*!< 页大小 (字节)    */
#define W25Q64_SECTOR_SIZE  4096    /*!< 扇区大小 (字节)   */
#define W25Q64_BLOCK_SIZE   65536   /*!< 块大小 (字节)     */
#define W25Q64_START_ADDR   ((uint32_t)0x000000)  /*!< Flash 起始地址 */
#define W25Q64_END_ADDR     ((uint32_t)0x7FFFFF)  /*!< Flash 结束地址 */

/* ========== 状态寄存器位 ========== */
#define W25Q64_BUSY         0x01    /*!< bit0: 忙标志 (1=擦除/写入中) */
#define W25Q64_WEL          0x02    /*!< bit1: 写使能锁存 (1=已使能)  */

/* ========== 非阻塞操作状态 ========== */
/*! @enum  DMA 非阻塞操作状态: IDLE→BUSY→DONE/ERROR */
typedef enum {
    W25Q64_NB_IDLE  = 0,    /*!< 空闲, 可发起新操作 */
    W25Q64_NB_BUSY  = 1,    /*!< 忙, 继续调用 Process() */
    W25Q64_NB_DONE  = 2,    /*!< 操作成功完成 */
    W25Q64_NB_ERROR = 3,    /*!< 操作失败 */
} W25Q64_NB_State;

/* ========== API 函数声明 ========== */

/* --- 初始化 & 识别 --- */
void     W25Q64_InitConfig(void);                                       /*!< 初始化 SPI + GPIO CS */
uint32_t W25Q64_GetID(void);                                            /*!< 读 JEDEC ID (0xEF4017) */

/* --- 状态 & 控制 --- */
uint8_t  W25Q64_ReadState(void);                                        /*!< 读状态寄存器 (BUSY/WEL) */
uint8_t  W25Q64_WriteENABLE(void);                                      /*!< 发送写使能, 返回状态 */
uint8_t  W25Q64_ReadWriteByte(uint8_t dat);                             /*!< SPI 收发 1 字节 (阻塞) */

/* --- 擦除 (阻塞) --- */
void W25Q64_erase_sector(uint32_t addr);                                /*!< 扇区擦除 4KB (阻塞 ~45ms) */
void W25Q64_BlockErase(uint32_t addr);                                  /*!< 块擦除 64KB (阻塞 ~200ms) */

/* --- 阻塞读取 --- */
uint8_t W25Q64_ReadByte(uint32_t addr);                                 /*!< 读 1 字节 (Fast Read) */

/* --- DMA 非阻塞读写 --- */
void            W25Q64_read16(uint16_t *buf, uint32_t addr, uint32_t size);   /*!< 发起 DMA 读 (size=uint16_t 数) */
void            W25Q64_write16(uint16_t *buf, uint32_t addr, uint32_t size);  /*!< 发起 DMA 写 (需先擦除) */
W25Q64_NB_State W25Q64_Process(void);                                         /*!< 推进 DMA 状态机 */

#endif /* _W25Q64_H_ 
 *
 *   // 或者直接判断
 *   uint8_t state = W25Q64_ReadState();
 *   if (state & W25Q64_BUSY) {
 *       // 忙，不能写入
 *   }
 *   if (state & W25Q64_WEL) {
 *       // 写使能已开启，可以擦除/写入
 *   }
 * @endcode
 */
uint8_t W25Q64_ReadState(void);


/**
 * ============================================================================
 * @brief W25Q64_ReadByte - 阻塞式读取单个字节
 * ============================================================================
 *
 * 【功能说明】
 *   从指定地址读取 1 个字节，使用 Fast Read 命令 (0x0B)
 *   函数会等待读取完成才返回
 *
 * 【使用场景】
 *   - 读取配置参数、状态标志等小数据
 *   - 调试时读取特定地址的值
 *
 * @param addr Flash 地址 (有效范围: 0x000000 ~ 0x7FFFFF)
 *
 * @return uint8_t - 读取到的数据
 * @return 如果地址超出范围，返回 0
 *
 * @note 【大白话】"我要读 1 个字节，等我读完再告诉我"
 *
 * @note 【阻塞特性】函数内部会等待 SPI 传输完成，如果 SPI 忙会等待
 *
 * 【使用示例】
 * @code
 *   // 读取地址 0x1000 的数据
 *   uint8_t data = W25Q64_ReadByte(0x1000);
 *
 *   // 读取多个连续地址
 *   for (uint32_t addr = 0x1000; addr < 0x1010; addr++) {
 *       uint8_t byte = W25Q64_ReadByte(addr);
 *       printf("Addr 0x%06X: 0x%02X\r\n", addr, byte);
 *   }
 *
 *   // 读取数据并校验
 *   uint8_t expected = 0x55;
 *   uint8_t actual = W25Q64_ReadByte(0x2000);
 *   if (actual != expected) {
 *       printf("校验失败! 期望: 0x%02X, 实际: 0x%02X\r\n", expected, actual);
 *   }
 * @endcode
 */
uint8_t W25Q64_ReadByte(uint32_t addr);


/**
 * ============================================================================
 * @brief W25Q64_WriteENABLE - 发送写使能指令
 * ============================================================================
 *
 * 【功能说明】
 *   发送 Write Enable 命令 (0x06)，开启写使能
 *   开启后，WEL 位被置 1，允许执行擦除/写入操作
 *
 * 【重要特性】
 *   - 执行擦除/写入命令前必须先执行写使能
 *   - 每次上电/复位后，WEL 位默认是 0
 *   - 执行一次擦除/写入后，WEL 位会自动清除为 0
 *   - 如果要连续擦除/写入多个扇区/页，需要每次都重新调用
 *
 * @note 【自动执行】实际上，大多数擦除/写入函数内部已经自动调用了此函数，
 *        所以一般情况下不需要手动调用此函数
 *
 * @param (无)
 *
 * @return uint8_t - 返回状态寄存器的值（用于判断 WEL 位是否成功置 1）
 *         - 返回值 & W25Q64_WEL != 0 表示写使能成功
 *         - 返回值 & W25Q64_WEL == 0 表示写使能失败
 *
 * @note 【大白话】"告诉芯片: 我准备写数据了，开门！"
 *
 * 【使用示例】
 * @code
 *   // 一般不需要手动调用，以下是高级用法示例
 *
 *   // 连续写入多个扇区
 *   for (uint32_t sector = 0; sector < 10; sector++) {
 *       // 每次写入前都需要重新使能写使能
 *       W25Q64_WriteENABLE();
 *       // 等待一下确保 WEL 位设置完成
 *       while (!(W25Q64_ReadState() & W25Q64_WEL));
 *       // 然后擦除
 *       W25Q64_erase_sector(sector * W25Q64_SECTOR_SIZE);
 *   }
 * @endcode
 */
uint8_t W25Q64_WriteENABLE(void);


/**
 * ============================================================================
 * @brief W25Q64_erase_sector - 扇区擦除 (4KB)
 * ============================================================================
 *
 * 【功能说明】
 *   擦除指定扇区，将 4KB 区域内的所有字节设置为 0xFF
 *
 * 【重要特性】
 *   ⚠️ 擦除操作会破坏原有数据，擦除后所有位变为 1 (0xFF)
 *   ⚠️ 写入前必须先擦除，否则数据会出错
 *   ⚠️ 此函数是【阻塞式】，会等待擦除完成才返回（约 30~200ms）
 *
 * 【擦除时间】
 *   典型值: 45ms，最坏情况: 400ms
 *
 * @param addr 扇区起始地址
 *             - 有效值: 0x000000, 0x1000, 0x2000, ..., 0x7F000
 *             - 必须是 4096 的倍数！
 *             - 地址会自动对齐到扇区边界
 *
 * @note 【大白话】"把这 4KB 的地盘清理干净，好让我写入新数据"
 *
 * @note 【地址对齐】传入的地址会自动向下对齐到扇区起始地址
 *        例如: 传入 0x1234，会自动擦除 0x1000 扇区
 *
 * 【使用示例】
 * @code
 *   // 擦除地址 0x1000 所在的扇区
 *   W25Q64_erase_sector(0x1000);
 *
 *   // 擦除多个扇区
 *   for (uint32_t i = 0; i < 5; i++) {
 *       printf("正在擦除扇区 %d...\r\n", i);
 *       W25Q64_erase_sector(i * W25Q64_SECTOR_SIZE);
 *       printf("扇区 %d 擦除完成\r\n", i);
 *   }
 *
 *   // 典型用法: 写入前先擦除
 *   uint16_t writeData[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *   W25Q64_erase_sector(0x1000);  // 先擦除
 *   W25Q64_write16(writeData, 0x1000, 10);  // 后写入
 * @endcode
 */
void W25Q64_erase_sector(uint32_t addr);


/**
 * ============================================================================
 * @brief W25Q64_BlockErase - 块擦除 (64KB)
 * ============================================================================
 *
 * 【功能说明】
 *   擦除指定的 64KB 块，将 64KB 区域内的所有字节设置为 0xFF
 *
 * 【对比扇区擦除】
 *   | 特性         | 扇区擦除         | 块擦除          |
 *   |---------- --|-----------------|-----------------|
 *   | 擦除大小     | 4KB             | 64KB            |
 *   | 擦除时间     | ~45ms (典型)     | ~200ms (典型)   |
 *   | 适用场景     | 精确控制，节省     | 批量清除，大    |
 *   |            | 不需要的数据       | 数据区域        |
 *
 * ⚠️ 注意: 块擦除会一次清除 16 个扇区，操作前请确保数据已备份！
 *
 * @param addr 块起始地址
 *             - 有效值: 0x00000, 0x10000, 0x20000, ..., 0x780000
 *             - 必须是 65536 的倍数！
 *
 * @note 【大白话】"一次性把 64KB (16个扇区) 的地盘全部清理"
 *
 * 【使用示例】
 * @code
 *   // 擦除第 0 块 (地址 0x00000 ~ 0x0FFFF)
 *   W25Q64_BlockErase(0x00000);
 *
 *   // 擦除第 1 块 (地址 0x10000 ~ 0x1FFFF)
 *   W25Q64_BlockErase(0x10000);
 *
 *   // 擦除整个芯片 (8MB = 128 个块)
 *   for (uint32_t block = 0; block < 128; block++) {
 *       printf("正在擦除块 %lu/%d...\r\n", block + 1, 128);
 *       W25Q64_BlockErase(block * W25Q64_BLOCK_SIZE);
 *   }
 *   printf("全片擦除完成!\r\n");
 *
 *   // 典型用法: 整块数据更新前擦除
 *   uint16_t bigData[10000];  // 20KB 数据
 *   W25Q64_BlockErase(0x20000);  // 擦除 64KB 空间
 *   W25Q64_write16(bigData, 0x20000, 10000);  // 写入 20KB
 * @endcode
 */
void W25Q64_BlockErase(uint32_t addr);


/**
 * ============================================================================
 * @brief W25Q64_read16 - 启动 DMA 非阻塞读取 (16-bit 数据)
 * ============================================================================
 *
 * 【功能说明】
 *   启动 DMA 读取操作，将数据从 Flash 读取到内存缓冲区
 *
 * 【核心特性】
 *   ⚠️ 此函数【只发起读取】，不会等待读取完成！
 *   ⚠️ 读取过程中需要【反复调用 W25Q64_Process()】来推进 DMA
 *
 * 【参数说明】
 *   @param buf   - 数据缓冲区指针，类型是 uint16_t*
 *                  读取的数据会存储到这个缓冲区
 *   @param addr  - Flash 起始地址 (有效范围: 0x000000 ~ 0x7FFFFF)
 *   @param size  - uint16_t 的数量，不是字节数！
 *                  实际读取字节数 = size * 2
 *
 * @note 【大白话】"开始读了，数据会自动送到 buf 里，读完通知我"
 *
 * @note 【16-bit 说明】buf 是 uint16_t* 类型，但 W25Q64 是字节存储，
 *        所以 size=100 表示读取 200 字节，存为 100 个 uint16_t
 *
 * 【使用示例】
 * @code
 *   // 在全局或函数中定义缓冲区
 *   uint16_t readBuffer[500];  // 1000 字节
 *
 *   // 发起读取
 *   W25Q64_read16(readBuffer, 0x1000, 500);
 *
 *   // 在主循环中轮询完成
 *   while (1) {
 *       W25Q64_NB_State state = W25Q64_Process();
 *       if (state == W25Q64_NB_DONE) {
 *           printf("读取完成!\r\n");
 *           // 数据已经在 readBuffer 中了
 *           for (int i = 0; i < 10; i++) {
 *               printf("%04X ", readBuffer[i]);
 *           }
 *           break;
 *       }
 *       if (state == W25Q64_NB_ERROR) {
 *           printf("读取失败!\r\n");
 *           break;
 *       }
 *       // 可以在这里处理其他任务
 *   }
 * @endcode
 *
 * 【进阶用法: 带超时的轮询】
 * @code
 *   W25Q64_read16(readBuffer, 0x1000, 500);
 *
 *   uint32_t timeout = 1000000;  // 超时计数
 *   while (timeout--) {
 *       if (W25Q64_Process() == W25Q64_NB_DONE) {
 *           break;
 *       }
 *   }
 *   if (timeout == 0) {
 *       printf("读取超时!\r\n");
 *   }
 * @endcode
 */
void W25Q64_read16(uint16_t *buf, uint32_t addr, uint32_t size);


/**
 * ============================================================================
 * @brief W25Q64_write16 - 启动 DMA 非阻塞写入 (16-bit 数据)
 * ============================================================================
 *
 * 【功能说明】
 *   启动 DMA 写入操作，将数据从内存缓冲区写入到 Flash
 *
 * 【核心特性】
 *   ⚠️ 此函数【只发起写入】，不会等待写入完成！
 *   ⚠️ 写入过程中需要【反复调用 W25Q64_Process()】来推进 DMA
 *   ⚠️ 写入前【必须先擦除】对应区域，否则数据会出错
 *
 * 【页边界处理】
 *   W25Q64 的页大小是 256 字节，超过一页会自动分页
 *   例如: 从地址 100 写入 300 字节，会自动分成两次页写入
 *
 * 【参数说明】
 *   @param buf   - 数据缓冲区指针，类型是 uint16_t*
 *                  这里的 uint16_t 数据会被拆成字节写入 Flash
 *   @param addr  - Flash 起始地址 (有效范围: 0x000000 ~ 0x7FFFFF)
 *   @param size  - uint16_t 的数量，不是字节数！
 *                  实际写入字节数 = size * 2
 *
 * @note 【大白话】"开始写了，buf 里的数据会送到 Flash 里，写完通知我"
 *
 * @note 【写入前必读】Flash 写入前必须先擦除！
 *        未擦除直接写入会导致数据出错（只能 1→0，不能 0→1）
 *
 * 【使用示例】
 * @code
 *   // 在全局或函数中定义数据
 *   uint16_t writeData[500] = {0};
 *   for (int i = 0; i < 500; i++) {
 *       writeData[i] = i;  // 填充测试数据
 *   }
 *
 *   // 【重要】写入前必须先擦除！
 *   W25Q64_erase_sector(0x1000);
 *
 *   // 发起写入
 *   W25Q64_write16(writeData, 0x1000, 500);
 *
 *   // 在主循环中轮询完成
 *   while (1) {
 *       W25Q64_NB_State state = W25Q64_Process();
 *       if (state == W25Q64_NB_DONE) {
 *           printf("写入完成!\r\n");
 *           break;
 *       }
 *       if (state == W25Q64_NB_ERROR) {
 *           printf("写入失败!\r\n");
 *           break;
 *       }
 *   }
 * @endcode
 *
 * 【常见错误示例】
 * @code
 *   // ❌ 错误: 直接写入而不擦除
 *   uint16_t data[10] = {1,2,3,4,5,6,7,8,9,10};
 *   W25Q64_write16(data, 0x1000, 10);  // 没有先擦除！
 *   // 结果: 数据可能不正确
 *
 *   // ✅ 正确: 先擦除后写入
 *   W25Q64_erase_sector(0x1000);  // 先擦除
 *   W25Q64_write16(data, 0x1000, 10);  // 再写入
 * @endcode
 */
void W25Q64_write16(uint16_t *buf, uint32_t addr, uint32_t size);


/**
 * ============================================================================
 * @brief W25Q64_Process - 推进 DMA 读写状态机
 * ============================================================================
 *
 * 【功能说明】
 *   推进 DMA 读写操作的状态机，检查并完成当前的 DMA 传输
 *
 * 【核心特性】
 *   ⚠️ 必须【在主循环中反复调用】此函数，直到返回 DONE 或 ERROR
 *   ⚠️ 调用频率建议: 每 1~10ms 调用一次
 *
 * 【使用场景】
 *   - 每次调用 W25Q64_read16() 或 W25Q64_write16() 后
 *   - 需要在主循环或定时器中断中调用
 *
 * @param (无)
 *
 * @return W25Q64_NB_State - 当前操作状态
 *         - W25Q64_NB_IDLE  : 空闲
 *         - W25Q64_NB_BUSY  : 正在传输中
 *         - W25Q64_NB_DONE  : 传输完成
 *         - W25Q64_NB_ERROR : 传输错误
 *
 * @note 【大白话】"Flash 正在干活，我来问问进度"
 *
 * @note 【推荐调用方式】
 *       建议在主循环中调用，或在 SysTick/Timer 中调用
 *       不要在中断中调用（除非中断频率足够高）
 *
 * 【使用示例】
 * @code
 *   uint16_t buffer[100];
 *
 *   // 发起读取
 *   W25Q64_read16(buffer, 0x1000, 100);
 *
 *   int main(void)
 *   {
 *       W25Q64_InitConfig();
 *
 *       while (1) {
 *           // 处理 Flash 操作
 *           W25Q64_Process();
 *
 *           // 处理其他任务
 *           // delay_ms(1);
 *       }
 *   }
 * @endcode
 *
 * 【完整读写示例】
 * @code
 *   void save_config(void)
 *   {
 *       uint16_t configData[64];
 *
 *       // 准备要写入的数据
 *       for (int i = 0; i < 64; i++) {
 *           configData[i] = i * 10;
 *       }
 *
 *       // 擦除扇区
 *       W25Q64_erase_sector(0x0000);
 *
 *       // 等待擦除完成（erase_sector 是阻塞的）
 *
 *       // 写入数据
 *       W25Q64_write16(configData, 0x0000, 64);
 *
 *       // 等待写入完成
 *       while (W25Q64_Process() != W25Q64_NB_DONE);
 *   }
 *
 *   void load_config(void)
 *   {
 *       uint16_t configData[64];
 *
 *       // 读取数据
 *       W25Q64_read16(configData, 0x0000, 64);
 *
 *       // 等待读取完成
 *       while (W25Q64_Process() != W25Q64_NB_DONE);
 *
 *       // 数据已存储在 configData 中
 *       for (int i = 0; i < 64; i++) {
 *           printf("config[%d] = %d\r\n", i, configData[i]);
 *       }
 *   }
 * @endcode
 */
W25Q64_NB_State W25Q64_Process(void);


/* ============================================================================
 * 【总结】 - 快速使用指南
 * ============================================================================
 *
 * 【初始化】
 *   W25Q64_InitConfig();  // 系统启动时调用一次
 *
 * 【读取数据】
 *   小数据 (1~几字节): W25Q64_ReadByte(addr)
 *   大数据 (多字节):   W25Q64_read16(buf, addr, size) + W25Q64_Process()
 *
 * 【写入数据】
 *   1. W25Q64_erase_sector(addr)  // 先擦除 (或 W25Q64_BlockErase)
 *   2. W25Q64_write16(buf, addr, size)  // 后写入
 *   3. W25Q64_Process()  // 等待完成
 *
 * 【检查 Flash】
 *   W25Q64_GetID()       // 读 ID，确认芯片正常
 *   W25Q64_ReadState()   // 读状态，检查 BUSY/WEL
 *
 * ============================================================================
 

#endif  _W25Q64_H_ */
