#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::arm::mlmal {
    extern void opcode_umull(uint32_t opcode);
    extern void opcode_umulls(uint32_t opcode);
    extern void opcode_umlal(uint32_t opcode);
    extern void opcode_umlals(uint32_t opcode);
    extern void opcode_smull(uint32_t opcode);
    extern void opcode_smulls(uint32_t opcode);
    extern void opcode_smlal(uint32_t opcode);
    extern void opcode_smlals(uint32_t opcode);
}
