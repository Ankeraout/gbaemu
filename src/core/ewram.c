#include <stdint.h>
#include <string.h>

#define C_EWRAM_SIZE_BYTES 262144

static uint8_t s_ewramData[C_EWRAM_SIZE_BYTES];

void ewramReset(void) {
    memset(s_ewramData, 0, C_EWRAM_SIZE_BYTES);
}

uint8_t ewramRead8(uint32_t p_address) {
    return s_ewramData[p_address & 0x0003ffff];
}

uint16_t ewramRead16(uint32_t p_address) {
    uint32_t l_address = p_address & 0x0003fffe;

    return (
        s_ewramData[l_address]
        | ((s_ewramData[l_address + 1]) << 8)
    );
}

uint32_t ewramRead32(uint32_t p_address) {
    uint32_t l_address = p_address & 0x0003fffc;

    return (
        s_ewramData[l_address]
        | ((s_ewramData[l_address + 1]) << 8)
        | ((s_ewramData[l_address + 2]) << 16)
        | ((s_ewramData[l_address + 3]) << 24)
    );
}

void ewramWrite8(uint32_t p_address, uint8_t p_value) {
    s_ewramData[p_address & 0x0003ffff] = p_value;
}

void ewramWrite16(uint32_t p_address, uint16_t p_value) {
    uint32_t l_address = p_address & 0x0003fffe;

    s_ewramData[l_address] = (uint8_t)p_value;
    s_ewramData[l_address + 1] = (uint8_t)(p_value >> 8);
}

void ewramWrite32(uint32_t p_address, uint32_t p_value) {
    uint32_t l_address = p_address & 0x0003fffc;

    s_ewramData[l_address] = (uint8_t)p_value;
    s_ewramData[l_address + 1] = (uint8_t)(p_value >> 8);
    s_ewramData[l_address + 2] = (uint8_t)(p_value >> 16);
    s_ewramData[l_address + 3] = (uint8_t)(p_value >> 24);
}
