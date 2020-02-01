#include <cstdint>

#include <gbaemu/gbaemu.hpp>
#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/swi.hpp>

namespace gbaemu::gba::cpu::impl::arm::swi {
    void swi(uint32_t opcode) {
        UNUSED(opcode);
        raiseSWI();
    }
}
