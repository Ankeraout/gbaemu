#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbPushPop(uint16_t p_opcode) {
    const bool l_isLoad = (p_opcode & (1 << 11)) != 0;
    const bool l_storeLrLoadPc = (p_opcode & (1 << 8)) != 0;
    const int l_registerCount = hammingWeight8(p_opcode) + l_storeLrLoadPc ? 1 : 0;
    const uint32_t l_base = g_cpuRegisterR[13];

    uint32_t l_offset = l_base;

    if(l_isLoad) {
        l_offset -= 4 * l_registerCount;
    }

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

    if(l_storeLrLoadPc) {
        if(l_isLoad) {
            cpuJump(busRead32(l_offset));
        } else {
            busWrite32(l_offset, g_cpuRegisterR[14]);
        }
    }

    cpuWriteRegister(13, l_base + (l_isLoad ? 4 : -4) * l_registerCount);
}
