#ifndef __IO_H__
#define __IO_H__

//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

//==============================================================================
// Public functions declaration
//==============================================================================
/**
 * @brief Reads a byte from the I/O registers memory space.
 *
 * @param[in] p_address The address of the byte to read in I/O memory space.
 *
 * @returns The byte read at the given address in the I/O memory space.
 */
uint8_t ioRead8(uint32_t p_address);

/**
 * @brief Reads a halfword from the I/O registers memory space.
 *
 * @param[in] p_address The address of the halfword to read in I/O memory space.
 *
 * @returns The halfword read at the given address in the I/O memory space.
 */
uint16_t ioRead16(uint32_t p_address);

/**
 * @brief Reads a word from the I/O registers memory space.
 *
 * @param[in] p_address The address of the word to read in I/O memory space.
 *
 * @returns The word read at the given address in the I/O memory space.
 */
uint32_t ioRead32(uint32_t p_address);

/**
 * @brief Writes a byte to the I/O registers memory space.
 *
 * @param[in] p_address The address where the byte shall be written.
 * @param[in] p_value The byte to write.
 */
void ioWrite8(uint32_t p_address, uint8_t p_value);

/**
 * @brief Writes a halfword to the I/O registers memory space.
 *
 * @param[in] p_address The address where the halfword shall be written.
 * @param[in] p_value The halfword to write.
 */
void ioWrite16(uint32_t p_address, uint16_t p_value);

/**
 * @brief Writes a word to the I/O registers memory space.
 *
 * @param[in] p_address The address where the word shall be written.
 * @param[in] p_value The word to write.
 */
void ioWrite32(uint32_t p_address, uint32_t p_value);

#endif // __IO_H__