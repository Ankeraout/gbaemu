//==============================================================================
// Included files
//==============================================================================
#include "bus.h"
#include "common.h"

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
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xx1xxxxxx: BIOS
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xx2xxxxxx: EWRAM
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = NULL,
        .write16 = NULL,
        .write32 = NULL
    },
    // 0xx3xxxxxx: IWRAM
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = NULL,
        .write16 = NULL,
        .write32 = NULL
    },
    // 0xx4xxxxxx: I/O registers
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = NULL,
        .write16 = NULL,
        .write32 = NULL
    },
    // 0xx5xxxxxx: Palette
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = NULL,
        .write16 = NULL,
        .write32 = NULL
    },
    // 0xx6xxxxxx: VRAM
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = NULL,
        .write16 = NULL,
        .write32 = NULL
    },
    // 0xx7xxxxxx: OAM
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = NULL,
        .write16 = NULL,
        .write32 = NULL
    },
    // 0xx8xxxxxx: Game Pak ROM (wait state 0)
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xx9xxxxxx: Game Pak ROM (wait state 0)
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xxaxxxxxx: Game Pak ROM (wait state 1)
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xxbxxxxxx: Game Pak ROM (wait state 1)
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xxcxxxxxx: Game Pak ROM (wait state 2)
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xxdxxxxxx: Game Pak ROM (wait state 2)
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = busDummyWrite8,
        .write16 = busDummyWrite16,
        .write32 = busDummyWrite32
    },
    // 0xxexxxxxx: Game Pak SRAM
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = NULL,
        .write16 = NULL,
        .write32 = NULL
    },
    // 0xxfxxxxxx: Game Pak SRAM
    {
        .read8 = NULL,
        .read16 = NULL,
        .read32 = NULL,
        .write8 = NULL,
        .write16 = NULL,
        .write32 = NULL
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
