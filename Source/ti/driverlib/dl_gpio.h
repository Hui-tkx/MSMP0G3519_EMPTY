/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 * 版权所有 (c) 2020，德州仪器公司 (Texas Instruments Incorporated)
 *
 * 允许在源代码和二进制形式下进行再分发和使用（无论是否经过修改），
 * 但必须满足以下条件：
 *
 * - 源代码的再分发必须保留上述版权声明、本条件列表以及以下免责声明。
 * - 二进制形式的再分发必须在提供的文档和/或其他材料中复制上述版权声明、
 *   本条件列表以及以下免责声明。
 * - 未经事先书面许可，不得使用 Texas Instruments Incorporated 或其
 *   贡献者的名称来支持或推广源自本软件的产品。
 *
 * 本软件由版权持有人和贡献者按“原样”提供，对任何明示或暗示的担保
 * （包括但不限于适销性和特定用途适用性的默示担保）不作任何保证。
 * 在任何情况下，版权持有人或贡献者均不对因使用本软件而导致的任何
 * 直接、间接、偶发、特殊、惩戒性或后果性损害（包括但不限于采购替代
 * 商品或服务；使用损失、数据丢失或利润损失；业务中断等）承担责任，
 * 无论其是基于合同、严格责任还是侵权行为（包括疏忽或其他原因），
 * 即使已被告知可能会发生此类损害。
 */
/*!****************************************************************************
 *  @file       dl_gpio.h
 *  @brief      通用输入/输出 (GPIO) 驱动库
 *  @defgroup   GPIO 通用输入/输出 (GPIO)
 *
 *  @anchor ti_dl_dl_gpio_Overview
 *  # 概述
 *
 *  GPIO 驱动库用于对 MSPM0 的 GPIO 模块进行完整配置。
 *  通过本库可以：
 *    - 配置引脚为输入/输出/外设复用/模拟功能
 *    - 读写引脚电平
 *    - 配置边沿检测、中断、滤波、唤醒等特性
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup GPIO
 * @{
 */
