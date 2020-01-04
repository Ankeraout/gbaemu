#pragma once

#include <cstdint>

#define DECLARE_LSIO_OPCODE(op) \
    void opcode_ ## op(uint16_t opcode); \
    void opcode_ ## op ## b(uint16_t opcode)

namespace gbaemu::gba::cpu::impl::thumb::lsio {
    DECLARE_LSIO_OPCODE(str);
    DECLARE_LSIO_OPCODE(ldr);
}
