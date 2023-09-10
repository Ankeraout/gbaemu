#ifndef __INCLUDE_CORE_IRQ_H__
#define __INCLUDE_CORE_IRQ_H__

#include <stdbool.h>
#include <stdint.h>

enum te_irqMask {
    E_IRQMASK_VBLANK = (1 << 0),
    E_IRQMASK_HBLANK = (1 << 1),
    E_IRQMASK_VCOUNT = (1 << 2),
    E_IRQMASK_TIMER0 = (1 << 3),
    E_IRQMASK_TIMER1 = (1 << 4),
    E_IRQMASK_TIMER2 = (1 << 5),
    E_IRQMASK_TIMER3 = (1 << 6),
    E_IRQMASK_SERIAL = (1 << 7),
    E_IRQMASK_DMA0 = (1 << 8),
    E_IRQMASK_DMA1 = (1 << 9),
    E_IRQMASK_DMA2 = (1 << 10),
    E_IRQMASK_DMA3 = (1 << 11),
    E_IRQMASK_KEYPAD = (1 << 12),
    E_IRQMASK_GAMEPAK = (1 << 13)
};

void irqReset(void);
void irqRaise(enum te_irqMask p_irqMask);
uint16_t irqIoRead16(uint32_t p_address);
void irqIoWrite16(uint32_t p_address, uint16_t p_value);
bool irqGetSignal();

#endif
