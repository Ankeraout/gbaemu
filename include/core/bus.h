#ifndef __INCLUDE_CORE_BUS_H__
#define __INCLUDE_CORE_BUS_H__

#include <stdint.h>

void busCycle(void);
uint8_t busRead8(uint32_t p_address);
uint16_t busRead16(uint32_t p_address);
uint32_t busRead32(uint32_t p_address);
void busWrite8(uint32_t p_address, uint8_t p_value);
void busWrite16(uint32_t p_address, uint16_t p_value);
void busWrite32(uint32_t p_address, uint32_t p_value);

#endif
