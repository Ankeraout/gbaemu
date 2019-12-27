#pragma once

namespace gbaemu::gba::cpu {
    typedef union {
        uint32_t value;

        struct {
            uint32_t mode : 5;
            uint32_t flagT : 1;
            uint32_t flagF : 1;
            uint32_t flagI : 1;
            uint32_t reserved : 19;
            uint32_t flagQ : 1;
            uint32_t flagV : 1;
            uint32_t flagC : 1;
            uint32_t flagZ : 1;
            uint32_t flagN : 1;
        } fields;
    } psr_t;

    typedef enum {
        PSR_MODE_USR = 0b10000,
        PSR_MODE_FIQ = 0b10001,
        PSR_MODE_IRQ = 0b10010,
        PSR_MODE_SVC = 0b10011,
        PSR_MODE_ABT = 0b10111,
        PSR_MODE_UND = 0b11011,
        PSR_MODE_SYS = 0b11111,
    } mode_t;

    enum {
        MODE_USR,
        MODE_FIQ,
        MODE_IRQ,
        MODE_SVC,
        MODE_ABT,
        MODE_UND,
        MODE_SYS,
        MODE_INV
    };

    extern void init();
}
