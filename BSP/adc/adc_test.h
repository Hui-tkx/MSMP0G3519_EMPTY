/**
 * @file adc_test.h
 * @brief ADC 库测试接口 (阻塞轮询 + 硬件平均)
 */
#ifndef _ADC_TEST_H_
#define _ADC_TEST_H_

#include "adc.h"
#include "uart.h"

#define ADC_TEST_SEPARATOR "=============================================="  /*!< 分隔线 */

void ADC_TEST_runAll(UART_Regs *uart);          /*!< 运行全部测试 */
void ADC_TEST_runBasicTests(UART_Regs *uart);   /*!< 基础 API 测试 */
void ADC_TEST_runAverageTests(UART_Regs *uart); /*!< 硬件平均测试 */

#endif
