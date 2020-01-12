#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/sprldstr.hpp>
#include <gbaemu/gba/mmu.hpp>

#define OPCODE_BASE \
    const uint16_t Rd = (opcode & 0x07) >> 8; \
    const uint32_t immediate = (opcode & 0x00ff) << 2; \
    \
    uint32_t address = registerRead(CPU_REG_SP) + immediate;

namespace gbaemu::gba::cpu::impl::thumb::sprldstr {
    void opcode_ldr(uint16_t opcode) {
        OPCODE_BASE;
        registerWrite(Rd, mmu::read32(address));
    }

    void opcode_str(uint16_t opcode) {
        OPCODE_BASE;
        const uint32_t Rd_v = registerRead(Rd);
        mmu::write32(address, Rd_v);
    }
}
