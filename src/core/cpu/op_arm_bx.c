#include <stdint.h>

#include "core/cpu/cpu.h"

void cpuOpcodeArmBx(uint32_t p_opcode) {
    if((p_opcode & 0x0ffffff0) != 0x012fff10) {
        cpuRaiseUnd();
        return;
    }

    const uint32_t l_rn = p_opcode & 0x0000000f;
    const uint32_t l_rnValue = s_cpuRegisterR[l_rn];

    if((l_rn & 0x00000001) != 0) {
        s_cpuFlagT = true;
        cpuJump(l_rnValue & 0xfffffffe);
    } else {
        s_cpuFlagT = false;
        cpuJump(l_rnValue & 0xfffffffc);
    }
}