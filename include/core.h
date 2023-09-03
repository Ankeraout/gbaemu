#ifndef __INCLUDE_CORE_H__
#define __INCLUDE_CORE_H__

#include <stdbool.h>
#include <stddef.h>

enum {
    E_CORE_INPUT_BUTTON_A,
    E_CORE_INPUT_BUTTON_B,
    E_CORE_INPUT_BUTTON_SELECT,
    E_CORE_INPUT_BUTTON_START,
    E_CORE_INPUT_BUTTON_RIGHT,
    E_CORE_INPUT_BUTTON_LEFT,
    E_CORE_INPUT_BUTTON_UP,
    E_CORE_INPUT_BUTTON_DOWN,
    E_CORE_INPUT_BUTTON_R,
    E_CORE_INPUT_BUTTON_L,
};

void coreInit(void);
void coreReset(void);
int coreSetBios(const void *p_buffer, size_t p_size);
int coreSetRom(const void *p_buffer, size_t p_size);
void coreStep(void);
void coreSetInput(int p_key, bool p_pressed);

#endif
