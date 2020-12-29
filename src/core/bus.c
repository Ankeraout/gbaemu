#include <stdint.h>

#include "platform.h"
#include "core/bios.h"
#include "core/cartridge.h"
#include "core/ewram.h"
#include "core/io.h"
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

        case 0x04: // IO
        return gba_io_read8(address);

        case 0x05: // Palette
        return gba_ppu_palette_read8(address);

        case 0x06: // VRAM
        return gba_ppu_vram_read8(address);

        case 0x07: // OAM
        return gba_ppu_oam_read8(address);

        case 0x08: // Game Pak ROM Wait State 0
        case 0x09:
        return gba_cartridge_rom_read8(address);

        case 0x0a: // Game Pak ROM Wait State 0
        case 0x0b:
        return gba_cartridge_rom_read8(address);

        case 0x0c: // Game Pak ROM Wait State 0
        case 0x0d:
        return gba_cartridge_rom_read8(address);

        case 0x0e: // Game Pak SRAM
        case 0x0f:
        return gba_cartridge_sram_read8(address);
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

        case 0x04: // IO
        return gba_io_read16(address);

        case 0x05: // Palette
        return gba_ppu_palette_read16(address);

        case 0x06: // VRAM
        return gba_ppu_vram_read16(address);

        case 0x07: // OAM
        return gba_ppu_oam_read16(address);

        case 0x08: // Game Pak ROM Wait State 0
        case 0x09:
        return gba_cartridge_rom_read16(address);

        case 0x0a: // Game Pak ROM Wait State 0
        case 0x0b:
        return gba_cartridge_rom_read16(address);

        case 0x0c: // Game Pak ROM Wait State 0
        case 0x0d:
        return gba_cartridge_rom_read16(address);

        case 0x0e: // Game Pak SRAM
        case 0x0f:
        return gba_cartridge_sram_read16(address);
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

        case 0x04: // IO
        return gba_io_read32(address);

        case 0x05: // Palette
        return gba_ppu_palette_read32(address);

        case 0x06: // VRAM
        return gba_ppu_vram_read32(address);

        case 0x07: // OAM
        return gba_ppu_oam_read32(address);

        case 0x08: // Game Pak ROM Wait State 0
        case 0x09:
        return gba_cartridge_rom_read32(address);

        case 0x0a: // Game Pak ROM Wait State 0
        case 0x0b:
        return gba_cartridge_rom_read32(address);

        case 0x0c: // Game Pak ROM Wait State 0
        case 0x0d:
        return gba_cartridge_rom_read32(address);

        case 0x0e: // Game Pak SRAM
        case 0x0f:
        return gba_cartridge_sram_read32(address);
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

        case 0x04: // IO
        gba_io_write8(address, value);
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

        case 0x0e: // Game Pak SRAM
        case 0x0f:
        gba_cartridge_sram_write8(address, value);
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

        case 0x04: // IO
        gba_io_write16(address, value);
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

        case 0x0e: // Game Pak SRAM
        case 0x0f:
        gba_cartridge_sram_write16(address, value);
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

        case 0x04: // IO
        gba_io_write32(address, value);
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

        case 0x0e: // Game Pak SRAM
        case 0x0f:
        gba_cartridge_sram_write32(address, value);
        break;
    }
}
