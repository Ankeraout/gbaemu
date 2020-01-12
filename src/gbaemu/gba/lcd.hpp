#pragma once

#include <cstdint>

#include <gbaemu/gbaemu.hpp>

namespace gbaemu::gba::lcd {
    typedef uint32_t lcd_color_t;

    typedef PACKED_STRUCT(
        uint16_t bgMode : 3;
        uint16_t cgb : 1;
        uint16_t displayFrameSelect : 1;
        uint16_t hblankIntervalFree : 1;
        uint16_t objCharacterVramMapping : 1;
        uint16_t forcedBlank : 1;
        uint16_t bg0 : 1;
        uint16_t bg1 : 1;
        uint16_t bg2 : 1;
        uint16_t bg3 : 1;
        uint16_t obj : 1;
        uint16_t win0 : 1;
        uint16_t win1 : 1;
        uint16_t objWin : 1;
    ) dispcnt_t;

    extern uint8_t palette[1024];
    extern uint8_t vram[98304];
    extern uint8_t oam[1024];

    extern void writeCallback_dispcnt(uint16_t value);
}
