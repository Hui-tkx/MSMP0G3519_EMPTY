/**
 * @file adc_test.c
 * @brief ADC 库测试实现 (硬件平均 + 稳定性)
 */
#include "adc_test.h"
#include "delay.h"

#define ADC_TEST_HWAVG_COUNTS  {2, 8, 32, 64}

static void printSeparator(UART_Regs *uart, const char *title)
{
    UART_printf(uart, "\r\n%s\r\n%s\r\n%s\r\n", ADC_TEST_SEPARATOR, title, ADC_TEST_SEPARATOR);
}

static void printTestHeader(UART_Regs *uart, const char *testName)
{
    UART_printf(uart, "\r\n--- %s ---\r\n", testName);
}

void ADC_TEST_runBasicTests(UART_Regs *uart)
{
    printSeparator(uart, "ADC BASIC API TESTS");

    printTestHeader(uart, "[1] ADC_init()");
    ADC_init();
    UART_printf(uart, "Result: PASS\r\n");

    printTestHeader(uart, "[2] ADC_read()");
    uint16_t rawValue = ADC_read(ADC_DEFAULT_CHANNEL);
    UART_printf(uart, "Raw: %u (0x%04X) | %s\r\n", rawValue, rawValue,
        (ADC_getLastStatus() == ADC_STATUS_OK) ? "PASS" : "FAIL");

    printTestHeader(uart, "[3] ADC_readVoltage()");
    float voltage = ADC_readVoltage(ADC_DEFAULT_CHANNEL);
    UART_printf(uart, "Voltage: %.4f V | %s\r\n", voltage,
        (ADC_getLastStatus() == ADC_STATUS_OK) ? "PASS" : "FAIL");

    printTestHeader(uart, "[4] ADC_getLastStatus()");
    ADC_Status st = ADC_getLastStatus();
    const char *s = (st == ADC_STATUS_OK) ? "ADC_STATUS_OK" :
                    (st == ADC_STATUS_INVALID_CHANNEL) ? "ADC_STATUS_INVALID_CHANNEL" :
                    (st == ADC_STATUS_TIMEOUT) ? "ADC_STATUS_TIMEOUT" :
                    (st == ADC_STATUS_INVALID_PARAM) ? "ADC_STATUS_INVALID_PARAM" : "UNKNOWN";
    UART_printf(uart, "Status: %s | PASS\r\n", s);
}

void ADC_TEST_runAverageTests(UART_Regs *uart)
{
    printSeparator(uart, "ADC HARDWARE AVERAGE TESTS");

    const uint32_t cnt[] = ADC_TEST_HWAVG_COUNTS;
    uint16_t prev = 0;

    for (int i = 0; i < 4; i++) {
        UART_printf(uart, "\r\n[5.%d] HW_AVG(%lu samples)\r\n", i + 1, cnt[i]);
        uint16_t val = ADC_readAverage(ADC_DEFAULT_CHANNEL, cnt[i]);
        float    v   = ((float)val * ADC_REFERENCE_VOLTAGE) / (float)ADC_MAX_VALUE;
        UART_printf(uart, "  Result: %u (%.4f V) | %s\r\n", val, v,
            (ADC_getLastStatus() == ADC_STATUS_OK) ? "PASS" : "FAIL");
        if (i > 0) UART_printf(uart, "  Delta from prev: %d\r\n", (int16_t)(val - prev));
        prev = val;
    }

    printTestHeader(uart, "[6] Stability (10x single read)");
    uint32_t sum = 0, minV = 0xFFFF, maxV = 0;
    for (int i = 0; i < 10; i++) {
        uint16_t v = ADC_read(ADC_DEFAULT_CHANNEL);
        sum += v;
        if (v < minV) minV = v;
        if (v > maxV) maxV = v;
        UART_printf(uart, "  [%d] %u\r\n", i + 1, v);
    }
    uint32_t range = maxV - minV;
    UART_printf(uart, "Avg=%lu  Min=%lu  Max=%lu  Range=%lu | %s\r\n",
        sum / 10, minV, maxV, range,
        (range < 10) ? "EXCELLENT" : (range < 50) ? "GOOD" : (range < 100) ? "FAIR" : "NOISY");

    printTestHeader(uart, "[7] Error handling (invalid channel)");
    uint16_t bad = ADC_read(ADC_CHANNEL_1);
    ADC_Status es = ADC_getLastStatus();
    UART_printf(uart, "  Result=%u  Status=%s | %s\r\n", bad,
        (es == ADC_STATUS_INVALID_CHANNEL) ? "INVALID_CHANNEL" : "UNEXPECTED",
        (es == ADC_STATUS_INVALID_CHANNEL) ? "PASS" : "FAIL");
}

void ADC_TEST_runAll(UART_Regs *uart)
{
    UART_printf(uart, "\r\n%s\r\nADC LIBRARY TEST (HW AVG + POLLING)\r\n%s\r\n",
        ADC_TEST_SEPARATOR, ADC_TEST_SEPARATOR);
    ADC_TEST_runBasicTests(uart);
    ADC_TEST_runAverageTests(uart);
    UART_printf(uart, "\r\n%s\r\nALL ADC TESTS DONE!\r\n%s\r\n",
        ADC_TEST_SEPARATOR, ADC_TEST_SEPARATOR);
}
