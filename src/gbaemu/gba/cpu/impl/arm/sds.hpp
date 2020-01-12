#pragma once

#include <cstdint>

namespace gbaemu::gba::cpu::impl::arm::sds {
    void opcode_swp(uint32_t opcode);
    void opcode_swpb(uint32_t opcode);
}
