/**
 * @file uart_test.h
 * @brief UART 库测试接口
 */
#ifndef _UART_TEST_H_
#define _UART_TEST_H_

#include "uart.h"

void UART_TEST_runAll(UART_Regs *uart);
void UART_TEST_runPollingTests(UART_Regs *uart);
void UART_TEST_runDmaTests(UART_Regs *uart);

#endif
