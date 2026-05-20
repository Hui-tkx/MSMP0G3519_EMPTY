/**
 * @file main.c
 */

#include "bsp.h"
#include "ti_msp_dl_config.h"
#include "temperature_test.h"

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
    TEMP_TEST_runAll(UART_0_INST);
    //*  TEST_runAll    | 运行所有测试             
     //*  TEST_runADC    | 运行 ADC 测试            
    // *  TEST_runDAC    | 运行 DAC 测试            
    // *  TEST_runUART   | 运行 UART 测试          
    // *  TEST_runRingBuffer | 运行 RingBuffer 测试 
    //TEST_runAll(UART_0_INST);
    //TEST_runADCTests(UART_0_INST);
    //TEST_runDACTests(UART_0_INST);
    //TEST_runringBuffer(UART_0_INST);
    while (1) {

    }
}
