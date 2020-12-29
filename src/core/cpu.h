#ifndef __CORE_CPU_H__
#define __CORE_CPU_H__

#include <stdbool.h>

extern void gba_cpu_init();
extern void gba_cpu_reset(bool skipBoot);
extern void gba_cpu_cycle();

#endif
