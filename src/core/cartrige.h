#ifndef __CARTRIDGE_H__
#define __CARTRIDGE_H__

//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

//==============================================================================
// Public functions declaration
//==============================================================================
/**
 * @brief Initializes the cartridge module.
 */
void cartridgeInit(void);

/**
 * @brief Reads a byte at the given address in the ROM address space.
 *
 * @param[in] p_address The address of the byte to read.
 *
 * @returns The byte read at the given address in the ROM address space.
 */
uint8_t cartridgeRomRead8(uint32_t p_address);

/**
 * @brief Reads a halfword at the given address in the ROM address space.
 *
 * @param[in] p_address The address of the halfword to read.
 *
 * @returns The halfword read at the given address in the ROM address space.
 */
uint16_t cartridgeRomRead16(uint32_t p_address);

/**
 * @brief Reads a word at the given address in the ROM address space.
 *
 * @param[in] p_address The address of the word to read.
 *
 * @returns The word read at the given address in the ROM address space.
 */
uint32_t cartridgeRomRead32(uint32_t p_address);

/**
 * @brief Reads a byte at the given address in the SRAM address space.
 *
 * @param[in] p_address The address of the byte to read.
 *
 * @returns The byte read at the given address in the SRAM address space.
 */
uint8_t cartridgeSramRead8(uint32_t p_address);

/**
 * @brief Reads a halfword at the given address in the SRAM address space.
 *
 * @param[in] p_address The address of the halfword to read.
 *
 * @returns The halfword read at the given address in the SRAM address space.
 */
uint16_t cartridgeSramRead16(uint32_t p_address);

/**
 * @brief Reads a word at the given address in the SRAM address space.
 *
 * @param[in] p_address The address of the word to read.
 *
 * @returns The word read at the given address in the SRAM address space.
 */
uint32_t cartridgeSramRead32(uint32_t p_address);

/**
 * @brief Writes a byte at the given address in the SRAM address space.
 *
 * @param[in] p_address The address of the byte to write.
 * @param[in] p_value The byte to write.
 */
void cartridgeSramWrite8(uint32_t p_address, uint8_t p_value);

/**
 * @brief Writes a halfword at the given address in the SRAM address space.
 *
 * @param[in] p_address The address of the halfword to write.
 * @param[in] p_value The halfword to write.
 */
void cartridgeSramWrite16(uint32_t p_address, uint16_t p_value);

/**
 * @brief Writes a word at the given address in the SRAM address space.
 *
 * @param[in] p_address The address of the word to write.
 * @param[in] p_value The word to write.
 */
void cartridgeSramWrite32(uint32_t p_address, uint32_t p_value);

#endif // __CARTRIDGE_H__