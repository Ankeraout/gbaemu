#include <stdint.h>
#include <string.h>

#include "frontend.h"

#define C_OAM_SIZE_BYTES 1024
#define C_OAM_ADDRESS_MASK_8 0x000003ff
#define C_OAM_ADDRESS_MASK_16 0x000003fe
#define C_OAM_ADDRESS_MASK_32 0x000003fc
#define C_PALETTE_RAM_SIZE_BYTES 1024
#define C_PALETTE_ADDRESS_MASK_8 0x000003ff
#define C_PALETTE_ADDRESS_MASK_16 0x000003fe
#define C_PALETTE_ADDRESS_MASK_32 0x000003fc
#define C_VRAM_SIZE_BYTES 98304
#define C_VRAM_ADDRESS_MASK_8 0x0001ffff
#define C_VRAM_ADDRESS_MASK_16 0x0001fffe
#define C_VRAM_ADDRESS_MASK_32 0x0001fffc
#define C_VRAM_ADDRESS_COMPUTE_HIGH 0x00017fff
#define C_VRAM_ADDRESS_COMPUTE_LOW 0x0000ffff

static uint8_t s_oamData[C_OAM_SIZE_BYTES];
static uint8_t s_paletteData[C_PALETTE_RAM_SIZE_BYTES];
static uint8_t s_vramData[C_VRAM_SIZE_BYTES];
static uint32_t s_cycleCounter;
static uint32_t s_frameBuffer[240 * 160];

static uint32_t gpuVramComputeAddress(uint32_t p_address);
static uint16_t getPaletteColor(uint8_t p_paletteIndex);
static uint32_t getColor(uint16_t p_color);
static void gpuDrawFrame(void);

void gpuInit(void) {

}

void gpuReset(void) {
    memset(s_oamData, 0, C_OAM_SIZE_BYTES);
    memset(s_paletteData, 0, C_PALETTE_RAM_SIZE_BYTES);
    memset(s_vramData, 0, C_VRAM_SIZE_BYTES);

    s_cycleCounter = 0;
}

void gpuCycle(void) {
    if(s_cycleCounter++ == 280896) {
        s_cycleCounter = 0;
        gpuDrawFrame();
    }
}

uint8_t gpuOamRead8(uint32_t p_address) {
    return s_oamData[p_address & C_OAM_ADDRESS_MASK_8];
}

uint16_t gpuOamRead16(uint32_t p_address) {
    uint32_t l_address = p_address & C_OAM_ADDRESS_MASK_16;

    return s_oamData[l_address]
        | (s_oamData[l_address + 1] << 8);
}

uint32_t gpuOamRead32(uint32_t p_address) {
    uint32_t l_address = p_address & C_OAM_ADDRESS_MASK_32;

    return s_oamData[l_address]
        | (s_oamData[l_address + 1] << 8)
        | (s_oamData[l_address + 2] << 16)
        | (s_oamData[l_address + 3] << 24);
}

void gpuOamWrite8(uint32_t p_address, uint8_t p_value) {
    uint32_t l_address = p_address & C_OAM_ADDRESS_MASK_16;

    s_oamData[l_address] = p_value;
    s_oamData[l_address + 1] = p_value;
}

void gpuOamWrite16(uint32_t p_address, uint16_t p_value) {
    uint32_t l_address = p_address & C_OAM_ADDRESS_MASK_16;

    s_oamData[l_address] = (uint8_t)p_value;
    s_oamData[l_address + 1] = (uint8_t)(p_value >> 8);
}

void gpuOamWrite32(uint32_t p_address, uint32_t p_value) {
    uint32_t l_address = p_address & C_OAM_ADDRESS_MASK_32;

    s_oamData[l_address] = (uint8_t)p_value;
    s_oamData[l_address + 1] = (uint8_t)(p_value >> 8);
    s_oamData[l_address + 2] = (uint8_t)(p_value >> 16);
    s_oamData[l_address + 3] = (uint8_t)(p_value >> 24);
}

uint8_t gpuPaletteRead8(uint32_t p_address) {
    return s_paletteData[p_address & C_PALETTE_ADDRESS_MASK_8];
}

uint16_t gpuPaletteRead16(uint32_t p_address) {
    uint32_t l_address = p_address & C_PALETTE_ADDRESS_MASK_16;

    return s_paletteData[l_address]
        | (s_paletteData[l_address + 1] << 8);
}

uint32_t gpuPaletteRead32(uint32_t p_address) {
    uint32_t l_address = p_address & C_PALETTE_ADDRESS_MASK_32;

    return s_paletteData[l_address]
        | (s_paletteData[l_address + 1] << 8)
        | (s_paletteData[l_address + 2] << 16)
        | (s_paletteData[l_address + 3] << 24);
}

