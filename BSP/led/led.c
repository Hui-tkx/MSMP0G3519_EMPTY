/** 
  * @file         led.c 
  * @details      LED控制功能的实现 
  * @author       惠 
  * @version      V1.0 
  * @date         2026.03.06 
  */ 
#include "led.h"
#include "delay.h"

/**
 * @brief   根据LED编号获取对应的端口和引脚
 * @param   led: LED编号 (LED_L1 ~ LED_L8)
 * @param   port: 输出参数，GPIO端口指针
 * @param   pin: 输出参数，GPIO引脚
 * @return  无
 * @note    内部辅助函数，用于获取LED对应的GPIO端口和引脚
 */
static bool LED_getPortPin(LED_NUM led, GPIO_Regs **port, uint32_t *pin)
{
    switch(led)
    {
        case LED_L1:
            *port = LED_L1_PORT;
            *pin = LED_L1_PIN;
            return true;
        case LED_L2:
            *port = LED_L2_PORT;
            *pin = LED_L2_PIN;
            return true;
        case LED_L3:
            *port = LED_L3_PORT;
            *pin = LED_L3_PIN;
            return true;
        case LED_L4:
            *port = LED_L4_PORT;
            *pin = LED_L4_PIN;
            return true;
        case LED_L5:
            *port = LED_L5_PORT;
            *pin = LED_L5_PIN;
            return true;
        case LED_L6:
            *port = LED_L6_PORT;
            *pin = LED_L6_PIN;
            return true;
        case LED_L7:
            *port = LED_L7_PORT;
            *pin = LED_L7_PIN;
            return true;
        case LED_L8:
            *port = LED_L8_PORT;
            *pin = LED_L8_PIN;
            return true;
        default:
            *port = NULL;
            *pin = 0;
            return false;
    }
}

static void LED_turnAllOff(void)
{
    LED_Off(LED_L1);
    LED_Off(LED_L2);
    LED_Off(LED_L3);
    LED_Off(LED_L4);
    LED_Off(LED_L5);
    LED_Off(LED_L6);
    LED_Off(LED_L7);
    LED_Off(LED_L8);
}

/**
 * @brief   LED点亮
 * @param   led: LED编号 (LED_L1 ~ LED_L8)
 * @return  无
 * @note    将指定的LED设置为低电平状态（点亮）
 */
void LED_On(LED_NUM led)
{
    GPIO_Regs* port;
    uint32_t pin;
    if (LED_getPortPin(led, &port, &pin))
    {
        DL_GPIO_clearPins(port, pin);
    }
}

/**
 * @brief   LED熄灭
 * @param   led: LED编号 (LED_L1 ~ LED_L8)
 * @return  无
 * @note    将指定的LED设置为高电平状态（关闭）
 */
void LED_Off(LED_NUM led)
{
    GPIO_Regs* port;
    uint32_t pin;
    if (LED_getPortPin(led, &port, &pin))
    {
        DL_GPIO_setPins(port, pin);
    }
}

/**
 * @brief   LED状态翻转
 * @param   led: LED编号 (LED_L1 ~ LED_L8)
 * @return  无
 * @note    将指定LED的状态从高电平变为低电平，或从低电平变为高电平
 */
void LED_Toggle(LED_NUM led)
{
    GPIO_Regs* port;
    uint32_t pin;
    if (LED_getPortPin(led, &port, &pin))
    {
        DL_GPIO_togglePins(port, pin);
    }
}

/**
 * @brief   LED闪烁
 * @param   led: LED编号 (LED_L1 ~ LED_L8)
 * @param   freq: 闪烁频率，单位：Hz
 * @return  无
 * @note    让指定的LED以指定频率闪烁一次（亮灭一个周期）
 * @example LED_Blink(LED_L1, 2); // 让LED L1以2Hz频率闪烁
 */
void LED_Blink(LED_NUM led, uint32_t freq)
{
    uint32_t halfPeriodMs;

    if (freq == 0U) {
        return;
    }

    halfPeriodMs = 500U / freq;
    if (halfPeriodMs == 0U) {
        halfPeriodMs = 1U;
    }

    LED_On(led);
    delay_ms(halfPeriodMs);
    LED_Off(led);
    delay_ms(halfPeriodMs);
}

/**
 * @brief   LED呼吸灯
 * @param   led: LED编号 (LED_L1 ~ LED_L8)
 * @param   freq: 呼吸频率，单位：Hz
 * @return  无
 * @note    让指定的LED以指定频率进行呼吸灯效果（渐亮渐暗）
 * @example LED_Breathe(LED_L2, 1); // 让LED L2以1Hz频率呼吸
 */
void LED_Breathe(LED_NUM led, uint32_t freq)
{
    GPIO_Regs *port;
    uint32_t pin;
    uint32_t cycleUs;
    uint32_t levelWindowUs;
    uint32_t i;

    if ((freq == 0U) || !LED_getPortPin(led, &port, &pin)) {
        return;
    }

    cycleUs = 1000000U / freq;
    levelWindowUs = cycleUs / 200U;
    if (levelWindowUs == 0U) {
        levelWindowUs = 1U;
    }

    for (i = 0U; i < 100U; i++) {
        uint32_t onTimeUs = (levelWindowUs * i) / 100U;
        uint32_t offTimeUs = levelWindowUs - onTimeUs;

        LED_On(led);
        delay_us(onTimeUs);
        LED_Off(led);
        delay_us(offTimeUs);
    }

    for (i = 100U; i > 0U; i--) {
        uint32_t onTimeUs = (levelWindowUs * i) / 100U;
        uint32_t offTimeUs = levelWindowUs - onTimeUs;

        LED_On(led);
        delay_us(onTimeUs);
        LED_Off(led);
        delay_us(offTimeUs);
    }
}

/**
 * @brief   跑马灯
 * @param   speed: 跑马速度，单位：ms
 * @return  无
 * @note    让LED L1到LED L8依次点亮，形成跑马灯效果
 * @example LED_Running(200); // 以200ms的速度运行跑马灯
 */
void LED_Running(uint32_t speed)
{
    uint8_t i;
    
    for(i = 1; i <= 8; i++)
    {
        LED_turnAllOff();
        LED_On((LED_NUM)i);
        delay_ms(speed);
    }

    LED_turnAllOff();
}
