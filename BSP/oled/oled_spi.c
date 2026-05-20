#include "oled_spi.h"
#include "oledfont.h"
#include "../delay/delay.h"
#include <string.h>

// 向SSD1306写入一个字节
// dat:要写入的数据/命令
// cmd:数据/命令标志 0，表示命令;1，表示数据;
u8 OLED_GRAM[130][8];

static const uint8_t g_oledBmp16x16[32] = {
    0x00, 0x18, 0x3C, 0x7E, 0xE7, 0xC3, 0x81, 0x81,
    0x81, 0x81, 0xC3, 0xE7, 0x7E, 0x3C, 0x18, 0x00,
    0x00, 0x18, 0x24, 0x42, 0x81, 0x81, 0x81, 0xBD,
    0xBD, 0x81, 0x81, 0x81, 0x42, 0x24, 0x18, 0x00
};

static void OLED_SPI_waitReady(void)
{
    while (DL_SPI_isTXFIFOFull(SPI_OLED_INST))
    {
    }
}

void OLED_WR_Byte(u8 dat, u8 cmd)
{
    if (cmd)
    {
        OLED_DC_Set();
    }
    else
    {
        OLED_DC_Clr();
    }
    OLED_CS_Clr();
    OLED_SPI_waitReady();
    DL_SPI_transmitData8(SPI_OLED_INST, dat);
    while (DL_SPI_isBusy(SPI_OLED_INST))
    {
    }
    OLED_CS_Set();
    OLED_DC_Set();
}

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    uint8_t column = x + 2U;

    OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((column & 0xf0U) >> 4) | 0x10U, OLED_CMD);
    OLED_WR_Byte(column & 0x0fU, OLED_CMD);
}

// 开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); // 设置DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD); // DCDC开启
    OLED_WR_Byte(0XAF, OLED_CMD); // 显示开启
}

// 关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); // 设置DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD); // DCDC关闭
    OLED_WR_Byte(0XAE, OLED_CMD); // 显示关闭
}

// 清屏函数，清屏后整个屏幕是黑色的！和没点亮一样!!!
void OLED_Clear(void)
{
    memset(OLED_GRAM, 0, sizeof(OLED_GRAM));
    OLED_Refresh();
}

// 在指定位置显示一个字符，包括部分符号
// x:0~127
// y:0~7（页地址）
void OLED_ShowChar(u8 x, u8 y, char chr)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; // 得到偏移后的值
    if (x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
    if (SIZE == 16)
    {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
    }
    else
    {
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 6; i++)
            OLED_WR_Byte(F6x8[c][i], OLED_DATA);
    }
}

// m^n函数
u32 oled_pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
        result *= m;
    return result;
}

void OLED_Fill(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot)
{
    u8 x;
    u8 y;

    if ((x1 >= Max_Column) || (y1 >= Max_Row))
    {
        return;
    }

    if (x2 >= Max_Column)
    {
        x2 = Max_Column - 1;
    }

    if (y2 >= Max_Row)
    {
        y2 = Max_Row - 1;
    }

    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)
        {
            OLED_DrawPoint(x, y, dot);
        }
    }
}

// 显示数字
// x,y :起点坐标
// len :数字的位数
// size2:字体大小
// num:数字（0~4294967295）;
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ');
                continue;
            }
            else
                enshow = 1;
        }
        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0');
    }
}

// 可以显示负数
void OLED_ShowSignedNum(u8 x, u8 y, int32_t num, u8 len, u8 size2)
{
    u8 t, temp;
    u8 enshow = 0;
    u8 isNegative = 0;
    uint32_t magnitude;

    if (num < 0)
    {
        isNegative = 1;
        magnitude = (uint32_t) (-(int64_t) num);
    }
    else
    {
        magnitude = (uint32_t) num;
    }

    if (isNegative)
    {
        OLED_ShowChar(x, y, '-');
        x += size2 / 2;
        len--;
    }

    for (t = 0; t < len; t++)
    {
        temp = (magnitude / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ');
                continue;
            }
            else
                enshow = 1;
        }
        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0');
    }
}

