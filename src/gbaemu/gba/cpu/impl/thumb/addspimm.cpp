#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/addspimm.hpp>

namespace gbaemu::gba::cpu::impl::thumb::addspimm {
    void opcode_add_sp_imm(uint16_t opcode) {
        registerWrite(CPU_REG_SP, registerRead(CPU_REG_SP) + (((int8_t)(opcode & 0x00ff)) << 2));
    }
}