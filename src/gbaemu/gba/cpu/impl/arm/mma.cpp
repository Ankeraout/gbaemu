#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/cpu/impl/arm/mma.hpp>

#define OPCODE_BASE \
    const uint32_t Rd = (opcode & 0x000f0000) >> 16; \
    const uint32_t Rs = (opcode & 0x00000f00) >> 8; \
    const uint32_t Rm = opcode & 0x0000000f; \
    \
    const uint32_t Rs_v = registerRead(Rs); \
    const uint32_t Rm_v = registerRead(Rm)

#define MUL_BASE \
    const uint32_t result = Rs_v * Rm_v;

#define MLA_BASE \
    const uint32_t Rn = (opcode & 0x0000f000) >> 12; \
    \
    const uint32_t Rn_v = registerRead(Rn); \
    const uint32_t result = Rs_v * Rm_v + Rn_v;

#define COMPUTE_FLAGS \
    cpsr.fields.flagN = SIGN32(result); \
    cpsr.fields.flagZ = !result

#define FOOTER \
    registerWrite(Rd, result)

namespace gbaemu::gba::cpu::impl::arm::mma {
    void opcode_mul(uint32_t opcode) {
        OPCODE_BASE;
        MUL_BASE;
        FOOTER;
    }

    void opcode_muls(uint32_t opcode) {
        OPCODE_BASE;
        MUL_BASE;
        COMPUTE_FLAGS;
        FOOTER;
    }

    void opcode_mla(uint32_t opcode) {
        OPCODE_BASE;
        MLA_BASE;
        FOOTER;
    }

    void opcode_mlas(uint32_t opcode) {
        OPCODE_BASE;
        MLA_BASE;
        COMPUTE_FLAGS;
        FOOTER;
    }
}