#ifndef __CORE_BUS_H__
#define __CORE_BUS_H__

#include <stdint.h>

extern uint8_t gba_bus_read8(uint32_t address);
extern uint16_t gba_bus_read16(uint32_t address);
extern uint32_t gba_bus_read32(uint32_t address);
extern void gba_bus_write8(uint32_t address, uint8_t value);
extern void gba_bus_write16(uint32_t address, uint16_t value);
extern void gba_bus_write32(uint32_t address, uint32_t value);

#endif
