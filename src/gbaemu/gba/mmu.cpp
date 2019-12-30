#include <gbaemu/gbaemu.hpp>
#include <gbaemu/gba/gba.hpp>
#include <gbaemu/gba/mmu.hpp>

#define access32(t, i) (*((uint32_t *)&t[i]))

namespace gbaemu::gba::mmu {
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
                return 0; // TODO

            case 5: // BG/OBJ Palette RAM
                return 0; // TODO

            case 6: // VRAM
                return 0; // TODO
            
            case 7: // OAM
                return 0; // TODO

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
                return 0; // TODO
        }

        return 0; // Dead code, but removes a warning
    }

    void write32(uint32_t address, uint32_t value) {
        UNUSED(address);
        UNUSED(value);
        // TODO
    }
}
