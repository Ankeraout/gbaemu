#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::arm::psrt {
    void opcode_mrs_cpsr(uint32_t opcode);
    void opcode_mrs_spsr(uint32_t opcode);
    void opcode_msr_cpsr(uint32_t opcode);
    void opcode_msr_spsr(uint32_t opcode);
    void opcode_msr_cpsr_Immediate(uint32_t opcode);
    void opcode_msr_spsr_Immediate(uint32_t opcode);
}
