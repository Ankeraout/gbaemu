#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lsro.hpp>
#include <gbaemu/gba/mmu.hpp>

namespace gbaemu::gba::cpu::impl::thumb::ub {
    void opcode_b(uint16_t opcode) {
        int32_t offset = opcode & 0x07ff;

        if(opcode & 0x0400) {
            offset |= 0xfffff800;
        }

        performJump(registerRead(CPU_REG_PC) + (offset << 1));
    }
}
