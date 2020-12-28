#include <stddef.h>

#include "platform.h"
#include "core/bios.h"
#include "core/ewram.h"
#include "core/iwram.h"

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
}

void gba_setBios(const void *buffer) {
    gba_bios_init(buffer);
    gba_reset();
}

void gba_setRom(const void *buffer, size_t size) {
    UNUSED(buffer);
    UNUSED(size);
}

void gba_setSram(void *buffer, size_t size) {
    UNUSED(buffer);
    UNUSED(size);
}
