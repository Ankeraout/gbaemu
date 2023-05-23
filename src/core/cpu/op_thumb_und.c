#include <stdint.h>

#include "common.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbUnd(uint16_t p_opcode) {
    M_UNUSED_PARAMETER(p_opcode);
    cpuRaiseUnd();
}
