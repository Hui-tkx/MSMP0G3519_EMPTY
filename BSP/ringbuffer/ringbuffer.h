/**
 * @file ringbuffer.h
 * @brief 通用环形缓冲区（Ring Buffer / Circular Buffer）库
 *
 * ============================================================================
 * 简介
 * ============================================================================
 * 一个通用的环形缓冲区实现，适用于各种数据类型的缓冲场景。
 * 采用固定大小数组实现，避免动态内存分配，支持中断安全操作。
 *
 * ============================================================================
 * 特性
 * ============================================================================
 * - 通用设计：可用于任何数据类型（字节、结构体等）
 * - 零拷贝：数据直接在缓冲区中操作
 * - 线程安全：支持禁用中断的原子操作
 * - O(1)复杂度：读写操作都是常数时间
 * - 两种模式：覆盖模式（数据被覆盖）和丢弃模式（新数据被丢弃）
 *
 * ============================================================================
 * 工作原理
 * ============================================================================
 *
 * 环形缓冲区结构:
 *   ┌────────────────────────────────────────────────────┐
 *   │  [pos0] [pos1] [pos2] [pos3] ... [posN-1]          │
 *   │                                              ↑     │
 *   │                                              │     │
 *   └────────────────────────────────────────────────────┘
 *     ↑                                       ↑
 *     write_pos (写入位置)           read_pos (读取位置)
 *
 *   - write_pos: 下一个数据写入的位置
 *   - read_pos: 下一个要读取的数据位置
 *   - 当 write_pos == read_pos 时，缓冲区为空
 *   - 当 (write_pos + 1) % size == read_pos 时，缓冲区为满
 *
 * ============================================================================
 * 使用示例
 * ============================================================================
 *
 * 示例 1: 基本的字节缓冲
 * @code
 * uint8_t buffer[256];
 * RingBuffer rb;
 *
 * RingBuffer_Init(&rb, buffer, sizeof(buffer));
 *
 * // 写入数据
 * uint8_t byte = 'A';
 * RingBuffer_Write(&rb, &byte, 1);
 *
 * // 读取数据
 * uint8_t read_byte;
 * RingBuffer_Read(&rb, &read_byte, 1);
 *
 * // 检查状态
 * uint32_t avail = RingBuffer_Available(&rb);
 * uint32_t free = RingBuffer_FreeSpace(&rb);
 * @endcode
 *
 * 示例 2: 批量写入/读取
 * @code
 * uint8_t tx_data[] = "Hello, World!";
 * uint8_t rx_data[32];
 *
 * // 写入字符串
 * RingBuffer_Write(&rb, tx_data, strlen((char*)tx_data));
 *
 * // 读取5个字节
 * uint32_t len = RingBuffer_Read(&rb, rx_data, 5);
 * @endcode
 *
 * 示例 3: 检查是否包含特定数据
 * @code
 * if (RingBuffer_Find(&rb, '\n') >= 0) {
 *     // 找到换行符，处理一行数据
 * }
 * @endcode
 *
 * 示例 4: 溢出处理
 * @code
 * // 设置为覆盖模式（默认）
 * rb.overflow_mode = RINGBUFFER_OVERWRITE;
 *
 * // 或设置为丢弃模式
 * rb.overflow_mode = RINGBUFFER_DROP;
 * @endcode
 *
 * ============================================================================
 * API 函数列表
 * ============================================================================
 * | 函数名                  | 功能                         |
 * |------------------------|------------------------------|
 * | RingBuffer_Init        | 初始化环形缓冲区              |
 * | RingBuffer_Clear      | 清空缓冲区                   |
 * | RingBuffer_Write      | 写入数据（自动扩展）         |
 * | RingBuffer_Read       | 读取数据（自动前进）         |
 * | RingBuffer_Peek       | 窥视数据（不前进）           |
 * | RingBuffer_Skip       | 跳过数据                     |
 * | RingBuffer_Available  | 获取可用数据大小             |
 * | RingBuffer_FreeSpace  | 获取剩余空间                 |
 * | RingBuffer_IsEmpty    | 检查是否为空                 |
 * | RingBuffer_IsFull     | 检查是否已满                 |
 * | RingBuffer_Find       | 查找特定字节的位置           |
 */

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief 环形缓冲区溢出模式
 *
 * - OVERWRITE: 覆盖模式 - 新数据覆盖旧数据
 * - DROP:      丢弃模式 - 新数据被丢弃
 */
