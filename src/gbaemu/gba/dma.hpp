#pragma once

#include <cstdint>

namespace gbaemu::gba::dma {
    extern void init();

    /* Returns true if at least 1 DMA is running. */
    extern bool cycle();

    extern void writeCallback_dma0cnt(uint16_t value);
    extern void writeCallback_dma1cnt(uint16_t value);
    extern void writeCallback_dma2cnt(uint16_t value);
    extern void writeCallback_dma3cnt(uint16_t value);
}
