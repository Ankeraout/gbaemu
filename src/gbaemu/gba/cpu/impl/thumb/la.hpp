#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::thumb::la {
    extern void opcode_add_pc(uint16_t opcode);
    extern void opcode_add_sp(uint16_t opcode);
}
