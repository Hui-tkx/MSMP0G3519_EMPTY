/**
 * @file dac_test.c
 * @brief DAC 测试库 (DMA 波形播放测试)
 *
 * 依次播放: 方波 4s → 三角波 4s → 正弦波 (持续)
 */
#include "dac_test.h"
#include "delay.h"

#define DAC_TEST_DURATION_MS  (4000U)

void DAC_TEST_runAll(UART_Regs *uart)
{
    static uint16_t buf[DAC_WAVE_BUF_SIZE];
    static bool done = false;
    if (done) return;
    done = true;

    /* 中心值 2048 (1.65V), 振幅 1800 (峰峰值 ≈ 2.9V) */
    const uint16_t offsetCode = 2048;
    const uint16_t ampCode    = 1800;
    uint32_t freq = DAC_SAMPLE_RATE_HZ / DAC_WAVE_BUF_SIZE;

    UART_printf(uart, "\r\nDAC DMA WAVEFORM TEST (%uHz, %u samples)\r\n",
                freq, DAC_WAVE_BUF_SIZE);

    /* 方波 4s */
    UART_printf(uart, "Square %uHz 4s...\r\n", freq);
    DAC_playWaveform(buf, DAC_WAVE_BUF_SIZE, DAC_WAVE_SQUARE, offsetCode, ampCode);
    delay_ms(DAC_TEST_DURATION_MS);
    DAC_waveStop();

    /* 三角波 4s */
    UART_printf(uart, "Triangle %uHz 4s...\r\n", freq);
    DAC_playWaveform(buf, DAC_WAVE_BUF_SIZE, DAC_WAVE_TRIANGLE, offsetCode, ampCode);
    delay_ms(DAC_TEST_DURATION_MS);
    DAC_waveStop();

    /* 正弦波持续播放 */
    UART_printf(uart, "Sine %uHz looping...\r\n", freq);
    DAC_playWaveform(buf, DAC_WAVE_BUF_SIZE, DAC_WAVE_SINE, offsetCode, ampCode);
    UART_printf(uart, "Done, sine wave looping.\r\n");
}
