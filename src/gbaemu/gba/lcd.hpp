#pragma once

#include <cstdint>

#include <gbaemu/gbaemu.hpp>

namespace gbaemu::gba::lcd {
    typedef uint32_t lcd_color_t;

    typedef union {
        PACKED_STRUCT(
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
        ) fields;

        uint16_t value;
     } dispcnt_t;

    typedef union {
        PACKED_STRUCT(
            uint16_t vblank : 1;
            uint16_t hblank : 1;
            uint16_t vcounter : 1;
            uint16_t irq_vblank : 1;
            uint16_t irq_hblank : 1;
            uint16_t irq_vcounter : 1;
            uint16_t unused : 2;
            uint16_t lyc : 8;
        ) fields;

        uint16_t value;
    } dispstat_t;

    extern uint8_t palette[1024];
    extern uint8_t vram[98304];
    extern uint8_t oam[1024];

    extern void writeCallback_dispcnt(uint16_t value);

    extern void cycle();
}
