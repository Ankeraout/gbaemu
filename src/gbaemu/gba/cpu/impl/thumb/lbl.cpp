#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lbl.hpp>

namespace gbaemu::gba::cpu::impl::thumb::lbl {
    void opcode_bl(uint16_t opcode) {
        uint16_t offset = opcode & 0x07ff;

        registerWrite(CPU_REG_LR, registerRead(CPU_REG_PC) + (offset << 12));
    }

    void opcode_blh(uint16_t opcode) {
        uint16_t offset = opcode & 0x07ff;
        uint32_t temp = registerRead(CPU_REG_PC) - 2;

        performJump(registerRead(CPU_REG_LR) + (offset << 1) + 2);
        registerWrite(CPU_REG_LR, temp | 1);
    }
}
