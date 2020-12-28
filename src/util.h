#ifndef __UTIL_H__
#define __UTIL_H__

#include <stddef.h>
#include <stdint.h>

#define ACCESS_N(bitsSize, array, offset) *((uint##bitsSize##_t *)(((size_t)(array) + (size_t)(offset))))
#define ACCESS_8(array, offset) ACCESS_N(8, array, offset)
#define ACCESS_16(array, offset) ACCESS_N(16, array, offset)
#define ACCESS_32(array, offset) ACCESS_N(32, array, offset)

#endif
