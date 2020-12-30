#include <stdint.h>
#include <string.h>

#include "core/dma.h"
#include "core/gba.h"
#include "core/io.h"
#include "core/timer.h"

gba_io_register_t gba_io_registers[512];
gba_io_register_t gba_io_register_internalMemoryControl_low;
gba_io_register_t gba_io_register_internalMemoryControl_high;
gba_io_register_t gba_io_nullRegister;

void gba_io_reset();
uint8_t gba_io_read8(uint32_t address);
uint16_t gba_io_read16(uint32_t address);
uint32_t gba_io_read32(uint32_t address);
void gba_io_write8(uint32_t address, uint8_t value);
void gba_io_write16(uint32_t address, uint16_t value);
void gba_io_write32(uint32_t address, uint32_t value);
gba_io_register_t *gba_io_getRegister(uint32_t address);
void gba_io_initRegister(uint32_t address, uint16_t initialValue, gba_io_writeCallack_t *writeCallback, uint16_t readMask, uint16_t writeMask);

void gba_io_reset() {
    memset(gba_io_registers, 0, sizeof(gba_io_registers));

    gba_io_register_internalMemoryControl_low.value = 0x0000;
    gba_io_register_internalMemoryControl_low.writeCallback = NULL;
    gba_io_register_internalMemoryControl_low.readMask = 0xffff;
    gba_io_register_internalMemoryControl_low.writeMask = 0xffff;

    gba_io_register_internalMemoryControl_high.value = 0x0000;
    gba_io_register_internalMemoryControl_high.writeCallback = NULL;
    gba_io_register_internalMemoryControl_high.readMask = 0xffff;
    gba_io_register_internalMemoryControl_high.writeMask = 0xffff;

    gba_io_nullRegister.value = 0x0000;
    gba_io_nullRegister.writeCallback = NULL;
    gba_io_nullRegister.readMask = 0x0000;
    gba_io_nullRegister.writeMask = 0xffff;

    gba_io_initRegister(0x04000000, 0x0000, NULL, 0xffff, 0xffff); // DISPCNT
    gba_io_initRegister(0x04000002, 0x0000, NULL, 0xffff, 0xffff); // GREENSWP
    gba_io_initRegister(0x04000004, 0x0000, NULL, 0xff3f, 0xff3f); // DISPSTAT
    gba_io_initRegister(0x04000006, 0x0000, NULL, 0xffff, 0x0000); // VCOUNT
    gba_io_initRegister(0x04000008, 0x0000, NULL, 0xffff, 0xffff); // BG0CNT
    gba_io_initRegister(0x0400000a, 0x0000, NULL, 0xffff, 0xffff); // BG1CNT
    gba_io_initRegister(0x0400000c, 0x0000, NULL, 0xffff, 0xffff); // BG2CNT
    gba_io_initRegister(0x0400000e, 0x0000, NULL, 0xffff, 0xffff); // BG3CNT
    gba_io_initRegister(0x04000010, 0x0000, NULL, 0x0000, 0x01ff); // BG0HOFS
    gba_io_initRegister(0x04000012, 0x0000, NULL, 0x0000, 0x01ff); // BG0VOFS
    gba_io_initRegister(0x04000014, 0x0000, NULL, 0x0000, 0x01ff); // BG1HOFS
    gba_io_initRegister(0x04000016, 0x0000, NULL, 0x0000, 0x01ff); // BG1VOFS
    gba_io_initRegister(0x04000018, 0x0000, NULL, 0x0000, 0x01ff); // BG2HOFS
    gba_io_initRegister(0x0400001a, 0x0000, NULL, 0x0000, 0x01ff); // BG2VOFS
    gba_io_initRegister(0x0400001c, 0x0000, NULL, 0x0000, 0x01ff); // BG3HOFS
    gba_io_initRegister(0x0400001e, 0x0000, NULL, 0x0000, 0x01ff); // BG3VOFS
    gba_io_initRegister(0x04000020, 0x0000, NULL, 0x0000, 0xffff); // BG2PA
    gba_io_initRegister(0x04000022, 0x0000, NULL, 0x0000, 0xffff); // BG2PB
    gba_io_initRegister(0x04000024, 0x0000, NULL, 0x0000, 0xffff); // BG2PC
    gba_io_initRegister(0x04000026, 0x0000, NULL, 0x0000, 0xffff); // BG2PD
    gba_io_initRegister(0x04000028, 0x0000, NULL, 0x0000, 0xffff); // BG2X_L
    gba_io_initRegister(0x0400002a, 0x0000, NULL, 0x0000, 0xffff); // BG2X_H
    gba_io_initRegister(0x0400002c, 0x0000, NULL, 0x0000, 0xffff); // BG2Y_L
    gba_io_initRegister(0x0400002e, 0x0000, NULL, 0x0000, 0xffff); // BG2Y_H
    gba_io_initRegister(0x04000030, 0x0000, NULL, 0x0000, 0xffff); // BG3PA
    gba_io_initRegister(0x04000032, 0x0000, NULL, 0x0000, 0xffff); // BG3PB
    gba_io_initRegister(0x04000034, 0x0000, NULL, 0x0000, 0xffff); // BG3PC
    gba_io_initRegister(0x04000036, 0x0000, NULL, 0x0000, 0xffff); // BG3PD
    gba_io_initRegister(0x04000038, 0x0000, NULL, 0x0000, 0xffff); // BG3X_L
    gba_io_initRegister(0x0400003a, 0x0000, NULL, 0x0000, 0xffff); // BG3X_H
    gba_io_initRegister(0x0400003c, 0x0000, NULL, 0x0000, 0xffff); // BG3Y_L
    gba_io_initRegister(0x0400003e, 0x0000, NULL, 0x0000, 0xffff); // BG3Y_H
    gba_io_initRegister(0x04000040, 0x0000, NULL, 0x0000, 0xffff); // WIN0H
    gba_io_initRegister(0x04000042, 0x0000, NULL, 0x0000, 0xffff); // WIN1H
    gba_io_initRegister(0x04000044, 0x0000, NULL, 0x0000, 0xffff); // WIN0V
    gba_io_initRegister(0x04000046, 0x0000, NULL, 0x0000, 0xffff); // WIN1V
    gba_io_initRegister(0x04000048, 0x0000, NULL, 0xffff, 0xffff); // WININ
    gba_io_initRegister(0x0400004a, 0x0000, NULL, 0xffff, 0xffff); // WINOUT
    gba_io_initRegister(0x0400004c, 0x0000, NULL, 0x0000, 0xffff); // MOSAIC
    gba_io_initRegister(0x04000050, 0x0000, NULL, 0xffff, 0xffff); // BLDCNT
    gba_io_initRegister(0x04000052, 0x0000, NULL, 0xffff, 0xffff); // BLDALPHA
    gba_io_initRegister(0x04000054, 0x0000, NULL, 0x0000, 0xffff); // BLDY
    gba_io_initRegister(0x040000b0, 0x0000, NULL, 0x0000, 0xffff); // DMA0SAD_L
    gba_io_initRegister(0x040000b2, 0x0000, NULL, 0x0000, 0x07ff); // DMA0SAD_H
    gba_io_initRegister(0x040000b4, 0x0000, NULL, 0x0000, 0xffff); // DMA0DAD_L
    gba_io_initRegister(0x040000b6, 0x0000, NULL, 0x0000, 0x07ff); // DMA0DAD_H
    gba_io_initRegister(0x040000b8, 0x0000, NULL, 0x0000, 0x3fff); // DMA0CNT_L
    gba_io_initRegister(0x040000ba, 0x0000, gba_dma_writeCallback_cntH0, 0xffe0, 0xffe0); // DMA0CNT_H
    gba_io_initRegister(0x040000bc, 0x0000, NULL, 0x0000, 0xffff); // DMA1SAD_L
    gba_io_initRegister(0x040000be, 0x0000, NULL, 0x0000, 0x07ff); // DMA1SAD_H
    gba_io_initRegister(0x040000c0, 0x0000, NULL, 0x0000, 0xffff); // DMA1DAD_L
    gba_io_initRegister(0x040000c2, 0x0000, NULL, 0x0000, 0x07ff); // DMA1DAD_H
    gba_io_initRegister(0x040000c4, 0x0000, NULL, 0x0000, 0x3fff); // DMA1CNT_L
    gba_io_initRegister(0x040000c6, 0x0000, gba_dma_writeCallback_cntH1, 0xffe0, 0xffe0); // DMA1CNT_H
    gba_io_initRegister(0x040000c8, 0x0000, NULL, 0x0000, 0xffff); // DMA2SAD_L
    gba_io_initRegister(0x040000ca, 0x0000, NULL, 0x0000, 0x07ff); // DMA2SAD_H
    gba_io_initRegister(0x040000cc, 0x0000, NULL, 0x0000, 0xffff); // DMA2DAD_L
    gba_io_initRegister(0x040000ce, 0x0000, NULL, 0x0000, 0x07ff); // DMA2DAD_H
    gba_io_initRegister(0x040000d0, 0x0000, NULL, 0x0000, 0x3fff); // DMA2CNT_L
    gba_io_initRegister(0x040000d2, 0x0000, gba_dma_writeCallback_cntH2, 0xffe0, 0xffe0); // DMA2CNT_H
    gba_io_initRegister(0x040000d4, 0x0000, NULL, 0x0000, 0xffff); // DMA3SAD_L
    gba_io_initRegister(0x040000d6, 0x0000, NULL, 0x0000, 0x0fff); // DMA3SAD_H
    gba_io_initRegister(0x040000d8, 0x0000, NULL, 0x0000, 0xffff); // DMA3DAD_L
    gba_io_initRegister(0x040000da, 0x0000, NULL, 0x0000, 0x0fff); // DMA3DAD_H
    gba_io_initRegister(0x040000dc, 0x0000, NULL, 0x0000, 0xffff); // DMA3CNT_L
    gba_io_initRegister(0x040000de, 0x0000, gba_dma_writeCallback_cntH3, 0xffe0, 0xffe0); // DMA3CNT_H
    gba_io_initRegister(0x04000100, 0x0000, gba_timer_writeCallback_channel0_reload, 0xffff, 0x0000); // TM0D
    gba_io_initRegister(0x04000102, 0x0000, gba_timer_writeCallback_channel0_control, 0x00c3, 0x00c3); // TM0CNT
    gba_io_initRegister(0x04000104, 0x0000, gba_timer_writeCallback_channel1_reload, 0xffff, 0x0000); // TM1D
    gba_io_initRegister(0x04000106, 0x0000, gba_timer_writeCallback_channel1_control, 0x00c3, 0x00c3); // TM1CNT
    gba_io_initRegister(0x04000108, 0x0000, gba_timer_writeCallback_channel2_reload, 0xffff, 0x0000); // TM2D
    gba_io_initRegister(0x0400010a, 0x0000, gba_timer_writeCallback_channel2_control, 0x00c3, 0x00c3); // TM2CNT
    gba_io_initRegister(0x0400010c, 0x0000, gba_timer_writeCallback_channel3_reload, 0xffff, 0x0000); // TM3D
    gba_io_initRegister(0x0400010e, 0x0000, gba_timer_writeCallback_channel3_control, 0x00c3, 0x00c3); // TM3CNT
    gba_io_initRegister(0x04000130, 0xffff, NULL, 0x03ff, 0x0000); // KEYINPUT
    gba_io_initRegister(0x04000132, 0x0000, NULL, 0xc3ff, 0xc3ff); // KEYCNT
    gba_io_initRegister(0x04000200, 0x0000, NULL, 0x3fff, 0x3fff); // IE
    gba_io_initRegister(0x04000202, 0x0000, gba_writeToIF, 0x3fff, 0x0000); // IF
    gba_io_initRegister(0x04000208, 0x0000, NULL, 0x0001, 0x0001); // IME
}

