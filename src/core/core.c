#include <stddef.h>

#include "core/cpu/cpu.h"
#include "core/ewram.h"
#include "core/gpu.h"
#include "core/io.h"
#include "core/irq.h"
#include "core/iwram.h"
#include "core/keypad.h"

void coreInit(void) {
    cpuInit();
    ioInit();
}

void coreReset(void) {
    cpuReset(true);
    ewramReset();
    gpuReset();
    iwramReset();
    irqReset();
    keypadReset();
}
