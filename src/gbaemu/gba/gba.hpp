#pragma once

#include <cstddef>
#include <cstdint>

#include <gbaemu/gba/cartridge.hpp>

namespace gbaemu::gba {
    static inline const size_t biosFileSize = 16384;
    extern uint8_t biosData[];

    extern void init(const char *biosFilePath, const char *romFilePath);
}
