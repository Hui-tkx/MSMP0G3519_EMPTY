/**
 * @file uart_ringbuffer.h
 * @brief UART RingBuffer - UART 专用环形缓冲区封装
 *
 * ============================================================================
 * 简介
 * ============================================================================
 * 基于 RingBuffer 库的 UART 接收缓冲区封装，专门用于 UART 异步接收场景。
 * 集成到 UART 中断中，自动接收数据，用户程序可以随时读取。
 *
 * ============================================================================
 * 为什么需要这个库？
 * ============================================================================
 * UART 接收数据的特点：
 * - 上位机随时可能发数据
 * - CPU 不可能一直在等待接收
 * - 没有缓冲区，数据就会丢失
 *
 * 使用环形缓冲区后：
 * - 数据到达时自动存入缓冲区
 * - CPU 空闲时从缓冲区慢慢取
 * - 再也不会丢数据
 *
 * ============================================================================
 * 使用示例
 * ============================================================================
 *
 * 示例 1: 基本使用
 * @code
 * // 定义缓冲区（在全局区）
 * static uint8_t g_uart_rx_buf[256];
 * UART_RingBuffer uart_rb;
 *
 * // 初始化
 * UART_RB_Init(&uart_rb, g_uart_rx_buf, sizeof(g_uart_rx_buf));
 *
 * // 在 UART RX 中断中调用
 * void UART0_IRQHandler(void) {
 *     if (UART0->STAT & RX_INTERRUPT_FLAG) {
 *         uint8_t data = UART_receiveData(UART0);
 *         UART_RB_WriteByte(&uart_rb, data);
 *     }
 * }
 *
 * // 主循环中读取
 * while (1) {
 *     if (UART_RB_Available(&uart_rb) > 0) {
 *         uint8_t byte;
 *         UART_RB_ReadByte(&uart_rb, &byte);
 *         // 处理 byte
 *     }
 * }
 * @endcode
 *
 * 示例 2: 读取一行（以换行符结束）
 * @code
 * void processCommands(void) {
 *     char line[64];
 *     uint32_t len = UART_RB_ReadLine(&uart_rb, line, sizeof(line));
 *     if (len > 0) {
 *         UART_RB_SendString(UART0, "Received: ");
 *         UART_RB_SendData(UART0, (uint8_t*)line, len);
 *         UART_RB_SendString(UART0, "\r\n");
 *     }
 * }
 * @endcode
 *
 * 示例 3: 回显功能
 * @code
 * void echoCallback(void) {
 *     while (UART_RB_Available(&uart_rb) > 0) {
 *         uint8_t byte;
 *         UART_RB_ReadByte(&uart_rb, &byte);
 *         UART_sendByte(UART0, byte);
 *     }
 * }
 * @endcode
 *
 * 示例 4: 处理二进制协议
 * @code
 * void processProtocol(void) {
 *     // 协议格式: 0xAA LEN DATA... CHECKSUM
 *     while (UART_RB_Available(&uart_rb) >= 4) {
 *         int head = UART_RB_FindByte(&uart_rb, 0xAA);
 *         if (head > 0) {
 *             UART_RB_Skip(&uart_rb, head);
 *         }
 *         uint8_t first;
 *         if (UART_RB_PeekByte(&uart_rb, &first) && first == 0xAA) {
 *             uint8_t len;
 *             if (UART_RB_PeekAt(&uart_rb, 1, &len)) {
 *                 if (UART_RB_Available(&uart_rb) >= (uint32_t)len + 3) {
 *                     uint8_t packet[256];
 *                     UART_RB_Skip(&uart_rb, 2);
 *                     UART_RB_Read(&uart_rb, packet, len + 1);
 *                 }
 *             }
 *         }
 *     }
 * }
 * @endcode
 *
 * ============================================================================
 * API 函数列表
 * ============================================================================
 * | 函数名              | 功能                         |
 * |--------------------|------------------------------|
 * | UART_RB_Init       | 初始化 UART 环形缓冲区        |
 * | UART_RB_Clear      | 清空缓冲区                   |
 * | UART_RB_WriteByte  | 写入单个字节（中断中调用）    |
 * | UART_RB_ReadByte   | 读取单个字节                 |
 * | UART_RB_Read       | 读取多个字节                 |
 * | UART_RB_PeekByte   | 查看单个字节（不删除）        |
 * | UART_RB_Peek       | 查看多个字节（不删除）        |
 * | UART_RB_Skip       | 跳过数据                     |
 * | UART_RB_Available  | 获取可用数据大小             |
 * | UART_RB_FindByte   | 查找特定字节的位置           |
 * | UART_RB_ReadLine   | 读取一行（以换行符结束）     |
 * | UART_RB_SendData   | 发送缓冲区数据               |
 * | UART_RB_SendString | 发送字符串                   |
 * | UART_RB_IsFull     | 检查缓冲区是否已满           |
 * | UART_RB_IsOverflow | 检查是否发生溢出             |
 * | UART_RB_GetOverflowCount | 获取溢出次数        |
 */

#ifndef _UART_RINGBUFFER_H_
#define _UART_RINGBUFFER_H_

#include "ringbuffer.h"
#include "uart.h"

/**
 * @brief UART 环形缓冲区结构体
 *
 * 封装了 RingBuffer 并添加了 UART 相关功能。
 */
typedef struct {
    RingBuffer rb;                      /**< 底层环形缓冲区 */
    volatile bool overflow_flag;        /**< 溢出标志 */
    volatile uint32_t overflow_count;   /**< 溢出次数计数 */
} UART_RingBuffer;

/**
 * ============================================================================
 * 初始化与配置
 * ============================================================================
 */

/**
 * @brief 初始化 UART 环形缓冲区
 *
 * @param rb     UART_RingBuffer 结构体指针
 * @param buffer 用户提供的缓冲区数组
 * @param size   缓冲区大小
 */
