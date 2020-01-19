# GBAEmu
GBAEmu is a GameBoy Advance emulator written in C++ by @Ankeraout.

## Supported platforms
One of the goals of this project is portability.
Here are the supported platforms:

- PC:
    - Windows (x86/x64) : Untested SDL2 dependency
    - Linux (x86/x64) : Supported
    - Mac OS X : Probably working, untested

## Dependencies
For now this project relies on [SDL2](https://www.libsdl.org/) for display and controls.
Because packed structures are used in the code, only GCC or Microsoft Visual C++ 2019 may be able to compile it, as there was no porting effort made for other compilers.