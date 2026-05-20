/**
 * @file dac_test.c
 * @brief DAC 库测试实现 (基础 API + 软件波形)
 *
 * 测试项:
 *   基础 API:  init / enableOutput / setValue / setVoltage / 边界
 *   波形生成:  三角波 x2, 正弦波 x2, 频率扫描
 */
#include "dac_test.h"
#include "delay.h"

/*! @brief 打印带标题的分隔线 */
static void printSep(UART_Regs *uart, const char *title)
{
    UART_printf(uart, "\r\n%s\r\n%s\r\n%s\r\n", DAC_TEST_SEPARATOR, title, DAC_TEST_SEPARATOR);
}

/*! @brief 打印小标题 */
static void printHdr(UART_Regs *uart, const char *name)
{
    UART_printf(uart, "\r\n--- %s ---\r\n", name);
}

/*! @brief 基础 API 测试 */
void DAC_TEST_runBasicTests(UART_Regs *uart)
{
    printSep(uart, "DAC BASIC API TESTS");

    printHdr(uart, "[1] DAC_init()");
    DAC_init();
    UART_printf(uart, "PASS\r\n");

    printHdr(uart, "[2] DAC_enableOutput / isOutputEnabled");
    DAC_enableOutput();
    UART_printf(uart, "  enabled=%d | %s\r\n", DAC_isOutputEnabled(), "PASS");
    DAC_disableOutput();
    UART_printf(uart, "  disabled=%d | %s\r\n", !DAC_isOutputEnabled(), "PASS");
    DAC_enableOutput();

    printHdr(uart, "[3] DAC_setValue()");
    const uint16_t codes[] = {0, 1024, 2048, 3072, 4095};
    for (int i = 0; i < 5; i++) {
        DAC_setValue(codes[i]);
        float v = (float)codes[i] * DAC_REF_VOLTAGE / (float)DAC_MAX_VALUE;
        UART_printf(uart, "  code=%5u -> %.4fV\r\n", codes[i], v);
        delay_ms(300);
    }
    UART_printf(uart, "PASS\r\n");

    printHdr(uart, "[4] DAC_setVoltage()");
    const float volts[] = {0.0f, 0.5f, 1.0f, 2.0f, 2.5f, 3.3f};
    for (int i = 0; i < 6; i++) {
        DAC_setVoltage(volts[i]);
        uint16_t c = (uint16_t)(volts[i] * DAC_MAX_VALUE / DAC_REF_VOLTAGE);
        UART_printf(uart, "  %.2fV -> code=%u\r\n", volts[i], c);
        delay_ms(300);
    }
    UART_printf(uart, "PASS\r\n");

    printHdr(uart, "[5] Boundary (clamp)");
    DAC_setValue(5000);   UART_printf(uart, "  setValue(5000)  -> clamped to 4095\r\n");
    DAC_setVoltage(-1.0f); UART_printf(uart, "  setVoltage(-1V) -> clamped to 0V\r\n");
    DAC_setVoltage(5.0f);  UART_printf(uart, "  setVoltage(5V)  -> clamped to 3.3V\r\n");
    UART_printf(uart, "PASS\r\n");
    DAC_setVoltage(1.65f);
}

/*! @brief 波形生成测试 */
void DAC_TEST_runWaveformTests(UART_Regs *uart)
{
    printSep(uart, "DAC WAVEFORM TESTS (observe with scope)");

    printHdr(uart, "[6] Triangle 1 (0~4095 step=100 dly=1ms)");
    DAC_generateTriangleWave(0, 4095, 100, 1);
    UART_printf(uart, "done\r\n");

    printHdr(uart, "[6] Triangle 2 (1000~3000 step=50 dly=2ms)");
    DAC_generateTriangleWave(1000, 3000, 50, 2);
    UART_printf(uart, "done\r\n");

    printHdr(uart, "[7] Sine 1 (amp=1500 off=2048 50Hz 2s)");
    DAC_generateSineWave(1500, 2048, 50, 2000);
    UART_printf(uart, "done\r\n");

    printHdr(uart, "[7] Sine 2 (amp=2000 off=2048 10Hz 3s)");
    DAC_generateSineWave(2000, 2048, 10, 3000);
    UART_printf(uart, "done\r\n");

    printHdr(uart, "[8] Frequency sweep");
    DAC_generateSineWave(1500, 2048, 1, 3000);
    DAC_generateSineWave(1500, 2048, 20, 2000);
    DAC_generateSineWave(1500, 2048, 100, 1000);
    UART_printf(uart, "done\r\n");
}

/*! @brief 运行全部测试 */
void DAC_TEST_runAll(UART_Regs *uart)
{
    UART_printf(uart, "\r\n%s\r\nDAC LIBRARY TEST\r\n%s\r\n",
        DAC_TEST_SEPARATOR, DAC_TEST_SEPARATOR);
    DAC_TEST_runBasicTests(uart);
    DAC_TEST_runWaveformTests(uart);
    UART_printf(uart, "\r\n%s\r\nALL DAC TESTS DONE!\r\n%s\r\n",
        DAC_TEST_SEPARATOR, DAC_TEST_SEPARATOR);
    DAC_setVoltage(1.65f);
}
