#ifndef __INCLUDE_CORE_TIMER_H__
#define __INCLUDE_CORE_TIMER_H__

#include <stdint.h>

#define C_IOADDRESS_TM0CNT_L 0x04000100
#define C_IOADDRESS_TM0CNT_H 0x04000102
#define C_IOADDRESS_TM1CNT_L 0x04000104
#define C_IOADDRESS_TM1CNT_H 0x04000106
#define C_IOADDRESS_TM2CNT_L 0x04000108
#define C_IOADDRESS_TM2CNT_H 0x0400010a
#define C_IOADDRESS_TM3CNT_L 0x0400010c
#define C_IOADDRESS_TM3CNT_H 0x0400010e

void timerReset(void);
void timerCycle(void);
uint16_t timerIoRead16(uint32_t p_address);
void timerIoWrite16(uint32_t p_address, uint16_t p_value);

#endif
