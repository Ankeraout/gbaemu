#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/cpu/impl/thumb/mcas.hpp>

#define DEFINE_MCAS_OPCODE(name, body) \
    void opcode_ ## name(uint16_t opcode) { \
        uint16_t Rd = (opcode & 0x0700) >> 8; \
        uint32_t immediate = opcode & 0x00ff; \
        \
        body; \
    }

#define logicSetFlags(result) \
    cpsr.fields.flagZ = !result; \
    cpsr.fields.flagN = SIGN32(result)

namespace gbaemu::gba::cpu::impl::thumb::mcas {
    DEFINE_MCAS_OPCODE(
        mov,
        registerWrite(Rd, immediate);
        logicSetFlags(immediate)
    )

    DEFINE_MCAS_OPCODE(
        cmp,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result = Rd_v - immediate;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rd_v, immediate, result);
        cpsr.fields.flagC = SUB32_FLAGC(Rd_v, immediate);
    )

    DEFINE_MCAS_OPCODE(
        add,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result = Rd_v + immediate;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = ADD32_FLAGV(Rd_v, immediate, (uint32_t)result);
        cpsr.fields.flagC = result > UINT32_MAX;

        registerWrite(Rd, (uint32_t)result)
    )

    DEFINE_MCAS_OPCODE(
        sub,
        uint32_t Rd_v = registerRead(Rd);
        uint32_t result = Rd_v - immediate;

        cpsr.fields.flagZ = !result;
        cpsr.fields.flagN = SIGN32(result);
        cpsr.fields.flagV = SUB32_FLAGV(Rd_v, immediate, result);
        cpsr.fields.flagC = SUB32_FLAGC(Rd_v, immediate);

        registerWrite(Rd, result)
    )
}
