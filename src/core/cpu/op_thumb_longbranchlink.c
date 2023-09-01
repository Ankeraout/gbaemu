#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbLongBranchLink(uint16_t p_opcode) {
    const bool l_isLow = (p_opcode & (1 << 11)) != 0;
    const uint32_t l_offset = p_opcode & 0x7ff;

    if(l_isLow) {
        uint32_t l_nextInstructionAddress = g_cpuRegisterR[15] - 2;
        cpuJump(g_cpuRegisterR[14] + (l_offset << 1));
        g_cpuRegisterR[14] = l_nextInstructionAddress | 1;
    } else {
        const bool l_isNegative = (l_offset & (1 << 10)) != 0;

        g_cpuRegisterR[14] = g_cpuRegisterR[15] + ((l_offset << 12) | (l_isNegative ? 0xfffff800 : 0));
    }
}
