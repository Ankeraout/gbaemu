#include <stdint.h>
#include <string.h>

#include "util.h"
#include "core/defines.h"
#include "core/dma.h"
#include "core/gba.h"
#include "core/io.h"
#include "frontend/frontend.h"

uint8_t gba_ppu_palette[GBA_PALETTE_SIZE];
uint8_t gba_ppu_vram[GBA_VRAM_SIZE];
uint8_t gba_ppu_oam[GBA_OAM_SIZE];

uint32_t gba_ppu_frameBuffer[GBA_SCREEN_WIDTH * GBA_SCREEN_HEIGHT];
uint_least32_t gba_ppu_currentRow;
uint_least32_t gba_ppu_currentColumn;
uint_least32_t gba_ppu_currentCycle;
unsigned int gba_ppu_layers[4];

void gba_ppu_reset();
uint8_t gba_ppu_palette_read8(uint32_t address);
uint16_t gba_ppu_palette_read16(uint32_t address);
uint32_t gba_ppu_palette_read32(uint32_t address);
void gba_ppu_palette_write8(uint32_t address, uint8_t value);
void gba_ppu_palette_write16(uint32_t address, uint16_t value);
void gba_ppu_palette_write32(uint32_t address, uint32_t value);
uint8_t gba_ppu_vram_read8(uint32_t address);
uint16_t gba_ppu_vram_read16(uint32_t address);
uint32_t gba_ppu_vram_read32(uint32_t address);
void gba_ppu_vram_write8(uint32_t address, uint8_t value);
void gba_ppu_vram_write16(uint32_t address, uint16_t value);
void gba_ppu_vram_write32(uint32_t address, uint32_t value);
uint8_t gba_ppu_oam_read8(uint32_t address);
uint16_t gba_ppu_oam_read16(uint32_t address);
uint32_t gba_ppu_oam_read32(uint32_t address);
void gba_ppu_oam_write8(uint32_t address, uint8_t value);
void gba_ppu_oam_write16(uint32_t address, uint16_t value);
void gba_ppu_oam_write32(uint32_t address, uint32_t value);
static inline uint32_t gba_ppu_colorToRgb(uint16_t color);
static inline uint16_t gba_ppu_getPaletteColor(uint8_t index);
static inline void gba_ppu_sortLayers();
static inline void gba_ppu_drawLayer(int layer);
static inline void gba_ppu_drawMode0();
static inline void gba_ppu_drawMode1();
static inline void gba_ppu_drawMode2();
static inline void gba_ppu_drawMode3();
static inline void gba_ppu_drawMode4();
static inline void gba_ppu_drawMode5();
static inline void gba_ppu_drawLine();
static inline void gba_ppu_onVblank();
static inline void gba_ppu_onHblank();

void gba_ppu_reset() {
    memset(gba_ppu_palette, 0, GBA_PALETTE_SIZE);
    memset(gba_ppu_vram, 0, GBA_VRAM_SIZE);
    memset(gba_ppu_oam, 0, GBA_OAM_SIZE);
}

void gba_ppu_cycle() {
    if((gba_ppu_currentCycle & 0x03) == 0x03) {
        gba_io_register_t *dispstat = gba_io_getRegister(0x04000004);

        if(gba_ppu_currentColumn == 308) {
            gba_ppu_currentColumn = 0;
            gba_ppu_currentRow++;

            dispstat->value &= ~(1 << 1);

            if((dispstat->value >> 8) == gba_ppu_currentRow) {
                dispstat->value |= (1 << 2);

                if(dispstat->value & (1 << 5)) {
                    gba_setInterruptFlag(1 << 2);
                }
            } else {
                dispstat->value &= ~(1 << 2);
            }

            if(gba_ppu_currentRow == 228) {
                gba_ppu_currentRow = 0;
                gba_ppu_currentCycle = 0;
                dispstat->value &= ~(1 << 0);
            } else if(gba_ppu_currentRow == GBA_SCREEN_HEIGHT) {
                dispstat->value |= (1 << 0);

                if(dispstat->value & (1 << 3)) {
                    gba_setInterruptFlag(1 << 0);
                }
                
                gba_ppu_onVblank();
            }

            gba_io_getRegister(0x04000006)->value = gba_ppu_currentRow;
        } else if(gba_ppu_currentColumn == GBA_SCREEN_WIDTH) {
            dispstat->value |= (1 << 1);

            if(dispstat->value & (1 << 4)) {
                gba_setInterruptFlag(1 << 1);
                gba_dma_onHblank();
            }

            if(gba_ppu_currentRow < GBA_SCREEN_HEIGHT) {
                gba_ppu_drawLine();
            }
        }

        gba_ppu_currentColumn++;
    }

    gba_ppu_currentCycle++;
}

