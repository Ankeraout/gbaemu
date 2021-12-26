//==============================================================================
// Included files
//==============================================================================
#include <stdio.h>
#include <stdlib.h>

#include "emulator.h"

//==============================================================================
// Public functions definition
//==============================================================================

/**
 * @brief This function is the entry point of the emulator.
 *
 * @param[in] p_argc Contains the number of elements in p_argv (arguments).
 * @param[in] p_argv Contains the command-line parameters of the program.
 *
 * @returns An integer that indicates the result of the execution of the
 *          emulator.
 * @retval 0 if no error occurred.
 * @retval Any other value if an error occurred.
 */
int main(int p_argc, const char *p_argv[]) {
    if(emulatorInit(p_argc, p_argv) != 0) {
        return EXIT_FAILURE;
    }

    emulatorExecute();

    return EXIT_SUCCESS;
}