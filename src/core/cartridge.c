//==============================================================================
// Included files
//==============================================================================
#include "cartrige.h"
#include "common.h"
#include "emulator.h"

//==============================================================================
// Private constants
//==============================================================================

//==============================================================================
// Private types
//==============================================================================

//==============================================================================
// Private variables
//==============================================================================
uint32_t s_romAddressMask;

//==============================================================================
// Private functions declaration
//==============================================================================

//==============================================================================
// Public functions definition
//==============================================================================
void cartridgeInit(void) {
    s_romAddressMask = (uint32_t)(g_romBufferSize - 1);
}

uint8_t cartridgeRomRead8(uint32_t p_address) {
    uint32_t l_address = p_address & s_romAddressMask;

    return g_romBuffer[l_address];
}

uint16_t cartridgeRomRead16(uint32_t p_address) {
    uint32_t l_address = p_address & s_romAddressMask;

    return (
        g_romBuffer[l_address]
        | ((g_romBuffer[l_address + 1]) << 8)
    );
}

uint32_t cartridgeRomRead32(uint32_t p_address) {
    uint32_t l_address = p_address & s_romAddressMask;

    return (
        g_romBuffer[l_address]
        | ((g_romBuffer[l_address + 1]) << 8)
        | ((g_romBuffer[l_address + 2]) << 16)
        | ((g_romBuffer[l_address + 3]) << 24)
    );
}

uint8_t cartridgeSramRead8(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);

    return 0;
}

uint16_t cartridgeSramRead16(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);

    return 0;
}

uint32_t cartridgeSramRead32(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);

    return 0;
}

void cartridgeSramWrite8(uint32_t p_address, uint8_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

void cartridgeSramWrite16(uint32_t p_address, uint16_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

void cartridgeSramWrite32(uint32_t p_address, uint32_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

//==============================================================================
// Private functions definition
//==============================================================================