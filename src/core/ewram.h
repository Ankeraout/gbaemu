#ifndef __CORE_EWRAM_H__
#define __CORE_EWRAM_H__

#include <stdint.h>

extern void gba_ewram_reset();
extern uint8_t gba_ewram_read8(uint32_t address);
extern uint16_t gba_ewram_read16(uint32_t address);
extern uint32_t gba_ewram_read32(uint32_t address);
extern void gba_ewram_write8(uint32_t address, uint8_t value);
extern void gba_ewram_write16(uint32_t address, uint16_t value);
extern void gba_ewram_write32(uint32_t address, uint32_t value);

#endif
