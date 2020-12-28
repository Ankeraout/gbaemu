#ifndef __CORE_DMA_H__
#define __CORE_DMA_H__

#include <stdbool.h>
#include <stdint.h>

extern void gba_dma_reset();
extern bool gba_dma_cycle();
extern void gba_dma_writeCallback_cntH0(uint32_t address, uint16_t value);
extern void gba_dma_writeCallback_cntH1(uint32_t address, uint16_t value);
extern void gba_dma_writeCallback_cntH2(uint32_t address, uint16_t value);
extern void gba_dma_writeCallback_cntH3(uint32_t address, uint16_t value);
extern void gba_dma_onVblank();
extern void gba_dma_onHblank();

#endif
