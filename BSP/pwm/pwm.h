/**
 * @file pwm.h
 * @brief 两路 PWM 输出控制接口。
 * @note
 * 1. 先初始化频率，再设置占空比，再启动输出。
 * 2. 占空比统一按 0 到 100 传入，表示 0% 到 100%。
 * 3. 改频率后，当前缓存的占空比会自动重新应用，不需要你手动重设一遍。
 *
 * 调用说明：
 * 1. PWM_init(freq)：初始化 PWM。freq 是频率，单位 Hz。
 * 2. PWM_setDuty(channel, duty)：设置占空比。channel 传 PWM_CHANNEL_0 或 PWM_CHANNEL_1，duty 传 0 到 100。
 * 3. PWM_setFreq(freq)：改 PWM 频率，单位还是 Hz。
 * 4. PWM_start(channel)：打开某一路 PWM 输出。
 * 5. PWM_stop(channel)：关闭某一路 PWM 输出。
 * 6. PWM_startAll()：两路一起打开。
 * 7. PWM_stopAll()：两路一起关闭。
 * 8. PWM_runSelfTest()：跑 PWM 自测。
 */
#ifndef _PWM_H_
#define _PWM_H_

#include "ti_msp_dl_config.h"
#include <stdint.h>

#define PWM_INST                PWM_0_INST
#define PWM_CHANNEL_0           GPIO_PWM_0_C0_IDX
#define PWM_CHANNEL_1           GPIO_PWM_0_C1_IDX

/**
 * @brief 初始化 PWM 基础参数。
 * @param freq PWM 频率，单位 Hz，传 0 不执行。
 */
void PWM_init(uint32_t freq);

/**
 * @brief 设置某一路 PWM 的占空比。
 * @param channel PWM_CHANNEL_0 或 PWM_CHANNEL_1。
 * @param duty 占空比百分比，超出 100 会自动夹紧到 100。
 */
void PWM_setDuty(uint8_t channel, uint8_t duty);

/**
 * @brief 修改 PWM 频率。
 * @param freq 新频率，单位 Hz，传 0 不执行。
 */
void PWM_setFreq(uint32_t freq);

/**
 * @brief 启动某一路 PWM 输出。
 * @param channel PWM_CHANNEL_0 或 PWM_CHANNEL_1。
 */
void PWM_start(uint8_t channel);

/**
 * @brief 关闭某一路 PWM 输出。
 * @param channel PWM_CHANNEL_0 或 PWM_CHANNEL_1。
 */
void PWM_stop(uint8_t channel);

/**
 * @brief 同时启动两路 PWM 输出。
 */
void PWM_startAll(void);

/**
 * @brief 同时关闭两路 PWM 输出。
 */
void PWM_stopAll(void);


#endif