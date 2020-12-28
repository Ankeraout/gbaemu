#include <stdint.h>

#include "platform.h"
#include "core/bios.h"
#include "core/ewram.h"
#include "core/iwram.h"
#include "core/ppu.h"

uint8_t gba_bus_read8(uint32_t address);
uint16_t gba_bus_read16(uint32_t address);
uint32_t gba_bus_read32(uint32_t address);
void gba_bus_write8(uint32_t address, uint8_t value);
void gba_bus_write16(uint32_t address, uint16_t value);
void gba_bus_write32(uint32_t address, uint32_t value);

uint8_t gba_bus_read8(uint32_t address) {
    switch((address & 0x0f000000) >> 24) {
        case 0x0: // BIOS
        case 0x1:
        return gba_bios_read8(address);

        case 0x02: // EWRAM
        return gba_ewram_read8(address);

        case 0x03: // IWRAM
        return gba_iwram_read8(address);

        case 0x05: // Palette
        return gba_ppu_palette_read8(address);

        case 0x06: // VRAM
        return gba_ppu_vram_read8(address);

        case 0x07: // OAM
        return gba_ppu_vram_read8(address);
    }

    return 0x00;
}

uint16_t gba_bus_read16(uint32_t address) {
    switch((address & 0x0f000000) >> 24) {
        case 0x0: // BIOS
        case 0x1:
        return gba_bios_read16(address);

        case 0x02: // EWRAM
        return gba_ewram_read16(address);

        case 0x03: // IWRAM
        return gba_iwram_read16(address);

        case 0x05: // Palette
        return gba_ppu_palette_read16(address);

        case 0x06: // VRAM
        return gba_ppu_vram_read16(address);

        case 0x07: // OAM
        return gba_ppu_vram_read16(address);
    }

    return 0x0000;
}

uint32_t gba_bus_read32(uint32_t address) {
    switch((address & 0x0f000000) >> 24) {
        case 0x0: // BIOS
        case 0x1:
        return gba_bios_read32(address);

        case 0x02: // EWRAM
        return gba_ewram_read32(address);

        case 0x03: // IWRAM
        return gba_iwram_read32(address);

        case 0x05: // Palette
        return gba_ppu_palette_read32(address);

        case 0x06: // VRAM
        return gba_ppu_vram_read32(address);

        case 0x07: // OAM
        return gba_ppu_vram_read32(address);
    }

    return 0x00000000;
}

void gba_bus_write8(uint32_t address, uint8_t value) {
    switch((address & 0x0f000000) >> 24) {
        case 0x02: // EWRAM
        gba_ewram_write8(address, value);
        break;

        case 0x03: // IWRAM
        gba_iwram_write8(address, value);
        break;

        case 0x05: // Palette
        gba_ppu_palette_write8(address, value);
        break;

        case 0x06: // VRAM
        gba_ppu_vram_write8(address, value);
        break;

        case 0x07: // OAM
        gba_ppu_oam_write8(address, value);
        break;
    }
}

void gba_bus_write16(uint32_t address, uint16_t value) {
    switch((address & 0x0f000000) >> 24) {
        case 0x02: // EWRAM
        gba_ewram_write16(address, value);
        break;

        case 0x03: // IWRAM
        gba_iwram_write16(address, value);
        break;

        case 0x05: // Palette
        gba_ppu_palette_write16(address, value);
        break;

        case 0x06: // VRAM
        gba_ppu_vram_write16(address, value);
        break;

        case 0x07: // OAM
        gba_ppu_oam_write16(address, value);
        break;
    }
}

void gba_bus_write32(uint32_t address, uint32_t value) {
    switch((address & 0x0f000000) >> 24) {
        case 0x02: // EWRAM
        gba_ewram_write32(address, value);
        break;

        case 0x03: // IWRAM
        gba_iwram_write32(address, value);
        break;

        case 0x05: // Palette
        gba_ppu_palette_write32(address, value);
        break;

        case 0x06: // VRAM
        gba_ppu_vram_write32(address, value);
        break;

        case 0x07: // OAM
        gba_ppu_oam_write32(address, value);
        break;
    }
}
