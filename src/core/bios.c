#include <stdint.h>

#include "util.h"

const void *gba_bios_buffer;

void gba_bios_init(const void *_buffer);
uint8_t gba_bios_read8(uint32_t address);
uint16_t gba_bios_read16(uint32_t address);
uint32_t gba_bios_read32(uint32_t address);

void gba_bios_init(const void *_buffer) {
    gba_bios_buffer = _buffer;
}

uint8_t gba_bios_read8(uint32_t address) {
    return ACCESS_8(gba_bios_buffer, address & 0x00003fff);
}

uint16_t gba_bios_read16(uint32_t address) {
    return ACCESS_16(gba_bios_buffer, address & 0x00003ffe);
}

uint32_t gba_bios_read32(uint32_t address) {
    return ACCESS_32(gba_bios_buffer, address & 0x00003ffc);
}
