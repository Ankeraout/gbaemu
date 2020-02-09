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

    typedef union {
        PACKED_STRUCT(
            uint16_t bgPriority : 2;
            uint16_t tileBaseAddress : 2;
            uint16_t unused : 2;
            uint16_t mosaic : 1;
            uint16_t colors : 1;
            uint16_t mapBaseAddress : 5;
            uint16_t displayAreaOverflow : 1;
            uint16_t screenSize : 2;
        ) fields;

        uint16_t value;
    } bgcnt_t;

    static inline const unsigned int screenWidth = 240;
    static inline const unsigned int screenHeight = 160;

    extern uint8_t palette[1024];
    extern uint8_t vram[98304];
    extern uint8_t oam[1024];

    static inline uint16_t getRed(uint16_t color) {
        return color & 0x001f;
    }

    static inline uint16_t getGreen(uint16_t color) {
        return (color & 0x03e0) >> 5;
    }

    static inline uint16_t getBlue(uint16_t color) {
        return (color & 0x7c00) >> 10;
    }

    static inline uint32_t colorToRgb(uint16_t color) {
        uint16_t red = getRed(color);
        uint16_t green = getGreen(color);
        uint16_t blue = getBlue(color);

        return 0xff000000 | (blue << 19) | (green << 11) | (red << 3);
    }

    static inline uint32_t getPaletteColor(uint8_t paletteIndex) {
        uint16_t *paletteTable = (uint16_t *)palette;
        return colorToRgb(paletteTable[paletteIndex]);
    }

    extern void writeCallback_dispcnt(uint16_t value);

    extern void cycle();

    extern const uint32_t *getFramebuffer();
}
