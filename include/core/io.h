#ifndef __INCLUDE_CORE_IO_H__
#define __INCLUDE_CORE_IO_H__

#include <stdint.h>

uint8_t ioRead8(uint32_t p_address);
uint16_t ioRead16(uint32_t p_address);
uint32_t ioRead32(uint32_t p_address);
void ioWrite8(uint32_t p_address, uint8_t p_value);
void ioWrite16(uint32_t p_address, uint16_t p_value);
void ioWrite32(uint32_t p_address, uint32_t p_value);

#endif
