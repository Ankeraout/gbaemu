#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbLoadAddress(uint16_t p_opcode) {
    const bool l_isSp = (p_opcode & (1 << 11)) != 0;
    const uint16_t l_rd = (p_opcode >> 8) & 0x7;
    const uint16_t l_offset = (p_opcode << 2) & 0x3fc;
    const uint32_t l_base = l_isSp ? g_cpuRegisterR[13] : (g_cpuRegisterR[15] & 0xfffffffc);

    g_cpuRegisterR[l_rd] = l_base + l_offset;
}
