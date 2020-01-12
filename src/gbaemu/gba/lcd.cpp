#include <cstdint>
#include <cstdio>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/io.hpp>
#include <gbaemu/gba/lcd.hpp>

namespace gbaemu::gba::lcd {
    uint8_t palette[1024];
    uint8_t vram[98304];
    uint8_t oam[1024];

    unsigned int currentRow = 0;
    unsigned int currentColumn = 0;
    uint_fast32_t cycleCounter = 0;

    void writeCallback_dispcnt(uint16_t value) {
        printf("Wrote %04x to DISPCNT\n", value);
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
                }
            } else if(currentColumn == 240) {
                // HBlank
                stat.fields.hblank = 1;

                if(stat.fields.irq_hblank) {
                    io::set(io::IF, io::get(io::IF) | cpu::IRQ_HBLANK);
                }
            }

            io::set(io::DISPSTAT, stat.value);
        }

        cycleCounter++;
    }
}