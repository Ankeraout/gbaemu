#pragma once

namespace gbaemu {
    typedef struct {
        const char *biosPath;
    } conf_t;

    extern gbaemu::conf_t conf;
}

