#include <cstdint>

#include <gbaemu/gbaemu.hpp>
#include <gbaemu/gba/cpu/impl/arm/nop.hpp>

namespace gbaemu::gba::cpu::impl::arm::nop {
    void opcode_nop(uint32_t opcode) {
        UNUSED(opcode);
    }
}
