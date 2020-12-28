#include <stdint.h>
#include <string.h>

#include "util.h"
#include "core/defines.h"

uint8_t gba_ewram_buffer[GBA_EWRAM_SIZE];

void gba_ewram_reset();
uint8_t gba_ewram_read8(uint32_t address);
uint16_t gba_ewram_read16(uint32_t address);
uint32_t gba_ewram_read32(uint32_t address);
void gba_ewram_write8(uint32_t address, uint8_t value);
void gba_ewram_write16(uint32_t address, uint16_t value);
void gba_ewram_write32(uint32_t address, uint32_t value);

void gba_ewram_reset() {
    memset(gba_ewram_buffer, 0, GBA_EWRAM_SIZE);
}

uint8_t gba_ewram_read8(uint32_t address) {
    return gba_ewram_buffer[address & 0x0003ffff];
}

uint16_t gba_ewram_read16(uint32_t address) {
    return ACCESS_16(gba_ewram_buffer, address & 0x0003fffe);
}

uint32_t gba_ewram_read32(uint32_t address) {
    return ACCESS_32(gba_ewram_buffer, address & 0x0003fffc);
}

void gba_ewram_write8(uint32_t address, uint8_t value) {
    gba_ewram_buffer[address & 0x0003ffff] = value;
}

void gba_ewram_write16(uint32_t address, uint16_t value) {
    ACCESS_16(gba_ewram_buffer, address & 0x0003fffe) = value;
}

void gba_ewram_write32(uint32_t address, uint32_t value) {
    ACCESS_32(gba_ewram_buffer, address & 0x0003fffc) = value;
}

