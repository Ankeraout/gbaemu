#ifndef __GBA_CARTRIDGE_H__
#define __GBA_CARTRIDGE_H__

#include <stdint.h>

#include "core/defines.h"

struct gba_cartridge_s {
    uint8_t rom[GBA_MAX_ROM_FILE_SIZE]; /**< The ROM buffer. */
    uint8_t sram[GBA_MAX_SRAM_FILE_SIZE]; /** The SRAM buffer. */
    size_t sramSize; /**< The SRAM size. */
};

/**
 * @brief Initializes the GameBoy Advance cartridge.
 * 
 * @param[in] cartridge The cartridge object to initialize.
 * @param[in] buffer The ROM buffer.
 * @param[in] bufferSize The size of the ROM buffer.
 */
extern void gba_cartridge_init(const void *buffer, size_t bufferSize);

/**
 * @brief Reads a 8-bit value from the ROM.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 8-bit value read from the ROM.
 */
uint8_t gba_rom_read8(uint32_t address);

/**
 * @brief Reads a 16-bit value from the ROM.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 16-bit value read from the ROM.
 */
uint16_t gba_rom_read16(uint32_t address);

/**
 * @brief Reads a 32-bit value from the ROM.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 32-bit value read from the ROM.
 */
uint32_t gba_rom_read32(uint32_t address);

/**
 * @brief Writes a 8-bit value from the ROM.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_rom_write8(uint32_t address, uint8_t value);

/**
 * @brief Writes a 16-bit value from the ROM.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_rom_write16(uint32_t address, uint16_t value);

/**
 * @brief Writes a 32-bit value from the ROM.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_rom_write32(uint32_t address, uint32_t value);

/**
 * @brief Reads a 8-bit value from the SRAM.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 8-bit value read from the SRAM.
 */
uint8_t gba_sram_read8(uint32_t address);

/**
 * @brief Reads a 16-bit value from the SRAM.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 16-bit value read from the SRAM.
 */
uint16_t gba_sram_read16(uint32_t address);

/**
 * @brief Reads a 32-bit value from the SRAM.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 32-bit value read from the SRAM.
 */
uint32_t gba_sram_read32(uint32_t address);

/**
 * @brief Writes a 8-bit value from the SRAM.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_sram_write8(uint32_t address, uint8_t value);

/**
 * @brief Writes a 16-bit value from the SRAM.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_sram_write16(uint32_t address, uint16_t value);

/**
 * @brief Writes a 32-bit value from the SRAM.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_sram_write32(uint32_t address, uint32_t value);

#endif
