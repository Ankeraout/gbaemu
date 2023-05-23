#ifndef __INCLUDE_CORE_BIOS_H__
#define __INCLUDE_CORE_BIOS_H__

#include <stdint.h>

uint8_t biosRead8(uint32_t p_address);
uint16_t biosRead16(uint32_t p_address);
uint32_t biosRead32(uint32_t p_address);

#endif
