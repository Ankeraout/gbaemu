#include <stddef.h>

#include "platform.h"
#include "core/bios.h"
#include "core/cartridge.h"
#include "core/ewram.h"
#include "core/iwram.h"
#include "core/ppu.h"

void gba_cycle();
void gba_frameAdvance();
size_t gba_getSramSize();
void gba_init();
void gba_reset();
void gba_setBios(const void *buffer);
void gba_setRom(const void *buffer, size_t size);
void gba_setSram(void *buffer, size_t size);

void gba_frameAdvance() {

}

void gba_cycle() {
    
}

size_t gba_getSramSize() {
    return 0;
}

void gba_init() {
    gba_reset();
}

void gba_reset() {
    gba_ewram_reset();
    gba_iwram_reset();
    gba_ppu_reset();
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
