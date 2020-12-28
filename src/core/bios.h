#ifndef __CORE_BIOS_H__
#define __CORE_BIOS_H__

#include <stdint.h>

void gba_bios_init(const void *buffer);
extern uint8_t gba_bios_read8(uint32_t address);
extern uint16_t gba_bios_read16(uint32_t address);
extern uint32_t gba_bios_read32(uint32_t address);

#endif
