#include <cstring>

#include <SDL2/SDL.h>

#include <gbaemu/gba/keypad.hpp>
#include <gbaemu/gba/lcd.hpp>
#include <gbaemu/gbaemu.hpp>
#include <gbaemu/frontend.hpp>

namespace gbaemu::frontend {
    SDL_Window *window;
    Uint32 nextFrameTime;
    Uint32 framerateCounterUpdateTime;
    Uint32 framerateCounter;
    Uint32 currentFramerate;

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
            "gbaemu [60 fps, 100%]",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            gbaemu::gba::lcd::screenWidth,
            gbaemu::gba::lcd::screenHeight,
            SDL_WINDOW_SHOWN
        );

        nextFrameTime = SDL_GetTicks();
        framerateCounter = 0;
        framerateCounterUpdateTime = SDL_GetTicks() + 1000;
        currentFramerate = 0;

        return !window;
    }

    int close() {
        SDL_Quit();
        return 0;
    }

    void limitFramerate() {
        while(nextFrameTime > SDL_GetTicks()) {
            SDL_Delay(1);
        }

        nextFrameTime = SDL_GetTicks() + 16;
    }

    void updateFramerate() {
        char buf[256];

        Uint32 currentTime = SDL_GetTicks();

        if(framerateCounterUpdateTime <= currentTime) {
            currentFramerate = framerateCounter + 1;
            framerateCounter = 0;
            framerateCounterUpdateTime = currentTime + 1000;

            sprintf(buf, "gbaemu [%d fps, %d%%]", currentFramerate, (currentFramerate * 100) / 60);

            SDL_SetWindowTitle(window, buf);
        } else {
            framerateCounter++;
        }
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

                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_CLOSE) {
                        quit();
                    }

                default:
                break;
            }
        }

        memcpy(surface->pixels, framebuffer, gbaemu::gba::lcd::screenWidth * gbaemu::gba::lcd::screenHeight * 4);

        int result = SDL_UpdateWindowSurface(window);

        updateFramerate();
        limitFramerate();

        return result;
    }
}
