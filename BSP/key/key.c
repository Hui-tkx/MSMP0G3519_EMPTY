/** 
  * @file         key.c 
  * @details      键盘控制功能的实现 
  * @author       惠 
  * @version      V1.0 
  * @date         2026.03.06 
  */ 
#include "key.h"
#include "delay.h"
#include "../uart/uart.h"

static const uint32_t g_keyRowPins[4] = {
    KEYBOARD_H1_PIN,
    KEYBOARD_H2_PIN,
    KEYBOARD_H3_PIN,
    KEYBOARD_H4_PIN
};

static const uint32_t g_keyColumnPins[4] = {
    KEYBOARD_V1_PIN,
    KEYBOARD_V2_PIN,
    KEYBOARD_V3_PIN,
    KEYBOARD_V4_PIN
};

static const char g_keyMap[4][4] = {
    {'1', '2', '3', 'a'},
    {'4', '5', '6', 'b'},
    {'7', '8', '9', 'c'},
    {'*', '0', '#', 'd'}
};

static void KEY_setAllRowsHigh(void)
{
    DL_GPIO_setPins(KEYBOARD_PORT,
        KEYBOARD_H1_PIN | KEYBOARD_H2_PIN | KEYBOARD_H3_PIN | KEYBOARD_H4_PIN);
}

static void KEY_setActiveRow(uint32_t rowPin)
{
    KEY_setAllRowsHigh();
    DL_GPIO_clearPins(KEYBOARD_PORT, rowPin);
}

static char KEY_scanRaw(void)
{
    uint32_t rowIndex;
    uint32_t columnIndex;

    for (rowIndex = 0U; rowIndex < 4U; rowIndex++)
    {
        KEY_setActiveRow(g_keyRowPins[rowIndex]);
        delay_us(100U);

        for (columnIndex = 0U; columnIndex < 4U; columnIndex++)
        {
            if (DL_GPIO_readPins(KEYBOARD_PORT, g_keyColumnPins[columnIndex]) == 0U)
            {
                delay_ms(20U);
                if (DL_GPIO_readPins(KEYBOARD_PORT, g_keyColumnPins[columnIndex]) == 0U)
                {
                    KEY_setAllRowsHigh();
                    return g_keyMap[rowIndex][columnIndex];
                }
            }
        }
    }

    KEY_setAllRowsHigh();
    return '\0';
}

/**
 * @brief   获取矩阵键盘按键字符
 * @param   无
 * @return  按键对应的字符，未检测到按键时返回'\0'
 * @note    扫描4x4矩阵键盘，返回按下的按键字符
 *          按键布局：123a 456b 789c *0#d
 * @example char key = getKeyValue(); // 获取当前按下的按键字符
 */
char getKeyValue(void)
{
    static char lastReportedKey = '\0';
    char currentKey = KEY_scanRaw();

    if (currentKey == '\0')
    {
        lastReportedKey = '\0';
        return '\0';
    }

    if (currentKey == lastReportedKey)
    {
        return '\0';
    }

    lastReportedKey = currentKey;
    return currentKey;
}
