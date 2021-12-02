#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "core/defines.h"
#include "core/gba.h"
#include "frontend/frontend.h"

static const char *biosPath;
static const char *romPath;

static void *biosBuffer;
static void *romBuffer;
static size_t romBufferSize;

/**
 * @brief The entry point of the application.
 * 
 * @param[in] argc The argument count of the application.
 * @param[in] argv The arguments of the application.
 * 
 * @returns An integer value that indicates the result of the execution of the
 *          program.
 * @retval 0 if the program executed normally without error.
 * @retval Any other value if the program exited with an error.
 */
int main(int argc, const char **argv);

/**
 * @brief Reads the application parameters.
 * 
 * @param[in] argc The argument count of the application.
 * @param[in] argv The arguments of the application.
 * 
 * @returns An integer value that indicates the result of the parsing of the
 *          program parameters.
 * @retval 0 if the parameters were parsed successfully.
 * @retval Any other value if an error occurred.
 */
static int readCommandLineArguments(int argc, const char **argv);

/**
 * @brief Prints the help message.
 */
static void printHelp();

/**
 * @brief Checks if the configuration of the program is valid.
 * 
 * @returns An integer value that indicates whether the program configuration
 *          is valid or not.
 * @retval 0 if the program configuration is valid.
 * @retval Any other value otherwise.
 */
static int checkConfiguration();

/**
 * @brief Loads the GameBoy Advance BIOS.
 * 
 * @returns An integer value that indicates whether the BIOS was loaded
 *          successfully.
 * @retval 0 if the BIOS was loaded successfully.
 * @retval Any other value otherwise.
 */
static int loadBios();

/**
 * @brief Loads the GameBoy Advance ROM.
 * 
 * @returns An integer value that indicates whether the ROM was loaded
 *          successfully.
 * @retval 0 if the ROM was loaded successfully.
 * @retval Any other value otherwise.
 */
static int loadRom();

int main(int argc, const char **argv) {
    if(readCommandLineArguments(argc, argv)) {
        printHelp();
        return EXIT_FAILURE;
    }

    if(checkConfiguration()) {
        return EXIT_FAILURE;
    }

    if(loadBios()) {
        return EXIT_FAILURE;
    }

    if(loadRom()) {
        return EXIT_FAILURE;
    }

    if(frontend_init()) {
        return EXIT_FAILURE;
    }

    gba_init(true, biosBuffer, romBuffer, romBufferSize);

    free(biosBuffer);
    free(romBuffer);

    while(true) {
        gba_frameAdvance();
    }

    frontend_close();

    return EXIT_SUCCESS;
}

static int readCommandLineArguments(int argc, const char **argv) {
    bool flag_bios = false;
    bool flag_rom = false;
    
    for(int i = 1; i < argc; i++) {
        if(flag_bios) {
            if(biosPath) {
                fprintf(stderr, "Too many BIOS files.\n");
                return 1;
            } else {
                biosPath = argv[i];
                flag_bios = false;
            }
        } else if(flag_rom) {
            if(romPath) {
                fprintf(stderr, "Too many ROM files.\n");
                return 1;
            } else {
                romPath = argv[i];
                flag_rom = false;
            }
        } else if(strcmp(argv[i], "--bios") == 0) {
            flag_bios = true;
        } else if(strcmp(argv[i], "--rom") == 0) {
            flag_rom = true;
        } else if(strcmp(argv[i], "--help") == 0) {
            return 1;
        } else {
            fprintf(stderr, "Invalid argument '%s'\n", argv[i]);
        }
    }

    return 0;
}

static void printHelp() {
    printf("gbaemu\n");
    printf("======\n");
    printf("\n");
    printf("Required command-line options:\n");
    printf("  --bios <bios file name>\n");
    printf("  --rom <rom file name>\n");
    printf("\n");
    printf("Optional command-line options:");
    printf("  --help\n");
}

static int checkConfiguration() {
    if(biosPath == NULL) {
        fprintf(stderr, "No BIOS file path specified.\n");
        return 1;
    }

    return 0;
}

static int loadBios() {
    long fileSize = GBA_BIOS_FILE_SIZE;

    biosBuffer = readFile(biosPath, &fileSize, true);

    if(biosBuffer) {
        if(fileSize != GBA_BIOS_FILE_SIZE) {
            fprintf(stderr, "Wrong BIOS file size.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Failed to read BIOS file.\n");
        return 1;
    }

    return 0;
}

static int loadRom() {
    long fileSize = GBA_MAX_ROM_FILE_SIZE;

    romBuffer = readFile(romPath, &fileSize, true);

    if(!romBuffer) {
        fprintf(stderr, "Failed to read ROM file.\n");
        return 1;
    }

    romBufferSize = fileSize;

    return 0;
}
