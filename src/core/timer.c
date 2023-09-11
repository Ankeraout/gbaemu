#include <stdint.h>

#include "core/irq.h"
#include "core/timer.h"

struct ts_timerRegisterControl {
    uint16_t preScaler;
    bool isCountingUp;
    bool isIrqEnabled;
    bool isEnabled;
};

static uint16_t s_timerRegisterPrescaler;
static uint16_t s_timerRegisterReload[4];
static uint16_t s_timerRegisterCounter[4];
static struct ts_timerRegisterControl s_timerRegisterControl[4];

static uint16_t readCounter(int p_index);
static uint16_t readControl(int p_index);
static void writeCounter(int p_index, uint16_t p_value);
static void writeControl(int p_index, uint16_t p_value);

void timerReset(void) {
    s_timerRegisterPrescaler = 0;

    for(int l_index = 0; l_index < 4; l_index++) {
        s_timerRegisterReload[l_index] = 0;
        s_timerRegisterCounter[l_index] = 0;
        s_timerRegisterControl[l_index].preScaler = 1;
        s_timerRegisterControl[l_index].isCountingUp = false;
        s_timerRegisterControl[l_index].isIrqEnabled = false;
        s_timerRegisterControl[l_index].isEnabled = false;
    }
}

void timerCycle(void) {
    bool l_lastTimerOverflowed = false;

    s_timerRegisterPrescaler++;

    for(int l_index = 0; l_index < 4; l_index++) {
        if(s_timerRegisterControl[l_index].isEnabled) {
            const bool l_count = (
                (s_timerRegisterControl[l_index].isCountingUp && l_lastTimerOverflowed)
                || (
                    !s_timerRegisterControl[l_index].isCountingUp
                    && ((s_timerRegisterPrescaler % s_timerRegisterControl[l_index].preScaler) == 0)
                )
            );

            if(l_count) {
                s_timerRegisterCounter[l_index]++;
                l_lastTimerOverflowed = s_timerRegisterCounter[l_index] == 0;

                if(l_lastTimerOverflowed) {
                    s_timerRegisterCounter[l_index] = s_timerRegisterReload[l_index];

                    if(s_timerRegisterControl[l_index].isIrqEnabled) {
                        irqRaise(E_IRQMASK_TIMER0 << l_index);
                    }
                }
            }
        } else {
            l_lastTimerOverflowed = false;
        }
    }
}

uint16_t timerIoRead16(uint32_t p_address) {
    switch(p_address) {
        case C_IOADDRESS_TM0CNT_L: return readCounter(0);
        case C_IOADDRESS_TM0CNT_H: return readControl(0);
        case C_IOADDRESS_TM1CNT_L: return readCounter(1);
        case C_IOADDRESS_TM1CNT_H: return readControl(1);
        case C_IOADDRESS_TM2CNT_L: return readCounter(2);
        case C_IOADDRESS_TM2CNT_H: return readControl(2);
        case C_IOADDRESS_TM3CNT_L: return readCounter(3);
        case C_IOADDRESS_TM3CNT_H: return readControl(3);
        default: return 0xffff;
    }
}

void timerIoWrite16(uint32_t p_address, uint16_t p_value) {
    switch(p_address) {
        case C_IOADDRESS_TM0CNT_L: writeCounter(0, p_value); break;
        case C_IOADDRESS_TM0CNT_H: writeControl(0, p_value); break;
        case C_IOADDRESS_TM1CNT_L: writeCounter(1, p_value); break;
        case C_IOADDRESS_TM1CNT_H: writeControl(1, p_value); break;
        case C_IOADDRESS_TM2CNT_L: writeCounter(2, p_value); break;
        case C_IOADDRESS_TM2CNT_H: writeControl(2, p_value); break;
        case C_IOADDRESS_TM3CNT_L: writeCounter(3, p_value); break;
        case C_IOADDRESS_TM3CNT_H: writeControl(3, p_value); break;
        default: break;
    }
}

static uint16_t readCounter(int p_index) {
    return s_timerRegisterCounter[p_index];
}

static uint16_t readControl(int p_index) {
    uint16_t l_value = 0;

    switch(s_timerRegisterControl[p_index].preScaler) {
        case 1: break;
        case 64: l_value = 1; break;
        case 256: l_value = 2; break;
        case 1024: l_value = 3; break;
    }

    if(s_timerRegisterControl[p_index].isCountingUp) {
        l_value |= 1 << 2;
    }

    if(s_timerRegisterControl[p_index].isIrqEnabled) {
        l_value |= 1 << 6;
    }

    if(s_timerRegisterControl[p_index].isEnabled) {
        l_value |= 1 << 7;
    }

    return l_value;
}

static void writeCounter(int p_index, uint16_t p_value) {
    s_timerRegisterReload[p_index] = p_value;
}

static void writeControl(int p_index, uint16_t p_value) {
    uint16_t l_preScaler;

    switch(p_value & 0x3) {
        case 0: l_preScaler = 1; break;
        case 1: l_preScaler = 64; break;
        case 2: l_preScaler = 256; break;
        case 3: l_preScaler = 1024; break;
    }

    const bool l_isCountingUp = (p_value & (1 << 2)) != 0;
    const bool l_isIrqEnabled = (p_value & (1 << 6)) != 0;
    const bool l_isEnabled = (p_value & (1 << 7)) != 0;

    if(!s_timerRegisterControl[p_index].isEnabled && l_isEnabled) {
        s_timerRegisterCounter[p_index] = s_timerRegisterReload[p_index];
    }

    s_timerRegisterControl[p_index].preScaler = l_preScaler;
    s_timerRegisterControl[p_index].isCountingUp = l_isCountingUp;
    s_timerRegisterControl[p_index].isIrqEnabled = l_isIrqEnabled;
    s_timerRegisterControl[p_index].isEnabled = l_isEnabled;
}
