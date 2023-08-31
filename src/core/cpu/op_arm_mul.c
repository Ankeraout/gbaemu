#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeArmMul(uint32_t p_opcode) {
    const bool l_accumulate = (p_opcode & (1 << 21)) != 0;
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_rd = (p_opcode >> 16) & 0xf;
    const uint32_t l_rs = (p_opcode >> 8) & 0xf;
    const uint32_t l_rm = p_opcode & 0xf;

    const uint32_t l_rsValue = g_cpuRegisterR[l_rs];
    const uint32_t l_rmValue = g_cpuRegisterR[l_rm];

    uint32_t l_result;

    if(l_accumulate) {
        const uint32_t l_rn = (p_opcode >> 12) & 0xf;
        const uint32_t l_rnValue = g_cpuRegisterR[l_rn];

        l_result = l_rnValue + l_rsValue * l_rmValue;
    } else {
        l_result = l_rsValue * l_rmValue;
    }

    if(l_setFlags) {
        setFlagsLogical(l_result);
    }

    cpuWriteRegister(l_rd, l_result);
}
