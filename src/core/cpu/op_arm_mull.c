#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeArmMull(uint32_t p_opcode) {
    const bool l_isUnsigned = (p_opcode & (1 << 22)) != 0;
    const bool l_accumulate = (p_opcode & (1 << 21)) != 0;
    const bool l_setFlags = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_rdHi = (p_opcode >> 16) & 0xf;
    const uint32_t l_rdLo = (p_opcode >> 12) & 0xf;
    const uint32_t l_rs = (p_opcode >> 8) & 0xf;
    const uint32_t l_rm = p_opcode & 0xf;

    const uint32_t l_rsValue = g_cpuRegisterR[l_rs];
    const uint32_t l_rmValue = g_cpuRegisterR[l_rm];

    uint64_t l_result;

    if(l_isUnsigned) {
        l_result = ((uint64_t)l_rsValue) * ((uint64_t)l_rmValue);
    } else {
        l_result = ((int64_t)((int32_t)l_rsValue)) * ((int64_t)((int32_t)l_rmValue));
    }

    if(l_accumulate) {
        const uint32_t l_rdHiValue = g_cpuRegisterR[l_rdHi];
        const uint32_t l_rdLoValue = g_cpuRegisterR[l_rdLo];

        l_result += (((uint64_t)l_rdHiValue) << 32ULL) | l_rdLoValue;
    }

    cpuWriteRegister(g_cpuRegisterR[l_rdHi], l_result >> 32UL);
    cpuWriteRegister(g_cpuRegisterR[l_rdLo], l_result);

    if(l_setFlags) {
        g_cpuFlagZ = l_result == 0ULL;
        g_cpuFlagN = (l_result & (1ULL << 63ULL)) != 0;
    }
}
