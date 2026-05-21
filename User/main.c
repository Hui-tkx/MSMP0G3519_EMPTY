/**
 * @file main.c
 *
 * 测试入口说明:
 *   TEST_all(UART_0_INST);          // 跑全部测试
 *   TEST_adc(UART_0_INST);          // ADC 测试
 *   TEST_dac(UART_0_INST);          // DAC 测试
 *   TEST_uart(UART_0_INST);         // UART 测试
 *   TEST_temperature(UART_0_INST);  // 温度传感器测试
 *   TEST_ringbuffer(UART_0_INST);   // RingBuffer 测试
 *   TEST_uart_rb(UART_0_INST);      // UART RingBuffer 测试
 */

#include "bsp.h"
#include "ti_msp_dl_config.h"

void HardFault_Handler(void)
{
    while (1) {
    }
}
int main(void)
{
    SYSCFG_DL_init();

    UART_init(UART_0_INST);
    ADC_init();
    DAC_init();

    TEST_temperature(UART_0_INST);

    while (1) {

    }
}
