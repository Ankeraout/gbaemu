#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::thumb::addspimm {
    extern void opcode_add_sp_imm(uint16_t opcode);
    extern void opcode_sub_sp_imm(uint16_t opcode);
}
