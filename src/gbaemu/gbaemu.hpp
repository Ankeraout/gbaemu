#pragma once

#define UNUSED_PARAMETER(parameter) ((void *)parameter)

namespace gbaemu {
    typedef struct {
        const char *biosPath;
        const char *romPath;
        const char *forcedSaveType;
    } conf_t;

    extern conf_t conf;
}

