#include <stddef.h>
#include <stdint.h>

#include "common.h"

static const uint8_t *s_romBuffer = NULL;
static size_t s_romBufferSize = 0;
static uint32_t s_romAddressMask8;
static uint32_t s_romAddressMask16;
static uint32_t s_romAddressMask32;

int coreSetRom(const void *p_buffer, size_t p_size) {
    s_romBuffer = p_buffer;
    s_romBufferSize = p_size;

    s_romAddressMask8 = (uint32_t)(s_romBufferSize - 1);
    s_romAddressMask16 = s_romAddressMask8 & 0xfffffffe;
    s_romAddressMask32 = s_romAddressMask8 & 0xfffffffc;

    return 0;
}

uint8_t cartridgeRomRead8(uint32_t p_address) {
    uint32_t l_address = p_address & s_romAddressMask8;

    return s_romBuffer[l_address];
}

uint16_t cartridgeRomRead16(uint32_t p_address) {
    uint32_t l_address = p_address & s_romAddressMask16;

    return (
        s_romBuffer[l_address]
        | ((s_romBuffer[l_address + 1]) << 8)
    );
}

uint32_t cartridgeRomRead32(uint32_t p_address) {
    uint32_t l_address = p_address & s_romAddressMask32;

    return (
        s_romBuffer[l_address]
        | ((s_romBuffer[l_address + 1]) << 8)
        | ((s_romBuffer[l_address + 2]) << 16)
        | ((s_romBuffer[l_address + 3]) << 24)
    );
}

uint8_t cartridgeRamRead8(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);

    return 0;
}

uint16_t cartridgeRamRead16(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);

    return 0;
}

uint32_t cartridgeRamRead32(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);

    return 0;
}

void cartridgeRamWrite8(uint32_t p_address, uint8_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

void cartridgeRamWrite16(uint32_t p_address, uint16_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

void cartridgeRamWrite32(uint32_t p_address, uint32_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}
