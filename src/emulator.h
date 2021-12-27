#ifndef __EMULATOR_H__
#define __EMULATOR_H__

//==============================================================================
// Included files
//==============================================================================
#include <stdint.h>

//==============================================================================
// Public variables
//==============================================================================
/**
 * @brief This variable contains a pointer to an array that contains the BIOS
 *        ROM data.
 */
extern uint8_t *g_biosBuffer;

/**
 * @brief This variable contains a pointer to an array that contains the ROM
 *        data.
 */
extern uint8_t *g_romBuffer;

/**
 * @brief This variable contains the size of the ROM buffer.
 */
extern long g_romBufferSize;

/**
 * @brief This variable contains a pointer to an array that contains the SRAM
 *        data.
 */
extern uint8_t *g_sramBuffer;

/**
 * @brief This variable contains the size of the SRAM buffer.
 */
extern long g_sramBufferSize;

//==============================================================================
// Public functions declaration
//==============================================================================

/**
 * @brief This function is called for initializing the emulator.
 * @details This function is responsible for parsing the command-line
 *          parameters, loading the BIOS and ROM files, performing pre-
 *          emulation checks (such as checking the ROM checksum for instance),
 *          and then preparing the core.
 *          If an error occurs while executing this function, then a message
 *          describing the error will be printed to the standard output.
 *
 * @param[in] p_argc Contains the number of elements in p_argv (arguments).
 * @param[in] p_argv Contains the command-line parameters of the program.
 *
 * @returns An integer that indicates the result of the execution of this
 *          function.
 * @retval 0 if no error occurred.
 * @retval Any other value if an error occurred.
 */
int emulatorInit(int p_argc, const char *p_argv[]);

/**
 * @brief This function contains the emulator loop.
 * @details This function is responsible for calling the core loop and also for
 *          regularly checking incoming events from user interface.
 */
void emulatorExecute(void);

#endif // __EMULATOR_H__