uint8_t gba_ppu_palette_read8(uint32_t address) {
    return gba_ppu_palette[address & 0x000003ff];
}

uint16_t gba_ppu_palette_read16(uint32_t address) {
    return ACCESS_16(gba_ppu_palette, address & 0x000003fe);
}

uint32_t gba_ppu_palette_read32(uint32_t address) {
    return ACCESS_32(gba_ppu_palette, address & 0x000003fc);
}

void gba_ppu_palette_write8(uint32_t address, uint8_t value) {
    gba_ppu_palette_write16(address, (uint16_t)((value << 8) | value));
}

void gba_ppu_palette_write16(uint32_t address, uint16_t value) {
    ACCESS_16(gba_ppu_palette, address & 0x000003fe) = value;
}

void gba_ppu_palette_write32(uint32_t address, uint32_t value) {
    ACCESS_32(gba_ppu_palette, address & 0x000003fc) = value;
}

uint8_t gba_ppu_vram_read8(uint32_t address) {
    if(address & 0x00010000) {
        address &= 0x00017fff;
    } else {
        address &= 0x0001ffff;
    }

    return gba_ppu_vram[address];
}

uint16_t gba_ppu_vram_read16(uint32_t address) {
    if(address & 0x00010000) {
        address &= 0x00017ffe;
    } else {
        address &= 0x0001fffe;
    }

    return ACCESS_16(gba_ppu_vram, address);
}

uint32_t gba_ppu_vram_read32(uint32_t address) {
    if(address & 0x00010000) {
        address &= 0x00017ffc;
    } else {
        address &= 0x0001fffc;
    }

    return ACCESS_32(gba_ppu_vram, address);
}

void gba_ppu_vram_write8(uint32_t address, uint8_t value) {
    gba_ppu_vram_write16(address, (uint16_t)((value << 8) | value));
}

void gba_ppu_vram_write16(uint32_t address, uint16_t value) {
    if(address & 0x00010000) {
        address &= 0x00017ffe;
    } else {
        address &= 0x0001fffe;
    }

    ACCESS_16(gba_ppu_vram, address) = value;
}

void gba_ppu_vram_write32(uint32_t address, uint32_t value) {
    if(address & 0x00010000) {
        address &= 0x00017ffc;
    } else {
        address &= 0x0001fffc;
    }

    ACCESS_32(gba_ppu_vram, address) = value;
}

uint8_t gba_ppu_oam_read8(uint32_t address) {
    return gba_ppu_oam[address & 0x000003ff];
}

uint16_t gba_ppu_oam_read16(uint32_t address) {
    return ACCESS_16(gba_ppu_oam, address & 0x000003fe);
}

uint32_t gba_ppu_oam_read32(uint32_t address) {
    return ACCESS_32(gba_ppu_oam, address & 0x000003fc);
}

void gba_ppu_oam_write8(uint32_t address, uint8_t value) {
    gba_ppu_oam_write16(address, (uint16_t)((value << 8) | value));
}

void gba_ppu_oam_write16(uint32_t address, uint16_t value) {
    ACCESS_16(gba_ppu_oam, address & 0x000003fe) = value;
}

void gba_ppu_oam_write32(uint32_t address, uint32_t value) {
    ACCESS_32(gba_ppu_oam, address & 0x000003fc) = value;
}

