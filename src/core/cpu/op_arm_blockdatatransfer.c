#include <stdbool.h>
#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeArmLdmStm(uint32_t p_opcode) {
    const bool l_isPreIndexing = (p_opcode & (1 << 24)) != 0;
    const bool l_isUp = (p_opcode & (1 << 23)) != 0;
    const bool l_loadPsrOrForceUserMode = (p_opcode & (1 << 22)) != 0;
    const bool l_writeBack = (p_opcode & (1 << 21)) != 0;
    const bool l_isLoad = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;

    const int l_registerCount = hammingWeight16(p_opcode);
    const uint32_t l_base = g_cpuRegisterR[l_rn];

    uint32_t l_offset = l_base;

    if(l_isUp == l_isPreIndexing) {
        l_offset += 4;
    }

    if(!l_isUp) {
        l_offset -= 4 * l_registerCount;
    }

    // loop
    for(int l_registerIndex = 0; l_registerIndex < 16; l_registerIndex++) {
        const uint32_t l_mask = (1 << l_registerIndex);

        if((p_opcode & l_mask) != 0) {
            if(l_isLoad) {
                cpuWriteRegister(l_registerIndex, busRead32(l_offset));
            } else {
                if(l_registerIndex == 15) {
                    busWrite32(l_offset, g_cpuRegisterR[15] + 4);
                } else {
                    busWrite32(l_offset, g_cpuRegisterR[l_registerIndex]);
                }
            }

            l_offset += 4;
        }
    }

    if(l_writeBack) {
        cpuWriteRegister(l_rn, l_base + (l_isUp ? 4 : -4) * l_registerCount);
    }
}