#ifndef __FRONTEND_FRONTEND_H__
#define __FRONTEND_FRONTEND_H__

#include <stdint.h>

extern void frontend_close();
extern void frontend_frame(const uint32_t *buffer);
extern int frontend_init();

#endif
