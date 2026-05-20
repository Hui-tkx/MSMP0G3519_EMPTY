/**
 * @file temperature.c
 * @brief 温度传感器驱动库
 *
 * 根本修复: configHwAverage(DISABLED) 必须在 configConversionMem 之前调用!
 * 否则 CTL1 中残留的 AVGN/AVGD 会导致 ADC 等待多次累积, MEMRES 永不置位。
 */
#include "temperature.h"
#include "ti_msp_dl_config.h"
#include "delay.h"
#include <stdio.h>
#include <ti/driverlib/m0p/dl_factoryregion.h>

static bool     g_init = false;
static uint32_t g_cal  = 0;
static void (*g_dbg)(const char *) = NULL;

#define DBG(fmt, ...) do { \
    if (g_dbg) { char _b[200]; snprintf(_b, sizeof(_b), fmt, ##__VA_ARGS__); g_dbg(_b); } \
} while(0)

uint32_t TEMP_getCalCode(void)
{
    uint32_t v = DL_FactoryRegion_getTemperatureVoltage();
    DBG("[TEMP] cal code = %u", v);
    return v;
}

/* 完全参照 ADC_read 的流程, 仅 VREF/通道/采样时间可配置 */
static uint16_t readAdcChan(uint32_t ch, uint32_t vref, uint32_t stime)
{
    ADC12_Regs *adc = ADC12_0_INST;
    uint32_t timeout = 500000;

    /*
     * SysConfig 将 TRIGSRC 初始化为 EVENT (硬件事件触发),
     * 此时 SC 位无法启动转换, 必须临时切到 SOFTWARE 触发.
     * 修改 CTL1 前需先关 ENC (ADC12 手册要求).
     */
    DL_ADC12_disableConversions(adc);
    DL_Common_updateReg(&adc->ULLMEM.CTL1,
        DL_ADC12_TRIG_SRC_SOFTWARE, ADC12_CTL1_TRIGSRC_MASK);
    DL_ADC12_enableConversions(adc);

    /* !!! 关键: 先禁用硬件平均, 否则 CTL1 残留的 AVGN 导致 MEMRES 永不置位 !!! */
    DL_ADC12_configHwAverage(adc,
        DL_ADC12_HW_AVG_NUM_ACC_DISABLED, DL_ADC12_HW_AVG_DEN_DIV_BY_1);

    DL_ADC12_configConversionMem(adc, DL_ADC12_MEM_IDX_0,
        ch, vref, stime,
        DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED,
        DL_ADC12_TRIGGER_MODE_AUTO_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

    DL_ADC12_clearInterruptStatus(adc, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);
    DL_ADC12_startConversion(adc);

    while (!DL_ADC12_getRawInterruptStatus(adc, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED)) {
        if (--timeout == 0) {
            DBG("[TEMP] TIMEOUT ch=%u CTL1=0x%08X", (unsigned)ch, adc->ULLMEM.CTL1);
            /* 恢复 EVENT 触发后返回 */
            DL_ADC12_disableConversions(adc);
            DL_Common_updateReg(&adc->ULLMEM.CTL1,
                DL_ADC12_TRIG_SRC_EVENT, ADC12_CTL1_TRIGSRC_MASK);
            DL_ADC12_enableConversions(adc);
            return 0;
        }
    }

    uint16_t r = DL_ADC12_getMemResult(adc, DL_ADC12_MEM_IDX_0);
    DL_ADC12_clearInterruptStatus(adc, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);

    /* 恢复 EVENT 触发 (SysConfig 默认, DMA 等依赖) */
    DL_ADC12_disableConversions(adc);
    DL_Common_updateReg(&adc->ULLMEM.CTL1,
        DL_ADC12_TRIG_SRC_EVENT, ADC12_CTL1_TRIGSRC_MASK);
    DL_ADC12_enableConversions(adc);

    DBG("[TEMP] ch=%u result = %u", (unsigned)ch, r);
    return r;
}

uint16_t TEMP_readRaw(void)
{
    DBG("[TEMP] --- read ---");

    /* 读 CH11 温度传感器 (INTREF 1.4V) */
    return readAdcChan(DL_ADC12_INPUT_CHAN_11,
        DL_ADC12_REFERENCE_VOLTAGE_INTREF,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP1);
}

void TEMP_setDebugPrint(void (*fn)(const char *)) { g_dbg = fn; }

void TEMP_init(void)
{
    DL_VREF_enableInternalRef(VREF);
    DL_ADC12_setSampleTime1(ADC12_0_INST, TEMP_SAMPLE_TIME_CLKS);
    g_cal = TEMP_getCalCode();
    g_init = true;
    DBG("[TEMP] init done, cal=%u", g_cal);
}

void TEMP_deinit(void) { g_init = false; }

int32_t TEMP_read_x10(void)
{
    if (!g_init) return 0;
    int32_t raw = TEMP_readRaw();
    int32_t vs_x10 = ((int32_t)raw * 10 - 5) * 1400 / 4096;
    int32_t vt_x10 = ((int32_t)g_cal * 10 - 5) * 1400 / 4096;
    int32_t t = (vs_x10 - vt_x10) * 10 / (-19) + 300;
    DBG("[TEMP] raw=%d temp*10=%d", raw, t);
    return t;
}

float TEMP_read(void) { return (float)TEMP_read_x10() / 10.0f; }
int32_t TEMP_getCalibration_x10(void) { return TEMP_TSTRIM * 10; }
