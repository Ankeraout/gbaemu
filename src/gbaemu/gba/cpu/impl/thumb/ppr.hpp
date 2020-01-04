#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::thumb::ppr {
    void opcode_push(uint16_t opcode);
    void opcode_push_lr(uint16_t opcode);
    void opcode_pop(uint16_t opcode);
    void opcode_pop_pc(uint16_t opcode);
}
