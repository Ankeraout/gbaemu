#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lssebh.hpp>
#include <gbaemu/gba/mmu.hpp>

#define OPCODE_BASE \
    uint16_t Ro = (opcode & 0x01c0) >> 6; \
    uint16_t Rb = (opcode & 0x0038) >> 3; \
    uint16_t Rd = opcode & 0x0007; \
    uint32_t Rb_v = registerRead(Rb); \
    uint32_t Ro_v = registerRead(Ro)

namespace gbaemu::gba::cpu::impl::thumb::lssebh {
    void opcode_strh(uint16_t opcode) {
        OPCODE_BASE;
        mmu::write16(Rb_v + Ro_v, registerRead(Rd));
    }

    void opcode_ldrh(uint16_t opcode) {
        OPCODE_BASE;
        registerWrite(Rd, mmu::read16(Rb_v + Ro_v));
    }
    
    void opcode_ldrsb(uint16_t opcode) {
        OPCODE_BASE;
        registerWrite(Rd, (int8_t)mmu::read8(Rb_v + Ro_v));
    }
    
    void opcode_ldrsh(uint16_t opcode) {
        OPCODE_BASE;
        registerWrite(Rd, (int16_t)mmu::read16(Rb_v + Ro_v));
    }
}
