#ifndef __CORE_BUS_H__
#define __CORE_BUS_H__

#include <stdint.h>

#include "core/gba.h"

/**
 * @brief Initializes the GBA bus.
 */
void gba_bus_init();

/**
 * @brief Reads a 8-bit value on the bus.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 8-bit value read on the bus.
 */
uint8_t gba_bus_read8(uint32_t address);

/**
 * @brief Reads a 16-bit value on the bus.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 16-bit value read on the bus.
 */
uint16_t gba_bus_read16(uint32_t address);

/**
 * @brief Reads a 32-bit value on the bus.
 * 
 * @param[in] address The address of the value to read.
 * 
 * @returns The 32-bit value read on the bus.
 */
uint32_t gba_bus_read32(uint32_t address);

/**
 * @brief Writes a 8-bit value on the bus.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_bus_write8(uint32_t address, uint8_t value);

/**
 * @brief Writes a 16-bit value on the bus.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_bus_write16(uint32_t address, uint16_t value);

/**
 * @brief Writes a 32-bit value on the bus.
 * 
 * @param[in] address The address where the value shall be written.
 * @param[in] value The value that shall be written.
 */
void gba_bus_write32(uint32_t address, uint32_t value);

#endif
