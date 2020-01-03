#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::thumb::addsub {
    void opcode_add_Rn(uint16_t opcode);
    void opcode_add_Offset(uint16_t opcode);
    void opcode_sub_Rn(uint16_t opcode);
    void opcode_sub_Offset(uint16_t opcode);
}
