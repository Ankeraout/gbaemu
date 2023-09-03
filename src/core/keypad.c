#include <stdbool.h>
#include <stdint.h>

#define C_KEYPAD_IOADDR_KEYINPUT 0x04000130
#define C_KEYPAD_IOADDR_KEYCNT 0x04000132

static bool s_keypadButtonState[10];
static uint16_t s_keypadRegisterKeycnt;

void keypadReset(void) {
    for(int l_i = 0; l_i < 10; l_i++) {
        s_keypadButtonState[l_i] = false;
    }
}

void coreSetInput(int p_key, bool p_pressed) {
    s_keypadButtonState[p_key] = p_pressed;
}

uint16_t keypadIoRead16(uint32_t p_address) {
    uint16_t l_result;

    switch(p_address) {
        case C_KEYPAD_IOADDR_KEYINPUT:
            l_result = 0xffff;

            for(int l_i = 0; l_i < 10; l_i++) {
                uint16_t l_mask = 1 << l_i;

                if(s_keypadButtonState[l_i]) {
                    l_result &= ~l_mask;
                }
            }

            break;

        case C_KEYPAD_IOADDR_KEYCNT:
            l_result = s_keypadRegisterKeycnt;
            break;

        default:
            l_result = 0xffff;
            break;
    }

    return l_result;
}

void keypadIoWrite16(uint32_t p_address, uint16_t p_value) {
    switch(p_address) {
        case C_KEYPAD_IOADDR_KEYCNT: s_keypadRegisterKeycnt = p_value; break;
        default: break;
    }
}
