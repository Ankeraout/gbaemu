//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

#include "iwram.h"

//==============================================================================
// Private constants
//==============================================================================
/**
 * @brief This constant defines the size of the IWRAM in bytes.
 */
#define C_IWRAM_SIZE_BYTES 262144

//==============================================================================
// Private variables
//==============================================================================
/**
 * @brief This variable contains the data of the IWRAM.
 */
uint8_t s_iwramData[C_IWRAM_SIZE_BYTES];

//==============================================================================
// Public functions definition
//==============================================================================
uint8_t iwramRead8(uint32_t p_address) {
    return s_iwramData[p_address & 0x0003ffff];
}

uint16_t iwramRead16(uint32_t p_address) {
    uint32_t l_address = p_address & 0x0003fffe;

    return (
        s_iwramData[l_address]
        | ((s_iwramData[l_address + 1]) << 8)
    );
}

uint32_t iwramRead32(uint32_t p_address) {
    uint32_t l_address = p_address & 0x0003fffc;

    return (
        s_iwramData[l_address]
        | ((s_iwramData[l_address + 1]) << 8)
        | ((s_iwramData[l_address + 2]) << 16)
        | ((s_iwramData[l_address + 3]) << 24)
    );
}

void iwramWrite8(uint32_t p_address, uint8_t p_value) {
    s_iwramData[p_address & 0x0003ffff] = p_value;
}

void iwramWrite16(uint32_t p_address, uint16_t p_value) {
    uint32_t l_address = p_address & 0x0003fffe;

    s_iwramData[l_address] = (uint8_t)p_value;
    s_iwramData[l_address + 1] = (uint8_t)(p_value >> 8);
}

void iwramWrite32(uint32_t p_address, uint32_t p_value) {
    uint32_t l_address = p_address & 0x0003fffc;

    s_iwramData[l_address] = (uint8_t)p_value;
    s_iwramData[l_address + 1] = (uint8_t)(p_value >> 8);
    s_iwramData[l_address + 2] = (uint8_t)(p_value >> 16);
    s_iwramData[l_address + 3] = (uint8_t)(p_value >> 24);
}
