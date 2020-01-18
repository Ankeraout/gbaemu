#include <cstdint>
#include <cstdio>

#include <gbaemu/gba/io.hpp>
#include <gbaemu/gba/keypad.hpp>

namespace gbaemu::gba::keypad {
    void init() {
        gbaemu::gba::io::set(io::KEYINPUT, 0x03ff);
        gbaemu::gba::io::set(io::KEYCNT, 0x0000);
    }

    void keyDown(key_t key) {
        uint16_t keyinput = gbaemu::gba::io::get(io::KEYINPUT);
        keyinput &= ~key;
        gbaemu::gba::io::set(io::KEYINPUT, keyinput);
    }

    void keyUp(key_t key) {
        uint16_t keyinput = gbaemu::gba::io::get(io::KEYINPUT);
        keyinput |= key;
        gbaemu::gba::io::set(io::KEYINPUT, keyinput);
    }
}
