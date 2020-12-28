#ifndef __CORE_TIMER_H__
#define __CORE_TIMER_H__

#include <stdint.h>

extern void gba_timer_reset();
extern void gba_timer_cycle();
extern void gba_timer_writeCallback_channel0_reload(uint32_t address, uint16_t value);
extern void gba_timer_writeCallback_channel0_control(uint32_t address, uint16_t value);
extern void gba_timer_writeCallback_channel1_reload(uint32_t address, uint16_t value);
extern void gba_timer_writeCallback_channel1_control(uint32_t address, uint16_t value);
extern void gba_timer_writeCallback_channel2_reload(uint32_t address, uint16_t value);
extern void gba_timer_writeCallback_channel2_control(uint32_t address, uint16_t value);
extern void gba_timer_writeCallback_channel3_reload(uint32_t address, uint16_t value);
extern void gba_timer_writeCallback_channel3_control(uint32_t address, uint16_t value);

#endif
