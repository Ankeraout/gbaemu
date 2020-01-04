#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::thumb::mls {
    void opcode_stmia(uint16_t opcode);
    void opcode_ldmia(uint16_t opcode);
}
