#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/ldstrhw.hpp>
#include <gbaemu/gba/mmu.hpp>

#define OPCODE_BASE \
    const uint16_t offset = (opcode & 0x07c0) >> 6; \
    const uint16_t Rb = (opcode & 0x0038) >> 3; \
    const uint16_t Rd = opcode & 0x0007; \
    \
    uint32_t address = registerRead(Rb) + offset

namespace gbaemu::gba::cpu::impl::thumb::ldrstrhw {
    void opcode_ldrh(uint16_t opcode) {
        OPCODE_BASE;
        registerWrite(Rd, mmu::read16(address));
    }

    void opcode_strh(uint16_t opcode) {
        OPCODE_BASE;
        mmu::write16(address, registerRead(Rd));
    }
}