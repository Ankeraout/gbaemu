//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

#include "common.h"
#include "io.h"

//==============================================================================
// Private constants
//==============================================================================
/**
 * @brief This constant defines the size of the I/O map in number of entries.
 */
#define C_IO_MAP_SIZE 0x400

//==============================================================================
// Private types
//==============================================================================
/**
 * @brief This structure represents an entry in the I/O registers map.
 */
typedef struct {
    uint8_t (*read8)(uint32_t p_address); /**< Handler for the read8
                                               operation. */
    uint16_t (*read16)(uint32_t p_address); /**< Handler for the read16
                                                 operation. */
    uint32_t (*read32)(uint32_t p_address); /**< Handler for the read32
                                                 operation. */
    void (*write8)(uint32_t p_address, uint8_t p_value); /**< Handler for the
                                                              write8
                                                              operation. */
    void (*write16)(uint32_t p_address, uint16_t p_value); /**< Handler for the
                                                              write16
                                                              operation. */
    void (*write32)(uint32_t p_address, uint32_t p_value); /**< Handler for the
                                                              write32
                                                              operation. */
} t_ioMapEntry;

//==============================================================================
// Private functions declaration
//==============================================================================
/**
 * @brief Reads a byte from the register at the given address in the I/O
 *        registers address space.
 *
 * @param[in] p_address The address of the byte to read.
 *
 * @returns The byte read at the given address.
 */
uint8_t ioReadDummy8(uint32_t p_address);

/**
 * @brief Reads a halfword from the register at the given address in the I/O
 *        registers address space.
 *
 * @param[in] p_address The address of the halfword to read.
 *
 * @returns The halfword read at the given address.
 */
uint16_t ioReadDummy16(uint32_t p_address);

/**
 * @brief Reads a word from the register at the given address in the I/O
 *        registers address space.
 *
 * @param[in] p_address The address of the word to read.
 *
 * @returns The word read at the given address.
 */
uint32_t ioReadDummy32(uint32_t p_address);

/**
 * @brief Writes a byte to the register at the given address in the I/O
 *        registers address space.
 *
 * @param[in] p_address The address where the byte shall be written.
 * @param[in] p_value The byte to write.
 */
void ioWriteDummy8(uint32_t p_address, uint8_t p_value);

/**
 * @brief Writes a halfword to the register at the given address in the I/O
 *        registers address space.
 *
 * @param[in] p_address The address where the halfword shall be written.
 * @param[in] p_value The halfword to write.
 */
void ioWriteDummy16(uint32_t p_address, uint16_t p_value);

/**
 * @brief Writes a word to the register at the given address in the I/O
 *        registers address space.
 *
 * @param[in] p_address The address where the word shall be written.
 * @param[in] p_value The word to write.
 */
void ioWriteDummy32(uint32_t p_address, uint32_t p_value);

//==============================================================================
// Private variables
//==============================================================================
t_ioMapEntry s_ioMap[C_IO_MAP_SIZE];

//==============================================================================
// Public functions definition
//==============================================================================
uint8_t ioRead8(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);
    return 0;
}

uint16_t ioRead16(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);
    return 0;
}

uint32_t ioRead32(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);
    return 0;
}

void ioWrite8(uint32_t p_address, uint8_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

void ioWrite16(uint32_t p_address, uint16_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

void ioWrite32(uint32_t p_address, uint32_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

//==============================================================================
// Private functions definition
//==============================================================================
uint8_t ioReadDummy8(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);

    return 0;
}

uint16_t ioReadDummy16(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);

    return 0;
}

uint32_t ioReadDummy32(uint32_t p_address) {
    M_UNUSED_PARAMETER(p_address);

    return 0;
}

void ioWriteDummy8(uint32_t p_address, uint8_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

void ioWriteDummy16(uint32_t p_address, uint16_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

void ioWriteDummy32(uint32_t p_address, uint32_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}
