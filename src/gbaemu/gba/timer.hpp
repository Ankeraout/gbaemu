#pragma once

#include <cstdint>

#include <gbaemu/gbaemu.hpp>

namespace gbaemu::gba::timer {
    typedef union {
        PACKED_STRUCT(
            uint16_t prescaler : 2;
            uint16_t countUp : 1;
            uint16_t unused : 3;
            uint16_t irq : 1;
            uint16_t operate : 1;
            uint16_t unused2 : 8;
        ) fields;

        uint16_t value; 
    } tmcnt_h_t;

    extern void init();
    extern void cycle();

    extern void writeCallback_tm0cnt_l(uint16_t value);
    extern void writeCallback_tm0cnt_h(uint16_t value);
    extern void writeCallback_tm1cnt_l(uint16_t value);
    extern void writeCallback_tm1cnt_h(uint16_t value);
    extern void writeCallback_tm2cnt_l(uint16_t value);
    extern void writeCallback_tm2cnt_h(uint16_t value);
    extern void writeCallback_tm3cnt_l(uint16_t value);
    extern void writeCallback_tm3cnt_h(uint16_t value);
}
