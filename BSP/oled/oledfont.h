#ifndef __OLEDFONT_H
#define __OLEDFONT_H
#pragma once

#include <stdint.h>

/*
 * 调用说明：
 * 1. F6x8：6x8 的 ASCII 字体表。你一般不用直接操作它，OLED 显示字符函数内部会用。
 * 2. F8X16：8x16 的 ASCII 字体表。也是给 OLED 字符显示函数准备的。
 * 3. 如果你只是正常显示字符，知道这两个字库存在就够了，不需要自己手动读表。
 */
//标准ASCII表
//偏移量32
//ASCII字符集
//偏移量32
//大小:12*6
/************************************6*8字体************************************/
extern const unsigned char  F6x8[][6];
/****************************************8*16字体************************************/
extern const unsigned char  F8X16[];

/**
 * @brief 字库自测：检查字库表是否可访问。
 * @return 0 表示通过，1 表示失败。
 */
uint8_t OLEDFONT_runSelfTest(void);

#endif