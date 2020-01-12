#include <cstdint>
#include <cstdio>

#include <gbaemu/gba/lcd.hpp>

namespace gbaemu::gba::lcd {
    uint8_t palette[1024];
    uint8_t vram[98304];
    uint8_t oam[1024];

    void writeCallback_dispcnt(uint16_t value) {
        printf("Wrote %04x to DISPCNT\n", value);
    }
}