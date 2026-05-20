/**
 * @file ringbuffer.c
 * @brief 通用环形缓冲区库实现
 */

#include "ringbuffer.h"
#include <string.h>

/**
 * ============================================================================
 * 实现说明
 * ============================================================================
 *
 * 环形缓冲区的核心原理：
 *
 * 1. 读写位置循环递增：
 *    write_pos = (write_pos + 1) % size
 *    read_pos  = (read_pos  + 1) % size
 *
 * 2. 空满判断：
 *    - 空: write_pos == read_pos
 *    - 满: (write_pos + 1) % size == read_pos
 *      (浪费一个位置来区分空满)
 *
 * 3. 数据量计算：
 *    if (write_pos >= read_pos)
 *        count = write_pos - read_pos
 *    else
 *        count = size - read_pos + write_pos
 *
 * ============================================================================
 * 函数实现
 * ============================================================================
 */

void RingBuffer_Init(RingBuffer *rb, uint8_t *buffer, uint32_t size)
{
    rb->buffer = buffer;
    rb->size = size;
    rb->write_pos = 0;
    rb->read_pos = 0;
    rb->overflow_mode = RINGBUFFER_OVERWRITE;
}

void RingBuffer_Clear(RingBuffer *rb)
{
    rb->write_pos = 0;
    rb->read_pos = 0;
}

void RingBuffer_Reset(RingBuffer *rb)
{
    RingBuffer_Clear(rb);
}

uint32_t RingBuffer_Write(RingBuffer *rb, const uint8_t *data, uint32_t length)
{
    uint32_t i;
    uint32_t free_space;

    if (rb == NULL || data == NULL || length == 0) {
        return 0;
    }

    free_space = RingBuffer_FreeSpace(rb);

    if (length > free_space) {
        if (rb->overflow_mode == RINGBUFFER_DROP) {
            length = free_space;
            if (length == 0) {
                return 0;
            }
        } else {
            // OVERWRITE 模式：需要移动读指针
            uint32_t overflow = length - free_space;
            rb->read_pos = (rb->read_pos + overflow) % rb->size;
        }
    }

    for (i = 0; i < length; i++) {
        rb->buffer[rb->write_pos] = data[i];
        rb->write_pos = (rb->write_pos + 1) % rb->size;
    }

    return length;
}

uint32_t RingBuffer_Read(RingBuffer *rb, uint8_t *data, uint32_t length)
{
    uint32_t i;
    uint32_t available;
    uint32_t first_chunk;
    uint32_t second_chunk;

    if (rb == NULL || data == NULL) {
        return 0;
    }

    available = RingBuffer_Available(rb);
    if (length > available) {
        length = available;
    }

    if (length == 0) {
        return 0;
    }

    first_chunk = rb->size - rb->read_pos;
    if (first_chunk >= length) {
        first_chunk = length;
        second_chunk = 0;
    } else {
        second_chunk = length - first_chunk;
    }

    if (first_chunk > 0) {
        memcpy(data, &rb->buffer[rb->read_pos], first_chunk);
        rb->read_pos = (rb->read_pos + first_chunk) % rb->size;
    }

    if (second_chunk > 0) {
        memcpy(data + first_chunk, rb->buffer, second_chunk);
        rb->read_pos = second_chunk;
    }

    return length;
}

uint32_t RingBuffer_Peek(const RingBuffer *rb, uint8_t *data, uint32_t length)
{
    uint32_t i;
    uint32_t available;
    uint32_t first_chunk;
    uint32_t second_chunk;
    uint32_t saved_read_pos;

    if (rb == NULL || data == NULL) {
        return 0;
    }

    available = RingBuffer_Available(rb);
    if (length > available) {
        length = available;
    }

    if (length == 0) {
        return 0;
    }

    saved_read_pos = rb->read_pos;

    first_chunk = rb->size - saved_read_pos;
    if (first_chunk >= length) {
        first_chunk = length;
        second_chunk = 0;
    } else {
        second_chunk = length - first_chunk;
    }

    if (first_chunk > 0) {
        memcpy(data, &rb->buffer[saved_read_pos], first_chunk);
        saved_read_pos = (saved_read_pos + first_chunk) % rb->size;
    }

    if (second_chunk > 0) {
        memcpy(data + first_chunk, rb->buffer, second_chunk);
    }

    return length;
}

