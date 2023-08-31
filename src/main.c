#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "core.h"
#include "core/cpu/cpu.h"
#include "core/gba.h"

static int loadBios();
static int loadRom();
static long po2_ceil(long p_initialValue);
static void *readFile(const char *p_fileName, long *p_fileSize, bool p_po2);

int main(int argc, char **argv) {
    coreInit();

    if(loadBios() != 0) {
        fprintf(stderr, "Failed to load BIOS file.\n");
        return 1;
    }

    if(loadRom() != 0) {
        fprintf(stderr, "Failed to read ROM file.\n");
        return 1;
    }

    coreReset();

    for(int i = 0; i < 64; i++) {
        coreStep();
        cpuDebug();
    }

    return 0;
}

static int loadRom() {
    long l_romBufferSize = C_MAX_ROM_FILE_SIZE_BYTES;

    uint8_t *l_romBuffer = readFile("roms/gba-tests/arm.gba", &l_romBufferSize, true);

    if(l_romBuffer == NULL) {
        fprintf(stderr, "Failed to read ROM file.\n");
        return 1;
    }

    return coreSetRom(l_romBuffer, l_romBufferSize);
}

static int loadBios() {
    long l_fileSize = C_BIOS_FILE_SIZE_BYTES;

    uint8_t *l_biosBuffer = readFile("roms/gba_bios.bin", &l_fileSize, true);

    if(l_biosBuffer == NULL) {
        fprintf(stderr, "Failed to read BIOS file.\n");
        return 1;
    }

    if(l_fileSize != C_BIOS_FILE_SIZE_BYTES) {
        fprintf(stderr, "Wrong BIOS file size.\n");
        return 1;
    }

    return coreSetBios(l_biosBuffer, C_BIOS_FILE_SIZE_BYTES);
}

static long po2_ceil(long p_initialValue) {
    int l_hammingWeight = 0;
    int l_lastBitTo1 = 0;

    for(unsigned long i = 0; i < sizeof(long); i++) {
        for(unsigned long j = 0; j < 8; j++) {
            unsigned long shift = i * 8 + j;
            int bit = (p_initialValue >> shift) & 1;

            if(bit) {
                l_lastBitTo1 = shift;
                l_hammingWeight++;
            }
        }
    }

    if(l_hammingWeight == 1) {
        return p_initialValue;
    } else {
        return 1 << (l_lastBitTo1 + 1);
    }
}

static void *readFile(const char *p_fileName, long *p_fileSize, bool p_po2) {
    // Open the file as read-binary
    FILE *l_file = fopen(p_fileName, "rb");

    // If the file could not be opened, return an error
    if(!l_file) {
        fprintf(stderr, "readFile(): Failed to open %s.", p_fileName);
        return NULL;
    }

    // Determine the file size
    fseek(l_file, 0, SEEK_END);
    long l_foundFileSize = ftell(l_file);
    fseek(l_file, 0, SEEK_SET);

    // Compare the file size
    if(*p_fileSize > 0 && l_foundFileSize > *p_fileSize) {
        fprintf(stderr, "readFile(): File is too large: %s.", p_fileName);
        return NULL;
    }

    long l_bufferSize = p_po2 ? po2_ceil(l_foundFileSize) : l_foundFileSize;

    // Allocate the buffer
    void *l_buffer = calloc(l_bufferSize, 1);

    if(!l_buffer) {
        fprintf(stderr, "readFile(): Failed to allocate buffer.\n");
        return NULL;
    }

    // Read the file
    size_t l_readBytes = fread(l_buffer, 1, l_foundFileSize, l_file);

    if(l_readBytes != (size_t)l_foundFileSize) {
        free(l_buffer);
        fprintf(
            stderr,
            "readFile(): Failed to read %ld bytes.\n",
            l_foundFileSize
        );
        return NULL;
    }

    *p_fileSize = l_bufferSize;

    return l_buffer;
}
