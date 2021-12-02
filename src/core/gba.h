#ifndef __CORE_GBA_H__
#define __CORE_GBA_H__

#include <stdint.h>

struct gba_s;

#include "core/bios.h"
#include "core/bus.h"
#include "core/cartridge.h"
#include "core/cpu.h"
#include "core/defines.h"

typedef void gba_frameHandler_t(const uint32_t *buffer);

/**
 * @brief This structure represents a GameBoy Advance.
 */
struct gba_s {
    gba_frameHandler_t *frameHandler_t; /**< The handler that will be called
                                            whenever the PPU finishes drawing a
                                            frame.*/
};

/**
 * @brief Initializes the GameBoy Advance.
 * 
 * @param[in] skipBoot A boolean value that indicates whether the boot ROM
 *                     execution should be skipped.
 * @param[in] biosBuffer The buffer that contains the BIOS. This buffer must
 *                       have a size of exactly 16384 bytes. Note that this
 *                       buffer will not be freed by this function. It can be
 *                       freed after the execution of this function.
 * @param[in] romBuffer The buffer that contains the ROM. This buffer must have
 *                      a size that is a power of two. Note that this buffer
 *                      will not be freed by this function. It can be freed
 *                      after the execution of this function.
 * @param[in] romBufferSize The size of the ROM buffer.
 */
extern void gba_init(bool skipBoot, const void *biosBuffer, const void *romBuffer, size_t romBufferSize);

/**
 * @brief Sets the frame handler.
 * @details The given function will be called everytime the PPU finishes
 *          drawing a frame. This is useful for updating the screen, for
 *          example.
 * 
 * @param[in] frameHandler The frame handler.
 */
extern void gba_setFrameHandler(gba_frameHandler_t *frameHandler);

/**
 * @brief Continues the execution of the GameBoy Advance until the PPU is done
 *        drawing a frame.
 */
extern void gba_frameAdvance();

#endif
