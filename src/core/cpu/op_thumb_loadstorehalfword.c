#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbLoadStoreHalfword(uint16_t p_opcode) {
    const bool l_isLoad = (p_opcode & (1 << 11)) != 0;

    const uint16_t l_offset = (p_opcode >> 5) & 0x3e;
    const uint16_t l_rb = (p_opcode >> 3) & 0x7;
    const uint16_t l_rd = p_opcode & 0x7;

    const uint32_t l_rbValue = g_cpuRegisterR[l_rb];
    const uint32_t l_address = l_rbValue + l_offset;

    if(l_isLoad) {
        const uint32_t l_rotation = (l_address & 1) == 0 ? 0 : 8;
        g_cpuRegisterR[l_rd] = rotateRight(busRead16(l_address), l_rotation);
    } else {
        busWrite16(l_address, g_cpuRegisterR[l_rd]);
    }
}