void UART_RB_Init(UART_RingBuffer *rb, uint8_t *buffer, uint32_t size);

/**
 * @brief 清空缓冲区
 */
void UART_RB_Clear(UART_RingBuffer *rb);

/**
 * ============================================================================
 * 数据写入（通常在中断中调用）
 * ============================================================================
 */

/**
 * @brief 写入单个字节
 *
 * @param rb    UART_RingBuffer 结构体指针
 * @param byte 要写入的字节
 *
 * @return bool 是否成功写入（缓冲区满时返回 false）
 */
bool UART_RB_WriteByte(UART_RingBuffer *rb, uint8_t byte);

/**
 * @brief 写入多个字节
 *
 * @param rb     UART_RingBuffer 结构体指针
 * @param data   要写入的数据
 * @param length 长度
 *
 * @return uint32_t 实际写入的字节数
 */
uint32_t UART_RB_Write(UART_RingBuffer *rb, const uint8_t *data, uint32_t length);

/**
 * ============================================================================
 * 数据读取（通常在主循环中调用）
 * ============================================================================
 */

/**
 * @brief 读取单个字节
 *
 * @param rb    UART_RingBuffer 结构体指针
 * @param byte 读取到的字节存放位置
 *
 * @return bool 是否成功读取
 */
bool UART_RB_ReadByte(UART_RingBuffer *rb, uint8_t *byte);

/**
 * @brief 读取多个字节
 *
 * @param rb     UART_RingBuffer 结构体指针
 * @param data   读取到的数据存放位置
 * @param length 要读取的长度
 *
 * @return uint32_t 实际读取的字节数
 */
uint32_t UART_RB_Read(UART_RingBuffer *rb, uint8_t *data, uint32_t length);

/**
 * ============================================================================
 * 数据查看（不删除数据）
 * ============================================================================
 */

/**
 * @brief 查看单个字节（不移动读指针）
 *
 * @param rb    UART_RingBuffer 结构体指针
 * @param byte 读取到的字节存放位置
 *
 * @return bool 是否成功读取
 */
bool UART_RB_PeekByte(UART_RingBuffer *rb, uint8_t *byte);

/**
 * @brief 查看多个字节（不移动读指针）
 *
 * @param rb     UART_RingBuffer 结构体指针
 * @param data   读取到的数据存放位置
 * @param length 要读取的长度
 *
 * @return uint32_t 实际读取的字节数
 */
uint32_t UART_RB_Peek(UART_RingBuffer *rb, uint8_t *data, uint32_t length);

/**
 * @brief 查看指定位置的字节
 *
 * @param rb     UART_RingBuffer 结构体指针
 * @param offset 相对于读指针的偏移
 * @param byte   读取到的字节存放位置
 *
 * @return bool 是否成功读取
 */
bool UART_RB_PeekAt(UART_RingBuffer *rb, uint32_t offset, uint8_t *byte);

/**
 * ============================================================================
 * 数据跳过
 * ============================================================================
 */

/**
 * @brief 跳过（丢弃）数据
 *
 * @param rb     UART_RingBuffer 结构体指针
 * @param length 要跳过的长度
 *
 * @return uint32_t 实际跳过的字节数
 */
uint32_t UART_RB_Skip(UART_RingBuffer *rb, uint32_t length);

/**
 * ============================================================================
 * 状态查询
 * ============================================================================
 */

/**
 * @brief 获取可用数据大小
 */
uint32_t UART_RB_Available(UART_RingBuffer *rb);

/**
 * @brief 获取剩余空间
 */
uint32_t UART_RB_FreeSpace(UART_RingBuffer *rb);

/**
 * @brief 检查是否为空
 */
bool UART_RB_IsEmpty(UART_RingBuffer *rb);

/**
 * @brief 检查是否已满
 */
bool UART_RB_IsFull(UART_RingBuffer *rb);

/**
 * @brief 查找特定字节
 *
 * @param rb    UART_RingBuffer 结构体指针
 * @param byte 要查找的字节
 *
 * @return int 位置（相对于读指针），-1=未找到
 */
int UART_RB_FindByte(UART_RingBuffer *rb, uint8_t byte);

/**
 * @brief 检查是否发生溢出
 */
bool UART_RB_IsOverflow(UART_RingBuffer *rb);

/**
 * @brief 获取溢出次数
 */
uint32_t UART_RB_GetOverflowCount(UART_RingBuffer *rb);

/**
 * @brief 清除溢出标志
 */
void UART_RB_ClearOverflow(UART_RingBuffer *rb);

/**
 * ============================================================================
 * 高级操作
 * ============================================================================
 */

/**
 * @brief 读取一行（以换行符或回车符结束）
 *
 * @param rb       UART_RingBuffer 结构体指针
 * @param buffer   读取到的数据存放位置
 * @param max_len  最大读取长度
 *
 * @return uint32_t 实际读取的字节数（不包括结束符）
 */
uint32_t UART_RB_ReadLine(UART_RingBuffer *rb, char *buffer, uint32_t max_len);

/**
 * @brief 通过 UART 发送缓冲区数据
 *
 * @param uart   UART 实例
 * @param rb     UART_RingBuffer 结构体指针
 * @param length 要发送的长度（0=发送所有可用数据）
 *
 * @return uint32_t 实际发送的字节数
 */
uint32_t UART_RB_SendData(UART_Regs *uart, UART_RingBuffer *rb, uint32_t length);

/**
 * @brief 通过 UART 发送字符串
 *
 * @param uart UART 实例
 * @param str  字符串
 *
 * @return uint32_t 发送的字节数
 */
uint32_t UART_RB_SendString(UART_Regs *uart, const char *str);

#endif
