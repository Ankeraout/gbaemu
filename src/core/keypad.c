#include <stdbool.h>
#include <stdint.h>

#include "core/gba.h"
#include "core/io.h"

void gba_keypad_update(bool *pressedKeys);
void gba_keypad_cycle();

void gba_keypad_update(bool *pressedKeys) {
    uint16_t value = 0xffff;

    for(int i = 0; i < 10; i++) {
        if(pressedKeys[i]) {
            value &= ~(1 << i);
        }
    }

    gba_io_register_t *keyinput = gba_io_getRegister(0x04000130);
    keyinput->value = value;
}

void gba_keypad_cycle() {
    gba_io_register_t *keyinput = gba_io_getRegister(0x04000130);
    gba_io_register_t *keycnt = gba_io_getRegister(0x04000132);

    if(keycnt->value & (1 << 14)) {
        if(keycnt->value & (1 << 15)) {
            if((keycnt->value & (~keyinput->value) & 0x03ff) == (keycnt->value & 0x03ff)) {
                gba_setInterruptFlag(1 << 12);
            }
        } else {
            if(keycnt->value & (~keyinput->value) & 0x03ff) {
                gba_setInterruptFlag(1 << 12);
            }
        }
    }
}
