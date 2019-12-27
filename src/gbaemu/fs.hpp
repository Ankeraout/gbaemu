#pragma once

#include <cstddef>

namespace gbaemu {
    extern bool fileExists(const char *filePath);
    extern size_t getFileSize(const char *filePath);
}
