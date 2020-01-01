#pragma once

#ifdef _MSC_VER
#define PACKED_STRUCT(__declaration__) __pragma(pack(push, 1)) struct {__declaration__} __pragma(pack(pop))
#elif defined(__GNUC__)
#define PACKED_STRUCT(__declaration__) struct {__declaration__} __attribute__((packed))
#else
#error Unsupported build environment.
#endif

#define UNUSED(parameter) ((void)parameter)

namespace gbaemu {
    typedef struct {
        const char *biosPath;
        const char *romPath;
        const char *forcedSaveType;
    } conf_t;

    extern conf_t conf;
}

