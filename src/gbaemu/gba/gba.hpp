#pragma once

#include <cstddef>
#include <cstdint>

namespace gbaemu::gba {
    class GBA {
        private:
        static inline const size_t biosFileSize = 16384;
        uint8_t *bios;
        uint8_t *rom;

        void loadBIOS(const char *biosFilePath);
        void loadROM(const char *romFilePath);

        public:
        GBA(const char *biosFilePath, const char *romFilePath);
    };
}
