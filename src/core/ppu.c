#include <stdint.h>
#include <string.h>

#include "util.h"
#include "core/defines.h"

uint8_t palette[GBA_PALETTE_SIZE];
uint8_t vram[GBA_VRAM_SIZE];
uint8_t oam[GBA_OAM_SIZE];

void gba_ppu_reset();
uint8_t gba_ppu_palette_read8(uint32_t address);
uint16_t gba_ppu_palette_read16(uint32_t address);
uint32_t gba_ppu_palette_read32(uint32_t address);
void gba_ppu_palette_write8(uint32_t address, uint8_t value);
void gba_ppu_palette_write16(uint32_t address, uint16_t value);
void gba_ppu_palette_write32(uint32_t address, uint32_t value);
uint8_t gba_ppu_vram_read8(uint32_t address);
uint16_t gba_ppu_vram_read16(uint32_t address);
uint32_t gba_ppu_vram_read32(uint32_t address);
void gba_ppu_vram_write8(uint32_t address, uint8_t value);
void gba_ppu_vram_write16(uint32_t address, uint16_t value);
void gba_ppu_vram_write32(uint32_t address, uint32_t value);
uint8_t gba_ppu_oam_read8(uint32_t address);
uint16_t gba_ppu_oam_read16(uint32_t address);
uint32_t gba_ppu_oam_read32(uint32_t address);
void gba_ppu_oam_write8(uint32_t address, uint8_t value);
void gba_ppu_oam_write16(uint32_t address, uint16_t value);
void gba_ppu_oam_write32(uint32_t address, uint32_t value);

void gba_ppu_reset() {
    memset(palette, 0, GBA_PALETTE_SIZE);
    memset(vram, 0, GBA_VRAM_SIZE);
    memset(oam, 0, GBA_OAM_SIZE);
}

void gba_ppu_cycle() {
    
}

uint8_t gba_ppu_palette_read8(uint32_t address) {
    return palette[address & 0x000003ff];
}

uint16_t gba_ppu_palette_read16(uint32_t address) {
    return ACCESS_16(palette, address & 0x000003fe);
}

uint32_t gba_ppu_palette_read32(uint32_t address) {
    return ACCESS_32(palette, address & 0x000003fc);
}

void gba_ppu_palette_write8(uint32_t address, uint8_t value) {
    gba_ppu_palette_write16(address, (uint16_t)((value << 8) | value));
}

void gba_ppu_palette_write16(uint32_t address, uint16_t value) {
    ACCESS_16(palette, address & 0x000003fe) = value;
}

void gba_ppu_palette_write32(uint32_t address, uint32_t value) {
    ACCESS_32(palette, address & 0x000003fc) = value;
}

uint8_t gba_ppu_vram_read8(uint32_t address) {
    if(address & 0x00010000) {
        address &= 0x00017fff;
    } else {
        address &= 0x0001ffff;
    }

    return vram[address];
}

uint16_t gba_ppu_vram_read16(uint32_t address) {
    if(address & 0x00010000) {
        address &= 0x00017ffe;
    } else {
        address &= 0x0001fffe;
    }

    return ACCESS_16(vram, address);
}

uint32_t gba_ppu_vram_read32(uint32_t address) {
    if(address & 0x00010000) {
        address &= 0x00017ffc;
    } else {
        address &= 0x0001fffc;
    }

    return ACCESS_32(vram, address);
}

void gba_ppu_vram_write8(uint32_t address, uint8_t value) {
    gba_ppu_vram_write16(address, (uint16_t)((value << 8) | value));
}

void gba_ppu_vram_write16(uint32_t address, uint16_t value) {
    if(address & 0x00010000) {
        address &= 0x00017ffe;
    } else {
        address &= 0x0001fffe;
    }

    ACCESS_16(vram, address) = value;
}

void gba_ppu_vram_write32(uint32_t address, uint32_t value) {
    if(address & 0x00010000) {
        address &= 0x00017ffc;
    } else {
        address &= 0x0001fffc;
    }

    ACCESS_32(vram, address) = value;
}

uint8_t gba_ppu_oam_read8(uint32_t address) {
    return oam[address & 0x000003ff];
}

uint16_t gba_ppu_oam_read16(uint32_t address) {
    return ACCESS_16(oam, address & 0x000003fe);
}

uint32_t gba_ppu_oam_read32(uint32_t address) {
    return ACCESS_32(oam, address & 0x000003fc);
}

void gba_ppu_oam_write8(uint32_t address, uint8_t value) {
    gba_ppu_oam_write16(address, (uint16_t)((value << 8) | value));
}

void gba_ppu_oam_write16(uint32_t address, uint16_t value) {
    ACCESS_16(oam, address & 0x000003fe) = value;
}

void gba_ppu_oam_write32(uint32_t address, uint32_t value) {
    ACCESS_32(oam, address & 0x000003fc) = value;
}
