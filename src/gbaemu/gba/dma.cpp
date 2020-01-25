#include <cstdint>
#include <cstdio>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/dma.hpp>
#include <gbaemu/gba/io.hpp>
#include <gbaemu/gba/lcd.hpp>
#include <gbaemu/gba/mmu.hpp>

#define DEFINE_DMACNT_WRITE_CALLBACK(n) \
    void writeCallback_dma ## n ## cnt(uint16_t value) { \
        writeCallback_dma_base(n, value); \
    }

namespace gbaemu::gba::dma {
    typedef enum {
        DMA_INCREMENT,
        DMA_DECREMENT,
        DMA_FIXED,
        DMA_INCREMENT_AND_RELOAD
    } adc_t;

    typedef enum {
        DMA_START_IMMEDIATELY,
        DMA_START_VBLANK,
        DMA_START_HBLANK,
        DMA_START_SPECIAL
    } startTiming_t;

    typedef struct {
        uint32_t sad;
        uint32_t dad;
        unsigned int cnt;
        int srcOffset;
        int dstOffset;
        unsigned int current;
        adc_t dadc;
        adc_t sadc;
        bool repeat;
        bool mode32;
        bool gamePakDRQ;
        startTiming_t startTiming;
        bool irq;
        bool enable;
        bool running;
    } dma_t;

    dma_t dma[4];

    void init() {
        for(int i = 0; i < 4; i++) {
            dma[i].enable = false;
            dma[i].running = false;
        }
    }

    bool cycle() {
        // DMA start check loop
        for(int i = 0; i < 4; i++) {
            if(dma[i].enable && !dma[i].running) {
                switch(dma[i].startTiming) {
                    case DMA_START_IMMEDIATELY:
                        dma[i].running = true;
                        break;

                    case DMA_START_VBLANK:
                        if(io::get(io::DISPSTAT) & 0x0001) {
                            dma[i].running = true;
                        }

                        break;

                    case DMA_START_HBLANK:
                        if(io::get(io::DISPSTAT) & 0x0002) {
                            dma[i].running = true;
                        }

                        break;

                    case DMA_START_SPECIAL:
                        // TODO
                        break;
                }

                // If the DMA was started
                if(dma[i].running) {
                    dma[i].dstOffset = 0;
                    dma[i].srcOffset = 0;
                    dma[i].current = 0;
                }
            }
        }

        // DMA run loop
        for(int i = 0; i < 4; i++) {
            if(dma[i].running) {
                if(dma[i].mode32) {
                    mmu::write32(dma[i].dad + dma[i].dstOffset, mmu::read32(dma[i].sad + dma[i].srcOffset));
                } else {
                    mmu::write16(dma[i].dad + dma[i].dstOffset, mmu::read16(dma[i].sad + dma[i].srcOffset));
                }

                dma[i].current++;

                switch(dma[i].dadc) {
                    case DMA_INCREMENT:
                        if(dma[i].mode32) {
                            dma[i].dstOffset += 4;
                        } else {
                            dma[i].dstOffset += 2;
                        }

                        break;

                    case DMA_DECREMENT:
                        if(dma[i].mode32) {
                            dma[i].dstOffset -= 4;
                        } else {
                            dma[i].dstOffset -= 2;
                        }

                        break;

                    case DMA_FIXED:
                        break;

                    case DMA_INCREMENT_AND_RELOAD:
                        if(dma[i].mode32) {
                            dma[i].dstOffset += 4;
                        } else {
                            dma[i].dstOffset += 2;
                        }

                        break;
                }

                switch(dma[i].sadc) {
                    case DMA_INCREMENT:
                        if(dma[i].mode32) {
                            dma[i].srcOffset += 4;
                        } else {
                            dma[i].srcOffset += 2;
                        }

                        break;

                    case DMA_DECREMENT:
                        if(dma[i].mode32) {
                            dma[i].srcOffset -= 4;
                        } else {
                            dma[i].srcOffset -= 2;
                        }

                        break;

                    case DMA_FIXED:
                        break;

                    case DMA_INCREMENT_AND_RELOAD:
                        if(dma[i].mode32) {
                            dma[i].srcOffset += 4;
                        } else {
                            dma[i].srcOffset += 2;
                        }

                        break;
                }

                if(dma[i].current == dma[i].cnt) {
                    if(dma[i].repeat) {
                        dma[i].cnt = io::get32(io::DMA0CNT_L + 12 * i);

                        if(dma[i].cnt == 0) {
                            dma[i].cnt = (i == 3) ? 0x00010000 : 0x00004000;
                        }

                        if(dma[i].dadc == DMA_INCREMENT_AND_RELOAD) {
                            dma[i].dad = io::get32(io::DMA0DAD_L + 12 * i) & (i == 3) ? 0x0fffffff : 0x07ffffff;
                        }

                        dma[i].running = false;

                        printf("DMA %d reloaded, SAD=%08x DAD=%08x CNT=%04x\n", i, dma[i].sad, dma[i].dad, dma[i].cnt);
                    } else {
                        dma[i].enable = false;
                        dma[i].running = false;
                    }

                    if(dma[i].irq) {
                        io::set(io::IF, io::get(io::IF) | (0x0100 << i));
                    }
                }

                return true;
            }
        }

        return false;
    }

    static inline void writeCallback_dma_base(int n, uint16_t value) {
        if(!dma[n].enable && (value & 0x8000)) {
            dma[n].sad = io::get32(io::DMA0SAD_L + 12 * n) & ((n == 3) ? 0x0fffffff : 0x07ffffff);
            dma[n].dad = io::get32(io::DMA0DAD_L + 12 * n) & ((n == 3) ? 0x0fffffff : 0x07ffffff);
            dma[n].cnt = io::get(io::DMA0CNT_L + 12 * n);

            if(dma[n].cnt == 0) {
                dma[n].cnt = (n == 3) ? 0x00010000 : 0x00004000;
            }

            dma[n].dadc = (adc_t)((value & 0x0060) >> 5);
            dma[n].sadc = (adc_t)((value & 0x0380) >> 7);
            dma[n].repeat = (value & 0x0200) >> 9;
            dma[n].mode32 = (value & 0x0400) >> 10;
            dma[n].gamePakDRQ = (value & 0x0800) >> 11;
            dma[n].startTiming = (startTiming_t)((value & 0x3000) >> 12);
            dma[n].irq = (value & 0x4000) >> 14;
            dma[n].enable = value >> 15;

            printf("[%08x] DMA %d enabled, SAD=%08x DAD=%08x CNT=%04x\n", cpu::registerRead(cpu::CPU_REG_PC), n, dma[n].sad, dma[n].dad, dma[n].cnt);
        }
    }

    DEFINE_DMACNT_WRITE_CALLBACK(0)
    DEFINE_DMACNT_WRITE_CALLBACK(1)
    DEFINE_DMACNT_WRITE_CALLBACK(2)
    DEFINE_DMACNT_WRITE_CALLBACK(3)
}
