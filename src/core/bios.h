#ifndef __BIOS_H__
#define __BIOS_H__

//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

//==============================================================================
// Public functions declaration
//==============================================================================
/**
 * @brief Reads a byte at the given address in the BIOS address space.
 *
 * @param[in] p_address The address of the byte to read.
 *
 * @returns The byte read at the given address in the BIOS address space.
 */
uint8_t biosRead8(uint32_t p_address);

/**
 * @brief Reads a halfword at the given address in the BIOS address space.
 *
 * @param[in] p_address The address of the halfword to read.
 *
 * @returns The halfword read at the given address in the BIOS address space.
 */
uint16_t biosRead16(uint32_t p_address);

/**
 * @brief Reads a byte at the given address in the BIOS address space.
 *
 * @param[in] p_address The address of the byte to read.
 *
 * @returns The byte read at the given address in the BIOS address space.
 */
uint32_t biosRead32(uint32_t p_address);

#endif // __BIOS_H__