#include <cstdint>
#include <cstdio>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/cpu/impl/thumb/ppr.hpp>
#include <gbaemu/gba/mmu.hpp>

#define DEFINE_PPR_PUSH_OPCODE(optName, optCode) \
    void opcode_push ## optName(uint16_t opcode) { \
        uint32_t sp = registerRead(CPU_REG_SP); \
        uint32_t Rlist = opcode & 0x00ff; \
        \
        optCode \
        \
        for(int i = 7; i >= 0; i--) { \
            if(Rlist & 0x0080) { \
                sp -= 4; \
                mmu::write32(sp, registerRead(i));\
            } \
            \
            Rlist <<= 1; \
        } \
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
    DEFINE_PPR_PUSH_OPCODE(,)
    DEFINE_PPR_PUSH_OPCODE(
        _lr,
        sp -= 4;
        mmu::write32(sp, registerRead(CPU_REG_LR));
    )
    DEFINE_PPR_POP_OPCODE(,)
    DEFINE_PPR_POP_OPCODE(
        _pc,
        performJump(mmu::read32(sp) - 2);
        sp += 4;
    )
}
