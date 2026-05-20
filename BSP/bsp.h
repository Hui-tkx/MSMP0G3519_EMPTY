/** 
  * @file         bsp.h 
  * @details      板级支持包，包含所有硬件库的头文件 
  * @author       惠 
  * @version      V1.0 
  * @date         2026.03.06 
  * 
   * @note 这个文件是所有硬件库的总头文件，你只需要包含这一个文件，就可以使用所有的硬件库函数。
   *       所有库都已经设计得非常简单易用，傻子都能懂！
   *
   * 傻子式使用说明：
   * 1. 如果你不想记每个库单独的头文件，就直接包含这个文件。
   * 2. 包含这个文件后，你就能直接用 LED、KEY、UART、ADC、DAC、PWM、FLASH、BUZZER、OLED、WIFI、BLUETOOTH 这些库的函数。
   * 3. 一般写程序的顺序是：先调 SYSCFG_DL_init() 做系统初始化，再按需要初始化串口、蜂鸣器、DAC 之类的外设，最后开始写你的业务逻辑。
   * 4. 如果你只是想跑板级测试，初始化好串口后可以直接调用 BSP_Test_runAll()。
   * 5. 如果你写的是普通应用程序，这个文件的作用就是“把所有常用库一次性帮你包含进来”，省得你自己一个个 include。
  */
#ifndef BSP_H
#define BSP_H

/******************系统头文件***************/
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "math.h"
#include "ti_msp_dl_config.h"

/******************硬件库头文件***************/
#include "oled_spi.h"    // OLED显示库
#include "delay.h"       // 延时库
#include "led.h"         // LED控制库
#include "oledfont.h"    // OLED字体库
#include "key.h"         // 键盘控制库
#include "uart.h"        // UART通信库
#include "adc.h"         // ADC模数转换库
#include "dac.h"         // DAC数模转换库
#include "pwm.h"         // PWM控制库
#include "flash.h"       // Flash读写库
#include "buzzer.h"      // 蜂鸣器控制库
#include "spi.h"         // SPI通信库
#include "ringbuffer.h"  // 环形缓冲区库
#include "uart_ringbuffer.h" // UART环形缓冲区封装
#include "w25q64.h"     // W25Q64 SPI Flash 驱动库
#include "temperature.h" // 温度传感器库
#include "test.h"        // 板级测试库
/*******************************************/

/******************通用宏定义***************/
#define u8 unsigned char
#define u32 unsigned int
/*******************************************/

/**
 * @brief BSP库使用示例（傻子都能懂的使用方法）
 *
 * @code
 * // 只需要包含这一个头文件
 * #include "bsp.h"
 * 
 * int main(void) {
 *     // 初始化系统
 *     SYSCFG_DL_init();
 *     
 *     // 初始化各个库（如果需要）
 *     UART_init(UART_0_INST);
 *     ADC_init();
 *     DAC_init();
 *     FLASH_init();
 *     
 *     // 使用LED库
 *     LED_On(LED_L1);
 *     delay_cycles(1000000);
 *     LED_Off(LED_L1);
 *     
 *     // 使用键盘库
 *     char key = getKeyValue();
 *     if (key != '\0') {
 *         // 处理按键
 *     }
 *     
 *     // 使用UART库
 *     UART_sendString(UART_0_INST, "Hello World!\r\n");
 *     
 *     // 使用ADC库
 *     float voltage = ADC_readVoltage(ADC_CHANNEL_0);
 *     
 *     // 使用DAC库
 *     DAC_enableOutput();
 *     DAC_setVoltage(1.65f);
 *     
 *     // 使用蜂鸣器库
 *     BUZZER_init();
 *     BUZZER_notify(); // 发出提示音
 *     
 *     while (1) {
 *         // 主循环
 *     }
 * }
 * @endcode
 */

#endif
