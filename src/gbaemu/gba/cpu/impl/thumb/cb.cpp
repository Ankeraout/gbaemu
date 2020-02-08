#include <cstdint>

#include <gbaemu/gba/cpu.hpp>

#define DEFINE_OPCODE_CB(condName, cond) \
    void opcode_b ## condName(uint16_t opcode) { \
        int32_t offset = ((int8_t)(opcode & 0x00ff)) << 1; \
        \
        if(cond) { \
            gbaemu::gba::cpu::performJump(registerRead(CPU_REG_PC) + offset); \
        } \
    }

namespace gbaemu::gba::cpu::impl::thumb::cb {
    DEFINE_OPCODE_CB(eq, cpsr.fields.flagZ)
    DEFINE_OPCODE_CB(ne, !cpsr.fields.flagZ)
    DEFINE_OPCODE_CB(cs, cpsr.fields.flagC)
    DEFINE_OPCODE_CB(cc, !cpsr.fields.flagC)
    DEFINE_OPCODE_CB(mi, cpsr.fields.flagN)
    DEFINE_OPCODE_CB(pl, !cpsr.fields.flagN)
    DEFINE_OPCODE_CB(vs, cpsr.fields.flagV)
    DEFINE_OPCODE_CB(vc, !cpsr.fields.flagV)
    DEFINE_OPCODE_CB(hi, cpsr.fields.flagC && !cpsr.fields.flagZ)
    DEFINE_OPCODE_CB(ls, (!cpsr.fields.flagC) || cpsr.fields.flagZ)
    DEFINE_OPCODE_CB(ge, cpsr.fields.flagN == cpsr.fields.flagV)
    DEFINE_OPCODE_CB(lt, cpsr.fields.flagN != cpsr.fields.flagV)
    DEFINE_OPCODE_CB(gt, (!cpsr.fields.flagZ) && (cpsr.fields.flagN == cpsr.fields.flagV))
    DEFINE_OPCODE_CB(le, cpsr.fields.flagZ || (cpsr.fields.flagN != cpsr.fields.flagV))
}
