#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/cpu/impl/thumb/msr.hpp>

#define MSR_OPCODE_HEADER \
    uint16_t offset = (opcode & 0x07c0) >> 6; \
    uint16_t Rs = (opcode & 0x0038) >> 3; \
    uint16_t Rd = opcode & 0x0007; \
    uint32_t Rs_v = registerRead(Rs)

#define logicSetFlags(result) \
    cpsr.fields.flagZ = !result; \
    cpsr.fields.flagN = SIGN32(result)

namespace gbaemu::gba::cpu::impl::thumb::msr {
    void opcode_lsl(uint16_t opcode) {
        MSR_OPCODE_HEADER;

        uint32_t result;

        if(offset) {
            result = Rs_v << offset;
            cpsr.fields.flagC = (Rs_v >> (32 - offset)) & 0x00000001;
        } else {
            result = Rs_v;
        }

        logicSetFlags(result);
        registerWrite(Rd, result);
    }

    void opcode_lsr(uint16_t opcode) {
        MSR_OPCODE_HEADER;

        uint32_t result;

        if(offset) {
            result = Rs_v >> offset;
            cpsr.fields.flagC = (Rs_v >> (offset - 1)) & 0x00000001;
        } else {
            result = 0;
            cpsr.fields.flagC = SIGN32(Rs_v);
        }

        logicSetFlags(result);
        registerWrite(Rd, result);
    }

    void opcode_asr(uint16_t opcode) {
        MSR_OPCODE_HEADER;

        uint32_t result;

        if(offset) {
            result = (int32_t)Rs_v >> offset;
            cpsr.fields.flagC = (Rs_v >> (offset - 1)) & 0x00000001;
        } else {
            result = (int32_t)Rs_v >> 31;
            cpsr.fields.flagC = SIGN32(Rs_v);
        }

        logicSetFlags(result);
        registerWrite(Rd, result);
    }
}
