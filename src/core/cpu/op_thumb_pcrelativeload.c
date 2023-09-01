#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbPcRelativeLoad(uint16_t p_opcode) {
    const uint16_t l_rd = (p_opcode >> 8) & 0x7;
    const uint16_t l_offset = (p_opcode << 2) & 0x3fc;
    const uint32_t l_pc = g_cpuRegisterR[15] & 0xfffffffc;

    g_cpuRegisterR[l_rd] = busRead32(l_pc + l_offset);
}
