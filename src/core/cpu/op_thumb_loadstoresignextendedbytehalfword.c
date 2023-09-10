#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbLoadStoreSignExtendedByteHalfword(uint16_t p_opcode) {
    const bool l_isHalfword = (p_opcode & (1 << 11)) != 0;
    const bool l_signExtend = (p_opcode & (1 << 10)) != 0;
    const uint16_t l_ro = (p_opcode >> 6) & 0x7;
    const uint16_t l_rb = (p_opcode >> 3) & 0x7;
    const uint16_t l_rd = p_opcode & 0x7;

    const uint32_t l_roValue = g_cpuRegisterR[l_ro];
    const uint32_t l_rbValue = g_cpuRegisterR[l_rb];

    const uint32_t l_address = l_roValue + l_rbValue;

    if(l_isHalfword) {
        const bool l_isMisaligned = (l_address & (1 << 0)) != 0;
        uint32_t l_loadedValue = busRead16(l_address);

        if(l_signExtend) {
            if(l_isMisaligned) {
                l_loadedValue = signExtend8to32(l_loadedValue >> 8);
            } else {
                l_loadedValue = signExtend16to32(l_loadedValue);
            }
        } else if(l_isMisaligned) {
            l_loadedValue = rotateRight(l_loadedValue, 8);
        }

        g_cpuRegisterR[l_rd] = l_loadedValue;
    } else {
        if(l_signExtend) {
            g_cpuRegisterR[l_rd] = signExtend8to32(busRead8(l_address));
        } else {
            busWrite16(l_address, g_cpuRegisterR[l_rd]);
        }
    }
}
