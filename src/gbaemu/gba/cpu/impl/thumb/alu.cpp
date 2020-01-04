#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lbl.hpp>

#define DEFINE_ALU_OPCODE(name, body) \
    void opcode_ ## name(uint16_t opcode) { \
        uint16_t Rs = (opcode & 0x0038) >> 3; \
        uint16_t Rd = opcode & 0x0007; \
        uint32_t Rs_v = registerRead(Rs); \
        \
        body \
    }

#define logicSetFlags(result) \
    cpsr.fields.flagZ = !result; \
    cpsr.fields.flagN = SIGN32(result)

namespace gbaemu::gba::cpu::impl::thumb::alu {
    DEFINE_ALU_OPCODE(
        and,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result = Rd_v & Rs_v;

        logicSetFlags(result);

        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        eor,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result = Rd_v ^ Rs_v;

        logicSetFlags(result);

        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        lsl,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result;

        if(!Rs_v) {
            result = Rd_v;
            gbaemu::gba::cpu::cpsr.fields.flagC = cpsr.fields.flagC;
        } else if(Rs_v < 32) {
            result = Rd_v << Rs_v;
            gbaemu::gba::cpu::cpsr.fields.flagC = (Rd_v >> (32 - Rs_v)) & 0x00000001;
        } else if(Rs_v == 32) {
            result = 0;
            gbaemu::gba::cpu::cpsr.fields.flagC = Rd_v & 0x00000001;
        } else {
            result = 0;
            gbaemu::gba::cpu::cpsr.fields.flagC = false;
        }

        logicSetFlags(result);
        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        lsr,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result;

        if(!Rs_v) {
            result = Rd_v;
            gbaemu::gba::cpu::cpsr.fields.flagC = cpsr.fields.flagC;
        } else if(Rs_v < 32) {
            result = Rd_v >> Rs_v;
            gbaemu::gba::cpu::cpsr.fields.flagC = (Rd_v >> (Rs_v - 1)) & 0x00000001;
        } else if(Rs_v == 32) {
            result = 0;
            gbaemu::gba::cpu::cpsr.fields.flagC = SIGN32(Rd_v);
        } else {
            result = 0;
            gbaemu::gba::cpu::cpsr.fields.flagC = false;
        }

        logicSetFlags(result);
        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        asr,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result;

        if(!Rs_v) {
            result = Rd_v;
            gbaemu::gba::cpu::cpsr.fields.flagC = cpsr.fields.flagC;
        } else if(Rs_v < 32) {
            result = Rd_v >> Rs_v;
            gbaemu::gba::cpu::cpsr.fields.flagC = (Rd_v >> (Rs_v - 1)) & 0x00000001;
        } else if(SIGN32(Rd_v)) {
            result = 0xffffffff;
            gbaemu::gba::cpu::cpsr.fields.flagC = true;
        } else {
            result = 0;
            gbaemu::gba::cpu::cpsr.fields.flagC = false;
        }

        logicSetFlags(result);
        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        adc,
        uint32_t Rd_v = registerRead(Rd);
        uint64_t result = Rd_v + Rs_v + cpsr.fields.flagC;

        logicSetFlags(result);
        cpsr.fields.flagV = ADD32_FLAGV(Rd_v, Rs_v, result);
        cpsr.fields.flagC = result > UINT32_MAX;

        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        sbc,
        uint32_t Rd_v = registerRead(Rd);
        uint64_t result = Rd_v - Rs_v - !cpsr.fields.flagC;

        logicSetFlags(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rd_v, Rs_v, result);
        cpsr.fields.flagC = SBC32_FLAGC(Rd_v, Rs_v, !cpsr.fields.flagC);

        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        ror,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result;
        uint32_t rotation = Rs_v & 0x1f;

        if(!Rs_v) {
            result = Rd_v;
            gbaemu::gba::cpu::cpsr.fields.flagC = cpsr.fields.flagC;
        } else if(rotation) {
            result = ROR(Rd_v, rotation);
            gbaemu::gba::cpu::cpsr.fields.flagC = (Rd_v >> (rotation - 1)) & 0x00000001;
        } else {
            result = Rd_v;
            gbaemu::gba::cpu::cpsr.fields.flagC = SIGN32(Rd_v);
        }

        logicSetFlags(result);
        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        tst,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result = Rd_v & Rs_v;

        logicSetFlags(result);
    )

    DEFINE_ALU_OPCODE(
        neg,
        uint32_t result = -Rs_v;

        logicSetFlags(result);

        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        cmp,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result = Rd_v - Rs_v;

        logicSetFlags(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rd_v, Rs_v, result);
        cpsr.fields.flagC = SUB32_FLAGC(Rd_v, Rs_v);
    )

    DEFINE_ALU_OPCODE(
        cmn,
        uint32_t Rd_v = registerRead(Rd);
        uint64_t result = Rd_v + Rs_v;

        logicSetFlags(result);
        cpsr.fields.flagV = ADD32_FLAGV(Rd_v, Rs_v, result);
        cpsr.fields.flagC = result > UINT32_MAX;
    )

    DEFINE_ALU_OPCODE(
        orr,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result = Rd_v | Rs_v;

        logicSetFlags(result);

        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        mul,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result = Rd_v * Rs_v;

        logicSetFlags(result);

        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        bic,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result = Rd_v & ~Rs_v;

        logicSetFlags(result);

        registerWrite(Rd, result);
    )

    DEFINE_ALU_OPCODE(
        mvn,
        uint32_t result = ~Rs_v;

        logicSetFlags(result);

        registerWrite(Rd, result);
    )
}
