#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::thumb::ldrstrhw {
    extern void opcode_ldrh(uint16_t opcode);
    extern void opcode_strh(uint16_t opcode);
}
