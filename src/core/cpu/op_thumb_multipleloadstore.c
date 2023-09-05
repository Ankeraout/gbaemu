#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

static inline uint16_t getFirstRegisterInList(uint16_t p_opcode);

void cpuOpcodeThumbMultipleLoadStore(uint16_t p_opcode) {
    const bool l_isLoad = (p_opcode & (1 << 11)) != 0;
    const uint16_t l_rb = (p_opcode >> 8) & 0x7;
    const bool l_isRlistEmpty = (p_opcode & 0xff) == 0;
    const uint16_t l_firstRegisterInList = getFirstRegisterInList(p_opcode);
    const bool l_rbIsFirstRegisterInList = l_rb == l_firstRegisterInList;
    const uint32_t l_base = g_cpuRegisterR[l_rb];
    const uint32_t l_registerCount = hammingWeight8(p_opcode);

    if(l_isRlistEmpty) {
        if(l_isLoad) {
            cpuJump(busRead32(l_base));
        } else {
            busWrite32(l_base, g_cpuRegisterR[15] + 2);
        }

        cpuWriteRegister(l_rb, l_base + 0x40);
    } else {
        uint32_t l_offset = g_cpuRegisterR[l_rb];

        // loop
        for(int l_registerIndex = 0; l_registerIndex < 8; l_registerIndex++) {
            const uint32_t l_mask = 1 << l_registerIndex;

            if((p_opcode & l_mask) != 0) {
                if(l_isLoad) {
                    g_cpuRegisterR[l_registerIndex] = busRead32(l_offset);
                } else {
                    uint32_t l_value;

                    if(
                        (l_registerIndex == l_rb)
                        && !l_rbIsFirstRegisterInList
                    ) {
                        l_value = l_base + 4 * l_registerCount;
                    } else {
                        l_value = g_cpuRegisterR[l_registerIndex];
                    }

                    busWrite32(l_offset, l_value);
                }

                l_offset += 4;
            }
        }

        cpuWriteRegister(l_rb, l_offset);
    }
}

static inline uint16_t getFirstRegisterInList(uint16_t p_opcode) {
    uint16_t l_registerIndex = 0;
    uint16_t l_registerMask = 1;

    while(l_registerIndex < 8) {
        if((p_opcode & l_registerMask) != 0) {
            break;
        }

        l_registerIndex++;
        l_registerMask <<= 1;
    }

    return l_registerIndex;
}
