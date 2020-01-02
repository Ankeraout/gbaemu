#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::decoder {
    typedef void (*armOpcodeCallback_t)(uint32_t opcode);
    typedef void (*thumbOpcodeCallback_t)(uint16_t opcode);
}
