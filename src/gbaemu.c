#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "core/defines.h"
#include "core/gba.h"

const char *biosPath;
const char *romPath;

const void *biosBuffer;
const void *romBuffer;
void *sramBuffer;

int main(int argc, const char **argv);
int readCommandLineArguments(int argc, const char **argv);
void printHelp();
int checkConfiguration();
int loadBios();
int loadRom();

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

    return EXIT_SUCCESS;
}

int readCommandLineArguments(int argc, const char **argv) {
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
}

void printHelp() {
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

int checkConfiguration() {
    if(biosPath == NULL) {
        fprintf(stderr, "No BIOS file path specified.\n");
    }
}

int loadBios() {
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
}

int loadRom() {
    long fileSize = GBA_MAX_ROM_FILE_SIZE;

    romBuffer = readFile(romPath, &fileSize, true);

    if(!romBuffer) {
        fprintf(stderr, "Failed to read ROM file.\n");
        return 1;
    }
}
