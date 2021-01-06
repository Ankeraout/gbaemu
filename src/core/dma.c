#include <stdbool.h>
#include <stdint.h>

#include "platform.h"
#include "core/bus.h"
#include "core/gba.h"
#include "core/io.h"

typedef enum {
    GBA_DMA_CHANNEL_DAC_INCREMENT,
    GBA_DMA_CHANNEL_DAC_DECREMENT,
    GBA_DMA_CHANNEL_DAC_FIXED,
    GBA_DMA_CHANNEL_DAC_INCREMENT_RELOAD
} gba_dma_channel_destinationAddressControl_t;

typedef enum {
    GBA_DMA_CHANNEL_SAC_INCREMENT,
    GBA_DMA_CHANNEL_SAC_DECREMENT,
    GBA_DMA_CHANNEL_SAC_FIXED,
    GBA_DMA_CHANNEL_SAC_PROHIBITED
} gba_dma_channel_sourceAddressControl_t;

typedef enum {
    GBA_DMA_CHANNEL_STARTTIMING_IMMEDIATELY,
    GBA_DMA_CHANNEL_STARTTIMING_VBLANK,
    GBA_DMA_CHANNEL_STARTTIMING_HBLANK,
    GBA_DMA_CHANNEL_STARTTIMING_SPECIAL
} gba_dma_channel_startTiming_t;

typedef struct {
    int index;
    uint32_t iobase;
    uint32_t sourceAddress;
    uint32_t destinationAddress;
    uint32_t wordCount;
    gba_dma_channel_destinationAddressControl_t destinationAddressControl;
    gba_dma_channel_sourceAddressControl_t sourceAddressControl;
    bool repeat;
    bool bitWidth;
    bool gamePakDRQ;
    gba_dma_channel_startTiming_t startTiming;
    bool irq;
    bool enabled;
    bool running;
} gba_dma_channel_t;

gba_dma_channel_t gba_dma_channels[4];

void gba_dma_reset();
bool gba_dma_cycle();
static inline void gba_dma_writeCallback_cntH(gba_dma_channel_t *channel, uint16_t value);
void gba_dma_writeCallback_cntH0(uint32_t address, uint16_t value);
void gba_dma_writeCallback_cntH1(uint32_t address, uint16_t value);
void gba_dma_writeCallback_cntH2(uint32_t address, uint16_t value);
void gba_dma_writeCallback_cntH3(uint32_t address, uint16_t value);
static inline void gba_dma_channel_init(gba_dma_channel_t *channel, int index);
static inline bool gba_dma_channel_cycle(gba_dma_channel_t *channel);
static inline void gba_dma_channel_finish(gba_dma_channel_t *channel);
static inline void gba_dma_channel_repeat(gba_dma_channel_t *channel);
static inline void gba_dma_channel_reloadRegisters(gba_dma_channel_t *channel, bool repeat);
static inline void gba_dma_channel_reloadSourceAddress(gba_dma_channel_t *channel);
static inline void gba_dma_channel_reloadDestinationAddress(gba_dma_channel_t *channel);
static inline void gba_dma_channel_reloadWordCount(gba_dma_channel_t *channel);
void gba_dma_onVblank();
void gba_dma_onHblank();

void gba_dma_reset() {
    for(int i = 0; i < 4; i++) {
        gba_dma_channel_init(&gba_dma_channels[i], i);
    }
}

bool gba_dma_cycle() {
    for(int i = 0; i < 4; i++) {
        if(gba_dma_channel_cycle(&gba_dma_channels[i])) {
            return true;
        }
    }

    return false;
}

static inline void gba_dma_writeCallback_cntH(gba_dma_channel_t *channel, uint16_t value) {
    bool oldEnabled = channel->enabled;

    channel->destinationAddressControl = (value & 0x0060) >> 5;
    channel->sourceAddressControl = (value & 0x0180) >> 7;
    channel->repeat = (value & (1 << 9)) != 0;
    channel->bitWidth = (value & (1 << 10)) != 0;
    channel->gamePakDRQ = (value & (1 << 11)) != 0;
    channel->startTiming = (value & (1 << 12)) != 0;
    channel->irq = (value & (1 << 14)) != 0;
    channel->enabled = (value & (1 << 15)) != 0;

    if(oldEnabled && !channel->enabled) {
        channel->running = false;
    } else {
        gba_dma_channel_reloadRegisters(channel, false);

        if(channel->startTiming == GBA_DMA_CHANNEL_STARTTIMING_IMMEDIATELY) {
            channel->running = true;
        }
    }
}

void gba_dma_writeCallback_cntH0(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_dma_writeCallback_cntH(&gba_dma_channels[0], value);
}

void gba_dma_writeCallback_cntH1(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_dma_writeCallback_cntH(&gba_dma_channels[1], value);
}

void gba_dma_writeCallback_cntH2(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_dma_writeCallback_cntH(&gba_dma_channels[2], value);
}

void gba_dma_writeCallback_cntH3(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_dma_writeCallback_cntH(&gba_dma_channels[3], value);
}

static inline void gba_dma_channel_init(gba_dma_channel_t *channel, int index) {
    channel->index = index;
    channel->iobase = 0x040000b0 + ((uint32_t)index * 12);
}

