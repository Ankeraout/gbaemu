#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbMultipleLoadStore(uint16_t p_opcode) {
    const bool l_isLoad = (p_opcode & (1 << 11)) != 0;
    const uint16_t l_rb = (p_opcode >> 8) & 0x7;

    uint32_t l_offset = g_cpuRegisterR[l_rb];

    // loop
    for(int l_registerIndex = 0; l_registerIndex < 8; l_registerIndex++) {
        const uint32_t l_mask = 1 << l_registerIndex;

        if((p_opcode & l_mask) != 0) {
            if(l_isLoad) {
                g_cpuRegisterR[l_registerIndex] = busRead32(l_offset);
            } else {
                busWrite32(l_offset, g_cpuRegisterR[l_registerIndex]);
            }

            l_offset += 4;
        }
    }

    cpuWriteRegister(l_rb, l_offset);
}
