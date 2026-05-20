/**
 * @file buzzer.h
 * @brief 蜂鸣器控制接口。
 * @note
 * 1. 这个库适合做提示音、报警音、简单音调。
 * 2. 所有发声函数都是阻塞式的，调用期间 CPU 会一直忙着延时。
 *
 * 调用说明：
 * 1. BUZZER_init()：初始化蜂鸣器。一般开机时调一次。
 * 2. BUZZER_on()：直接打开蜂鸣器，会一直响。
 * 3. BUZZER_off()：关闭蜂鸣器。
 * 4. BUZZER_beep(duration)：响一小段时间。duration 单位是毫秒。
 * 5. BUZZER_tone(frequency, duration)：按指定频率发声。frequency 是 Hz，duration 是毫秒。
 * 6. BUZZER_notify()：播一个提示音。
 * 7. BUZZER_alert()：播一个警告音序列。
 * 8. BUZZER_runSelfTest()：跑蜂鸣器自测。
 */

#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "ti_msp_dl_config.h"
#include <stdint.h>

#define BUZ_PORT        BUZZER_PORT
#define BUZ_PIN         BUZZER_BUZ_PIN

/**
 * @brief 蜂鸣器初始化占位函数。
 * @note 引脚初始化通常由 SysConfig 生成代码完成。
 */
void BUZZER_init(void);

/**
 * @brief 打开蜂鸣器。
 */
void BUZZER_on(void);

/**
 * @brief 关闭蜂鸣器。
 */
void BUZZER_off(void);

/**
 * @brief 让蜂鸣器持续响一段时间。
 * @param duration 发声时间，单位毫秒。
 */
void BUZZER_beep(uint32_t duration);

/**
 * @brief 输出一个固定频率的方波音调。
 * @param frequency 音调频率，单位 Hz，传 0 不执行。
 * @param duration 发声时间，单位毫秒。
 */
void BUZZER_tone(uint32_t frequency, uint32_t duration);

/**
 * @brief 播放一个短提示音。
 */
void BUZZER_notify(void);

/**
 * @brief 播放一个短警告音序列。
 */
void BUZZER_alert(void);

#endif