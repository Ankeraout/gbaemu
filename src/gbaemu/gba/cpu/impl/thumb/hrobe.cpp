#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/cpu/impl/thumb/hrobe.hpp>

#define DEFINE_HROBE_ALU_OPCODE(op, h1off, h2off, body) \
    void opcode_ ## op(uint16_t opcode) { \
        uint16_t Rs = ((opcode & 0x0038) >> 3) h1off; \
        uint16_t Rd = ((opcode & 0x0007) >> 3) h2off; \
        uint32_t Rs_v = registerRead(Rs); \
        \
        body \
    }

#define DEFINE_HROBE_ALU_OPCODE_VARIANTS(op, body) \
    DEFINE_HROBE_ALU_OPCODE(op ## _Rd_Hs, , + 8, body); \
    DEFINE_HROBE_ALU_OPCODE(op ## _Hd_Rs, + 8, , body); \
    DEFINE_HROBE_ALU_OPCODE(op ## _Hd_Hs, + 8, + 8, body)

#define DEFINE_HROBE_BX_OPCODE(variant, h1off) \
    void opcode_bx_ ## variant(uint16_t opcode) { \
        uint32_t dest = registerRead(((opcode & 0x0038) >> 3) h1off); \
        cpsr.fields.flagT = dest & 0x00000001; \
        \
        if(dest & 0x00000001) { \
            dest &= 0xfffffffe; \
            dest += 2; \
        } else { \
            dest &= 0xfffffffc; \
        } \
        \
        performJump(dest); \
    }

#define logicSetFlags(result) \
    cpsr.fields.flagZ = !result; \
    cpsr.fields.flagN = SIGN32(result)

namespace gbaemu::gba::cpu::impl::thumb::hrobe {
    DEFINE_HROBE_ALU_OPCODE_VARIANTS(
        add,
        registerWrite(Rd, Rs_v + registerRead(Rd));
    )

    DEFINE_HROBE_ALU_OPCODE_VARIANTS(
        cmp,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result = Rd_v - Rs_v;

        logicSetFlags(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rd_v, Rs_v, result);
        cpsr.fields.flagC = SUB32_FLAGC(Rd_v, Rs_v);
    )

    DEFINE_HROBE_ALU_OPCODE_VARIANTS(
        mov,
        registerWrite(Rd, Rs_v);
    )

    DEFINE_HROBE_BX_OPCODE(Rs, )
    DEFINE_HROBE_BX_OPCODE(Hs, + 8)
}
