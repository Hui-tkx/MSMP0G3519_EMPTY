/**
 * @file uart_test.c
 * @brief UART 库测试实现 (轮询 + DMA)
 */
#include "uart_test.h"
#include <string.h>

#define DMA_RX_BUFFER_SIZE 256
#define SEP "========================"

static uint8_t g_dmaRxBuffer[DMA_RX_BUFFER_SIZE];

static void testRxCallback(uint8_t data, void *arg)
{
    UART_printf((UART_Regs*)arg, "  [RX] 0x%02X ('%c')\r\n", data,
                (data >= ' ' && data <= '~') ? data : '.');
}

static void testTxCallback(void *arg)
{
    UART_printf((UART_Regs*)arg, "  [TX] done\r\n");
}

static void testDmaRxCallback(uint32_t avail, void *arg)
{
    UART_printf((UART_Regs*)arg, "  [DMA RX] %lu bytes\r\n", avail);
}

void UART_TEST_runPollingTests(UART_Regs *uart)
{
    UART_Error st;
    uint32_t baud;

    UART_printf(uart, "\r\n%s\r\nPOLLING TESTS\r\n%s\r\n", SEP, SEP);

    st = UART_init(uart);
    UART_printf(uart, "[1] init: %s\r\n", st == UART_OK ? "PASS" : "FAIL");

    st = UART_sendByte(uart, 'X');
    UART_printf(uart, "[2] sendByte('X'): %s\r\n", st == UART_OK ? "PASS" : "FAIL");

    st = UART_sendString(uart, "Hello!\r\n");
    UART_printf(uart, "[3] sendString: %s\r\n", st == UART_OK ? "PASS" : "FAIL");

    st = UART_sendBuffer(uart, (uint8_t*)"TEST", 4);
    UART_printf(uart, "[4] sendBuffer: %s\r\n", st == UART_OK ? "PASS" : "FAIL");

    UART_printf(uart, "[5] receiveByte: SKIP (no input)\r\n");
    UART_printf(uart, "[6] receiveString: SKIP\r\n");

    UART_printf(uart, "[7] isDataAvailable: %s\r\n", UART_isDataAvailable(uart) ? "YES" : "NO");
    st = UART_clearRxBuffer(uart);
    UART_printf(uart, "[8] clearRxBuffer: %s\r\n", st == UART_OK ? "PASS" : "FAIL");

    int ret = UART_printf(uart, "[9] printf: Int=%d Hex=0x%X Float=%.2f\r\n", 42, 0xABCD, 3.14f);
    UART_printf(uart, "    -> %d bytes | %s\r\n", ret, ret > 0 ? "PASS" : "FAIL");

    st = UART_setRxCallback(uart, testRxCallback, (void*)uart);
    UART_printf(uart, "[10] setRxCallback: %s\r\n", st == UART_OK ? "PASS" : "FAIL");
    st = UART_setTxCallback(uart, testTxCallback, (void*)uart);
    UART_printf(uart, "    setTxCallback: %s\r\n", st == UART_OK ? "PASS" : "FAIL");

    st = UART_getBaudRate(uart, &baud);
    UART_printf(uart, "[11] getBaudRate: %lu | %s\r\n", baud, st == UART_OK ? "PASS" : "FAIL");

    st = UART_setBaudRate(uart, 9600);
    UART_printf(uart, "[12] setBaudRate(9600): %s\r\n", st == UART_OK ? "PASS" : "FAIL");
    UART_setBaudRate(uart, 115200);
}

void UART_TEST_runDmaTests(UART_Regs *uart)
{
    UART_Error st;
    uint8_t buf[64];
    uint32_t n;

    UART_printf(uart, "\r\n%s\r\nDMA TESTS\r\n%s\r\n", SEP, SEP);

    st = UART_DMA_init(uart, g_dmaRxBuffer, DMA_RX_BUFFER_SIZE,
                       testDmaRxCallback, (void*)uart, testTxCallback, (void*)uart);
    UART_printf(uart, "[1] DMA_init: %s\r\n", st == UART_OK ? "PASS" : "FAIL");

    st = UART_DMA_sendString(uart, "DMA Hello!\r\n");
    UART_printf(uart, "[2] DMA_sendString: %s\r\n", st == UART_OK ? "PASS" : "FAIL");
    while (UART_DMA_isTxBusy(uart));

    st = UART_DMA_send(uart, (uint8_t*)"DMA Data", 8);
    UART_printf(uart, "[3] DMA_send: %s\r\n", st == UART_OK ? "PASS" : "FAIL");
    while (UART_DMA_isTxBusy(uart));

    UART_printf(uart, "[4] isTxBusy: %s\r\n", UART_DMA_isTxBusy(uart) ? "BUSY" : "IDLE (PASS)");

    UART_printf(uart, "[5] available: %lu\r\n", UART_DMA_available(uart));

    n = UART_DMA_read(uart, buf, sizeof(buf));
    UART_printf(uart, "[6] read: %lu bytes\r\n", n);

    UART_printf(uart, "[7] isRxOverflowed: %s\r\n",
                UART_DMA_isRxOverflowed(uart) ? "YES" : "NO");
    UART_DMA_clearRxOverflow(uart);
    UART_printf(uart, "[8] clearRxOverflow: %s\r\n",
                UART_DMA_isRxOverflowed(uart) ? "FAIL" : "PASS");
}

void UART_TEST_runAll(UART_Regs *uart)
{
    UART_printf(uart, "\r\n%s\r\nUART LIBRARY TEST\r\n%s\r\n", SEP, SEP);
    UART_TEST_runPollingTests(uart);
    UART_TEST_runDmaTests(uart);
    UART_printf(uart, "\r\n%s\r\nALL DONE!\r\n%s\r\n", SEP, SEP);
}
