#ifndef __INCLUDE_CORE_CARTRIDGE_H__
#define __INCLUDE_CORE_CARTRIDGE_H__

#include <stdint.h>

uint8_t cartridgeRamRead8(uint32_t p_address);
uint16_t cartridgeRamRead16(uint32_t p_address);
uint32_t cartridgeRamRead32(uint32_t p_address);
void cartridgeRamWrite8(uint32_t p_address, uint8_t p_value);
void cartridgeRamWrite16(uint32_t p_address, uint16_t p_value);
void cartridgeRamWrite32(uint32_t p_address, uint32_t p_value);
uint8_t cartridgeRomRead8(uint32_t p_address);
uint16_t cartridgeRomRead16(uint32_t p_address);
uint32_t cartridgeRomRead32(uint32_t p_address);

#endif