static inline bool gba_dma_channel_cycle(gba_dma_channel_t *channel) {
    if(channel->running) {
        if(channel->bitWidth) {
            gba_bus_write32(channel->destinationAddress, gba_bus_read32(channel->sourceAddress));

            switch(channel->destinationAddressControl) {
                case GBA_DMA_CHANNEL_DAC_INCREMENT: channel->destinationAddress += 4; break;
                case GBA_DMA_CHANNEL_DAC_DECREMENT: channel->destinationAddress -= 4; break;
                case GBA_DMA_CHANNEL_DAC_FIXED: break;
                case GBA_DMA_CHANNEL_DAC_INCREMENT_RELOAD: channel->destinationAddress += 4; break;
            }

            switch(channel->sourceAddressControl) {
                case GBA_DMA_CHANNEL_SAC_INCREMENT: channel->sourceAddress += 4; break;
                case GBA_DMA_CHANNEL_SAC_DECREMENT: channel->sourceAddress -= 4; break;
                case GBA_DMA_CHANNEL_SAC_FIXED: break;
                case GBA_DMA_CHANNEL_SAC_PROHIBITED: channel->sourceAddress += 4; break;
            }
        } else {
            gba_bus_write16(channel->destinationAddress, gba_bus_read16(channel->sourceAddress));

            switch(channel->destinationAddressControl) {
                case GBA_DMA_CHANNEL_DAC_INCREMENT: channel->destinationAddress += 2; break;
                case GBA_DMA_CHANNEL_DAC_DECREMENT: channel->destinationAddress -= 2; break;
                case GBA_DMA_CHANNEL_DAC_FIXED: break;
                case GBA_DMA_CHANNEL_DAC_INCREMENT_RELOAD: channel->destinationAddress += 2; break;
            }

            switch(channel->sourceAddressControl) {
                case GBA_DMA_CHANNEL_SAC_INCREMENT: channel->sourceAddress += 2; break;
                case GBA_DMA_CHANNEL_SAC_DECREMENT: channel->sourceAddress -= 2; break;
                case GBA_DMA_CHANNEL_SAC_FIXED: break;
                case GBA_DMA_CHANNEL_SAC_PROHIBITED: channel->sourceAddress += 2; break;
            }
        }

        channel->wordCount--;

        if(channel->wordCount == 0) {
            gba_dma_channel_finish(channel);
        }

        return true;
    }

    return false;
}

static inline void gba_dma_channel_finish(gba_dma_channel_t *channel) {
    channel->running = false;

    if(channel->repeat) {
        gba_dma_channel_repeat(channel);

        if(channel->startTiming == GBA_DMA_CHANNEL_STARTTIMING_IMMEDIATELY) {
            channel->running = true;
        }
    } else {
        channel->enabled = false;
        gba_io_getRegister(channel->iobase + 10)->value &= 0x7fff;
    }

    if(channel->irq) {
        gba_setInterruptFlag(1 << (channel->index + 8));
    }
}

static inline void gba_dma_channel_repeat(gba_dma_channel_t *channel) {
    gba_dma_channel_reloadRegisters(channel, true);
}

static inline void gba_dma_channel_reloadRegisters(gba_dma_channel_t *channel, bool repeat) {
    if(repeat) {
        if(channel->destinationAddressControl == GBA_DMA_CHANNEL_DAC_INCREMENT_RELOAD) {
            gba_dma_channel_reloadDestinationAddress(channel);
        }

        gba_dma_channel_reloadWordCount(channel);
    } else {
        gba_dma_channel_reloadSourceAddress(channel);
        gba_dma_channel_reloadDestinationAddress(channel);
        gba_dma_channel_reloadWordCount(channel);
    }
}

static inline void gba_dma_channel_reloadSourceAddress(gba_dma_channel_t *channel) {
    channel->sourceAddress = (uint32_t)gba_io_getRegister(channel->iobase)->value | (uint32_t)(gba_io_getRegister(channel->iobase + 2)->value << 16);
}

static inline void gba_dma_channel_reloadDestinationAddress(gba_dma_channel_t *channel) {
    channel->destinationAddress = (uint32_t)gba_io_getRegister(channel->iobase + 4)->value | (uint32_t)(gba_io_getRegister(channel->iobase + 6)->value << 16);
}

static inline void gba_dma_channel_reloadWordCount(gba_dma_channel_t *channel) {
    channel->wordCount = gba_io_getRegister(channel->iobase + 8)->value;

    if(channel->wordCount == 0) {
        if(channel->iobase == 0x040000d4) {
            channel->wordCount = 0x10000;
        } else {
            channel->wordCount = 0x4000;
        }
    }
}

void gba_dma_onVblank() {
    for(int i = 0; i < 4; i++) {
        if(gba_dma_channels[i].enabled && !gba_dma_channels[i].running && gba_dma_channels[i].startTiming == GBA_DMA_CHANNEL_STARTTIMING_VBLANK) {
            gba_dma_channels[i].running = true;
        }
    }
}

void gba_dma_onHblank() {
    for(int i = 0; i < 4; i++) {
        if(gba_dma_channels[i].enabled && !gba_dma_channels[i].running && gba_dma_channels[i].startTiming == GBA_DMA_CHANNEL_STARTTIMING_HBLANK) {
            gba_dma_channels[i].running = true;
        }
    }
}
