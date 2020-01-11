#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::thumb::swi {
    extern void opcode_swi(uint16_t opcode);
}
