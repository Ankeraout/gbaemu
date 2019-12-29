#include <cstddef>
#include <gbaemu/platform.hpp>

#if defined(GBAEMU_OS_UNIX)
#include <unistd.h>
#include <sys/stat.h>
#elif defined(GBAEMU_OS_WINDOWS)
#include <windows.h>
#else
#error Unsupported compilation target.
#endif

#include <gbaemu/fs.hpp>

bool gbaemu::fileExists(const char *filePath) {
#if defined(GBAEMU_OS_UNIX)
    return access(filePath, F_OK) != -1;
#elif defined(GBAEMU_OS_WINDOWS)
    DWORD fileAttributes = GetFileAttributes(filePath);

    return (
        (fileAttributes != INVALID_FILE_ATTRIBUTES)
        && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    );
#endif
}

size_t gbaemu::getFileSize(const char *filePath) {
#if defined(GBAEMU_OS_UNIX)
    struct stat st;
    stat(filePath, &st);
    return st.st_size;
#elif defined(GBAEMU_OS_WINDOWS)
    WIN32_FIND_DATA findFileData;
    HANDLE fileHandle = FindFirstFile(filePath, &findFileData);
    
    size_t fileSize = findFileData.nFileSizeHigh;
    fileSize <<= sizeof(findFileData.nFileSizeLow) * 8;
    fileSize |= findFileData.nFileSizeLow;

    FindClose(fileHandle);

    return fileSize;
#endif
}
