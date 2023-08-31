#ifndef __INCLUDE_CORE_CPU_FLAGS_H__
#define __INCLUDE_CORE_CPU_FLAGS_H__

#include <stdint.h>

#include "core/cpu/cpu.h"
#include "core/cpu/shifter.h"

static inline void cpuUtilSetFlagsArithmetical(uint32_t p_result) {
    g_cpuFlagZ = p_result == 0;
    g_cpuFlagN = (p_result & 0x80000000) != 0;
}

static inline void cpuUtilSetFlagsLogical(uint32_t p_result) {
    cpuUtilSetFlagsArithmetical(p_result);
    g_cpuFlagC = g_cpuShifterCarry;
}

#endif
