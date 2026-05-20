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
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ti/driverlib/dl_adc12.h>

#ifdef __MSPM0_HAS_ADC12__

/**
 * @brief 配置ADC12采样时钟
 *
 * 此函数用于配置ADC12的采样时钟源、分频比和频率范围。
 *
 * @param adc12    指向ADC12寄存器结构体的指针
 * @param config   指向时钟配置结构体的指针，包含以下成员:
 *                - clockSel:    时钟源选择 (SYSOSC/ULPCLK/HFCLK)
 *                - divideRatio: 分频比 (1/2/4/8/16/24/32/48)
 *                - freqRange:   频率范围 (1-4MHz到40-48MHz)
 *
 * @par 使用示例
 * @code
 * DL_ADC12_ClockConfig clockConfig = {
 *     .clockSel = DL_ADC12_CLOCK_SYSOSC,           // 选择SYSOSC作为时钟源
 *     .freqRange = DL_ADC12_CLOCK_FREQ_RANGE_1_TO_4, // 频率范围1-4MHz
 *     .divideRatio = DL_ADC12_CLOCK_DIVIDE_1      // 分频1
 * };
 * DL_ADC12_setClockConfig(ADC12, &clockConfig);
 * @endcode
 */
void DL_ADC12_setClockConfig(
    ADC12_Regs *adc12, const DL_ADC12_ClockConfig *config)
{
    // 解锁时钟配置寄存器并设置时钟源
    DL_Common_updateReg(&adc12->ULLMEM.GPRCM.CLKCFG,
        (ADC12_CLKCFG_KEY_UNLOCK_W | (uint32_t) config->clockSel),
        (ADC12_CLKCFG_KEY_MASK | ADC12_CLKCFG_SAMPCLK_MASK));

    // 设置采样时钟分频比
    DL_Common_updateReg(&adc12->ULLMEM.CTL0, (uint32_t) config->divideRatio,
        ADC12_CTL0_SCLKDIV_MASK);

    // 设置时钟频率范围
    adc12->ULLMEM.CLKFREQ = (uint32_t) config->freqRange;
}

/**
 * @brief 获取ADC12采样时钟配置
 *
 * 此函数用于获取ADC12当前配置的时钟源、分频比和频率范围。
 *
 * @param adc12   指向ADC12寄存器结构体的指针
 * @param config  指向时钟配置结构体的指针，用于存储获取的配置
 *
 * @par 使用示例
 * @code
 * DL_ADC12_ClockConfig clockConfig;
 * DL_ADC12_getClockConfig(ADC12, &clockConfig);
 * // 现在可以读取 clockConfig.clockSel, clockConfig.divideRatio, clockConfig.freqRange
 * @endcode
 */
void DL_ADC12_getClockConfig(
    const ADC12_Regs *adc12, DL_ADC12_ClockConfig *config)
{
    // 读取时钟源配置
    uint32_t clockConfig =
        adc12->ULLMEM.GPRCM.CLKCFG & ADC12_CLKCFG_SAMPCLK_MASK;
    // 读取分频比配置
    uint32_t divideRatio = adc12->ULLMEM.CTL0 & ADC12_CTL0_SCLKDIV_MASK;
    // 读取频率范围配置
    uint32_t freqRange   = adc12->ULLMEM.CLKFREQ & ADC12_CLKFREQ_FRANGE_MASK;

    // 将读取的值存储到配置结构体中
    config->clockSel = (DL_ADC12_CLOCK)(clockConfig);
    config->divideRatio = (DL_ADC12_CLOCK_DIVIDE)(divideRatio);
    config->freqRange = (DL_ADC12_CLOCK_FREQ_RANGE)(freqRange);
}

#endif /* __MSPM0_HAS_ADC12__ */
