//==============================================================================
// Included files
//==============================================================================
#include "cartrige.h"
#include "common.h"
#include "emulator.h"

//==============================================================================
// Private variables
//==============================================================================
/**
 * @brief This variable contains the ROM address mask for byte accesses.
 */
uint32_t s_romAddressMask8;

/**
 * @brief This variable contains the ROM address mask for halfword accesses.
 */
uint32_t s_romAddressMask16;

/**
 * @brief This variable contains the ROM address mask for word accesses.
 */
uint32_t s_romAddressMask32;

//==============================================================================
// Public functions definition
//==============================================================================
void cartridgeInit(void) {
    s_romAddressMask8 = (uint32_t)(g_romBufferSize - 1);
    s_romAddressMask16 = s_romAddressMask8 & 0xfffffffe;
    s_romAddressMask32 = s_romAddressMask8 & 0xfffffffc;
}

uint8_t cartridgeRomRead8(uint32_t p_address) {
    uint32_t l_address = p_address & s_romAddressMask8;

    return g_romBuffer[l_address];
}

uint16_t cartridgeRomRead16(uint32_t p_address) {
    uint32_t l_address = p_address & s_romAddressMask16;

    return (
        g_romBuffer[l_address]
        | ((g_romBuffer[l_address + 1]) << 8)
    );
}

uint32_t cartridgeRomRead32(uint32_t p_address) {
    uint32_t l_address = p_address & s_romAddressMask32;

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
