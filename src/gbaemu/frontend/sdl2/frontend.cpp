#include <cstring>

#include <SDL2/SDL.h>

#include <gbaemu/gba/lcd.hpp>

#include <gbaemu/frontend.hpp>

namespace gbaemu::frontend {
    SDL_Window *window;

    int init() {
        int returnValue = SDL_Init(SDL_INIT_VIDEO);

        if(returnValue) {
            return returnValue;
        }

        window = SDL_CreateWindow(
            "gbaemu",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            gbaemu::gba::lcd::screenWidth,
            gbaemu::gba::lcd::screenHeight,
            SDL_WINDOW_SHOWN
        );

        return !window;
    }

    int close() {
        SDL_Quit();
        return 0;
    }

    int update() {
        SDL_Event event;
        SDL_Surface *surface = SDL_GetWindowSurface(window);
        const uint32_t *framebuffer = gbaemu::gba::lcd::getFramebuffer();

        //memset((void *)framebuffer, 255, 240 * 160 * 4);

        while(SDL_PollEvent(&event)) {

        }

        memcpy(surface->pixels, framebuffer, gbaemu::gba::lcd::screenWidth * gbaemu::gba::lcd::screenHeight * 4);

        return SDL_UpdateWindowSurface(window);
    }
}
