#pragma once

#include <cstddef>
#include <cstdint>

#include <gbaemu/gba/cartridge.hpp>

namespace gbaemu::gba {
    class GBA {
        private:
        static inline const size_t biosFileSize = 16384;
        uint8_t *bios;
        Cartridge *cartridge;

        void loadBIOS(const char *biosFilePath);

        public:
        GBA(const char *biosFilePath, const char *romFilePath);
    };
}
