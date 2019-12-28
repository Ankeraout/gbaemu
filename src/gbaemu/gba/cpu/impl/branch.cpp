#include <cstdio>
#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/branch.hpp>

namespace gbaemu::gba::cpu::impl::branch {
    static inline int32_t computeOffset(uint32_t opcode) {
        int32_t offset = opcode & 0x00ffffff;

        if(offset & 0x00800000) {
            // Negative offset
            offset |= 0xff000000;
        }

        offset <<= 2;

        return offset;
    }

    void b(uint32_t opcode) {
        if(gbaemu::gba::cpu::checkCondition(opcode)) {
            performJump(getFetchOffset() + computeOffset(opcode));
        }
    }

    void bl(uint32_t opcode) {
        if(gbaemu::gba::cpu::checkCondition(opcode)) {
            registerWrite(CPU_REG_LR, registerRead(CPU_REG_PC));
            performJump(getFetchOffset() + computeOffset(opcode));
        }
    }

    void bx(uint32_t opcode) {
        if(gbaemu::gba::cpu::checkCondition(opcode)) {
            uint32_t dest = registerRead(opcode & 0x0000000f);
            psr_t psr = readCPSR();

            psr.fields.flagT = dest & 0x00000001;
            performJump(dest & (psr.fields.flagT ? 0xfffffffe : 0xfffffffc));
            writeCPSR(psr);
        }
    }
}
