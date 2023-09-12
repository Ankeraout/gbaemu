#include <stdint.h>

#include "common.h"
#include "core/dma.h"
#include "core/gpu.h"
#include "core/io.h"
#include "core/irq.h"
#include "core/keypad.h"
#include "core/timer.h"

#define C_IOADDR_MASK_BIT_0 0x00000001
#define C_IOADDR_MASK_16 0xfffffffe
#define C_IOADDR_MASK_32 0xfffffffc
#define C_UINT16_MASK_LSB 0x000000ff
#define C_UINT16_MASK_MSB 0x0000ff00
#define C_IOADDR_POSTFLG 0x04000300
#define C_IOADDR_HALTCNT 0x04000301

static uint16_t (*s_ioReadFunc[512])(uint32_t p_address);
static void (*s_ioWriteFunc[512])(uint32_t p_address, uint16_t p_value);

static uint16_t ioReadDummy(uint32_t p_address);
static void ioWriteDummy(uint32_t p_address, uint16_t p_value);

void ioInit(void) {
    for(uint32_t l_address = 0x04000000; l_address < 0x04000400; l_address += 2) {
        uint16_t (*l_ioReadFunc)(uint32_t);
        void (*l_ioWriteFunc)(uint32_t, uint16_t);

        if((l_address >= 0x04000000) && (l_address <= 0x0400005f)) {
            l_ioReadFunc = gpuIoRead16;
            l_ioWriteFunc = gpuIoWrite16;
        } else if((l_address >= 0x04000100) && (l_address <= 0x0400010f)) {
            l_ioReadFunc = timerIoRead16;
            l_ioWriteFunc = timerIoWrite16;
        } else if((l_address >= 0x04000130) && (l_address <= 0x04000132)) {
            l_ioReadFunc = keypadIoRead16;
            l_ioWriteFunc = keypadIoWrite16;
        } else if((l_address >= 0x04000200) && (l_address <= 0x04000208)) {
            l_ioReadFunc = irqIoRead16;
            l_ioWriteFunc = irqIoWrite16;
        } else if((l_address >= 0x040000b0) && (l_address <= 0x040000df)) {
            l_ioReadFunc = dmaIoRead16;
            l_ioWriteFunc = dmaIoWrite16;
        } else {
            l_ioReadFunc = ioReadDummy;
            l_ioWriteFunc = ioWriteDummy;
        }

        s_ioReadFunc[(l_address >> 1) & 0x1ff] = l_ioReadFunc;
        s_ioWriteFunc[(l_address >> 1) & 0x1ff] = l_ioWriteFunc;
    }
}

uint8_t ioRead8(uint32_t p_address) {
    uint8_t l_returnValue;

    switch(p_address) {
        case C_IOADDR_POSTFLG: return 0; break; // TODO: Implement POSTFLG
        case C_IOADDR_HALTCNT: return 0; break; // TODO: Implement HALTCNT
        default:
            {
                uint16_t l_value = ioRead16(p_address);

                if((p_address & C_IOADDR_MASK_BIT_0) == 0) {
                    l_returnValue = (uint8_t)l_value;
                } else {
                    l_returnValue = (uint8_t)(l_value >> 8);
                }
            }

            break;
    }

    return l_returnValue;
}

uint16_t ioRead16(uint32_t p_address) {
    return s_ioReadFunc[(p_address >> 1) & 0x1ff](p_address);
}

uint32_t ioRead32(uint32_t p_address) {
    uint32_t l_address = p_address & C_IOADDR_MASK_32;

    switch(l_address) {
        default:
            {
                uint16_t l_lowHalfword = ioRead16(l_address);
                uint16_t l_highHalfword = ioRead16(l_address + 2);

                return l_lowHalfword | (l_highHalfword << 16);
            }
    }
}

void ioWrite8(uint32_t p_address, uint8_t p_value) {
    switch(p_address) {
        case C_IOADDR_POSTFLG: break; // TODO: Implement POSTFLG
        case C_IOADDR_HALTCNT: break; // TODO: Implement HALTCNT
        default:
            {
                uint16_t l_value = ioRead16(p_address);

                if((p_address & C_IOADDR_MASK_BIT_0) == 0) {
                    l_value &= (uint16_t)C_UINT16_MASK_MSB;
                    l_value |= (uint16_t)p_value;
                } else {
                    l_value &= (uint16_t)C_UINT16_MASK_LSB;
                    l_value |= (uint16_t)(p_value << 8);
                }

                ioWrite16(p_address, l_value);
            }

            break;
    }
}

void ioWrite16(uint32_t p_address, uint16_t p_value) {
    s_ioWriteFunc[(p_address >> 1) & 0x1ff](p_address, p_value);
}

void ioWrite32(uint32_t p_address, uint32_t p_value) {
    uint32_t l_address = p_address & C_IOADDR_MASK_32;

    switch(l_address) {
        default:
            {
                uint16_t l_lowHalfword = (uint16_t)p_value;
                uint16_t l_highHalfword = (uint16_t)(p_value >> 16);

                ioWrite16(l_address, l_lowHalfword);
                ioWrite16(l_address + 2, l_highHalfword);
            }

            break;
    }
}

static uint16_t ioReadDummy(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);
    return 0xffff;
}

static void ioWriteDummy(uint32_t p_address, uint16_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}
