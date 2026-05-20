/**
 * @file spi.h
 * @brief SPI 字节级访问 (供 W25Q64 驱动复用)
 */
#ifndef SPI1_H
#define SPI1_H

#include <stdint.h>

uint8_t spi1_read_write_byte(uint8_t dat);  /*!< SPI 收发 1 字节 (阻塞轮询) */

#endif