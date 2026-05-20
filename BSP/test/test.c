/**
 * @file test.c
 * @brief 板级测试库实现
 */

#include "test.h"
#include "adc_test.h"
#include "dac_test.h"
#include "uart_test.h"
#include "ringbuffer.h"
#include "uart_ringbuffer.h"

static uint8_t g_rb_buffer[256] = {0};
static RingBuffer g_rb;

static uint8_t g_uart_rb_buffer[256] = {0};
static UART_RingBuffer g_uart_rb;

static void test_ringbuffer_basic(UART_Regs *uart)
{
    UART_printf(uart, "\r\n=== RingBuffer Basic Test ===\r\n");

    RingBuffer_Init(&g_rb, g_rb_buffer, sizeof(g_rb_buffer));

    UART_printf(uart, "Initialized buffer size: %lu\r\n", RingBuffer_GetSize(&g_rb));
    UART_printf(uart, "Initial available: %lu\r\n", RingBuffer_Available(&g_rb));
    UART_printf(uart, "Initial free space: %lu\r\n", RingBuffer_FreeSpace(&g_rb));

    const char *test_data = "Hello, RingBuffer!";
    uint32_t written = RingBuffer_Write(&g_rb, (const uint8_t *)test_data, strlen(test_data));
    UART_printf(uart, "Written %lu bytes: \"%s\"\r\n", written, test_data);

    uint8_t read_buf[64];
    uint32_t read_count = RingBuffer_Read(&g_rb, read_buf, 10);
    read_buf[read_count] = '\0';
    UART_printf(uart, "Read %lu bytes: \"%s\"\r\n", read_count, read_buf);

    UART_printf(uart, "After read - available: %lu\r\n", RingBuffer_Available(&g_rb));

    RingBuffer_Clear(&g_rb);
    UART_printf(uart, "After clear - available: %lu\r\n", RingBuffer_Available(&g_rb));
}

static void test_ringbuffer_overflow(UART_Regs *uart)
{
    UART_printf(uart, "\r\n=== RingBuffer Overflow Test ===\r\n");

    uint8_t small_buf[16];
    RingBuffer rb;
    RingBuffer_Init(&rb, small_buf, sizeof(small_buf));

    uint8_t data[32];
    for (int i = 0; i < 32; i++) {
        data[i] = 'A' + i;
    }

    UART_printf(uart, "Small buffer size: 16 bytes\r\n");
    UART_printf(uart, "Writing 32 bytes (will overflow)...\r\n");

    uint32_t written = RingBuffer_Write(&rb, data, 32);
    UART_printf(uart, "Written %lu bytes\r\n", written);
    UART_printf(uart, "Available: %lu (should be 15, one slot wasted)\r\n", RingBuffer_Available(&rb));

    uint8_t temp[16];
    RingBuffer_Read(&rb, temp, 16);
    UART_printf(uart, "Read 16 bytes: \"");
    for (int i = 0; i < 16; i++) {
        UART_printf(uart, "%c", temp[i]);
    }
    UART_printf(uart, "\"\r\n");

    RingBuffer_Clear(&rb);
}

static void test_ringbuffer_find(UART_Regs *uart)
{
    UART_printf(uart, "\r\n=== RingBuffer Find Test ===\r\n");

    RingBuffer_Init(&g_rb, g_rb_buffer, sizeof(g_rb_buffer));

    const char *test_data = "Hello\nWorld\nTest\n";
    RingBuffer_Write(&g_rb, (const uint8_t *)test_data, strlen(test_data));

    int pos = RingBuffer_Find(&g_rb, '\n');
    UART_printf(uart, "Found newline at position: %d\r\n", pos);

    uint8_t byte;
    RingBuffer_ReadByte(&g_rb, &byte);
    UART_printf(uart, "Read first byte: '%c'\r\n", byte);

    RingBuffer_Clear(&g_rb);
}

static void test_ringbuffer_batch(UART_Regs *uart)
{
    UART_printf(uart, "\r\n=== RingBuffer Batch Test ===\r\n");

    RingBuffer_Init(&g_rb, g_rb_buffer, sizeof(g_rb_buffer));

    uint8_t write_data[100];
    for (int i = 0; i < 100; i++) {
        write_data[i] = i % 256;
    }

    UART_printf(uart, "Writing 100 bytes...\r\n");
    uint32_t written = RingBuffer_Write(&g_rb, write_data, 100);
    UART_printf(uart, "Written: %lu bytes, Available: %lu\r\n", written, RingBuffer_Available(&g_rb));

    uint8_t peek_data[20];
    RingBuffer_Peek(&g_rb, peek_data, 20);
    UART_printf(uart, "Peek first 20 bytes: ");
    for (int i = 0; i < 20; i++) {
        UART_printf(uart, "%02X ", peek_data[i]);
    }
    UART_printf(uart, "\r\n");

    UART_printf(uart, "Available after peek: %lu (should be same)\r\n", RingBuffer_Available(&g_rb));

    RingBuffer_Skip(&g_rb, 50);
    UART_printf(uart, "After skip 50, Available: %lu\r\n", RingBuffer_Available(&g_rb));

    RingBuffer_Clear(&g_rb);
}

