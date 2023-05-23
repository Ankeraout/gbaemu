#include <stddef.h>

#include "core/cpu/cpu.h"
#include "core/gpu.h"

void coreInit(void) {
    cpuInit();
}

void coreReset(void) {
    cpuReset(true);
    gpuReset();
}
