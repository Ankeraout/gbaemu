#include <cstddef>

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include <unistd.h>
#include <sys/stat.h>
#else
#error Unsupported compilation target.
#endif

#include <gbaemu/fs.hpp>

bool gbaemu::fileExists(const char *filePath) {
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
    return access(filePath, F_OK) != -1;
#endif
}

size_t gbaemu::getFileSize(const char *filePath) {
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
    struct stat st;
    stat(filePath, &st);
    return st.st_size;
#endif
}
