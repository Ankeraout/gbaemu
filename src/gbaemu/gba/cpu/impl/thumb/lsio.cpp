#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lsio.hpp>
#include <gbaemu/gba/mmu.hpp>

#define DEFINE_LSIO_OPCODE(op, body, offsetModifier) \
    void opcode_ ## op(uint16_t opcode) { \
        uint16_t offset = ((opcode & 0x07c0) >> 6) offsetModifier; \
        uint16_t Rb = (opcode & 0x0038) >> 3; \
        uint16_t Rd = opcode & 0x0007; \
        uint32_t Rb_v = registerRead(Rb); \
        uint32_t address = Rb_v + offset; \
        \
        body; \
    }

namespace gbaemu::gba::cpu::impl::thumb::lsio {
    DEFINE_LSIO_OPCODE(str, mmu::write32(address, registerRead(Rd)), << 2)

    DEFINE_LSIO_OPCODE(
        ldr,
        registerWrite(Rd, ROR32(gbaemu::gba::mmu::read32(address), (address & 0x03) << 3)),
        << 2
    )

    DEFINE_LSIO_OPCODE(strb, mmu::write8(address, registerRead(Rd)), )
    DEFINE_LSIO_OPCODE(ldrb, registerWrite(Rd, mmu::read8(address)), )
}
