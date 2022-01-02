#ifndef __GBA_H__
#define __GBA_H__

//==============================================================================
// Included files
//==============================================================================
#include <stdbool.h>

//==============================================================================
// Public constants
//==============================================================================

//==============================================================================
// Public types
//==============================================================================

//==============================================================================
// Public functions declaration
//==============================================================================
/**
 * @brief Initializes the GameBoy Advance core.
 *
 * @returns An integer that indicates the result of the operation.
 * @retval 0 if no error occurred.
 * @retval Any other value if an error occurred.
 */
int gbaInit(void);

/**
 * @brief Performs a GBA reset.
 */
void gbaReset(bool p_skipBoot);

/**
 * @brief Performs a GBA cycle (see cpuCycle for more information about the
 *        timing of a cycle compared to the system clock).
 */
void gbaCycle(void);

/**
 * @brief Performs a GBA cycle without executing the CPU cycle. This function
 *        shall only be called by the CPU module.
 */
void gbaCycleNoCPU(void);

/**
 * @brief Runs the GBA core until the next VBlank is reached.
 */
void gbaFrameAdvance(void);

#endif // __GBA_H__