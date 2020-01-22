#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/ldrstrhw.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/mmu.hpp>

#define OPCODE_BASE \
    const uint16_t offset = (opcode & 0x07c0) >> 5; \
    const uint16_t Rb = (opcode & 0x0038) >> 3; \
    const uint16_t Rd = opcode & 0x0007; \
    \
    uint32_t address = registerRead(Rb) + offset

namespace gbaemu::gba::cpu::impl::thumb::ldrstrhw {
    void opcode_ldrh(uint16_t opcode) {
        OPCODE_BASE;
        uint16_t read = mmu::read16(address);
        registerWrite(Rd, ROR32(read, ((address & 0x00000001) ? 8 : 0)));
    }

    void opcode_strh(uint16_t opcode) {
        OPCODE_BASE;
        mmu::write16(address, registerRead(Rd));
    }
}
