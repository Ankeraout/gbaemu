#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "core/gba.h"
#include "core/io.h"

struct gba_timer_channel_s;

typedef struct gba_timer_channel_s {
    struct gba_timer_channel_s *nextChannel;
    uint16_t irqFlag;
    int index;
    uint16_t reloadValue;
    uint32_t prescaler;
    bool countUp;
    bool irq;
    bool operate;
    uint16_t counter;
} gba_timer_channel_t;

gba_timer_channel_t gba_timer_channels[4];
uint_least32_t gba_timer_cycleCounter;

void gba_timer_reset();
void gba_timer_cycle();
static inline void gba_timer_channel_init(gba_timer_channel_t *channel, gba_timer_channel_t *nextChannel, int index);
static inline void gba_timer_channel_cycle(gba_timer_channel_t *channel, uint_least32_t cycleCounter);
static inline void gba_timer_channel_increment(gba_timer_channel_t *channel);
static inline void gba_timer_channel_lastOverflowed(gba_timer_channel_t *channel);
static inline void gba_timer_writeCallback_channel_reload(int index, uint16_t value);
static inline void gba_timer_writeCallback_channel_control(int index, uint16_t value);
void gba_timer_writeCallback_channel0_reload(uint32_t address, uint16_t value);
void gba_timer_writeCallback_channel0_control(uint32_t address, uint16_t value);
void gba_timer_writeCallback_channel1_reload(uint32_t address, uint16_t value);
void gba_timer_writeCallback_channel1_control(uint32_t address, uint16_t value);
void gba_timer_writeCallback_channel2_reload(uint32_t address, uint16_t value);
void gba_timer_writeCallback_channel2_control(uint32_t address, uint16_t value);
void gba_timer_writeCallback_channel3_reload(uint32_t address, uint16_t value);
void gba_timer_writeCallback_channel3_control(uint32_t address, uint16_t value);

void gba_timer_reset() {
    gba_timer_channel_init(&gba_timer_channels[3], NULL, 3);
    gba_timer_channel_init(&gba_timer_channels[2], &gba_timer_channels[3], 2);
    gba_timer_channel_init(&gba_timer_channels[1], &gba_timer_channels[2], 1);
    gba_timer_channel_init(&gba_timer_channels[0], &gba_timer_channels[1], 0);
}

void gba_timer_cycle() {
    for(int i = 0; i < 4; i++) {
        gba_timer_channel_cycle(&gba_timer_channels[i], gba_timer_cycleCounter);
    }

    gba_timer_cycleCounter++;
}

static inline void gba_timer_channel_init(gba_timer_channel_t *channel, gba_timer_channel_t *nextChannel, int index) {
    channel->nextChannel = nextChannel;
    channel->irqFlag = 1 << (index + 3);
    channel->index = index;
}

static inline void gba_timer_channel_cycle(gba_timer_channel_t *channel, uint_least32_t cycleCounter) {
    if(channel->operate && !channel->countUp) {
        if((cycleCounter & channel->prescaler) == 0) {
            gba_timer_channel_increment(channel);
        }
    }
}

static inline void gba_timer_channel_increment(gba_timer_channel_t *channel) {
    channel->counter++;
    
    if(channel->counter == 0) {
        if(channel->irq) {
            gba_setInterruptFlag(channel->irqFlag);
        }

        if(channel->nextChannel) {
            gba_timer_channel_lastOverflowed(channel->nextChannel);
        }

        channel->counter = channel->reloadValue;
    }
}

static inline void gba_timer_channel_lastOverflowed(gba_timer_channel_t *channel) {
    if(channel->countUp) {
        gba_timer_channel_increment(channel);
    }
}

static inline void gba_timer_writeCallback_channel_reload(int index, uint16_t value) {
    gba_timer_channels[index].reloadValue = value;
}

static inline void gba_timer_writeCallback_channel_control(int index, uint16_t value) {
    switch(value & 0x0003) {
        case 0: gba_timer_channels[index].prescaler = 0; break;
        case 1: gba_timer_channels[index].prescaler = 63; break;
        case 2: gba_timer_channels[index].prescaler = 255; break;
        case 3: gba_timer_channels[index].prescaler = 1023; break;
    }

    if(index == 0) {
        gba_timer_channels->countUp = false;
        gba_io_getRegister(0x04000102)->value &= 0xfffb;
    } else {
        gba_timer_channels->countUp = (value & (1 << 2)) != 0;
    }

    gba_timer_channels[index].irq = (value & (1 << 6)) != 0;
    gba_timer_channels[index].operate = (value & (1 << 7)) != 0;
}

void gba_timer_writeCallback_channel0_reload(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_timer_writeCallback_channel_reload(0, value);
}

void gba_timer_writeCallback_channel0_control(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_timer_writeCallback_channel_control(0, value);
}

void gba_timer_writeCallback_channel1_reload(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_timer_writeCallback_channel_reload(1, value);
}

void gba_timer_writeCallback_channel1_control(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_timer_writeCallback_channel_control(1, value);
}

void gba_timer_writeCallback_channel2_reload(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_timer_writeCallback_channel_reload(2, value);
}

void gba_timer_writeCallback_channel2_control(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_timer_writeCallback_channel_control(2, value);
}

void gba_timer_writeCallback_channel3_reload(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_timer_writeCallback_channel_reload(3, value);
}

void gba_timer_writeCallback_channel3_control(uint32_t address, uint16_t value) {
    UNUSED(address);
    gba_timer_writeCallback_channel_control(3, value);
}
