//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

#include "bios.h"
#include "emulator.h"

//==============================================================================
// Public functions definition
//==============================================================================
uint8_t biosRead8(uint32_t p_address) {
    return g_biosBuffer[p_address & 0x00003fff];
}

uint16_t biosRead16(uint32_t p_address) {
    uint32_t l_address = p_address & 0x00003ffe;

    return (
        g_biosBuffer[l_address]
        | ((g_biosBuffer[l_address + 1]) << 8)
    );
}

uint32_t biosRead32(uint32_t p_address) {
    uint32_t l_address = p_address & 0x00003ffe;

    return (
        g_biosBuffer[l_address]
        | ((g_biosBuffer[l_address + 1]) << 8)
        | ((g_biosBuffer[l_address + 2]) << 16)
        | ((g_biosBuffer[l_address + 3]) << 24)
    );
}