static void test_ringbuffer_runAll(UART_Regs *uart)
{
    UART_printf(uart, "\r\n");
    UART_printf(uart, "========================================\r\n");
    UART_printf(uart, "  RING BUFFER LIBRARY TEST SUITE\r\n");
    UART_printf(uart, "========================================\r\n");

    test_ringbuffer_basic(uart);
    test_ringbuffer_overflow(uart);
    test_ringbuffer_find(uart);
    test_ringbuffer_batch(uart);

    UART_printf(uart, "\r\n========================================\r\n");
    UART_printf(uart, "  RING BUFFER TESTS COMPLETED\r\n");
    UART_printf(uart, "========================================\r\n");
}

static void test_uart_ringbuffer_basic(UART_Regs *uart)
{
    UART_printf(uart, "\r\n=== UART RingBuffer Basic Test ===\r\n");

    UART_RB_Init(&g_uart_rb, g_uart_rb_buffer, sizeof(g_uart_rb_buffer));

    UART_printf(uart, "Initialized buffer size: %lu\r\n", RingBuffer_GetSize(&g_uart_rb.rb));

    const char *test_data = "UART RingBuffer Test!";
    UART_printf(uart, "Writing test string...\r\n");
    UART_RB_Write(&g_uart_rb, (const uint8_t *)test_data, strlen(test_data));

    UART_printf(uart, "Available: %lu bytes\r\n", UART_RB_Available(&g_uart_rb));

    uint8_t read_buf[32];
    uint32_t read_count = UART_RB_Read(&g_uart_rb, read_buf, 10);
    read_buf[read_count] = '\0';
    UART_printf(uart, "Read %lu bytes: \"%s\"\r\n", read_count, read_buf);

    UART_printf(uart, "Remaining: %lu bytes\r\n", UART_RB_Available(&g_uart_rb));

    UART_RB_Clear(&g_uart_rb);
    UART_printf(uart, "After clear - available: %lu\r\n", UART_RB_Available(&g_uart_rb));
}

static void test_uart_ringbuffer_line(UART_Regs *uart)
{
    UART_printf(uart, "\r\n=== UART RingBuffer Line Read Test ===\r\n");

    UART_RB_Init(&g_uart_rb, g_uart_rb_buffer, sizeof(g_uart_rb_buffer));

    const char *line1 = "First line\r\n";
    const char *line2 = "Second line\n";
    const char *line3 = "Third line";

    UART_RB_Write(&g_uart_rb, (const uint8_t *)line1, strlen(line1));
    UART_RB_Write(&g_uart_rb, (const uint8_t *)line2, strlen(line2));
    UART_RB_Write(&g_uart_rb, (const uint8_t *)line3, strlen(line3));

    UART_printf(uart, "Buffer has 3 lines, reading one by one...\r\n");

    uint8_t byte;
    char line_buf[32] = {0};
    uint32_t idx = 0;

    while (idx < sizeof(line_buf) - 1) {
        if (!UART_RB_ReadByte(&g_uart_rb, &byte)) break;
        if (byte == '\n' || byte == '\r') {
            if (idx > 0) break;
        } else {
            line_buf[idx++] = (char)byte;
        }
    }
    UART_printf(uart, "Line 1 [%lu]: \"%s\"\r\n", idx, line_buf);

    memset(line_buf, 0, sizeof(line_buf));
    idx = 0;
    while (idx < sizeof(line_buf) - 1) {
        if (!UART_RB_ReadByte(&g_uart_rb, &byte)) break;
        if (byte == '\n' || byte == '\r') {
            if (idx > 0) break;
        } else {
            line_buf[idx++] = (char)byte;
        }
    }
    UART_printf(uart, "Line 2 [%lu]: \"%s\"\r\n", idx, line_buf);

    memset(line_buf, 0, sizeof(line_buf));
    idx = 0;
    while (idx < sizeof(line_buf) - 1) {
        if (!UART_RB_ReadByte(&g_uart_rb, &byte)) break;
        if (byte == '\n' || byte == '\r') {
            if (idx > 0) break;
        } else {
            line_buf[idx++] = (char)byte;
        }
    }
    UART_printf(uart, "Line 3 [%lu]: \"%s\"\r\n", idx, line_buf);

    UART_printf(uart, "Remaining: %lu bytes\r\n", UART_RB_Available(&g_uart_rb));
    UART_RB_Clear(&g_uart_rb);
}

