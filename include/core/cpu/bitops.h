#ifndef __INCLUDE_CORE_BITOPS_H__
#define __INCLUDE_CORE_BITOPS_H__

#include <stdint.h>

static inline uint32_t rotateRight(
    const uint32_t p_value,
    const uint32_t p_rotation) {
    uint32_t l_value = (uint32_t)(p_value >> p_rotation);
    l_value |= (uint32_t)(p_value << (32 - p_rotation));
    return l_value;
}

static inline uint32_t signExtend8to32(const uint8_t p_value) {
    return (int8_t)p_value;
}

static inline uint32_t signExtend16to32(const uint16_t p_value) {
    return (int16_t)p_value;
}

static inline uint32_t signExtend26to32(const uint32_t p_value) {
    return (uint32_t)(((int32_t)(p_value << 6)) >> 6);
}

static inline int64_t signExtend32to64(const uint32_t p_value) {
    return (int64_t)((int32_t)p_value);
}

static inline int hammingWeight8(uint8_t p_value) {
    int l_result = 0;

    while(p_value != 0) {
        l_result += p_value & 0x1;
        p_value >>= 1;
    }

    return l_result;
}

static inline int hammingWeight16(uint16_t p_value) {
    int l_result = 0;

    while(p_value != 0) {
        l_result += p_value & 0x1;
        p_value >>= 1;
    }

    return l_result;
}

#endif
