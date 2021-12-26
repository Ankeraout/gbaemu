#ifndef __BUS_H__
#define __BUS_H__

//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

//==============================================================================
// Public constants
//==============================================================================

//==============================================================================
// Public types
//==============================================================================

//==============================================================================
// Public functions declaration
//==============================================================================
/**
 * @brief Reads a 8-bit value from the bus.
 *
 * @param[in] p_address The address from which the value shall be read.
 *
 * @returns The 8-bit value read from the bus at the given address.
 */
uint8_t busRead8(uint32_t p_address);

/**
 * @brief Reads a 16-bit value from the bus.
 *
 * @param[in] p_address The address from which the value shall be read.
 *
 * @returns The 16-bit value read from the bus at the given address.
 */
uint16_t busRead16(uint32_t p_address);

/**
 * @brief Reads a 32-bit value from the bus.
 *
 * @param[in] p_address The address from which the value shall be read.
 *
 * @returns The 32-bit value read from the bus at the given address.
 */
uint32_t busRead32(uint32_t p_address);

/**
 * @brief Writes a 8-bit value to the bus.
 *
 * @param[in] p_address The address to which the value shall be written.
 * @param[in] p_value The value that shall be written on the bus.
 */
void busWrite8(uint32_t p_address, uint8_t p_value);

/**
 * @brief Writes a 16-bit value to the bus.
 *
 * @param[in] p_address The address to which the value shall be written.
 * @param[in] p_value The value that shall be written on the bus.
 */
void busWrite16(uint32_t p_address, uint16_t p_value);

/**
 * @brief Writes a 32-bit value to the bus.
 *
 * @param[in] p_address The address to which the value shall be written.
 * @param[in] p_value The value that shall be written on the bus.
 */
void busWrite32(uint32_t p_address, uint32_t p_value);

#endif // __BUS_H__