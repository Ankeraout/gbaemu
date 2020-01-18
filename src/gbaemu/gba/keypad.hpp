#pragma once

#include <cstdint>

namespace gbaemu::gba::keypad {
    typedef enum {
        A = 0x0001,
        B = 0x0002,
        SELECT = 0x0004,
        START = 0x0008,
        RIGHT = 0x0010,
        LEFT = 0x0020,
        UP = 0x0040,
        DOWN = 0x0080,
        R = 0x0100,
        L = 0x0200
    } key_t;

    void init();
    void keyDown(key_t key);
    void keyUp(key_t key);
}
