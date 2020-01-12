#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::arm::mma {
    void opcode_mul(uint32_t opcode);
    void opcode_muls(uint32_t opcode);
    void opcode_mla(uint32_t opcode);
    void opcode_mlas(uint32_t opcode);
}
