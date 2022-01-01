//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

#include "common.h"
#include "io.h"

//==============================================================================
// Public constants definition
//==============================================================================
#define C_IOADDR_MASK_BIT_0 0x00000001
#define C_IOADDR_MASK_16 0xfffffffe
#define C_IOADDR_MASK_32 0xfffffffc
#define C_UINT16_MASK_LSB 0x000000ff
#define C_UINT16_MASK_MSB 0x0000ff00
#define C_UINT32_MASK_MSH 0xffff0000
#define C_IOADDR_POSTFLG 0x04000300
#define C_IOADDR_HALTCNT 0x04000301

//==============================================================================
// Public functions definition
//==============================================================================
uint8_t ioRead8(uint32_t p_address) {
    uint8_t l_returnValue;

    switch(p_address) {
        case C_IOADDR_POSTFLG: return 0; break; // TODO: Implement POSTFLG
        case C_IOADDR_HALTCNT: return 0; break; // TODO: Implement HALTCNT
        default:
            uint16_t l_value = ioRead16(p_address);

            if((p_address & C_IOADDR_MASK_BIT_0) == 0) {
                l_returnValue = (uint8_t)l_value;
            } else {
                l_returnValue = (uint8_t)(l_value >> 8);
            }

            break;
    }

    return l_returnValue;
}

uint16_t ioRead16(uint32_t p_address) {
    uint32_t l_address = p_address & C_IOADDR_MASK_16;

    switch(l_address) {
        default:
            return 0;
    }
}

uint32_t ioRead32(uint32_t p_address) {
    uint32_t l_address = p_address & C_IOADDR_MASK_32;

    switch(l_address) {
        default:
            uint16_t l_lowHalfword = ioRead16(l_address);
            uint16_t l_highHalfword = ioRead16(l_address + 2);

            return l_lowHalfword | (l_highHalfword << 16);
    }
}

void ioWrite8(uint32_t p_address, uint8_t p_value) {
    switch(p_address) {
        case C_IOADDR_POSTFLG: break; // TODO: Implement POSTFLG
        case C_IOADDR_HALTCNT: break; // TODO: Implement HALTCNT
        default:
            uint16_t l_value = ioRead16(p_address);

            if((p_address & C_IOADDR_MASK_BIT_0) == 0) {
                l_value &= (uint16_t)C_UINT16_MASK_MSB;
                l_value |= (uint16_t)p_value;
            } else {
                l_value &= (uint16_t)C_UINT16_MASK_LSB;
                l_value |= (uint16_t)(p_value << 8);
            }

            ioWrite16(p_address, l_value);

            break;
    }
}

void ioWrite16(uint32_t p_address, uint16_t p_value) {
    uint32_t l_address = p_address & C_IOADDR_MASK_16;

    switch(l_address) {
        default:
            break;
    }
}

void ioWrite32(uint32_t p_address, uint32_t p_value) {
    uint32_t l_address = p_address & C_IOADDR_MASK_32;

    switch(l_address) {
        default:
            uint16_t l_lowHalfword = (uint16_t)p_value;
            uint16_t l_highHalfword = (uint16_t)(p_value >> 16);

            ioWrite16(l_address, l_lowHalfword);
            ioWrite16(l_address + 2, l_highHalfword);

            break;
    }
}