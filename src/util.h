#ifndef __UTIL_H__
#define __UTIL_H__

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Accesses to the byte at the given offset in the given array.
 */
#define ACCESS_8(array, offset) (*((uint8_t *)(((size_t)(array) + (size_t)(offset)))))

/**
 * @brief Accesses to the 16-bit word at the given offset in the given array.
 */
#define ACCESS_16(array, offset) (*((uint16_t *)(((size_t)(array) + (size_t)(offset)))))

/**
 * @brief Accesses to the 32-bit word at the given offset in the given array.
 */
#define ACCESS_32(array, offset) (*((uint32_t *)(((size_t)(array) + (size_t)(offset)))))

#endif
