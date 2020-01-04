#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/mls.hpp>
#include <gbaemu/gba/mmu.hpp>

#include <cstdint>

#define DEFINE_MLS_OPCODE(name, operation) \
    void opcode_ ## name ## mia(uint16_t opcode) { \
        uint16_t Rb = (opcode & 0x0700) >> 8; \
        uint16_t Rlist = opcode & 0x00ff; \
        uint32_t Rb_v = registerRead(Rb) & 0xfffffffc; \
        \
        for(unsigned int i = 0; i < 8; i++) { \
            if(Rlist & 0x01) { \
                operation; \
                Rb_v += 4; \
            } \
            \
            Rlist >>= 1; \
        } \
        \
        registerWrite(Rb, Rb_v); \
    }

namespace gbaemu::gba::cpu::impl::thumb::mls {
    DEFINE_MLS_OPCODE(
        st,
        mmu::write32(Rb_v, registerRead(i))
    )

    DEFINE_MLS_OPCODE(
        ld,
        registerWrite(i, mmu::read32(Rb_v))
    )
}