typedef enum {
    RINGBUFFER_OVERWRITE = 0,
    RINGBUFFER_DROP = 1
} RingBuffer_OverflowMode;

/**
 * @brief 环形缓冲区结构体
 *
 * 用户需要提供一个固定大小的缓冲区数组。
 *
 * 【为什么要这样设计？】
 *   - 避免动态内存分配（嵌入式友好）
 *   - 用户控制内存分配
 *   - 可在栈、堆、全局区分配
 */
typedef struct {
    uint8_t *buffer;           /**< 缓冲区数组指针 */
    uint32_t size;             /**< 缓冲区总大小（字节） */
    volatile uint32_t write_pos; /**< 写入位置（下一个写入的位置） */
    volatile uint32_t read_pos;  /**< 读取位置（下一个读取的位置） */
    RingBuffer_OverflowMode overflow_mode; /**< 溢出模式 */
} RingBuffer;

/**
 * ============================================================================
 * 基础操作函数
 * ============================================================================
 */

/**
 * @brief 初始化环形缓冲区
 *
 * @param rb     环形缓冲区结构体指针
 * @param buffer 用户提供的缓冲区数组
 * @param size   缓冲区大小（字节）
 *
 * @note size 建议为 2 的幂次方，便于优化
 */
void RingBuffer_Init(RingBuffer *rb, uint8_t *buffer, uint32_t size);

/**
 * @brief 清空环形缓冲区
 *
 * @param rb 环形缓冲区结构体指针
 */
void RingBuffer_Clear(RingBuffer *rb);

/**
 * @brief 重置读写位置（等同于 Clear）
 */
void RingBuffer_Reset(RingBuffer *rb);

/**
 * ============================================================================
 * 数据读写函数
 * ============================================================================
 */

/**
 * @brief 写入数据到缓冲区
 *
 * @param rb     环形缓冲区结构体指针
 * @param data   要写入的数据
 * @param length 数据长度（字节）
 *
 * @return uint32_t 实际写入的字节数
 *
 * @note 在覆盖模式下，如果空间不足，会覆盖最旧的数据
 *       在丢弃模式下，如果空间不足，只会写入能容纳的部分
 */
uint32_t RingBuffer_Write(RingBuffer *rb, const uint8_t *data, uint32_t length);

/**
 * @brief 从缓冲区读取数据
 *
 * @param rb     环形缓冲区结构体指针
 * @param data   读取到的数据存放位置
 * @param length 要读取的长度
 *
 * @return uint32_t 实际读取的字节数
 *
 * @note 读取后数据仍然保留在缓冲区中（直到被覆盖）
 *       如果需要真正删除数据，使用 RingBuffer_Skip()
 */
uint32_t RingBuffer_Read(RingBuffer *rb, uint8_t *data, uint32_t length);

/**
 * @brief 窥视缓冲区（读取但不移动读指针）
 *
 * @param rb     环形缓冲区结构体指针
 * @param data   读取到的数据存放位置
 * @param length 要读取的长度
 *
 * @return uint32_t 实际读取的字节数
 */
uint32_t RingBuffer_Peek(const RingBuffer *rb, uint8_t *data, uint32_t length);

/**
 * @brief 跳过（丢弃）数据
 *
 * @param rb     环形缓冲区结构体指针
 * @param length 要跳过的长度
 *
 * @return uint32_t 实际跳过的字节数
 */
