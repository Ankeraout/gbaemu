#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/psrt.hpp>
#include <gbaemu/gba/cpu/impl/arm/shift_inline.hpp>

#define DEFINE_MRS(sourceRegisterName, sourceRegisterValue) \
    void opcode_mrs_ ## sourceRegisterName(uint32_t opcode) { \
        uint32_t Rd = (opcode & 0x0000f000) >> 12; \
        \
        registerWrite(Rd, sourceRegisterValue); \
    }

namespace gbaemu::gba::cpu::impl::arm::psrt {
    DEFINE_MRS(cpsr, gbaemu::gba::cpu::cpsr.value)
    DEFINE_MRS(spsr, gbaemu::gba::cpu::readSPSR().value)

    void opcode_msr_cpsr(uint32_t opcode) {
        if(opcode & 0x00010000) {
            uint32_t Rm = opcode & 0x0000000f;
            gbaemu::gba::cpu::writeCPSR(gbaemu::gba::cpu::registerRead(Rm));
        } else {
            gbaemu::gba::cpu::impl::arm::shift::shiftImmediate(opcode);
            gbaemu::gba::cpu::cpsr.value &= 0x0fffffff;
            gbaemu::gba::cpu::cpsr.value |= gbaemu::gba::cpu::shifter.result & 0xf0000000;
        }
    }

    void opcode_msr_spsr(uint32_t opcode) {
        if(opcode & 0x00010000) {
            uint32_t Rm = opcode & 0x0000000f;
            gbaemu::gba::cpu::writeSPSR(gbaemu::gba::cpu::registerRead(Rm));
        } else {
            uint32_t value = gbaemu::gba::cpu::readSPSR().value;

            gbaemu::gba::cpu::impl::arm::shift::shiftImmediate(opcode);
            value &= 0x0fffffff;
            value |= gbaemu::gba::cpu::shifter.result & 0xf0000000;

            gbaemu::gba::cpu::writeSPSR(value);
        }
    }

    void opcode_msr_cpsr_flags_Immediate(uint32_t opcode) {
        gbaemu::gba::cpu::impl::arm::shift::shiftImmediate(opcode);
        gbaemu::gba::cpu::cpsr.value &= 0x0fffffff;
        gbaemu::gba::cpu::cpsr.value |= gbaemu::gba::cpu::shifter.result & 0xf0000000;
    }

    void opcode_msr_spsr_flags_Immediate(uint32_t opcode) {
        uint32_t value = gbaemu::gba::cpu::readSPSR().value;

        gbaemu::gba::cpu::impl::arm::shift::shiftImmediate(opcode);
        value &= 0x0fffffff;
        value |= gbaemu::gba::cpu::shifter.result & 0xf0000000;

        gbaemu::gba::cpu::writeSPSR(value);
    }
}
