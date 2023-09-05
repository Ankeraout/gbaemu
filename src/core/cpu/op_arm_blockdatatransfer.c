#include <stdbool.h>
#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

static inline uint32_t getFirstRegisterInList(uint32_t p_opcode);

void cpuOpcodeArmLdmStm(uint32_t p_opcode) {
    const bool l_isPreIndexing = (p_opcode & (1 << 24)) != 0;
    const bool l_isUp = (p_opcode & (1 << 23)) != 0;
    const bool l_writeBack = (p_opcode & (1 << 21)) != 0;
    const bool l_isLoad = (p_opcode & (1 << 20)) != 0;
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const bool l_isRlistEmpty = (p_opcode & 0xffff) == 0;
    const uint32_t l_firstRegisterInList = getFirstRegisterInList(p_opcode);
    const bool l_rnIsFirstRegisterInList = l_rn == l_firstRegisterInList;

    const uint32_t l_base = g_cpuRegisterR[l_rn];

    if(l_isRlistEmpty) {
        uint32_t l_offset;

        if(l_isPreIndexing) {
            if(l_isUp) {
                l_offset = 4;
            } else {
                l_offset = -0x40;
            }
        } else {
            if(l_isUp) {
                l_offset = 0;
            } else {
                l_offset = -0x3c;
            }
        }

        const uint32_t l_address = l_base + l_offset;

        if(l_isLoad) {
            cpuJump(busRead32(l_address));
        } else {
            busWrite32(l_address, g_cpuRegisterR[15] + 4);
        }

        if(l_writeBack) {
            if(l_isUp) {
                cpuWriteRegister(l_rn, l_base + 0x40);
            } else {
                cpuWriteRegister(l_rn, l_base - 0x40);
            }
        }
    } else {
        const bool l_loadPsrOrForceUserMode = (p_opcode & (1 << 22)) != 0;
        const bool l_isPcInList = (p_opcode & (1 << 15)) != 0;

        const int l_registerCount = hammingWeight16(p_opcode);

        uint32_t l_offset = l_base;

        if(l_isUp == l_isPreIndexing) {
            l_offset += 4;
        }

        if(!l_isUp) {
            l_offset -= 4 * l_registerCount;
        }

        uint32_t l_registerIndex = 0;
        uint32_t l_registerMask = 1;

        while(l_registerIndex < 16) {
            if((p_opcode & l_registerMask) != 0) {
                if(l_isLoad) {
                    if(l_loadPsrOrForceUserMode) {
                        if(l_isPcInList) {
                            if(l_registerIndex == 15) {
                                cpuSetCpsr(cpuGetSpsr());
                            }
                        } else {
                            cpuWriteRegisterUsr(l_registerIndex, busRead32(l_offset));
                        }
                    } else {
                        cpuWriteRegister(l_registerIndex, busRead32(l_offset));
                    }
                } else {
                    if(l_loadPsrOrForceUserMode) {
                        if(l_registerIndex == 15) {
                            busWrite32(l_offset, g_cpuRegisterR[15] + 4);
                        } else {
                            busWrite32(l_offset, cpuReadRegisterUsr(l_registerIndex));
                        }
                    } else {
                        uint32_t l_value;

                        if(l_registerIndex == 15) {
                            l_value = g_cpuRegisterR[15] + 4;
                        } else if(
                            (l_registerIndex == l_rn)
                            && !l_rnIsFirstRegisterInList
                            && l_writeBack
                        ) {
                            l_value = l_base + (l_isUp ? 4 : -4) * l_registerCount;
                        } else {
                            l_value = g_cpuRegisterR[l_registerIndex];
                        }

                        busWrite32(l_offset, l_value);
                    }
                }

                l_offset += 4;
            }

            l_registerIndex++;
            l_registerMask <<= 1;
        }

        if(l_writeBack && !(l_isLoad && ((p_opcode & (1 << l_rn)) != 0))) {
            cpuWriteRegister(l_rn, l_base + (l_isUp ? 4 : -4) * l_registerCount);
        }
    }
}

static inline uint32_t getFirstRegisterInList(uint32_t p_opcode) {
    uint32_t l_registerIndex = 0;
    uint32_t l_registerMask = 1;

    while(l_registerIndex < 16) {
        if((p_opcode & l_registerMask) != 0) {
            break;
        }

        l_registerIndex++;
        l_registerMask <<= 1;
    }

    return l_registerIndex;
}
