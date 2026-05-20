/**
 * @file led.h
 * @brief 板载 LED 控制接口。
 * @note
 * 1. 这个板子的 LED 是低电平点亮，所以 LED_On 内部会拉低引脚。
 * 2. 如果你看到“逻辑和肉眼相反”，先别怀疑人生，优先怀疑是不是低电平有效。
 *
 * 调用说明：
 * 1. LED_On(led)：点亮某个灯。led 传 LED_L1 到 LED_L8。
 * 2. LED_Off(led)：熄灭某个灯。参数还是传灯号。
 * 3. LED_Toggle(led)：把这个灯当前状态反过来，亮就灭，灭就亮。
 * 4. LED_Blink(led, freq)：让这个灯闪一下。freq 是频率，单位 Hz。
 * 5. LED_Breathe(led, freq)：做呼吸灯效果。freq 越大，变化越快。
 * 6. LED_Running(speed)：从 L1 跑到 L8。speed 是每个灯停留多少毫秒。
 * 7. LED_runSelfTest()：跑 LED 自测。
 */
#ifndef _LED_H_
#define _LED_H_

#include "ti_msp_dl_config.h"
#include <stdint.h>

typedef enum {
    LED_L1 = 1,
    LED_L2 = 2,
    LED_L3 = 3,
    LED_L4 = 4,
    LED_L5 = 5,
    LED_L6 = 6,
    LED_L7 = 7,
    LED_L8 = 8
} LED_NUM;

/**
 * @brief 点亮一个 LED。
 * @param led 目标 LED 编号。
 */
void LED_On(LED_NUM led);

/**
 * @brief 熄灭一个 LED。
 * @param led 目标 LED 编号。
 */
void LED_Off(LED_NUM led);

/**
 * @brief 翻转一个 LED 的当前状态。
 * @param led 目标 LED 编号。
 */
void LED_Toggle(LED_NUM led);

/**
 * @brief 让 LED 完整闪烁一次。
 * @param led 目标 LED 编号。
 * @param freq 闪烁频率，单位 Hz，传 0 不执行。
 */
void LED_Blink(LED_NUM led, uint32_t freq);

/**
 * @brief 做一次简易“呼吸灯”效果。
 * @param led 目标 LED 编号。
 * @param freq 呼吸频率，单位 Hz，传 0 不执行。
 * @note 这是软件延时模拟，不适合高精度亮度控制。
 */
void LED_Breathe(LED_NUM led, uint32_t freq);

/**
 * @brief 从 L1 到 L8 依次跑一遍灯。
 * @param speed 每个灯停留的时间，单位毫秒。
 */
void LED_Running(uint32_t speed);


#endif