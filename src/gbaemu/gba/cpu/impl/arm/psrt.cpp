#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/psrt.hpp>
#include <gbaemu/gba/cpu/impl/arm/shift_inline.hpp>

#define DEFINE_MRS(sourceRegisterName, sourceRegisterValue) \
    void opcode_mrs_ ## sourceRegisterName(uint32_t opcode) { \
        \
        if((opcode & 0x000f0fff) != 0x000f0000) { \
            cpu::raiseUnd(); \
        } \
        \
        uint32_t Rd = (opcode & 0x0000f000) >> 12; \
        \
        registerWrite(Rd, sourceRegisterValue); \
    }

#define MSR_CORE \
    uint32_t mask = 0x00000000; \
    \
    if(opcode & 0x00010000) { \
        mask |= 0x000000ff; \
    } \
    \
    if(opcode & 0x00020000) { \
        mask |= 0x0000ff00; \
    } \
    \
    if(opcode & 0x00040000) { \
        mask |= 0x00ff0000; \
    } \
    \
    if(opcode & 0x00080000) { \
        mask |= 0xff000000; \
    } \
    \
    psr &= ~mask; \
    psr |= mask & operand

#define DEFINE_MSR_SPSR(__operand, __suffix) \
    void opcode_msr_spsr ## __suffix(uint32_t opcode) { \
        uint32_t psr = readSPSR().value; \
        uint32_t operand = __operand; \
        \
        MSR_CORE; \
        \
        if(cpsr.fields.mode != PSR_MODE_USR) { \
            writeSPSR(psr); \
        } \
    }

#define DEFINE_MSR_CPSR(__operand, __suffix) \
    void opcode_msr_cpsr ## __suffix(uint32_t opcode) { \
        uint32_t psr = cpsr.value; \
        uint32_t operand = __operand; \
        \
        MSR_CORE; \
        \
        writeCPSR(psr); \
    }

#define OPERAND_REGISTER \
    registerRead(opcode & 0x0000000f)

#define OPERAND_IMMEDIATE \
    shift(opcode)

#define SUFFIX_IMMEDIATE _Immediate

namespace gbaemu::gba::cpu::impl::arm::psrt {
    static inline uint32_t shift(uint32_t opcode) {
        shift::shiftImmediate(opcode);
        return shifter.result;
    }

    DEFINE_MRS(cpsr, gbaemu::gba::cpu::cpsr.value)
    DEFINE_MRS(spsr, gbaemu::gba::cpu::readSPSR().value)
    DEFINE_MSR_CPSR(OPERAND_REGISTER, )
    DEFINE_MSR_CPSR(OPERAND_IMMEDIATE, _Immediate)
    DEFINE_MSR_SPSR(OPERAND_REGISTER, )
    DEFINE_MSR_SPSR(OPERAND_IMMEDIATE, _Immediate)
}
