#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lsro.hpp>
#include <gbaemu/gba/mmu.hpp>

#define OPCODE_BASE \
    const uint16_t Rd = (opcode & 0x0700) >> 8; \
    const uint16_t immediate = (opcode & 0x00ff) << 2

namespace gbaemu::gba::cpu::impl::thumb::la {
    void opcode_add_pc(uint16_t opcode) {
        OPCODE_BASE;
        registerWrite(Rd, (registerRead(CPU_REG_PC) & 0xfffffffc) + immediate);
    }

    void opcode_add_sp(uint16_t opcode) {
        OPCODE_BASE;
        registerWrite(Rd, registerRead(CPU_REG_SP) + immediate);
    }
}
