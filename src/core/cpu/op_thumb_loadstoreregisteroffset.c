#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbLoadStoreRegisterOffset(uint16_t p_opcode) {
    const bool l_isLoad = (p_opcode & (1 << 11)) != 0;
    const bool l_isByte = (p_opcode & (1 << 10)) != 0;
    const uint16_t l_ro = (p_opcode >> 6) & 0x7;
    const uint16_t l_rb = (p_opcode >> 3) & 0x7;
    const uint16_t l_rd = p_opcode & 0x7;

    const uint32_t l_roValue = g_cpuRegisterR[l_ro];
    const uint32_t l_rbValue = g_cpuRegisterR[l_rb];

    const uint32_t l_address = l_roValue + l_rbValue;

    if(l_isLoad) {
        uint32_t l_result;

        if(l_isByte) {
            l_result = busRead8(l_address);
        } else {
            const uint32_t l_rotation = (l_address & 0x3) << 3;
            l_result = rotateRight(busRead32(l_address), l_rotation);
        }
        
        g_cpuRegisterR[l_rd] = l_result;
    } else {
        if(l_isByte) {
            busWrite8(l_address, g_cpuRegisterR[l_rd]);
        } else {
            busWrite32(l_address, g_cpuRegisterR[l_rd]);
        }
    }
}