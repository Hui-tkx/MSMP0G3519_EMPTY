/**
 * @file temperature_test.h
 * @brief 温度传感器测试接口 (UART 输出)
 */
#ifndef _TEMPERATURE_TEST_H_
#define _TEMPERATURE_TEST_H_

#include "uart.h"

void TEMP_TEST_runAll(UART_Regs *uart);

#endif
