/** 
  * @file         buzzer.c 
  * @details      蜂鸣器控制功能的实现 
  * @author       惠 
  * @version      V1.0 
  * @date         2026.03.06 
  */ 

#include "buzzer.h"
#include "delay.h"

/**
 * @brief 初始化蜂鸣器
 * @return 无
 * @note 初始化蜂鸣器引脚为输出模式
 */
void BUZZER_init(void)
{
    /*
     * SysConfig 生成代码会先把蜂鸣器脚拉低再打开输出。
     * 对低电平有效的蜂鸣器来说，这等价于一上电就导通。
     */
    BUZZER_off();
}

/**
 * @brief 开启蜂鸣器
 * @return 无
 * @note 开启蜂鸣器，使其发声
 */
void BUZZER_on(void)
{
    DL_GPIO_clearPins(BUZ_PORT, BUZ_PIN);
}

/**
 * @brief 关闭蜂鸣器
 * @return 无
 * @note 关闭蜂鸣器，停止发声
 */
void BUZZER_off(void)
{
    DL_GPIO_setPins(BUZ_PORT, BUZ_PIN);
}

/**
 * @brief 蜂鸣器发声指定时间
 * @param duration 发声时间（毫秒）
 * @return 无
 * @note 让蜂鸣器发声指定的时间后自动关闭
 */
void BUZZER_beep(uint32_t duration)
{
    if (duration == 0U)
    {
        return;
    }

    BUZZER_on();
    delay_ms(duration);
    BUZZER_off();
}

/**
 * @brief 蜂鸣器发出指定频率的声音
 * @param frequency 频率（Hz）
 * @param duration 发声时间（毫秒）
 * @return 无
 * @note 让蜂鸣器发出指定频率的声音，持续指定的时间
 */
void BUZZER_tone(uint32_t frequency, uint32_t duration)
{
    uint32_t period;
    uint32_t halfPeriod;
    uint64_t remainingUs;

    if ((frequency == 0U) || (duration == 0U))
    {
        return;
    }

    period = 1000000U / frequency;
    if (period < 2U)
    {
        period = 2U;
    }

    halfPeriod = period / 2U;
    remainingUs = (uint64_t) duration * 1000ULL;

    while (remainingUs >= period)
    {
        BUZZER_on();
        delay_us(halfPeriod);
        BUZZER_off();
        delay_us(period - halfPeriod);
        remainingUs -= period;
    }
}

/**
 * @brief 蜂鸣器发出提示音
 * @return 无
 * @note 发出一个简短的提示音
 */
void BUZZER_notify(void)
{
    BUZZER_tone(1000, 100);
    delay_ms(100);
    BUZZER_tone(1500, 100);
}

/**
 * @brief 蜂鸣器发出警告音
 * @return 无
 * @note 发出一个持续的警告音
 */
void BUZZER_alert(void)
{
    BUZZER_tone(800, 300);
    delay_ms(100);
    BUZZER_tone(800, 300);
    delay_ms(100);
    BUZZER_tone(800, 300);
}
