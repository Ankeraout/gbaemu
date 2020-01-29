#include <stdio.h>

#include <gbaemu/gbaemu.hpp>
#include <gbaemu/gba/gba.hpp>
#include <gbaemu/gba/io.hpp>
#include <gbaemu/gba/lcd.hpp>
#include <gbaemu/gba/mmu.hpp>

#define access8(t, i) (*((uint8_t *)&t[i]))
#define access16(t, i) (*((uint16_t *)&t[i]))
#define access32(t, i) (*((uint32_t *)&t[i]))
#define concat(x, y) x ## y

namespace gbaemu::gba::mmu {
    uint8_t read8(uint32_t address) {
        switch((address >> 24) & 0x0f) {
            case 0: // BIOS
                return access8(gbaemu::gba::biosData, address & 0x00003fff);

            case 1: // Unused
                return 0;

            case 2: // Slow WRAM
                return access8(gbaemu::gba::wramData_slow, address & 0x0003ffff);

            case 3: // Fast WRAM
                return access8(gbaemu::gba::wramData_fast, address & 0x00007fff);

            case 4: // I/O registers
                return gbaemu::gba::io::read8(address);

            case 5: // BG/OBJ Palette RAM
                return access8(gbaemu::gba::lcd::palette, address & 0x000003ff);

            case 6: // VRAM
                if((address & 0x0001ffff) <= 0x00017fff) {
                    return access8(gbaemu::gba::lcd::vram, address & 0x0001ffff);
                } else {
                    return 0;
                }

            case 7: // OAM
                return access8(gbaemu::gba::lcd::oam, address & 0x000003ff);

            case 8: // Game Pak ROM (wait state 0)
            case 9:
                return access8(gbaemu::gba::cartridge::romData, address & gbaemu::gba::cartridge::romAddressMask);

            case 0xa: // Game Pak ROM (wait state 1)
            case 0xb:
                return access8(gbaemu::gba::cartridge::romData, address & gbaemu::gba::cartridge::romAddressMask);

            case 0xc: // Game Pak ROM (wait state 2)
            case 0xd:
                return access8(gbaemu::gba::cartridge::romData, address & gbaemu::gba::cartridge::romAddressMask);

            case 0xe: // Game Pak SRAM
            case 0xf:
                printf("Read8 from Game Pak SRAM\n");
                return 0; // TODO
        }

        return 0; // Dead code, but removes a warning
    }

    uint16_t read16(uint32_t address) {
        address &= 0xfffffffe;

        switch((address >> 24) & 0x0f) {
            case 0: // BIOS
                return access16(gbaemu::gba::biosData, address & 0x00003fff);

            case 1: // Unused
                return 0;

            case 2: // Slow WRAM
                return access16(gbaemu::gba::wramData_slow, address & 0x0003ffff);

            case 3: // Fast WRAM
                return access16(gbaemu::gba::wramData_fast, address & 0x00007fff);

            case 4: // I/O registers
                return gbaemu::gba::io::read16(address);

            case 5: // BG/OBJ Palette RAM
                return access16(gbaemu::gba::lcd::palette, address & 0x000003ff);

            case 6: // VRAM
                if((address & 0x0001ffff) <= 0x00017fff) {
                    return access16(gbaemu::gba::lcd::vram, address & 0x0001ffff);
                } else {
                    return 0;
                }

            case 7: // OAM
                return access16(gbaemu::gba::lcd::oam, address & 0x000003ff);

            case 8: // Game Pak ROM (wait state 0)
            case 9:
                return access16(gbaemu::gba::cartridge::romData, address & gbaemu::gba::cartridge::romAddressMask);

            case 0xa: // Game Pak ROM (wait state 1)
            case 0xb:
                return access16(gbaemu::gba::cartridge::romData, address & gbaemu::gba::cartridge::romAddressMask);

            case 0xc: // Game Pak ROM (wait state 2)
            case 0xd:
                return access16(gbaemu::gba::cartridge::romData, address & gbaemu::gba::cartridge::romAddressMask);

            case 0xe: // Game Pak SRAM
            case 0xf:
                printf("Read16 from Game Pak SRAM\n");
                return 0; // TODO
        }

        return 0; // Dead code, but removes a warning
    }

    uint32_t read32(uint32_t address) {
        // As stated in http://problemkaputt.de/gbatek.htm#armcpumemoryalignments,
        // ARM CPUs do not support mis-aligned memory accesses. 32-bit access must
        // be 32-bit aligned.
        address &= 0xfffffffc;

        switch((address >> 24) & 0x0f) {
            case 0: // BIOS
                return access32(gbaemu::gba::biosData, address & 0x00003fff);

            case 1: // Unused
                return 0;

            case 2: // Slow WRAM
                return access32(gbaemu::gba::wramData_slow, address & 0x0003ffff);

            case 3: // Fast WRAM
                return access32(gbaemu::gba::wramData_fast, address & 0x00007fff);

            case 4: // I/O registers
                return gbaemu::gba::io::read32(address);

            case 5: // BG/OBJ Palette RAM
                return access32(gbaemu::gba::lcd::palette, address & 0x000003ff);

            case 6: // VRAM
                if((address & 0x0001ffff) <= 0x00017fff) {
                    return access32(gbaemu::gba::lcd::vram, address & 0x0001ffff);
                } else {
                    return 0;
                }
            
            case 7: // OAM
                return access32(gbaemu::gba::lcd::oam, address & 0x000003ff);

            case 8: // Game Pak ROM (wait state 0)
            case 9:
                return access32(gbaemu::gba::cartridge::romData, address & gbaemu::gba::cartridge::romAddressMask);

            case 0xa: // Game Pak ROM (wait state 1)
            case 0xb:
                return access32(gbaemu::gba::cartridge::romData, address & gbaemu::gba::cartridge::romAddressMask);

            case 0xc: // Game Pak ROM (wait state 2)
            case 0xd:
                return access32(gbaemu::gba::cartridge::romData, address & gbaemu::gba::cartridge::romAddressMask);

            case 0xe: // Game Pak SRAM
            case 0xf:
                printf("Read32 from Game Pak SRAM\n");
                return 0; // TODO
        }

        return 0; // Dead code, but removes a warning
    }

