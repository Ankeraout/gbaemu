#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::thumb::lsro {
    extern void opcode_str(uint16_t opcode);
    extern void opcode_strb(uint16_t opcode);
    extern void opcode_ldr(uint16_t opcode);
    extern void opcode_ldrb(uint16_t opcode);
}
