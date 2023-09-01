#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbSpRelativeLoadStore(uint16_t p_opcode) {
    const bool l_isLoad = (p_opcode & (1 << 11)) != 0;
    const uint16_t l_rd = (p_opcode >> 8) & 0x7;
    const uint16_t l_offset = (p_opcode << 2) & 0x3fc;
    const uint32_t l_address = g_cpuRegisterR[13] + l_offset;

    if(l_isLoad) {
        g_cpuRegisterR[l_rd] = busRead32(l_address);
    } else {
        busWrite32(l_address, g_cpuRegisterR[l_rd]);
    }
}
