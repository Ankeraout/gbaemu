#pragma once

#include <cstdint>

#include <gbaemu/gba/cpu.hpp>

namespace gbaemu::gba::cpu::decoder::arm {
    armOpcodeCallback_t decode(uint32_t opcode);
}