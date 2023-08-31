#include <stdint.h>

#include "common.h"
#include "core/cpu/cpu.h"

void cpuOpcodeArmSwi(uint32_t p_opcode) {
    M_UNUSED_PARAMETER(p_opcode);
    cpuRaiseSwi();
}
