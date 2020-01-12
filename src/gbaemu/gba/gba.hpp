#pragma once

#include <cstddef>
#include <cstdint>

#include <gbaemu/gba/cartridge.hpp>

namespace gbaemu::gba {
    static inline const size_t biosFileSize = 16384;
    static inline const size_t wramSlowSize = 262144;
    static inline const size_t wramFastSize = 32768;
    
    extern uint8_t biosData[];
    extern uint8_t wramData_slow[];
    extern uint8_t wramData_fast[];

    extern void init(const char *biosFilePath, const char *romFilePath);
    extern void cycle();
}