// 显示一个字符串
void OLED_ShowString(u8 x, u8 y, const char *chr)
{
    unsigned char j = 0;

    if (chr == NULL)
    {
        return;
    }

    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j]);
        x += 8;
        if (x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

void OLED_ShowCHinese(u8 x, u8 y, u8 no)
{
    /*
     * 当前工程没有真正的 16x16 中文字库表，
     * 所以这里先显示占位内容，至少保证接口可用、测试可跑通。
     */
    OLED_ShowChar(x, y, 'C');
    OLED_ShowChar(x + 8U, y, 'N');
    OLED_ShowNum(x + 16U, y, no, 2U, 16U);
}

void OLED_Show_float(u8 x, u8 y, float num, u8 len_1, u8 len_2)
{
    float value = num;
    u32 scale = oled_pow(10U, len_2);
    u32 integerPart;
    u32 fractionPart;

    if (value < 0.0f)
    {
        OLED_ShowChar(x, y, '-');
        x += 8U;
        value = -value;
    }

    integerPart = (u32) value;
    fractionPart = (u32) ((value - (float) integerPart) * (float) scale + 0.5f);
    if (fractionPart >= scale)
    {
        integerPart++;
        fractionPart -= scale;
    }

    OLED_ShowNum(x, y, integerPart, len_1, 16U);
    OLED_ShowChar(x + (len_1 * 8U), y, '.');
    OLED_ShowNum(x + ((len_1 + 1U) * 8U), y, fractionPart, len_2, 16U);
}


/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为需要使用的页的范围1～8*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, const unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++)
    {
        OLED_Set_Pos(x0, y);
        for (x = x0; x < x1; x++)
        {
            OLED_WR_Byte(BMP[j++], OLED_DATA);
        }
    }
}

// 颜色反转
void OLED_ColorTurn(u8 i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xA6, OLED_CMD); // 正常显示
    }
    if (i == 1)
    {
        OLED_WR_Byte(0xA7, OLED_CMD); // 反色显示
    }
}

// 屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xC8, OLED_CMD); // 正常显示
        OLED_WR_Byte(0xA1, OLED_CMD);
    }
    if (i == 1)
    {
        OLED_WR_Byte(0xC0, OLED_CMD); // 旋转显示
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}