static inline uint32_t gba_ppu_colorToRgb(uint16_t color) {
    uint32_t blue = (color & 0x7c00) >> 7;
    uint32_t green = (color & 0x03e0) >> 2;
    uint32_t red = color & 0x001f;

    red <<= 3;
    
    red |= red >> 5;
    green |= green >> 5;
    blue |= blue >> 5;

    return 0xff000000 | red | (green << 8) | (blue << 16);
}

static inline uint16_t gba_ppu_getPaletteColor(uint8_t index) {
    return gba_ppu_palette_read16(0x05000000 | (index << 1));
}

static inline void gba_ppu_sortLayers() {
    gba_ppu_layers[0] = 3;
    gba_ppu_layers[1] = 2;
    gba_ppu_layers[2] = 1;
    gba_ppu_layers[3] = 0;

    int priorities[4];

    for(int i = 0; i < 4; i++) {
        priorities[i] = gba_io_getRegister(0x04000008 + (i << 1))->value & 0x0003;
    }

    for(int i = 0; i < 3; i++) {
        int layerPriority = priorities[i];
        int maxPriority = layerPriority;
        int maxPriorityIndex = i;

        for(int j = i + 1; j < 4; j++) {
            int currentLayerPriority = priorities[j];

            if(currentLayerPriority > maxPriority) {
                maxPriority = currentLayerPriority;
                maxPriorityIndex = j;
            }
        }

        unsigned int exchange = gba_ppu_layers[i];
        gba_ppu_layers[i] = gba_ppu_layers[maxPriorityIndex];
        gba_ppu_layers[maxPriorityIndex] = exchange;
    }
}

static inline void gba_ppu_drawLayer(int layer) {
    gba_io_register_t *bgcnt = gba_io_getRegister(0x04000008 + (layer << 1));

    unsigned int hofs = gba_io_getRegister(0x04000010 + (layer << 2))->value;
    unsigned int vofs = gba_io_getRegister(0x04000012 + (layer << 2))->value;
    uint32_t mapBase = (bgcnt->value & 0x1f00) << 3;
    uint32_t tileBase = (bgcnt->value & 0x000c) << 12;
    unsigned int yLayer = gba_ppu_currentRow + vofs;

    uint32_t mapOffsetY = 0x00000000;

    if(bgcnt->value & (1 << 15)) {
        yLayer &= 0x000001ff;

        if(((bgcnt->value & 0xc000) == 0xc000) && (yLayer >= 0x100)) {
            mapOffsetY = 0x00001000;
        }
    } else {
        yLayer &= 0x000000ff;
    }

    unsigned int yChunk = yLayer & 0x000000ff;
    unsigned int yMap = yChunk >> 3;
    unsigned int yTile = yChunk & 7;

    for(unsigned int x = 0; x < GBA_SCREEN_WIDTH; x++) {
        unsigned int xLayer = x + hofs;
        uint32_t mapOffsetX = 0x00000000;

        if(bgcnt->value & (1 << 14)) {
            xLayer &= 0x000001ff;

            if((bgcnt->value & (1 << 14)) && (xLayer >= 0x100)) {
                mapOffsetX = 0x00000800;
            }
        } else {
            xLayer &= 0x000000ff;
        }

        unsigned int xChunk = xLayer & 0x000000ff;
        unsigned int xMap = xChunk >> 3;
        unsigned int xTile = xChunk & 0x07;
        uint32_t mapOffset = mapBase + mapOffsetX + mapOffsetY;
        uint32_t mapAddress = mapOffset + (yMap << 6) + (xMap << 1);
        uint16_t mapValue = gba_ppu_vram[mapAddress] | (gba_ppu_vram[mapAddress + 1] << 8);
        uint16_t tileNumber = mapValue & 0x03ff;
        bool flipHorizontal = (mapValue & (1 << 10)) != 0;
        bool flipVertical = (mapValue & (1 << 11)) != 0;

        unsigned int yTileReal = flipVertical ? 7 - yTile : yTile;
        unsigned int xTileReal = flipHorizontal ? 7 - xTile : xTile;

        uint32_t colorAddress;

        if(bgcnt->value & (1 << 7)) {
            uint32_t tileDataAddress = (tileNumber << 6) | (yTileReal << 3) | xTileReal;
            uint8_t tileValue = gba_ppu_vram[tileBase + tileDataAddress];
            colorAddress = tileValue << 1;
        } else {
            uint32_t tileDataAddress = (tileNumber << 5) | (yTileReal << 2) | (xTileReal >> 1);
            uint8_t tileValue = gba_ppu_vram[tileBase + tileDataAddress];

            if(!((xTileReal & (1 << 0)) ^ flipHorizontal)) {
                tileValue >>= 4;
            } else {
                tileValue &= 0x0f;
            }

            unsigned int paletteNumber = mapValue >> 12;

            colorAddress = (paletteNumber << 5) | (tileValue << 1);
        }

        uint16_t pixelColor = gba_ppu_palette[colorAddress] | (gba_ppu_palette[colorAddress + 1] << 8);
        gba_ppu_frameBuffer[gba_ppu_currentRow * GBA_SCREEN_WIDTH + x] = gba_ppu_colorToRgb(pixelColor);
    }
}

