#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeArmHalfwordSignedDataTransfer(uint32_t p_opcode) {
    const bool l_isPreIndexed = (p_opcode & (1 << 24)) != 0;
    const bool l_isUp = (p_opcode & (1 << 23)) != 0;
    const bool l_isImmediate = (p_opcode & (1 << 22)) != 0;
    const bool l_writeBack = (p_opcode & (1 << 21)) != 0;
    const bool l_isLoad = (p_opcode & (1 << 20)) != 0;
    const bool l_isSigned = (p_opcode & (1 << 6)) != 0;
    const bool l_isHalfword = (p_opcode & (1 << 5)) != 0;
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;

    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];

    uint32_t l_offset;

    if(l_isImmediate) {
        l_offset = ((p_opcode & 0x00000f00) >> 4) | (p_opcode & 0x0000000f);
    } else {
        const uint32_t l_rm = p_opcode & 0xf;
        l_offset = g_cpuRegisterR[l_rm];
    }

    uint32_t l_address = l_rnValue;

    if(l_isPreIndexed) {
        if(l_isUp) {
            l_address += l_offset;
        } else {
            l_address -= l_offset;
        }
    }

    if(l_isLoad) {
        uint32_t l_loadedValue;

        if(l_isHalfword) {
            const bool l_isMisaligned = (l_address & (1 << 0)) != 0;
            l_loadedValue = busRead16(l_address);

            if(l_isSigned) {
                if(l_isMisaligned) {
                    l_loadedValue = signExtend8to32(l_loadedValue >> 8);
                } else {
                    l_loadedValue = signExtend16to32(l_loadedValue);
                }
            } else if(l_isMisaligned) {
                l_loadedValue = rotateRight(l_loadedValue, 8);
            }
        } else {
            l_loadedValue = busRead8(l_address);

            if(l_isSigned) {
                l_loadedValue = signExtend8to32(l_loadedValue);
            }
        }

        cpuWriteRegister(l_rd, l_loadedValue);
    } else {
        const uint32_t l_rdValue = g_cpuRegisterR[l_rd];

        if(l_isHalfword) {
            busWrite16(l_address, l_rdValue);
        } else {
            busWrite8(l_address, l_rdValue);
        }
    }

    if((!l_isPreIndexed || l_writeBack) && !(l_isLoad && (l_rd == l_rn))) {
        if(!l_isPreIndexed) {
            if(l_isUp) {
                l_address += l_offset;
            } else {
                l_address -= l_offset;
            }
        }

        cpuWriteRegister(l_rn, l_address);
    }
}