// 更新显存到OLED
void OLED_Refresh(void)
{
    u8 i, n;

    for (i = 0; i < 8; i++)
    {
        OLED_Set_Pos(0, i);
        for (n = 0; n < 128; n++)
        {
            OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}

// 画点
// x:0~127
// y:0~63
// t:1 点亮 0,熄灭
void OLED_DrawPoint(u8 x, u8 y, u8 t)
{
    u8 i, m, n;

    if ((x >= Max_Column) || (y >= Max_Row))
    {
        return;
    }

    i = y / 8;
    m = y % 8;
    n = 1 << m;
    if (t)
    {
        OLED_GRAM[x][i] |= n;
    }
    else
    {
        OLED_GRAM[x][i] &= (u8) (~n);
    }
}

// 画线
// x1,y1:起点坐标
// x2,y2:终点坐标
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; // 计算坐标差值
    delta_y = y2 - y1;
    uRow = x1; // 画线起点坐标
    uCol = y1;
    if (delta_x > 0)
        incx = 1; // 设置单步方向
    else if (delta_x == 0)
        incx = 0; // 垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // 水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; // 选取增量大的为基本增量
    else
        distance = delta_y;
    for (t = 0; t < distance + 1; t++)
    {
        OLED_DrawPoint(uRow, uCol, mode); // 画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

// 画圆
// x,y:圆心坐标
// r:圆的半径
void OLED_DrawCircle(u8 x, u8 y, u8 r)
{
    int a, b, num;
    a = 0;
    b = r;
    while (2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b, 1);
        OLED_DrawPoint(x - a, y - b, 1);
        OLED_DrawPoint(x - a, y + b, 1);
        OLED_DrawPoint(x + a, y + b, 1);

        OLED_DrawPoint(x + b, y + a, 1);
        OLED_DrawPoint(x + b, y - a, 1);
        OLED_DrawPoint(x - b, y - a, 1);
        OLED_DrawPoint(x - b, y + a, 1);

        a++;
        num = (a * a + b * b) - r * r; // 计算画点的离圆心的距离
        if (num > 0)
        {
            b--;
            a--;
        }
    }
}

// 初始化SSD1306
void OLED_Init(void)
{
    OLED_RST_Set();
    delay_cycles(CPU_Frq * 100);
    OLED_RST_Clr();
    delay_cycles(CPU_Frq * 100);
    OLED_RST_Set();

    OLED_WR_Byte(0xAE, OLED_CMD); // 关闭OLED面板
    OLED_WR_Byte(0x02, OLED_CMD); // 设置低列地址
    OLED_WR_Byte(0x10, OLED_CMD); // 设置高列地址
    OLED_WR_Byte(0x40, OLED_CMD); // 设置起始行地址  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81, OLED_CMD); // 设置对比度控制寄存器
    OLED_WR_Byte(0xCF, OLED_CMD); // 设置SEG输出电流亮度
    OLED_WR_Byte(0xA1, OLED_CMD); // 设置SEG/列映射 0xa0左右反转 0xa1正常
    OLED_WR_Byte(0xC8, OLED_CMD); // 设置COM/行扫描方向 0xc0上下反转 0xc8正常
    OLED_WR_Byte(0xA6, OLED_CMD); // 设置正常显示
    OLED_WR_Byte(0xA8, OLED_CMD); // 设置多路复用率(1 到 64)
    OLED_WR_Byte(0x3f, OLED_CMD); // 1/64占空比
    OLED_WR_Byte(0xD3, OLED_CMD); // 设置显示偏移 Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD); // 无偏移
    OLED_WR_Byte(0xd5, OLED_CMD); // 设置显示时钟分频比/振荡器频率
    OLED_WR_Byte(0x80, OLED_CMD); // 设置分频比，设置时钟为100帧/秒
    OLED_WR_Byte(0xD9, OLED_CMD); // 设置预充电周期
    OLED_WR_Byte(0xF1, OLED_CMD); // 设置预充电为15个时钟，放电为1个时钟
    OLED_WR_Byte(0xDA, OLED_CMD); // 设置COM引脚硬件配置
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD); // 设置VCOMH
    OLED_WR_Byte(0x40, OLED_CMD); // 设置VCOM解选电平
    OLED_WR_Byte(0x20, OLED_CMD); // 设置页面寻址模式 (0x00/0x01/0x02)
    OLED_WR_Byte(0x02, OLED_CMD);
    OLED_WR_Byte(0x8D, OLED_CMD); // 设置电荷泵使能/禁用
    OLED_WR_Byte(0x14, OLED_CMD); // 启用(0x10禁用)
    OLED_WR_Byte(0xA4, OLED_CMD); // 禁用整个显示开启 (0xa4/0xa5)
    OLED_WR_Byte(0xA6, OLED_CMD); // 禁用反相显示开启 (0xa6/a7)
    OLED_WR_Byte(0xAF, OLED_CMD); // 开启OLED面板

    OLED_WR_Byte(0xAF, OLED_CMD); /*显示开启*/
    OLED_Clear();
    OLED_Set_Pos(0, 0);
}


//// 以下为原代码中被注释的冗余部分（已修正注释乱码，保留原结构）
//#if 0
//// 向SSD1106写入一个字节
//// dat:要写入的数据/命令
//// cmd:数据/命令标志 0，表示命令;1，表示数据;
//void OLED_WR_Byte(u8 dat,u8 cmd)
//{
//    DATAOUT(dat);
//    if(cmd)
//      OLED_DC_Set();
//    else
//      OLED_DC_Clr();
//    OLED_CS_Clr();
//    OLED_WR_Clr();
//    OLED_WR_Set();
//    OLED_CS_Set();
//    OLED_DC_Set();
//}
//#else
//// 向SSD1306写入一个字节
//// dat:要写入的数据/命令
//// cmd:数据/命令标志 0，表示命令;1，表示数据;
//void OLED_WR_Byte(u8 dat,u8 cmd)
//{
//    u8 i;
//    if(cmd)
//      OLED_DC_Set();
//    else
//      OLED_DC_Clr();
//    OLED_CS_Clr();
//    for(i=0;i<8;i++)
//    {
//        OLED_SCLK_Clr();
//        if(dat&0x80)
//            {
//           OLED_SDIN_Set();
//            }
//else
//           OLED_SDIN_Clr();
//                OLED_SCLK_Set();
//        dat<<=1;
//    }
//    OLED_CS_Set();
//    OLED_DC_Set();
//}
//#endif

//void OLED_Set_Pos(unsigned char x, unsigned char y)
//{
//    OLED_WR_Byte(0xb0+y,OLED_CMD);
//    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
//    OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
//}
//// 开启OLED显示
//void OLED_Display_On(void)
//{
//    OLED_WR_Byte(0X8D,OLED_CMD);  //设置DCDC命令
//    OLED_WR_Byte(0X14,OLED_CMD);  //DCDC开启
//    OLED_WR_Byte(0XAF,OLED_CMD);  //显示开启
//}
//// 关闭OLED显示
//void OLED_Display_Off(void)
//{
//    OLED_WR_Byte(0X8D,OLED_CMD);  //设置DCDC命令
//    OLED_WR_Byte(0X10,OLED_CMD);  //DCDC关闭
//    OLED_WR_Byte(0XAE,OLED_CMD);  //显示关闭
//}
//// 清屏函数，清屏后整个屏幕是黑色的！和没点亮一样!!!
//void OLED_Clear(void)
//{
//    u8 i,n;
//    for(i=0;i<8;i++)
//    {
//        OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
//        OLED_WR_Byte (0x02,OLED_CMD);      //设置显示位置—列低地址
//        OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
//        for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);
//    } // 更新显示
//}

//// 在指定位置显示一个字符，包括部分符号
//// x:0~127
//// y:0~63
//// mode:0，反白显示;1，正常显示
//// size:选择字体 16/12
//void OLED_ShowChar(u8 x,u8 y,u8 chr)
//{
//    unsigned char c=0,i=0;
//    c=chr-' ';//得到偏移后的值
//    if(x>Max_Column-1){x=0;y=y+2;}
//    if(SIZE ==16)
//        {
//        OLED_Set_Pos(x,y);
//        for(i=0;i<8;i++)
//        OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
//        OLED_Set_Pos(x,y+1);
//        for(i=0;i<8;i++)
//        OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
//        }
//        else {
//                OLED_Set_Pos(x,y+1);
//                for(i=0;i<6;i++)
//                OLED_WR_Byte(F6x8[c][i],OLED_DATA);

//        }
//}
//// m^n函数
//u32 oled_pow(u8 m,u8 n)
//{
//    u32 result=1;
//    while(n--)result*=m;
//    return result;
//}
//// 显示2个数字
//// x,y :起点坐标
//// len :数字的位数
//// size:字体大小
//// mode:模式  0，填充模式;1，叠加模式
//// num:数字（0~4294967295）;
//void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
//{
//    u8 t,temp;
//    u8 enshow=0;
//    for(t=0;t<len;t++)
//    {
//        temp=(num/oled_pow(10,len-t-1))%10;
//        if(enshow==0&&t<(len-1))
//        {
//            if(temp==0)
//            {
//                OLED_ShowChar(x+(size2/2)*t,y,' ');
//                continue;
//            }else enshow=1;

//        }
//          OLED_ShowChar(x+(size2/2)*t,y,temp+'0');
//    }
//}
//// 显示一个字符串
//void OLED_ShowString(u8 x,u8 y,u8 *chr)
//{
//    unsigned char j=0;
//    while (chr[j]!='\0')
//    {        OLED_ShowChar(x,y,chr[j]);
//            x+=8;
//    if(x>120){x=0;y+=2;}
//            j++;
//    }
//}
//// 显示汉字
//void OLED_ShowCHinese(u8 x,u8 y,u8 no)
//{
//    u8 t,adder=0;
//    OLED_Set_Pos(x,y);
//    for(t=0;t<16;t++)
//        {
//                OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
//                adder+=1;
//     }
//        OLED_Set_Pos(x,y+1);
//    for(t=0;t<16;t++)
//            {
//                OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
//                adder+=1;
//      }
//}

//// 显示int32类型数据（实际显示位数比输入位数多1，用于符号显示）
//void OLED_Show_int32(u8 x,u8 y,int32_t num,u8 len,u8 size2)
//{
//    if(num<0)
//    {
//    OLED_ShowChar(x,y,'-');
//        OLED_ShowNum(x+8,y,-num,len,size2);
//    }
//        else
//        {
//            OLED_ShowNum(x,y,num,len+1,size2);
//        }
//}

//// 显示浮点数
//// len_1为整数位位数，len_2为小数位位数,实际显示位数为len_1+len_2+2
//void OLED_Show_float(u8 x,u8 y,float num,u8 len_1,u8 len_2)
//{
//    if(num<0)
//    {
//    OLED_ShowChar(x,y,'-');
//      OLED_ShowNum(x+8,y,-num,len_1,16);
//    OLED_ShowChar(x+len_1*8+8,y,'.');
//      OLED_ShowNum(x+len_1*8+16,y,-(num-(int32_t)num)*oled_pow(10,len_2),len_2,16);
//    }
//        else
//        {
//            OLED_ShowNum(x,y,num,len_1+1,16);
//            OLED_ShowChar(x+len_1*8+8,y,'.');
//            OLED_ShowNum(x+len_1*8+16,y,(num-(int32_t)num)*oled_pow(10,len_2),len_2,16);
//        }
//}

///***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
//void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
//{
// unsigned int j=0;
// unsigned char x,y;

//  if(y1%8==0) y=y1/8;
//  else y=y1/8+1;
//    for(y=y0;y<y1;y++)
//    {
//        OLED_Set_Pos(x0,y);
//    for(x=x0;x<x1;x++)
//        {
//            OLED_WR_Byte(BMP[j++],OLED_DATA);
//        }
//    }
//}

//// 初始化SSD1306
//void OLED_Init(void)
//{
//  OLED_RST_Set();
//    system_delay_ms(100);
//    OLED_RST_Clr();
//    system_delay_ms(100);
//    OLED_RST_Set();

//    OLED_WR_Byte(0xAE,OLED_CMD);//关闭oled面板
//    OLED_WR_Byte(0x00,OLED_CMD);//设置低列地址
//    OLED_WR_Byte(0x10,OLED_CMD);//设置高列地址
//    OLED_WR_Byte(0x40,OLED_CMD);//设置起始行地址  Set Mapping RAM Display Start Line (0x00~0x3F)
//    OLED_WR_Byte(0x81,OLED_CMD);//设置对比度控制寄存器
//    OLED_WR_Byte(0xCF,OLED_CMD); // 设置SEG输出电流亮度
//    OLED_WR_Byte(0xA1,OLED_CMD);//设置SEG/列映射 0xa0左右反转 0xa1正常
//    OLED_WR_Byte(0xC8,OLED_CMD);//设置COM/行扫描方向 0xc0上下反转 0xc8正常
//    OLED_WR_Byte(0xA6,OLED_CMD);//设置正常显示
//    OLED_WR_Byte(0xA8,OLED_CMD);//设置多路复用率(1 到 64)
//    OLED_WR_Byte(0x3f,OLED_CMD);//1/64占空比
//    OLED_WR_Byte(0xD3,OLED_CMD);//设置显示偏移 Shift Mapping RAM Counter (0x00~0x3F)
//    OLED_WR_Byte(0x00,OLED_CMD);//无偏移
//    OLED_WR_Byte(0xd5,OLED_CMD);//设置显示时钟分频比/振荡器频率
//    OLED_WR_Byte(0x80,OLED_CMD);//设置分频比，设置时钟为100帧/秒
//    OLED_WR_Byte(0xD9,OLED_CMD);//设置预充电周期
//    OLED_WR_Byte(0xF1,OLED_CMD);//设置预充电为15个时钟，放电为1个时钟
//    OLED_WR_Byte(0xDA,OLED_CMD);//设置COM引脚硬件配置
//    OLED_WR_Byte(0x12,OLED_CMD);
//    OLED_WR_Byte(0xDB,OLED_CMD);//设置VCOMH
//    OLED_WR_Byte(0x40,OLED_CMD);//设置VCOM解选电平
//    OLED_WR_Byte(0x20,OLED_CMD);//设置页面寻址模式 (0x00/0x01/0x02)
//    OLED_WR_Byte(0x02,OLED_CMD);
//    OLED_WR_Byte(0x8D,OLED_CMD);//设置电荷泵使能/禁用
//    OLED_WR_Byte(0x14,OLED_CMD);//启用(0x10禁用)
//    OLED_WR_Byte(0xA4,OLED_CMD);//禁用整个显示开启 (0xa4/0xa5)
//    OLED_WR_Byte(0xA6,OLED_CMD);//禁用反相显示开启 (0xa6/a7)
//    OLED_WR_Byte(0xAF,OLED_CMD);//开启oled面板

//    OLED_WR_Byte(0xAF,OLED_CMD); /*显示开启*/
//    OLED_Clear();
//    OLED_Set_Pos(0,0);
//}