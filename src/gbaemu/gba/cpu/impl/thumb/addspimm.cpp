#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/addspimm.hpp>

namespace gbaemu::gba::cpu::impl::thumb::addspimm {
    void opcode_add_sp_imm(uint16_t opcode) {
        registerWrite(CPU_REG_SP, registerRead(CPU_REG_SP) + ((opcode & 0x007f) << 2));
    }

    void opcode_sub_sp_imm(uint16_t opcode) {
        registerWrite(CPU_REG_SP, registerRead(CPU_REG_SP) - ((opcode & 0x007f) << 2));
    }
}
