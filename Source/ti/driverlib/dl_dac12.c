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

#include <ti/driverlib/dl_dac12.h>

#ifdef __MSPM0_HAS_DAC12__

/**
 * @brief 初始化DAC12模块
 *
 * 此函数初始化DAC12的所有常用可配置选项，包括数据格式、DAC模块配置、
 * FIFO设置和采样定时器设置。
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 * @param config  指向DAC12配置结构体的指针
 *
 * @par 使用示例
 * @code
 * DL_DAC12_Config dacConfig = {
 *     .outputEnable = DL_DAC12_OUTPUT_ENABLED,
 *     .resolution = DL_DAC12_RESOLUTION_12BIT,
 *     .representation = DL_DAC12_REPRESENTATION_BINARY,
 *     .voltageReferenceSource = DL_DAC12_VREF_SOURCE_VDDA_VSSA,
 *     .amplifierSetting = DL_DAC12_AMP_ON,
 *     .fifoEnable = DL_DAC12_FIFO_DISABLED,
 *     .fifoTriggerSource = DL_DAC12_FIFO_TRIGGER_SAMPLETIMER,
 *     .dmaTriggerEnable = DL_DAC12_DMA_TRIGGER_DISABLED,
 *     .dmaTriggerThreshold = DL_DAC12_FIFO_THRESHOLD_ONE_QTR_EMPTY,
 *     .sampleTimeGeneratorEnable = DL_DAC12_SAMPLETIMER_DISABLE,
 *     .sampleRate = DL_DAC12_SAMPLES_PER_SECOND_1K
 * };
 * DL_DAC12_init(DAC12, &dacConfig);
 * @endcode
 */
void DL_DAC12_init(DAC12_Regs *dac12, const DL_DAC12_Config *config)
{
    /* CTL0 - 输入数据格式化 */
    DL_DAC12_configDataFormat(
        dac12, config->representation, config->resolution);

    /* CTL1 - DAC模块 */
    DL_Common_updateReg(&dac12->CTL1,
        (uint32_t) config->outputEnable |
            (uint32_t) config->voltageReferenceSource |
            (uint32_t) config->amplifierSetting,
        DAC12_CTL1_OPS_MASK | DAC12_CTL1_REFSP_MASK | DAC12_CTL1_REFSN_MASK |
            DAC12_CTL1_AMPEN_MASK | DAC12_CTL1_AMPHIZ_MASK);

    /* CTL2 - FIFO */
    DL_Common_updateReg(&dac12->CTL2,
        ((uint32_t) config->fifoEnable | (uint32_t) config->fifoTriggerSource |
            (uint32_t) config->dmaTriggerEnable |
            (uint32_t) config->dmaTriggerThreshold),
        DAC12_CTL2_DMATRIGEN_MASK | DAC12_CTL2_FIFOTH_MASK |
            DAC12_CTL2_FIFOEN_MASK | DAC12_CTL2_FIFOTRIGSEL_MASK);

    /* CTL3 - 采样时间发生器 */
    DL_Common_updateReg(&dac12->CTL3,
        ((uint32_t) config->sampleTimeGeneratorEnable |
            (uint32_t) config->sampleRate),
        DAC12_CTL3_STIMCONFIG_MASK | DAC12_CTL3_STIMEN_MASK);
}

/**
 * @brief 阻塞方式输出8位数据到DAC
 *
 * 此函数等待FIFO不满，然后写入单个值。适用于FIFO禁用的场景
 * (如DC生成) 或FIFO使能时写入数据。
 *
 * @param dac12  指向DAC12寄存器结构体的指针
 * @param data   要写入的数据值 (0x00 - 0xFF)
 *
 * @par 使用示例
 * @code
 * // 输出8位值 128 到DAC
 * DL_DAC12_outputBlocking8(DAC12, 128);
 * @endcode
 */
void DL_DAC12_outputBlocking8(DAC12_Regs *dac12, uint8_t data)
{
    // 等待FIFO不满
    while (DL_DAC12_isFIFOFull(dac12)) {
        ;
    }

    // 输出数据
    DL_DAC12_output8(dac12, data);
}

