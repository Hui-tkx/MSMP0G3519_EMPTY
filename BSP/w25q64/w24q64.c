/**
 * @file w25q64.c
 * @brief W25Q64 SPI Flash 驱动实现 (DMA 非阻塞读写)
 *
 * 核心设计:
 * - CS 由 GPIO 软件控制, SPI 模式 0
 * - DMA 读写通过状态机分块传输 (单次 ≤65535B)
 * - 写入自动处理页边界 (256B)
 * - 同一时间只支持一个 DMA 任务
 */
#include "w25q64.h"
#include "spi.h"
#include "ti_msp_dl_config.h"

/* ========== 硬件操作宏 ========== */
#define W25Q64_CS_LOW()     DL_GPIO_clearPins(GPIO_SPI_W25Q64_CS0_PORT, GPIO_SPI_W25Q64_CS0_PIN)
#define W25Q64_CS_HIGH()    DL_GPIO_setPins(GPIO_SPI_W25Q64_CS0_PORT, GPIO_SPI_W25Q64_CS0_PIN)
#define W25Q64_DMA_MAX_XFER 0xFFFFU    /*!< DMA 单次最大字节数 */

/* ========== 内部状态机阶段 ========== */
typedef enum {
    W25Q64_STAGE_IDLE = 0,              /*!< 空闲 */
    W25Q64_STAGE_READ_DMA,              /*!< DMA 读取中 */
    W25Q64_STAGE_WRITE_WAIT_READY,      /*!< 写: 等待 Flash 空闲 */
    W25Q64_STAGE_WRITE_DMA,             /*!< 写: DMA 发送中 */
    W25Q64_STAGE_WRITE_WAIT_BUSY,       /*!< 写: 等待写入完成 */
    W25Q64_STAGE_DONE,                  /*!< 完成 */
    W25Q64_STAGE_ERROR                  /*!< 错误 */
} W25Q64_Stage;

/*! @brief DMA 传输上下文 */
typedef struct {
    W25Q64_Stage stage;     /*!< 当前阶段       */
    uint8_t     *buf;       /*!< 缓冲区指针     */
    uint32_t     addr;      /*!< Flash 当前地址  */
    uint32_t     remaining; /*!< 剩余字节数     */
    uint32_t     chunk;     /*!< 当前块大小     */
} W25Q64_AsyncContext;

/* ========== 全局变量 ========== */
static W25Q64_AsyncContext g_w25q64Ctx = { W25Q64_STAGE_IDLE, 0, 0U, 0U, 0U };
static uint8_t g_w25q64DummyTx = 0xFF;  /*!< DMA 读时的假发送数据 */
static uint8_t g_w25q64DummyRx = 0x00;  /*!< DMA 写时的假接收数据 */

/* ========== 内部辅助函数 ========== */

static uint32_t W25Q64_minU32(uint32_t a, uint32_t b) { return (a < b) ? a : b; }

static uint8_t W25Q64_isRangeValid(uint32_t addr, uint32_t len)
{
    if (len == 0U || addr > W25Q64_END_ADDR) return 0U;
    if ((len - 1U) > (W25Q64_END_ADDR - addr)) return 0U;
    return 1U;
}

static W25Q64_NB_State W25Q64_GetStateFromStage(void)
{
    switch (g_w25q64Ctx.stage) {
    case W25Q64_STAGE_IDLE:  return W25Q64_NB_IDLE;
    case W25Q64_STAGE_DONE:  return W25Q64_NB_DONE;
    case W25Q64_STAGE_ERROR: return W25Q64_NB_ERROR;
    default:                 return W25Q64_NB_BUSY;
    }
}

static void W25Q64_DmaStop(void)
{
    uint32_t m = (1U << DMA_W25Q64_RX_CHAN_ID) | (1U << DMA_W25Q64_TX_CHAN_ID);
    DL_DMA_disableChannel(DMA, DMA_W25Q64_RX_CHAN_ID);
    DL_DMA_disableChannel(DMA, DMA_W25Q64_TX_CHAN_ID);
    DL_DMA_clearInterruptStatus(DMA, m);
}

