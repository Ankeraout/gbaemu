#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "core/bios.h"
#include "core/cartridge.h"
#include "core/cpu.h"
#include "core/dma.h"
#include "core/ewram.h"
#include "core/io.h"
#include "core/iwram.h"
#include "core/ppu.h"
#include "core/timer.h"

bool gba_skipBoot;
bool gba_frame;

void gba_cycle();
void gba_frameAdvance();
size_t gba_getSramSize();
void gba_init(bool skipBoot);
void gba_reset();
void gba_setBios(const void *buffer);
void gba_setRom(const void *buffer, size_t size);
void gba_setSram(void *buffer, size_t size);
void gba_setInterruptFlag(uint16_t flag);
void gba_writeToIF(uint32_t address, uint16_t flag);

void gba_frameAdvance() {
    gba_frame = false;

    while(!gba_frame) {
        gba_cycle();
    }
}

void gba_cycle() {
    if(!gba_dma_cycle()) {
        gba_cpu_cycle();
    }

    gba_ppu_cycle();
    gba_timer_cycle();
}

size_t gba_getSramSize() {
    return 0;
}

void gba_init(bool skipBoot) {
    gba_skipBoot = skipBoot;
    gba_reset(skipBoot);
    gba_cpu_init();
}

void gba_reset() {
    gba_cpu_reset(gba_skipBoot);
    gba_dma_reset();
    gba_ewram_reset();
    gba_io_reset();
    gba_iwram_reset();
    gba_ppu_reset();
    gba_timer_reset();
}

void gba_setBios(const void *buffer) {
    gba_bios_init(buffer);
    gba_reset();
}

void gba_setRom(const void *buffer, size_t size) {
    gba_cartridge_init(buffer, size);
    gba_reset();
}

void gba_setSram(void *buffer, size_t size) {
    UNUSED(buffer);
    UNUSED(size);
}

void gba_setInterruptFlag(uint16_t flag) {
    gba_io_getRegister(0x04000202)->value |= flag;
}

void gba_writeToIF(uint32_t address, uint16_t flag) {
    UNUSED(address);

    gba_io_getRegister(0x04000202)->value &= ~flag;
}

void gba_onFrame() {
    gba_frame = true;
}
