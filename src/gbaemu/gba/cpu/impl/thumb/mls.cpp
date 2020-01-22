#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/bdt.hpp>
#include <gbaemu/gba/cpu/impl/thumb/mls.hpp>
#include <gbaemu/gba/mmu.hpp>

#include <cstdint>

#define DEFINE_MLS_OPCODE(name, body) \
    void opcode_ ## name ## mia(uint16_t opcode) { \
        uint16_t Rb = (opcode & 0x0700) >> 8; \
        uint16_t Rlist = opcode & 0x00ff; \
        uint32_t Rb_v = registerRead(Rb); \
        \
        body; \
        \
        registerWrite(Rb, Rb_v); \
    }

namespace gbaemu::gba::cpu::impl::thumb::mls {
    static inline unsigned int hammingWeight8(uint16_t v) {
        unsigned int hammingWeight = 0;

        for(unsigned int i = 0; i < 8; i++) {
            if(v & 0x0001) {
                hammingWeight++;
            }

            v >>= 1;
        }

        return hammingWeight;
    }

    DEFINE_MLS_OPCODE(
        st,
        if(Rlist == 0) {
            mmu::write32(Rb_v, registerRead(CPU_REG_PC) + 2);
            Rb_v += 0x40;
        } else {
            bool first = true;

            for(int i = 0; i < 8; i++) {
                if(Rlist & 0x0001) {
                    if(!first && (i == Rb)) {
                        mmu::write32(Rb_v, Rb_v + (hammingWeight8(Rlist) << 2));
                    } else {
                        mmu::write32(Rb_v, registerRead(i));
                    }

                    Rb_v += 4;
                    first = false;
                }
                
                Rlist >>= 1;
            }
        }
    )

    DEFINE_MLS_OPCODE(
        ld,
        if(Rlist == 0) {
            registerWrite(CPU_REG_PC, mmu::read32(Rb_v));
            Rb_v += 0x40;
        } else {
            for(int i = 0; i < 8; i++) {
                if(Rlist & 0x0001) {
                    registerWrite(i, mmu::read32(Rb_v));
                    Rb_v += 4;
                }
                
                Rlist >>= 1;
            }
        }
    )
}
