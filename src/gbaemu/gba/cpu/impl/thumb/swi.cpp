#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/swi.hpp>

namespace gbaemu::gba::cpu::impl::thumb::swi {
    void opcode_swi(uint16_t opcode) {
        UNUSED(opcode);
        raiseSWI();
    }
}