void gpuPaletteWrite8(uint32_t p_address, uint8_t p_value) {
    uint32_t l_address = p_address & C_PALETTE_ADDRESS_MASK_8;

    s_paletteData[l_address] = p_value;
    s_paletteData[l_address + 1] = p_value;
}

void gpuPaletteWrite16(uint32_t p_address, uint16_t p_value) {
    uint32_t l_address = p_address & C_PALETTE_ADDRESS_MASK_16;

    s_paletteData[l_address] = (uint8_t)p_value;
    s_paletteData[l_address + 1] = (uint8_t)(p_value >> 8);
}

void gpuPaletteWrite32(uint32_t p_address, uint32_t p_value) {
    uint32_t l_address = p_address & C_PALETTE_ADDRESS_MASK_32;

    s_paletteData[l_address] = (uint8_t)p_value;
    s_paletteData[l_address + 1] = (uint8_t)(p_value >> 8);
    s_paletteData[l_address + 2] = (uint8_t)(p_value >> 16);
    s_paletteData[l_address + 3] = (uint8_t)(p_value >> 24);
}

uint8_t gpuVramRead8(uint32_t p_address) {
    return s_vramData[gpuVramComputeAddress(p_address & C_VRAM_ADDRESS_MASK_8)];
}

uint16_t gpuVramRead16(uint32_t p_address) {
    uint32_t l_address = gpuVramComputeAddress(
        p_address & C_VRAM_ADDRESS_MASK_16
    );

    return s_vramData[l_address]
        | (s_vramData[l_address + 1] << 8);
}

uint32_t gpuVramRead32(uint32_t p_address) {
    uint32_t l_address = gpuVramComputeAddress(
        p_address & C_VRAM_ADDRESS_MASK_32
    );

    return s_vramData[l_address]
        | (s_vramData[l_address + 1] << 8)
        | (s_vramData[l_address + 2] << 16)
        | (s_vramData[l_address + 3] << 24);
}

void gpuVramWrite8(uint32_t p_address, uint8_t p_value) {
    uint32_t l_address = gpuVramComputeAddress(p_address);

    s_vramData[l_address] = p_value;
    s_vramData[l_address + 1] = p_value;
}

void gpuVramWrite16(uint32_t p_address, uint16_t p_value) {
    uint32_t l_address = gpuVramComputeAddress(
        p_address & C_VRAM_ADDRESS_MASK_16
    );

    s_vramData[l_address] = (uint8_t)p_value;
    s_vramData[l_address + 1] = (uint8_t)(p_value >> 8);
}

void gpuVramWrite32(uint32_t p_address, uint32_t p_value) {
    uint32_t l_address = gpuVramComputeAddress(
        p_address & C_VRAM_ADDRESS_MASK_32
    );

    s_vramData[l_address] = (uint8_t)p_value;
    s_vramData[l_address + 1] = (uint8_t)(p_value >> 8);
    s_vramData[l_address + 2] = (uint8_t)(p_value >> 16);
    s_vramData[l_address + 3] = (uint8_t)(p_value >> 24);
}

static uint32_t gpuVramComputeAddress(uint32_t p_address) {
    uint32_t l_mask;

    if((p_address & 0x00010000) != 0) {
        l_mask = C_VRAM_ADDRESS_COMPUTE_HIGH;
    } else {
        l_mask = C_VRAM_ADDRESS_COMPUTE_LOW;
    }

    return p_address & l_mask;
}

static uint16_t getPaletteColor(uint8_t p_paletteIndex) {
    uint32_t l_paletteOffset = p_paletteIndex << 1;

    return (s_paletteData[l_paletteOffset] << 8)
        | s_paletteData[l_paletteOffset | 1];
}

static uint32_t getColor(uint16_t p_color) {
    uint32_t l_red = p_color & 0x1f;
    uint32_t l_green = (p_color >> 5) & 0x1f;
    uint32_t l_blue = (p_color >> 10) & 0x1f;

    l_red = (l_red << 3) | (l_red >> 2);
    l_green = (l_green << 3) | (l_green >> 2);
    l_blue = (l_blue << 3) | (l_blue >> 2);

    return l_red | (l_green << 8) | (l_blue << 16);
}

static void gpuDrawFrame(void) {
    for(uint32_t l_i = 0; l_i < 38400; l_i++) {
        uint8_t l_paletteIndex = s_vramData[l_i];
        s_frameBuffer[l_i] = getColor(getPaletteColor(l_paletteIndex));
        //s_frameBuffer[l_i] = getColor(((uint16_t *)s_vramData)[l_i]);
    }

    frontendFrame(s_frameBuffer);
}
