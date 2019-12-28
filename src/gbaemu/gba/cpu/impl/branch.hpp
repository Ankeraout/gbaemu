#pragma once

#include <stdint.h>

namespace gbaemu::gba::cpu::impl::branch {
    extern void b(uint32_t opcode);
    extern void bl(uint32_t opcode);
    extern void bx(uint32_t opcode);
}
