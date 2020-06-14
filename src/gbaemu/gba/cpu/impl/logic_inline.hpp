#pragma once

#include <cstdint>

#define CARRY(a, b)
#define ROR32(value, rotation) (((uint32_t)(value) << (32 - (uint32_t)(rotation))) | ((uint32_t)(value) >> (uint32_t)(rotation)))
#define SIGN32(value) (((uint32_t)(value)) >> 31)
#define SIGN64(value) (((uint64_t)(value)) >> 63)
#define SUB32_FLAGC(a, b) (((uint32_t)(a)) >= ((uint32_t)(b)))
#define SUB32_FLAGV(a, b, r) (SIGN32(((uint32_t)(a)) ^ ((uint32_t)(b))) && SIGN32(((uint32_t)(a)) ^ ((uint32_t)(r))))
#define ADD32_FLAGV(a, b, r) (!(SIGN32(((uint32_t)(a)) ^ ((uint32_t)(b)))) && SIGN32(((uint32_t)(a)) ^ ((uint32_t)(r))))
#define SBC32_FLAGC(a, b, c) ((a >= b) && ((a - b) >= !c))
#define SIGN32TO64(x) ((int64_t)((int32_t)(x)))
#define SIGN16TO32(x) ((int32_t)((int16_t)(x)))
#define SIGN8TO16(x) ((int16_t)((int8_t)(x)))
#define SIGN8TO32(x) (SIGN16TO32(SIGN8TO16(x)))
