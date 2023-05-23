#include <stddef.h>
#include <stdint.h>

#include "core/gba.h"

static const uint8_t *s_biosBuffer = NULL;

int coreSetBios(const void *p_buffer, size_t p_size) {
    if(p_size != C_BIOS_FILE_SIZE_BYTES) {
        return 1;
    }

    s_biosBuffer = p_buffer;

    return 0;
}

uint8_t biosRead8(uint32_t p_address) {
    return s_biosBuffer[p_address & 0x00003fff];
}

uint16_t biosRead16(uint32_t p_address) {
    uint32_t l_address = p_address & 0x00003ffe;

    return (
        s_biosBuffer[l_address]
        | ((s_biosBuffer[l_address + 1]) << 8)
    );
}

uint32_t biosRead32(uint32_t p_address) {
    uint32_t l_address = p_address & 0x00003ffe;

    return (
        s_biosBuffer[l_address]
        | ((s_biosBuffer[l_address + 1]) << 8)
        | ((s_biosBuffer[l_address + 2]) << 16)
        | ((s_biosBuffer[l_address + 3]) << 24)
    );
}
