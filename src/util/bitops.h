#ifndef __BITOPS_H__
#define __BITOPS_H__

//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

//==============================================================================
// Public functions declaration
//==============================================================================
/**
 * @brief Rotates right the given 32-bit value.
 *
 * @param[in] p_value The value to rotate.
 * @param[in] p_rotation The rotation to apply.
 *
 * @returns The result of the rotation operation.
 */
static inline uint32_t rotateRight(
    const uint32_t p_value,
    const uint32_t p_rotation
);

/**
 * @brief Sign-extends the given 26-bit word to a 32-bit word.
 *
 * @param[in] p_value The 26-bit integer to sign-extend.
 *
 * @returns The sign-extended version of the given integer.
 */
static inline uint32_t signExtend26to32(const uint32_t p_value);

//==============================================================================
// Public functions definition
//==============================================================================
static inline uint32_t rotateRight(
    const uint32_t p_value,
    const uint32_t p_rotation) {
    uint32_t l_value = (uint32_t)(p_value >> p_rotation);
    l_value |= (uint32_t)(p_value << (32 - p_rotation));
    return l_value;
}

static inline uint32_t signExtend26to32(const uint32_t p_value) {
    return (uint32_t)(((int32_t)(p_value << 6)) >> 6);
}

static inline int64_t signExtend32to64(const uint32_t p_value) {
    return (int64_t)((int32_t)p_value);
}

#endif // __BITOPS_H__