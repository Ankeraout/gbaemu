#pragma once

namespace gbaemu {
    typedef struct {
        const char *biosPath;
        const char *romPath;
    } conf_t;

    extern gbaemu::conf_t conf;
}

