#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/bdt.hpp>
#include <gbaemu/gba/mmu.hpp>

#define DEFINE_BDT_OPCODE(name, header, body, footer) \
    void opcode_ ## name(uint32_t opcode) { \
        uint32_t Rn = (opcode & 0x000f0000) >> 16; \
        uint32_t Rn_v = registerRead(Rn); \
        uint32_t Rlist = opcode & 0x0000ffff; \
        uint16_t Rcount = hammingWeight16(Rlist); \
        \
        header; \
        \
        for(unsigned int i = 0; i < 16; i++) { \
            if(Rlist & 0x0001) { \
                body; \
                accessAddress += 4; \
            } \
            \
            Rlist >>= 1; \
        } \
        \
        footer; \
    }

#define HEADER_DA \
    Rn_v -= Rcount << 2; \
    uint32_t accessAddress = Rn_v + 4

#define HEADER_DB \
    Rn_v -= Rcount << 2; \
    uint32_t accessAddress = Rn_v;
    
#define HEADER_IA \
    uint32_t accessAddress = Rn_v; \
    Rn_v += Rcount << 2;

#define HEADER_IB \
    uint32_t accessAddress = Rn_v + 4; \
    Rn_v += Rcount << 2;

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

#define DO_WRITEBACK \
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

    DEFINE_BDT_OPCODE(ldmda, HEADER_DA, BODY_LDM, )
    DEFINE_BDT_OPCODE(ldmdaw, HEADER_DA; DO_WRITEBACK, BODY_LDM, )
    DEFINE_BDT_OPCODE(ldmdas, HEADER_DA; HEADER_LDM_S, BODY_LDM_S, FOOTER_LDM_S)
    DEFINE_BDT_OPCODE(ldmdasw, HEADER_DA; HEADER_LDM_S; DO_WRITEBACK, BODY_LDM_S, FOOTER_LDM_S)
    DEFINE_BDT_OPCODE(ldmia, HEADER_IA, BODY_LDM, )
    DEFINE_BDT_OPCODE(ldmiaw, HEADER_IA; DO_WRITEBACK, BODY_LDM, )
    DEFINE_BDT_OPCODE(ldmias, HEADER_IA; HEADER_LDM_S, BODY_LDM_S, FOOTER_LDM_S)
    DEFINE_BDT_OPCODE(ldmiasw, HEADER_IA; HEADER_LDM_S; DO_WRITEBACK, BODY_LDM_S, FOOTER_LDM_S)
    DEFINE_BDT_OPCODE(ldmdb, HEADER_DB, BODY_LDM, )
    DEFINE_BDT_OPCODE(ldmdbw, HEADER_DB; DO_WRITEBACK, BODY_LDM, )
    DEFINE_BDT_OPCODE(ldmdbs, HEADER_DB; HEADER_LDM_S, BODY_LDM_S, FOOTER_LDM_S)
    DEFINE_BDT_OPCODE(ldmdbsw, HEADER_DB; HEADER_LDM_S; DO_WRITEBACK, BODY_LDM_S, FOOTER_LDM_S)
    DEFINE_BDT_OPCODE(ldmib, HEADER_IB, BODY_LDM, )
    DEFINE_BDT_OPCODE(ldmibw, HEADER_IB; DO_WRITEBACK, BODY_LDM, )
    DEFINE_BDT_OPCODE(ldmibs, HEADER_IB; HEADER_LDM_S, BODY_LDM_S, FOOTER_LDM_S)
    DEFINE_BDT_OPCODE(ldmibsw, HEADER_IB; HEADER_LDM_S; DO_WRITEBACK, BODY_LDM_S, FOOTER_LDM_S)

    DEFINE_BDT_OPCODE(stmda, HEADER_DA, BODY_STM, )
    DEFINE_BDT_OPCODE(stmdaw, HEADER_DA, BODY_STM, DO_WRITEBACK)
    DEFINE_BDT_OPCODE(stmdas, HEADER_DA, BODY_STM_S, )
    DEFINE_BDT_OPCODE(stmdasw, HEADER_DA, BODY_STM_S, DO_WRITEBACK)
    DEFINE_BDT_OPCODE(stmia, HEADER_IA, BODY_STM, )
    DEFINE_BDT_OPCODE(stmiaw, HEADER_IA, BODY_STM, DO_WRITEBACK)
    DEFINE_BDT_OPCODE(stmias, HEADER_IA, BODY_STM_S, )
    DEFINE_BDT_OPCODE(stmiasw, HEADER_IA, BODY_STM_S, DO_WRITEBACK)
    DEFINE_BDT_OPCODE(stmdb, HEADER_DB, BODY_STM, )
    DEFINE_BDT_OPCODE(stmdbw, HEADER_DB, BODY_STM, DO_WRITEBACK)
    DEFINE_BDT_OPCODE(stmdbs, HEADER_DB, BODY_STM_S, )
    DEFINE_BDT_OPCODE(stmdbsw, HEADER_DB, BODY_STM_S, DO_WRITEBACK)
    DEFINE_BDT_OPCODE(stmib, HEADER_IB, BODY_STM, )
    DEFINE_BDT_OPCODE(stmibw, HEADER_IB, BODY_STM, DO_WRITEBACK)
    DEFINE_BDT_OPCODE(stmibs, HEADER_IB, BODY_STM_S, )
    DEFINE_BDT_OPCODE(stmibsw, HEADER_IB, BODY_STM_S, DO_WRITEBACK)
}
