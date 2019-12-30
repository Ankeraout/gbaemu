#include <cstdint>

#include <gbaemu/gbaemu.hpp>
#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/dataproc.hpp>

#define HEADER_SHIFT_RM_0 \
    uint32_t Rm = opcode & 0x0000000f; \
    uint32_t immediate = (opcode & 0x00000f80) >> 7; \
    uint32_t Rm_v = registerRead(Rm)

#define HEADER_SHIFT_RM_1 \
    uint32_t Rm = opcode & 0x0000000f; \
    uint32_t Rs = (opcode & 0x00000f00) >> 8; \
    uint32_t Rm_v = registerRead(Rm); \
    uint32_t Rs_v = registerRead(Rs); \
    \
    uint32_t shift = Rs_v; \
    \
    if(Rs == CPU_REG_PC) { \
        shift += 4; \
    } \
    \
    shift &= 0xff; \
    \
    uint32_t value = Rm_v; \
    \
    if(Rm == CPU_REG_PC) { \
        value += 4; \
    }

#define CARRY(a, b)
#define ROR(value, rotation) ((((uint32_t)(value)) >> (rotation)) | (((uint32_t)(value)) << ((-(rotation)) & 0x1f)))
#define SIGN32(value) (((uint32_t)(value)) >> 31)

#define SUB32_FLAGC(a, b) ((a) >= (b))
#define SUB32_FLAGV(a, b, r) (SIGN32((a) ^ (b)) && SIGN32((a) ^ (r)))

#define SBC32_FLAGC(a, b, c) ((a) >= ((b) + (c)))

#define ADD32_FLAGV(a, b, r) (!(SIGN32((a) ^ (b))) && SIGN32((a) ^ (r)))

#define logicSetFlags(result) \
    cpsr.fields.flagZ = !result; \
    cpsr.fields.flagN = SIGN32(result); \
    cpsr.fields.flagC = shifter.flagC


#define DECLARE_DATAPROC_OPCODE_SINGLE(name, suffix, body) \
    void opcode_ ## name ## _ ## suffix (uint32_t opcode) { \
        uint32_t Rn = (opcode & 0x000f0000) >> 16; \
        uint32_t Rd = (opcode & 0x0000f000) >> 12; \
        uint32_t Rn_v = registerRead(Rn); \
        uint32_t &op2 = shifter.result; \
        \
        shift ## suffix (opcode); \
        body \
    }

#define DECLARE_DATAPROC_OPCODE(name, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSL_Rm_0, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSL_Rm_1, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSR_Rm_0, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSR_Rm_1, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ASR_Rm_0, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ASR_Rm_1, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ROR_Rm_0, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ROR_Rm_1, body) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, Immediate, body)

namespace gbaemu::gba::cpu::impl::dataproc {
    static inline void shiftLSL_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;

