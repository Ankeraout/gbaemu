#ifndef __CORE_IWRAM_H__
#define __CORE_IWRAM_H__

#include <stdint.h>

extern void gba_iwram_reset();
extern uint8_t gba_iwram_read8(uint32_t address);
extern uint16_t gba_iwram_read16(uint32_t address);
extern uint32_t gba_iwram_read32(uint32_t address);
extern void gba_iwram_write8(uint32_t address, uint8_t value);
extern void gba_iwram_write16(uint32_t address, uint16_t value);
extern void gba_iwram_write32(uint32_t address, uint32_t value);

#endif
