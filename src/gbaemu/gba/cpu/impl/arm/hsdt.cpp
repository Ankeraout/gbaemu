#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/hsdt.hpp>
#include <gbaemu/gba/mmu.hpp>

#define DEFINE_HSDT_OPCODE_SINGLE(name, body) \
    void opcode_ ## name(uint32_t opcode) { \
        const uint32_t Rn = (opcode & 0x000f0000) >> 16; \
        const uint32_t Rd = (opcode & 0x0000f000) >> 12; \
        const uint32_t Rn_v = registerRead(Rn); \
        uint32_t address = Rn_v; \
        \
        body; \
    }

#define DEFINE_STR_OPCODE_SINGLE(variant, body) \
    DEFINE_HSDT_OPCODE_SINGLE( \
        str ## variant, \
        const uint32_t value = registerRead(Rd); \
        \
        body \
    )

#define DEFINE_STR_OPCODE(variant, body) \
    DEFINE_STR_OPCODE_SINGLE( \
        h ## variant, \
        const uint32_t Rm = opcode & 0x0000000f; \
        const uint32_t offset = registerRead(Rm); \
        \
        body \
    ) \
    \
    DEFINE_STR_OPCODE_SINGLE( \
        ih ## variant, \
        const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f); \
        \
        body \
    )

#define DEFINE_LDR_OPCODE_SINGLE(variant, body) \
    DEFINE_HSDT_OPCODE_SINGLE( \
        ldr ## variant, \
        body; \
        \
        registerWrite(Rd, value) \
    )

#define DEFINE_LDR_OPCODE(variant, body) \
    DEFINE_LDR_OPCODE_SINGLE( \
        variant, \
        const uint32_t Rm = opcode & 0x0000000f; \
        const uint32_t offset = registerRead(Rm); \
        \
        body \
    ) \
    \
    DEFINE_LDR_OPCODE_SINGLE( \
        i ## variant, \
        const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f); \
        \
        body \
    )

#define DO_PREINDEX \
    address -= offset

#define DO_PREINDEX_U \
    address += offset

#define DO_POSTINDEX \
    address -= offset; \
    DO_WRITEBACK

#define DO_POSTINDEX_U \
    address += offset; \
    DO_WRITEBACK

#define DO_STORE \
    mmu::write16(address & 0xfffffffe, (uint16_t)value)

#define DO_LOAD_H \
    const uint32_t value = mmu::read16(address & 0xfffffffe)

#define DO_LOAD_S \
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address))

#define DO_LOAD_SH \
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe))

#define DO_WRITEBACK \
    registerWrite(Rn, address)

namespace gbaemu::gba::cpu::impl::arm::hsdt {
    DEFINE_STR_OPCODE(, DO_STORE; DO_POSTINDEX)
    DEFINE_STR_OPCODE(u, DO_STORE; DO_POSTINDEX_U)
    DEFINE_STR_OPCODE(p, DO_PREINDEX; DO_STORE)
    DEFINE_STR_OPCODE(pw, DO_PREINDEX; DO_STORE; DO_WRITEBACK)
    DEFINE_STR_OPCODE(pu, DO_PREINDEX_U; DO_STORE)
    DEFINE_STR_OPCODE(puw, DO_PREINDEX_U; DO_STORE; DO_WRITEBACK)

    DEFINE_LDR_OPCODE(h, DO_LOAD_H; DO_POSTINDEX)
    DEFINE_LDR_OPCODE(hu, DO_LOAD_H; DO_POSTINDEX_U)
    DEFINE_LDR_OPCODE(hp, DO_PREINDEX; DO_LOAD_H)
    DEFINE_LDR_OPCODE(hpw, DO_PREINDEX; DO_LOAD_H; DO_WRITEBACK)
    DEFINE_LDR_OPCODE(hpu, DO_PREINDEX_U; DO_LOAD_H)
    DEFINE_LDR_OPCODE(hpuw, DO_PREINDEX_U; DO_LOAD_H; DO_WRITEBACK)

    DEFINE_LDR_OPCODE(s, DO_LOAD_S; DO_POSTINDEX)
    DEFINE_LDR_OPCODE(su, DO_LOAD_S; DO_POSTINDEX_U)
    DEFINE_LDR_OPCODE(sp, DO_PREINDEX; DO_LOAD_S)
    DEFINE_LDR_OPCODE(spw, DO_PREINDEX; DO_LOAD_S; DO_WRITEBACK)
    DEFINE_LDR_OPCODE(spu, DO_PREINDEX_U; DO_LOAD_S)
    DEFINE_LDR_OPCODE(spuw, DO_PREINDEX_U; DO_LOAD_S; DO_WRITEBACK)

    DEFINE_LDR_OPCODE(sh, DO_LOAD_SH; DO_POSTINDEX)
    DEFINE_LDR_OPCODE(shu, DO_LOAD_SH; DO_POSTINDEX_U)
    DEFINE_LDR_OPCODE(shp, DO_PREINDEX; DO_LOAD_SH)
    DEFINE_LDR_OPCODE(shpw, DO_PREINDEX; DO_LOAD_SH; DO_WRITEBACK)
    DEFINE_LDR_OPCODE(shpu, DO_PREINDEX_U; DO_LOAD_SH)
    DEFINE_LDR_OPCODE(shpuw, DO_PREINDEX_U; DO_LOAD_SH; DO_WRITEBACK)
}
