#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "util.h"

const void *gba_cartridge_rom_buffer;
size_t gba_cartridge_rom_size;
void *gba_cartridge_sram_buffer;
size_t gba_cartridge_sram_size;

uint32_t gba_cartridge_rom_addressMask8;
uint32_t gba_cartridge_rom_addressMask16;
uint32_t gba_cartridge_rom_addressMask32;

void gba_cartridge_init(const void *buffer, size_t size);
uint8_t gba_cartridge_rom_read8(uint32_t address);
uint16_t gba_cartridge_rom_read16(uint32_t address);
uint32_t gba_cartridge_rom_read32(uint32_t address);
uint8_t gba_cartridge_sram_read8(uint32_t address);
uint16_t gba_cartridge_sram_read16(uint32_t address);
uint32_t gba_cartridge_sram_read32(uint32_t address);
void gba_cartridge_sram_write8(uint32_t address, uint8_t value);
void gba_cartridge_sram_write16(uint32_t address, uint16_t value);
void gba_cartridge_sram_write32(uint32_t address, uint32_t value);

void gba_cartridge_init(const void *buffer, size_t size) {
    gba_cartridge_rom_buffer = buffer;
    gba_cartridge_rom_size = size;
    gba_cartridge_sram_buffer = NULL;
    gba_cartridge_sram_size = 0;

    gba_cartridge_rom_addressMask8 = size - 1;
    gba_cartridge_rom_addressMask16 = gba_cartridge_rom_addressMask8 & ~0x00000001;
    gba_cartridge_rom_addressMask32 = gba_cartridge_rom_addressMask16 & ~0x00000002;
}

uint8_t gba_cartridge_rom_read8(uint32_t address) {
    return ACCESS_8(gba_cartridge_rom_buffer, address & gba_cartridge_rom_addressMask8);
}

uint16_t gba_cartridge_rom_read16(uint32_t address) {
    return ACCESS_16(gba_cartridge_rom_buffer, address & gba_cartridge_rom_addressMask16);
}

uint32_t gba_cartridge_rom_read32(uint32_t address) {
    return ACCESS_32(gba_cartridge_rom_buffer, address & gba_cartridge_rom_addressMask32);
}

uint8_t gba_cartridge_sram_read8(uint32_t address) {
    UNUSED(address);
    return 0x00;
}

uint16_t gba_cartridge_sram_read16(uint32_t address) {
    UNUSED(address);
    return 0x0000;
}

uint32_t gba_cartridge_sram_read32(uint32_t address) {
    UNUSED(address);
    return 0x00000000;
}

void gba_cartridge_sram_write8(uint32_t address, uint8_t value) {
    UNUSED(address);
    UNUSED(value);
}

void gba_cartridge_sram_write16(uint32_t address, uint16_t value) {
    UNUSED(address);
    UNUSED(value);
}

void gba_cartridge_sram_write32(uint32_t address, uint32_t value) {
    UNUSED(address);
    UNUSED(value);
}
