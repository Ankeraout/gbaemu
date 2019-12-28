#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/branch.hpp>

namespace gbaemu::gba::cpu::impl::branch {
    void b(uint32_t opcode) {
        if(gbaemu::gba::cpu::checkCondition(opcode)) {
            resetPipeline();
            registerWrite(CPU_REG_PC, opcode & 0x00ffffff);
        }
    }

    void bl(uint32_t opcode) {
        if(gbaemu::gba::cpu::checkCondition(opcode)) {
            resetPipeline();
            registerWrite(CPU_REG_LR, registerRead(CPU_REG_PC));
            registerWrite(CPU_REG_PC, opcode & 0x00ffffff);
        }
    }

    void bx(uint32_t opcode) {
        if(gbaemu::gba::cpu::checkCondition(opcode)) {
            uint32_t dest = registerRead(opcode & 0x0000000f);
            psr_t psr = readCPSR();

            resetPipeline();

            psr.fields.flagT = opcode & 0x00000001;
            registerWrite(CPU_REG_PC, dest & (psr.fields.flagT ? 0xfffffffe : 0xfffffffc));
            writeCPSR(psr);
        }
    }
}
