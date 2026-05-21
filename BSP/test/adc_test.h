/**
 * @file adc_test.h
 * @brief ADC 库测试接口 (阻塞轮询 + 硬件平均)
 */
#ifndef _ADC_TEST_H_
#define _ADC_TEST_H_

#include "adc.h"
#include "uart.h"

#define ADC_TEST_SEPARATOR "=============================================="

void ADC_TEST_runAll(UART_Regs *uart);
void ADC_TEST_runBasicTests(UART_Regs *uart);
void ADC_TEST_runAverageTests(UART_Regs *uart);

#endif
