#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lsro.hpp>
#include <gbaemu/gba/mmu.hpp>

namespace gbaemu::gba::cpu::impl::thumb::ub {
    void opcode_b(uint16_t opcode) {
        performJump(registerRead(CPU_REG_PC) + ((opcode & 0x07ff) << 1));
    }
}
