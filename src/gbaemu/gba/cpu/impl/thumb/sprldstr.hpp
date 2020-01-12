#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::thumb::sprldstr {
    extern void opcode_ldr(uint16_t opcode);
    extern void opcode_str(uint16_t opcode);
}
