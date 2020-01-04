#pragma once

#include <cstdint>

#define CARRY(a, b)
#define ROR(value, rotation) ((((uint32_t)(value)) >> (rotation)) | (((uint32_t)(value)) << ((-(rotation)) & 0x1f)))
#define SIGN32(value) (((uint32_t)(value)) >> 31)
#define SUB32_FLAGC(a, b) ((a) >= (b))
#define SUB32_FLAGV(a, b, r) (SIGN32((a) ^ (b)) && SIGN32((a) ^ (r)))
#define ADD32_FLAGV(a, b, r) (!(SIGN32((a) ^ (b))) && SIGN32((a) ^ (r)))
