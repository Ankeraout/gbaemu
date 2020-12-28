#ifndef __CORE_CARTRIDGE_H__
#define __CORE_CARTRIDGE_H__

#include <stddef.h>
#include <stdint.h>

extern void gba_cartridge_init(const void *buffer, size_t size);
extern uint8_t gba_cartridge_rom_read8(uint32_t address);
extern uint16_t gba_cartridge_rom_read16(uint32_t address);
extern uint32_t gba_cartridge_rom_read32(uint32_t address);
extern uint8_t gba_cartridge_sram_read8(uint32_t address);
extern uint16_t gba_cartridge_sram_read16(uint32_t address);
extern uint32_t gba_cartridge_sram_read32(uint32_t address);
extern void gba_cartridge_sram_write8(uint32_t address, uint8_t value);
extern void gba_cartridge_sram_write16(uint32_t address, uint16_t value);
extern void gba_cartridge_sram_write32(uint32_t address, uint32_t value);

#endif
