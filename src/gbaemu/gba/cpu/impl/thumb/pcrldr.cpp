#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/pcrldr.hpp>
#include <gbaemu/gba/mmu.hpp>

namespace gbaemu::gba::cpu::impl::thumb::pcrldr {
    extern void pcrldr(uint16_t opcode) {
        unsigned int Rd = (opcode & 0x0700) >> 8;
        unsigned int imm = (opcode & 0x00ff) << 2;
        
        // PC bit 1 is forced to 0 so that the address is word-aligned
        uint32_t address = (gbaemu::gba::cpu::registerRead(CPU_REG_PC) & 0xfffffffc) + imm;

        gbaemu::gba::cpu::registerWrite(Rd, gbaemu::gba::mmu::read32(address));
    }
}
