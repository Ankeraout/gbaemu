#ifndef __INCLUDE_CORE_EWRAM_H__
#define __INCLUDE_CORE_EWRAM_H__

#include <stdint.h>

void ewramReset(void);
uint8_t ewramRead8(uint32_t p_address);
uint16_t ewramRead16(uint32_t p_address);
uint32_t ewramRead32(uint32_t p_address);
void ewramWrite8(uint32_t p_address, uint8_t p_value);
void ewramWrite16(uint32_t p_address, uint16_t p_value);
void ewramWrite32(uint32_t p_address, uint32_t p_value);

#endif
