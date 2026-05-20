#include "ti_msp_dl_config.h"
#include "delay.h"
#include "../led/led.h"

#define DELAY_CPU_CYCLES_PER_US    (CPUCLK_FREQ / 1000000U)

static void delay_runCycles(uint32_t cycles)
{
	if (cycles > 0U) {
		delay_cycles(cycles);
	}
}

void delay_us(uint32_t count)
{
	uint32_t maxChunkUs;
	uint32_t chunkUs;

	if (DELAY_CPU_CYCLES_PER_US == 0U) {
		return;
	}

	maxChunkUs = UINT32_MAX / DELAY_CPU_CYCLES_PER_US;
	while (count > 0U) {
		chunkUs = (count > maxChunkUs) ? maxChunkUs : count;
		delay_runCycles(chunkUs * DELAY_CPU_CYCLES_PER_US);
		count -= chunkUs;
	}
}

void delay_ms(uint32_t count)
{
	uint32_t maxChunkMs = UINT32_MAX / 1000U;
	uint32_t chunkMs;

	while (count > 0U) {
		chunkMs = (count > maxChunkMs) ? maxChunkMs : count;
		delay_us(chunkMs * 1000U);
		count -= chunkMs;
	}
}

void delay_s(uint32_t count)
{
	uint32_t maxChunkS = UINT32_MAX / 1000U;
	uint32_t chunkS;

	while (count > 0U) {
		chunkS = (count > maxChunkS) ? maxChunkS : count;
		delay_ms(chunkS * 1000U);
		count -= chunkS;
	}
}
