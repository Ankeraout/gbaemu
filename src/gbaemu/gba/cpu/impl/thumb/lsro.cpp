#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lsro.hpp>
#include <gbaemu/gba/mmu.hpp>

#define OPCODE_BASE \
    const uint16_t Ro = (opcode & 0x01c0) >> 6; \
    const uint16_t Rb = (opcode & 0x0038) >> 3; \
    const uint16_t Rd = opcode & 0x0007; \
    \
    const uint32_t Rb_v = registerRead(Rb); \
    const uint32_t Ro_v = registerRead(Ro); \
    \
    const uint32_t address = Rb_v + Ro_v

namespace gbaemu::gba::cpu::impl::thumb::lsro {
    void opcode_str(uint16_t opcode) {
        OPCODE_BASE;
        const uint32_t Rd_v = registerRead(Rd);
        mmu::write32(address, Rd_v);
    }

    void opcode_strb(uint16_t opcode) {
        OPCODE_BASE;
        const uint32_t Rd_v = registerRead(Rd);
        mmu::write8(address, Rd_v);
    }
    
    void opcode_ldr(uint16_t opcode) {
        OPCODE_BASE;
        registerWrite(Rd, mmu::read32(address));
    }
    
    void opcode_ldrb(uint16_t opcode) {
        OPCODE_BASE;
        registerWrite(Rd, mmu::read8(address));
    }
}
