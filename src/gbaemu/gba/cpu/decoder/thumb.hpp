#pragma once

#include <gbaemu/gba/cpu/decoder.hpp>

namespace gbaemu::gba::cpu::decoder::thumb {
    thumbOpcodeCallback_t decode(uint16_t opcode);
}
