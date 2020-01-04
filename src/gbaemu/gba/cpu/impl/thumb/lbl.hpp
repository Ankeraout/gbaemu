#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::thumb::lbl {
    void opcode_bl(uint16_t opcode);
    void opcode_blh(uint16_t opcode);
}
