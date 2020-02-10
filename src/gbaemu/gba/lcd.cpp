#include <cstdint>
#include <cstdio>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/io.hpp>
#include <gbaemu/gba/lcd.hpp>

#include <gbaemu/frontend.hpp>

#define access16(t, i) (*((uint16_t *)&t[i]))

namespace gbaemu::gba::lcd {
    uint8_t palette[1024];
    uint8_t vram[98304];
    uint8_t oam[1024];
    uint32_t frameBuffer[screenWidth * screenHeight];

    unsigned int currentRow = 0;
    unsigned int currentColumn = 0;
    uint_fast32_t cycleCounter = 0;

    void writeCallback_dispcnt(uint16_t value) {
        printf("Wrote %04x to DISPCNT\n", value);
    }

    void sortLayersByPriority(int *layers) {
        layers[0] = 3;
        layers[1] = 2;
        layers[2] = 1;
        layers[3] = 0;

        for(int i = 0; i < 3; i++) {
            int layerPriority = io::get(io::BG0CNT + 2 * i);
            int maxPriority = layerPriority;
            int maxPriorityIndex = i;

            for(int j = i + 1; j < 4; j++) {
                int currentLayerPriority = io::get(io::BG0CNT + 2 * j);
                
                if(currentLayerPriority > maxPriority) {
                    maxPriorityIndex = j;
                    maxPriority = currentLayerPriority;
                }
            }

            int exchange = layers[i];
            layers[i] = layers[maxPriorityIndex];
            layers[maxPriorityIndex] = exchange;
        }
    }
    
    static inline void drawLayer(int layer) {
        bgcnt_t bgcnt;
        bgcnt.value = io::get(io::BG0CNT + 2 * layer);
        int bghofs = io::get(io::BG0HOFS + 2 * layer);
        int bgvofs = io::get(io::BG0VOFS + 2 * layer);
        int mapBase = bgcnt.fields.mapBaseAddress << 11;
        int tileBase = bgcnt.fields.tileBaseAddress << 14;
        int yLayer = currentRow + bgvofs;

        if(bgcnt.fields.screenSize & 2) {
            yLayer &= 0x01ff;
        } else {
            yLayer &= 0x00ff;
        }

        int yMapShift;

        if(bgcnt.fields.screenSize & 1) {
            yMapShift = 7;
        } else {
            yMapShift = 6;
        }

        int yMap = yLayer >> 3;
        int yTile = yLayer & 7;
        int xLayer = bghofs;
        int mapTileShift = 5 + bgcnt.fields.colors;
        int mapValueMask = bgcnt.fields.colors ? 0x01ff : 0x03ff;
        int mapTileYshift = bgcnt.fields.colors ? 3 : 2;
        int mapTileXshift = bgcnt.fields.colors ? 0 : 1;

        for(int x = 0; x < 240; x++) {
            if(bgcnt.fields.screenSize & 1) {
                xLayer &= 0x01ff;
            } else {
                xLayer &= 0x00ff;
            }

            int xMap = xLayer >> 3;
            int xTile = xLayer & 7;
            int mapValue = access16(vram, mapBase + (yMap << yMapShift) + (xMap << 1)) & mapValueMask;
            int tileValueAddress = (tileBase + (mapValue << mapTileShift) + (yTile << mapTileYshift) + (xTile >> mapTileXshift));
            int tileValue = vram[tileValueAddress];

            if(!bgcnt.fields.colors) {
                if(xTile & 1) {
                    tileValue >>= 4;
                } else {
                    tileValue &= 0x0f;
                }
            }

            frameBuffer[currentRow * screenWidth + x] = getPaletteColor(tileValue);

            xLayer++;
        }
    }

    static inline void drawMode0() {
        int layers[3];
        dispcnt_t dispcnt;

        sortLayersByPriority(layers);
        dispcnt.value = io::get(io::DISPCNT);

        for(int i = 0; i < 4; i++) {
            if(dispcnt.value & (1 << (8 + i))) {
                drawLayer(i);
            }
        }
    }

    static inline void drawMode1() {
        drawMode0();
    }

    static inline void drawMode2() {
        drawMode0();
    }

    static inline void drawMode3() {
        for(unsigned int x = 0; x < 240; x++) {
            frameBuffer[currentRow * screenWidth + x] = colorToRgb(((uint16_t *)vram)[currentRow * screenWidth + x]);
        }
    }

    static inline void drawMode4() {
        dispcnt_t dispcnt;
        dispcnt.value = io::get(io::DISPCNT);

        uint32_t offset = dispcnt.fields.displayFrameSelect ? 0x0000a000 : 0x00000000;

        for(unsigned int x = 0; x < 240; x++) {
            frameBuffer[currentRow * screenWidth + x] = getPaletteColor(vram[currentRow * screenWidth + x + offset]);
        }
    }

    static inline void drawMode5() {
        dispcnt_t dispcnt;
        dispcnt.value = io::get(io::DISPCNT);

        uint32_t offset = dispcnt.fields.displayFrameSelect ? 0x00005000 : 0x00000000;

        if(currentRow < 128) {
            for(unsigned int x = 0; x < 160; x++) {
                frameBuffer[currentRow * screenWidth + x] = colorToRgb(((uint16_t *)vram)[currentRow * 160 + x + offset]);
            }
        }
    }

    void drawLine() {
        dispcnt_t dispcnt;
        dispcnt.value = io::get(io::DISPCNT);

        switch(dispcnt.fields.bgMode) {
            case 0: drawMode0(); break;
            case 1: drawMode1(); break;
            case 2: drawMode2(); break;
            case 3: drawMode3(); break;
            case 4: drawMode4(); break;
            case 5: drawMode5(); break;

            default:
            break;
        }
    }

    void cycle() {
        if((cycleCounter & 0x03) == 0x03) {
            dispstat_t stat;
            stat.value = io::get(io::DISPSTAT);

            currentColumn++;

            if(currentColumn == 308) {
                currentColumn = 0;
                currentRow++;
                
                stat.fields.hblank = 0;
                stat.fields.vcounter = (currentRow == stat.fields.lyc);

                if(stat.fields.irq_vcounter) {
                    io::set(io::IF, io::get(io::IF) | cpu::IRQ_VCOUNTER);
                }

                if(currentRow == 228) {
                    currentRow = 0;
                    cycleCounter = 0;
                    stat.fields.vblank = 0;
                } else if(currentRow == 160) {
                    // VBlank
                    stat.fields.vblank = 1;

                    if(stat.fields.irq_vblank) {
                        io::set(io::IF, io::get(io::IF) | cpu::IRQ_VBLANK);
                    }

                    gbaemu::frontend::update();
                }

                io::set(io::VCOUNT, currentRow);
            } else if(currentColumn == 240) {
                // HBlank
                stat.fields.hblank = 1;

                if(stat.fields.irq_hblank) {
                    io::set(io::IF, io::get(io::IF) | cpu::IRQ_HBLANK);
                }

                if(currentRow < 160) {
                    drawLine();
                }
            }

            io::set(io::DISPSTAT, stat.value);
        }

        cycleCounter++;
    }

    const uint32_t *getFramebuffer() {
        return frameBuffer;
    }
}
