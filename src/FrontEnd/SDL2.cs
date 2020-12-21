using SDL2;
using System;

namespace gbaemu.FrontEnd {
    public class SDL2 : IFrontEnd {
        private const int SCREEN_WIDTH = 240;
        private const int SCREEN_HEIGHT = 160;
        private const int SCREEN_SCALE = 2;
        private const int WINDOW_WIDTH = SCREEN_WIDTH * SCREEN_SCALE;
        private const int WINDOW_HEIGHT = SCREEN_HEIGHT * SCREEN_SCALE;

        private GBA.GBA gba;
        private IntPtr window;
        private IntPtr windowSurface;
        private IntPtr screenSurface;
        private SDL.SDL_Rect screenSurfaceRect;
        private SDL.SDL_Rect windowSurfaceRect;

        private bool[] keys;

        public SDL2(GBA.GBA gba) {
            this.gba = gba;

            window = SDL.SDL_CreateWindow("gbaemu", SDL.SDL_WINDOWPOS_UNDEFINED, SDL.SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL.SDL_WindowFlags.SDL_WINDOW_SHOWN);

            if(window == IntPtr.Zero) {
                throw new Exception("SDL_CreateWindow() failed: " + SDL.SDL_GetError());
            }

            windowSurface = SDL.SDL_GetWindowSurface(window);

            if(windowSurface == IntPtr.Zero) {
                throw new Exception("SDL_GetWindowSurface() failed:" + SDL.SDL_GetError());
            }

            windowSurfaceRect = new SDL.SDL_Rect() {x = 0, y = 0, w = WINDOW_WIDTH, h = WINDOW_HEIGHT};

            screenSurface = SDL.SDL_CreateRGBSurface(SDL.SDL_SWSURFACE, 240, 160, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
            screenSurfaceRect = new SDL.SDL_Rect() {x = 0, y = 0, w = SCREEN_WIDTH, h = SCREEN_HEIGHT};

            if(screenSurface == IntPtr.Zero) {
                throw new Exception("SDL_CreateRGBSurface() failed: " + SDL.SDL_GetError());
            }

            keys = new bool[10];
        }

        private unsafe void Frame_UpdatePixels(uint[] color) {
            SDL.SDL_Surface *s = (SDL.SDL_Surface *)screenSurface.ToPointer();

            uint *pixels = (uint *)s->pixels.ToPointer();

            for(int i = 0; i < 240 * 160; i++) {
                pixels[i] = color[i];
            }
        }

        private void UpdateKey(SDL.SDL_Keysym keysym, bool pressed) {
            switch(keysym.sym) {
                case SDL.SDL_Keycode.SDLK_a: keys[0] = pressed; break;
                case SDL.SDL_Keycode.SDLK_b: keys[1] = pressed; break;
                case SDL.SDL_Keycode.SDLK_RSHIFT: keys[2] = pressed; break;
                case SDL.SDL_Keycode.SDLK_RETURN: keys[3] = pressed; break;
                case SDL.SDL_Keycode.SDLK_RIGHT: keys[4] = pressed; break;
                case SDL.SDL_Keycode.SDLK_LEFT: keys[5] = pressed; break;
                case SDL.SDL_Keycode.SDLK_UP: keys[6] = pressed; break;
                case SDL.SDL_Keycode.SDLK_DOWN: keys[7] = pressed; break;
                case SDL.SDL_Keycode.SDLK_r: keys[8] = pressed; break;
                case SDL.SDL_Keycode.SDLK_l: keys[9] = pressed; break;
            }
        }

        private void Frame_UpdateKeypad() {
            SDL.SDL_Event e;

            while(SDL.SDL_PollEvent(out e) == 1) {
                switch(e.type) {
                    case SDL.SDL_EventType.SDL_WINDOWEVENT:
                    switch(e.window.windowEvent) {
                        case SDL.SDL_WindowEventID.SDL_WINDOWEVENT_CLOSE:
                        throw new QuitException();
                    }
                    break;

                    case SDL.SDL_EventType.SDL_KEYDOWN:
                    UpdateKey(e.key.keysym, true);
                    break;

                    case SDL.SDL_EventType.SDL_KEYUP:
                    UpdateKey(e.key.keysym, false);
                    break;
                }
            }

            gba.Keypad.UpdatePressedKeys(keys);
        }

        public void Frame(uint[] color) {
            Frame_UpdatePixels(color);

            SDL.SDL_BlitScaled(screenSurface, ref screenSurfaceRect, windowSurface, ref windowSurfaceRect);
            SDL.SDL_UpdateWindowSurface(window);

            Frame_UpdateKeypad();
        }

        public void Close() {
            SDL.SDL_FreeSurface(screenSurface);
            SDL.SDL_DestroyWindow(window);
            SDL.SDL_Quit();
        }
    }
}
