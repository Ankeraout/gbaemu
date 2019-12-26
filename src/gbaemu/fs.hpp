#pragma once

#include <cstddef>

namespace gbaemu {
    bool fileExists(const char *filePath);
    size_t getFileSize(const char *filePath);
}
