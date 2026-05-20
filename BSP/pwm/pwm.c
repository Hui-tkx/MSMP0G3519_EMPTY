/** 
  * @file         pwm.c 
  * @details      PWM控制库实现文件，用于实现PWM相关的函数
  * @author       zjs 
  */ 

#include "pwm.h"
#include "../delay/delay.h"

/* 全局变量 */
static uint32_t g_pwmPeriod = 0;  /* PWM周期 */
static uint8_t g_pwmDuty[2] = {0U, 0U};

static int32_t PWM_getChannelIndex(uint8_t channel)
{
    if (channel == PWM_CHANNEL_0) {
        return 0;
    }

    if (channel == PWM_CHANNEL_1) {
        return 1;
    }

    return -1;
}

static uint32_t PWM_calculatePeriod(uint32_t freq)
{
    uint32_t periodCounts;

    if (freq == 0U) {
        return 0U;
    }

    periodCounts = PWM_0_INST_CLK_FREQ / freq;
    if (periodCounts == 0U) {
        return 0U;
    }

    return periodCounts - 1U;
}

static uint32_t PWM_calculateCompareValue(uint8_t duty)
{
    uint32_t compareValue;

    if ((g_pwmPeriod == 0U) || (duty == 0U)) {
        return 0U;
    }

    if (duty >= 100U) {
        return g_pwmPeriod;
    }

    compareValue = ((g_pwmPeriod + 1U) * duty) / 100U;
    if (compareValue > g_pwmPeriod) {
        compareValue = g_pwmPeriod;
    }

    return compareValue;
}

/**
 * @brief 初始化PWM模块
 * @param freq PWM频率，单位Hz
 * @return 无
 * @note 初始化PWM模块，设置定时器和通道
 */
void PWM_init(uint32_t freq)
{
    g_pwmPeriod = PWM_calculatePeriod(freq);
    if (g_pwmPeriod == 0U) {
        return;
    }

    g_pwmDuty[0] = 0U;
    g_pwmDuty[1] = 0U;
    
    /* 启动TimerA时钟 */
    DL_TimerA_enableClock(PWM_INST);
    
    /* 设置TimerA为PWM模式 */
    DL_TimerA_setCounterMode(PWM_INST, DL_TIMER_COUNT_MODE_DOWN);
    
    /* 设置PWM周期 */
    DL_TimerA_setLoadValue(PWM_INST, g_pwmPeriod);
    
    /* 初始化通道0 */
    DL_TimerA_setCaptureCompareValue(PWM_INST, 0, PWM_CHANNEL_0);
    DL_TimerA_setCaptureCompareCtl(PWM_INST, DL_TIMER_CC_MODE_COMPARE, 0, PWM_CHANNEL_0);
    DL_TimerA_setCaptureCompareOutCtl(PWM_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW, DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL, PWM_CHANNEL_0);
    
    /* 初始化通道1 */
    DL_TimerA_setCaptureCompareValue(PWM_INST, 0, PWM_CHANNEL_1);
    DL_TimerA_setCaptureCompareCtl(PWM_INST, DL_TIMER_CC_MODE_COMPARE, 0, PWM_CHANNEL_1);
    DL_TimerA_setCaptureCompareOutCtl(PWM_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW, DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL, PWM_CHANNEL_1);
    
    /* 启动TimerA */
    DL_TimerA_startCounter(PWM_INST);
}

/**
 * @brief 设置PWM占空比
 * @param channel PWM通道，使用PWM_CHANNEL_0或PWM_CHANNEL_1
 * @param duty 占空比，范围0-100
 * @return 无
 * @note 设置指定通道的PWM占空比
 */
void PWM_setDuty(uint8_t channel, uint8_t duty)
{
    int32_t channelIndex = PWM_getChannelIndex(channel);

    if (channelIndex < 0) {
        return;
    }

    if (duty > 100U) {
        duty = 100U;
    }

    g_pwmDuty[channelIndex] = duty;
    DL_TimerA_setCaptureCompareValue(PWM_INST, PWM_calculateCompareValue(duty), channel);
}

/**
 * @brief 设置PWM频率
 * @param freq PWM频率，单位Hz
 * @return 无
 * @note 设置PWM的频率
 */
void PWM_setFreq(uint32_t freq)
{
    g_pwmPeriod = PWM_calculatePeriod(freq);
    if (g_pwmPeriod == 0U) {
        return;
    }

    DL_TimerA_setLoadValue(PWM_INST, g_pwmPeriod);
    DL_TimerA_setCaptureCompareValue(PWM_INST, PWM_calculateCompareValue(g_pwmDuty[0]), PWM_CHANNEL_0);
    DL_TimerA_setCaptureCompareValue(PWM_INST, PWM_calculateCompareValue(g_pwmDuty[1]), PWM_CHANNEL_1);
}

/**
 * @brief 启动PWM输出
 * @param channel PWM通道，使用PWM_CHANNEL_0或PWM_CHANNEL_1
 * @return 无
 * @note 启动指定通道的PWM输出
 */
void PWM_start(uint8_t channel)
{
    if (channel == PWM_CHANNEL_0) {
        DL_TimerA_setCCPOutputDisabledAdv(PWM_INST, DL_TIMER_CCP0_DIS_OUT_ADV_SET_BY_OCTL);
    } else if (channel == PWM_CHANNEL_1) {
        DL_TimerA_setCCPOutputDisabledAdv(PWM_INST, DL_TIMER_CCP1_DIS_OUT_ADV_SET_BY_OCTL);
    }
}

/**
 * @brief 停止PWM输出
 * @param channel PWM通道，使用PWM_CHANNEL_0或PWM_CHANNEL_1
 * @return 无
 * @note 停止指定通道的PWM输出
 */
void PWM_stop(uint8_t channel)
{
    if (channel == PWM_CHANNEL_0) {
        DL_TimerA_setCCPOutputDisabledAdv(PWM_INST, DL_TIMER_CCP0_DIS_OUT_ADV_FORCE_LOW);
    } else if (channel == PWM_CHANNEL_1) {
        DL_TimerA_setCCPOutputDisabledAdv(PWM_INST, DL_TIMER_CCP1_DIS_OUT_ADV_FORCE_LOW);
    }
}

/**
 * @brief 启动所有PWM通道
 * @return 无
 * @note 启动所有PWM通道的输出
 */
void PWM_startAll(void)
{
    /* 启动通道0 */
    PWM_start(PWM_CHANNEL_0);
    
    /* 启动通道1 */
    PWM_start(PWM_CHANNEL_1);
}

/**
 * @brief 停止所有PWM通道
 * @return 无
 * @note 停止所有PWM通道的输出
 */
void PWM_stopAll(void)
{
    /* 停止通道0 */
    PWM_stop(PWM_CHANNEL_0);
    
    /* 停止通道1 */
    PWM_stop(PWM_CHANNEL_1);
}