/**
 * @brief 阻塞方式输出12位数据到DAC
 *
 * 此函数等待FIFO不满，然后写入单个值。适用于FIFO禁用的场景
 * (如DC生成) 或FIFO使能时写入数据。
 *
 * @param dac12  指向DAC12寄存器结构体的指针
 * @param data   要写入的数据值 (0x000 - 0xFFF)
 *
 * @par 使用示例
 * @code
 * // 输出12位值 2048 到DAC
 * DL_DAC12_outputBlocking12(DAC12, 2048);
 * @endcode
 */
void DL_DAC12_outputBlocking12(DAC12_Regs *dac12, uint16_t data)
{
    // 等待FIFO不满
    while (DL_DAC12_isFIFOFull(dac12)) {
        ;
    }

    // 输出数据
    DL_DAC12_output12(dac12, data);
}

/**
 * @brief 用8位数据填充DAC FIFO
 *
 * 从数据缓冲区尽可能多地向FIFO写入值，然后在计数匹配或FIFO满时退出。
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 * @param buffer  包含要写入的数据值 (0x00 - 0xFF) 的数组
 * @param count   缓冲区长度，或最大期望传输数量
 *
 * @return 成功传输到FIFO的计数
 *
 * @retval 0 - min(FIFO空槽数量, count)
 *
 * @par 使用示例
 * @code
 * uint8_t waveform[100];
 * for (int i = 0; i < 100; i++) {
 *     waveform[i] = i * 2;  // 生成简单波形
 * }
 * uint32_t written = DL_DAC12_fillFIFO8(DAC12, waveform, 100);
 * @endcode
 */
uint32_t DL_DAC12_fillFIFO8(
    DAC12_Regs *dac12, const uint8_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++) {
        if (!DL_DAC12_isFIFOFull(dac12)) {
            DL_DAC12_output8(dac12, buffer[i]);
        } else {
            break;
        }
    }

    return i;
}

/**
 * @brief 用12位数据填充DAC FIFO
 *
 * 从数据缓冲区尽可能多地向FIFO写入值，然后在计数匹配或FIFO满时退出。
 *
 * @param dac12   指向DAC12寄存器结构体的指针
 * @param buffer  包含要写入的数据值 (0x00 - 0xFFF) 的数组
 * @param count   缓冲区长度，或最大期望传输数量
 *
 * @return 成功传输到FIFO的计数
 *
 * @retval 0 - min(FIFO空槽数量, count)
 *
 * @par 使用示例
 * @code
 * uint16_t waveform[100];
 * for (int i = 0; i < 100; i++) {
 *     waveform[i] = i * 16;  // 生成12位波形
 * }
 * uint32_t written = DL_DAC12_fillFIFO12(DAC12, waveform, 100);
 * @endcode
 */
uint32_t DL_DAC12_fillFIFO12(
    DAC12_Regs *dac12, const uint16_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++) {
        if (!DL_DAC12_isFIFOFull(dac12)) {
            DL_DAC12_output12(dac12, buffer[i]);
        } else {
            break;
        }
    }

    return i;
}

/**
 * @brief 执行阻塞式DAC自校准
 *
 * 启动校准并等待校准完成。在校准期间，输出为三态状态，
 * CALON位将保持设置状态直到校准完成。
 *
 * @param dac12  指向DAC12寄存器结构体的指针
 *
 * @pre  应先配置放大器设置 @ref DL_DAC12_setAmplifier
 * @post 校准完成，CALDATA被锁定以防止写入
 *
 * @par 使用示例
 * @code
 * // 配置放大器后进行自校准
 * DL_DAC12_setAmplifier(DAC12, DL_DAC12_AMP_ON);
 * DL_DAC12_performSelfCalibrationBlocking(DAC12);
 * @endcode
 */
void DL_DAC12_performSelfCalibrationBlocking(DAC12_Regs *dac12)
{
    DL_DAC12_startCalibration(dac12);
    /*
     * 校准期间为三态状态，CALON将保持设置
     * 防止函数在校准完成前退出。
     */
    while (DL_DAC12_isCalibrationRunning(dac12)) {
        ;
    }
}

#endif /* __MSPM0_HAS_DAC12__ */
