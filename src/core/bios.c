#include <stdint.h>

#include "util.h"

const void *buffer;

void gba_bios_init(const void *_buffer);
uint8_t gba_bios_read8(uint32_t address);
uint16_t gba_bios_read16(uint32_t address);
uint32_t gba_bios_read32(uint32_t address);

void gba_bios_init(const void *_buffer) {
    buffer = _buffer;
}

uint8_t gba_bios_read8(uint32_t address) {
    return ACCESS_8(buffer, address & 0x00003fff);
}

uint16_t gba_bios_read16(uint32_t address) {
    return ACCESS_16(buffer, address & 0x00003ffe);
}

uint32_t gba_bios_read32(uint32_t address) {
    return ACCESS_32(buffer, address & 0x00003ffc);
}
