#pragma once

#include <cstdint>

namespace gbaemu::gba::mmu {
    extern uint8_t read8(uint32_t address);
    extern void write8(uint32_t address, uint8_t value);
    extern uint16_t read16(uint32_t address);
    extern void write16(uint32_t address, uint16_t value);
    extern uint32_t read32(uint32_t address);
    extern void write32(uint32_t address, uint32_t value);
}
