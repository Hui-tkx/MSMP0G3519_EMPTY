/**
 * @file audio_task.h
 * @brief 音频采集/回放任务接口（DMA + W25Q64）。
 * @details 封装 ADC+DMA 录音、DAC+DMA 播放、Flash 读写状态机，
 *          所有函数均为非阻塞，需在主循环中周期调用 Audio_Process()。
 * @note 大白话：录音和播放的"后台管家"，你只要告诉它"开始录/开始放"就行。
 *
 * API 函数列表:
 * - Audio_Init            — 初始化音频模块
 * - Audio_StartRecord     — 开始录音（自动擦除 Flash）
 * - Audio_StopRecord      — 停止录音，将剩余数据写入 Flash
 * - Audio_StartPlayback   — 开始播放
 * - Audio_StopPlayback    — 停止播放
 * - Audio_Process         — 周期调用，驱动录音写入 Flash
 * - Audio_IsRecordDone    — 查询录音是否完成
 * - Audio_IsRecordOverrun — 查询是否发生过溢出
 * - Audio_GetRecordedSamples — 获取实际录到的采样点数
 * - Audio_SetVolume       — 设置播放音量增益
 * - Audio_InitLoopback    — 初始化直通模式
 * - Audio_RunLoopback     — 执行一次直通传输
 */

#ifndef AUDIO_TASK_H
#define AUDIO_TASK_H

#include <stdint.h>
#include <stdbool.h>

/*==============================================================================
 * 宏定义
 *============================================================================*/

#define SAMPLE_RATE           8000    /**< 采样率 8kHz */
#define RECORD_SECONDS        100     /**< 最大录音时长（秒） */
#define TOTAL_SAMPLES         (SAMPLE_RATE * RECORD_SECONDS)  /**< 总采样点数 */
#define DMA_CHUNK_SIZE        2048    /**< 每块 DMA 采样点数 */
#define ADC_BUF_COUNT         4       /**< ADC 乒乓缓冲块数 */
#define DAC_BUF_COUNT         4       /**< DAC 乒乓缓冲块数 */
#define LOOPBACK_SIZE         1024    /**< 直通模式缓冲区大小 */

/*==============================================================================
 * 函数声明
 *============================================================================*/

/** @brief 初始化音频模块。在 SYSCFG_DL_init 后调用。 */
void Audio_Init(void);

/** @brief 开始录音。自动擦除 Flash，启动 ADC+DMA。 */
bool Audio_StartRecord(void);

/**
 * @brief 停止录音，将 ADC 队列中剩余数据写入 Flash。
 * @note 此函数会阻塞等待 Flash 写完。
 */
void Audio_StopRecord(void);

/** @brief 开始播放。从 Flash 读取数据送到 DAC+DMA。 */
bool Audio_StartPlayback(void);

/** @brief 停止播放。 */
void Audio_StopPlayback(void);

/**
 * @brief 音频任务周期处理。
 * @details 必须在主循环中周期调用，用于：
 *          - 驱动 W25Q64 DMA 写入状态机
 *          - 将已完成 ADC 的缓冲写入 Flash
 *          - 检测录音完成/溢出
 * @note 大白话：录音时必须一直调它，否则数据写不进 Flash。
 */
void Audio_Process(void);

/** @brief 查询录音是否完成。 */
bool Audio_IsRecordDone(void);

/** @brief 查询是否发生溢出（ADC 快于 Flash 写入）。 */
bool Audio_IsRecordOverrun(void);

/** @brief 获取实际录到的采样点数。 */
uint32_t Audio_GetRecordedSamples(void);

/** @brief 设置播放音量增益。1=正常，2=2 倍。 */
void Audio_SetVolume(uint8_t gain);

/** @brief 初始化直通模式。 */
void Audio_InitLoopback(void);

/**
 * @brief 执行一次直通传输（ADC 读 → DAC 播放）。
 * @return true 表示传输已启动
 */
bool Audio_RunLoopback(void);

/** @brief DMA 中断调用：ADC 传输完成。 */
void Audio_DMA_ADC_Done(void);

/** @brief DMA 中断调用：DAC 传输完成。 */
void Audio_DMA_DAC_Done(void);

/** @brief 查询 DAC DMA 是否传输完成。 */
bool Audio_IsDacDmaDone(void);

/** @brief 查询 ADC DMA 是否传输完成（非录音模式）。 */
bool Audio_IsAdcDmaDone(void);

#endif /* AUDIO_TASK_H */