static void test_uart_ringbuffer_overflow(UART_Regs *uart)
{
    UART_printf(uart, "\r\n=== UART RingBuffer Overflow Test ===\r\n");

    uint8_t small_buf[8];
    UART_RingBuffer rb;
    UART_RB_Init(&rb, small_buf, sizeof(small_buf));

    UART_printf(uart, "Small buffer size: 8 bytes\r\n");

    for (int i = 0; i < 15; i++) {
        bool success = UART_RB_WriteByte(&rb, 'A' + i);
        UART_printf(uart, "Write %d: %s", i + 1, success ? "OK" : "FAIL");

        if (UART_RB_IsOverflow(&rb)) {
            UART_printf(uart, " [OVERFLOW! count=%lu]", UART_RB_GetOverflowCount(&rb));
        }
        UART_printf(uart, "\r\n");
    }

    UART_printf(uart, "Available: %lu, Overflow flag: %d\r\n",
               UART_RB_Available(&rb), UART_RB_IsOverflow(&rb));

    UART_RB_Clear(&rb);
}

static void simulate_uart_interrupt(UART_Regs *uart)
{
    (void) uart;
    static const char *commands[] = {
        "LED ON\r\n",
        "LED OFF\r\n",
        "GET STATUS\r\n",
        "ADC READ\r\n"
    };

    static int cmd_idx = 0;
    const char *data = commands[cmd_idx % 4];

    while (*data != '\0') {
        UART_RB_WriteByte(&g_uart_rb, (uint8_t)*data);
        data++;
    }

    cmd_idx++;
}

static void test_uart_ringbuffer_commands(UART_Regs *uart)
{
    UART_printf(uart, "\r\n=== UART RingBuffer Command Test ===\r\n");

    UART_RB_Init(&g_uart_rb, g_uart_rb_buffer, sizeof(g_uart_rb_buffer));

    UART_printf(uart, "Simulating incoming commands...\r\n");

    for (int i = 0; i < 4; i++) {
        simulate_uart_interrupt(uart);
        UART_printf(uart, "Received command: \"");

        char cmd[32];
        uint32_t len = UART_RB_ReadLine(&g_uart_rb, cmd, sizeof(cmd) - 1);
        if (len > 0) {
            cmd[len] = '\0';
            UART_printf(uart, "%s", cmd);
        }
        UART_printf(uart, "\"\r\n");
    }

    UART_printf(uart, "Remaining in buffer: %lu bytes\r\n", UART_RB_Available(&g_uart_rb));
    UART_RB_Clear(&g_uart_rb);
}

static void test_uart_ringbuffer_runAll(UART_Regs *uart)
{
    UART_printf(uart, "\r\n");
    UART_printf(uart, "========================================\r\n");
    UART_printf(uart, "  UART RING BUFFER TEST SUITE\r\n");
    UART_printf(uart, "========================================\r\n");

    test_uart_ringbuffer_basic(uart);
    test_uart_ringbuffer_line(uart);
    test_uart_ringbuffer_overflow(uart);
    test_uart_ringbuffer_commands(uart);

    UART_printf(uart, "\r\n========================================\r\n");
    UART_printf(uart, "  UART RING BUFFER TESTS COMPLETED\r\n");
    UART_printf(uart, "========================================\r\n");
}

void TEST_runAll(UART_Regs *uart)
{
    UART_printf(uart, "\r\n");
    UART_printf(uart, "****************************************\r\n");
    UART_printf(uart, "*                                      *\r\n");
    UART_printf(uart, "*     ALL-IN-ONE BOARD TEST SUITE     *\r\n");
    UART_printf(uart, "*                                      *\r\n");
    UART_printf(uart, "****************************************\r\n");
    UART_printf(uart, "\r\n");

    delay_ms(500);

    TEST_runADC(uart);
    UART_printf(uart, "\r\n");

    TEST_runDAC(uart);
    UART_printf(uart, "\r\n");

    TEST_runRingBuffer(uart);
    UART_printf(uart, "\r\n");

    TEST_runUARTRingBuffer(uart);
    UART_printf(uart, "\r\n");

    TEST_runUART(uart);

    UART_printf(uart, "\r\n");
    UART_printf(uart, "****************************************\r\n");
    UART_printf(uart, "*                                      *\r\n");
    UART_printf(uart, "*     ALL TESTS COMPLETED!             *\r\n");
    UART_printf(uart, "*                                      *\r\n");
    UART_printf(uart, "****************************************\r\n");
    UART_printf(uart, "\r\n");
}

void TEST_runADC(UART_Regs *uart)
{
    ADC_TEST_runAll(uart);
}

void TEST_runDAC(UART_Regs *uart)
{
    DAC_TEST_runAll(uart);
}

void TEST_runUART(UART_Regs *uart)
{
    UART_printf(uart, "\r\n--- UART Polling Tests ---\r\n");
    UART_TEST_runPollingTests(uart);
    UART_printf(uart, "\r\n");

    UART_printf(uart, "--- UART DMA Tests ---\r\n");
    delay_ms(1000);
    UART_TEST_runDmaTests(uart);

    UART_init(uart);
}

void TEST_runRingBuffer(UART_Regs *uart)
{
    test_ringbuffer_runAll(uart);
}

void TEST_runUARTRingBuffer(UART_Regs *uart)
{
    test_uart_ringbuffer_runAll(uart);
}
