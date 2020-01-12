#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/sds.hpp>
#include <gbaemu/gba/mmu.hpp>

#define OPCODE_BASE \
    const uint32_t Rn = (opcode & 0x000f0000) >> 16; \
    const uint32_t Rd = (opcode & 0x0000f000) >> 12; \
    const uint32_t Rm = opcode & 0x0000000f; \
    \
    const uint32_t Rn_v = registerRead(Rn); \
    const uint32_t Rm_v = registerRead(Rm)

namespace gbaemu::gba::cpu::impl::arm::sds {
    void opcode_swp(uint32_t opcode) {
        OPCODE_BASE;
        const uint32_t Rd_v = mmu::read32(Rn_v);
        mmu::write32(Rn_v, Rm_v);
        registerWrite(Rd, Rd_v);
    }

    void opcode_swpb(uint32_t opcode) {
        OPCODE_BASE;
        const uint8_t Rd_v = mmu::read8(Rn_v);
        mmu::write8(Rn_v, Rm_v);
        registerWrite(Rd, (registerRead(Rd) & 0xffffff00) | Rd_v);
    }
}