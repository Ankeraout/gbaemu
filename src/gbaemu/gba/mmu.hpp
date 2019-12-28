#pragma once

#include <cstdint>

namespace gbaemu::gba::mmu {
    extern uint32_t read32(uint32_t address);
    extern void write32(uint32_t address, uint32_t value);
}
