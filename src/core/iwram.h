#ifndef __IWRAM_H__
#define __IWRAM_H__

//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

//==============================================================================
// Public functions declaration
//==============================================================================
/**
 * @brief Reads a byte at the given address in the IWRAM address space.
 *
 * @param[in] p_address The address of the byte to read.
 *
 * @returns The byte read at the given address in the IWRAM address space.
 */
uint8_t iwramRead8(uint32_t p_address);

/**
 * @brief Reads a halfword at the given address in the IWRAM address space.
 *
 * @param[in] p_address The address of the halfword to read.
 *
 * @returns The halfword read at the given address in the IWRAM address space.
 */
uint16_t iwramRead16(uint32_t p_address);

/**
 * @brief Reads a word at the given address in the IWRAM address space.
 *
 * @param[in] p_address The address of the word to read.
 *
 * @returns The word read at the given address in the IWRAM address space.
 */
uint32_t iwramRead32(uint32_t p_address);

/**
 * @brief Writes a byte at the given address in the IWRAM address space.
 *
 * @param[in] p_address The address of the byte to write.
 * @param[in] p_value The byte to write at the given address in the IWRAM
 *                    address space.
 */
void iwramWrite8(uint32_t p_address, uint8_t p_value);

/**
 * @brief Writes a halfword at the given address in the IWRAM address space.
 *
 * @param[in] p_address The address of the halfword to write.
 * @param[in] p_value The halfword to write at the given address in the IWRAM
 *                    address space.
 */
void iwramWrite16(uint32_t p_address, uint16_t p_value);

/**
 * @brief Write a word at the given address in the IWRAM address space.
 *
 * @param[in] p_address The address of the word to write.
 * @param[in] p_value The word to write at the given address in the IWRAM
 *                    address space.
 */
void iwramWrite32(uint32_t p_address, uint32_t p_value);

#endif // __IWRAM_H__