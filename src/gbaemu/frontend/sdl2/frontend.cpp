#include <cstring>

#include <SDL2/SDL.h>

#include <gbaemu/gba/keypad.hpp>
#include <gbaemu/gba/lcd.hpp>

#include <gbaemu/frontend.hpp>

namespace gbaemu::frontend {
    SDL_Window *window;

    SDL_Keycode keyBindings[] = {
        SDLK_a,
        SDLK_b,
        SDLK_RSHIFT,
        SDLK_RETURN,
        SDLK_RIGHT,
        SDLK_LEFT,
        SDLK_UP,
        SDLK_DOWN,
        SDLK_r,
        SDLK_l
    };

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
            switch(event.type) {
                case SDL_KEYDOWN:
                    for(int i = 0; i < 10; i++) {
                        if(event.key.keysym.sym == keyBindings[i]) {
                            gbaemu::gba::keypad::keyDown((gbaemu::gba::keypad::key_t)(1 << i));
                            break;
                        }
                    }

                    break;

                case SDL_KEYUP:
                    for(int i = 0; i < 10; i++) {
                        if(event.key.keysym.sym == keyBindings[i]) {
                            gbaemu::gba::keypad::keyUp((gbaemu::gba::keypad::key_t)(1 << i));
                            break;
                        }
                    }

                    break;

                default:
                break;
            }
        }

        memcpy(surface->pixels, framebuffer, gbaemu::gba::lcd::screenWidth * gbaemu::gba::lcd::screenHeight * 4);

        return SDL_UpdateWindowSurface(window);
    }
}
