/**
 * @file dac_test.h
 * @brief DAC 库测试接口 (基础 API + 波形生成)
 */
#ifndef _DAC_TEST_H_
#define _DAC_TEST_H_

#include "dac.h"
#include "uart.h"

#define DAC_TEST_SEPARATOR "=============================================="  /*!< 分隔线 */

void DAC_TEST_runAll(UART_Regs *uart);          /*!< 运行全部测试 */
void DAC_TEST_runBasicTests(UART_Regs *uart);   /*!< 基础 API 测试 */
void DAC_TEST_runWaveformTests(UART_Regs *uart);/*!< 波形生成测试 */

#endif
