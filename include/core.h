#ifndef __INCLUDE_CORE_H__
#define __INCLUDE_CORE_H__

#include <stddef.h>

void coreInit(void);
void coreReset(void);
int coreSetBios(const void *p_buffer, size_t p_size);
int coreSetRom(const void *p_buffer, size_t p_size);
void coreStep(void);

#endif
