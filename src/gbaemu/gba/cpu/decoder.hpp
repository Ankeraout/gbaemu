#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::decoder {
    typedef void (*opcodeCallback_t)(uint32_t opcode);
}
