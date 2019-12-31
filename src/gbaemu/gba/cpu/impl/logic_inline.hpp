#pragma once

#include <cstdint>

#define CARRY(a, b)
#define ROR(value, rotation) ((((uint32_t)(value)) >> (rotation)) | (((uint32_t)(value)) << ((-(rotation)) & 0x1f)))
#define SIGN32(value) (((uint32_t)(value)) >> 31)
