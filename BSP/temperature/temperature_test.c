/**
 * @file temperature_test.c
 * @brief 温度传感器测试实现 (UART 输出, 含详细调试)
 */
#include "temperature_test.h"
#include "temperature.h"
#include "delay.h"

#define SEP "=========================="

/*! @brief UART 调试打印缓冲区 (由 TEMP_setDebugPrint 注册) */
static UART_Regs *g_uart = NULL;

/*! @brief 调试打印回调: 将调试信息通过 UART 输出 */
static void dbgPrint(const char *msg)
{
    if (g_uart) {
        UART_printf(g_uart, "%s\r\n", msg);
    }
}

void TEMP_TEST_runAll(UART_Regs *uart)
{
    int32_t min_x10 = 9999, max_x10 = -9999, sum_x10 = 0;
    int32_t val_x10;
    float   val;

    g_uart = uart;

    UART_printf(uart, "\r\n%s\r\n", SEP);
    UART_printf(uart, "TEMPERATURE SENSOR TEST\r\n");
    UART_printf(uart, "%s\r\n", SEP);

    /* 注册调试打印 */
    TEMP_setDebugPrint(dbgPrint);
    UART_printf(uart, "\r\n[0] Debug print registered, detailed log follows:\r\n");

    /* --- 1. 初始化 --- */
    UART_printf(uart, "\r\n[1] TEMP_init()...\r\n");
    TEMP_init();
    UART_printf(uart, "[1] TEMP_init() OK\r\n");

    /* --- 2. 读校准值 --- */
    uint32_t calCode = TEMP_getCalCode();
    UART_printf(uart, "\r\n[2] Calibration ADC code: %lu\r\n", calCode);
    UART_printf(uart, "    Calibration temp: %d.%d C\r\n",
                TEMP_getCalibration_x10() / 10, TEMP_getCalibration_x10() % 10);

    /* --- 3. 连续读 10 次 --- */
    UART_printf(uart, "\r\n[3] Reading 10 samples:\r\n");
    for (int i = 0; i < 10; i++) {
        val_x10 = TEMP_read_x10();
        val     = (float)val_x10 / 10.0f;

        UART_printf(uart, "  [%d] %d.%d C\r\n",
                    i + 1, val_x10 / 10, val_x10 % 10);

        if (val_x10 < min_x10) min_x10 = val_x10;
        if (val_x10 > max_x10) max_x10 = val_x10;
        sum_x10 += val_x10;

        delay_ms(200);
    }

    /* --- 4. 统计 --- */
    UART_printf(uart, "\r\n[4] Statistics:\r\n");
    UART_printf(uart, "  Min:  %d.%d C\r\n", min_x10 / 10, min_x10 % 10);
    UART_printf(uart, "  Max:  %d.%d C\r\n", max_x10 / 10, max_x10 % 10);
    UART_printf(uart, "  Avg:  %d.%d C\r\n",
                (sum_x10 / 10) / 10, (sum_x10 / 10) % 10);

    /* --- 5. 浮点版 --- */
    float fval = TEMP_read();
    UART_printf(uart, "\r\n[5] TEMP_read() (float): %.2f C\r\n", fval);

    UART_printf(uart, "\r\n%s\r\n", SEP);
    UART_printf(uart, "TEST DONE\r\n");
    UART_printf(uart, "%s\r\n", SEP);
}
