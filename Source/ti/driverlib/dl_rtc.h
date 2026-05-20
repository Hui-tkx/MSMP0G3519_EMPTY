/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file       dl_rtc.h
 *  @brief      实时时钟 (RTC) 外设接口
 *  @defgroup   RTC 实时时钟 (RTC)
 *
 *  @anchor ti_devices_msp_dl_rtc_Overview
 *  # 概述
 *  RTC 驱动库允许完全配置 MSPM0 RTC 模块。
 *  实时时钟 (RTC) 模块提供具有日历模式的时钟计数器、
 *  灵活的可编程闹钟、偏移校准和温度补偿功能。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup RTC
 * @{
 */
#ifndef ti_dl_dl_rtc__include
#define ti_dl_dl_rtc__include

#include <ti/driverlib/dl_rtc_common.h>

#ifdef __MSPM0_HAS_RTC__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/**
 * @brief 日历结构体，用于 DL_RTC_initCalendar()
 * @brief 所有结构体值必须使用相同的格式（二进制或 BCD）。
 * @brief RTC 格式在初始化期间设置。
 */
typedef DL_RTC_Common_Calendar                  DL_RTC_Calendar;

/**
 * @brief 闹钟结构体，用于 DL_RTC_setCalendarAlarm
 * @brief 所有结构体值必须使用相同的格式（二进制或 BCD）。
 * @brief RTC 格式在初始化期间设置。
 */
typedef DL_RTC_Common_CalendarAlarm             DL_RTC_CalendarAlarm;

/*!
 * @brief 选择实时时钟的二进制格式
 */
#define DL_RTC_FORMAT_BINARY                    DL_RTC_COMMON_FORMAT_BINARY

/*!
 * @brief 选择实时时钟的BCD格式
 */
#define DL_RTC_FORMAT_BCD                       DL_RTC_COMMON_FORMAT_BCD

/*!
 * @brief RTC 安全可读状态
 */
#define DL_RTC_STATUS_READY                     DL_RTC_COMMON_STATUS_READY

/*!
 * @brief RTC 不可安全可读状态
 */
#define DL_RTC_STATUS_NOT_READY                 DL_RTC_COMMON_STATUS_NOT_READY

/*!
 * @brief RTC 准备好获取新的补偿值
 */
#define DL_RTC_COMPENSATION_STATUS_READY        DL_RTC_COMMON_COMPENSATION_STATUS_READY

/*!
 * @brief RTC 未准备好获取新的补偿值
 */
#define DL_RTC_COMPENSATION_STATUS_NOT_READY    DL_RTC_COMMON_COMPENSATION_STATUS_NOT_READY

/*!
 * @brief 补偿写入成功
 */
#define DL_RTC_COMPENSATION_WRITE_RESULT_OK     DL_RTC_COMMON_COMPENSATION_WRITE_RESULT_OK

/*!
 * @brief 补偿写入失败
 */
#define DL_RTC_COMPENSATION_WRITE_RESULT_NOT_OK DL_RTC_COMMON_COMPENSATION_WRITE_RESULT_NOT_OK

/*!
 * @brief 校准频率向下调整
 */
#define DL_RTC_OFFSET_CALIBRATION_SIGN_DOWN     DL_RTC_COMMON_OFFSET_CALIBRATION_SIGN_DOWN

/*!
 * @brief 校准频率向上调整
 */
#define DL_RTC_OFFSET_CALIBRATION_SIGN_UP       DL_RTC_COMMON_OFFSET_CALIBRATION_SIGN_UP

/*!
 * @brief 无频率输出到RTCCLK引脚
 */
#define DL_RTC_OFFSET_CALIBRATION_FREQUENCY_OFF DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_OFF

/*!
 * @brief 512Hz频率输出到RTCLK引脚
 */
#define DL_RTC_OFFSET_CALIBRATION_FREQUENCY_512       DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_512

/*!
 * @brief 256Hz频率输出到RTCLK引脚
 */
#define DL_RTC_OFFSET_CALIBRATION_FREQUENCY_256       DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_256

/*!
 * @brief 1Hz频率输出到RTCLK引脚
 */
#define DL_RTC_OFFSET_CALIBRATION_FREQUENCY_1         DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_1

/*!
 * @brief 以-1ppm的步长进行校准
 */
#define DL_RTC_TEMP_CALIBRATION_DOWN1PPM        DL_RTC_COMMON_TEMP_CALIBRATION_DOWN1PPM

/*!
 * @brief 以+1ppm的步长进行校准
 */
#define DL_RTC_TEMP_CALIBRATION_UP1PPM          DL_RTC_COMMON_TEMP_CALIBRATION_UP1PPM

/*!
 * @brief 每分钟触发日历中断
 */
#define DL_RTC_INTERVAL_ALARM_MINUTECHANGE      DL_RTC_COMMON_INTERVAL_ALARM_MINUTECHANGE

/*!
 * @brief 每小时触发日历中断
 */
#define DL_RTC_INTERVAL_ALARM_HOURCHANGE        DL_RTC_COMMON_INTERVAL_ALARM_HOURCHANGE

/*!
 * @brief 每天中午（12:00:00）触发日历中断
 */
#define DL_RTC_INTERVAL_ALARM_NOON              DL_RTC_COMMON_INTERVAL_ALARM_NOON

/*!
 * @brief 每天午夜（00:00:00）触发日历中断
 */
#define DL_RTC_INTERVAL_ALARM_MIDNIGHT          DL_RTC_COMMON_INTERVAL_ALARM_MIDNIGHT

/*!
 * @brief 预分频器0分频8
 */
#define DL_RTC_PRESCALER0_DIVIDE_8              DL_RTC_COMMON_PRESCALER0_DIVIDE_8

/*!
 * @brief 预分频器0分频16
 */
#define DL_RTC_PRESCALER0_DIVIDE_16              DL_RTC_COMMON_PRESCALER0_DIVIDE_16

/*!
 * @brief 预分频器0分频32
 */
#define DL_RTC_PRESCALER0_DIVIDE_32              DL_RTC_COMMON_PRESCALER0_DIVIDE_32

/*!
 * @brief 预分频器0分频64
 */
#define DL_RTC_PRESCALER0_DIVIDE_64              DL_RTC_COMMON_PRESCALER0_DIVIDE_64

/*!
 * @brief 预分频器0分频128
 */
#define DL_RTC_PRESCALER0_DIVIDE_128              DL_RTC_COMMON_PRESCALER0_DIVIDE_128

/*!
 * @brief 预分频器0分频256
 */
#define DL_RTC_PRESCALER0_DIVIDE_256              DL_RTC_COMMON_PRESCALER0_DIVIDE_256

/*!
 * @brief 预分频器1分频2
 */
#define DL_RTC_PRESCALER1_DIVIDE_2              DL_RTC_COMMON_PRESCALER1_DIVIDE_2

/*!
 * @brief 预分频器1分频4
 */
#define DL_RTC_PRESCALER1_DIVIDE_4              DL_RTC_COMMON_PRESCALER1_DIVIDE_4

/*!
 * @brief 预分频器1分频8
 */
#define DL_RTC_PRESCALER1_DIVIDE_8              DL_RTC_COMMON_PRESCALER1_DIVIDE_8

/*!
 * @brief 预分频器1分频16
 */
#define DL_RTC_PRESCALER1_DIVIDE_16              DL_RTC_COMMON_PRESCALER1_DIVIDE_16

/*!
 * @brief 预分频器1分频32
 */
#define DL_RTC_PRESCALER1_DIVIDE_32              DL_RTC_COMMON_PRESCALER1_DIVIDE_32

/*!
 * @brief 预分频器1分频64
 */
#define DL_RTC_PRESCALER1_DIVIDE_64              DL_RTC_COMMON_PRESCALER1_DIVIDE_64

/*!
 * @brief 预分频器1分频128
 */
#define DL_RTC_PRESCALER1_DIVIDE_128              DL_RTC_COMMON_PRESCALER1_DIVIDE_128

/*!
 * @brief 预分频器1分频256
 */
#define DL_RTC_PRESCALER1_DIVIDE_256              DL_RTC_COMMON_PRESCALER1_DIVIDE_256

/*!
 * @brief RTC 闹钟1中断索引
 */
#define DL_RTC_IIDX_ALARM1                      DL_RTC_COMMON_IIDX_ALARM1

/*!
 * @brief RTC 闹钟2中断索引
 */
#define DL_RTC_IIDX_ALARM2                      DL_RTC_COMMON_IIDX_ALARM2

/*!
 * @brief RTC 预分频器0中断索引
 */
#define DL_RTC_IIDX_PRESCALER0                  DL_RTC_COMMON_IIDX_PRESCALER0

/*!
 * @brief RTC 预分频器1中断索引
 */
#define DL_RTC_IIDX_PRESCALER1                  DL_RTC_COMMON_IIDX_PRESCALER1

/*!
 * @brief RTC 间隔定时器中断索引
 */
#define DL_RTC_IIDX_INTERVAL_TIMER            DL_RTC_COMMON_IIDX_INTERVAL_TIMER

/*!
 * @brief RTC 就绪中断索引
 */
#define DL_RTC_IIDX_READY                     DL_RTC_COMMON_IIDX_READY

/** @addtogroup DL_RTC_INTERRUPT
 *  @{
 */

/*!
 * @brief RTC 日历闹钟1中断
 */
#define DL_RTC_INTERRUPT_CALENDAR_ALARM1        DL_RTC_COMMON_INTERRUPT_CALENDAR_ALARM1

/*!
 * @brief RTC 日历闹钟2中断
 */
#define DL_RTC_INTERRUPT_CALENDAR_ALARM2        DL_RTC_COMMON_INTERRUPT_CALENDAR_ALARM2

/*!
 * @brief RTC 预分频器0中断
 */
#define DL_RTC_INTERRUPT_PRESCALER0             DL_RTC_COMMON_INTERRUPT_PRESCALER0

/*!
 * @brief RTC 预分频器1中断
 */
#define DL_RTC_INTERRUPT_PRESCALER1             DL_RTC_COMMON_INTERRUPT_PRESCALER1

/*!
 * @brief RTC 间隔闹钟中断
 */
#define DL_RTC_INTERRUPT_INTERVAL_ALARM         DL_RTC_COMMON_INTERRUPT_INTERVAL_ALARM

/*!
 * @brief RTC 就绪中断
 */
#define DL_RTC_INTERRUPT_READY                  DL_RTC_COMMON_INTERRUPT_READY

/** @}*/

/** @addtogroup DL_RTC_EVENT
 *  @{
 */

/*!
 * @brief RTC 日历闹钟1事件
 */
#define DL_RTC_EVENT_CALENDAR_ALARM1            DL_RTC_COMMON_EVENT_CALENDAR_ALARM1

/*!
 * @brief RTC 日历闹钟2事件
 */
#define DL_RTC_EVENT_CALENDAR_ALARM2            DL_RTC_COMMON_EVENT_CALENDAR_ALARM2

/*!
 * @brief RTC 预分频器0事件
 */
#define DL_RTC_EVENT_PRESCALER0                 DL_RTC_COMMON_EVENT_PRESCALER0

/*!
 * @brief RTC 预分频器1事件
 */
#define DL_RTC_EVENT_PRESCALER1                 DL_RTC_COMMON_EVENT_PRESCALER1

/*!
 * @brief RTC 间隔闹钟事件
 */
#define DL_RTC_EVENT_INTERVAL_ALARM             DL_RTC_COMMON_EVENT_INTERVAL_ALARM

/*!
 * @brief RTC 就绪事件
 */
#define DL_RTC_EVENT_READY                      DL_RTC_COMMON_EVENT_READY

/** @}*/

/*!
 * @brief 启用RTC电源
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enablePower                      DL_RTC_Common_enablePower

/*!
 * @brief 禁用RTC电源
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disablePower                     DL_RTC_Common_disablePower

/*!
 * @brief 检查RTC电源是否启用
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return true 如果外设寄存器访问已启用
 * @return false 如果外设寄存器访问已禁用
 */
#define DL_RTC_isPowerEnabled                   DL_RTC_Common_isPowerEnabled

/*!
 * @brief 重置RTC模块的所有寄存器
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_reset                            DL_RTC_Common_reset

/*!
 * @brief 检查RTC模块是否已重置
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return true 如果模块已重置
 * @return false 如果模块未重置
 */
#define DL_RTC_isReset                          DL_RTC_Common_isReset

/*!
 * @brief 启用RTC时钟控制
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableClockControl               DL_RTC_Common_enableClockControl

/*!
 * @brief 禁用RTC时钟控制
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableClockControl              DL_RTC_Common_disableClockControl

/*!
 * @brief 启用调试模式下的中断
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableDebugInterrupts            DL_RTC_Common_enableDebugInterrupts

/*!
 * @brief 禁用调试模式下的中断
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableDebugInterrupts           DL_RTC_Common_disableDebugInterrupts

/*!
 * @brief 启用RTC调试模式
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableDebugMode                  DL_RTC_Common_enableDebugMode

/*!
 * @brief 禁用RTC调试模式
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableDebugMode                 DL_RTC_Common_disableDebugMode

/*!
 * @brief 设置实时时钟的格式
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] formatSelect 选择RTC外设将提供时间的格式
 */
#define DL_RTC_setClockFormat                   DL_RTC_Common_setClockFormat

/*!
 * @brief 获取当前用于计数实时时钟的格式
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 实时时钟计数格式
 */
#define DL_RTC_getClockFormat                   DL_RTC_Common_getClockFormat

/*!
 * @brief 设置间隔闹钟事件条件
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] alarmEvent 间隔闹钟中断条件
 */
#define DL_RTC_setIntervalAlarm                 DL_RTC_Common_setIntervalAlarm

/*!
 * @brief 获取间隔闹钟事件条件
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 间隔闹钟事件条件
 */
#define DL_RTC_getIntervalAlarm                 DL_RTC_Common_getIntervalAlarm

/*!
 * @brief 检查RTC是否可以安全读取
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return true 如果RTC可以安全读取
 * @return false 如果RTC值正在转换中
 */
#define DL_RTC_isSafeToRead                     DL_RTC_Common_isSafeToRead

/*!
 * @brief 检查RTC是否准备好获取新的校准值
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return true 如果RTC可以获取新的校准值
 * @return false 如果RTC未准备好获取新的校准值
 */
#define DL_RTC_isReadyToCalibrate               DL_RTC_Common_isReadyToCalibrate

/*!
 * @brief 检查RTC新的校准值是否写入成功
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return true 如果校准写入成功
 * @return false 如果校准写入失败
 */
#define DL_RTC_isCalibrationWriteResultOK       DL_RTC_Common_isCalibrationWriteResultOK

/*!
 * @brief 设置偏移校准的输出频率
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] offsetFrequency 偏移校准的频率
 */
#define DL_RTC_setOffsetCalibrationFrequency    DL_RTC_Common_setOffsetCalibrationFrequency

/*!
 * @brief 设置偏移误差校准调整值
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] offsetSign 偏移校准的符号
 * @param[in] offsetValue 偏移将作为因子的值
 */
#define DL_RTC_setOffsetCalibrationAdjValue     DL_RTC_Common_setOffsetCalibrationAdjValue

/*!
 * @brief 设置偏移误差校准
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] offsetFrequency 偏移校准的频率
 * @param[in] offsetSign 偏移校准的符号
 * @param[in] offsetValue 偏移将作为因子的值
 */
#define DL_RTC_setOffsetCalibration             DL_RTC_Common_setOffsetCalibration

/*!
 * @brief 获取偏移校准频率
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 偏移校准中使用的频率
 */
#define DL_RTC_getOffsetCalibrationFrequency    DL_RTC_Common_getOffsetCalibrationFrequency

/*!
 * @brief 获取偏移校准符号
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 偏移校准中使用的符号
 */
#define DL_RTC_getOffsetCalibrationSign         DL_RTC_Common_getOffsetCalibrationSign

/*!
 * @brief 设置指定的RTC温度补偿
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] offsetDirection 校准偏移的方向
 * @param[in] offsetValue 偏移将作为因子的值
 */
#define DL_RTC_setTemperatureCompensation       DL_RTC_Common_setTemperatureCompensation

/*!
 * @brief 获取RTC温度补偿
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 累积温度补偿
 */
#define DL_RTC_getTemperatureCompensation       DL_RTC_Common_getTemperatureCompensation

/*!
 * @brief 设置RTC日历的BCD格式秒
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] seconds RTC日历秒的值，有效值为0x0-0x59
 */
#define DL_RTC_setCalendarSecondsBCD            DL_RTC_Common_setCalendarSecondsBCD

/*!
 * @brief 获取RTC日历的BCD格式秒
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的秒值，范围：0x0-0x59
 */
#define DL_RTC_getCalendarSecondsBCD            DL_RTC_Common_getCalendarSecondsBCD

/*!
 * @brief 设置RTC日历的BCD格式分
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] minutes RTC日历分的值，有效值为0x0-0x59
 */
#define DL_RTC_setCalendarMinutesBCD            DL_RTC_Common_setCalendarMinutesBCD

/*!
 * @brief 获取RTC日历的BCD格式分
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的分值，范围：0x0-0x59
 */
#define DL_RTC_getCalendarMinutesBCD            DL_RTC_Common_getCalendarMinutesBCD

/*!
 * @brief 设置RTC日历的BCD格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] hours RTC日历小时的值，有效值为0x0-0x23
 */
#define DL_RTC_setCalendarHoursBCD              DL_RTC_Common_setCalendarHoursBCD

/*!
 * @brief 获取RTC日历的BCD格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的小时值，范围：0x0-0x23
 */
#define DL_RTC_getCalendarHoursBCD              DL_RTC_Common_getCalendarHoursBCD

/*!
 * @brief 设置RTC日历的BCD格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day RTC日历星期的值，有效值为0x0-0x6
 */
#define DL_RTC_setCalendarDayOfWeekBCD          DL_RTC_Common_setCalendarDayOfWeekBCD

/*!
 * @brief 获取RTC日历的BCD格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的星期值，范围：0x0-0x6
 */
#define DL_RTC_getCalendarDayOfWeekBCD          DL_RTC_Common_getCalendarDayOfWeekBCD

/*!
 * @brief 设置RTC日历的BCD格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day RTC日历日期的值，有效值为0x1-0x31
 */
#define DL_RTC_setCalendarDayOfMonthBCD         DL_RTC_Common_setCalendarDayOfMonthBCD

/*!
 * @brief 获取RTC日历的BCD格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的日期值，范围：0x1-0x31
 */
#define DL_RTC_getCalendarDayOfMonthBCD         DL_RTC_Common_getCalendarDayOfMonthBCD

/*!
 * @brief 设置RTC日历的BCD格式月份
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] month RTC日历月份的值，有效值为0x1-0x12
 */
#define DL_RTC_setCalendarMonthBCD              DL_RTC_Common_setCalendarMonthBCD

/*!
 * @brief 获取RTC日历的BCD格式月份
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的月份值，范围：0x1-0x12
 */
#define DL_RTC_getCalendarMonthBCD              DL_RTC_Common_getCalendarMonthBCD

/*!
 * @brief 设置RTC日历的BCD格式年份
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] year RTC日历年份的值，有效值为0x0-0x4095
 */
#define DL_RTC_setCalendarYearBCD               DL_RTC_Common_setCalendarYearBCD

/*!
 * @brief 获取RTC日历的BCD格式年份
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的年份值，范围：0x0-0x4095
 */
#define DL_RTC_getCalendarYearBCD               DL_RTC_Common_getCalendarYearBCD

/*!
 * @brief 启用RTC日历闹钟1的分钟匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm1MinutesBCD           DL_RTC_Common_enableAlarm1MinutesBCD

/*!
 * @brief 禁用RTC日历闹钟1的分钟匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm1MinutesBCD          DL_RTC_Common_disableAlarm1MinutesBCD

/*!
 * @brief 设置RTC日历闹钟1的BCD格式分钟
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] minutes 闹钟1的分钟值，有效值为0x0-0x59
 */
#define DL_RTC_setAlarm1MinutesBCD              DL_RTC_Common_setAlarm1MinutesBCD

/*!
 * @brief 获取RTC日历闹钟1的BCD格式分钟
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟1的分钟值，范围：0x0-0x59
 */
#define DL_RTC_getAlarm1MinutesBCD              DL_RTC_Common_getAlarm1MinutesBCD

/*!
 * @brief 启用RTC日历闹钟1的小时匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm1HoursBCD             DL_RTC_Common_enableAlarm1HoursBCD

/*!
 * @brief 禁用RTC日历闹钟1的小时匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm1HoursBCD            DL_RTC_Common_disableAlarm1HoursBCD

/*!
 * @brief 设置RTC日历闹钟1的BCD格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] hours 闹钟1的小时值，有效值为0x0-0x23
 */
#define DL_RTC_setAlarm1HoursBCD                DL_RTC_Common_setAlarm1HoursBCD

/*!
 * @brief 获取RTC日历闹钟1的BCD格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟1的小时值，范围：0x0-0x23
 */
#define DL_RTC_getAlarm1HoursBCD                DL_RTC_Common_getAlarm1HoursBCD

/*!
 * @brief 启用RTC日历闹钟1的星期匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm1DayOfWeekBCD         DL_RTC_Common_enableAlarm1DayOfWeekBCD

/*!
 * @brief 禁用RTC日历闹钟1的星期匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm1DayOfWeekBCD        DL_RTC_Common_disableAlarm1DayOfWeekBCD

/*!
 * @brief 设置RTC日历闹钟1的BCD格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day 闹钟1的星期值，有效值为0x0-0x6
 */
#define DL_RTC_setAlarm1DayOfWeekBCD            DL_RTC_Common_setAlarm1DayOfWeekBCD

/*!
 * @brief 获取RTC日历闹钟1的BCD格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟1的星期值，范围：0x0-0x6
 */
#define DL_RTC_getAlarm1DayOfWeekBCD            DL_RTC_Common_getAlarm1DayOfWeekBCD

/*!
 * @brief 启用RTC日历闹钟1的日期匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm1DayOfMonthBCD        DL_RTC_Common_enableAlarm1DayOfMonthBCD

/*!
 * @brief 禁用RTC日历闹钟1的日期匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm1DayOfMonthBCD       DL_RTC_Common_disableAlarm1DayOfMonthBCD

/*!
 * @brief 设置RTC日历闹钟1的BCD格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day 闹钟1的日期值，有效值为0x1-0x31
 */
#define DL_RTC_setAlarm1DayOfMonthBCD           DL_RTC_Common_setAlarm1DayOfMonthBCD

/*!
 * @brief 获取RTC日历闹钟1的BCD格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟1的日期值，范围：0x1-0x31
 */
#define DL_RTC_getAlarm1DayOfMonthBCD           DL_RTC_Common_getAlarm1DayOfMonthBCD

/*!
 * @brief 启用RTC日历闹钟2的分钟匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm2MinutesBCD           DL_RTC_Common_enableAlarm2MinutesBCD

/*!
 * @brief 禁用RTC日历闹钟2的分钟匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm2MinutesBCD          DL_RTC_Common_disableAlarm2MinutesBCD

/*!
 * @brief 设置RTC日历闹钟2的BCD格式分钟
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] minutes 闹钟2的分钟值，有效值为0x0-0x59
 */
#define DL_RTC_setAlarm2MinutesBCD              DL_RTC_Common_setAlarm2MinutesBCD

/*!
 * @brief 获取RTC日历闹钟2的BCD格式分钟
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟2的分钟值，范围：0x0-0x59
 */
#define DL_RTC_getAlarm2MinutesBCD              DL_RTC_Common_getAlarm2MinutesBCD

/*!
 * @brief 启用RTC日历闹钟2的小时匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm2HoursBCD             DL_RTC_Common_enableAlarm2HoursBCD

/*!
 * @brief 禁用RTC日历闹钟2的小时匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm2HoursBCD            DL_RTC_Common_disableAlarm2HoursBCD

/*!
 * @brief 设置RTC日历闹钟2的BCD格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] hours 闹钟2的小时值，有效值为0x0-0x23
 */
#define DL_RTC_setAlarm2HoursBCD                DL_RTC_Common_setAlarm2HoursBCD

/*!
 * @brief 获取RTC日历闹钟2的BCD格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟2的小时值，范围：0x0-0x23
 */
#define DL_RTC_getAlarm2HoursBCD                DL_RTC_Common_getAlarm2HoursBCD

/*!
 * @brief 启用RTC日历闹钟2的星期匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm2DayOfWeekBCD         DL_RTC_Common_enableAlarm2DayOfWeekBCD

/*!
 * @brief 禁用RTC日历闹钟2的星期匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm2DayOfWeekBCD        DL_RTC_Common_disableAlarm2DayOfWeekBCD

/*!
 * @brief 设置RTC日历闹钟2的BCD格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day 闹钟2的星期值，有效值为0x0-0x6
 */
#define DL_RTC_setAlarm2DayOfWeekBCD            DL_RTC_Common_setAlarm2DayOfWeekBCD

/*!
 * @brief 获取RTC日历闹钟2的BCD格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟2的星期值，范围：0x0-0x6
 */
#define DL_RTC_getAlarm2DayOfWeekBCD            DL_RTC_Common_getAlarm2DayOfWeekBCD

/*!
 * @brief 启用RTC日历闹钟2的日期匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm2DayOfMonthBCD        DL_RTC_Common_enableAlarm2DayOfMonthBCD

/*!
 * @brief 禁用RTC日历闹钟2的日期匹配
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm2DayOfMonthBCD       DL_RTC_Common_disableAlarm2DayOfMonthBCD

/*!
 * @brief 设置RTC日历闹钟2的BCD格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day 闹钟2的日期值，有效值为0x1-0x31
 */
#define DL_RTC_setAlarm2DayOfMonthBCD           DL_RTC_Common_setAlarm2DayOfMonthBCD

/*!
 * @brief 获取RTC日历闹钟2的BCD格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟2的日期值，范围：0x1-0x31
 */
#define DL_RTC_getAlarm2DayOfMonthBCD           DL_RTC_Common_getAlarm2DayOfMonthBCD

/*!
 * @brief 设置RTC日历的二进制格式秒
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] seconds RTC日历秒的值，有效值为0-59
 */
#define DL_RTC_setCalendarSecondsBinary         DL_RTC_Common_setCalendarSecondsBinary

/*!
 * @brief 获取RTC日历的二进制格式秒
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的秒值，范围：0-59
 */
#define DL_RTC_getCalendarSecondsBinary         DL_RTC_Common_getCalendarSecondsBinary

/*!
 * @brief 设置RTC日历的二进制格式分
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] minutes RTC日历分的值，有效值为0-59
 */
#define DL_RTC_setCalendarMinutesBinary         DL_RTC_Common_setCalendarMinutesBinary

/*!
 * @brief 获取RTC日历的二进制格式分
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的分值，范围：0-59
 */
#define DL_RTC_getCalendarMinutesBinary         DL_RTC_Common_getCalendarMinutesBinary

/*!
 * @brief 设置RTC日历的二进制格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] hours RTC日历小时的值，有效值为0-23
 */
#define DL_RTC_setCalendarHoursBinary           DL_RTC_Common_setCalendarHoursBinary

/*!
 * @brief 获取RTC日历的二进制格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的小时值，范围：0-23
 */
#define DL_RTC_getCalendarHoursBinary           DL_RTC_Common_getCalendarHoursBinary

/*!
 * @brief 设置RTC日历的二进制格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day RTC日历星期的值，有效值为0-6
 */
#define DL_RTC_setCalendarDayOfWeekBinary       DL_RTC_Common_setCalendarDayOfWeekBinary

/*!
 * @brief 获取RTC日历的二进制格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的星期值，范围：0-6
 */
#define DL_RTC_getCalendarDayOfWeekBinary       DL_RTC_Common_getCalendarDayOfWeekBinary

/*!
 * @brief 设置RTC日历的二进制格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day RTC日历日期的值，有效值为1-31
 */
#define DL_RTC_setCalendarDayOfMonthBinary      DL_RTC_Common_setCalendarDayOfMonthBinary

/*!
 * @brief 获取RTC日历的二进制格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的日期值，范围：1-31
 */
#define DL_RTC_getCalendarDayOfMonthBinary      DL_RTC_Common_getCalendarDayOfMonthBinary

/*!
 * @brief 设置RTC日历的二进制格式月份
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] month RTC日历月份的值，有效值为1-12
 */
#define DL_RTC_setCalendarMonthBinary           DL_RTC_Common_setCalendarMonthBinary

/*!
 * @brief 获取RTC日历的二进制格式月份
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的月份值，范围：1-12
 */
#define DL_RTC_getCalendarMonthBinary           DL_RTC_Common_getCalendarMonthBinary

/*!
 * @brief 设置RTC日历的二进制格式年份
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] year RTC日历年份的值，有效值为0-4095
 */
#define DL_RTC_setCalendarYearBinary            DL_RTC_Common_setCalendarYearBinary

/*!
 * @brief 获取RTC日历的二进制格式年份
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return RTC日历的年份值，范围：0-4095
 */
#define DL_RTC_getCalendarYearBinary            DL_RTC_Common_getCalendarYearBinary

/*!
 * @brief 启用RTC日历闹钟1的分钟匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm1MinutesBinary        DL_RTC_Common_enableAlarm1MinutesBinary

/*!
 * @brief 禁用RTC日历闹钟1的分钟匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm1MinutesBinary       DL_RTC_Common_disableAlarm1MinutesBinary

/*!
 * @brief 设置RTC日历闹钟1的二进制格式分钟
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] minutes 闹钟1的分钟值，有效值为0-59
 */
#define DL_RTC_setAlarm1MinutesBinary           DL_RTC_Common_setAlarm1MinutesBinary

/*!
 * @brief 获取RTC日历闹钟1的二进制格式分钟
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟1的分钟值，范围：0-59
 */
#define DL_RTC_getAlarm1MinutesBinary           DL_RTC_Common_getAlarm1MinutesBinary

/*!
 * @brief 启用RTC日历闹钟1的小时匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm1HoursBinary          DL_RTC_Common_enableAlarm1HoursBinary

/*!
 * @brief 禁用RTC日历闹钟1的小时匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm1HoursBinary         DL_RTC_Common_disableAlarm1HoursBinary

/*!
 * @brief 设置RTC日历闹钟1的二进制格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] hours 闹钟1的小时值，有效值为0-23
 */
#define DL_RTC_setAlarm1HoursBinary             DL_RTC_Common_setAlarm1HoursBinary

/*!
 * @brief 获取RTC日历闹钟1的二进制格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟1的小时值，范围：0-23
 */
#define DL_RTC_getAlarm1HoursBinary             DL_RTC_Common_getAlarm1HoursBinary

/*!
 * @brief 启用RTC日历闹钟1的星期匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm1DayOfWeekBinary      DL_RTC_Common_enableAlarm1DayOfWeekBinary

/*!
 * @brief 禁用RTC日历闹钟1的星期匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm1DayOfWeekBinary     DL_RTC_Common_disableAlarm1DayOfWeekBinary

/*!
 * @brief 设置RTC日历闹钟1的二进制格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day 闹钟1的星期值，有效值为0-6
 */
#define DL_RTC_setAlarm1DayOfWeekBinary         DL_RTC_Common_setAlarm1DayOfWeekBinary

/*!
 * @brief 获取RTC日历闹钟1的二进制格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟1的星期值，范围：0-6
 */
#define DL_RTC_getAlarm1DayOfWeekBinary         DL_RTC_Common_getAlarm1DayOfWeekBinary

/*!
 * @brief 启用RTC日历闹钟1的日期匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm1DayOfMonthBinary     DL_RTC_Common_enableAlarm1DayOfMonthBinary

/*!
 * @brief 禁用RTC日历闹钟1的日期匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm1DayOfMonthBinary    DL_RTC_Common_disableAlarm1DayOfMonthBinary

/*!
 * @brief 设置RTC日历闹钟1的二进制格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day 闹钟1的日期值，有效值为1-31
 */
#define DL_RTC_setAlarm1DayOfMonthBinary        DL_RTC_Common_setAlarm1DayOfMonthBinary

/*!
 * @brief 获取RTC日历闹钟1的二进制格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟1的日期值，范围：1-31
 */
#define DL_RTC_getAlarm1DayOfMonthBinary        DL_RTC_Common_getAlarm1DayOfMonthBinary

/*!
 * @brief 启用RTC日历闹钟2的分钟匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm2MinutesBinary        DL_RTC_Common_enableAlarm2MinutesBinary

/*!
 * @brief 禁用RTC日历闹钟2的分钟匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm2MinutesBinary       DL_RTC_Common_disableAlarm2MinutesBinary

/*!
 * @brief 设置RTC日历闹钟2的二进制格式分钟
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] minutes 闹钟2的分钟值，有效值为0-59
 */
#define DL_RTC_setAlarm2MinutesBinary           DL_RTC_Common_setAlarm2MinutesBinary

/*!
 * @brief 获取RTC日历闹钟2的二进制格式分钟
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟2的分钟值，范围：0-59
 */
#define DL_RTC_getAlarm2MinutesBinary           DL_RTC_Common_getAlarm2MinutesBinary

/*!
 * @brief 启用RTC日历闹钟2的小时匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm2HoursBinary          DL_RTC_Common_enableAlarm2HoursBinary

/*!
 * @brief 禁用RTC日历闹钟2的小时匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm2HoursBinary         DL_RTC_Common_disableAlarm2HoursBinary

/*!
 * @brief 设置RTC日历闹钟2的二进制格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] hours 闹钟2的小时值，有效值为0-23
 */
#define DL_RTC_setAlarm2HoursBinary             DL_RTC_Common_setAlarm2HoursBinary

/*!
 * @brief 获取RTC日历闹钟2的二进制格式小时
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟2的小时值，范围：0-23
 */
#define DL_RTC_getAlarm2HoursBinary             DL_RTC_Common_getAlarm2HoursBinary

/*!
 * @brief 启用RTC日历闹钟2的星期匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm2DayOfWeekBinary      DL_RTC_Common_enableAlarm2DayOfWeekBinary

/*!
 * @brief 禁用RTC日历闹钟2的星期匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm2DayOfWeekBinary     DL_RTC_Common_disableAlarm2DayOfWeekBinary

/*!
 * @brief 设置RTC日历闹钟2的二进制格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day 闹钟2的星期值，有效值为0-6
 */
#define DL_RTC_setAlarm2DayOfWeekBinary         DL_RTC_Common_setAlarm2DayOfWeekBinary

/*!
 * @brief 获取RTC日历闹钟2的二进制格式星期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟2的星期值，范围：0-6
 */
#define DL_RTC_getAlarm2DayOfWeekBinary         DL_RTC_Common_getAlarm2DayOfWeekBinary

/*!
 * @brief 启用RTC日历闹钟2的日期匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableAlarm2DayOfMonthBinary     DL_RTC_Common_enableAlarm2DayOfMonthBinary

/*!
 * @brief 禁用RTC日历闹钟2的日期匹配（二进制格式）
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableAlarm2DayOfMonthBinary    DL_RTC_Common_disableAlarm2DayOfMonthBinary

/*!
 * @brief 设置RTC日历闹钟2的二进制格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] day 闹钟2的日期值，有效值为1-31
 */
#define DL_RTC_setAlarm2DayOfMonthBinary        DL_RTC_Common_setAlarm2DayOfMonthBinary

/*!
 * @brief 获取RTC日历闹钟2的二进制格式日期
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 闹钟2的日期值，范围：1-31
 */
#define DL_RTC_getAlarm2DayOfMonthBinary        DL_RTC_Common_getAlarm2DayOfMonthBinary

/*!
 * @brief 设置预分频器事件
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] prescaler0Divide 预分频器0的分频值
 * @param[in] prescaler1Divide 预分频器1的分频值
 */
#define DL_RTC_setPrescalerEvents               DL_RTC_Common_setPrescalerEvents

/*!
 * @brief 设置周期性闹钟0
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] divide 预分频器0的分频值
 */
#define DL_RTC_setPeriodicAlarm0                DL_RTC_Common_setPeriodicAlarm0

/*!
 * @brief 设置周期性闹钟1
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] divide 预分频器1的分频值
 */
#define DL_RTC_setPeriodicAlarm1                DL_RTC_Common_setPeriodicAlarm1

/*!
 * @brief 获取预分频器0的值
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 预分频器0的当前值
 */
#define DL_RTC_getPrescaler0                    DL_RTC_Common_getPrescaler0

/*!
 * @brief 获取预分频器1的值
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 预分频器1的当前值
 */
#define DL_RTC_getPrescaler1                    DL_RTC_Common_getPrescaler1

/*!
 * @brief 启用RTC中断
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] interruptMask 要启用的中断掩码
 */
#define DL_RTC_enableInterrupt                  DL_RTC_Common_enableInterrupt

/*!
 * @brief 禁用RTC中断
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] interruptMask 要禁用的中断掩码
 */
#define DL_RTC_disableInterrupt                 DL_RTC_Common_disableInterrupt

/*!
 * @brief 获取已启用的中断
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 已启用的中断掩码
 */
#define DL_RTC_getEnabledInterrupts             DL_RTC_Common_getEnabledInterrupts

/*!
 * @brief 获取已启用的中断状态
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 已启用的中断状态
 */
#define DL_RTC_getEnabledInterruptStatus        DL_RTC_Common_getEnabledInterruptStatus

/*!
 * @brief 获取原始中断状态
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 原始中断状态
 */
#define DL_RTC_getRawInterruptStatus            DL_RTC_Common_getRawInterruptStatus

/*!
 * @brief 获取挂起的中断
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 挂起的中断索引
 */
#define DL_RTC_getPendingInterrupt              DL_RTC_Common_getPendingInterrupt

/*!
 * @brief 清除中断状态
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] interruptMask 要清除的中断掩码
 */
#define DL_RTC_clearInterruptStatus             DL_RTC_Common_clearInterruptStatus

/*!
 * @brief 启用RTC事件
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] eventMask 要启用的事件掩码
 */
#define DL_RTC_enableEvent                      DL_RTC_Common_enableEvent

/*!
 * @brief 禁用RTC事件
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] eventMask 要禁用的事件掩码
 */
#define DL_RTC_disableEvent                     DL_RTC_Common_disableEvent

/*!
 * @brief 获取已启用的事件
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 已启用的事件掩码
 */
#define DL_RTC_getEnabledEvents                 DL_RTC_Common_getEnabledEvents

/*!
 * @brief 获取已启用的事件状态
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 已启用的事件状态
 */
#define DL_RTC_getEnabledEventStatus            DL_RTC_Common_getEnabledEventStatus

/*!
 * @brief 获取原始事件状态
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 原始事件状态
 */
#define DL_RTC_getRawEventsStatus               DL_RTC_Common_getRawEventsStatus

/*!
 * @brief 清除事件状态
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] eventMask 要清除的事件掩码
 */
#define DL_RTC_clearEventsStatus                DL_RTC_Common_clearEventsStatus

/*!
 * @brief 设置发布者通道ID
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] chanID 通道ID
 */
#define DL_RTC_setPublisherChanID               DL_RTC_Common_setPublisherChanID

/*!
 * @brief 获取发布者通道ID
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @return 通道ID
 */
#define DL_RTC_getPublisherChanID               DL_RTC_Common_getPublisherChanID

/*!
 * @brief 初始化RTC日历
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] calendar 日历结构体指针
 */
#define DL_RTC_initCalendar                     DL_RTC_Common_initCalendar

/*!
 * @brief 获取RTC日历时间
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[out] calendar 日历结构体指针，用于存储当前时间
 */
#define DL_RTC_getCalendarTime                  DL_RTC_Common_getCalendarTime

/*!
 * @brief 设置RTC日历闹钟1
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] alarm 闹钟结构体指针
 */
#define DL_RTC_setCalendarAlarm1                DL_RTC_Common_setCalendarAlarm1

/*!
 * @brief 获取RTC日历闹钟1
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[out] alarm 闹钟结构体指针，用于存储当前闹钟设置
 */
#define DL_RTC_getCalendarAlarm1                DL_RTC_Common_getCalendarAlarm1

/*!
 * @brief 启用RTC日历闹钟1
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableCalendarAlarm1             DL_RTC_Common_enableCalendarAlarm1

/*!
 * @brief 禁用RTC日历闹钟1
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableCalendarAlarm1            DL_RTC_Common_disableCalendarAlarm1

/*!
 * @brief 设置RTC日历闹钟2
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[in] alarm 闹钟结构体指针
 */
#define DL_RTC_setCalendarAlarm2                DL_RTC_Common_setCalendarAlarm2

/*!
 * @brief 获取RTC日历闹钟2
 * @param[in] rtc 指向RTC寄存器结构体的指针
 * @param[out] alarm 闹钟结构体指针，用于存储当前闹钟设置
 */
#define DL_RTC_getCalendarAlarm2                DL_RTC_Common_getCalendarAlarm2

/*!
 * @brief 启用RTC日历闹钟2
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_enableCalendarAlarm2             DL_RTC_Common_enableCalendarAlarm2

/*!
 * @brief 禁用RTC日历闹钟2
 * @param[in] rtc 指向RTC寄存器结构体的指针
 */
#define DL_RTC_disableCalendarAlarm2            DL_RTC_Common_disableCalendarAlarm2

/* clang-format on */

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_RTC__ */

#endif /* ti_dl_dl_rtc__include */
/** @}*/
