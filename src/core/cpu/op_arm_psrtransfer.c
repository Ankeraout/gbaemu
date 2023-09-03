#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"
#include "core/cpu/op_arm_psrtransfer.h"

void cpuOpcodeArmMrs(uint32_t p_opcode) {
    const bool l_isSpsr = (p_opcode & (1 << 22)) != 0;
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_value = l_isSpsr ? cpuGetSpsr() : cpuGetCpsr();

    cpuWriteRegister(l_rd, l_value);
}

void cpuOpcodeArmMsr(uint32_t p_opcode) {
    const bool l_isSpsr = (p_opcode & (1 << 22)) != 0;
    const bool l_isImmediate = (p_opcode & (1 << 25)) != 0;

    uint32_t l_value;

    if(l_isImmediate) {
        const uint32_t l_rotation = (p_opcode >> 7) & 0x1e;
        const uint32_t l_immediate = p_opcode & 0xff;
        l_value = rotateRight(l_immediate, l_rotation);
    } else {
        const uint32_t l_rm = p_opcode & 0xf;
        l_value = g_cpuRegisterR[l_rm];
    }

    if(l_isSpsr) {
        cpuSetSpsr(l_value);
    } else {
        cpuSetCpsr(l_value);
    }
}
