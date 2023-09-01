#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbUnconditionalBranch(uint16_t p_opcode) {
    const bool l_isNegative = (p_opcode & (1 << 10)) != 0;
    const uint32_t l_offset = ((p_opcode << 1) & 0xffe) | (l_isNegative ? 0xfffff000 : 0);

    cpuJump(g_cpuRegisterR[15] + l_offset);
}
