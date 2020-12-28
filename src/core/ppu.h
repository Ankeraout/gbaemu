#ifndef __CORE_PPU_H__
#define __CORE_PPU_H__

#include <stdint.h>

extern void gba_ppu_reset();
extern uint8_t gba_ppu_palette_read8(uint32_t address);
extern uint16_t gba_ppu_palette_read16(uint32_t address);
extern uint32_t gba_ppu_palette_read32(uint32_t address);
extern void gba_ppu_palette_write8(uint32_t address, uint8_t value);
extern void gba_ppu_palette_write16(uint32_t address, uint16_t value);
extern void gba_ppu_palette_write32(uint32_t address, uint32_t value);
extern uint8_t gba_ppu_vram_read8(uint32_t address);
extern uint16_t gba_ppu_vram_read16(uint32_t address);
extern uint32_t gba_ppu_vram_read32(uint32_t address);
extern void gba_ppu_vram_write8(uint32_t address, uint8_t value);
extern void gba_ppu_vram_write16(uint32_t address, uint16_t value);
extern void gba_ppu_vram_write32(uint32_t address, uint32_t value);
extern uint8_t gba_ppu_oam_read8(uint32_t address);
extern uint16_t gba_ppu_oam_read16(uint32_t address);
extern uint32_t gba_ppu_oam_read32(uint32_t address);
extern void gba_ppu_oam_write8(uint32_t address, uint8_t value);
extern void gba_ppu_oam_write16(uint32_t address, uint16_t value);
extern void gba_ppu_oam_write32(uint32_t address, uint32_t value);

#endif
