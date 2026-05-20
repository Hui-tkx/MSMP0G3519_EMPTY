/**
 * @file key.h
 * @brief 4x4 矩阵键盘扫描接口。
 * @note
 * 1. 每次调用只返回一个按键字符。
 * 2. 没按键时返回 '\0'。
 * 3. 函数内部做了最基本的消抖，并且同一次按住只会上报一次，不会卡在函数里等松手。
 * 4. 当前这套引脚配置使用列上拉输入，所以按键按下时读到的是低电平。
 *
 * 调用说明：
 * 1. getKeyValue()：扫一次键盘，返回按键字符。
 * 2. 如果没按键，会返回 '\0'，这表示“什么都没按”。
 * 3. 返回值默认可能是 1 2 3 a / 4 5 6 b / 7 8 9 c / * 0 # d 这些字符。
 * 4. KEY_runSelfTest()：跑按键库自测。
 */
#ifndef _KEY_H_
#define _KEY_H_

#include "ti_msp_dl_config.h"
#include <stdint.h>

/**
 * @brief 扫描矩阵键盘并返回当前按下的键值。
 * @return
 * 返回按键字符；如果当前没有稳定按键，返回 '\0'。
 *
 * @note 默认字符布局按行排列如下：
 * 1 2 3 a
 * 4 5 6 b
 * 7 8 9 c
 * * 0 # d
 */
char getKeyValue(void);


#endif