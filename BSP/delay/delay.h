#ifndef DELAY_H
#define DELAY_H

#include "ti_msp_dl_config.h"
#include <stdint.h>

/**
 * @file delay.h
 * @brief 最基础的阻塞延时函数。
 * @note
 * 1. 这些函数会让 CPU 原地等待，等待期间不会去做别的事。
 * 2. 适合做很短的硬件稳定延时，比如复位、时序间隔、简单轮询。
 * 3. 不适合做长时间任务调度；如果你以后要做复杂程序，建议换成定时器或中断。
 *
 * 调用说明：
 * 1. delay_us(count)：按微秒延时。count 传多少，就等多少微秒。
 * 2. delay_ms(count)：按毫秒延时。最常用。
 * 3. delay_s(count)：按秒延时。做长等待时用。
 * 4. DELAY_runSelfTest()：跑延时库自测。
 */

/**
 * @brief 按微秒阻塞延时。
 * @param count 要等待的微秒数。
 */
void delay_us(uint32_t count);

/**
 * @brief 按毫秒阻塞延时。
 * @param count 要等待的毫秒数。
 */
void delay_ms(uint32_t count);

/**
 * @brief 按秒阻塞延时。
 * @param count 要等待的秒数。
 */
void delay_s(uint32_t count);


#endif