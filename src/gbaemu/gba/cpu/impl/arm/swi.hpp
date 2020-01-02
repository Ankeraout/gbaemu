#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::arm::swi {
    extern void swi(uint32_t opcode);
}
