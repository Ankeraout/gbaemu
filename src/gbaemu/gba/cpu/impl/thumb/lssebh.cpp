#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lssebh.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/mmu.hpp>

#define OPCODE_BASE \
    uint16_t Ro = (opcode & 0x01c0) >> 6; \
    uint16_t Rb = (opcode & 0x0038) >> 3; \
    uint16_t Rd = opcode & 0x0007; \
    uint32_t Rb_v = registerRead(Rb); \
    uint32_t Ro_v = registerRead(Ro); \
    uint32_t accessAddress = Rb_v + Ro_v

namespace gbaemu::gba::cpu::impl::thumb::lssebh {
    void opcode_strh(uint16_t opcode) {
        OPCODE_BASE;
        mmu::write16(accessAddress, registerRead(Rd));
    }

    void opcode_ldrh(uint16_t opcode) {
        OPCODE_BASE;
        const uint16_t readValue = mmu::read16(accessAddress);
        registerWrite(Rd, ROR32(readValue, (accessAddress & 0x00000001) ? 8 : 0));
    }
    
    void opcode_ldrsb(uint16_t opcode) {
        OPCODE_BASE;
        registerWrite(Rd, (int8_t)mmu::read8(accessAddress));
    }
    
    void opcode_ldrsh(uint16_t opcode) {
        OPCODE_BASE;

        if(accessAddress & 0x00000001) {
            registerWrite(Rd, (int8_t)mmu::read8(accessAddress));
        } else {
            registerWrite(Rd, (int16_t)mmu::read16(accessAddress));
        }
    }
}
