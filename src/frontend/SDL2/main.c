#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "core.h"
#include "core/cpu/cpu.h"
#include "core/gba.h"

#define DEBUG 0

static SDL_Window *s_window;
static SDL_Surface *s_screenSurface;

static int loadBios();
static int loadRom();
static long po2_ceil(long p_initialValue);
static void *readFile(const char *p_fileName, long *p_fileSize, bool p_po2);

int main(int argc, char **argv) {
    int l_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_AUDIO);

    if(l_result != 0) {
        fprintf(stderr, "SDL_Init() failed with code %d.\n", l_result);
        return 1;
    }

    s_window = SDL_CreateWindow(
        "gbaemu",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        480,
        320,
        0
    );

    if(s_window == NULL) {
        SDL_Quit();
        fprintf(stderr, "SDL_CreateWindow() failed: %s\n", SDL_GetError());
        return 1;
    }

    s_screenSurface = SDL_CreateRGBSurface(
        0,
        240,
        160,
        32,
        0x000000ff,
        0x0000ff00,
        0x00ff0000,
        0
    );

    if(s_screenSurface == NULL) {
        SDL_Quit();
        fprintf(stderr, "SDL_CreateRGBSurface() failed: %s\n", SDL_GetError());
        return 1;
    }

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

#if DEBUG
    for(int i = 0; i < 1024000; i++) {
        coreStep();
        cpuDebug();
    }
#else
    while(true) {
        coreStep();
    }
#endif

    return 0;
}

static int loadRom() {
    long l_romBufferSize = C_MAX_ROM_FILE_SIZE_BYTES;

    uint8_t *l_romBuffer = readFile("roms/gba-tests/hello.gba", &l_romBufferSize, true);

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

void frontendFrame(const uint32_t *p_pixels) {
    for(int l_i = 0; l_i < 38400; l_i++) {
        ((uint32_t *)s_screenSurface->pixels)[l_i] = p_pixels[l_i];
    }

    SDL_Surface *l_windowSurface = SDL_GetWindowSurface(s_window);
    SDL_BlitScaled(s_screenSurface, NULL, l_windowSurface, NULL);
    SDL_UpdateWindowSurface(s_window);

    SDL_Event l_event;

    while(SDL_PollEvent(&l_event) != 0) {
        if(l_event.type == SDL_WINDOWEVENT) {
            if(l_event.window.event == SDL_WINDOWEVENT_CLOSE) {
                exit(0);
            }
        }
    }
}
