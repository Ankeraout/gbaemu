#ifndef __GPU_H__
#define __GPU_H__

//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

//==============================================================================
// Public functions declaration
//==============================================================================
/**
 * @brief Reads a byte from the OAM address space.
 *
 * @param[in] p_address The address where the byte shall be read.
 *
 * @returns The byte at the given address.
 */
uint8_t gpuOamRead8(uint32_t p_address);

/**
 * @brief Reads a halfword from the OAM address space.
 *
 * @param[in] p_address The address where the halfword shall be read.
 *
 * @returns The halfword at the given address.
 */
uint16_t gpuOamRead16(uint32_t p_address);

/**
 * @brief Reads a word from the OAM address space.
 *
 * @param[in] p_address The address where the word shall be read.
 *
 * @returns The word at the given address.
 */
uint32_t gpuOamRead32(uint32_t p_address);

/**
 * @brief Writes a byte to the OAM address space.
 *
 * @param[in] p_address The address where the byte shall be written.
 * @param[in] p_value The byte to write.
 */
void gpuOamWrite8(uint32_t p_address, uint8_t p_value);

/**
 * @brief Writes a halfword to the OAM address space.
 *
 * @param[in] p_address The address where the halfword shall be written.
 * @param[in] p_value The halfword to write.
 */
void gpuOamWrite16(uint32_t p_address, uint16_t p_value);

/**
 * @brief Writes a word to the OAM address space.
 *
 * @param[in] p_address The address where the word shall be written.
 * @param[in] p_value The word to write.
 */
void gpuOamWrite32(uint32_t p_address, uint32_t p_value);

/**
 * @brief Reads a byte from the palette address space.
 *
 * @param[in] p_address The address where the byte shall be read.
 *
 * @returns The byte at the given address.
 */
uint8_t gpuPaletteRead8(uint32_t p_address);

/**
 * @brief Reads a halfword from the palette address space.
 *
 * @param[in] p_address The address where the halfword shall be read.
 *
 * @returns The halfword at the given address.
 */
uint16_t gpuPaletteRead16(uint32_t p_address);

/**
 * @brief Reads a word from the palette address space.
 *
 * @param[in] p_address The address where the word shall be read.
 *
 * @returns The word at the given address.
 */
uint32_t gpuPaletteRead32(uint32_t p_address);

/**
 * @brief Writes a byte to the palette address space.
 *
 * @param[in] p_address The address where the byte shall be written.
 * @param[in] p_value The byte to write.
 */
void gpuPaletteWrite8(uint32_t p_address, uint8_t p_value);

/**
 * @brief Writes a halfword to the palette address space.
 *
 * @param[in] p_address The address where the halfword shall be written.
 * @param[in] p_value The halfword to write.
 */
void gpuPaletteWrite16(uint32_t p_address, uint16_t p_value);

/**
 * @brief Writes a word to the palette address space.
 *
 * @param[in] p_address The address where the word shall be written.
 * @param[in] p_value The word to write.
 */
void gpuPaletteWrite32(uint32_t p_address, uint32_t p_value);

/**
 * @brief Reads a byte from the VRAM address space.
 *
 * @param[in] p_address The address where the byte shall be read.
 *
 * @returns The byte at the given address.
 */
uint8_t gpuVramRead8(uint32_t p_address);

/**
 * @brief Reads a halfword from the VRAM address space.
 *
 * @param[in] p_address The address where the halfword shall be read.
 *
 * @returns The halfword at the given address.
 */
uint16_t gpuVramRead16(uint32_t p_address);

/**
 * @brief Reads a word from the VRAM address space.
 *
 * @param[in] p_address The address where the word shall be read.
 *
 * @returns The word at the given address.
 */
uint32_t gpuVramRead32(uint32_t p_address);

/**
 * @brief Writes a byte to the VRAM address space.
 *
 * @param[in] p_address The address where the byte shall be written.
 * @param[in] p_value The byte to write.
 */
void gpuVramWrite8(uint32_t p_address, uint8_t p_value);

/**
 * @brief Writes a halfword to the VRAM address space.
 *
 * @param[in] p_address The address where the halfword shall be written.
 * @param[in] p_value The halfword to write.
 */
void gpuVramWrite16(uint32_t p_address, uint16_t p_value);

/**
 * @brief Writes a word to the VRAM address space.
 *
 * @param[in] p_address The address where the word shall be written.
 * @param[in] p_value The word to write.
 */
void gpuVramWrite32(uint32_t p_address, uint32_t p_value);

#endif // __GPU_H__