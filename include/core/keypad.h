#ifndef __INCLUDE_CORE_KEYPAD_H__
#define __INCLUDE_CORE_KEYPAD_H__

#include <stdint.h>

void keypadReset(void);
uint16_t keypadIoRead16(uint32_t p_address);
void keypadIoWrite16(uint32_t p_address, uint16_t p_value);

#endif
