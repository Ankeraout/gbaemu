#include <stdint.h>
#include <string.h>

#include "util.h"
#include "core/defines.h"

uint8_t gba_iwram_buffer[GBA_IWRAM_SIZE];

void gba_iwram_reset();
uint8_t gba_iwram_read8(uint32_t address);
uint16_t gba_iwram_read16(uint32_t address);
uint32_t gba_iwram_read32(uint32_t address);
void gba_iwram_write8(uint32_t address, uint8_t value);
void gba_iwram_write16(uint32_t address, uint16_t value);
void gba_iwram_write32(uint32_t address, uint32_t value);

void gba_iwram_reset() {
    memset(gba_iwram_buffer, 0, GBA_IWRAM_SIZE);
}

uint8_t gba_iwram_read8(uint32_t address) {
    return gba_iwram_buffer[address & 0x00007fff];
}

uint16_t gba_iwram_read16(uint32_t address) {
    return ACCESS_16(gba_iwram_buffer, address & 0x00007ffe);
}

uint32_t gba_iwram_read32(uint32_t address) {
    return ACCESS_32(gba_iwram_buffer, address & 0x00007ffc);
}

void gba_iwram_write8(uint32_t address, uint8_t value) {
    gba_iwram_buffer[address & 0x00007fff] = value;
}

void gba_iwram_write16(uint32_t address, uint16_t value) {
    ACCESS_16(gba_iwram_buffer, address & 0x00007ffe) = value;
}

void gba_iwram_write32(uint32_t address, uint32_t value) {
    ACCESS_32(gba_iwram_buffer, address & 0x00007ffc) = value;
}

