#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/cpu/impl/thumb/addsub.hpp>

#define SUB32_FLAGC(a, b) ((a) >= (b))
#define SUB32_FLAGV(a, b, r) (SIGN32((a) ^ (b)) && SIGN32((a) ^ (r)))

#define ADD32_FLAGV(a, b, r) (!(SIGN32((a) ^ (b))) && SIGN32((a) ^ (r)))

#define ADDSUB_HEADER \
    uint16_t Rn_offset3 = (opcode & 0x01c0) >> 6; \
    uint16_t Rs = (opcode & 0x0038) >> 3; \
    uint16_t Rd = opcode & 0x0007; \
    uint32_t Rs_v = registerRead(Rs)

namespace gbaemu::gba::cpu::impl::thumb::addsub {
    void opcode_add_Rn(uint16_t opcode) {
        ADDSUB_HEADER;

        uint32_t Rn_v = registerRead(Rn_offset3);
        uint64_t result = Rs_v + Rn_v;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = ADD32_FLAGV(Rs_v, Rn_v, (uint32_t)result);
        cpsr.fields.flagC = result > UINT32_MAX;

        registerWrite(Rd, (uint32_t)result);
    }

    void opcode_add_Offset(uint16_t opcode) {
        ADDSUB_HEADER;

        uint64_t result = Rs_v + Rn_offset3;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = ADD32_FLAGV(Rs_v, Rn_offset3, (uint32_t)result);
        cpsr.fields.flagC = result > UINT32_MAX;

        registerWrite(Rd, (uint32_t)result);
    }

    void opcode_sub_Rn(uint16_t opcode) {
        ADDSUB_HEADER;

        uint32_t Rn_v = registerRead(Rn_offset3);
        uint32_t result = Rs_v - Rn_v;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rs_v, Rn_v, result);
        cpsr.fields.flagC = SUB32_FLAGC(Rs_v, Rn_v);

        registerWrite(Rd, result);
    }

    void opcode_sub_Offset(uint16_t opcode) {
        ADDSUB_HEADER;

        uint32_t result = Rs_v - Rn_offset3;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rs_v, Rn_offset3, result);
        cpsr.fields.flagC = SUB32_FLAGC(Rs_v, Rn_offset3);

        registerWrite(Rd, result);
    }
}