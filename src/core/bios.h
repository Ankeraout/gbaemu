#ifndef __CORE_BIOS_H__
#define __CORE_BIOS_H__

#include <stdint.h>

#include "core/defines.h"

/**
 * @brief Initializes the GameBoy Advance BIOS.
 * 
 * @param[in] buffer The buffer that contains the GBA BIOS.
 */
extern void gba_bios_init(const void *buffer);

/**
 * @brief Reads a 8-bit value from the BIOS.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 8-bit value read from the BIOS.
 */
uint8_t gba_bios_read8(uint32_t address);

/**
 * @brief Reads a 16-bit value from the BIOS.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 16-bit value read from the BIOS.
 */
uint16_t gba_bios_read16(uint32_t address);

/**
 * @brief Reads a 32-bit value from the BIOS.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 32-bit value read from the BIOS.
 */
uint32_t gba_bios_read32(uint32_t address);

/**
 * @brief Writes a 8-bit value from the BIOS.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_bios_write8(uint32_t address, uint8_t value);

/**
 * @brief Writes a 16-bit value from the BIOS.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_bios_write16(uint32_t address, uint16_t value);

/**
 * @brief Writes a 32-bit value from the BIOS.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_bios_write32(uint32_t address, uint32_t value);

#endif
