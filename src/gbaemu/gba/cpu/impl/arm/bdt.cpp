#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/bdt.hpp>
#include <gbaemu/gba/mmu.hpp>

#define DEFINE_BDT_OPCODE_SINGLE(opcodeName, header, rlistEmpty, body, footer) \
    void opcode_ ## opcodeName(uint32_t opcode) { \
        uint32_t Rn = (opcode & 0x000f0000) >> 16; \
        uint32_t Rn_v = registerRead(Rn); \
        uint32_t Rlist = opcode & 0x0000ffff; \
        uint16_t Rcount = (Rlist == 0) ? 16 : hammingWeight16(Rlist); \
        \
        header; \
        \
        if(Rlist) { \
            for(int i = 0; i < 16; i++) { \
                if(Rlist & 0x0001) { \
                    body; \
                    accessAddress += 4; \
                } \
                \
                Rlist >>= 1; \
            } \
        } else { \
            rlistEmpty; \
        } \
        \
        footer; \
    }

#define DEFINE_BDT_OPCODE(opcode, suffix, header, rlistEmpty, body, footer) \
    DEFINE_BDT_OPCODE_SINGLE(opcode ## da ## suffix, HEADER_DA; header, rlistEmpty, body, footer) \
    DEFINE_BDT_OPCODE_SINGLE(opcode ## db ## suffix, HEADER_DB; header, rlistEmpty, body, footer) \
    DEFINE_BDT_OPCODE_SINGLE(opcode ## ia ## suffix, HEADER_IA; header, rlistEmpty, body, footer) \
    DEFINE_BDT_OPCODE_SINGLE(opcode ## ib ## suffix, HEADER_IB; header, rlistEmpty, body, footer)

#define HEADER_DA \
    Rn_v -= Rcount << 2; \
    uint32_t accessAddress = Rn_v + 4

#define HEADER_DB \
    Rn_v -= Rcount << 2; \
    uint32_t accessAddress = Rn_v

#define HEADER_IA \
    uint32_t accessAddress = Rn_v; \
    Rn_v += Rcount << 2

#define HEADER_IB \
    uint32_t accessAddress = Rn_v + 4; \
    Rn_v += Rcount << 2

#define HEADER_LDM \
    if(Rlist == 0) { \
        registerWrite(CPU_REG_PC, mmu::read32(accessAddress)); \
    }

#define HEADER_LDM_S \
    int accessMode; \
    \
    if(opcode & 0x00008000) { \
        accessMode = cpsr.fields.mode; \
    } else { \
        accessMode = PSR_MODE_USR; \
    }

#define OFFSET_R15 \
    ((i == 15) ? 4 : 0)

#define BODY_LDM \
    registerWrite(i, mmu::read32(accessAddress))

#define BODY_STM \
    mmu::write32(accessAddress, registerRead(i) + OFFSET_R15)

#define BODY_LDM_S \
    registerWrite(i, accessMode, mmu::read32(accessAddress))

#define BODY_STM_S \
    mmu::write32(accessAddress, registerRead(i, PSR_MODE_USR) + OFFSET_R15)

#define RLISTEMPTY_STM \
    mmu::write32(accessAddress, registerRead(CPU_REG_PC))

#define RLISTEMPTY_LDM \
    registerWrite(CPU_REG_PC, mmu::read32(accessAddress))

#define FOOTER_WRITEBACK \
    registerWrite(Rn, Rn_v)

#define FOOTER_LDM_S \
    if(opcode & 0x00008000) { \
        cpsr = readSPSR(); \
    }

namespace gbaemu::gba::cpu::impl::arm::bdt {
    static inline unsigned int hammingWeight16(uint16_t v) {
        unsigned int hammingWeight = 0;

        for(unsigned int i = 0; i < 16; i++) {
            if(v & 0x0001) {
                hammingWeight++;
            }

            v >>= 1;
        }

        return hammingWeight;
    }

    DEFINE_BDT_OPCODE(stm, , , RLISTEMPTY_STM, BODY_STM, )
    DEFINE_BDT_OPCODE(stm, s, , RLISTEMPTY_STM, BODY_STM_S, )
    DEFINE_BDT_OPCODE(stm, w, , RLISTEMPTY_STM, BODY_STM, FOOTER_WRITEBACK)
    DEFINE_BDT_OPCODE(stm, sw, , RLISTEMPTY_STM, BODY_STM_S, FOOTER_WRITEBACK)

    DEFINE_BDT_OPCODE(ldm, , HEADER_LDM, RLISTEMPTY_LDM, BODY_LDM, )
    DEFINE_BDT_OPCODE(ldm, s, HEADER_LDM; HEADER_LDM_S, RLISTEMPTY_LDM, BODY_LDM_S, FOOTER_LDM_S)
    DEFINE_BDT_OPCODE(ldm, w, HEADER_LDM, RLISTEMPTY_LDM, BODY_LDM, FOOTER_WRITEBACK)
    DEFINE_BDT_OPCODE(ldm, sw, HEADER_LDM; HEADER_LDM_S, RLISTEMPTY_LDM, BODY_LDM_S, FOOTER_WRITEBACK; FOOTER_LDM_S)
}
