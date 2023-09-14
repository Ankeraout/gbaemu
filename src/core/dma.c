#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "core/bus.h"
#include "core/dma.h"
#include "core/gpu.h"

enum te_dmaStartTiming {
    E_DMASTARTTIMING_IMMEDIATELY,
    E_DMASTARTTIMING_VBLANK,
    E_DMASTARTTIMING_HBLANK,
    E_DMASTARTTIMING_SPECIAL
};

struct ts_dmaChannel {
    uint32_t registerSourceAddress;
    uint32_t registerDestinationAddress;
    uint16_t registerCount;
    bool enable;
    bool running;
    bool repeat;
    bool is32bit;
    bool enableIrq;
    bool reloadDestination;
    enum te_dmaStartTiming startTiming;
    uint32_t currentSourceAddress;
    uint32_t currentDestinationAddress;
    uint32_t currentCount;
    uint32_t sourceAddressIncrement;
    uint32_t destinationAddressIncrement;
};

static struct ts_dmaChannel s_dmaChannels[4];
static bool s_dmaIsRunning;
static bool s_dmaTransferring;

static inline uint16_t dmaReadControl(int p_index);
static inline bool dmaCheckRunCondition(int p_index);
static inline void dmaStartTransfer(int p_index);
static inline void dmaDoTransfer(int p_index);
static inline void dmaCheckTransferFinished(int p_index);
static inline void dmaReloadCounter(int p_index);
static inline void dmaSetSourceAddressLow(int p_index, uint16_t p_value);
static inline void dmaSetSourceAddressHigh(int p_index, uint16_t p_value);
static inline void dmaSetDestinationAddressLow(int p_index, uint16_t p_value);
static inline void dmaSetDestinationAddressHigh(int p_index, uint16_t p_value);
static inline void dmaWriteControl(int p_index, uint16_t p_value);

void dmaReset(void) {
    memset(s_dmaChannels, 0, sizeof(s_dmaChannels));
    s_dmaTransferring = false;
    s_dmaIsRunning = false;
}

void dmaCycle(void) {
    if(s_dmaTransferring) {
        return;
    }

    for(int l_channel = 0; l_channel < 4; l_channel++) {
        if(!s_dmaChannels[l_channel].enable) {
            s_dmaIsRunning = false;
            continue;
        }

        s_dmaIsRunning = dmaCheckRunCondition(l_channel);

        if(s_dmaIsRunning) {
            dmaDoTransfer(l_channel);
            return;
        }
    }
}

uint16_t dmaIoRead16(uint32_t p_address) {
    switch(p_address) {
        case C_IOADDRESS_DMA0SAD_L: return s_dmaChannels[0].registerSourceAddress;
        case C_IOADDRESS_DMA1SAD_L: return s_dmaChannels[1].registerSourceAddress;
        case C_IOADDRESS_DMA2SAD_L: return s_dmaChannels[2].registerSourceAddress;
        case C_IOADDRESS_DMA3SAD_L: return s_dmaChannels[3].registerSourceAddress;
        case C_IOADDRESS_DMA0SAD_H: return s_dmaChannels[0].registerSourceAddress >> 16;
        case C_IOADDRESS_DMA1SAD_H: return s_dmaChannels[1].registerSourceAddress >> 16;
        case C_IOADDRESS_DMA2SAD_H: return s_dmaChannels[2].registerSourceAddress >> 16;
        case C_IOADDRESS_DMA3SAD_H: return s_dmaChannels[3].registerSourceAddress >> 16;
        case C_IOADDRESS_DMA0DAD_L: return s_dmaChannels[0].registerDestinationAddress;
        case C_IOADDRESS_DMA1DAD_L: return s_dmaChannels[1].registerDestinationAddress;
        case C_IOADDRESS_DMA2DAD_L: return s_dmaChannels[2].registerDestinationAddress;
        case C_IOADDRESS_DMA3DAD_L: return s_dmaChannels[3].registerDestinationAddress;
        case C_IOADDRESS_DMA0DAD_H: return s_dmaChannels[0].registerDestinationAddress >> 16;
        case C_IOADDRESS_DMA1DAD_H: return s_dmaChannels[1].registerDestinationAddress >> 16;
        case C_IOADDRESS_DMA2DAD_H: return s_dmaChannels[2].registerDestinationAddress >> 16;
        case C_IOADDRESS_DMA3DAD_H: return s_dmaChannels[3].registerDestinationAddress >> 16;
        case C_IOADDRESS_DMA0CNT_L: return s_dmaChannels[0].registerCount;
        case C_IOADDRESS_DMA1CNT_L: return s_dmaChannels[1].registerCount;
        case C_IOADDRESS_DMA2CNT_L: return s_dmaChannels[2].registerCount;
        case C_IOADDRESS_DMA3CNT_L: return s_dmaChannels[3].registerCount;
        case C_IOADDRESS_DMA0CNT_H: return dmaReadControl(0);
        case C_IOADDRESS_DMA1CNT_H: return dmaReadControl(1);
        case C_IOADDRESS_DMA2CNT_H: return dmaReadControl(2);
        case C_IOADDRESS_DMA3CNT_H: return dmaReadControl(3);
    }

    return 0xffff;
}

