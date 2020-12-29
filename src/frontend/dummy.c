#include <stdint.h>

#include "platform.h"

void frontend_close() {

}

void frontend_frame(const uint32_t *buffer) {
    UNUSED(buffer);
}

int frontend_init() {
    return 0;
}