        if(immediate) {
            shifter.result = Rm_v << immediate;
            shifter.flagC = (Rm_v >> (32 - immediate)) & 0x00000001;
        } else {
            shifter.result = registerRead(Rm);
            shifter.flagC = cpsr.fields.flagC;
        }
    }

    static inline void shiftLSL_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;

        if(!shift) {
            shifter.result = value;
            shifter.flagC = cpsr.fields.flagC;
        } else if(shift < 32) {
            shifter.result = value << shift;
            shifter.flagC = (value >> (32 - shift)) & 0x00000001;
        } else if(shift == 32) {
            shifter.result = 0;
            shifter.flagC = value & 0x00000001;
        } else {
            shifter.result = 0;
            shifter.flagC = false;
        }
    }

    static inline void shiftLSR_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;

        if(immediate) {
            shifter.result = Rm_v >> immediate;
            shifter.flagC = (Rm_v >> (immediate - 1)) & 0x00000001;
        } else {
            shifter.result = 0;
            shifter.flagC = SIGN32(Rm_v);
        }
    }

    static inline void shiftLSR_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;

        if(!shift) {
            shifter.result = value;
            shifter.flagC = cpsr.fields.flagC;
        } else if(shift < 32) {
            shifter.result = value >> shift;
            shifter.flagC = (value >> (shift - 1)) & 0x00000001;
        } else if(shift == 32) {
            shifter.result = 0;
            shifter.flagC = SIGN32(value);
        } else {
            shifter.result = 0;
            shifter.flagC = false;
        }
    }

    static inline void shiftASR_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;

        if(immediate) {
            shifter.result = Rm_v >> immediate;
            shifter.flagC = (Rm_v >> (immediate - 1)) & 0x00000001;
        } else {
            shifter.result = SIGN32(Rm_v);
            shifter.flagC = shifter.flagC;
        }
    }

    static inline void shiftASR_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;

        if(!shift) {
            shifter.result = value;
            shifter.flagC = cpsr.fields.flagC;
        } else if(shift < 32) {
            shifter.result = value >> shift;
            shifter.flagC = (value >> (shift - 1)) & 0x00000001;
        } else if(SIGN32(Rm_v)) {
            shifter.result = 0xffffffff;
            shifter.flagC = true;
        } else {
            shifter.result = 0;
            shifter.flagC = false;
        }
    }

    static inline void shiftROR_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;

        if(immediate) {
            shifter.result = ROR(Rm_v, immediate);
            shifter.flagC = (Rm_v >> (immediate - 1)) & 0x00000001;
        } else {
            // RRX
            shifter.result = (cpsr.fields.flagC << 31) | (Rm_v >> 1);
            shifter.flagC = Rm_v & 0x00000001;
        }
    }

    static inline void shiftROR_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;

        uint32_t rotation = shift & 0x1f;

        if(!shift) {
            shifter.result = value;
            shifter.flagC = cpsr.fields.flagC;
        } else if(rotation) {
            shifter.result = ROR(value, rotation);
            shifter.flagC = (value >> (rotation - 1)) & 0x00000001;
        } else {
            shifter.result = value;
            shifter.flagC = SIGN32(value);
        }
    }

    static inline void shiftImmediate(uint32_t opcode) {
        unsigned int rotation = (opcode & 0x00000f00) >> 8;
        unsigned int immediate = opcode & 0x000000ff;

        if(rotation) {
            shifter.result = ROR(immediate, rotation);
            shifter.flagC = SIGN32(shifter.result);
        } else {
            shifter.result = immediate;
            shifter.flagC = cpsr.fields.flagC;
        }
    }

    DECLARE_DATAPROC_OPCODE(
        and,
        registerWrite(Rd, Rn_v & op2);
    )

    DECLARE_DATAPROC_OPCODE(
        ands,
        uint32_t result = Rn_v & op2;
        
        logicSetFlags(result);
        registerWrite(Rd, result);
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
    )

    DECLARE_DATAPROC_OPCODE(
        add,
        registerWrite(Rd, Rn_v + op2);
    )

    DECLARE_DATAPROC_OPCODE(
        adds,
        uint64_t result = Rn_v + op2;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = ADD32_FLAGV(op2, Rn_v, (uint32_t)result);
        cpsr.fields.flagC = result > UINT32_MAX;

        registerWrite(Rd, (uint32_t)result);
    )

    DECLARE_DATAPROC_OPCODE(
        adc,
        registerWrite(Rd, Rn_v + op2 + cpsr.fields.flagC);
    )

    DECLARE_DATAPROC_OPCODE(
        adcs,
        uint64_t result = Rn_v + op2 + cpsr.fields.flagC;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = ADD32_FLAGV(op2, Rn_v, (uint32_t)result);
        cpsr.fields.flagC = result > UINT32_MAX;

        registerWrite(Rd, (uint32_t)result);
    )

    DECLARE_DATAPROC_OPCODE(
        sbc,
        registerWrite(Rd, Rn_v - op2 - !cpsr.fields.flagC);
    )

    DECLARE_DATAPROC_OPCODE(
        sbcs,
        uint32_t result = Rn_v - op2 - !cpsr.fields.flagC;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rn_v, op2, result);
        cpsr.fields.flagC = SBC32_FLAGC(Rn_v, op2, !cpsr.fields.flagC);

        registerWrite(Rd, result);
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
    )

    DECLARE_DATAPROC_OPCODE(
        tst,
        uint32_t result = Rn_v & op2;
        
        logicSetFlags(result);
        registerWrite(Rd, result);
    )

    DECLARE_DATAPROC_OPCODE(
        teq,
        uint32_t result = Rn_v ^ op2;
        
        logicSetFlags(result);
        registerWrite(Rd, result);
    )

    DECLARE_DATAPROC_OPCODE(
        cmp,
        UNUSED(Rd);

        uint32_t result = Rn_v - op2;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rn_v, op2, result);
        cpsr.fields.flagC = SUB32_FLAGC(Rn_v, op2);
    )

    DECLARE_DATAPROC_OPCODE(
        cmn,
        UNUSED(Rd);
        uint64_t result = Rn_v + op2;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = ADD32_FLAGV(op2, Rn_v, (uint32_t)result);
        cpsr.fields.flagC = result > UINT32_MAX;
    )
}
