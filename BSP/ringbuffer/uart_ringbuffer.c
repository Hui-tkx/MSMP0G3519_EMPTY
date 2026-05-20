/**
 * @file uart_ringbuffer.c
 * @brief UART 专用环形缓冲区实现
 */

#include "uart_ringbuffer.h"

void UART_RB_Init(UART_RingBuffer *rb, uint8_t *buffer, uint32_t size)
{
    RingBuffer_Init(&rb->rb, buffer, size);
    rb->overflow_flag = false;
    rb->overflow_count = 0;
}

void UART_RB_Clear(UART_RingBuffer *rb)
{
    RingBuffer_Clear(&rb->rb);
    rb->overflow_flag = false;
}

bool UART_RB_WriteByte(UART_RingBuffer *rb, uint8_t byte)
{
    uint32_t free_space;
    uint32_t available_before;

    if (rb == NULL) {
        return false;
    }

    available_before = RingBuffer_Available(&rb->rb);
    free_space = RingBuffer_FreeSpace(&rb->rb);

    if (free_space == 0) {
        rb->overflow_flag = true;
        rb->overflow_count++;
        RingBuffer_Skip(&rb->rb, 1);
    }

    return RingBuffer_WriteByte(&rb->rb, byte);
}

uint32_t UART_RB_Write(UART_RingBuffer *rb, const uint8_t *data, uint32_t length)
{
    uint32_t free_space;

    if (rb == NULL || data == NULL) {
        return 0;
    }

    free_space = RingBuffer_FreeSpace(&rb->rb);

    if (length > free_space) {
        rb->overflow_flag = true;
        rb->overflow_count++;
    }

    return RingBuffer_Write(&rb->rb, data, length);
}

bool UART_RB_ReadByte(UART_RingBuffer *rb, uint8_t *byte)
{
    if (rb == NULL || byte == NULL) {
        return false;
    }
    return RingBuffer_ReadByte(&rb->rb, byte);
}

uint32_t UART_RB_Read(UART_RingBuffer *rb, uint8_t *data, uint32_t length)
{
    if (rb == NULL || data == NULL) {
        return 0;
    }
    return RingBuffer_Read(&rb->rb, data, length);
}

bool UART_RB_PeekByte(UART_RingBuffer *rb, uint8_t *byte)
{
    if (rb == NULL || byte == NULL) {
        return false;
    }
    return RingBuffer_PeekByte(&rb->rb, byte);
}

uint32_t UART_RB_Peek(UART_RingBuffer *rb, uint8_t *data, uint32_t length)
{
    if (rb == NULL || data == NULL) {
        return 0;
    }
    return RingBuffer_Peek(&rb->rb, data, length);
}

bool UART_RB_PeekAt(UART_RingBuffer *rb, uint32_t offset, uint8_t *byte)
{
    uint32_t available;
    uint8_t temp[1];

    if (rb == NULL || byte == NULL) {
        return false;
    }

    available = RingBuffer_Available(&rb->rb);
    if (offset >= available) {
        return false;
    }

    if (RingBuffer_Peek(&rb->rb, temp, offset + 1) == (offset + 1)) {
        *byte = temp[offset];
        return true;
    }

    return false;
}

uint32_t UART_RB_Skip(UART_RingBuffer *rb, uint32_t length)
{
    if (rb == NULL) {
        return 0;
    }
    return RingBuffer_Skip(&rb->rb, length);
}

uint32_t UART_RB_Available(UART_RingBuffer *rb)
{
    if (rb == NULL) {
        return 0;
    }
    return RingBuffer_Available(&rb->rb);
}

uint32_t UART_RB_FreeSpace(UART_RingBuffer *rb)
{
    if (rb == NULL) {
        return 0;
    }
    return RingBuffer_FreeSpace(&rb->rb);
}

bool UART_RB_IsEmpty(UART_RingBuffer *rb)
{
    if (rb == NULL) {
        return true;
    }
    return RingBuffer_IsEmpty(&rb->rb);
}

bool UART_RB_IsFull(UART_RingBuffer *rb)
{
    if (rb == NULL) {
        return false;
    }
    return RingBuffer_IsFull(&rb->rb);
}

int UART_RB_FindByte(UART_RingBuffer *rb, uint8_t byte)
{
    if (rb == NULL) {
        return -1;
    }
    return RingBuffer_Find(&rb->rb, byte);
}

bool UART_RB_IsOverflow(UART_RingBuffer *rb)
{
    if (rb == NULL) {
        return false;
    }
    return rb->overflow_flag;
}

uint32_t UART_RB_GetOverflowCount(UART_RingBuffer *rb)
{
    if (rb == NULL) {
        return 0;
    }
    return rb->overflow_count;
}

void UART_RB_ClearOverflow(UART_RingBuffer *rb)
{
    if (rb != NULL) {
        rb->overflow_flag = false;
    }
}

uint32_t UART_RB_ReadLine(UART_RingBuffer *rb, char *buffer, uint32_t max_len)
{
    uint32_t i;
    uint32_t available;
    uint32_t read_count;
    uint8_t byte;

    if (rb == NULL || buffer == NULL || max_len == 0) {
        return 0;
    }

    available = RingBuffer_Available(&rb->rb);
    if (available == 0) {
        return 0;
    }

    read_count = 0;

    for (i = 0; i < available && read_count < (max_len - 1); i++) {
        if (!RingBuffer_PeekByte(&rb->rb, &byte)) {
            break;
        }

        if (byte == '\n' || byte == '\r') {
            if (read_count > 0) {
                RingBuffer_Skip(&rb->rb, read_count + 1);
                buffer[read_count] = '\0';
                return read_count;
            } else {
                RingBuffer_Skip(&rb->rb, 1);
            }
        } else {
            buffer[read_count] = (char) byte;
            read_count++;
        }
    }

    if (read_count > 0) {
        RingBuffer_Skip(&rb->rb, read_count);
        buffer[read_count] = '\0';
    }

    return read_count;
}

uint32_t UART_RB_SendData(UART_Regs *uart, UART_RingBuffer *rb, uint32_t length)
{
    uint32_t available;
    uint32_t to_send;
    uint8_t temp[64];
    uint32_t sent = 0;

    if (uart == NULL || rb == NULL) {
        return 0;
    }

    available = RingBuffer_Available(&rb->rb);
    if (available == 0) {
        return 0;
    }

    if (length == 0) {
        length = available;
    }

    while (sent < length && available > 0) {
        to_send = (available < sizeof(temp)) ? available : sizeof(temp);
        if ((length - sent) < to_send) {
            to_send = length - sent;
        }

        (void) RingBuffer_Read(&rb->rb, temp, to_send);
        (void) UART_sendBuffer(uart, temp, to_send);
        sent += to_send;
        available -= to_send;
    }

    return sent;
}

uint32_t UART_RB_SendString(UART_Regs *uart, const char *str)
{
    uint32_t len;

    if (uart == NULL || str == NULL) {
        return 0;
    }

    len = 0;
    while (str[len] != '\0') {
        len++;
    }

    if (len > 0) {
        (void) UART_sendBuffer(uart, (const uint8_t *) str, len);
    }

    return len;
}