uint32_t RingBuffer_Skip(RingBuffer *rb, uint32_t length);

/**
 * @brief 写入单个字节
 *
 * @param rb    环形缓冲区结构体指针
 * @param byte  要写入的字节
 *
 * @return bool 是否成功写入
 */
bool RingBuffer_WriteByte(RingBuffer *rb, uint8_t byte);

/**
 * @brief 读取单个字节
 *
 * @param rb    环形缓冲区结构体指针
 * @param byte  读取到的字节存放位置
 *
 * @return bool 是否成功读取
 */
bool RingBuffer_ReadByte(RingBuffer *rb, uint8_t *byte);

/**
 * @brief 查看单个字节（不移动读指针）
 *
 * @param rb    环形缓冲区结构体指针
 * @param byte  读取到的字节存放位置
 *
 * @return bool 是否成功读取
 */
bool RingBuffer_PeekByte(const RingBuffer *rb, uint8_t *byte);

/**
 * ============================================================================
 * 状态查询函数
 * ============================================================================
 */

/**
 * @brief 获取可用数据大小
 *
 * @param rb 环形缓冲区结构体指针
 *
 * @return uint32_t 可读的字节数
 */
uint32_t RingBuffer_Available(const RingBuffer *rb);

/**
 * @brief 获取剩余空间
 *
 * @param rb 环形缓冲区结构体指针
 *
 * @return uint32_t 可写入的字节数
 */
uint32_t RingBuffer_FreeSpace(const RingBuffer *rb);

/**
 * @brief 检查缓冲区是否为空
 *
 * @param rb 环形缓冲区结构体指针
 *
 * @return bool true=空，false=非空
 */
bool RingBuffer_IsEmpty(const RingBuffer *rb);

/**
 * @brief 检查缓冲区是否已满
 *
 * @param rb 环形缓冲区结构体指针
 *
 * @return bool true=满，false=未满
 */
bool RingBuffer_IsFull(const RingBuffer *rb);

/**
 * @brief 查找特定字节的位置
 *
 * @param rb    环形缓冲区结构体指针
 * @param byte  要查找的字节
 *
 * @return int  位置（相对于读指针），-1=未找到
 *
 * @note 从读指针位置开始查找，不会移动读指针
 */
int RingBuffer_Find(const RingBuffer *rb, uint8_t byte);

/**
 * @brief 查找字符串（以特定字节结尾）
 *
 * @param rb       环形缓冲区结构体指针
 * @param terminator 结束符
 * @param data     读取到的数据存放位置
 * @param max_len  最大读取长度
 *
 * @return uint32_t 实际读取的字节数（不包括结束符）
 *
 * @note 找到结束符时停止读取，并将结束符前的数据复制出来
 */
uint32_t RingBuffer_ReadUntil(RingBuffer *rb, uint8_t terminator, uint8_t *data, uint32_t max_len);

/**
 * ============================================================================
 * 高级操作函数
 * ============================================================================
 */

/**
 * @brief 设置溢出模式
 *
 * @param rb    环形缓冲区结构体指针
 * @param mode  溢出模式
 */
void RingBuffer_SetOverflowMode(RingBuffer *rb, RingBuffer_OverflowMode mode);

/**
 * @brief 获取缓冲区总大小
 *
 * @param rb 环形缓冲区结构体指针
 *
 * @return uint32_t 缓冲区总大小
 */
uint32_t RingBuffer_GetSize(const RingBuffer *rb);

/**
 * @brief 获取已使用的数据大小
 *
 * @param rb 环形缓冲区结构体指针
 *
 * @return uint32_t 已使用的大小
 */
uint32_t RingBuffer_UsedSpace(const RingBuffer *rb);

/**
 * @brief 压缩缓冲区（将数据移到开头）
 *
 * @param rb 环形缓冲区结构体指针
 *
 * @note 用于需要连续内存的场景
 */
void RingBuffer_Compact(RingBuffer *rb);

#endif
