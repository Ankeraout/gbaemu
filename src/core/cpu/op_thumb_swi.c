#include <stdint.h>

#include "common.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbSwi(uint16_t p_opcode) {
    M_UNUSED_PARAMETER(p_opcode);
    cpuRaiseSwi();
}
