/**
 * @file test.h
 * @brief 板级测试库
 *
 * ============================================================================
 * 简介
 * ============================================================================
 * 集成所有外设库的测试功能，方便快速验证硬件是否正常工作。
 * 测试结果通过 UART 串口输出。
 *
 * ============================================================================
 * 测试项目
 * ============================================================================
 * - ADC 测试 (模数转换)
 * - DAC 测试 (数模转换)
 * - UART 测试 (串口通信)
 * - RingBuffer 测试 (通用环形缓冲区)
 * - UART RingBuffer 测试 (UART专用环形缓冲区)
 *
 * ============================================================================
 * 使用方法
 * ============================================================================
 * @code
 * #include "bsp.h"
 *
 * int main(void)
 * {
 *     SYSCFG_DL_init();
 *     UART_init(UART_0_INST);
 *     ADC_init();
 *     DAC_init();
 *
 *     // 运行所有测试
 *     TEST_runAll(UART_0_INST);
 *
 *     while (1) { }
 * }
 * @endcode
 *
 * ============================================================================
 * API 函数列表
 * ============================================================================
 * | 函数名          | 功能                     |
 * |----------------|--------------------------|
 * | TEST_runAll    | 运行所有测试             |
 * | TEST_runADC    | 运行 ADC 测试            |
 * | TEST_runDAC    | 运行 DAC 测试            |
 * | TEST_runUART   | 运行 UART 测试          |
 * | TEST_runRingBuffer | 运行 RingBuffer 测试 |
 */

#ifndef _TEST_H_
#define _TEST_H_

#include "ti_msp_dl_config.h"
#include "bsp.h"
/**
 * @brief 运行所有测试
 *
 * @param uart UART 实例指针，用于输出测试结果
 */
void TEST_runAll(UART_Regs *uart);

/**
 * @brief 运行 ADC 测试
 */
void TEST_runADC(UART_Regs *uart);

/**
 * @brief 运行 DAC 测试
 */
void TEST_runDAC(UART_Regs *uart);

/**
 * @brief 运行 UART 测试
 */
void TEST_runUART(UART_Regs *uart);

/**
 * @brief 运行 RingBuffer 测试
 */
void TEST_runRingBuffer(UART_Regs *uart);

/**
 * @brief 运行 UART RingBuffer 测试
 */
void TEST_runUARTRingBuffer(UART_Regs *uart);

#endif
