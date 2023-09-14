#ifndef __INCLUDE_CORE_DMA_H__
#define __INCLUDE_CORE_DMA_H__

#include <stdbool.h>
#include <stdint.h>

#define C_IOADDRESS_DMA0SAD_L 0x040000b0
#define C_IOADDRESS_DMA0SAD_H 0x040000b2
#define C_IOADDRESS_DMA1SAD_L 0x040000bc
#define C_IOADDRESS_DMA1SAD_H 0x040000be
#define C_IOADDRESS_DMA2SAD_L 0x040000c8
#define C_IOADDRESS_DMA2SAD_H 0x040000ca
#define C_IOADDRESS_DMA3SAD_L 0x040000d4
#define C_IOADDRESS_DMA3SAD_H 0x040000d6
#define C_IOADDRESS_DMA0DAD_L 0x040000b4
#define C_IOADDRESS_DMA0DAD_H 0x040000b6
#define C_IOADDRESS_DMA1DAD_L 0x040000c0
#define C_IOADDRESS_DMA1DAD_H 0x040000c2
#define C_IOADDRESS_DMA2DAD_L 0x040000cc
#define C_IOADDRESS_DMA2DAD_H 0x040000ce
#define C_IOADDRESS_DMA3DAD_L 0x040000d8
#define C_IOADDRESS_DMA3DAD_H 0x040000da
#define C_IOADDRESS_DMA0CNT_L 0x040000b8
#define C_IOADDRESS_DMA0CNT_H 0x040000ba
#define C_IOADDRESS_DMA1CNT_L 0x040000c4
#define C_IOADDRESS_DMA1CNT_H 0x040000c6
#define C_IOADDRESS_DMA2CNT_L 0x040000d0
#define C_IOADDRESS_DMA2CNT_H 0x040000d2
#define C_IOADDRESS_DMA3CNT_L 0x040000dc
#define C_IOADDRESS_DMA3CNT_H 0x040000de

void dmaReset(void);
void dmaCycle(void);
uint16_t dmaIoRead16(uint32_t p_address);
void dmaIoWrite16(uint32_t p_address, uint16_t p_value);
bool dmaIsRunning(void);

#endif
