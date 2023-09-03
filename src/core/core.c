#include <stddef.h>

#include "core/cpu/cpu.h"
#include "core/gpu.h"
#include "core/io.h"
#include "core/keypad.h"

void coreInit(void) {
    cpuInit();
    ioInit();
}

void coreReset(void) {
    cpuReset(true);
    gpuReset();
    keypadReset();
}