uint8_t gba_io_read8(uint32_t address) {
    uint16_t value = gba_io_read16(address);

    if(address & 1) {
        return value >> 8;
    } else {
        return value;
    }
}

uint16_t gba_io_read16(uint32_t address) {
    return gba_io_getRegister(address)->value;
}

uint32_t gba_io_read32(uint32_t address) {
    return gba_io_read16(address) | (gba_io_read16(address + 2) << 16);
}

void gba_io_write8(uint32_t address, uint8_t value) {
    uint16_t v = gba_io_read16(address);

    if(address & 1) {
        v &= 0x00ff;
        v |= value << 8;
    } else {
        v &= 0xff00;
        v |= value;
    }

    gba_io_write16(address, v);
}

void gba_io_write16(uint32_t address, uint16_t value) {
    gba_io_register_t *reg = gba_io_getRegister(address);

    reg->value &= ~reg->writeMask;
    reg->value |= value & reg->writeMask;

    if(reg->writeCallback) {
        reg->writeCallback(address, value);
    }
}

void gba_io_write32(uint32_t address, uint32_t value) {
    gba_io_write16(address, value);
    gba_io_write16(address + 2, value >> 16);
}

gba_io_register_t *gba_io_getRegister(uint32_t address) {
    if((address & 0x0f00fffe) == 0x04000800) {
        return &gba_io_register_internalMemoryControl_low;
    } else if((address & 0x0f00fffe) == 0x04000802) {
        return &gba_io_register_internalMemoryControl_high;
    } else if((address & 0x0ffffffe) < 0x04000400) {
        return &gba_io_registers[(address & 0x000003ff) >> 1];
    } else {
        return &gba_io_nullRegister;
    }
}

void gba_io_initRegister(uint32_t address, uint16_t initialValue, gba_io_writeCallack_t *writeCallback, uint16_t readMask, uint16_t writeMask) {
    gba_io_register_t *reg = gba_io_getRegister(address);

    if(reg != &gba_io_nullRegister) {
        reg->value = initialValue;
        reg->writeCallback = writeCallback;
        reg->readMask = readMask;
        reg->writeMask = writeMask;
    }
}
