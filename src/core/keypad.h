#ifndef __CORE_KEYPAD_H__
#define __CORE_KEYPAD_H__

#include <stdbool.h>

extern void gba_keypad_update(bool *pressedKeys);
extern void gba_keypad_cycle();

#endif
