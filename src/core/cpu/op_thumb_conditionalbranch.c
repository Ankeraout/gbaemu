#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbConditionalBranch(uint16_t p_opcode) {
    const uint16_t l_condition = (p_opcode >> 8) & 0xf;
    const uint32_t l_offset = signExtend8to32(p_opcode) << 1;

    if(cpuCheckCondition((enum te_cpuCondition)l_condition)) {
        cpuJump(g_cpuRegisterR[15] + l_offset);
    }
}