void dmaIoWrite16(uint32_t p_address, uint16_t p_value) {
    switch(p_address) {
        case C_IOADDRESS_DMA0SAD_L: dmaSetSourceAddressLow(0, p_value); break;
        case C_IOADDRESS_DMA1SAD_L: dmaSetSourceAddressLow(1, p_value); break;
        case C_IOADDRESS_DMA2SAD_L: dmaSetSourceAddressLow(2, p_value); break;
        case C_IOADDRESS_DMA3SAD_L: dmaSetSourceAddressLow(3, p_value); break;
        case C_IOADDRESS_DMA0SAD_H: dmaSetSourceAddressHigh(0, p_value); break;
        case C_IOADDRESS_DMA1SAD_H: dmaSetSourceAddressHigh(1, p_value); break;
        case C_IOADDRESS_DMA2SAD_H: dmaSetSourceAddressHigh(2, p_value); break;
        case C_IOADDRESS_DMA3SAD_H: dmaSetSourceAddressHigh(3, p_value); break;
        case C_IOADDRESS_DMA0DAD_L: dmaSetDestinationAddressLow(0, p_value); break;
        case C_IOADDRESS_DMA1DAD_L: dmaSetDestinationAddressLow(1, p_value); break;
        case C_IOADDRESS_DMA2DAD_L: dmaSetDestinationAddressLow(2, p_value); break;
        case C_IOADDRESS_DMA3DAD_L: dmaSetDestinationAddressLow(3, p_value); break;
        case C_IOADDRESS_DMA0DAD_H: dmaSetDestinationAddressHigh(0, p_value); break;
        case C_IOADDRESS_DMA1DAD_H: dmaSetDestinationAddressHigh(1, p_value); break;
        case C_IOADDRESS_DMA2DAD_H: dmaSetDestinationAddressHigh(2, p_value); break;
        case C_IOADDRESS_DMA3DAD_H: dmaSetDestinationAddressHigh(3, p_value); break;
        case C_IOADDRESS_DMA0CNT_L: s_dmaChannels[0].registerCount = p_value; break;
        case C_IOADDRESS_DMA1CNT_L: s_dmaChannels[1].registerCount = p_value; break;
        case C_IOADDRESS_DMA2CNT_L: s_dmaChannels[2].registerCount = p_value; break;
        case C_IOADDRESS_DMA3CNT_L: s_dmaChannels[3].registerCount = p_value; break;
        case C_IOADDRESS_DMA0CNT_H: dmaWriteControl(0, p_value); break;
        case C_IOADDRESS_DMA1CNT_H: dmaWriteControl(1, p_value); break;
        case C_IOADDRESS_DMA2CNT_H: dmaWriteControl(2, p_value); break;
        case C_IOADDRESS_DMA3CNT_H: dmaWriteControl(3, p_value); break;
    }
}

bool dmaIsRunning(void) {
    return s_dmaIsRunning;
}

static inline uint16_t dmaReadControl(int p_index) {
    uint16_t l_value = 0;

    switch(s_dmaChannels[p_index].sourceAddressIncrement) {
        case (uint32_t)-1: l_value |= (1 << 5); break;
        case 0: l_value |= (2 << 5); break;
    }

    switch(s_dmaChannels[p_index].destinationAddressIncrement) {
        case (uint32_t)-1: l_value |= (1 << 7); break;
        case 0: l_value |= (2 << 7); break;
        case 1: l_value |= s_dmaChannels[p_index].reloadDestination ? (3 << 7) : 0; break;
    }

    if(s_dmaChannels[p_index].repeat) {
        l_value |= 1 << 9;
    }

    if(s_dmaChannels[p_index].is32bit) {
        l_value |= 1 << 10;
    }

    if(s_dmaChannels[p_index].enableIrq) {
        l_value |= 1 << 14;
    }

    if(s_dmaChannels[p_index].enable) {
        l_value |= 1 << 15;
    }

    return l_value;
}

static inline bool dmaCheckRunCondition(int p_index) {
    if(s_dmaChannels[p_index].running) {
        return true;
    }

    bool l_startCondition;

    switch(s_dmaChannels[p_index].startTiming) {
        case E_DMASTARTTIMING_IMMEDIATELY:
            l_startCondition = true;
            break;

        case E_DMASTARTTIMING_VBLANK:
            l_startCondition = gpuIsVBlank();
            break;

        case E_DMASTARTTIMING_HBLANK:
            l_startCondition = gpuIsHBlank();
            break;

        case E_DMASTARTTIMING_SPECIAL:
            // TODO: implement DMA special starts
            l_startCondition = false;
            break;
    }

    if(l_startCondition) {
        dmaStartTransfer(p_index);
    }

    return l_startCondition;
}

