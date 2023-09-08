#include <stdint.h>
#include <string.h>

#define C_IWRAM_SIZE_BYTES 32768

static uint8_t s_iwramData[C_IWRAM_SIZE_BYTES];

void iwramReset(void) {
    memset(s_iwramData, 0, C_IWRAM_SIZE_BYTES);
}

uint8_t iwramRead8(uint32_t p_address) {
    return s_iwramData[p_address & 0x00007fff];
}

uint16_t iwramRead16(uint32_t p_address) {
    uint32_t l_address = p_address & 0x00007ffe;

    return (
        s_iwramData[l_address]
        | ((s_iwramData[l_address + 1]) << 8)
    );
}

uint32_t iwramRead32(uint32_t p_address) {
    uint32_t l_address = p_address & 0x00007ffc;

    return (
        s_iwramData[l_address]
        | ((s_iwramData[l_address + 1]) << 8)
        | ((s_iwramData[l_address + 2]) << 16)
        | ((s_iwramData[l_address + 3]) << 24)
    );
}

void iwramWrite8(uint32_t p_address, uint8_t p_value) {
    s_iwramData[p_address & 0x00007fff] = p_value;
}

void iwramWrite16(uint32_t p_address, uint16_t p_value) {
    uint32_t l_address = p_address & 0x00007ffe;

    s_iwramData[l_address] = (uint8_t)p_value;
    s_iwramData[l_address + 1] = (uint8_t)(p_value >> 8);
}

void iwramWrite32(uint32_t p_address, uint32_t p_value) {
    uint32_t l_address = p_address & 0x00007ffc;

    s_iwramData[l_address] = (uint8_t)p_value;
    s_iwramData[l_address + 1] = (uint8_t)(p_value >> 8);
    s_iwramData[l_address + 2] = (uint8_t)(p_value >> 16);
    s_iwramData[l_address + 3] = (uint8_t)(p_value >> 24);
}