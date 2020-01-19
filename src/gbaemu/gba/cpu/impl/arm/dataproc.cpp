#include <cstdint>

#include <gbaemu/gbaemu.hpp>
#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/dataproc.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/cpu/impl/arm/shift_inline.hpp>

#define logicSetFlags(result) \
    cpsr.fields.flagZ = !result; \
    cpsr.fields.flagN = SIGN32(result); \
    cpsr.fields.flagC = shifter.flagC

#define S_FOOTER \
    if(Rd == 15) { \
        cpsr = readSPSR(); \
    }

#define DECLARE_DATAPROC_OPCODE_SINGLE(name, suffix, rncheck, body) \
    void opcode_ ## name ## _ ## suffix (uint32_t opcode) { \
        uint32_t Rn = (opcode & 0x000f0000) >> 16; \
        uint32_t Rd = (opcode & 0x0000f000) >> 12; \
        uint64_t Rn_v = registerRead(Rn); \
        \
        rncheck; \
        \
        gbaemu::gba::cpu::impl::arm::shift::shift ## suffix (opcode); \
        \
        uint64_t op2 = shifter.result; \
        \
        body; \
    }

#define CHECK_RN_SHIFTREG \
    if(Rn == 15) { \
        Rn_v += 4; \
    }

#define DECLARE_DATAPROC_OPCODE(name, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSL_Rm_0, , body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSL_Rm_1, CHECK_RN_SHIFTREG, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSR_Rm_0, , body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSR_Rm_1, CHECK_RN_SHIFTREG, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ASR_Rm_0, , body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ASR_Rm_1, CHECK_RN_SHIFTREG, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ROR_Rm_0, , body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ROR_Rm_1, CHECK_RN_SHIFTREG, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, Immediate, , body)

namespace gbaemu::gba::cpu::impl::arm::dataproc {
    DECLARE_DATAPROC_OPCODE(
        and,
        registerWrite(Rd, Rn_v & op2);
    )

    DECLARE_DATAPROC_OPCODE(
        ands,
        uint32_t result = Rn_v & op2;
        
        logicSetFlags(result);
        registerWrite(Rd, result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        eor,
        registerWrite(Rd, Rn_v ^ op2);
    )

    DECLARE_DATAPROC_OPCODE(
        eors,
        uint32_t result = Rn_v ^ op2;
        
        logicSetFlags(result);
        registerWrite(Rd, result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        sub,
        registerWrite(Rd, Rn_v - op2);
    )

    DECLARE_DATAPROC_OPCODE(
        subs,
        uint32_t result = Rn_v - op2;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rn_v, op2, result);
        cpsr.fields.flagC = SUB32_FLAGC(Rn_v, op2);

        registerWrite(Rd, result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        rsb,
        registerWrite(Rd, op2 - Rn_v);
    )

    DECLARE_DATAPROC_OPCODE(
        rsbs,
        uint32_t result = op2 - Rn_v;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = SUB32_FLAGV(op2, Rn_v, result);
        cpsr.fields.flagC = SUB32_FLAGC(op2, Rn_v);

        registerWrite(Rd, result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        add,
        registerWrite(Rd, Rn_v + op2);
    )

    DECLARE_DATAPROC_OPCODE(
        adds,
        uint64_t result = Rn_v + op2;

        cpsr.fields.flagZ = !((uint32_t)result);
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = ADD32_FLAGV(op2, Rn_v, (uint32_t)result);
        cpsr.fields.flagC = result >> 32;

        registerWrite(Rd, (uint32_t)result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        adc,
        registerWrite(Rd, Rn_v + op2 + cpsr.fields.flagC);
    )

    DECLARE_DATAPROC_OPCODE(
        adcs,
        uint64_t result = Rn_v + op2 + cpsr.fields.flagC;

        cpsr.fields.flagZ = !((uint32_t)result);
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = ADD32_FLAGV(op2, Rn_v, (uint32_t)result);
        cpsr.fields.flagC = result >> 32;

        registerWrite(Rd, (uint32_t)result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        sbc,
        registerWrite(Rd, Rn_v - op2 + cpsr.fields.flagC - 1);
    )

    DECLARE_DATAPROC_OPCODE(
        sbcs,
        uint32_t result = Rn_v - op2 + cpsr.fields.flagC - 1;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rn_v, op2, result);
        cpsr.fields.flagC = SBC32_FLAGC(Rn_v, op2, !cpsr.fields.flagC);

        registerWrite(Rd, result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        rsc,
        registerWrite(Rd, op2 - Rn_v - !cpsr.fields.flagC);
    )

    DECLARE_DATAPROC_OPCODE(
        rscs,
        uint32_t result = op2 - Rn_v - !cpsr.fields.flagC;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = SUB32_FLAGV(op2, Rn_v, result);
        cpsr.fields.flagC = SBC32_FLAGC(op2, Rn_v, !cpsr.fields.flagC);

        registerWrite(Rd, result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        tst,
        uint32_t result = Rn_v & op2;
        
        logicSetFlags(result);
        registerWrite(Rd, result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        teq,
        uint32_t result = Rn_v ^ op2;
        
        logicSetFlags(result);
        registerWrite(Rd, result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        cmp,
        UNUSED(Rd);

        uint32_t result = Rn_v - op2;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rn_v, op2, result);
        cpsr.fields.flagC = SUB32_FLAGC(Rn_v, op2);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        cmn,
        UNUSED(Rd);
        uint64_t result = Rn_v + op2;

        cpsr.fields.flagZ = !((uint32_t)result);
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = ADD32_FLAGV(op2, Rn_v, (uint32_t)result);
        cpsr.fields.flagC = result > UINT32_MAX;

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        orr,
        registerWrite(Rd, Rn_v | op2);
    )

    DECLARE_DATAPROC_OPCODE(
        orrs,
        uint32_t result = Rn_v | op2;
        
        logicSetFlags(result);
        registerWrite(Rd, result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        mov,
        UNUSED(Rn_v);

        registerWrite(Rd, op2);
    )

    DECLARE_DATAPROC_OPCODE(
        movs,
        UNUSED(Rn_v);

        logicSetFlags(op2);
        registerWrite(Rd, op2);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        bic,
        registerWrite(Rd, Rn_v & ~op2);
    )

    DECLARE_DATAPROC_OPCODE(
        bics,
        uint32_t result = Rn_v & ~op2;
        
        logicSetFlags(result);
        registerWrite(Rd, result);

        S_FOOTER;
    )

    DECLARE_DATAPROC_OPCODE(
        mvn,
        UNUSED(Rn_v);

        registerWrite(Rd, ~op2);
    )

    DECLARE_DATAPROC_OPCODE(
        mvns,
        UNUSED(Rn_v);
        uint32_t result = ~op2;

        logicSetFlags(result);
        registerWrite(Rd, result);

        S_FOOTER;
    )
}