static uint8_t W25Q64_DmaDone(void)
{
    uint32_t m = (1U << DMA_W25Q64_RX_CHAN_ID) | (1U << DMA_W25Q64_TX_CHAN_ID);
    uint32_t s = DL_DMA_getRawInterruptStatus(DMA, m);
    if ((s & m) == m) return 1U;
    if (DL_DMA_getTransferSize(DMA, DMA_W25Q64_RX_CHAN_ID) == 0U &&
        DL_DMA_getTransferSize(DMA, DMA_W25Q64_TX_CHAN_ID) == 0U) return 1U;
    return 0U;
}

static void W25Q64_DmaStartRead(uint8_t *dst, uint16_t len)
{
    uint32_t m = (1U << DMA_W25Q64_RX_CHAN_ID) | (1U << DMA_W25Q64_TX_CHAN_ID);
    DL_DMA_disableChannel(DMA, DMA_W25Q64_RX_CHAN_ID);
    DL_DMA_disableChannel(DMA, DMA_W25Q64_TX_CHAN_ID);
    DL_DMA_clearInterruptStatus(DMA, m);

    DL_DMA_setSrcAddr(DMA, DMA_W25Q64_RX_CHAN_ID, (uint32_t)(&SPI_W25Q64_INST->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_W25Q64_RX_CHAN_ID, (uint32_t)dst);
    DL_DMA_setTransferSize(DMA, DMA_W25Q64_RX_CHAN_ID, len);
    DL_DMA_setSrcIncrement(DMA, DMA_W25Q64_RX_CHAN_ID, DL_DMA_ADDR_UNCHANGED);
    DL_DMA_setDestIncrement(DMA, DMA_W25Q64_RX_CHAN_ID, DL_DMA_ADDR_INCREMENT);

    DL_DMA_setSrcAddr(DMA, DMA_W25Q64_TX_CHAN_ID, (uint32_t)(&g_w25q64DummyTx));
    DL_DMA_setDestAddr(DMA, DMA_W25Q64_TX_CHAN_ID, (uint32_t)(&SPI_W25Q64_INST->TXDATA));
    DL_DMA_setTransferSize(DMA, DMA_W25Q64_TX_CHAN_ID, len);
    DL_DMA_setSrcIncrement(DMA, DMA_W25Q64_TX_CHAN_ID, DL_DMA_ADDR_UNCHANGED);
    DL_DMA_setDestIncrement(DMA, DMA_W25Q64_TX_CHAN_ID, DL_DMA_ADDR_UNCHANGED);

    DL_DMA_enableChannel(DMA, DMA_W25Q64_RX_CHAN_ID);
    DL_DMA_enableChannel(DMA, DMA_W25Q64_TX_CHAN_ID);
}

static void W25Q64_DmaStartWrite(const uint8_t *src, uint16_t len)
{
    uint32_t mask = (1U << DMA_W25Q64_RX_CHAN_ID) | (1U << DMA_W25Q64_TX_CHAN_ID);

    /* 先停止之前的 DMA */
    DL_DMA_disableChannel(DMA, DMA_W25Q64_RX_CHAN_ID);
    DL_DMA_disableChannel(DMA, DMA_W25Q64_TX_CHAN_ID);
    DL_DMA_clearInterruptStatus(DMA, mask);

    /* ========== 配置 TX 通道 (发送数据) ========== */
    /* 源地址: 缓冲区，DMA 会自动递增 */
    DL_DMA_setSrcAddr(DMA, DMA_W25Q64_TX_CHAN_ID, (uint32_t)(src));
    /* 目标地址: SPI 发送寄存器，固定不变 */
    DL_DMA_setDestAddr(DMA, DMA_W25Q64_TX_CHAN_ID, (uint32_t)(&SPI_W25Q64_INST->TXDATA));
    /* 传输字节数 */
    DL_DMA_setTransferSize(DMA, DMA_W25Q64_TX_CHAN_ID, len);
    /* 源地址递增，目标地址不变 */
    DL_DMA_setSrcIncrement(DMA, DMA_W25Q64_TX_CHAN_ID, DL_DMA_ADDR_INCREMENT);
    DL_DMA_setDestIncrement(DMA, DMA_W25Q64_TX_CHAN_ID, DL_DMA_ADDR_UNCHANGED);

    /* ========== 配置 RX 通道 (接收假数据) ========== */
    /* 源地址: SPI 接收寄存器，固定不变 */
    DL_DMA_setSrcAddr(DMA, DMA_W25Q64_RX_CHAN_ID, (uint32_t)(&SPI_W25Q64_INST->RXDATA));
    /* 目标地址: 假数据变量，丢弃 */
    DL_DMA_setDestAddr(DMA, DMA_W25Q64_RX_CHAN_ID, (uint32_t)(&g_w25q64DummyRx));
    /* 传输字节数 */
    DL_DMA_setTransferSize(DMA, DMA_W25Q64_RX_CHAN_ID, len);
    /* 源地址和目标地址都不变 */
    DL_DMA_setSrcIncrement(DMA, DMA_W25Q64_RX_CHAN_ID, DL_DMA_ADDR_UNCHANGED);
    DL_DMA_setDestIncrement(DMA, DMA_W25Q64_RX_CHAN_ID, DL_DMA_ADDR_UNCHANGED);

    /* 启动 DMA */
    DL_DMA_enableChannel(DMA, DMA_W25Q64_RX_CHAN_ID);
    DL_DMA_enableChannel(DMA, DMA_W25Q64_TX_CHAN_ID);
}


/* ============================================================================
 * 【公共函数实现】
 * ============================================================================ */

/*! @brief 初始化 SPI + GPIO CS (模式0, 软件CS, DMA触发) */
void W25Q64_InitConfig(void)
{
    /* 配置 GPIO 片选引脚为数字输出 */
    DL_GPIO_initDigitalOutput(GPIO_SPI_W25Q64_IOMUX_CS0);

    /* 初始片选为高（不选中任何 Flash） */
    DL_GPIO_setPins(GPIO_SPI_W25Q64_CS0_PORT, GPIO_SPI_W25Q64_CS0_PIN);

    /* 使能 GPIO 输出 */
    DL_GPIO_enableOutput(GPIO_SPI_W25Q64_CS0_PORT, GPIO_SPI_W25Q64_CS0_PIN);

    /* 配置 SPI */
    DL_SPI_disable(SPI_W25Q64_INST);  /* 先禁用再配置 */

    /* 设置 SPI 帧格式: Motorola 模式 0 (CPOL=0, CPHA=0) */
    DL_SPI_setFrameFormat(SPI_W25Q64_INST, DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA0);

    /* 不使用硬件片选，使用 GPIO 软件控制 */
    DL_SPI_setChipSelect(SPI_W25Q64_INST, DL_SPI_CHIP_SELECT_NONE);

    /* 开启 DMA 触发 */
    DL_SPI_enableDMATransmitEvent(SPI_W25Q64_INST);
    DL_SPI_enableDMAReceiveEvent(SPI_W25Q64_INST, DL_SPI_DMA_INTERRUPT_RX);

    /* 使能 SPI */
    DL_SPI_enable(SPI_W25Q64_INST);

    /* 确保片选初始为高 */
    W25Q64_CS_HIGH();
}


/*! @brief SPI 收发 1 字节 (阻塞, 委托 spi1_read_write_byte) */
uint8_t W25Q64_ReadWriteByte(uint8_t dat)
{
    return spi1_read_write_byte(dat);
}


/*! @brief 清空 SPI RX FIFO */
void W25Q64_FlushRX(void)
{
    while (!DL_SPI_isRXFIFOEmpty(SPI_W25Q64_INST)) {
        (void)DL_SPI_receiveData8(SPI_W25Q64_INST);
    }
}


/*! @brief 读 JEDEC ID (0x9F, 返回 24-bit) */
uint32_t W25Q64_GetID(void)
{
    uint8_t id1, id2, id3;

    /* 清空 RX FIFO，避免残留数据干扰 */
    W25Q64_FlushRX();

    /* 拉低片选，开始通信 */
    W25Q64_CS_LOW();

    /* 发送 JEDEC Read ID 命令 */
    W25Q64_ReadWriteByte(0x9F);

    /* 读取 3 字节 ID */
    id1 = W25Q64_ReadWriteByte(0xFF);  /* 厂商 ID */
    id2 = W25Q64_ReadWriteByte(0xFF);  /* 芯片类型 */
    id3 = W25Q64_ReadWriteByte(0xFF);  /* 容量 ID */

    /* 拉高片选，结束通信 */
    W25Q64_CS_HIGH();

    /* 组合成 24-bit ID 并返回 */
    return (((uint32_t)id1 << 16) | ((uint32_t)id2 << 8) | id3);
}


/*! @brief 读状态寄存器 (0x05, 返回 BUSY/WEL 等) */
uint8_t W25Q64_ReadState(void)
{
    uint8_t state;

    /* 拉低片选 */
    W25Q64_CS_LOW();

    /* 发送 Read Status Register 1 命令 */
    W25Q64_ReadWriteByte(0x05);

    /* 读取状态 */
    state = W25Q64_ReadWriteByte(0xFF);

    /* 拉高片选 */
    W25Q64_CS_HIGH();

    return state;
}


/*! @brief 发送写使能 (0x06), 返回状态 & WEL */
uint8_t W25Q64_WriteENABLE(void)
{
    /* 拉低片选 */
    W25Q64_CS_LOW();

    /* 发送 Write Enable 命令 */
    W25Q64_ReadWriteByte(0x06);

    /* 拉高片选 */
    W25Q64_CS_HIGH();

    /* 返回状态，检查 WEL 位是否置1 */
    return W25Q64_ReadState() & W25Q64_WEL;
}


/*! @brief 扇区擦除 4KB (0x20, 阻塞 ~45ms) */
void W25Q64_erase_sector(uint32_t addr)
{
    uint32_t timeout = 0;

    /* 等待 Flash 空闲 */
    timeout = 0;
    while ((W25Q64_ReadState() & W25Q64_BUSY) && timeout < 100000) {
        timeout++;
    }

    /* 发送写使能 */
    W25Q64_WriteENABLE();

    /* 拉低片选 */
    W25Q64_CS_LOW();

    /* 发送扇区擦除命令 */
    W25Q64_ReadWriteByte(0x20);

    /* 发送 24-bit 地址 (大端格式) */
    W25Q64_ReadWriteByte((addr & 0xFF0000) >> 16);  /* A23-A16 */
    W25Q64_ReadWriteByte((addr & 0xFF00) >> 8);     /* A15-A8 */
    W25Q64_ReadWriteByte(addr & 0xFF);              /* A7-A0 */

    /* 拉高片选 */
    W25Q64_CS_HIGH();

    /* 等待擦除完成 */
    timeout = 0;
    while ((W25Q64_ReadState() & W25Q64_BUSY) && timeout < 1000000) {
        timeout++;
    }
}


/*! @brief 块擦除 64KB (0xD8, 阻塞 ~200ms) */
void W25Q64_BlockErase(uint32_t addr)
{
    uint32_t timeout = 0;

    /* 等待 Flash 空闲 */
    timeout = 0;
    while ((W25Q64_ReadState() & W25Q64_BUSY) && timeout < 100000) {
        timeout++;
    }

    /* 发送写使能 */
    W25Q64_WriteENABLE();

    /* 拉低片选 */
    W25Q64_CS_LOW();

    /* 发送块擦除命令 */
    W25Q64_ReadWriteByte(0xD8);

    /* 发送 24-bit 地址 */
    W25Q64_ReadWriteByte((addr & 0xFF0000) >> 16);
    W25Q64_ReadWriteByte((addr & 0xFF00) >> 8);
    W25Q64_ReadWriteByte(addr & 0xFF);

    /* 拉高片选 */
    W25Q64_CS_HIGH();

    /* 等待擦除完成 */
    timeout = 0;
    while ((W25Q64_ReadState() & W25Q64_BUSY) && timeout < 1000000) {
        timeout++;
    }
}


/*! @brief 发起 DMA 读取 (Fast Read 0x0B, size=uint16_t 数) */
void W25Q64_read16(uint16_t *buf, uint32_t addr, uint32_t size)
{
    uint64_t byte_len = (uint64_t)size * 2U;

    /* ========== 参数检查 ========== */

    /* 检查是否已有传输在进行 */
    if ((g_w25q64Ctx.stage != W25Q64_STAGE_IDLE) &&
        (g_w25q64Ctx.stage != W25Q64_STAGE_DONE) &&
        (g_w25q64Ctx.stage != W25Q64_STAGE_ERROR)) {
        g_w25q64Ctx.stage = W25Q64_STAGE_ERROR;
        return;
    }

    /* 检查长度是否为0 */
    if (byte_len == 0U) {
        g_w25q64Ctx.stage = W25Q64_STAGE_DONE;
        return;
    }

    /* 检查是否超出 Flash 容量 */
    if (byte_len > (uint64_t)(W25Q64_END_ADDR - W25Q64_START_ADDR + 1U)) {
        g_w25q64Ctx.stage = W25Q64_STAGE_ERROR;
        return;
    }

    /* 检查地址范围 */
    if (!W25Q64_isRangeValid(addr, (uint32_t)byte_len)) {
        g_w25q64Ctx.stage = W25Q64_STAGE_ERROR;
        return;
    }

    /* 检查 SPI 是否忙 */
    if (DL_SPI_isBusy(SPI_W25Q64_INST)) {
        g_w25q64Ctx.stage = W25Q64_STAGE_ERROR;
        return;
    }

    /* ========== 保存传输参数 ========== */
    g_w25q64Ctx.buf = (uint8_t *)buf;
    g_w25q64Ctx.addr = addr;
    g_w25q64Ctx.remaining = (uint32_t)byte_len;

    /* ========== 开始读取 ========== */

    /* 清空 RX FIFO */
    W25Q64_FlushRX();

    /* 拉低片选 */
    W25Q64_CS_LOW();

    /* 发送 Fast Read 命令 (0x0B) */
    W25Q64_ReadWriteByte(0x0B);

    /* 发送 24-bit 地址 */
    W25Q64_ReadWriteByte((addr >> 16) & 0xFF);
    W25Q64_ReadWriteByte((addr >> 8) & 0xFF);
    W25Q64_ReadWriteByte(addr & 0xFF);

    /* 发送空字节（Fast Read 需要 dummy clock） */
    W25Q64_ReadWriteByte(0x00);

    /* 计算本次 DMA 传输的大小（不超过最大限制） */
    g_w25q64Ctx.chunk = W25Q64_minU32(g_w25q64Ctx.remaining, W25Q64_DMA_MAX_XFER);

    /* 启动 DMA 读取 */
    W25Q64_DmaStartRead(g_w25q64Ctx.buf, (uint16_t)g_w25q64Ctx.chunk);

    /* 进入读取阶段 */
    g_w25q64Ctx.stage = W25Q64_STAGE_READ_DMA;
}


/*! @brief 阻塞读 1 字节 (Fast Read 0x0B) */
uint8_t W25Q64_ReadByte(uint32_t addr)
{
    uint8_t data;
    uint32_t timeout = 0;

    /* 检查地址有效性 */
    if (addr > W25Q64_END_ADDR) {
        return 0U;
    }

    /* 等待 Flash 空闲 */
    timeout = 0;
    while ((W25Q64_ReadState() & W25Q64_BUSY) && timeout < 100000) {
        timeout++;
    }

    /* 清空 RX FIFO */
    W25Q64_FlushRX();

    /* 拉低片选 */
    W25Q64_CS_LOW();

    /* 发送 Fast Read 命令 */
    W25Q64_ReadWriteByte(0x0B);

    /* 发送地址 */
    W25Q64_ReadWriteByte((addr & 0xFF0000) >> 16);
    W25Q64_ReadWriteByte((addr & 0xFF00) >> 8);
    W25Q64_ReadWriteByte(addr & 0xFF);

    /* 发送 dummy clock */
    W25Q64_ReadWriteByte(0x00);

    /* 读取数据 */
    data = W25Q64_ReadWriteByte(0xFF);

    /* 拉高片选 */
    W25Q64_CS_HIGH();

    return data;
}


/*! @brief 发起 DMA 写入 (Page Program 0x02, 需先擦除) */
void W25Q64_write16(uint16_t *buf, uint32_t addr, uint32_t size)
{
    uint64_t byte_len = (uint64_t)size * 2U;

    /* ========== 参数检查 ========== */

    if ((g_w25q64Ctx.stage != W25Q64_STAGE_IDLE) &&
        (g_w25q64Ctx.stage != W25Q64_STAGE_DONE) &&
        (g_w25q64Ctx.stage != W25Q64_STAGE_ERROR)) {
        g_w25q64Ctx.stage = W25Q64_STAGE_ERROR;
        return;
    }

    if (byte_len == 0U) {
        g_w25q64Ctx.stage = W25Q64_STAGE_DONE;
        return;
    }

    if (byte_len > (uint64_t)(W25Q64_END_ADDR - W25Q64_START_ADDR + 1U)) {
        g_w25q64Ctx.stage = W25Q64_STAGE_ERROR;
        return;
    }

    if (!W25Q64_isRangeValid(addr, (uint32_t)byte_len)) {
        g_w25q64Ctx.stage = W25Q64_STAGE_ERROR;
        return;
    }

    if (DL_SPI_isBusy(SPI_W25Q64_INST)) {
        g_w25q64Ctx.stage = W25Q64_STAGE_ERROR;
        return;
    }

    /* ========== 保存传输参数 ========== */
    g_w25q64Ctx.buf = (uint8_t *)buf;
    g_w25q64Ctx.addr = addr;
    g_w25q64Ctx.remaining = (uint32_t)byte_len;
    g_w25q64Ctx.chunk = 0U;  /* 稍后在 Process 中计算 */

    /* 进入写入等待阶段 */
    g_w25q64Ctx.stage = W25Q64_STAGE_WRITE_WAIT_READY;
}


/*! @brief 推进 DMA 状态机 (主循环反复调用) */
W25Q64_NB_State W25Q64_Process(void)
{
    switch (g_w25q64Ctx.stage) {

    /* ========== 读取阶段 ========== */
    case W25Q64_STAGE_READ_DMA:
        if (W25Q64_DmaDone()) {
            /* 等待 SPI 真正空闲 */
            if (DL_SPI_isBusy(SPI_W25Q64_INST)) {
                break;
            }

            /* 停止 DMA */
            W25Q64_DmaStop();

            /* 更新进度 */
            g_w25q64Ctx.buf += g_w25q64Ctx.chunk;
            g_w25q64Ctx.addr += g_w25q64Ctx.chunk;
            g_w25q64Ctx.remaining -= g_w25q64Ctx.chunk;

            /* 检查是否全部读完 */
            if (g_w25q64Ctx.remaining == 0U) {
                W25Q64_CS_HIGH();
                g_w25q64Ctx.stage = W25Q64_STAGE_DONE;
            } else {
                /* 还有数据，发起下一块 DMA */
                g_w25q64Ctx.chunk = W25Q64_minU32(
                    g_w25q64Ctx.remaining, W25Q64_DMA_MAX_XFER);
                W25Q64_DmaStartRead(g_w25q64Ctx.buf, (uint16_t)g_w25q64Ctx.chunk);
            }
        }
        break;

    /* ========== 写入阶段 1: 等待 Flash 准备好 ========== */
    case W25Q64_STAGE_WRITE_WAIT_READY:
        /* 等待 Flash 空闲 */
        if (W25Q64_ReadState() & W25Q64_BUSY) {
            break;
        }

        /* 计算本次写入大小
         * 限制1: 不能超过剩余数据量
         * 限制2: 不能超过页边界 (256字节)
         *   页地址 = addr / 256
         *   页内偏移 = addr % 256
         *   页内剩余 = 256 - 偏移
         */
        g_w25q64Ctx.chunk = W25Q64_minU32(
            g_w25q64Ctx.remaining,
            W25Q64_PAGE_SIZE - (g_w25q64Ctx.addr % W25Q64_PAGE_SIZE));

        /* 检查计算结果 */
        if (g_w25q64Ctx.chunk == 0U) {
            g_w25q64Ctx.stage = W25Q64_STAGE_ERROR;
            break;
        }

        /* 发送写使能 */
        if ((W25Q64_WriteENABLE() & W25Q64_WEL) == 0U) {
            g_w25q64Ctx.stage = W25Q64_STAGE_ERROR;
            break;
        }

        /* 清空 RX FIFO */
        W25Q64_FlushRX();

        /* 拉低片选 */
        W25Q64_CS_LOW();

        /* 发送 Page Program 命令 */
        W25Q64_ReadWriteByte(0x02);

        /* 发送 24-bit 地址 */
        W25Q64_ReadWriteByte((g_w25q64Ctx.addr >> 16) & 0xFF);
        W25Q64_ReadWriteByte((g_w25q64Ctx.addr >> 8) & 0xFF);
        W25Q64_ReadWriteByte(g_w25q64Ctx.addr & 0xFF);

        /* 启动 DMA 发送 */
        W25Q64_DmaStartWrite(g_w25q64Ctx.buf, (uint16_t)g_w25q64Ctx.chunk);

        /* 进入写入 DMA 阶段 */
        g_w25q64Ctx.stage = W25Q64_STAGE_WRITE_DMA;
        break;

    /* ========== 写入阶段 2: DMA 传输中 ========== */
    case W25Q64_STAGE_WRITE_DMA:
        if (W25Q64_DmaDone()) {
            /* 等待 SPI 真正空闲 */
            if (DL_SPI_isBusy(SPI_W25Q64_INST)) {
                break;
            }

            /* 停止 DMA */
            W25Q64_DmaStop();

            /* 拉高片选，结束传输 */
            W25Q64_CS_HIGH();

            /* 进入等待写入完成阶段 */
            g_w25q64Ctx.stage = W25Q64_STAGE_WRITE_WAIT_BUSY;
        }
        break;

    /* ========== 写入阶段 3: 等待写入完成 ========== */
    case W25Q64_STAGE_WRITE_WAIT_BUSY:
        /* 等待 Flash 写入完成 */
        if (W25Q64_ReadState() & W25Q64_BUSY) {
            break;
        }

        /* 更新进度 */
        g_w25q64Ctx.buf += g_w25q64Ctx.chunk;
        g_w25q64Ctx.addr += g_w25q64Ctx.chunk;
        g_w25q64Ctx.remaining -= g_w25q64Ctx.chunk;

        /* 检查是否全部写完 */
        if (g_w25q64Ctx.remaining == 0U) {
            g_w25q64Ctx.stage = W25Q64_STAGE_DONE;
        } else {
            /* 还有数据，继续写入下一个块/页 */
            g_w25q64Ctx.stage = W25Q64_STAGE_WRITE_WAIT_READY;
        }
        break;

    /* ========== 空闲/完成/错误阶段 ========== */
    default:
        break;
    }

    /* 返回外部可见的状态 */
    return W25Q64_GetStateFromStage();
}
