#include <cstdint>
#include <cstdio>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/cpu/impl/thumb/ppr.hpp>
#include <gbaemu/gba/mmu.hpp>

#define DEFINE_PPR_PUSH_OPCODE(optName, optHeader, rlistEmpty, optFooter) \
    void opcode_push ## optName(uint16_t opcode) { \
        uint32_t sp = registerRead(CPU_REG_SP); \
        uint32_t Rlist = opcode & 0x00ff; \
        unsigned int Rcount = (Rlist == 0) ? 16 : hammingWeight8(Rlist); \
        \
        optHeader; \
        \
        sp -= (Rcount << 2); \
        uint32_t accessAddress = sp; \
        \
        if(Rlist == 0) { \
            rlistEmpty; \
        } else { \
            for(int i = 0; i < 8; i++) { \
                if(Rlist & 0x0001) { \
                    mmu::write32(accessAddress, registerRead(i)); \
                    accessAddress += 4; \
                } \
                \
                Rlist >>= 1; \
            } \
        } \
        \
        optFooter; \
        \
        registerWrite(CPU_REG_SP, sp); \
    }

#define DEFINE_PPR_POP_OPCODE(optName, optCode) \
    void opcode_pop ## optName(uint16_t opcode) { \
        uint32_t sp = registerRead(CPU_REG_SP); \
        uint32_t Rlist = opcode & 0x00ff; \
        \
        for(int i = 0; i < 8; i++) { \
            if(Rlist & 0x0001) { \
                registerWrite(i, mmu::read32(sp)); \
                sp += 4; \
            } \
            \
            Rlist >>= 1; \
        } \
        \
        optCode \
        \
        registerWrite(CPU_REG_SP, sp); \
    }

namespace gbaemu::gba::cpu::impl::thumb::ppr {
    static inline unsigned int hammingWeight8(unsigned int v) {
        unsigned int hammingWeight = 0;

        for(unsigned int i = 0; i < 8; i++) {
            if(v & 0x01) {
                hammingWeight++;
            }

            v >>= 1;
        }

        return hammingWeight;
    }

    DEFINE_PPR_PUSH_OPCODE(,,
        mmu::write32(accessAddress, registerRead(CPU_REG_PC) + 2),
    )
    DEFINE_PPR_PUSH_OPCODE(
        _lr,
        Rcount++,
        ,
        mmu::write32(accessAddress, registerRead(CPU_REG_LR));
    )
    DEFINE_PPR_POP_OPCODE(,)
    DEFINE_PPR_POP_OPCODE(
        _pc,
        performJump(mmu::read32(sp));
        sp += 4;
    )
}
