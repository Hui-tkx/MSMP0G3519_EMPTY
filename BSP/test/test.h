/**
 * @file test.h
 * @brief 板级测试库 — 统一测试入口
 *
 * 所有外设测试通过此库统一调用，方便在 main 中快速验证硬件。
 *
 * 使用示例:
 * @code
 * #include "bsp.h"
 *
 * int main(void) {
 *     SYSCFG_DL_init();
 *     UART_init(UART_0_INST);
 *     ADC_init();
 *     DAC_init();
 *
 *     TEST_all(UART_0_INST);       // 跑全部测试
 *     // 或单项测试:
 *     // TEST_adc(UART_0_INST);
 *     // TEST_dac(UART_0_INST);
 *     // TEST_temperature(UART_0_INST);
 *
 *     while (1) { }
 * }
 * @endcode
 *
 * API 列表:
 * | 函数                | 功能                   |
 * |-------------------|------------------------|
 * | TEST_all          | 运行所有测试           |
 * | TEST_adc          | 运行 ADC 测试          |
 * | TEST_dac          | 运行 DAC 测试          |
 * | TEST_uart         | 运行 UART 测试         |
 * | TEST_temperature  | 运行温度传感器测试     |
 * | TEST_ringbuffer   | 运行 RingBuffer 测试   |
 * | TEST_uart_rb      | 运行 UART RingBuffer 测试 |
 */

#ifndef _TEST_H_
#define _TEST_H_

#include "ti_msp_dl_config.h"
#include "uart.h"

/* === 统一测试入口 === */
void TEST_all(UART_Regs *uart);             /*!< 运行所有测试 */
void TEST_adc(UART_Regs *uart);             /*!< 运行 ADC 测试 */
void TEST_dac(UART_Regs *uart);             /*!< 运行 DAC 测试 */
void TEST_uart(UART_Regs *uart);            /*!< 运行 UART 测试 */
void TEST_temperature(UART_Regs *uart);     /*!< 运行温度传感器测试 */
void TEST_ringbuffer(UART_Regs *uart);      /*!< 运行 RingBuffer 测试 */
void TEST_uart_rb(UART_Regs *uart);         /*!< 运行 UART RingBuffer 测试 */

#endif
