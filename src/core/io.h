#ifndef __CORE_IO_H__
#define __CORE_IO_H__

#include <stdint.h>

typedef void gba_io_writeCallack_t(uint32_t address, uint16_t value);

typedef struct {
    uint16_t value;
    uint16_t readMask;
    uint16_t writeMask;
    gba_io_writeCallack_t *writeCallback;
} gba_io_register_t;

extern void gba_io_reset();
extern uint8_t gba_io_read8(uint32_t address);
extern uint16_t gba_io_read16(uint32_t address);
extern uint32_t gba_io_read32(uint32_t address);
extern void gba_io_write8(uint32_t address, uint8_t value);
extern void gba_io_write16(uint32_t address, uint16_t value);
extern void gba_io_write32(uint32_t address, uint32_t value);
extern gba_io_register_t *gba_io_getRegister(uint32_t address);

#endif
