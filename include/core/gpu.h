#ifndef __INCLUDE_CORE_GPU_H__
#define __INCLUDE_CORE_GPU_H__

#include <stdint.h>

void gpuReset(void);
void gpuCycle(void);
uint8_t gpuOamRead8(uint32_t p_address);
uint16_t gpuOamRead16(uint32_t p_address);
uint32_t gpuOamRead32(uint32_t p_address);
void gpuOamWrite8(uint32_t p_address, uint8_t p_value);
void gpuOamWrite16(uint32_t p_address, uint16_t p_value);
void gpuOamWrite32(uint32_t p_address, uint32_t p_value);
uint8_t gpuPaletteRead8(uint32_t p_address);
uint16_t gpuPaletteRead16(uint32_t p_address);
uint32_t gpuPaletteRead32(uint32_t p_address);
void gpuPaletteWrite8(uint32_t p_address, uint8_t p_value);
void gpuPaletteWrite16(uint32_t p_address, uint16_t p_value);
void gpuPaletteWrite32(uint32_t p_address, uint32_t p_value);
uint8_t gpuVramRead8(uint32_t p_address);
uint16_t gpuVramRead16(uint32_t p_address);
uint32_t gpuVramRead32(uint32_t p_address);
void gpuVramWrite8(uint32_t p_address, uint8_t p_value);
void gpuVramWrite16(uint32_t p_address, uint16_t p_value);
void gpuVramWrite32(uint32_t p_address, uint32_t p_value);

#endif
