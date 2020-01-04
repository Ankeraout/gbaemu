#pragma once

#include <cstdint>

#define DECLARE_MCAS_OPCODE(name) void opcode_ ## name(uint16_t opcode)

namespace gbaemu::gba::cpu::impl::thumb::mcas {
    DECLARE_MCAS_OPCODE(mov);
    DECLARE_MCAS_OPCODE(cmp);
    DECLARE_MCAS_OPCODE(add);
    DECLARE_MCAS_OPCODE(sub);
}
