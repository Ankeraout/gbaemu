#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/sdt.hpp>
#include <gbaemu/gba/mmu.hpp>

#define SDT_OPCODE_BASE \
    const uint32_t Rn = (opcode & 0x000f0000) >> 16; \
    const uint32_t Rd = (opcode & 0x0000f000) >> 12; \
    const uint32_t Rn_v = registerRead(Rn); \
    uint32_t address = Rn_v

#define DEFINE_SDT_OPCODE_SINGLE(name, suffix, body) \
    void opcode_ ## name ## _ ## suffix (uint32_t opcode) { \
        SDT_OPCODE_BASE; \
        uint32_t &op2 = shifter.result; \
        \
        gbaemu::gba::cpu::impl::arm::shift::shift ## suffix (opcode); \
        body; \
    }

#define DEFINE_SDT_OPCODE_SINGLE_I(name, body) \
    void opcode_ ## name ## _Immediate (uint32_t opcode) { \
        SDT_OPCODE_BASE; \
        uint32_t op2 = opcode & 0x00000fff; \
        \
        body; \
    }

#define DEFINE_SDT_OPCODE(name, body) \
    DEFINE_SDT_OPCODE_SINGLE(name, LSL_Rm_0, body) \
    DEFINE_SDT_OPCODE_SINGLE(name, LSL_Rm_1, body) \
    DEFINE_SDT_OPCODE_SINGLE(name, LSR_Rm_0, body) \
    DEFINE_SDT_OPCODE_SINGLE(name, LSR_Rm_1, body) \
    DEFINE_SDT_OPCODE_SINGLE(name, ASR_Rm_0, body) \
    DEFINE_SDT_OPCODE_SINGLE(name, ASR_Rm_1, body) \
    DEFINE_SDT_OPCODE_SINGLE(name, ROR_Rm_0, body) \
    DEFINE_SDT_OPCODE_SINGLE(name, ROR_Rm_1, body) \
    DEFINE_SDT_OPCODE_SINGLE_I(name, body)

#define DEFINE_STR_OPCODE(suffix, body) \
    DEFINE_SDT_OPCODE( \
        str ## suffix, \
        uint32_t Rd_v = registerRead(Rd); \
        body \
    )

#define DEFINE_LDR_OPCODE(suffix, body) \
    DEFINE_SDT_OPCODE( \
        ldr ## suffix, \
        body \
    )

#define DO_STORE \
    gbaemu::gba::mmu::write32(address, Rd_v)

#define DO_STORE_B \
    gbaemu::gba::mmu::write8(address, (uint8_t)Rd_v)

#define DO_WRITEBACK \
    registerWrite(Rn, address)

#define DO_POSTINDEX \
    address -= op2; \
    DO_WRITEBACK

#define DO_POSTINDEX_U \
    address += op2; \
    DO_WRITEBACK

#define DO_PREINDEX \
    address -= op2

#define DO_PREINDEX_U \
    address += op2

#define DO_LOAD \
    registerWrite(Rd, gbaemu::gba::mmu::read32(address))

#define DO_LOAD_B \
    registerWrite(Rd, gbaemu::gba::mmu::read8(address))

namespace gbaemu::gba::cpu::impl::arm::sdt {
    DEFINE_STR_OPCODE(, DO_STORE; DO_POSTINDEX)
    DEFINE_STR_OPCODE(b, DO_STORE_B; DO_POSTINDEX)
    DEFINE_STR_OPCODE(u, DO_STORE; DO_POSTINDEX_U)
    DEFINE_STR_OPCODE(ub, DO_STORE_B; DO_POSTINDEX_U)
    DEFINE_STR_OPCODE(p, DO_PREINDEX; DO_STORE)
    DEFINE_STR_OPCODE(pw, DO_PREINDEX; DO_STORE; DO_WRITEBACK)
    DEFINE_STR_OPCODE(pb, DO_PREINDEX; DO_STORE_B)
    DEFINE_STR_OPCODE(pbw, DO_PREINDEX; DO_STORE_B; DO_WRITEBACK)
    DEFINE_STR_OPCODE(pu, DO_PREINDEX_U; DO_STORE)
    DEFINE_STR_OPCODE(puw, DO_PREINDEX_U; DO_STORE; DO_WRITEBACK)
    DEFINE_STR_OPCODE(pub, DO_PREINDEX_U; DO_STORE_B)
    DEFINE_STR_OPCODE(pubw, DO_PREINDEX_U; DO_STORE_B; DO_WRITEBACK)

    DEFINE_LDR_OPCODE(, DO_LOAD; DO_POSTINDEX)
    DEFINE_LDR_OPCODE(b, DO_LOAD_B; DO_POSTINDEX)
    DEFINE_LDR_OPCODE(u, DO_LOAD; DO_POSTINDEX_U)
    DEFINE_LDR_OPCODE(ub, DO_LOAD_B; DO_POSTINDEX_U)
    DEFINE_LDR_OPCODE(p, DO_PREINDEX; DO_LOAD)
    DEFINE_LDR_OPCODE(pw, DO_PREINDEX; DO_LOAD; DO_WRITEBACK)
    DEFINE_LDR_OPCODE(pb, DO_PREINDEX; DO_LOAD_B)
    DEFINE_LDR_OPCODE(pbw, DO_PREINDEX; DO_LOAD_B; DO_WRITEBACK)
    DEFINE_LDR_OPCODE(pu, DO_PREINDEX_U; DO_LOAD)
    DEFINE_LDR_OPCODE(puw, DO_PREINDEX_U; DO_LOAD; DO_WRITEBACK)
    DEFINE_LDR_OPCODE(pub, DO_PREINDEX_U; DO_LOAD_B)
    DEFINE_LDR_OPCODE(pubw, DO_PREINDEX_U; DO_LOAD_B; DO_WRITEBACK)
}