static inline void gba_ppu_drawMode0() {
    gba_ppu_sortLayers();

    gba_io_register_t *dispcnt = gba_io_getRegister(0x04000000);

    for(int i = 0; i < 4; i++) {
        if(dispcnt->value & (1 << (8 + i))) {
            gba_ppu_drawLayer(i);
        }
    }
}

static inline void gba_ppu_drawMode1() {
    gba_ppu_drawMode0();
}

static inline void gba_ppu_drawMode2() {
    gba_ppu_drawMode0();
}

static inline void gba_ppu_drawMode3() {
    for(int x = 0; x < GBA_SCREEN_WIDTH; x++) {
        gba_ppu_frameBuffer[gba_ppu_currentRow * GBA_SCREEN_WIDTH + x] = gba_ppu_colorToRgb(gba_ppu_vram_read16(0x06000000 | ((gba_ppu_currentRow * GBA_SCREEN_WIDTH + x) << 1)));
    }
}

static inline void gba_ppu_drawMode4() {
    gba_io_register_t *dispcnt = gba_io_getRegister(0x04000000);

    uint32_t offset = (dispcnt->value & (1 << 4)) ? 0x0000a000 : 0x00000000;

    for(int x = 0; x < GBA_SCREEN_WIDTH; x++) {
        gba_ppu_frameBuffer[gba_ppu_currentRow * GBA_SCREEN_WIDTH + x] = gba_ppu_colorToRgb(gba_ppu_getPaletteColor(gba_ppu_vram[gba_ppu_currentRow * GBA_SCREEN_WIDTH + x + offset]));
    }
}

static inline void gba_ppu_drawMode5() {
    gba_io_register_t *dispcnt = gba_io_getRegister(0x04000000);

    uint32_t offset = (dispcnt->value & (1 << 4)) ? 0x00005000 : 0x00000000;

    unsigned int currentRow = gba_ppu_currentRow - 16;

    if(currentRow < 128) {
        for(int x = 0; x < 160; x++) {
            gba_ppu_frameBuffer[currentRow * GBA_SCREEN_WIDTH + x + 40] = gba_ppu_colorToRgb(gba_ppu_vram_read16(0x06000000 | ((currentRow * 160 + x + offset) << 1)));
        }
    }
}

static inline void gba_ppu_drawLine() {
    gba_io_register_t *dispcnt = gba_io_getRegister(0x04000000);

    switch(dispcnt->value & 0x0007) {
        case 0: gba_ppu_drawMode0(); break;
        case 1: gba_ppu_drawMode1(); break;
        case 2: gba_ppu_drawMode2(); break;
        case 3: gba_ppu_drawMode3(); break;
        case 4: gba_ppu_drawMode4(); break;
        case 5: gba_ppu_drawMode5(); break;
        default: break;
    }
}

static inline void gba_ppu_onVblank() {
    gba_dma_onVblank();
    frontend_frame(gba_ppu_frameBuffer);
    gba_onFrame();
}

static inline void gba_ppu_onHblank() {
    gba_dma_onHblank();
}
