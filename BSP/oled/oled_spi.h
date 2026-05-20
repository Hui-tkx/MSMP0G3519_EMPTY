#ifndef __OLED_SPI_V0_2_H__
#define __OLED_SPI_V0_2_H__

#include "ti_msp_dl_config.h"
#include <stdint.h>

#define OLED_CMD  0
#define OLED_DATA 1
#define OLED_MODE 0

typedef unsigned char u8;
typedef unsigned int  u32;

#define SPI_OLED_INST SPI_INST

#define OLED_DC_Clr() DL_GPIO_clearPins(OLED_DC_PORT, OLED_DC_PIN)
#define OLED_DC_Set() DL_GPIO_setPins(OLED_DC_PORT, OLED_DC_PIN)

#define OLED_RST_Clr() DL_GPIO_clearPins(OLED_RES_PORT, OLED_RES_PIN)
#define OLED_RST_Set() DL_GPIO_setPins(OLED_RES_PORT, OLED_RES_PIN)

#define OLED_CS_Clr()  DL_GPIO_clearPins(OLED_CS_PORT, OLED_CS_PIN)
#define OLED_CS_Set()  DL_GPIO_setPins(OLED_CS_PORT, OLED_CS_PIN)

#define CPU_Frq      (CPUCLK_FREQ / 1000U)
#define SIZE         12
#define XLevelL      0x02
#define XLevelH      0x10
#define Max_Column   128
#define Max_Row      64
#define Brightness   0xFF
#define X_WIDTH      128
#define Y_WIDTH      64

/**
 * @file oled_spi.h
 * @brief SPI OLED 显示驱动接口。
 * @note
 * 1. 这套接口分两层：一层是直接写屏，一层是操作显存 OLED_GRAM 后再刷新。
 * 2. 画点、画线、画圆、显示字符串这些函数主要改的是显存。
 * 3. 改完显存以后，记得调用 OLED_Refresh，屏幕才会真的更新。
 *
 * 调用说明：
 * 1. OLED_Init()：初始化 OLED。一般开机后先调它。
 * 2. OLED_Clear()：清屏。
 * 3. OLED_ShowChar(x, y, chr)：显示 1 个字符。x 和 y 是坐标，chr 是字符。
 * 4. OLED_ShowString(x, y, p)：显示字符串。p 传字符串地址。
 * 5. OLED_ShowNum(x, y, num, len, size2)：显示无符号整数。num 是数字，len 是显示位数，size2 是字体大小参数。
 * 6. OLED_ShowSignedNum(x, y, num, len, size2)：显示有符号整数。
 * 7. OLED_Show_float(x, y, num, len_1, len_2)：显示浮点数。len_1 是整数部分位数，len_2 是小数部分位数。
 * 8. OLED_DrawPoint、OLED_DrawLine、OLED_DrawCircle、OLED_Fill：画点画线画圆和区域填充。
 * 9. OLED_Refresh()：把显存真正刷到屏幕上。很多画图函数调完后都要再调这个。
 * 10. OLED_runSelfTest()：跑 OLED 自测。
 */

/** @brief 向 OLED 写 1 个字节。 */
void OLED_WR_Byte(u8 dat, u8 cmd);

/** @brief 打开 OLED 显示。 */
void OLED_Display_On(void);

/** @brief 关闭 OLED 显示。 */
void OLED_Display_Off(void);

/** @brief 初始化 OLED。 */
void OLED_Init(void);

/** @brief 清空显存并刷新到屏幕。 */
void OLED_Clear(void);

/** @brief 设置当前写入位置，y 以页为单位。 */
void OLED_Set_Pos(u8 x, u8 y);

/** @brief 在显存中画一个点。 */
void OLED_DrawPoint(u8 x, u8 y, u8 t);

/** @brief 填充一个矩形区域。 */
void OLED_Fill(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot);

/** @brief 显示单个 ASCII 字符。 */
void OLED_ShowChar(u8 x, u8 y, char chr);

/** @brief 显示无符号整数。 */
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2);

/** @brief 显示有符号整数。 */
void OLED_ShowSignedNum(u8 x, u8 y, int32_t num, u8 len, u8 size2);

/** @brief 显示字符串。 */
void OLED_ShowString(u8 x, u8 y, const char *p);

/** @brief 显示字库中的中文字符。 */
void OLED_ShowCHinese(u8 x, u8 y, u8 no);

/** @brief 绘制一张位图。 */
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, const u8 BMP[]);

/** @brief 设置是否反色显示。 */
void OLED_ColorTurn(u8 i);

/** @brief 设置是否 180 度翻转显示方向。 */
void OLED_DisplayTurn(u8 i);

/** @brief 把显存内容刷新到 OLED 屏幕。 */
void OLED_Refresh(void);

/** @brief 画一条直线。 */
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode);

/** @brief 画一个圆。 */
void OLED_DrawCircle(u8 x, u8 y, u8 r);

/** @brief 显示浮点数。 */
void OLED_Show_float(u8 x, u8 y, float num, u8 len_1, u8 len_2);


#endif