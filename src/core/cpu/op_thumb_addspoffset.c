#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbAddSpOffset(uint16_t p_opcode) {
    const bool l_isNegative = (p_opcode & (1 << 7)) != 0;
    const uint16_t l_immediate = (p_opcode << 2) & 0x1fc;

    if(l_isNegative) {
        g_cpuRegisterR[13] -= l_immediate;
    } else {
        g_cpuRegisterR[13] += l_immediate;
    }
}
