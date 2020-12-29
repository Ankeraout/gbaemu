#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "core/defines.h"
#include "core/keypad.h"

#define SCREEN_SCALE 2
#define WINDOW_WIDTH (GBA_SCREEN_WIDTH * SCREEN_SCALE)
#define WINDOW_HEIGHT (GBA_SCREEN_HEIGHT * SCREEN_SCALE)

SDL_Window *window;
SDL_Surface *windowSurface;
SDL_Surface *screenSurface;

Uint32 timestamp;
int frameCounter;

bool keys[10];

void frontend_close();
void frontend_frame(const uint32_t *buffer);
int frontend_init();
static inline void updateFps();
static inline void updateKey(SDL_Keysym *keysym, bool pressed);
static inline void updateKeypad();
static inline void updateScreen(const uint32_t *buffer);

int frontend_init() {
    if(SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("gbaemu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if(!window) {
        fprintf(stderr, "SDL_CreateWindow() failed: %s\n", SDL_GetError());
        return 1;
    }

    windowSurface = SDL_GetWindowSurface(window);

    if(!windowSurface) {
        fprintf(stderr, "SDL_GetWindowSurface() failed: %s\n", SDL_GetError());
        return 1;
    }

    screenSurface = SDL_CreateRGBSurface(0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    if(!screenSurface) {
        fprintf(stderr, "SDL_CreateRGBSurface() failed: %s\n", SDL_GetError());
        return 1;
    }

    frameCounter = 0;
    timestamp = SDL_GetTicks();

    for(int i = 0; i < 10; i++) {
        keys[i] = false;
    }

    return 0;
}

static inline void updateScreen(const uint32_t *buffer) {
    memcpy(screenSurface->pixels, buffer, GBA_SCREEN_WIDTH * GBA_SCREEN_HEIGHT);
    SDL_BlitScaled(screenSurface, NULL, windowSurface, NULL);
    SDL_UpdateWindowSurface(window);
}

static inline void updateFps() {
    Uint32 milliseconds = SDL_GetTicks();

    frameCounter++;

    if((milliseconds - timestamp) >= 1000) {
        char buffer[100];
        sprintf(buffer, "gbaemu [%d fps | %d%%]", frameCounter, (frameCounter * 100) / 60);
        SDL_SetWindowTitle(window, buffer);

        frameCounter = 0;
        timestamp = SDL_GetTicks();
    }
}

static inline void updateKey(SDL_Keysym *keysym, bool pressed) {
    switch(keysym->sym) {
        case SDLK_a: keys[0] = pressed; break;
        case SDLK_b: keys[1] = pressed; break;
        case SDLK_RSHIFT: keys[2] = pressed; break;
        case SDLK_RETURN: keys[3] = pressed; break;
        case SDLK_RIGHT: keys[4] = pressed; break;
        case SDLK_LEFT: keys[5] = pressed; break;
        case SDLK_UP: keys[6] = pressed; break;
        case SDLK_DOWN: keys[7] = pressed; break;
        case SDLK_r: keys[8] = pressed; break;
        case SDLK_l: keys[9] = pressed; break;
    }
}

static inline void updateKeypad() {
    SDL_Event e;

    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_WINDOWEVENT:
            switch(e.window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                exit(0);
                break;
            }

            break;

            case SDL_KEYDOWN:
            updateKey(&e.key.keysym, true);
            break;

            case SDL_KEYUP:
            updateKey(&e.key.keysym, false);
            break;
        }
    }

    gba_keypad_update(keys);
}

void frontend_frame(const uint32_t *buffer) {
    updateScreen(buffer);
    updateKeypad();
    updateFps();
}

void frontend_close() {
    SDL_FreeSurface(screenSurface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
