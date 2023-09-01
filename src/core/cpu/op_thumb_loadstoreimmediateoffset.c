#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbLoadStoreImmediateOffset(uint16_t p_opcode) {
    const bool l_isByte = (p_opcode & (1 << 12)) != 0;
    const bool l_isLoad = (p_opcode & (1 << 11)) != 0;
    const uint16_t l_offset = ((p_opcode >> 6) & 0x1f) << (l_isByte ? 0 : 2);
    const uint16_t l_rb = (p_opcode >> 3) & 0x7;
    const uint16_t l_rd = p_opcode & 0x7;

    const uint32_t l_rbValue = g_cpuRegisterR[l_rb];
    const uint32_t l_address = l_rbValue + l_offset;

    if(l_isLoad) {
        if(l_isByte) {
            g_cpuRegisterR[l_rd] = busRead8(l_address);
        } else {
            g_cpuRegisterR[l_rd] = busRead32(l_address);
        }
    } else {
        if(l_isByte) {
            busWrite8(l_address, g_cpuRegisterR[l_rd]);
        } else {
            busWrite32(l_address, g_cpuRegisterR[l_rd]);
        }
    }
}
