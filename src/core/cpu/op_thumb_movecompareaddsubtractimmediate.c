#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbMoveCompareAddSubtractImmediate(uint16_t p_opcode) {
    const uint16_t l_op = (p_opcode >> 11) & 0x3;
    const uint16_t l_rd = (p_opcode >> 8) & 0x7;
    const uint16_t l_immediate = p_opcode & 0xff;

    if(l_op == 0) { // MOV
        g_cpuRegisterR[l_rd] = l_immediate;
        setFlagsArithmetical(l_immediate);
    } else {
        const uint32_t l_rdValue = g_cpuRegisterR[l_rd];

        if(l_op == 1) { // CMP
            const uint32_t l_result = l_rdValue - l_immediate;
            g_cpuFlagC = getCarrySub(l_rdValue, l_immediate);
            g_cpuFlagV = getOverflowSub(l_rdValue, l_immediate, l_result);
            setFlagsArithmetical(l_result);
        } else {
            uint32_t l_result;

            if(l_op == 2) { // ADD
                l_result = l_rdValue + l_immediate;
                g_cpuFlagC = l_result < l_rdValue;
                g_cpuFlagV = getOverflowAdd(l_rdValue, l_immediate, l_result);
            } else { // SUB
                l_result = l_rdValue - l_immediate;
                g_cpuFlagC = getCarrySub(l_rdValue, l_immediate);
                g_cpuFlagV = getOverflowSub(l_rdValue, l_immediate, l_result);
            }

            g_cpuRegisterR[l_rd] = l_result;
            setFlagsArithmetical(l_result);
        }
    }
}
