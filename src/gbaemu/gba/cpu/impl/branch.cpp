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
        performJump(registerRead(CPU_REG_PC) + computeOffset(opcode));
    }

    void bl(uint32_t opcode) {
        registerWrite(CPU_REG_LR, registerRead(CPU_REG_PC));
        performJump(registerRead(CPU_REG_PC) + computeOffset(opcode));
    }

    void bx(uint32_t opcode) {
        uint32_t dest = registerRead(opcode & 0x0000000f);
        cpsr.fields.flagT = dest & 0x00000001;

        if(cpsr.fields.flagT) {
            dest &= 0xfffffffe;
            dest += 2;
        } else {
            dest &= 0xfffffffc;
        }

        performJump(dest);
    }
}
