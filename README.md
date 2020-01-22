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

## Test ROMs
In order to build test ROMs, you need to have [FASMARM](https://arm.flatassembler.net/) in your path. Then just use `make testroms` to build them.
The .gba ROM files will be right next to the corresponding .asm file.
The `header.inc` file is not a test ROM itself, but it is included by every test ROM.
It contains the ROM header for all test ROMs, with a precalculated header checksum.
