//==============================================================================
// Included files
//==============================================================================
#include "bios.h"
#include "bus.h"
#include "cartrige.h"
#include "common.h"
#include "ewram.h"
#include "gpu.h"
#include "io.h"
#include "iwram.h"

//==============================================================================
// Private constants
//==============================================================================

//==============================================================================
// Private types
//==============================================================================
/**
 * @brief This structure stores the handlers for all the possible operations on
 *        the bus.
 */
typedef struct {
    uint8_t (*read8)(uint32_t p_address); ///< Handler for the read8 operation.
    uint16_t (*read16)(uint32_t p_address); /**< Handler for the read16
                                                 operation. */
    uint32_t (*read32)(uint32_t p_address); /**< Handler for the read32
                                                 operation. */
    void (*write8)(uint32_t p_address, uint8_t p_value); /**< Handler for the
                                                                 write8
                                                                 operation.*/
    void (*write16)(uint32_t p_address, uint16_t p_value); /**< Handler for the
                                                                 write16
                                                                 operation.*/
    void (*write32)(uint32_t p_address, uint32_t p_value); /**< Handler for the
                                                                 write32
                                                                 operation.*/
} t_busOperationHandlers;

//==============================================================================
// Private functions declaration
//==============================================================================
/**
 * @brief Dummy handler for the bus 8-bit write operation.
 *
 * @param[in] p_address The address where the value shall be written.
 * @param[in] p_value The value that shall be written.
 */
static void busDummyWrite8(uint32_t p_address, uint8_t p_value);

/**
 * @brief Dummy handler for the bus 16-bit write operation.
 *
 * @param[in] p_address The address where the value shall be written.
 * @param[in] p_value The value that shall be written.
 */
static void busDummyWrite16(uint32_t p_address, uint16_t p_value);

/**
 * @brief Dummy handler for the bus 32-bit write operation.
 *
 * @param[in] p_address The address where the value shall be written.
 * @param[in] p_value The value that shall be written.
 */
static void busDummyWrite32(uint32_t p_address, uint32_t p_value);

//==============================================================================
// Private variables
//==============================================================================
const t_busOperationHandlers s_busOperationHandlers[16] = {
    // 0xx0xxxxxx: BIOS
    {
        .read8 = biosRead8,
        .read16 = biosRead16,
        .read32 = biosRead32,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xx1xxxxxx: BIOS
    {
        .read8 = biosRead8,
        .read16 = biosRead16,
        .read32 = biosRead32,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xx2xxxxxx: EWRAM
    {
        .read8 = ewramRead8,
        .read16 = ewramRead16,
        .read32 = ewramRead32,
        .write8 = ewramWrite8,
        .write16 = ewramWrite16,
        .write32 = ewramWrite32
    },
    // 0xx3xxxxxx: IWRAM
    {
        .read8 = iwramRead8,
        .read16 = iwramRead16,
        .read32 = iwramRead32,
        .write8 = iwramWrite8,
        .write16 = iwramWrite16,
        .write32 = iwramWrite32
    },
    // 0xx4xxxxxx: I/O registers
    {
        .read8 = ioRead8,
        .read16 = ioRead16,
        .read32 = ioRead32,
        .write8 = ioWrite8,
        .write16 = ioWrite16,
        .write32 = ioWrite32
    },
    // 0xx5xxxxxx: Palette
    {
        .read8 = gpuPaletteRead8,
        .read16 = gpuPaletteRead16,
        .read32 = gpuPaletteRead32,
        .write8 = gpuPaletteWrite8,
        .write16 = gpuPaletteWrite16,
        .write32 = gpuPaletteWrite32
    },
    // 0xx6xxxxxx: VRAM
    {
        .read8 = gpuVramRead8,
        .read16 = gpuVramRead16,
        .read32 = gpuVramRead32,
        .write8 = gpuVramWrite8,
        .write16 = gpuVramWrite16,
        .write32 = gpuVramWrite32
    },
    // 0xx7xxxxxx: OAM
    {
        .read8 = gpuOamRead8,
        .read16 = gpuOamRead16,
        .read32 = gpuOamRead32,
        .write8 = gpuOamWrite8,
        .write16 = gpuOamWrite16,
        .write32 = gpuOamWrite32
    },
    // 0xx8xxxxxx: Game Pak ROM (wait state 0)
    {
        .read8 = cartridgeRomRead8,
        .read16 = cartridgeRomRead16,
        .read32 = cartridgeRomRead32,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xx9xxxxxx: Game Pak ROM (wait state 0)
    {
        .read8 = cartridgeRomRead8,
        .read16 = cartridgeRomRead16,
        .read32 = cartridgeRomRead32,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xxaxxxxxx: Game Pak ROM (wait state 1)
    {
        .read8 = cartridgeRomRead8,
        .read16 = cartridgeRomRead16,
        .read32 = cartridgeRomRead32,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xxbxxxxxx: Game Pak ROM (wait state 1)
    {
        .read8 = cartridgeRomRead8,
        .read16 = cartridgeRomRead16,
        .read32 = cartridgeRomRead32,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xxcxxxxxx: Game Pak ROM (wait state 2)
    {
        .read8 = cartridgeRomRead8,
        .read16 = cartridgeRomRead16,
        .read32 = cartridgeRomRead32,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xxdxxxxxx: Game Pak ROM (wait state 2)
    {
        .read8 = cartridgeRomRead8,
        .read16 = cartridgeRomRead16,
        .read32 = cartridgeRomRead32,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xxexxxxxx: Game Pak SRAM
    {
        .read8 = cartridgeSramRead8,
        .read16 = cartridgeSramRead16,
        .read32 = cartridgeSramRead32,
        .write8 = cartridgeSramWrite8,
        .write16 = cartridgeSramWrite16,
        .write32 = cartridgeSramWrite32
    },
    // 0xxfxxxxxx: Game Pak SRAM
    {
        .read8 = cartridgeSramRead8,
        .read16 = cartridgeSramRead16,
        .read32 = cartridgeSramRead32,
        .write8 = cartridgeSramWrite8,
        .write16 = cartridgeSramWrite16,
        .write32 = cartridgeSramWrite32
    },
};

//==============================================================================
// Public functions definition
//==============================================================================
uint8_t busRead8(uint32_t p_address) {
    return s_busOperationHandlers[(p_address & 0x0f000000) >> 24]
        .read8(p_address);
}

uint16_t busRead16(uint32_t p_address) {
    return s_busOperationHandlers[(p_address & 0x0f000000) >> 24]
        .read16(p_address);
}

uint32_t busRead32(uint32_t p_address) {
    return s_busOperationHandlers[(p_address & 0x0f000000) >> 24]
        .read32(p_address);
}

void busWrite8(uint32_t p_address, uint8_t p_value) {
    s_busOperationHandlers[(p_address & 0x0f000000) >> 24]
        .write8(p_address, p_value);
}

void busWrite16(uint32_t p_address, uint16_t p_value) {
    s_busOperationHandlers[(p_address & 0x0f000000) >> 24]
        .write16(p_address, p_value);
}

void busWrite32(uint32_t p_address, uint32_t p_value) {
    s_busOperationHandlers[(p_address & 0x0f000000) >> 24]
        .write32(p_address, p_value);
}

//==============================================================================
// Private functions definition
//==============================================================================
static void busDummyWrite8(uint32_t p_address, uint8_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

static void busDummyWrite16(uint32_t p_address, uint16_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}

static void busDummyWrite32(uint32_t p_address, uint32_t p_value) {
    M_UNUSED_PARAMETER(p_address);
    M_UNUSED_PARAMETER(p_value);
}