#ifndef ti_dl_dl_gpio__include
#define ti_dl_dl_gpio__include

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_GPIO__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_GPIO_PIN
 *  @{
 *  GPIO 引脚位掩码定义（按位 OR 组合多个引脚）
 */
/*!
 * @brief GPIO 引脚 0
 */
#define DL_GPIO_PIN_0                                               (0x00000001)

/*!
 * @brief GPIO 引脚 1
 */
#define DL_GPIO_PIN_1                                               (0x00000002)

/*!
 * @brief GPIO 引脚 2
 */
#define DL_GPIO_PIN_2                                               (0x00000004)

/*!
 * @brief GPIO 引脚 3
 */
#define DL_GPIO_PIN_3                                               (0x00000008)

/*!
 * @brief GPIO 引脚 4
 */
#define DL_GPIO_PIN_4                                               (0x00000010)

/*!
 * @brief GPIO 引脚 5
 */
#define DL_GPIO_PIN_5                                               (0x00000020)

/*!
 * @brief GPIO 引脚 6
 */
#define DL_GPIO_PIN_6                                               (0x00000040)

/*!
 * @brief GPIO 引脚 7
 */
#define DL_GPIO_PIN_7                                               (0x00000080)

/*!
 * @brief GPIO 引脚 8
 */
#define DL_GPIO_PIN_8                                               (0x00000100)

/*!
 * @brief GPIO 引脚 9
 */
#define DL_GPIO_PIN_9                                               (0x00000200)

/*!
 * @brief GPIO 引脚 10
 */
#define DL_GPIO_PIN_10                                              (0x00000400)

/*!
 * @brief GPIO 引脚 11
 */
#define DL_GPIO_PIN_11                                              (0x00000800)

/*!
 * @brief GPIO 引脚 12
 */
#define DL_GPIO_PIN_12                                              (0x00001000)

/*!
 * @brief GPIO 引脚 13
 */
#define DL_GPIO_PIN_13                                              (0x00002000)

/*!
 * @brief GPIO 引脚 14
 */
#define DL_GPIO_PIN_14                                              (0x00004000)

/*!
 * @brief GPIO 引脚 15
 */
#define DL_GPIO_PIN_15                                              (0x00008000)

/*!
 * @brief GPIO 引脚 16
 */
#define DL_GPIO_PIN_16                                              (0x00010000)

/*!
 * @brief GPIO 引脚 17
 */
#define DL_GPIO_PIN_17                                              (0x00020000)

/*!
 * @brief GPIO 引脚 18
 */
#define DL_GPIO_PIN_18                                              (0x00040000)

/*!
 * @brief GPIO 引脚 19
 */
#define DL_GPIO_PIN_19                                              (0x00080000)

/*!
 * @brief GPIO 引脚 20
 */
#define DL_GPIO_PIN_20                                              (0x00100000)

/*!
 * @brief GPIO 引脚 21
 */
#define DL_GPIO_PIN_21                                              (0x00200000)

/*!
 * @brief GPIO 引脚 22
 */
#define DL_GPIO_PIN_22                                              (0x00400000)

/*!
 * @brief GPIO 引脚 23
 */
#define DL_GPIO_PIN_23                                              (0x00800000)

/*!
 * @brief GPIO 引脚 24
 */
#define DL_GPIO_PIN_24                                              (0x01000000)

/*!
 * @brief GPIO 引脚 25
 */
#define DL_GPIO_PIN_25                                              (0x02000000)

/*!
 * @brief GPIO 引脚 26
 */
#define DL_GPIO_PIN_26                                              (0x04000000)

/*!
 * @brief GPIO 引脚 27
 */
#define DL_GPIO_PIN_27                                              (0x08000000)

/*!
 * @brief GPIO 引脚 28
 */
#define DL_GPIO_PIN_28                                              (0x10000000)

/*!
 * @brief GPIO Pin 29
 */
#define DL_GPIO_PIN_29                                              (0x20000000)

/*!
 * @brief GPIO Pin 30
 */
#define DL_GPIO_PIN_30                                              (0x40000000)

/*!
 * @brief GPIO Pin 31
 */
#define DL_GPIO_PIN_31                                              (0x80000000)

/** @}*/

/** @addtogroup DL_GPIO_EDGE_POLARITY
 *  @{
 *  GPIO 边沿检测极性配置（用于中断/事件）
 */
/*!
 * @brief GPIO Pin 0, disable edge detection
 */
#define DL_GPIO_PIN_0_EDGE_DISABLE              (GPIO_POLARITY15_0_DIO0_DISABLE)

/*!
 * @brief GPIO Pin 0, enable detection on rising edge
 */
#define DL_GPIO_PIN_0_EDGE_RISE                    (GPIO_POLARITY15_0_DIO0_RISE)

/*!
 * @brief GPIO Pin 0, enable detection on falling edge
 */
#define DL_GPIO_PIN_0_EDGE_FALL                    (GPIO_POLARITY15_0_DIO0_FALL)

/*!
 * @brief GPIO Pin 0, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_0_EDGE_RISE_FALL          (GPIO_POLARITY15_0_DIO0_RISE_FALL)

/*!
 * @brief GPIO Pin 1, disable edge detection
 */
#define DL_GPIO_PIN_1_EDGE_DISABLE              (GPIO_POLARITY15_0_DIO1_DISABLE)

/*!
 * @brief GPIO Pin 1, enable detection on rising edge
 */
#define DL_GPIO_PIN_1_EDGE_RISE                    (GPIO_POLARITY15_0_DIO1_RISE)

/*!
 * @brief GPIO Pin 1, enable detection on falling edge
 */
#define DL_GPIO_PIN_1_EDGE_FALL                    (GPIO_POLARITY15_0_DIO1_FALL)

/*!
 * @brief GPIO Pin 1, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_1_EDGE_RISE_FALL          (GPIO_POLARITY15_0_DIO1_RISE_FALL)

/*!
 * @brief GPIO Pin 2, disable edge detection
 */
#define DL_GPIO_PIN_2_EDGE_DISABLE              (GPIO_POLARITY15_0_DIO2_DISABLE)

/*!
 * @brief GPIO Pin 2, enable detection on rising edge
 */
#define DL_GPIO_PIN_2_EDGE_RISE                    (GPIO_POLARITY15_0_DIO2_RISE)

/*!
 * @brief GPIO Pin 2, enable detection on falling edge
 */
#define DL_GPIO_PIN_2_EDGE_FALL                    (GPIO_POLARITY15_0_DIO2_FALL)

/*!
 * @brief GPIO Pin 2, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_2_EDGE_RISE_FALL          (GPIO_POLARITY15_0_DIO2_RISE_FALL)

/*!
 * @brief GPIO Pin 3, disable edge detection
 */
#define DL_GPIO_PIN_3_EDGE_DISABLE              (GPIO_POLARITY15_0_DIO3_DISABLE)

/*!
 * @brief GPIO Pin 3, enable detection on rising edge
 */
#define DL_GPIO_PIN_3_EDGE_RISE                    (GPIO_POLARITY15_0_DIO3_RISE)

/*!
 * @brief GPIO Pin 3, enable detection on falling edge
 */
#define DL_GPIO_PIN_3_EDGE_FALL                    (GPIO_POLARITY15_0_DIO3_FALL)

/*!
 * @brief GPIO Pin 3, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_3_EDGE_RISE_FALL          (GPIO_POLARITY15_0_DIO3_RISE_FALL)

/*!
 * @brief GPIO Pin 4, disable edge detection
 */
#define DL_GPIO_PIN_4_EDGE_DISABLE              (GPIO_POLARITY15_0_DIO4_DISABLE)

/*!
 * @brief GPIO Pin 4, enable detection on rising edge
 */
#define DL_GPIO_PIN_4_EDGE_RISE                    (GPIO_POLARITY15_0_DIO4_RISE)

/*!
 * @brief GPIO Pin 4, enable detection on falling edge
 */
#define DL_GPIO_PIN_4_EDGE_FALL                    (GPIO_POLARITY15_0_DIO4_FALL)

/*!
 * @brief GPIO Pin 4, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_4_EDGE_RISE_FALL          (GPIO_POLARITY15_0_DIO4_RISE_FALL)

/*!
 * @brief GPIO Pin 5, disable edge detection
 */
#define DL_GPIO_PIN_5_EDGE_DISABLE              (GPIO_POLARITY15_0_DIO5_DISABLE)

/*!
 * @brief GPIO Pin 5, enable detection on rising edge
 */
#define DL_GPIO_PIN_5_EDGE_RISE                    (GPIO_POLARITY15_0_DIO5_RISE)

/*!
 * @brief GPIO Pin 5, enable detection on falling edge
 */
#define DL_GPIO_PIN_5_EDGE_FALL                    (GPIO_POLARITY15_0_DIO5_FALL)

/*!
 * @brief GPIO Pin 5, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_5_EDGE_RISE_FALL          (GPIO_POLARITY15_0_DIO5_RISE_FALL)

/*!
 * @brief GPIO Pin 6, disable edge detection
 */
#define DL_GPIO_PIN_6_EDGE_DISABLE              (GPIO_POLARITY15_0_DIO6_DISABLE)

/*!
 * @brief GPIO Pin 6, enable detection on rising edge
 */
#define DL_GPIO_PIN_6_EDGE_RISE                    (GPIO_POLARITY15_0_DIO6_RISE)

/*!
 * @brief GPIO Pin 6, enable detection on falling edge
 */
#define DL_GPIO_PIN_6_EDGE_FALL                    (GPIO_POLARITY15_0_DIO6_FALL)

/*!
 * @brief GPIO Pin 6, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_6_EDGE_RISE_FALL          (GPIO_POLARITY15_0_DIO6_RISE_FALL)

/*!
 * @brief GPIO Pin 7, disable edge detection
 */
#define DL_GPIO_PIN_7_EDGE_DISABLE              (GPIO_POLARITY15_0_DIO7_DISABLE)

/*!
 * @brief GPIO Pin 7, enable detection on rising edge
 */
#define DL_GPIO_PIN_7_EDGE_RISE                    (GPIO_POLARITY15_0_DIO7_RISE)

/*!
 * @brief GPIO Pin 7, enable detection on falling edge
 */
#define DL_GPIO_PIN_7_EDGE_FALL                    (GPIO_POLARITY15_0_DIO7_FALL)

/*!
 * @brief GPIO Pin 7, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_7_EDGE_RISE_FALL          (GPIO_POLARITY15_0_DIO7_RISE_FALL)

/*!
 * @brief GPIO Pin 8, disable edge detection
 */
#define DL_GPIO_PIN_8_EDGE_DISABLE              (GPIO_POLARITY15_0_DIO8_DISABLE)

/*!
 * @brief GPIO Pin 8, enable detection on rising edge
 */
#define DL_GPIO_PIN_8_EDGE_RISE                    (GPIO_POLARITY15_0_DIO8_RISE)

/*!
 * @brief GPIO Pin 8, enable detection on falling edge
 */
#define DL_GPIO_PIN_8_EDGE_FALL                    (GPIO_POLARITY15_0_DIO8_FALL)

/*!
 * @brief GPIO Pin 8, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_8_EDGE_RISE_FALL          (GPIO_POLARITY15_0_DIO8_RISE_FALL)

/*!
 * @brief GPIO Pin 9, disable edge detection
 */
#define DL_GPIO_PIN_9_EDGE_DISABLE              (GPIO_POLARITY15_0_DIO9_DISABLE)

/*!
 * @brief GPIO Pin 9, enable detection on rising edge
 */
#define DL_GPIO_PIN_9_EDGE_RISE                    (GPIO_POLARITY15_0_DIO9_RISE)

/*!
 * @brief GPIO Pin 9, enable detection on falling edge
 */
#define DL_GPIO_PIN_9_EDGE_FALL                    (GPIO_POLARITY15_0_DIO9_FALL)

/*!
 * @brief GPIO Pin 9, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_9_EDGE_RISE_FALL          (GPIO_POLARITY15_0_DIO9_RISE_FALL)

/*!
 * @brief GPIO Pin 10, disable edge detection
 */
#define DL_GPIO_PIN_10_EDGE_DISABLE            (GPIO_POLARITY15_0_DIO10_DISABLE)

/*!
 * @brief GPIO Pin 10, enable detection on rising edge
 */
#define DL_GPIO_PIN_10_EDGE_RISE                  (GPIO_POLARITY15_0_DIO10_RISE)

/*!
 * @brief GPIO Pin 10, enable detection on falling edge
 */
#define DL_GPIO_PIN_10_EDGE_FALL                  (GPIO_POLARITY15_0_DIO10_FALL)

/*!
 * @brief GPIO Pin 10, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_10_EDGE_RISE_FALL        (GPIO_POLARITY15_0_DIO10_RISE_FALL)

/*!
 * @brief GPIO Pin 11, disable edge detection
 */
#define DL_GPIO_PIN_11_EDGE_DISABLE            (GPIO_POLARITY15_0_DIO11_DISABLE)

/*!
 * @brief GPIO Pin 11, enable detection on rising edge
 */
#define DL_GPIO_PIN_11_EDGE_RISE                  (GPIO_POLARITY15_0_DIO11_RISE)

/*!
 * @brief GPIO Pin 11, enable detection on falling edge
 */
#define DL_GPIO_PIN_11_EDGE_FALL                  (GPIO_POLARITY15_0_DIO11_FALL)

/*!
 * @brief GPIO Pin 11, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_11_EDGE_RISE_FALL        (GPIO_POLARITY15_0_DIO11_RISE_FALL)

/*!
 * @brief GPIO Pin 12, disable edge detection
 */
#define DL_GPIO_PIN_12_EDGE_DISABLE            (GPIO_POLARITY15_0_DIO12_DISABLE)

/*!
 * @brief GPIO Pin 12, enable detection on rising edge
 */
#define DL_GPIO_PIN_12_EDGE_RISE                  (GPIO_POLARITY15_0_DIO12_RISE)

/*!
 * @brief GPIO Pin 12, enable detection on falling edge
 */
#define DL_GPIO_PIN_12_EDGE_FALL                  (GPIO_POLARITY15_0_DIO12_FALL)

/*!
 * @brief GPIO Pin 12, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_12_EDGE_RISE_FALL        (GPIO_POLARITY15_0_DIO12_RISE_FALL)

/*!
 * @brief GPIO Pin 13, disable edge detection
 */
#define DL_GPIO_PIN_13_EDGE_DISABLE            (GPIO_POLARITY15_0_DIO13_DISABLE)

/*!
 * @brief GPIO Pin 13, enable detection on rising edge
 */
#define DL_GPIO_PIN_13_EDGE_RISE                  (GPIO_POLARITY15_0_DIO13_RISE)

/*!
 * @brief GPIO Pin 13, enable detection on falling edge
 */
#define DL_GPIO_PIN_13_EDGE_FALL                  (GPIO_POLARITY15_0_DIO13_FALL)

/*!
 * @brief GPIO Pin 13, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_13_EDGE_RISE_FALL        (GPIO_POLARITY15_0_DIO13_RISE_FALL)

/*!
 * @brief GPIO Pin 14, disable edge detection
 */
#define DL_GPIO_PIN_14_EDGE_DISABLE            (GPIO_POLARITY15_0_DIO14_DISABLE)

/*!
 * @brief GPIO Pin 14, enable detection on rising edge
 */
#define DL_GPIO_PIN_14_EDGE_RISE                  (GPIO_POLARITY15_0_DIO14_RISE)

/*!
 * @brief GPIO Pin 14, enable detection on falling edge
 */
#define DL_GPIO_PIN_14_EDGE_FALL                  (GPIO_POLARITY15_0_DIO14_FALL)

/*!
 * @brief GPIO Pin 14, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_14_EDGE_RISE_FALL        (GPIO_POLARITY15_0_DIO14_RISE_FALL)

/*!
 * @brief GPIO Pin 15, disable edge detection
 */
#define DL_GPIO_PIN_15_EDGE_DISABLE            (GPIO_POLARITY15_0_DIO15_DISABLE)

/*!
 * @brief GPIO Pin 15, enable detection on rising edge
 */
#define DL_GPIO_PIN_15_EDGE_RISE                  (GPIO_POLARITY15_0_DIO15_RISE)

/*!
 * @brief GPIO Pin 15, enable detection on falling edge
 */
#define DL_GPIO_PIN_15_EDGE_FALL                  (GPIO_POLARITY15_0_DIO15_FALL)

/*!
 * @brief GPIO Pin 15, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_15_EDGE_RISE_FALL        (GPIO_POLARITY15_0_DIO15_RISE_FALL)

/*!
 * @brief GPIO Pin 16, disable edge detection
 */
#define DL_GPIO_PIN_16_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO16_DISABLE)

/*!
 * @brief GPIO Pin 16, enable detection on rising edge
 */
#define DL_GPIO_PIN_16_EDGE_RISE                 (GPIO_POLARITY31_16_DIO16_RISE)

/*!
 * @brief GPIO Pin 16, enable detection on falling edge
 */
#define DL_GPIO_PIN_16_EDGE_FALL                 (GPIO_POLARITY31_16_DIO16_FALL)

/*!
 * @brief GPIO Pin 16, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_16_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO16_RISE_FALL)

/*!
 * @brief GPIO Pin 17, disable edge detection
 */
#define DL_GPIO_PIN_17_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO17_DISABLE)

/*!
 * @brief GPIO Pin 17, enable detection on rising edge
 */
#define DL_GPIO_PIN_17_EDGE_RISE                 (GPIO_POLARITY31_16_DIO17_RISE)

/*!
 * @brief GPIO Pin 17, enable detection on falling edge
 */
#define DL_GPIO_PIN_17_EDGE_FALL                 (GPIO_POLARITY31_16_DIO17_FALL)

/*!
 * @brief GPIO Pin 17, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_17_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO17_RISE_FALL)

/*!
 * @brief GPIO Pin 18, disable edge detection
 */
#define DL_GPIO_PIN_18_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO18_DISABLE)

/*!
 * @brief GPIO Pin 18, enable detection on rising edge
 */
#define DL_GPIO_PIN_18_EDGE_RISE                 (GPIO_POLARITY31_16_DIO18_RISE)

/*!
 * @brief GPIO Pin 18, enable detection on falling edge
 */
#define DL_GPIO_PIN_18_EDGE_FALL                 (GPIO_POLARITY31_16_DIO18_FALL)

/*!
 * @brief GPIO Pin 18, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_18_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO18_RISE_FALL)

/*!
 * @brief GPIO Pin 19, disable edge detection
 */
#define DL_GPIO_PIN_19_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO19_DISABLE)

/*!
 * @brief GPIO Pin 19, enable detection on rising edge
 */
#define DL_GPIO_PIN_19_EDGE_RISE                 (GPIO_POLARITY31_16_DIO19_RISE)

/*!
 * @brief GPIO Pin 19, enable detection on falling edge
 */
#define DL_GPIO_PIN_19_EDGE_FALL                 (GPIO_POLARITY31_16_DIO19_FALL)

/*!
 * @brief GPIO Pin 19, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_19_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO19_RISE_FALL)

/*!
 * @brief GPIO Pin 20, disable edge detection
 */
#define DL_GPIO_PIN_20_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO20_DISABLE)

/*!
 * @brief GPIO Pin 20, enable detection on rising edge
 */
#define DL_GPIO_PIN_20_EDGE_RISE                 (GPIO_POLARITY31_16_DIO20_RISE)

/*!
 * @brief GPIO Pin 20, enable detection on falling edge
 */
#define DL_GPIO_PIN_20_EDGE_FALL                 (GPIO_POLARITY31_16_DIO20_FALL)

/*!
 * @brief GPIO Pin 20, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_20_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO20_RISE_FALL)

/*!
 * @brief GPIO Pin 21, disable edge detection
 */
#define DL_GPIO_PIN_21_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO21_DISABLE)

/*!
 * @brief GPIO Pin 21, enable detection on rising edge
 */
#define DL_GPIO_PIN_21_EDGE_RISE                 (GPIO_POLARITY31_16_DIO21_RISE)

/*!
 * @brief GPIO Pin 21, enable detection on falling edge
 */
#define DL_GPIO_PIN_21_EDGE_FALL                 (GPIO_POLARITY31_16_DIO21_FALL)

/*!
 * @brief GPIO Pin 21, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_21_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO21_RISE_FALL)

/*!
 * @brief GPIO Pin 22, disable edge detection
 */
#define DL_GPIO_PIN_22_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO22_DISABLE)

/*!
 * @brief GPIO Pin 22, enable detection on rising edge
 */
#define DL_GPIO_PIN_22_EDGE_RISE                 (GPIO_POLARITY31_16_DIO22_RISE)

/*!
 * @brief GPIO Pin 22, enable detection on falling edge
 */
#define DL_GPIO_PIN_22_EDGE_FALL                 (GPIO_POLARITY31_16_DIO22_FALL)

/*!
 * @brief GPIO Pin 22, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_22_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO22_RISE_FALL)

/*!
 * @brief GPIO Pin 23, disable edge detection
 */
#define DL_GPIO_PIN_23_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO23_DISABLE)

/*!
 * @brief GPIO Pin 23, enable detection on rising edge
 */
#define DL_GPIO_PIN_23_EDGE_RISE                 (GPIO_POLARITY31_16_DIO23_RISE)

/*!
 * @brief GPIO Pin 23, enable detection on falling edge
 */
#define DL_GPIO_PIN_23_EDGE_FALL                 (GPIO_POLARITY31_16_DIO23_FALL)

/*!
 * @brief GPIO Pin 23, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_23_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO23_RISE_FALL)

/*!
 * @brief GPIO Pin 24, disable edge detection
 */
#define DL_GPIO_PIN_24_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO24_DISABLE)

/*!
 * @brief GPIO Pin 24, enable detection on rising edge
 */
#define DL_GPIO_PIN_24_EDGE_RISE                 (GPIO_POLARITY31_16_DIO24_RISE)

/*!
 * @brief GPIO Pin 24, enable detection on falling edge
 */
#define DL_GPIO_PIN_24_EDGE_FALL                 (GPIO_POLARITY31_16_DIO24_FALL)

/*!
 * @brief GPIO Pin 24, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_24_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO24_RISE_FALL)

/*!
 * @brief GPIO Pin 25, disable edge detection
 */
#define DL_GPIO_PIN_25_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO25_DISABLE)

/*!
 * @brief GPIO Pin 25, enable detection on rising edge
 */
#define DL_GPIO_PIN_25_EDGE_RISE                 (GPIO_POLARITY31_16_DIO25_RISE)

/*!
 * @brief GPIO Pin 25, enable detection on falling edge
 */
#define DL_GPIO_PIN_25_EDGE_FALL                 (GPIO_POLARITY31_16_DIO25_FALL)

/*!
 * @brief GPIO Pin 25, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_25_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO25_RISE_FALL)

/*!
 * @brief GPIO Pin 26, disable edge detection
 */
#define DL_GPIO_PIN_26_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO26_DISABLE)

/*!
 * @brief GPIO Pin 26, enable detection on rising edge
 */
#define DL_GPIO_PIN_26_EDGE_RISE                 (GPIO_POLARITY31_16_DIO26_RISE)

/*!
 * @brief GPIO Pin 26, enable detection on falling edge
 */
#define DL_GPIO_PIN_26_EDGE_FALL                 (GPIO_POLARITY31_16_DIO26_FALL)

/*!
 * @brief GPIO Pin 26, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_26_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO26_RISE_FALL)

/*!
 * @brief GPIO Pin 27, disable edge detection
 */
#define DL_GPIO_PIN_27_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO27_DISABLE)

/*!
 * @brief GPIO Pin 27, enable detection on rising edge
 */
#define DL_GPIO_PIN_27_EDGE_RISE                 (GPIO_POLARITY31_16_DIO27_RISE)

/*!
 * @brief GPIO Pin 27, enable detection on falling edge
 */
#define DL_GPIO_PIN_27_EDGE_FALL                 (GPIO_POLARITY31_16_DIO27_FALL)

/*!
 * @brief GPIO Pin 27, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_27_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO27_RISE_FALL)

/*!
 * @brief GPIO Pin 28, disable edge detection
 */
#define DL_GPIO_PIN_28_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO28_DISABLE)

/*!
 * @brief GPIO Pin 28, enable detection on rising edge
 */
#define DL_GPIO_PIN_28_EDGE_RISE                 (GPIO_POLARITY31_16_DIO28_RISE)

/*!
 * @brief GPIO Pin 28, enable detection on falling edge
 */
#define DL_GPIO_PIN_28_EDGE_FALL                 (GPIO_POLARITY31_16_DIO28_FALL)

/*!
 * @brief GPIO Pin 28, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_28_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO28_RISE_FALL)

/*!
 * @brief GPIO Pin 29, disable edge detection
 */
#define DL_GPIO_PIN_29_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO29_DISABLE)

/*!
 * @brief GPIO Pin 29, enable detection on rising edge
 */
#define DL_GPIO_PIN_29_EDGE_RISE                 (GPIO_POLARITY31_16_DIO29_RISE)

/*!
 * @brief GPIO Pin 29, enable detection on falling edge
 */
#define DL_GPIO_PIN_29_EDGE_FALL                 (GPIO_POLARITY31_16_DIO29_FALL)

/*!
 * @brief GPIO Pin 29, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_29_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO29_RISE_FALL)

/*!
 * @brief GPIO Pin 30, disable edge detection
 */
#define DL_GPIO_PIN_30_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO30_DISABLE)

/*!
 * @brief GPIO Pin 30, enable detection on rising edge
 */
#define DL_GPIO_PIN_30_EDGE_RISE                 (GPIO_POLARITY31_16_DIO30_RISE)

/*!
 * @brief GPIO Pin 30, enable detection on falling edge
 */
#define DL_GPIO_PIN_30_EDGE_FALL                 (GPIO_POLARITY31_16_DIO30_FALL)

/*!
 * @brief GPIO Pin 30, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_30_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO30_RISE_FALL)

/*!
 * @brief GPIO Pin 31, disable edge detection
 */
#define DL_GPIO_PIN_31_EDGE_DISABLE           (GPIO_POLARITY31_16_DIO31_DISABLE)

/*!
 * @brief GPIO Pin 31, enable detection on rising edge
 */
#define DL_GPIO_PIN_31_EDGE_RISE                 (GPIO_POLARITY31_16_DIO31_RISE)

/*!
 * @brief GPIO Pin 31, enable detection on falling edge
 */
#define DL_GPIO_PIN_31_EDGE_FALL                 (GPIO_POLARITY31_16_DIO31_FALL)

/*!
 * @brief GPIO Pin 31, enable detection on both rising and falling edge
 */
#define DL_GPIO_PIN_31_EDGE_RISE_FALL       (GPIO_POLARITY31_16_DIO31_RISE_FALL)

/** @}*/

/** @addtogroup DL_GPIO_INPUT_FILTER
 *  @{
 *  GPIO 输入滤波配置（去抖/滤除毛刺）
 */
/*!
 * @brief GPIO Pin 0, disable input filter
 */
#define DL_GPIO_PIN_0_INPUT_FILTER_DISABLE      (GPIO_FILTEREN15_0_DIN0_DISABLE)

/*!
 * @brief GPIO Pin 0, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_0_INPUT_FILTER_1_CYCLE    (GPIO_FILTEREN15_0_DIN0_ONE_CYCLE)

/*!
 * @brief GPIO Pin 0, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_0_INPUT_FILTER_3_CYCLES  \
                                            (GPIO_FILTEREN15_0_DIN0_THREE_CYCLE)

/*!
 * @brief GPIO Pin 0, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_0_INPUT_FILTER_8_CYCLES \
                                            (GPIO_FILTEREN15_0_DIN0_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 1, disable input filter
 */
#define DL_GPIO_PIN_1_INPUT_FILTER_DISABLE      (GPIO_FILTEREN15_0_DIN1_DISABLE)

/*!
 * @brief GPIO Pin 1, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_1_INPUT_FILTER_1_CYCLE    (GPIO_FILTEREN15_0_DIN1_ONE_CYCLE)

/*!
 * @brief GPIO Pin 1, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_1_INPUT_FILTER_3_CYCLES  \
                                            (GPIO_FILTEREN15_0_DIN1_THREE_CYCLE)

/*!
 * @brief GPIO Pin 1, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_1_INPUT_FILTER_8_CYCLES \
                                            (GPIO_FILTEREN15_0_DIN1_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 2, disable input filter
 */
#define DL_GPIO_PIN_2_INPUT_FILTER_DISABLE      (GPIO_FILTEREN15_0_DIN2_DISABLE)

/*!
 * @brief GPIO Pin 2, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_2_INPUT_FILTER_1_CYCLE    (GPIO_FILTEREN15_0_DIN2_ONE_CYCLE)

/*!
 * @brief GPIO Pin 2, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_2_INPUT_FILTER_3_CYCLES  \
                                            (GPIO_FILTEREN15_0_DIN2_THREE_CYCLE)

/*!
 * @brief GPIO Pin 2, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_2_INPUT_FILTER_8_CYCLES \
                                            (GPIO_FILTEREN15_0_DIN2_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 3, disable input filter
 */
#define DL_GPIO_PIN_3_INPUT_FILTER_DISABLE      (GPIO_FILTEREN15_0_DIN3_DISABLE)

/*!
 * @brief GPIO Pin 3, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_3_INPUT_FILTER_1_CYCLE    (GPIO_FILTEREN15_0_DIN3_ONE_CYCLE)

/*!
 * @brief GPIO Pin 3, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_3_INPUT_FILTER_3_CYCLES  \
                                            (GPIO_FILTEREN15_0_DIN3_THREE_CYCLE)

/*!
 * @brief GPIO Pin 3, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_3_INPUT_FILTER_8_CYCLES \
                                            (GPIO_FILTEREN15_0_DIN3_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 4, disable input filter
 */
#define DL_GPIO_PIN_4_INPUT_FILTER_DISABLE      (GPIO_FILTEREN15_0_DIN4_DISABLE)

/*!
 * @brief GPIO Pin 4, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_4_INPUT_FILTER_1_CYCLE    (GPIO_FILTEREN15_0_DIN4_ONE_CYCLE)

/*!
 * @brief GPIO Pin 4, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_4_INPUT_FILTER_3_CYCLES  \
                                            (GPIO_FILTEREN15_0_DIN4_THREE_CYCLE)

/*!
 * @brief GPIO Pin 4, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_4_INPUT_FILTER_8_CYCLES \
                                            (GPIO_FILTEREN15_0_DIN4_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 5, disable input filter
 */
#define DL_GPIO_PIN_5_INPUT_FILTER_DISABLE      (GPIO_FILTEREN15_0_DIN5_DISABLE)

/*!
 * @brief GPIO Pin 5, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_5_INPUT_FILTER_1_CYCLE    (GPIO_FILTEREN15_0_DIN5_ONE_CYCLE)

/*!
 * @brief GPIO Pin 5, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_5_INPUT_FILTER_3_CYCLES  \
                                            (GPIO_FILTEREN15_0_DIN5_THREE_CYCLE)

/*!
 * @brief GPIO Pin 5, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_5_INPUT_FILTER_8_CYCLES \
                                            (GPIO_FILTEREN15_0_DIN5_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 6, disable input filter
 */
#define DL_GPIO_PIN_6_INPUT_FILTER_DISABLE      (GPIO_FILTEREN15_0_DIN6_DISABLE)

/*!
 * @brief GPIO Pin 6, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_6_INPUT_FILTER_1_CYCLE    (GPIO_FILTEREN15_0_DIN6_ONE_CYCLE)

/*!
 * @brief GPIO Pin 6, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_6_INPUT_FILTER_3_CYCLES  \
                                            (GPIO_FILTEREN15_0_DIN6_THREE_CYCLE)

/*!
 * @brief GPIO Pin 6, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_6_INPUT_FILTER_8_CYCLES \
                                            (GPIO_FILTEREN15_0_DIN6_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 7, disable input filter
 */
#define DL_GPIO_PIN_7_INPUT_FILTER_DISABLE      (GPIO_FILTEREN15_0_DIN7_DISABLE)

/*!
 * @brief GPIO Pin 7, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_7_INPUT_FILTER_1_CYCLE    (GPIO_FILTEREN15_0_DIN7_ONE_CYCLE)

/*!
 * @brief GPIO Pin 7, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_7_INPUT_FILTER_3_CYCLES  \
                                            (GPIO_FILTEREN15_0_DIN7_THREE_CYCLE)

/*!
 * @brief GPIO Pin 7, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_7_INPUT_FILTER_8_CYCLES \
                                            (GPIO_FILTEREN15_0_DIN7_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 8, disable input filter
 */
#define DL_GPIO_PIN_8_INPUT_FILTER_DISABLE      (GPIO_FILTEREN15_0_DIN8_DISABLE)

/*!
 * @brief GPIO Pin 8, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_8_INPUT_FILTER_1_CYCLE    (GPIO_FILTEREN15_0_DIN8_ONE_CYCLE)

/*!
 * @brief GPIO Pin 8, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_8_INPUT_FILTER_3_CYCLES  \
                                            (GPIO_FILTEREN15_0_DIN8_THREE_CYCLE)

/*!
 * @brief GPIO Pin 8, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_8_INPUT_FILTER_8_CYCLES \
                                            (GPIO_FILTEREN15_0_DIN8_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 9, disable input filter
 */
#define DL_GPIO_PIN_9_INPUT_FILTER_DISABLE      (GPIO_FILTEREN15_0_DIN9_DISABLE)

/*!
 * @brief GPIO Pin 9, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_9_INPUT_FILTER_1_CYCLE    (GPIO_FILTEREN15_0_DIN9_ONE_CYCLE)

/*!
 * @brief GPIO Pin 9, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_9_INPUT_FILTER_3_CYCLES  \
                                            (GPIO_FILTEREN15_0_DIN9_THREE_CYCLE)

/*!
 * @brief GPIO Pin 9, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_9_INPUT_FILTER_8_CYCLES \
                                            (GPIO_FILTEREN15_0_DIN9_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 10, disable input filter
 */
#define DL_GPIO_PIN_10_INPUT_FILTER_DISABLE     (GPIO_FILTEREN15_0_DIN10_DISABLE)

/*!
 * @brief GPIO Pin 10, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_10_INPUT_FILTER_1_CYCLE   (GPIO_FILTEREN15_0_DIN10_ONE_CYCLE)

/*!
 * @brief GPIO Pin 10, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_10_INPUT_FILTER_3_CYCLES  \
                                           (GPIO_FILTEREN15_0_DIN10_THREE_CYCLE)

/*!
 * @brief GPIO Pin 10, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_10_INPUT_FILTER_8_CYCLES \
                                           (GPIO_FILTEREN15_0_DIN10_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 11, disable input filter
 */
#define DL_GPIO_PIN_11_INPUT_FILTER_DISABLE     (GPIO_FILTEREN15_0_DIN11_DISABLE)

/*!
 * @brief GPIO Pin 11, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_11_INPUT_FILTER_1_CYCLE   (GPIO_FILTEREN15_0_DIN11_ONE_CYCLE)

/*!
 * @brief GPIO Pin 11, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_11_INPUT_FILTER_3_CYCLES  \
                                           (GPIO_FILTEREN15_0_DIN11_THREE_CYCLE)

/*!
 * @brief GPIO Pin 11, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_11_INPUT_FILTER_8_CYCLES \
                                           (GPIO_FILTEREN15_0_DIN11_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 12, disable input filter
 */
#define DL_GPIO_PIN_12_INPUT_FILTER_DISABLE    (GPIO_FILTEREN15_0_DIN12_DISABLE)

/*!
 * @brief GPIO Pin 12, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_12_INPUT_FILTER_1_CYCLE  (GPIO_FILTEREN15_0_DIN12_ONE_CYCLE)

/*!
 * @brief GPIO Pin 12, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_12_INPUT_FILTER_3_CYCLES  \
                                           (GPIO_FILTEREN15_0_DIN12_THREE_CYCLE)

/*!
 * @brief GPIO Pin 12, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_12_INPUT_FILTER_8_CYCLES \
                                           (GPIO_FILTEREN15_0_DIN12_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 13, disable input filter
 */
#define DL_GPIO_PIN_13_INPUT_FILTER_DISABLE    (GPIO_FILTEREN15_0_DIN13_DISABLE)

/*!
 * @brief GPIO Pin 13, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_13_INPUT_FILTER_1_CYCLE  (GPIO_FILTEREN15_0_DIN13_ONE_CYCLE)

/*!
 * @brief GPIO Pin 13, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_13_INPUT_FILTER_3_CYCLES  \
                                           (GPIO_FILTEREN15_0_DIN13_THREE_CYCLE)

/*!
 * @brief GPIO Pin 13, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_13_INPUT_FILTER_8_CYCLES \
                                           (GPIO_FILTEREN15_0_DIN13_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 14, disable input filter
 */
#define DL_GPIO_PIN_14_INPUT_FILTER_DISABLE    (GPIO_FILTEREN15_0_DIN14_DISABLE)

/*!
 * @brief GPIO Pin 14, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_14_INPUT_FILTER_1_CYCLE  (GPIO_FILTEREN15_0_DIN14_ONE_CYCLE)

/*!
 * @brief GPIO Pin 14, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_14_INPUT_FILTER_3_CYCLES  \
                                           (GPIO_FILTEREN15_0_DIN14_THREE_CYCLE)

/*!
 * @brief GPIO Pin 14, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_14_INPUT_FILTER_8_CYCLES \
                                           (GPIO_FILTEREN15_0_DIN14_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 15, disable input filter
 */
#define DL_GPIO_PIN_15_INPUT_FILTER_DISABLE    (GPIO_FILTEREN15_0_DIN15_DISABLE)

/*!
 * @brief GPIO Pin 15, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_15_INPUT_FILTER_1_CYCLE  (GPIO_FILTEREN15_0_DIN15_ONE_CYCLE)

/*!
 * @brief GPIO Pin 15, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_15_INPUT_FILTER_3_CYCLES  \
                                           (GPIO_FILTEREN15_0_DIN15_THREE_CYCLE)

/*!
 * @brief GPIO Pin 15, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_15_INPUT_FILTER_8_CYCLES \
                                           (GPIO_FILTEREN15_0_DIN15_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 16, disable input filter
 */
#define DL_GPIO_PIN_16_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN16_DISABLE)

/*!
 * @brief GPIO Pin 16, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_16_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN16_ONE_CYCLE)

/*!
 * @brief GPIO Pin 16, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_16_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN16_THREE_CYCLE)

/*!
 * @brief GPIO Pin 16, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_16_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN16_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 17, disable input filter
 */
#define DL_GPIO_PIN_17_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN17_DISABLE)

/*!
 * @brief GPIO Pin 17, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_17_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN17_ONE_CYCLE)

/*!
 * @brief GPIO Pin 17, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_17_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN17_THREE_CYCLE)

/*!
 * @brief GPIO Pin 17, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_17_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN17_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 18, disable input filter
 */
#define DL_GPIO_PIN_18_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN18_DISABLE)

/*!
 * @brief GPIO Pin 18, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_18_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN18_ONE_CYCLE)

/*!
 * @brief GPIO Pin 18, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_18_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN18_THREE_CYCLE)

/*!
 * @brief GPIO Pin 18, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_18_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN18_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 19, disable input filter
 */
#define DL_GPIO_PIN_19_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN19_DISABLE)

/*!
 * @brief GPIO Pin 19, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_19_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN19_ONE_CYCLE)

/*!
 * @brief GPIO Pin 19, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_19_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN19_THREE_CYCLE)

/*!
 * @brief GPIO Pin 19, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_19_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN19_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 20, disable input filter
 */
#define DL_GPIO_PIN_20_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN20_DISABLE)

/*!
 * @brief GPIO Pin 20, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_20_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN20_ONE_CYCLE)

/*!
 * @brief GPIO Pin 20, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_20_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN20_THREE_CYCLE)

/*!
 * @brief GPIO Pin 20, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_20_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN20_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 21, disable input filter
 */
#define DL_GPIO_PIN_21_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN21_DISABLE)

/*!
 * @brief GPIO Pin 21, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_21_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN21_ONE_CYCLE)

/*!
 * @brief GPIO Pin 21, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_21_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN21_THREE_CYCLE)

/*!
 * @brief GPIO Pin 21, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_21_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN21_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 22, disable input filter
 */
#define DL_GPIO_PIN_22_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN22_DISABLE)

/*!
 * @brief GPIO Pin 22, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_22_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN22_ONE_CYCLE)

/*!
 * @brief GPIO Pin 22, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_22_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN22_THREE_CYCLE)

/*!
 * @brief GPIO Pin 22, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_22_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN22_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 23, disable input filter
 */
#define DL_GPIO_PIN_23_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN23_DISABLE)

/*!
 * @brief GPIO Pin 23, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_23_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN23_ONE_CYCLE)

/*!
 * @brief GPIO Pin 23, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_23_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN23_THREE_CYCLE)

/*!
 * @brief GPIO Pin 23, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_23_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN23_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 24, disable input filter
 */
#define DL_GPIO_PIN_24_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN24_DISABLE)

/*!
 * @brief GPIO Pin 24, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_24_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN24_ONE_CYCLE)

/*!
 * @brief GPIO Pin 24, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_24_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN24_THREE_CYCLE)

/*!
 * @brief GPIO Pin 24, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_24_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN24_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 25, disable input filter
 */
#define DL_GPIO_PIN_25_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN25_DISABLE)

/*!
 * @brief GPIO Pin 25, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_25_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN25_ONE_CYCLE)

/*!
 * @brief GPIO Pin 25, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_25_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN25_THREE_CYCLE)

/*!
 * @brief GPIO Pin 25, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_25_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN25_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 26, disable input filter
 */
#define DL_GPIO_PIN_26_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN26_DISABLE)

/*!
 * @brief GPIO Pin 26, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_26_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN26_ONE_CYCLE)

/*!
 * @brief GPIO Pin 26, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_26_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN26_THREE_CYCLE)

/*!
 * @brief GPIO Pin 26, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_26_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN26_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 27, disable input filter
 */
#define DL_GPIO_PIN_27_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN27_DISABLE)

/*!
 * @brief GPIO Pin 27, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_27_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN27_ONE_CYCLE)

/*!
 * @brief GPIO Pin 27, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_27_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN27_THREE_CYCLE)

/*!
 * @brief GPIO Pin 27, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_27_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN27_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 28, disable input filter
 */
#define DL_GPIO_PIN_28_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN28_DISABLE)

/*!
 * @brief GPIO Pin 28, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_28_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN28_ONE_CYCLE)

/*!
 * @brief GPIO Pin 28, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_28_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN28_THREE_CYCLE)

/*!
 * @brief GPIO Pin 28, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_28_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN28_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 29, disable input filter
 */
#define DL_GPIO_PIN_29_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN29_DISABLE)

/*!
 * @brief GPIO Pin 29, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_29_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN29_ONE_CYCLE)

/*!
 * @brief GPIO Pin 29, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_29_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN29_THREE_CYCLE)

/*!
 * @brief GPIO Pin 29, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_29_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN29_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 30, disable input filter
 */
#define DL_GPIO_PIN_30_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN30_DISABLE)

/*!
 * @brief GPIO Pin 30, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_30_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN30_ONE_CYCLE)

/*!
 * @brief GPIO Pin 30, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_30_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN30_THREE_CYCLE)

/*!
 * @brief GPIO Pin 30, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_30_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN30_EIGHT_CYCLE)

/*!
 * @brief GPIO Pin 31, disable input filter
 */
#define DL_GPIO_PIN_31_INPUT_FILTER_DISABLE   (GPIO_FILTEREN31_16_DIN31_DISABLE)

/*!
 * @brief GPIO Pin 31, enable input filter to 1 ulpclk period
 */
#define DL_GPIO_PIN_31_INPUT_FILTER_1_CYCLE (GPIO_FILTEREN31_16_DIN31_ONE_CYCLE)

/*!
 * @brief GPIO Pin 31, enable input filter to 3 ulpclk periods
 */
#define DL_GPIO_PIN_31_INPUT_FILTER_3_CYCLES  \
                                          (GPIO_FILTEREN31_16_DIN31_THREE_CYCLE)

/*!
 * @brief GPIO Pin 31, enable input filter to 8 ulpclk periods
 */
#define DL_GPIO_PIN_31_INPUT_FILTER_8_CYCLES \
                                          (GPIO_FILTEREN31_16_DIN31_EIGHT_CYCLE)

/** @}*/

/* clang-format on */

/*! @enum DL_GPIO_INVERSION
 *  @brief GPIO 引脚输入/输出电平反相配置
 */
typedef enum {
    /*! Enable Inversion */
    DL_GPIO_INVERSION_ENABLE = IOMUX_PINCM_INV_ENABLE,
    /*! Disable Inversion */
    DL_GPIO_INVERSION_DISABLE = IOMUX_PINCM_INV_DISABLE,
} DL_GPIO_INVERSION;

/*! @enum DL_GPIO_DRIVE_STRENGTH
 *  @brief GPIO 输出驱动能力配置（低/高驱动）
 */
typedef enum {
    /*! Disable High-Drive Strength */
    DL_GPIO_DRIVE_STRENGTH_LOW = IOMUX_PINCM_DRV_DRVVAL0,
    /*! Enable High-Drive (where allowed) */
    DL_GPIO_DRIVE_STRENGTH_HIGH = IOMUX_PINCM_DRV_DRVVAL1,
} DL_GPIO_DRIVE_STRENGTH;

/*! @enum DL_GPIO_RESISTOR
 *  @brief GPIO 内部上下拉电阻配置
 */
typedef enum {
    /*! Use no pull up/pull down resistors on pin */
    DL_GPIO_RESISTOR_NONE =
        (IOMUX_PINCM_PIPU_DISABLE | IOMUX_PINCM_PIPD_DISABLE),

    /*! Use a pull up resistor on the pin */
    DL_GPIO_RESISTOR_PULL_UP =
        (IOMUX_PINCM_PIPU_ENABLE | IOMUX_PINCM_PIPD_DISABLE),

    /*! Use a pull down resistor on the pin */
    DL_GPIO_RESISTOR_PULL_DOWN =
        (IOMUX_PINCM_PIPU_DISABLE | IOMUX_PINCM_PIPD_ENABLE)
} DL_GPIO_RESISTOR;

/*! @enum DL_GPIO_HYSTERESIS
 *  @brief GPIO 输入迟滞特性配置（抑制抖动）
 */
typedef enum {
    /*! Enable Hysteresis on pin */
    DL_GPIO_HYSTERESIS_ENABLE = IOMUX_PINCM_HYSTEN_ENABLE,
    /*! Disable Hysteresis on pin */
    DL_GPIO_HYSTERESIS_DISABLE = IOMUX_PINCM_HYSTEN_DISABLE,
} DL_GPIO_HYSTERESIS;

/*! @enum DL_GPIO_WAKEUP
 *  @brief GPIO 唤醒配置（允许/禁止及电平条件）
 */
typedef enum {
    /*! Wakeup enabled */
    DL_GPIO_WAKEUP_ENABLE = IOMUX_PINCM_WUEN_ENABLE,
    /*! Wakeup disabled */
    DL_GPIO_WAKEUP_DISABLE = IOMUX_PINCM_WUEN_DISABLE,
    /*! Wakeup when pin changes to 0 */
    DL_GPIO_WAKEUP_ON_0 = (IOMUX_PINCM_WUEN_ENABLE | IOMUX_PINCM_WCOMP_MATCH0),
    /*! Wakeup when pin changes to 1 */
    DL_GPIO_WAKEUP_ON_1 = (IOMUX_PINCM_WUEN_ENABLE | IOMUX_PINCM_WCOMP_MATCH1),
} DL_GPIO_WAKEUP;

/*! @enum DL_GPIO_WAKEUP_COMPARE_VALUE
 *  @brief GPIO 唤醒比较参考电平
 */
typedef enum {
    /*! Wakeup compare value of 0 */
    DL_GPIO_WAKEUP_COMPARE_VALUE_0 = IOMUX_PINCM_WCOMP_MATCH0,
    /*! Wakeup compare value of 1 */
    DL_GPIO_WAKEUP_COMPARE_VALUE_1 = IOMUX_PINCM_WCOMP_MATCH1,
} DL_GPIO_WAKEUP_COMPARE_VALUE;

/*! @enum DL_GPIO_HIZ
 *  @brief GPIO 高阻状态配置
 */
typedef enum {
    /*! Enable Hi-Z on pin */
    DL_GPIO_HIZ_ENABLE = IOMUX_PINCM_HIZ1_ENABLE,
    /*! Disable Hi-Z on pin */
    DL_GPIO_HIZ_DISABLE = IOMUX_PINCM_HIZ1_DISABLE,
} DL_GPIO_HIZ;

/** @enum DL_GPIO_EVENT_ROUTE
 *  @brief GPIO 事件路由选择（事件输出通道）
 */
typedef enum {
    /*! GPIO event route 1 */
    DL_GPIO_EVENT_ROUTE_1 = 0,
    /*! GPIO event route 2 */
    DL_GPIO_EVENT_ROUTE_2 = 12
} DL_GPIO_EVENT_ROUTE;

/** @enum DL_GPIO_PUBLISHER_INDEX
 *  @brief GPIO 事件发布者索引
 */
typedef enum {
    /*! GPIO Publisher index 0 */
    DL_GPIO_PUBLISHER_INDEX_0 = 0,
    /*! GPIO Publisher index 1 */
    DL_GPIO_PUBLISHER_INDEX_1 = 1
} DL_GPIO_PUBLISHER_INDEX;

/** @enum DL_GPIO_SUBSCRIBER_INDEX
 *  @brief GPIO 事件订阅者索引
 */
typedef enum {
    /*! GPIO Subscriber index 0 */
    DL_GPIO_SUBSCRIBER_INDEX_0 = 0,
    /*! GPIO Subscriber index 1 */
    DL_GPIO_SUBSCRIBER_INDEX_1 = 1
} DL_GPIO_SUBSCRIBER_INDEX;

/** @enum DL_GPIO_SUBSCRIBER_OUT_POLICY
 *  @brief GPIO 订阅事件后的输出动作策略
 */
typedef enum {
    /*! GPIO is set */
    DL_GPIO_SUBSCRIBER_OUT_POLICY_SET = GPIO_SUB0CFG_OUTPOLICY_SET,
    /*! GPIO is cleared */
    DL_GPIO_SUBSCRIBER_OUT_POLICY_CLEAR = GPIO_SUB0CFG_OUTPOLICY_CLR,
    /*! GPIO is toggled */
    DL_GPIO_SUBSCRIBER_OUT_POLICY_TOGGLE = GPIO_SUB0CFG_OUTPOLICY_TOGGLE
} DL_GPIO_SUBSCRIBER_OUT_POLICY;

/** @enum DL_GPIO_SUBSCRIBERx_PIN
 *  @brief GPIO 订阅者对应的实际 DIO 引脚选择
 */
typedef enum {
    /*! Selects DIO 0 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_0 = 0x00000000,
    /*! Selects DIO 1 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_1 = 0x00010000,
    /*! Selects DIO 2 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_2 = 0x00020000,
    /*! Selects DIO 3 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_3 = 0x00030000,
    /*! Selects DIO 4 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_4 = 0x00040000,
    /*! Selects DIO 5 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_5 = 0x00050000,
    /*! Selects DIO 6 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_6 = 0x00060000,
    /*! Selects DIO 7 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_7 = 0x00070000,
    /*! Selects DIO 8 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_8 = 0x00080000,
    /*! Selects DIO 9 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_9 = 0x00090000,
    /*! Selects DIO 10 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_10 = 0x000A0000,
    /*! Selects DIO 11 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_11 = 0x000B0000,
    /*! Selects DIO 12 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_12 = 0x000C0000,
    /*! Selects DIO 13 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_13 = 0x000D0000,
    /*! Selects DIO 14 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_14 = 0x000E0000,
    /*! Selects DIO 15 when Subscriber index 0 is selected */
    DL_GPIO_SUBSCRIBER0_PIN_15 = 0x000F0000,
    /*! Selects DIO 16 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_16 = 0x00000000,
    /*! Selects DIO 17 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_17 = 0x00010000,
    /*! Selects DIO 18 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_18 = 0x00020000,
    /*! Selects DIO 19 when Subscriber index 1  is selected */
    DL_GPIO_SUBSCRIBER1_PIN_19 = 0x00030000,
    /*! Selects DIO 20 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_20 = 0x00040000,
    /*! Selects DIO 21 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_21 = 0x00050000,
    /*! Selects DIO 22 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_22 = 0x00060000,
    /*! Selects DIO 23 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_23 = 0x00070000,
    /*! Selects DIO 24 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_24 = 0x00080000,
    /*! Selects DIO 25 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_25 = 0x00090000,
    /*! Selects DIO 26 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_26 = 0x000A0000,
    /*! Selects DIO 27 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_27 = 0x000B0000,
    /*! Selects DIO 28 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_28 = 0x000C0000,
    /*! Selects DIO 29 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_29 = 0x000D0000,
    /*! Selects DIO 30 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_30 = 0x000E0000,
    /*! Selects DIO 31 when Subscriber index 1 is selected */
    DL_GPIO_SUBSCRIBER1_PIN_31 = 0x000F0000
} DL_GPIO_SUBSCRIBERx_PIN;

/*! @enum DL_GPIO_IIDX
 *  @brief GPIO 单个引脚中断索引（读取 IIDX 使用）
 */
typedef enum {
    /*! Interrupt index for no interrupt  */
    DL_GPIO_IIDX_NO_INTR = GPIO_CPU_INT_IIDX_STAT_NO_INTR,
    /*! Interrupt index for GPIO DIO0 */
    DL_GPIO_IIDX_DIO0 = GPIO_CPU_INT_IIDX_STAT_DIO0,
    /*! Interrupt index for GPIO DIO1 */
    DL_GPIO_IIDX_DIO1 = GPIO_CPU_INT_IIDX_STAT_DIO1,
    /*! Interrupt index for GPIO DIO2 */
    DL_GPIO_IIDX_DIO2 = GPIO_CPU_INT_IIDX_STAT_DIO2,
    /*! Interrupt index for GPIO DIO3 */
    DL_GPIO_IIDX_DIO3 = GPIO_CPU_INT_IIDX_STAT_DIO3,
    /*! Interrupt index for GPIO DIO4 */
    DL_GPIO_IIDX_DIO4 = GPIO_CPU_INT_IIDX_STAT_DIO4,
    /*! Interrupt index for GPIO DIO5 */
    DL_GPIO_IIDX_DIO5 = GPIO_CPU_INT_IIDX_STAT_DIO5,
    /*! Interrupt index for GPIO DIO6 */
    DL_GPIO_IIDX_DIO6 = GPIO_CPU_INT_IIDX_STAT_DIO6,
    /*! Interrupt index for GPIO DIO7 */
    DL_GPIO_IIDX_DIO7 = GPIO_CPU_INT_IIDX_STAT_DIO7,
    /*! Interrupt index for GPIO DIO8 */
    DL_GPIO_IIDX_DIO8 = GPIO_CPU_INT_IIDX_STAT_DIO8,
    /*! Interrupt index for GPIO DIO9 */
    DL_GPIO_IIDX_DIO9 = GPIO_CPU_INT_IIDX_STAT_DIO9,
    /*! Interrupt index for GPIO DIO10 */
    DL_GPIO_IIDX_DIO10 = GPIO_CPU_INT_IIDX_STAT_DIO10,
    /*! Interrupt index for GPIO DIO11 */
    DL_GPIO_IIDX_DIO11 = GPIO_CPU_INT_IIDX_STAT_DIO11,
    /*! Interrupt index for GPIO DIO12 */
    DL_GPIO_IIDX_DIO12 = GPIO_CPU_INT_IIDX_STAT_DIO12,
    /*! Interrupt index for GPIO DIO13 */
    DL_GPIO_IIDX_DIO13 = GPIO_CPU_INT_IIDX_STAT_DIO13,
    /*! Interrupt index for GPIO DIO14 */
    DL_GPIO_IIDX_DIO14 = GPIO_CPU_INT_IIDX_STAT_DIO14,
    /*! Interrupt index for GPIO DIO15 */
    DL_GPIO_IIDX_DIO15 = GPIO_CPU_INT_IIDX_STAT_DIO15,
    /*! Interrupt index for GPIO DIO16 */
    DL_GPIO_IIDX_DIO16 = GPIO_CPU_INT_IIDX_STAT_DIO16,
    /*! Interrupt index for GPIO DIO17 */
    DL_GPIO_IIDX_DIO17 = GPIO_CPU_INT_IIDX_STAT_DIO17,
    /*! Interrupt index for GPIO DIO18 */
    DL_GPIO_IIDX_DIO18 = GPIO_CPU_INT_IIDX_STAT_DIO18,
    /*! Interrupt index for GPIO DIO19 */
    DL_GPIO_IIDX_DIO19 = GPIO_CPU_INT_IIDX_STAT_DIO19,
    /*! Interrupt index for GPIO DIO20 */
    DL_GPIO_IIDX_DIO20 = GPIO_CPU_INT_IIDX_STAT_DIO20,
    /*! Interrupt index for GPIO DIO21 */
    DL_GPIO_IIDX_DIO21 = GPIO_CPU_INT_IIDX_STAT_DIO21,
    /*! Interrupt index for GPIO DIO22 */
    DL_GPIO_IIDX_DIO22 = GPIO_CPU_INT_IIDX_STAT_DIO22,
    /*! Interrupt index for GPIO DIO23 */
    DL_GPIO_IIDX_DIO23 = GPIO_CPU_INT_IIDX_STAT_DIO23,
    /*! Interrupt index for GPIO DIO24 */
    DL_GPIO_IIDX_DIO24 = GPIO_CPU_INT_IIDX_STAT_DIO24,
    /*! Interrupt index for GPIO DIO25 */
    DL_GPIO_IIDX_DIO25 = GPIO_CPU_INT_IIDX_STAT_DIO25,
    /*! Interrupt index for GPIO DIO26 */
    DL_GPIO_IIDX_DIO26 = GPIO_CPU_INT_IIDX_STAT_DIO26,
    /*! Interrupt index for GPIO DIO27 */
    DL_GPIO_IIDX_DIO27 = GPIO_CPU_INT_IIDX_STAT_DIO27,
    /*! Interrupt index for GPIO DIO28 */
    DL_GPIO_IIDX_DIO28 = GPIO_CPU_INT_IIDX_STAT_DIO28,
    /*! Interrupt index for GPIO DIO29 */
    DL_GPIO_IIDX_DIO29 = GPIO_CPU_INT_IIDX_STAT_DIO29,
    /*! Interrupt index for GPIO DIO30 */
    DL_GPIO_IIDX_DIO30 = GPIO_CPU_INT_IIDX_STAT_DIO30,
    /*! Interrupt index for GPIO DIO31 */
    DL_GPIO_IIDX_DIO31 = GPIO_CPU_INT_IIDX_STAT_DIO31
} DL_GPIO_IIDX;

/**
 * @brief Enables the Peripheral Write Enable (PWREN) register for the GPIO
 *        打开 GPIO 外设写使能（PWREN），允许访问 GPIO 寄存器
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *
 * @param gpio        Pointer to the register overlay for the peripheral
 *                    指向 GPIO 寄存器结构体的指针
 */
__STATIC_INLINE void DL_GPIO_enablePower(GPIO_Regs* gpio)
{
    gpio->GPRCM.PWREN = (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Disables the Peripheral Write Enable (PWREN) register for the GPIO
 *        关闭 GPIO 外设写使能（PWREN），禁止访问 GPIO 寄存器
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *
 *  @note This API does not provide large power savings.
 *
 * @param gpio        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_GPIO_disablePower(GPIO_Regs* gpio)
{
    gpio->GPRCM.PWREN = (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_DISABLE);
}

/**
 * @brief Returns if the Peripheral Write Enable (PWREN) register for the GPIO
 *        is enabled
 *        查询 GPIO 外设写使能（PWREN）当前是否开启
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *
 * @param gpio        Pointer to the register overlay for the peripheral
 *
 * @return true  允许访问 GPIO 寄存器
 * @return false 禁止访问 GPIO 寄存器
 */
__STATIC_INLINE bool DL_GPIO_isPowerEnabled(GPIO_Regs* gpio)
{
    return ((gpio->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK) ==
            GPIO_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Resets gpio peripheral
 *        对 GPIO 外设进行复位
 *
 * @param gpio        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_GPIO_reset(GPIO_Regs* gpio)
{
    gpio->GPRCM.RSTCTL =
        (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETSTKYCLR_CLR |
            GPIO_RSTCTL_RESETASSERT_ASSERT);
}

/**
 * @brief Returns if gpio peripheral was reset
 *        查询 GPIO 外设是否发生过复位（复位标志）
 *
 * @param gpio        Pointer to the register overlay for the peripheral
 *
 * @return true  如果外设自上次清除标志后被复位
 * @return false 如果外设未被复位
 *
 */
__STATIC_INLINE bool DL_GPIO_isReset(GPIO_Regs* gpio)
{
    return ((gpio->GPRCM.STAT & GPIO_STAT_RESETSTKY_MASK) ==
            GPIO_STAT_RESETSTKY_RESET);
}

/**
 *  @brief      Configures a pin as a basic GPIO output
 *              将对应引脚配置为最基本的数字输出（推挽输出，无上下拉等附加配置）
 *
 *  @param[in]  pincmIndex  The PINCM register index that maps to the target
 *                          GPIO pin.
 *                          与目标 GPIO 引脚对应的 PINCM 寄存器索引
 */
__STATIC_INLINE void DL_GPIO_initDigitalOutput(uint32_t pincmIndex)
{
    /* GPIO functionality is always a pin function of 0x00000001 */
    IOMUX->SECCFG.PINCM[pincmIndex] =
        (IOMUX_PINCM_PC_CONNECTED | ((uint32_t) 0x00000001));
}

/**
 *  @brief      Configures a pin as a GPIO output
 *              将引脚配置为带可选特性的数字输出（反相/上下拉/驱动能力/高阻）
 *
 *  @param[in]  pincmIndex        The PINCM register index that maps to the target
 *                                GPIO pin.
 *  @param[in]  inversion         Enable Inversion of the pin output. One of @ref
 *                                DL_GPIO_INVERSION.
 *  @param[in]  internalResistor  Internal resistor to use. One of
 *                                @ref DL_GPIO_RESISTOR.
 *  @param[in]  driveStrength     Enable High-Drive for the pin. One of @ref
 *                                DL_GPIO_DRIVE_STRENGTH.
 *  @param[in]  hiZ               Enable/disable Hi-Z for the pin. One of
 *                                @ref DL_GPIO_HIZ.
 */
__STATIC_INLINE void DL_GPIO_initDigitalOutputFeatures(uint32_t pincmIndex,
    DL_GPIO_INVERSION inversion, DL_GPIO_RESISTOR internalResistor,
    DL_GPIO_DRIVE_STRENGTH driveStrength, DL_GPIO_HIZ hiZ)
{
    /* GPIO functionality is always a pin function of 0x00000001 */
    IOMUX->SECCFG.PINCM[pincmIndex] =
        IOMUX_PINCM_PC_CONNECTED | ((uint32_t) 0x00000001) |
        (uint32_t) inversion | (uint32_t) internalResistor |
        (uint32_t) driveStrength | (uint32_t) hiZ;
}

/**
 *  @brief      Configures internal resistor for digital pin
 *              为数字引脚单独配置内部上下拉电阻
 *
 *  @param[in]  pincmIndex        The PINCM register index that maps to the target
 *                                GPIO pin.
 *  @param[in]  internalResistor  Internal resistor to use. One of
 *                                @ref DL_GPIO_RESISTOR.
 */
__STATIC_INLINE void DL_GPIO_setDigitalInternalResistor(
    uint32_t pincmIndex, DL_GPIO_RESISTOR internalResistor)
{
    IOMUX->SECCFG.PINCM[pincmIndex] &=
        ~(DL_GPIO_RESISTOR_PULL_UP | DL_GPIO_RESISTOR_PULL_DOWN);
    IOMUX->SECCFG.PINCM[pincmIndex] |=
        IOMUX_PINCM_PC_CONNECTED | (uint32_t) internalResistor;
}

/**
 *  @brief      Configures internal resistor for analog pin
 *              为模拟功能引脚配置内部上下拉电阻（通常用于 ADC 等）
 *
 *  @param[in]  pincmIndex        The PINCM register index that maps to the target
 *                                GPIO pin.
 *  @param[in]  internalResistor  Internal resistor to use. One of
 *                                @ref DL_GPIO_RESISTOR.
 */
__STATIC_INLINE void DL_GPIO_setAnalogInternalResistor(
    uint32_t pincmIndex, DL_GPIO_RESISTOR internalResistor)
{
    /* GPIO functionality is always a pin function of 0x00000001 */
    /* For analog use case, setting IOMUX input enable */
    IOMUX->SECCFG.PINCM[pincmIndex] =
        IOMUX_PINCM_PC_UNCONNECTED | (uint32_t) internalResistor;
}

/**
 *  @brief      Configures a pin as a basic GPIO input
 *              将引脚配置为最基本的数字输入（开漏/上下拉等需另外配置）
 *
 *  Configures the pin as a basic GPIO input. If you want to use additional
 *  features of the input mode, refer to @ref DL_GPIO_initDigitalInputFeatures.
 *
 *  @param[in]  pincmIndex  The PINCM register index that maps to the target
 *                          GPIO pin.
 */
__STATIC_INLINE void DL_GPIO_initDigitalInput(uint32_t pincmIndex)
{
    /* GPIO functionality is always a pin function of 0x00000001 */
    IOMUX->SECCFG.PINCM[pincmIndex] = IOMUX_PINCM_INENA_ENABLE |
                                      IOMUX_PINCM_PC_CONNECTED |
                                      ((uint32_t) 0x00000001);
}

/**
 *  @brief      Configures a pin as a GPIO input
 *              将引脚配置为带可选特性的数字输入（反相/上下拉/迟滞/唤醒）
 *
 *  @param[in]  pincmIndex        The PINCM register index that maps to the target
 *                                GPIO pin.
 *  @param[in]  inversion         Enable Inversion of the pin input. One of @ref
 *                                DL_GPIO_INVERSION.
 *  @param[in]  internalResistor  Internal resistor to use. One of @ref
 *                                DL_GPIO_RESISTOR.
 *  @param[in]  hysteresis        Enable/disable Hysteresis on the pin. One of
 *                                @ref DL_GPIO_HYSTERESIS.
 *  @param[in]  wakeup            Configure wakeup behavior for the pin. One of
 *                                @ref DL_GPIO_WAKEUP.
 */
__STATIC_INLINE void DL_GPIO_initDigitalInputFeatures(uint32_t pincmIndex,
    DL_GPIO_INVERSION inversion, DL_GPIO_RESISTOR internalResistor,
    DL_GPIO_HYSTERESIS hysteresis, DL_GPIO_WAKEUP wakeup)
{
    /* GPIO functionality is always a pin function of 0x00000001 */
    IOMUX->SECCFG.PINCM[pincmIndex] =
        IOMUX_PINCM_INENA_ENABLE | IOMUX_PINCM_PC_CONNECTED |
        ((uint32_t) 0x00000001) | (uint32_t) inversion |
        (uint32_t) internalResistor | (uint32_t) hysteresis |
        ((uint32_t) wakeup & IOMUX_PINCM_WCOMP_MASK);
    IOMUX->SECCFG.PINCM[pincmIndex] |=
        ((uint32_t) wakeup & IOMUX_PINCM_WUEN_MASK);
}

/**
 *  @brief      Configure a pin to operate with peripheral functionality
 *              将引脚配置为外设复用功能（如 UART/SPI/定时器等）
 *
 *  @param[in]  pincmIndex The PINCM register index that maps to the target
 *                         pin to configure the peripheral functionality for
 *  @param[in]  function   Function to configure the pin for. See definition
 *                         of IOMUX_PINCMx_PF_xxx in the device header file.
 */
__STATIC_INLINE void DL_GPIO_initPeripheralFunction(
    uint32_t pincmIndex, uint32_t function)
{
    IOMUX->SECCFG.PINCM[pincmIndex] = function | IOMUX_PINCM_PC_CONNECTED;
}

/**
 *  @brief      Configure a pin to operate with peripheral output functionality
 *              将引脚配置为外设输出功能
 *
 *  @param[in]  pincmIndex The PINCM register index that maps to the target
 *                         pin to configure the peripheral functionality for
 *  @param[in]  function   Function to configure the pin for. See definition
 *                         of IOMUX_PINCMx_PF_xxx in the device header file.
 */
__STATIC_INLINE void DL_GPIO_initPeripheralOutputFunction(
    uint32_t pincmIndex, uint32_t function)
{
    IOMUX->SECCFG.PINCM[pincmIndex] = function | IOMUX_PINCM_PC_CONNECTED;
}

/**
 *  @brief      Configure a pin to operate with peripheral output functionality with optional features
 *              将引脚配置为带附加特性的外设输出功能（可配置反相/上下拉/驱动能力/高阻）
 *
 *  @param[in]  pincmIndex The PINCM register index that maps to the target
 *                         pin to configure the peripheral functionality for
 *  @param[in]  function          Function to configure the pin for. Check
 *                         definition of IOMUX_PINCMx_PF_xxx in the device
 *                         header file.
 *  @param[in]  inversion         Enable Inversion of the pin output. One of @ref
 *                                DL_GPIO_INVERSION.
 *  @param[in]  internalResistor  Internal resistor to use. One of
 *                                @ref DL_GPIO_RESISTOR.
 *  @param[in]  driveStrength     Enable High-Drive for the pin. One of @ref
 *                                DL_GPIO_DRIVE_STRENGTH.
 *  @param[in]  hiZ               Enable/disable Hi-Z for the pin. One of
 *                                @ref DL_GPIO_HIZ.
 */
__STATIC_INLINE void DL_GPIO_initPeripheralOutputFunctionFeatures(
    uint32_t pincmIndex, uint32_t function, DL_GPIO_INVERSION inversion,
    DL_GPIO_RESISTOR internalResistor, DL_GPIO_DRIVE_STRENGTH driveStrength,
    DL_GPIO_HIZ hiZ)
{
    IOMUX->SECCFG.PINCM[pincmIndex] =
        function | IOMUX_PINCM_PC_CONNECTED | (uint32_t) inversion |
        (uint32_t) internalResistor | (uint32_t) driveStrength |
        (uint32_t) hiZ;
}

/**
 *  @brief      Configure a pin to operate with peripheral input functionality
 *              将引脚配置为外设输入功能（并打开输入使能）
 *
 *  @param[in]  pincmIndex The PINCM register index that maps to the target
 *                         pin to configure the peripheral functionality for
 *  @param[in]  function   Function to configure the pin for. See definition
 *                         of IOMUX_PINCMx_PF_xxx in the device header file.
 */
__STATIC_INLINE void DL_GPIO_initPeripheralInputFunction(
    uint32_t pincmIndex, uint32_t function)
{
    IOMUX->SECCFG.PINCM[pincmIndex] =
        function | IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM_INENA_ENABLE;
}

/**
 *  @brief      Configure a pin to operate with peripheral input functionality with optional features
 *              将引脚配置为带附加特性的外设输入功能（反相/上下拉/迟滞/唤醒）
 *
 *  @param[in]  pincmIndex The PINCM register index that maps to the target
 *                         pin to configure the peripheral functionality for
 *  @param[in]  function   Function to configure the pin for. See definition
 *                         of IOMUX_PINCMx_PF_xxx in the device header file.
 *  @param[in]  inversion         Enable Inversion of the pin input. One of @ref
 *                                DL_GPIO_INVERSION.
 *  @param[in]  internalResistor  Internal resistor to use. One of @ref
 *                                DL_GPIO_RESISTOR.
 *  @param[in]  hysteresis        Enable/disable Hystersis on the pin. One of
 *                                @ref DL_GPIO_HYSTERESIS.
 *  @param[in]  wakeup            Configure wakeup behavior for the pin. One of
 *                                @ref DL_GPIO_WAKEUP.
 */
__STATIC_INLINE void DL_GPIO_initPeripheralInputFunctionFeatures(
    uint32_t pincmIndex, uint32_t function, DL_GPIO_INVERSION inversion,
    DL_GPIO_RESISTOR internalResistor, DL_GPIO_HYSTERESIS hysteresis,
    DL_GPIO_WAKEUP wakeup)
{
    IOMUX->SECCFG.PINCM[pincmIndex] =
        function | IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM_INENA_ENABLE |
        (uint32_t) inversion | (uint32_t) internalResistor |
        (uint32_t) hysteresis | ((uint32_t) wakeup & IOMUX_PINCM_WCOMP_MASK);
    IOMUX->SECCFG.PINCM[pincmIndex] |=
        ((uint32_t) wakeup & IOMUX_PINCM_WUEN_MASK);
}

/**
 *  @brief      Configure a pin to operate with analog functionality
 *              将引脚配置为模拟功能（与数字 GPIO 断开）
 *
 *  @param[in]  pincmIndex The PINCM register index that maps to the target
 *                         pin to configure the analog functionality for
 *
 */
__STATIC_INLINE void DL_GPIO_initPeripheralAnalogFunction(uint32_t pincmIndex)
{
    IOMUX->SECCFG.PINCM[pincmIndex] = IOMUX_PINCM_PC_UNCONNECTED;
}

/**
 *  @brief Set GPIO pin's wakeup enable bit.
 *         使能某 GPIO 引脚的唤醒功能
 *
 *  @param[in]  pincmIndex  The PINCM register index that maps to the target
 *                          GPIO pin.
 *
 *  @note Before enabling wakeup, the wakeup compare value should be configured first
 *        in @ref DL_GPIO_setWakeupCompareValue.
 *        在使能唤醒功能前，应先配置唤醒比较电平
 *        （参考 @ref DL_GPIO_setWakeupCompareValue）。
 */
__STATIC_INLINE void DL_GPIO_enableWakeUp(uint32_t pincmIndex)
{
    IOMUX->SECCFG.PINCM[pincmIndex] |= DL_GPIO_WAKEUP_ENABLE;
}

/**
 *  @brief Clear GPIO pin's wakeup enable bit.
 *         关闭某 GPIO 引脚的唤醒功能
 *
 *  @param[in]  pincmIndex  The PINCM register index that maps to the target
 *                          GPIO pin.
 *
 */
__STATIC_INLINE void DL_GPIO_disableWakeUp(uint32_t pincmIndex)
{
    IOMUX->SECCFG.PINCM[pincmIndex] &= ~(IOMUX_PINCM_WUEN_MASK);
}

/**
 *  @brief Returns if GPIO pin's wake up bit is enabled.
 *         查询某 GPIO 引脚的唤醒功能是否已开启
 *
 *  @param[in]  pincmIndex  The PINCM register index that maps to the target
 *                          GPIO pin.
 *
 *  @return     True if wake up enabled on GPIO pin.
 */
__STATIC_INLINE bool DL_GPIO_isWakeUpEnabled(uint32_t pincmIndex)
{
    return ((IOMUX->SECCFG.PINCM[pincmIndex] & IOMUX_PINCM_WUEN_MASK) ==
            IOMUX_PINCM_WUEN_ENABLE);
}

/**
 *  @brief Set the compare value to use for wake for the specified pin
 *         设置某 GPIO 引脚用于唤醒的比较电平
 *
 *  @param[in]  pincmIndex  The PINCM register index that maps to the target
 *                          GPIO pin.
 *  @param[in]  value       The wakeup compare value to set.
 *                          One of @ref DL_GPIO_WAKEUP_COMPARE_VALUE
 */
__STATIC_INLINE void DL_GPIO_setWakeupCompareValue(
    uint32_t pincmIndex, DL_GPIO_WAKEUP_COMPARE_VALUE value)
{
    DL_Common_updateReg(&IOMUX->SECCFG.PINCM[pincmIndex], (uint32_t) value,
        IOMUX_PINCM_WCOMP_MASK);
}

/**
 *  @brief Get the compare value to use for wake for the specified pin
 *         读取某 GPIO 引脚当前配置的唤醒比较电平
 *
 *  @param[in]  pincmIndex  The PINCM register index that maps to the target
 *                          GPIO pin.
 *
 *  @return     The wakeup compare value for the specified pin
 *
 *  @retval     One of @ref DL_GPIO_WAKEUP_COMPARE_VALUE
 */
__STATIC_INLINE DL_GPIO_WAKEUP_COMPARE_VALUE DL_GPIO_getWakeupCompareValue(
    uint32_t pincmIndex)
{
    uint32_t value = IOMUX->SECCFG.PINCM[pincmIndex] & IOMUX_PINCM_WCOMP_MASK;

    return (DL_GPIO_WAKEUP_COMPARE_VALUE)(value);
}

/**
 *  @brief Checks if the GPIO pin's Wake State bit is active.
 *         检查某 GPIO 引脚是否产生了唤醒事件（Wake State 状态位）
 *
 *  @param[in]  pincmIndex  The PINCM register index that maps to the target
 *                          GPIO pin.
 *
 *  @return     True if 0x00002000U, False if 0x00000000U.
 */
__STATIC_INLINE bool DL_GPIO_isWakeStateGenerated(uint32_t pincmIndex)
{
    return ((IOMUX->SECCFG.PINCM[pincmIndex] & IOMUX_PINCM_WAKESTAT_MASK) ==
            IOMUX_PINCM_WAKESTAT_ENABLE);
}

/**
 *  @brief      Read a group of GPIO pins
 *              读取一组 GPIO 引脚当前输入电平
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Pins to read. Bitwise OR of @ref DL_GPIO_PIN.
 *
 *  @return     The pins (from the selection) that are currently high
 *
 *  @retval     Bitwise OR of @ref DL_GPIO_PIN of pins that are currently high
 *              from the input selection.
 */
__STATIC_INLINE uint32_t DL_GPIO_readPins(GPIO_Regs* gpio, uint32_t pins)
{
    return (gpio->DIN31_0 & pins);
}

/**
 *  @brief      Write a group of GPIO pins
 *              一次性写入一组 GPIO 引脚的输出值
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Pins to write. All enabled GPIO pins will be set to
 *                    the equivalent bit value in pins.
 */
__STATIC_INLINE void DL_GPIO_writePins(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->DOUT31_0 = pins;
}

/**
 *  @brief      Update the value of one or more GPIO pins
 *              在不影响其它位的前提下，按掩码更新部分 GPIO 输出位
 *
 *  @param[in]  gpio      Pointer to the register overlay for the peripheral
 *  @param[in]  pinsMask  The GPIO pin(s) you want to update. Bitwise OR of
 *                        @ref DL_GPIO_PIN.
 *  @param[in]  pinsVal   Value(s) for the GPIO pin(s) you want to update.
 *                        Only the values for pins specified in pinsMask will
 *                        change.
 */
__STATIC_INLINE void DL_GPIO_writePinsVal(
    GPIO_Regs* gpio, uint32_t pinsMask, uint32_t pinsVal)
{
    uint32_t doutVal = gpio->DOUT31_0;
    doutVal &= ~pinsMask;
    doutVal |= (pinsVal & pinsMask);
    gpio->DOUT31_0 = doutVal;
}

/**
 *  @brief      Set a group of GPIO pins
 *              将一组 GPIO 引脚置为高电平（置位）
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Pins to set high. Bitwise OR of @ref DL_GPIO_PIN.
 */
__STATIC_INLINE void DL_GPIO_setPins(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->DOUTSET31_0 = pins;
}

/**
 *  @brief      Clear a group of GPIO pins
 *              将一组 GPIO 引脚置为低电平（清零）
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Pins to clear. Bitwise OR of @ref DL_GPIO_PIN.
 */
__STATIC_INLINE void DL_GPIO_clearPins(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->DOUTCLR31_0 = pins;
}

/**
 *  @brief      Toggle a group of GPIO pins
 *              翻转一组 GPIO 引脚当前输出状态
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Pins to toggle. Bitwise OR of @ref DL_GPIO_PIN.
 */
__STATIC_INLINE void DL_GPIO_togglePins(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->DOUTTGL31_0 = pins;
}

/**
 *  @brief      Enable output on a group of GPIO pins
 *              使能一组 GPIO 引脚的输出功能（方向设为输出）
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Pins to enable output. Bitwise OR of @ref DL_GPIO_PIN.
 */
__STATIC_INLINE void DL_GPIO_enableOutput(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->DOESET31_0 = pins;
}

/**
 *  @brief      Disable output on a group of GPIO pins
 *              关闭一组 GPIO 引脚的输出功能（方向设为输入或高阻）
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Pins to disable output. Bitwise OR of @ref DL_GPIO_PIN.
 */
__STATIC_INLINE void DL_GPIO_disableOutput(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->DOECLR31_0 = pins;
}

/**
 *  @brief      Enable DMA access on a group of pins
 *              为一组 GPIO 引脚打开 DMA 访问使能
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Pins to toggle. Bitwise OR of @ref DL_GPIO_PIN.
 */
__STATIC_INLINE void DL_GPIO_enableDMAAccess(GPIO_Regs* gpio, uint32_t pins)
{
    DL_Common_updateReg(&gpio->DMAMASK, pins, pins);
}

/**
 *  @brief      Disable DMA access on a group of pins
 *              关闭一组 GPIO 引脚的 DMA 访问
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Pins to disable DMA access on. Bitwise OR of @ref
 *                    DL_GPIO_PIN.
 */
__STATIC_INLINE void DL_GPIO_disableDMAAccess(GPIO_Regs* gpio, uint32_t pins)
{
    DL_Common_updateReg(&gpio->DMAMASK, 0x00000000, pins);
}

/**
 *  @brief      Check if DMA access is enabled on a group of pins
 *              查询一组 GPIO 引脚哪些开启了 DMA 访问
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Pins to check DMA access on. Bitwise OR of @ref
 *                    DL_GPIO_PIN.
 *
 *  @return     Which pins of the requested group have DMA access enabled
 *
 *  @retval     Bitwise OR of @ref DL_GPIO_PIN values
 */
__STATIC_INLINE uint32_t DL_GPIO_isDMAccessEnabled(
    GPIO_Regs* gpio, uint32_t pins)
{
    return (gpio->DMAMASK & pins);
}

/**
 * @brief       Set the polarity of all bits [0, 15] in the group of pins
 *              设置低 16 个 GPIO 引脚（0~15）的边沿检测极性
 *
 * @param[in]  gpio      Pointer to the register overlay for the peripheral
 * @param[in]  polarity  Bitwise OR of @ref DL_GPIO_EDGE_POLARITY for
 *                       pins [0, 15]
 */
__STATIC_INLINE void DL_GPIO_setLowerPinsPolarity(
    GPIO_Regs* gpio, uint32_t polarity)
{
    gpio->POLARITY15_0 |= polarity;
}

/**
 * @brief       Set the polarity of all bits [16, 31] in the group of pins
 *              设置高 16 个 GPIO 引脚（16~31）的边沿检测极性
 *
 * @param[in]  gpio     Pointer to the register overlay for the peripheral
 * @param[in]  polarity  Bitwise OR of @ref DL_GPIO_EDGE_POLARITY for
 *                       pins [16, 31]
 */
__STATIC_INLINE void DL_GPIO_setUpperPinsPolarity(
    GPIO_Regs* gpio, uint32_t polarity)
{
    gpio->POLARITY31_16 |= polarity;
}

/**
 * @brief      Get the polarity of bits [0, 15] in the group of pins
 *             读取低 16 个 GPIO 引脚（0~15）的边沿检测极性配置
 *
 * @param[in]  gpio     Pointer to the register overlay for the peripheral
 *
 * @retval     Polarity setting with bitwise OR of @ref DL_GPIO_EDGE_POLARITY
 *             for pins [0, 15]
 *
 */
__STATIC_INLINE uint32_t DL_GPIO_getLowerPinsPolarity(GPIO_Regs* gpio)
{
    return gpio->POLARITY15_0;
}

/**
 * @brief      Get the polarity of bits [16, 31] in the group of pins
 *             读取高 16 个 GPIO 引脚（16~31）的边沿检测极性配置
 *
 * @param[in]  gpio     Pointer to the register overlay for the peripheral
 *
 * @retval     Polarity setting with bitwise OR of @ref DL_GPIO_EDGE_POLARITY
 *             for pins [16, 31]
 *
 */
__STATIC_INLINE uint32_t DL_GPIO_getUpperPinsPolarity(GPIO_Regs* gpio)
{
    return gpio->POLARITY31_16;
}

/**
 * @brief       Set the input filter of bits [0, 15] in the group of pins
 *              设置低 16 个 GPIO 引脚（0~15）的输入滤波参数
 *
 * @param[in]  gpio      Pointer to the register overlay for the peripheral
 * @param[in]  filter    Bitwise OR of @ref DL_GPIO_INPUT_FILTER for
 *                       pins [0, 15]
 */
__STATIC_INLINE void DL_GPIO_setLowerPinsInputFilter(
    GPIO_Regs* gpio, uint32_t filter)
{
    gpio->FILTEREN15_0 |= filter;
}

/**
 * @brief       Set the input filter of bits [16, 31] in the group of pins
 *              设置高 16 个 GPIO 引脚（16~31）的输入滤波参数
 *
 * @param[in]  gpio      Pointer to the register overlay for the peripheral
 * @param[in]  filter    Bitwise OR of @ref DL_GPIO_INPUT_FILTER for
 *                       pins [16, 31]
 */
__STATIC_INLINE void DL_GPIO_setUpperPinsInputFilter(
    GPIO_Regs* gpio, uint32_t filter)
{
    gpio->FILTEREN31_16 |= filter;
}

/**
 * @brief      Get the input filter of bits [0, 15] in the group of pins
 *             读取低 16 个 GPIO 引脚（0~15）的输入滤波配置
 *
 * @param[in]  gpio     Pointer to the register overlay for the peripheral
 *
 * @retval     Input filter setting with bitwise OR of @ref DL_GPIO_INPUT_FILTER
 *             for pins [0, 15]
 *
 */
__STATIC_INLINE uint32_t DL_GPIO_getLowerPinsInputFilter(GPIO_Regs* gpio)
{
    return gpio->FILTEREN15_0;
}

/**
 * @brief      Get the input filter of bits [16, 31] in the group of pins
 *             读取高 16 个 GPIO 引脚（16~31）的输入滤波配置
 *
 * @param[in]  gpio     Pointer to the register overlay for the peripheral
 *
 * @retval     Input filter setting with bitwise OR of @ref DL_GPIO_INPUT_FILTER
 *             for pins [16, 31]
 *
 */
__STATIC_INLINE uint32_t DL_GPIO_getUpperPinsInputFilter(GPIO_Regs* gpio)
{
    return gpio->FILTEREN31_16;
}

/**
 *  @brief      Enable Global Fast Wake
 *              使能 GPIO 全局快速唤醒功能
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_GPIO_enableGlobalFastWake(GPIO_Regs* gpio)
{
    gpio->CTL |= GPIO_CTL_FASTWAKEONLY_GLOBAL_EN;
}

/**
 *  @brief      Disable Global Fast Wake
 *              关闭 GPIO 全局快速唤醒功能
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_GPIO_disableGlobalFastWake(GPIO_Regs* gpio)
{
    gpio->CTL &= ~GPIO_CTL_FASTWAKEONLY_GLOBAL_EN;
}

/**
 *  @brief      Enable fast wake for pins
 *              为指定引脚使能快速唤醒特性
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Bit mask of pins to enable fast-wake feature. Bitwise OR
 *                    of @ref DL_GPIO_PIN.
 */
__STATIC_INLINE void DL_GPIO_enableFastWakePins(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->FASTWAKE |= pins;
}

/**
 *  @brief      Disable fast wake for pins
 *              关闭指定引脚的快速唤醒特性
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Bit mask of pins to disable fast-wake feature. Bitwise OR
 *                    of @ref DL_GPIO_PIN.
 */
__STATIC_INLINE void DL_GPIO_disableFastWakePins(
    GPIO_Regs* gpio, uint32_t pins)
{
    gpio->FASTWAKE &= ~(pins);
}

/**
 *  @brief      Enable Hi-Z for the pin
 *              使能某 GPIO 引脚的高阻状态（输出关闭）
 *
 *  @param[in]  pincmIndex  The PINCM register index that maps to the target
 *                          GPIO pin
 */
__STATIC_INLINE void DL_GPIO_enableHiZ(uint32_t pincmIndex)
{
    IOMUX->SECCFG.PINCM[pincmIndex] |= IOMUX_PINCM_HIZ1_ENABLE;
}

/**
 *  @brief      Disable Hi-Z for the pin
 *              关闭某 GPIO 引脚的高阻状态
 *
 *  @param[in]  pincmIndex  The PINCM register index that maps to the target
 *                          GPIO pin
 */
__STATIC_INLINE void DL_GPIO_disableHiZ(uint32_t pincmIndex)
{
    IOMUX->SECCFG.PINCM[pincmIndex] &= ~(IOMUX_PINCM_HIZ1_ENABLE);
}

/**
 *  @brief      Check which pins have fast wake feature enabled
 *              查询一组 GPIO 引脚中哪些启用了快速唤醒
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Bit mask of pins to check. Bitwise OR of @ref DL_GPIO_PIN.
 *
 *  @return     Which of the requested GPIO pins have fast wake enabled
 *
 *  @retval     Bitwise OR of @ref DL_GPIO_PIN values
 */
__STATIC_INLINE uint32_t DL_GPIO_getEnabledFastWakePins(
    GPIO_Regs* gpio, uint32_t pins)
{
    return (gpio->FASTWAKE & pins);
}

/**
 *  @brief      Enable GPIO interrupts
 *              使能指定 GPIO 引脚的 CPU 中断
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Bit mask of interrupts to enable. Bitwise OR of
 *                    @ref DL_GPIO_PIN.
 */
__STATIC_INLINE void DL_GPIO_enableInterrupt(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->CPU_INT.IMASK |= pins;
}

/**
 *  @brief      Disable GPIO interrupts
 *              关闭指定 GPIO 引脚的 CPU 中断
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Bit mask of interrupts to disable. Bitwise OR of
 *                    @ref DL_GPIO_PIN.
 */
__STATIC_INLINE void DL_GPIO_disableInterrupt(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->CPU_INT.IMASK &= ~(pins);
}

/**
 *  @brief      Check which GPIO interrupts are enabled
 *              查询一组 GPIO 引脚当前哪些开启了中断
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Bit mask of interrupts to check. Bitwise OR of
 *                    @ref DL_GPIO_PIN.
 *
 *  @return     Which of the requested GPIO interrupts are enabled
 *
 *  @retval     Bitwise OR of @ref DL_GPIO_PIN values
 */
__STATIC_INLINE uint32_t DL_GPIO_getEnabledInterrupts(
    GPIO_Regs* gpio, uint32_t pins)
{
    return (gpio->CPU_INT.IMASK & pins);
}

/**
 *  @brief      Check interrupt flag of enabled GPIO interrupts
 *              查询已使能的 GPIO 中断中，哪些当前处于挂起状态
 *
 *  Checks if any of the GPIO interrupts that were previously enabled are
 *  pending.
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Bit mask of interrupts to check. Bitwise OR of
 *                    @ref DL_GPIO_PIN.
 *
 *  @return     Which of the requested GPIO interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_GPIO_PIN values
 *
 *  @sa         DL_GPIO_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_GPIO_getEnabledInterruptStatus(
    GPIO_Regs* gpio, uint32_t pins)
{
    return (gpio->CPU_INT.MIS & pins);
}

/**
 *  @brief      Set interrupt flag of any GPIO
 *              手动置位 GPIO 中断挂起标志（软触发）
 *
 *  Manually set a GPIO interrupt to be pending
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *
 *  @param[in]  pins  Bit mask of interrupts to set. Bitwise OR of
 *                    @ref DL_GPIO_PIN.
 *
 */
__STATIC_INLINE void DL_GPIO_setInterrupt(GPIO_Regs* gpio, uint32_t pins)
{
    gpio->CPU_INT.ISET = pins;
}

/**
 *  @brief      Check interrupt flag of any GPIO interrupt
 *              查询一组 GPIO 引脚的原始中断挂起状态（与是否使能无关）
 *
 *  Checks if any of the GPIO interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *
 *  @param[in]  pins  Bit mask of interrupts to check. Bitwise OR of
 *                    @ref DL_GPIO_PIN.
 *
 *  @return     Which of the requested GPIO interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_GPIO_PIN values
 */
__STATIC_INLINE uint32_t DL_GPIO_getRawInterruptStatus(
    GPIO_Regs* gpio, uint32_t pins)
{
    return (gpio->CPU_INT.RIS & pins);
}

/**
 *  @brief      Get highest priority pending GPIO interrupt
 *              获取当前优先级最高的 GPIO 中断索引（读取 IIDX 会自动清除对应标志）
 *
 *  Checks if any of the GPIO interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *
 *  @return     The highest priority pending GPIO interrupt
 *
 *  @retval     One of @ref DL_GPIO_IIDX
 */
__STATIC_INLINE DL_GPIO_IIDX DL_GPIO_getPendingInterrupt(GPIO_Regs* gpio)
{
    return (DL_GPIO_IIDX)(gpio->CPU_INT.IIDX);
}

/**
 *  @brief      Clear pending GPIO interrupts
 *              清除一组 GPIO 引脚的中断挂起标志
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  pins  Bit mask of interrupts to check. Bitwise OR of
 *                    @ref DL_GPIO_PIN.
 *
 */
__STATIC_INLINE void DL_GPIO_clearInterruptStatus(
    GPIO_Regs* gpio, uint32_t pins)
{
    gpio->CPU_INT.ICLR |= pins;
}

/**
 * @brief Configures GPIO subscriber. This API preserves enable/disbale status
 *        of subscriber.
 *
 * @param[in] gpio     Pointer to the register overlay for the peripheral
 * @param[in] index    Specifies the subscriber event index to be configured
 * @param[in] policy   Specifies the the GPIO behavior when the subscriber
 *                     receives publisher event.
 * @param[in] pinIndex Specifies the GPIO bit number which will be targeted by
 *                     the subscriber action.
 *
 */
__STATIC_INLINE void DL_GPIO_configSubscriber(GPIO_Regs* gpio,
    DL_GPIO_SUBSCRIBER_INDEX index, DL_GPIO_SUBSCRIBER_OUT_POLICY policy,
    DL_GPIO_SUBSCRIBERx_PIN pinIndex)

{
    volatile uint32_t* pReg = &gpio->SUB0CFG;

    pReg += ((uint32_t) index << 3);

    DL_Common_updateReg(pReg, ((uint32_t) pinIndex | (uint32_t) policy),
        (GPIO_SUB0CFG_INDEX_MASK | GPIO_SUB1CFG_OUTPOLICY_MASK));
}

/**
 * @brief Enables GPIO subscriber
 *
 * @param[in] gpio     Pointer to the register overlay for the peripheral
 * @param[in] index    Specifies the subscriber event index to be configured
 *
 */
__STATIC_INLINE void DL_GPIO_enableSubscriber(
    GPIO_Regs* gpio, DL_GPIO_SUBSCRIBER_INDEX index)
{
    volatile uint32_t* pReg = &gpio->SUB0CFG;

    pReg += ((uint32_t) index << 3);
    *(pReg) |= (GPIO_SUB1CFG_ENABLE_SET);
}

/**
 * @brief Disables GPIO subscriber
 *
 * @param[in] gpio     Pointer to the register overlay for the peripheral
 * @param[in] index    Specifies the subscriber event index to be configured
 *
 */
__STATIC_INLINE void DL_GPIO_disableSubscriber(
    GPIO_Regs* gpio, DL_GPIO_SUBSCRIBER_INDEX index)
{
    volatile uint32_t* pReg = &gpio->SUB0CFG;

    pReg += ((uint32_t) index << 3);
    *(pReg) &= ~(GPIO_SUB1CFG_ENABLE_SET);
}

/**
 * @brief Returns if GPIO subscriber is enabled
 *
 * @param[in] gpio     Pointer to the register overlay for the peripheral
 * @param[in] index    Specifies the subscriber event index to be configured
 *
 * @return true if GPIO subscriber is enabled
 * @return false if GPIO subscriber is disabled
 *
 */
__STATIC_INLINE bool DL_GPIO_isSubscriberEnabled(
    GPIO_Regs* gpio, DL_GPIO_SUBSCRIBER_INDEX index)
{
    volatile uint32_t* pReg = &gpio->SUB0CFG;

    pReg += ((uint32_t) index << 3);
    return (GPIO_SUB1CFG_ENABLE_SET == (*(pReg) &GPIO_SUB1CFG_ENABLE_MASK));
}

/**
 *  @brief Sets the event publisher channel id
 *
 *  @param[in]  gpio   Pointer to the register overlay for the peripheral
 *  @param[in]  index  Specifies the register event index to be configured
 *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0
 *                      subscriber is disconnected. Consult your device
 *                      datasheet on the actual maximum number of channels.
 *
 */
__STATIC_INLINE void DL_GPIO_setPublisherChanID(
    GPIO_Regs* gpio, DL_GPIO_PUBLISHER_INDEX index, uint8_t chanID)
{
    volatile uint32_t* pReg = &gpio->FPUB_0;

    *(pReg + (uint32_t) index) =
        ((uint32_t) chanID & GPIO_FSUB_0_CHANID_MAXIMUM);
}

/**
 *  @brief Gets the event publisher channel id
 *
 *  @param[in]  gpio   Pointer to the register overlay for the peripheral
 *  @param[in]  index  Specifies the register event index to be configured
 *
 *  @return     Event publisher channel ID
 *
 */
__STATIC_INLINE uint8_t DL_GPIO_getPublisherChanID(
    GPIO_Regs* gpio, DL_GPIO_PUBLISHER_INDEX index)
{
    volatile uint32_t* pReg = &gpio->FPUB_0;

    return ((uint8_t)(*(pReg + (uint32_t) index) & GPIO_FPUB_0_CHANID_MASK));
}

/**
 *  @brief Sets the event subscriber channel id
 *
 *  @param[in]  gpio    Pointer to the register overlay for the peripheral
 *  @param[in]  index   Specifies the register event index to be configured
 *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0
 *                      subscriber is disconnected. Consult your device
 *                      datasheet on the actual maximum number of channels.
 *
 */
__STATIC_INLINE void DL_GPIO_setSubscriberChanID(
    GPIO_Regs* gpio, DL_GPIO_SUBSCRIBER_INDEX index, uint8_t chanID)
{
    volatile uint32_t* pReg = &gpio->FSUB_0;

    *(pReg + (uint32_t) index) =
        ((uint32_t) chanID & GPIO_FSUB_0_CHANID_MAXIMUM);
}

/**
 *  @brief Gets the event subscriber channel id
 *
 *  @param[in]  gpio   Pointer to the register overlay for the peripheral
 *  @param[in]  index  Specifies the register event index to be configured
 *
 *  @return     Event subscriber channel ID
 *
 */
__STATIC_INLINE uint8_t DL_GPIO_getSubscriberChanID(
    GPIO_Regs* gpio, DL_GPIO_SUBSCRIBER_INDEX index)
{
    volatile uint32_t* pReg = &gpio->FSUB_0;

    return ((uint8_t)(*(pReg + (uint32_t) index) & GPIO_FSUB_0_CHANID_MASK));
}

/**
 *  @brief Enables GPIO events
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  index Specifies the register event index to be configured
 *  @param[in]  pins  Valid options will depend on index argument. When
 *               index == @ref DL_GPIO_EVENT_ROUTE_1, valid pins values are
 *               bitwise OR of @ref DL_GPIO_PIN (0-15). When
 *               index == @ref DL_GPIO_EVENT_ROUTE_2, valid pins Bitwise OR of
 *               @ref DL_GPIO_PIN (16-31).
 *
 */
__STATIC_INLINE void DL_GPIO_enableEvents(
    GPIO_Regs* gpio, DL_GPIO_EVENT_ROUTE index, uint32_t pins)
{
    switch (index) {
        case DL_GPIO_EVENT_ROUTE_1:
            gpio->GEN_EVENT0.IMASK |= (pins & 0x0000FFFFU);
            break;
        case DL_GPIO_EVENT_ROUTE_2:
            gpio->GEN_EVENT1.IMASK |= (pins & 0xFFFF0000U);
            break;
        default:
            break;
    }
}

/**
 *  @brief      Disable GPIO events
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  index Specifies the register event index to be configured
 *  @param[in]  pins  Valid options will depend on index argument. When
 *               index == @ref DL_GPIO_EVENT_ROUTE_1, valid pins values are
 *               bitwise OR of @ref DL_GPIO_PIN (0-15). When
 *               index == @ref DL_GPIO_EVENT_ROUTE_2, valid pins Bitwise OR of
 *               @ref DL_GPIO_PIN (16-31).
 *
 */
__STATIC_INLINE void DL_GPIO_disableEvents(
    GPIO_Regs* gpio, DL_GPIO_EVENT_ROUTE index, uint32_t pins)
{
    switch (index) {
        case DL_GPIO_EVENT_ROUTE_1:
            gpio->GEN_EVENT0.IMASK &= ~(pins & 0x0000FFFFU);
            break;
        case DL_GPIO_EVENT_ROUTE_2:
            gpio->GEN_EVENT1.IMASK &= ~(pins & 0xFFFF0000U);
            break;
        default:
            break;
    }
}

/**
 *  @brief      Check which GPIO events are enabled
 *
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  index Specifies the register event index to be configured
 *  @param[in]  pins  Valid options will depend on index argument. When
 *               index == @ref DL_GPIO_EVENT_ROUTE_1, valid pins values are
 *               bitwise OR of @ref DL_GPIO_PIN (0-15). When
 *               index == @ref DL_GPIO_EVENT_ROUTE_2, valid pins Bitwise OR of
 *               @ref DL_GPIO_PIN (16-31).
 *
 *  @return     Which of the requested GPIO events are enabled
 *
 *  @retval     Bitwise OR of @ref DL_GPIO_PIN values
 */
__STATIC_INLINE uint32_t DL_GPIO_getEnabledEvents(
    GPIO_Regs* gpio, DL_GPIO_EVENT_ROUTE index, uint32_t pins)
{
    volatile uint32_t* pReg = &gpio->GEN_EVENT0.IMASK;

    return ((*(pReg + (uint32_t) index) & pins));
}

/**
 *  @brief Checks if any of the GPIO events which were previously enabled are
 *  pending.
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  index Specifies the register event index to be configured
 *  @param[in]  pins  Valid options will depend on index argument. When
 *               index == @ref DL_GPIO_EVENT_ROUTE_1, valid pins values are
 *               bitwise OR of @ref DL_GPIO_PIN (0-15). When
 *               index == @ref DL_GPIO_EVENT_ROUTE_2, valid pins Bitwise OR of
 *               @ref DL_GPIO_PIN (16-31).
 *
 *  @return     Which of the requested GPIO events are pending
 *
 *  @retval     Bitwise OR of @ref DL_GPIO_PIN values
 *
 *  @sa         DL_GPIO_enableEvents
 */
__STATIC_INLINE uint32_t DL_GPIO_getEnabledEventStatus(
    GPIO_Regs* gpio, DL_GPIO_EVENT_ROUTE index, uint32_t pins)
{
    const volatile uint32_t* pReg = &gpio->GEN_EVENT0.MIS;

    return ((*(pReg + (uint32_t) index) & pins));
}

/**
 *  @brief      Clear pending GPIO event
 *
 *  @param[in]  gpio  Pointer to the register overlay for the peripheral
 *  @param[in]  index Specifies the register event index to be configured
 *  @param[in]  pins  Valid options will depend on index argument. When
 *               index == @ref DL_GPIO_EVENT_ROUTE_1, valid pins values are
 *               bitwise OR of @ref DL_GPIO_PIN (0-15). When
 *               index == @ref DL_GPIO_EVENT_ROUTE_2, valid pins Bitwise OR of
 *               @ref DL_GPIO_PIN (16-31).
 *
 */
__STATIC_INLINE void DL_GPIO_clearEventStatus(
    GPIO_Regs* gpio, DL_GPIO_EVENT_ROUTE index, uint32_t pins)
{
    switch (index) {
        case DL_GPIO_EVENT_ROUTE_1:
            gpio->GEN_EVENT0.ICLR |= (pins & 0x0000FFFFU);
            break;
        case DL_GPIO_EVENT_ROUTE_2:
            gpio->GEN_EVENT1.ICLR |= (pins & 0xFFFF0000U);
            break;
        default:
            break;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_GPIO__ */

#endif /* ti_dl_dl_gpio__include */
/** @}*/
