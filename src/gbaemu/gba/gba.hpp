#pragma once

#include <cstddef>
#include <cstdint>

namespace gbaemu::gba {
    class GBA {
        private:
        static inline const size_t biosFileSize = 16384;
        uint8_t *bios;

        void loadBIOS(const char *biosFilePath);

        public:
        GBA(const char *biosFilePath);
    };
}
