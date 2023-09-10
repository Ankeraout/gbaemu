#include <stdint.h>

#include "core/irq.h"

#define C_IOADDRESS_IE 0x04000200
#define C_IOADDRESS_IF 0x04000202
#define C_IOADDRESS_IME 0x04000208

static uint16_t s_irqRegisterInterruptEnable;
static uint16_t s_irqRegisterInterruptFlag;
static bool s_irqRegisterInterruptMasterEnable;

void irqReset(void) {
    s_irqRegisterInterruptEnable = 0;
    s_irqRegisterInterruptFlag = 0;
}

void irqRaise(enum te_irqMask p_irqMask) {
    s_irqRegisterInterruptFlag |= p_irqMask;
}

uint16_t irqIoRead16(uint32_t p_address) {
    switch(p_address) {
        case C_IOADDRESS_IE: return s_irqRegisterInterruptEnable;
        case C_IOADDRESS_IF: return s_irqRegisterInterruptFlag;
        case C_IOADDRESS_IME: return s_irqRegisterInterruptMasterEnable ? 1 : 0;
        default: return 0xffff;
    }
}

void irqIoWrite16(uint32_t p_address, uint16_t p_value) {
    switch(p_address) {
        case C_IOADDRESS_IE: s_irqRegisterInterruptEnable = p_value; break;
        case C_IOADDRESS_IF: s_irqRegisterInterruptFlag &= ~p_value; break;
        case C_IOADDRESS_IME: s_irqRegisterInterruptMasterEnable = (p_value & 1) != 0; break;
        default: break;
    }
}

bool irqGetSignal() {
    return s_irqRegisterInterruptMasterEnable
        && ((s_irqRegisterInterruptEnable & s_irqRegisterInterruptFlag) != 0);
}