    void write8(uint32_t address, uint8_t value) {
        switch((address >> 24) & 0x0f) {
            case 0: // BIOS
            case 1: // Unused
                break;

            case 2: // Slow WRAM
                access8(gbaemu::gba::wramData_slow, address & 0x0003ffff) = value;
                break;

            case 3: // Fast WRAM
                access8(gbaemu::gba::wramData_fast, address & 0x00007fff) = value;
                break;

            case 4: // I/O registers
                gbaemu::gba::io::write8(address, value);
                break;

            case 5: // BG/OBJ Palette RAM
                access16(gbaemu::gba::lcd::palette, address & 0x000003fe) = value | (value << 8);
                break;

            case 6: // VRAM
                if((address & 0x0001ffff) <= 0x00017fff) {
                    access16(gbaemu::gba::lcd::vram, address & 0x0001fffe) = value | (value << 8);
                }

                break;

            case 7: // OAM
                access16(gbaemu::gba::lcd::oam, address & 0x000003fe) = value | (value << 8);
                break;

            case 8: // Game Pak ROM (wait state 0)
            case 9:
            case 0xa: // Game Pak ROM (wait state 1)
            case 0xb:
            case 0xc: // Game Pak ROM (wait state 2)
            case 0xd:
                break;

            case 0xe: // Game Pak SRAM
            case 0xf:
                printf("Write8 to GamePak SRAM\n");
                break; // TODO
        }
    }

    void write16(uint32_t address, uint16_t value) {
        address &= 0xfffffffe;

        switch((address >> 24) & 0x0f) {
            case 0: // BIOS
            case 1: // Unused
                break;

            case 2: // Slow WRAM
                access16(gbaemu::gba::wramData_slow, address & 0x0003ffff) = value;
                break;

            case 3: // Fast WRAM
                access16(gbaemu::gba::wramData_fast, address & 0x00007fff) = value;
                break;

            case 4: // I/O registers
                gbaemu::gba::io::write16(address, value);
                break;

            case 5: // BG/OBJ Palette RAM
                access16(gbaemu::gba::lcd::palette, address & 0x000003ff) = value;
                break;

            case 6: // VRAM
                if((address & 0x0001ffff) <= 0x00017fff) {
                    access16(gbaemu::gba::lcd::vram, address & 0x0001ffff) = value;
                }
                
                break;

            case 7: // OAM
                access16(gbaemu::gba::lcd::oam, address & 0x000003ff) = value;
                break;

            case 8: // Game Pak ROM (wait state 0)
            case 9:
            case 0xa: // Game Pak ROM (wait state 1)
            case 0xb:
            case 0xc: // Game Pak ROM (wait state 2)
            case 0xd:
                break;

            case 0xe: // Game Pak SRAM
            case 0xf:
                printf("Write16 to GamePak SRAM\n");
                break; // TODO
        }
    }

    void write32(uint32_t address, uint32_t value) {
        // As stated in http://problemkaputt.de/gbatek.htm#armcpumemoryalignments,
        // ARM CPUs do not support mis-aligned memory accesses. 32-bit access must
        // be 32-bit aligned.
        address &= 0xfffffffc;

        switch((address >> 24) & 0x0f) {
            case 0: // BIOS
            case 1: // Unused
                break;

            case 2: // Slow WRAM
                access32(gbaemu::gba::wramData_slow, address & 0x0003ffff) = value;
                break;

            case 3: // Fast WRAM
                access32(gbaemu::gba::wramData_fast, address & 0x00007fff) = value;
                break;

            case 4: // I/O registers
                gbaemu::gba::io::write32(address, value);
                break;

            case 5: // BG/OBJ Palette RAM
                access32(gbaemu::gba::lcd::palette, address & 0x000003ff) = value;
                break;

            case 6: // VRAM
                if((address & 0x0001ffff) <= 0x00017fff) {
                    access32(gbaemu::gba::lcd::vram, address & 0x0001ffff) = value;
                }

                break;

            case 7: // OAM
                access32(gbaemu::gba::lcd::oam, address & 0x000003ff) = value;
                break;

            case 8: // Game Pak ROM (wait state 0)
            case 9:
            case 0xa: // Game Pak ROM (wait state 1)
            case 0xb:
            case 0xc: // Game Pak ROM (wait state 2)
            case 0xd:
                break;

            case 0xe: // Game Pak SRAM
            case 0xf:
                printf("Write32 to GamePak SRAM\n");
                break; // TODO
        }
    }
}
