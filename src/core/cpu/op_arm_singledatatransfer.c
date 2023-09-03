#include <stdbool.h>
#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"
#include "core/cpu/shifter.h"

static inline uint32_t getOperand(uint32_t p_opcode);

void cpuOpcodeArmSingleDataTransfer(uint32_t p_opcode) {
    const bool l_isPreIndexed = (p_opcode & (1 << 24)) != 0;
    const bool l_isUp = (p_opcode & (1 << 23)) != 0;
    const bool l_isByte = (p_opcode & (1 << 22)) != 0;
    const bool l_writeBack = (p_opcode & (1 << 21)) != 0;
    const bool l_isLoad = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;

    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];

    uint32_t l_address = l_rnValue;
    const uint32_t l_offset = getOperand(p_opcode);

    if(l_isPreIndexed) {
        if(l_isUp) {
            l_address += l_offset;
        } else {
            l_address -= l_offset;
        }
    }

    if(l_isLoad) {
        if(l_isByte) {
            g_cpuRegisterR[l_rd] = busRead8(l_address);
        } else {
            g_cpuRegisterR[l_rd] = busRead32(l_address);
        }
    } else {
        if(l_isByte) {
            busWrite8(l_address, g_cpuRegisterR[l_rd]);
        } else {
            busWrite32(l_address, g_cpuRegisterR[l_rd]);
        }
    }

    if(!l_isPreIndexed || l_writeBack) {
        if(!l_isPreIndexed) {
            if(l_isUp) {
                l_address += l_offset;
            } else {
                l_address -= l_offset;
            }
        }

        cpuWriteRegister(l_rn, l_address);
    }
}

static inline uint32_t getOperand(uint32_t p_opcode) {
    bool l_isImmediate = (p_opcode & (1 << 25)) == 0;
    uint32_t l_result;

    if(l_isImmediate) {
        l_result = p_opcode & 0x00000fff;
    } else {
        enum te_shiftType l_shiftType = ((uint32_t)(p_opcode & 0x00000060)) >> 5;
        uint32_t l_operandRegister = (uint32_t)(p_opcode & 0x0000000f);
        uint32_t l_operand = g_cpuRegisterR[l_operandRegister];

        if(l_operandRegister == E_CPUREGISTER_PC) {
            l_operand += C_INSTRUCTION_SIZE_ARM;
        }

        uint32_t l_shiftAmount = (uint32_t)((p_opcode & 0x00000f80) >> 7);

        switch(l_shiftType) {
            case E_SHIFTTYPE_LSL:
                if(l_shiftAmount == 0) {
                    g_cpuShifterCarry = g_cpuFlagC;
                    l_result = l_operand;
                } else {
                    g_cpuShifterCarry =
                        ((l_operand >> (32 - l_shiftAmount)) & 0x00000001)
                        != 0;
                    l_result = l_operand << l_shiftAmount;
                }

                break;

            case E_SHIFTTYPE_LSR:
                if(l_shiftAmount == 0) {
                    g_cpuShifterCarry = (l_operand & 0x80000000) != 0;
                    l_result = 0;
                } else {
                    g_cpuShifterCarry =
                        ((l_operand >> (l_shiftAmount - 1)) & 0x00000001)
                        != 0;
                    l_result = l_operand >> l_shiftAmount;
                }

                break;

            case E_SHIFTTYPE_ASR:
                if(l_shiftAmount == 0) {
                    g_cpuShifterCarry = (l_operand & 0x80000000) != 0;
                    l_result =
                        (uint32_t)(((int32_t)l_operand) >> 31);
                } else {
                    g_cpuShifterCarry =
                        ((l_operand >> (l_shiftAmount - 1)) & 0x00000001)
                        != 0;
                    l_result =
                        (uint32_t)(((int32_t)l_operand) >> l_shiftAmount);
                }

                break;

            case E_SHIFTTYPE_ROR:
                if(l_shiftAmount == 0) {
                    l_result = l_operand >> 1;

                    if(g_cpuFlagC) {
                        l_result |= 0x80000000;
                    }
                } else {
                    g_cpuShifterCarry =
                        ((l_operand >> (l_shiftAmount - 1)) & 0x00000001)
                        != 0;
                    l_result =
                        rotateRight(l_operand, l_shiftAmount);
                }

                break;
        }
    }

    return l_result;
}
