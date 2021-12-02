#include <stddef.h>

#include "core/bios.h"
#include "core/bus.h"
#include "core/cartridge.h"
#include "core/cpu.h"
#include "core/gba.h"

void gba_init(bool skipBoot, const void *biosBuffer, const void *romBuffer, size_t romBufferSize) {
    gba_bios_init(biosBuffer);
    gba_bus_init();
    gba_cartridge_init(romBuffer, romBufferSize);
    gba_cpu_init();
    gba_cpu_reset(skipBoot);
}
