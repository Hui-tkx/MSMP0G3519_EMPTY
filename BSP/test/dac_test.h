/**
 * @file dac_test.h
 * @brief DAC 测试接口 (DMA 波形播放)
 */
#ifndef _DAC_TEST_H_
#define _DAC_TEST_H_

#include "dac.h"
#include "uart.h"

void DAC_TEST_runAll(UART_Regs *uart);

#endif
