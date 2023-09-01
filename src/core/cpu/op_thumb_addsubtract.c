#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbAddSubtract(uint16_t p_opcode) {
    const bool l_isImmediate = (p_opcode & (1 << 10)) != 0;
    const bool l_isSub = (p_opcode & (1 << 9)) != 0;
    const uint16_t l_rn = (p_opcode >> 6) & 0x7;
    const uint16_t l_rs = (p_opcode >> 3) & 0x7;
    const uint16_t l_rd = p_opcode & 0x7;

    const uint32_t l_operand2 = l_isImmediate ? l_rn : g_cpuRegisterR[l_rn];
    const uint32_t l_operand = g_cpuRegisterR[l_rs];

    uint32_t l_result;

    if(l_isSub) {
        l_result = l_operand - l_operand2;
        g_cpuFlagC = getCarrySub(l_operand, l_operand2);
        g_cpuFlagV = getOverflowSub(l_operand, l_operand2, l_result);
    } else {
        l_result = l_operand + l_operand2;
        g_cpuFlagC = l_result < l_operand;
        g_cpuFlagV = getOverflowAdd(l_operand, l_operand2, l_result);
    }

    g_cpuRegisterR[l_rd] = l_result;
    setFlagsArithmetical(l_result);
}