#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeArmB(uint32_t p_opcode) {
    const uint32_t l_offset = signExtend26to32((p_opcode & 0x00ffffff) << 2);
    cpuJump(g_cpuRegisterR[E_CPUREGISTER_PC] + l_offset);
}

void cpuOpcodeArmBl(uint32_t p_opcode) {
    const uint32_t l_offset = signExtend26to32((p_opcode & 0x00ffffff) << 2);

    g_cpuRegisterR[E_CPUREGISTER_LR] = g_cpuRegisterR[E_CPUREGISTER_PC] - 4;

    cpuJump(g_cpuRegisterR[E_CPUREGISTER_PC] + l_offset);
}