static inline void dmaStartTransfer(int p_index) {
    s_dmaChannels[p_index].running = true;
}

static inline void dmaDoTransfer(int p_index) {
    s_dmaTransferring = true;

    if(s_dmaChannels[p_index].is32bit) {
        busWrite32(s_dmaChannels[p_index].currentDestinationAddress, busRead32(s_dmaChannels[p_index].currentSourceAddress));
    } else {
        busWrite16(s_dmaChannels[p_index].currentDestinationAddress, busRead16(s_dmaChannels[p_index].currentSourceAddress));
    }

    s_dmaTransferring = false;

    s_dmaChannels[p_index].currentSourceAddress += s_dmaChannels[p_index].sourceAddressIncrement;
    s_dmaChannels[p_index].currentDestinationAddress += s_dmaChannels[p_index].destinationAddressIncrement;
    s_dmaChannels[p_index].currentCount--;

    dmaCheckTransferFinished(p_index);
}

static inline void dmaCheckTransferFinished(int p_index) {
    if(s_dmaChannels[p_index].currentCount != 0) {
        return;
    }

    // Check reload
    if(s_dmaChannels[p_index].repeat) {
        dmaReloadCounter(p_index);

        if(s_dmaChannels[p_index].reloadDestination) {
            s_dmaChannels[p_index].currentDestinationAddress = s_dmaChannels[p_index].registerDestinationAddress;
        }
    } else {
        s_dmaChannels[p_index].enable = false;
    }
}

static inline void dmaReloadCounter(int p_index) {
    if(s_dmaChannels[p_index].registerCount == 0) {
        s_dmaChannels[p_index].currentCount = p_index == 3 ? 0x10000 : 0x4000;
    } else {
        s_dmaChannels[p_index].currentCount = s_dmaChannels[p_index].registerCount;
        s_dmaChannels[p_index].currentCount &= p_index == 3 ? 0xffff : 0x3fff;
    }
}

static inline void dmaSetSourceAddressLow(int p_index, uint16_t p_value) {
    s_dmaChannels[p_index].registerSourceAddress &= 0xffff0000;
    s_dmaChannels[p_index].registerSourceAddress |= p_value;
}

static inline void dmaSetSourceAddressHigh(int p_index, uint16_t p_value) {
    s_dmaChannels[p_index].registerSourceAddress &= 0xffff;
    s_dmaChannels[p_index].registerSourceAddress |= p_value << 16;
}

static inline void dmaSetDestinationAddressLow(int p_index, uint16_t p_value) {
    s_dmaChannels[p_index].registerDestinationAddress &= 0xffff0000;
    s_dmaChannels[p_index].registerDestinationAddress |= p_value;
}

static inline void dmaSetDestinationAddressHigh(int p_index, uint16_t p_value) {
    s_dmaChannels[p_index].registerDestinationAddress &= 0xffff;
    s_dmaChannels[p_index].registerDestinationAddress |= p_value << 16;
}

static inline void dmaWriteControl(int p_index, uint16_t p_value) {
    const bool l_is32bit = (p_value & (1 << 10)) != 0;
    const bool l_enable = (p_value & (1 << 15)) != 0;

    switch((p_value >> 5) & 3) {
        case 0: s_dmaChannels[p_index].destinationAddressIncrement = l_is32bit ? 4 : 2; break;
        case 1: s_dmaChannels[p_index].destinationAddressIncrement = l_is32bit ? -4 : -2; break;
        case 2: s_dmaChannels[p_index].destinationAddressIncrement = 0; break;
        case 3: s_dmaChannels[p_index].destinationAddressIncrement = l_is32bit ? 4 : 2; break;
    }

    switch((p_value >> 7) & 3) {
        case 0: s_dmaChannels[p_index].sourceAddressIncrement = l_is32bit ? 4 : 2; break;
        case 1: s_dmaChannels[p_index].sourceAddressIncrement = l_is32bit ? -4 : -2; break;
        case 2: s_dmaChannels[p_index].sourceAddressIncrement = 0; break;
    }

    s_dmaChannels[p_index].repeat = (p_value & (1 << 9)) != 0;
    s_dmaChannels[p_index].is32bit = l_is32bit;
    s_dmaChannels[p_index].startTiming = (p_value >> 12) & 3;
    s_dmaChannels[p_index].enableIrq = (p_value & (1 << 14)) != 0;

    if(!s_dmaChannels[p_index].enable && l_enable) {
        s_dmaChannels[p_index].currentSourceAddress = s_dmaChannels[p_index].registerSourceAddress;
        s_dmaChannels[p_index].currentDestinationAddress = s_dmaChannels[p_index].registerDestinationAddress;
        dmaReloadCounter(p_index);
    }

    s_dmaChannels[p_index].enable = l_enable;
}
