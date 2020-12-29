#ifndef __CORE_GBA_H__
#define __CORE_GBA_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern void gba_frameAdvance();
extern size_t gba_getSramSize();
extern void gba_init(bool skipBoot);
extern void gba_reset();
extern void gba_setBios(const void *buffer);
extern void gba_setRom(const void *buffer, size_t size);
extern void gba_setSram(void *buffer, size_t size);
extern void gba_setInterruptFlag(uint16_t flag);
extern void gba_writeToIF(uint32_t address, uint16_t flag);
extern void gba_onFrame();

#endif
