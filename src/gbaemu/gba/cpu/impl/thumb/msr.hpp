#pragma once

#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>

namespace gbaemu::gba::cpu::impl::thumb::msr {
    void opcode_lsl(uint16_t opcode);
    void opcode_lsr(uint16_t opcode);
    void opcode_asr(uint16_t opcode);
}
