#pragma once

#include <cstdint>

namespace gbaemu::gba::io {
    typedef void writeCallback_t(uint16_t value);

    typedef enum {
        IOREGNONE,
        DISPCNT,
        GREENSWP,
        DISPSTAT,
        VCOUNT,
        BG0CNT,
        BG1CNT,
        BG2CNT,
        BG3CNT,
        BG0HOFS,
        BG0VOFS,
        BG1HOFS,
        BG1VOFS,
        BG2HOFS,
        BG2VOFS,
        BG3HOFS,
        BG3VOFS,
        BG2PA,
        BG2PB,
        BG2PC,
        BG2PD,
        BG2X_L,
        BG2X_H,
        BG2Y_L,
        BG2Y_H,
        BG3PA,
        BG3PB,
        BG3PC,
        BG3PD,
        BG3X_L,
        BG3X_H,
        BG3Y_L,
        BG3Y_H,
        WIN0H,
        WIN1H,
        WIN0V,
        WIN1V,
        WININ,
        WINOUT,
        MOSAIC,
        BLDCNT,
        BLDALPHA,
        BLDY,
        SOUND1CNT_L,
        SOUND1CNT_H,
        SOUND1CNT_X,
        SOUND2CNT_L,
        SOUND2CNT_H,
        SOUND3CNT_L,
        SOUND3CNT_H,
        SOUND3CNT_X,
        SOUND4CNT_L,
        SOUND4CNT_H,
        SOUNDCNT_L,
        SOUNDCNT_H,
        SOUNDCNT_X,
        SOUNDBIAS,
        WAVE_RAM,
        FIFO_A_L,
        FIFO_A_H,
        FIFO_B_L,
        FIFO_B_H,
        DMA0SAD_L,
        DMA0SAD_H,
        DMA0DAD_L,
        DMA0DAD_H,
        DMA0CNT_L,
        DMA0CNT_H,
        DMA1SAD_L,
        DMA1SAD_H,
        DMA1DAD_L,
        DMA1DAD_H,
        DMA1CNT_L,
        DMA1CNT_H,
        DMA2SAD_L,
        DMA2SAD_H,
        DMA2DAD_L,
        DMA2DAD_H,
        DMA2CNT_L,
        DMA2CNT_H,
        DMA3SAD_L,
        DMA3SAD_H,
        DMA3DAD_L,
        DMA3DAD_H,
        DMA3CNT_L,
        DMA3CNT_H,
        TM0CNT_L,
        TM0CNT_H,
        TM1CNT_L,
        TM1CNT_H,
        TM2CNT_L,
        TM2CNT_H,
        TM3CNT_L,
        TM3CNT_H,
        SIOMULTI0,
        SIOMULTI1,
        SIOMULTI2,
        SIOMULTI3,
        SIOCNT,
        SIOMLT_SEND,
        KEYINPUT,
        KEYCNT,
        RCNT,
        IR,
        JOYCNT,
        JOY_RECV_L,
        JOY_RECV_H,
        JOY_TRANS_L,
        JOY_TRANS_H,
        JOYSTAT,
        IE,
        IF,
        WAITCNT,
        IME,
        POSTFLG,
        HALTCNT,
        UNKBUG,
        MEMCTL_L,
        MEMCTL_H
    } ioreg_t;

    typedef struct {
        int registerNumber;
        uint16_t value;
        writeCallback_t *writeCallback;
        uint16_t readMask;
        uint16_t writeMask;
    } ioregTableEntry_t;
    
    extern ioregTableEntry_t io[0x402];

    extern void init();
    extern uint8_t read8(uint32_t address);
    extern uint16_t read16(uint32_t address);
    extern uint32_t read32(uint32_t address);
    extern void write8(uint32_t address, uint8_t value);
    extern void write16(uint32_t address, uint16_t value);
    extern void write32(uint32_t address, uint32_t value);
    extern void set(unsigned int index, uint16_t value);
    extern uint16_t get(unsigned int index);
}
