#include <cstdint>
#include <cstdio>

#include <gbaemu/gba/lcd.hpp>

namespace gbaemu::gba::lcd {
    void writeCallback_dispcnt(uint16_t value) {
        printf("Wrote %04x to DISPCNT\n", value);
    }
}