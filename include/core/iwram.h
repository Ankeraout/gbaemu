#ifndef __INCLUDE_CORE_IWRAM_H__
#define __INCLUDE_CORE_IWRAM_H__

#include <stdint.h>

void iwramReset(void);
uint8_t iwramRead8(uint32_t p_address);
uint16_t iwramRead16(uint32_t p_address);
uint32_t iwramRead32(uint32_t p_address);
void iwramWrite8(uint32_t p_address, uint8_t p_value);
void iwramWrite16(uint32_t p_address, uint16_t p_value);
void iwramWrite32(uint32_t p_address, uint32_t p_value);

#endif
