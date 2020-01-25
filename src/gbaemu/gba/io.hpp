#pragma once

#include <cstdint>

namespace gbaemu::gba::io {
    typedef void writeCallback_t(uint16_t value);

    typedef enum {
        IOREGNONE,
        DISPCNT = 0x000,
        GREENSWP = 0x002,
        DISPSTAT = 0x004,
        VCOUNT = 0x006,
        BG0CNT = 0x008,
        BG1CNT = 0x00a,
        BG2CNT = 0x00c,
        BG3CNT = 0x00e,
        BG0HOFS = 0x010,
        BG0VOFS = 0x012,
        BG1HOFS = 0x014,
        BG1VOFS = 0x016,
        BG2HOFS = 0x018,
        BG2VOFS = 0x01a,
        BG3HOFS = 0x01c,
        BG3VOFS = 0x01e,
        BG2PA = 0x020,
        BG2PB = 0x022,
        BG2PC = 0x024,
        BG2PD = 0x026,
        BG2X_L = 0x028,
        BG2X_H = 0x02a,
        BG2Y_L = 0x02c,
        BG2Y_H = 0x02e,
        BG3PA = 0x030,
        BG3PB = 0x032,
        BG3PC = 0x034,
        BG3PD = 0x036,
        BG3X_L = 0x038,
        BG3X_H = 0x03a,
        BG3Y_L = 0x03c,
        BG3Y_H = 0x03e,
        WIN0H = 0x040,
        WIN1H = 0x042,
        WIN0V = 0x044,
        WIN1V = 0x046,
        WININ = 0x048,
        WINOUT = 0x04a,
        MOSAIC = 0x04c,
        BLDCNT = 0x050,
        BLDALPHA = 0x052,
        BLDY = 0x054,
        SOUND1CNT_L = 0x060,
        SOUND1CNT_H = 0x062,
        SOUND1CNT_X = 0x064,
        SOUND2CNT_L = 0x068,
        SOUND2CNT_H = 0x06c,
        SOUND3CNT_L = 0x070,
        SOUND3CNT_H = 0x072,
        SOUND3CNT_X = 0x074,
        SOUND4CNT_L = 0x078,
        SOUND4CNT_H = 0x07c,
        SOUNDCNT_L = 0x080,
        SOUNDCNT_H = 0x082,
        SOUNDCNT_X = 0x084,
        SOUNDBIAS = 0x088,
        WAVE_RAM = 0x090,
        FIFO_A_L = 0x0a0,
        FIFO_A_H = 0x0a2,
        FIFO_B_L = 0x0a4,
        FIFO_B_H = 0x0a6,
        DMA0SAD_L = 0x0b0,
        DMA0SAD_H = 0x0b2,
        DMA0DAD_L = 0x0b4,
        DMA0DAD_H = 0x0b6,
        DMA0CNT_L = 0x0b8,
        DMA0CNT_H = 0x0ba,
        DMA1SAD_L = 0x0bc,
        DMA1SAD_H = 0x0be,
        DMA1DAD_L = 0x0c0,
        DMA1DAD_H = 0x0c2,
        DMA1CNT_L = 0x0c4,
        DMA1CNT_H = 0x0c6,
        DMA2SAD_L = 0x0c8,
        DMA2SAD_H = 0x0ca,
        DMA2DAD_L = 0x0cc,
        DMA2DAD_H = 0x0ce,
        DMA2CNT_L = 0x0d0,
        DMA2CNT_H = 0x0d2,
        DMA3SAD_L = 0x0d4,
        DMA3SAD_H = 0x0d6,
        DMA3DAD_L = 0x0d8,
        DMA3DAD_H = 0x0da,
        DMA3CNT_L = 0x0dc,
        DMA3CNT_H = 0x0de,
        TM0CNT_L = 0x100,
        TM0CNT_H = 0x102,
        TM1CNT_L = 0x104,
        TM1CNT_H = 0x106,
        TM2CNT_L = 0x108,
        TM2CNT_H = 0x10a,
        TM3CNT_L = 0x10c,
        TM3CNT_H = 0x10e,
        SIOMULTI0 = 0x120,
        SIOMULTI1 = 0x122,
        SIOMULTI2 = 0x124,
        SIOMULTI3 = 0x126,
        SIOCNT = 0x128,
        SIOMLT_SEND = 0x12a,
        KEYINPUT = 0x130,
        KEYCNT = 0x132,
        RCNT = 0x134,
        IR = 0x136,
        JOYCNT = 0x140,
        JOY_RECV_L = 0x150,
        JOY_RECV_H = 0x152,
        JOY_TRANS_L = 0x154,
        JOY_TRANS_H = 0x156,
        JOYSTAT = 0x158,
        IE = 0x200,
        IF = 0x200,
        WAITCNT = 0x204,
        IME = 0x208,
        POSTFLG = 0x300,
        HALTCNT = 0x301,
        UNKBUG = 0x410,
        MEMCTL_L = 0x800,
        MEMCTL_H = 0x802
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
    extern uint32_t get32(unsigned int index);
}
