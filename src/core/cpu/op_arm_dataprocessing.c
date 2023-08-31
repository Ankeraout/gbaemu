#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"
#include "core/cpu/shifter.h"

static inline uint32_t getOperand2(uint32_t p_opcode);

void cpuOpcodeArmAnd(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_result = l_operand & l_rnValue;

    cpuWriteRegister(l_rd, l_result);

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmEor(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_result = l_operand ^ l_rnValue;

    cpuWriteRegister(l_rd, l_result);

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmSub(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_result = l_rnValue - l_operand;

    cpuWriteRegister(l_rd, l_result);

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmRsb(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_result = l_operand - l_rnValue;

    cpuWriteRegister(l_rd, l_result);

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmAdd(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_result = l_rnValue + l_operand;

    cpuWriteRegister(l_rd, l_result);

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmAdc(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_carry = g_cpuFlagC ? 1 : 0;
    const uint32_t l_result = l_rnValue + l_operand + l_carry;

    cpuWriteRegister(l_rd, l_result);

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmSbc(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_carry = g_cpuFlagC ? 0 : -1;
    const uint32_t l_result = l_rnValue - l_operand + l_carry;

    cpuWriteRegister(l_rd, l_result);

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmRsc(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_carry = g_cpuFlagC ? 0 : -1;
    const uint32_t l_result = l_operand - l_rnValue + l_carry;

    cpuWriteRegister(l_rd, l_result);

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmTst(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_result = l_operand & l_rnValue;

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmTeq(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_result = l_operand ^ l_rnValue;

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmCmp(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_result = l_rnValue - l_operand;

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmCmn(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_result = l_rnValue + l_operand;

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmOrr(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_result = l_operand | l_rnValue;

    cpuWriteRegister(l_rd, l_result);

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmMov(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;

    cpuWriteRegister(l_rd, l_operand);

    if(l_setFlags) {
        setFlagsLogical(l_operand);
    }
}

void cpuOpcodeArmBic(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_result = l_operand & ~l_rnValue;

    cpuWriteRegister(l_rd, l_result);

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

void cpuOpcodeArmMvn(uint32_t p_opcode) {
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_operand = getOperand2(p_opcode);
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_result = ~l_operand;

    cpuWriteRegister(l_rd, l_result);

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }
}

static inline uint32_t getOperand2(uint32_t p_opcode) {
    bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;
    uint32_t l_result;

    if(l_isImmediate) {
        uint32_t l_immediate = (uint32_t)(p_opcode & 0x000000ff);
        uint32_t l_shift = (uint32_t)((p_opcode >> 8) & 0x0000000f);

        l_shift <<= 1;

        if(l_shift == 0) {
            g_cpuShifterCarry = g_cpuFlagC;
            l_result = l_immediate;
        } else {
            l_result = rotateRight(l_immediate, l_shift);
            g_cpuShifterCarry = ((uint32_t)l_result >> 31) != 0;
        }
    } else {
        enum te_shiftType l_shiftType = ((uint32_t)(p_opcode & 0x00000060)) >> 5;
        bool l_shiftByRegister = ((uint32_t)(p_opcode & 0x00000010)) != 0;
        uint32_t l_operandRegister = (uint32_t)(p_opcode & 0x0000000f);
        uint32_t l_operand = g_cpuRegisterR[l_operandRegister];

        if(l_operandRegister == E_CPUREGISTER_PC) {
            l_operand += C_INSTRUCTION_SIZE_ARM;
        }

        if(l_shiftByRegister) {
            uint32_t l_shiftRegister = ((uint32_t)(p_opcode & 0x00000f00)) >> 8;
            uint32_t l_shift = g_cpuRegisterR[l_shiftRegister];

            switch(l_shiftType) {
                case E_SHIFTTYPE_LSL:
                    if(l_shift == 0) {
                        g_cpuShifterCarry = g_cpuFlagC;
                        l_result = l_operand;
                    } else if(l_shift < 32) {
                        g_cpuShifterCarry =
                            ((l_operand >> (32 - l_shift)) & 0x00000001) != 0;
                        l_result = (uint32_t)(l_operand << l_shift);
                    } else if(l_shift == 32) {
                        g_cpuShifterCarry = (l_operand & 0x00000001) != 0;
                        l_result = 0;
                    } else {
                        g_cpuShifterCarry = false;
                        l_result = 0;
                    }

                    break;

                case E_SHIFTTYPE_LSR:
                    if(l_shift == 0) {
                        g_cpuShifterCarry = g_cpuFlagC;
                        l_result = l_operand;
                    } else if(l_shift < 32) {
                        g_cpuShifterCarry =
                            ((l_operand >> (l_shift - 1)) & 0x00000001) != 0;
                        l_result = (uint32_t)(l_operand >> l_shift);
                    } else if(l_shift == 32) {
                        g_cpuShifterCarry = (l_operand & 0x80000000) != 0;
                        l_result = 0;
                    } else {
                        g_cpuShifterCarry = false;
                        l_result = 0;
                    }

                    break;

                case E_SHIFTTYPE_ASR:
                    if(l_shift == 0) {
                        g_cpuShifterCarry = g_cpuFlagC;
                        l_result = l_operand;
                    } else if(l_shift < 32) {
                        g_cpuShifterCarry =
                            ((l_operand >> (l_shift - 1)) & 0x00000001) != 0;
                        l_result =
                            (uint32_t)(((int32_t)l_operand) >> l_shift);
                    } else {
                        g_cpuShifterCarry = (l_operand & 0x80000000) != 0;
                        l_result =
                            (uint32_t)(((int32_t)l_operand) >> 31);
                    }

                    break;

                case E_SHIFTTYPE_ROR:
                    {
                        uint32_t l_rotation = l_shift & 0x0000001f;

                        if(l_shift == 0) {
                            g_cpuShifterCarry = g_cpuFlagC;
                            l_result = l_operand;
                        } else if(l_rotation == 0) {
                            g_cpuShifterCarry = (l_operand & 0x80000000) != 0;
                            l_result = l_operand;
                        } else {
                            g_cpuShifterCarry =
                                ((l_operand >> (l_rotation - 1)) & 0x00000001) != 0;
                            l_result = rotateRight(l_operand, l_rotation);
                        }
                    }
            }
        } else {
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
    }

    return l_result;
}