uint32_t RingBuffer_Skip(RingBuffer *rb, uint32_t length)
{
    uint32_t available;

    if (rb == NULL || length == 0) {
        return 0;
    }

    available = RingBuffer_Available(rb);
    if (length > available) {
        length = available;
    }

    rb->read_pos = (rb->read_pos + length) % rb->size;
    return length;
}

bool RingBuffer_WriteByte(RingBuffer *rb, uint8_t byte)
{
    return RingBuffer_Write(rb, &byte, 1) == 1;
}

bool RingBuffer_ReadByte(RingBuffer *rb, uint8_t *byte)
{
    return RingBuffer_Read(rb, byte, 1) == 1;
}

bool RingBuffer_PeekByte(const RingBuffer *rb, uint8_t *byte)
{
    return RingBuffer_Peek(rb, byte, 1) == 1;
}

uint32_t RingBuffer_Available(const RingBuffer *rb)
{
    uint32_t write_pos;
    uint32_t read_pos;
    uint32_t size;

    if (rb == NULL) {
        return 0;
    }

    write_pos = rb->write_pos;
    read_pos = rb->read_pos;
    size = rb->size;

    if (write_pos >= read_pos) {
        return write_pos - read_pos;
    } else {
        return size - read_pos + write_pos;
    }
}

uint32_t RingBuffer_FreeSpace(const RingBuffer *rb)
{
    if (rb == NULL) {
        return 0;
    }
    return rb->size - RingBuffer_Available(rb) - 1;
}

bool RingBuffer_IsEmpty(const RingBuffer *rb)
{
    if (rb == NULL) {
        return true;
    }
    return rb->write_pos == rb->read_pos;
}

bool RingBuffer_IsFull(const RingBuffer *rb)
{
    if (rb == NULL) {
        return false;
    }
    return (rb->write_pos + 1) % rb->size == rb->read_pos;
}

int RingBuffer_Find(const RingBuffer *rb, uint8_t byte)
{
    uint32_t i;
    uint32_t available;
    uint32_t pos;
    uint32_t size;

    if (rb == NULL) {
        return -1;
    }

    available = RingBuffer_Available(rb);
    if (available == 0) {
        return -1;
    }

    size = rb->size;
    pos = rb->read_pos;

    for (i = 0; i < available; i++) {
        if (rb->buffer[pos] == byte) {
            return (int) i;
        }
        pos = (pos + 1) % size;
    }

    return -1;
}

uint32_t RingBuffer_ReadUntil(RingBuffer *rb, uint8_t terminator, uint8_t *data, uint32_t max_len)
{
    uint32_t total_read = 0;
    uint32_t chunk_read;
    uint32_t available;
    uint32_t pos;
    uint32_t size;
    uint32_t i;

    if (rb == NULL || data == NULL || max_len == 0) {
        return 0;
    }

    available = RingBuffer_Available(rb);
    size = rb->size;
    pos = rb->read_pos;

    for (i = 0; i < available && total_read < max_len; i++) {
        if (rb->buffer[pos] == terminator) {
            chunk_read = RingBuffer_Read(rb, data, total_read);
            (void) chunk_read;
            return total_read;
        }

        data[total_read] = rb->buffer[pos];
        total_read++;
        pos = (pos + 1) % size;
    }

    if (total_read > 0) {
        chunk_read = RingBuffer_Skip(rb, total_read);
        (void) chunk_read;
    }

    return total_read;
}

void RingBuffer_SetOverflowMode(RingBuffer *rb, RingBuffer_OverflowMode mode)
{
    if (rb != NULL) {
        rb->overflow_mode = mode;
    }
}

uint32_t RingBuffer_GetSize(const RingBuffer *rb)
{
    if (rb == NULL) {
        return 0;
    }
    return rb->size;
}

uint32_t RingBuffer_UsedSpace(const RingBuffer *rb)
{
    return RingBuffer_Available(rb);
}

void RingBuffer_Compact(RingBuffer *rb)
{
    uint8_t temp[256];
    uint32_t available;
    uint32_t saved_write_pos;

    if (rb == NULL || rb->read_pos == 0) {
        return;
    }

    available = RingBuffer_Available(rb);
    if (available == 0) {
        rb->write_pos = 0;
        rb->read_pos = 0;
        return;
    }

    if (available > sizeof(temp)) {
        return;
    }

    saved_write_pos = rb->write_pos;

    (void) RingBuffer_Read(rb, temp, available);

    memcpy(rb->buffer, temp, available);

    rb->read_pos = 0;
    rb->write_pos = available;
}
