#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::thumb::lssebh {
    extern void opcode_strh(uint16_t opcode);
    extern void opcode_ldrh(uint16_t opcode);
    extern void opcode_ldrsb(uint16_t opcode);
    extern void opcode_ldrsh(uint16_t opcode);
}
