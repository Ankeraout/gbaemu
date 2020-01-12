#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/mlmal.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/mmu.hpp>

#define OPCODE_BASE \
    const uint32_t RdHi = (opcode & 0x000f0000) >> 16; \
    const uint32_t RdLo = (opcode & 0x0000f000) >> 12; \
    const uint32_t Rs = (opcode & 0x00000f00) >> 8; \
    const uint32_t Rm = opcode & 0x0000000f; \
    \
    const uint32_t Rm_v = registerRead(Rm); \
    const uint32_t Rs_v = registerRead(Rs); \

#define ACCUMULATE_BASE \
    const uint64_t RdHi_v = registerRead(RdHi); \
    const uint64_t RdLo_v = registerRead(RdLo); \
    \
    const uint64_t Rd_v = (RdHi_v << 32) | RdLo_v

#define UMULL_BASE \
    const uint64_t result = Rm_v * Rs_v

#define UMLAL_BASE \
    ACCUMULATE_BASE; \
    const uint64_t result = Rm_v * Rs_v + Rd_v

#define SMULL_BASE \
    const uint64_t result = (int32_t)Rm_v * (int32_t)Rs_v

#define SMLAL_BASE \
    ACCUMULATE_BASE; \
    const uint64_t result = (int32_t)Rm_v * (int32_t)Rs_v + Rd_v

#define COMPUTE_FLAGS \
    cpsr.fields.flagZ = !result; \
    cpsr.fields.flagN = SIGN64(result)

#define FOOTER \
    registerWrite(RdHi, result >> 32); \
    registerWrite(RdLo, result)

namespace gbaemu::gba::cpu::impl::arm::mlmal {
    void opcode_umull(uint32_t opcode) {
        OPCODE_BASE;
        UMULL_BASE;
        FOOTER;
    }

    void opcode_umulls(uint32_t opcode) {
        OPCODE_BASE;
        UMULL_BASE;
        COMPUTE_FLAGS;
        FOOTER;
    }

    void opcode_umlal(uint32_t opcode) {
        OPCODE_BASE;
        UMLAL_BASE;
        FOOTER;
    }

    void opcode_umlals(uint32_t opcode) {
        OPCODE_BASE;
        UMLAL_BASE;
        COMPUTE_FLAGS;
        FOOTER;
    }

    void opcode_smull(uint32_t opcode) {
        OPCODE_BASE;
        SMULL_BASE;
        FOOTER;
    }

    void opcode_smulls(uint32_t opcode) {
        OPCODE_BASE;
        SMULL_BASE;
        COMPUTE_FLAGS;
        FOOTER;
    }

    void opcode_smlal(uint32_t opcode) {
        OPCODE_BASE;
        SMLAL_BASE;
        FOOTER;
    }

    void opcode_smlals(uint32_t opcode) {
        OPCODE_BASE;
        SMLAL_BASE;
        COMPUTE_FLAGS;
        FOOTER;
    }
}