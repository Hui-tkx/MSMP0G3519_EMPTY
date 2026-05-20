/**
 * @file spi.c
 * @brief SPI 字节级访问实现 (阻塞轮询)
 */
#include "spi.h"
#include "ti_msp_dl_config.h"

/*! @brief SPI 收发 1 字节 (等 TX 空位->发->等 RX 数据->读) */
uint8_t spi1_read_write_byte(uint8_t dat)
{
    while (DL_SPI_isTXFIFOFull(SPI_W25Q64_INST));
    DL_SPI_transmitData8(SPI_W25Q64_INST, dat);
    while (DL_SPI_isBusy(SPI_W25Q64_INST));
    while (DL_SPI_isRXFIFOEmpty(SPI_W25Q64_INST));
    return DL_SPI_receiveData8(SPI_W25Q64_INST);
}
