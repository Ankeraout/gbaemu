#pragma once

#include <cstdint>

#define DECLARE_HROBE_OPCODE(op) \
    void opcode_ ## op(uint16_t opcode)

#define DECLARE_HROBE_OPCODE_VARIANTS(op) \
    DECLARE_HROBE_OPCODE(op ## _Rd_Hs); \
    DECLARE_HROBE_OPCODE(op ## _Hd_Rs); \
    DECLARE_HROBE_OPCODE(op ## _Hd_Hs)

namespace gbaemu::gba::cpu::impl::thumb::hrobe {
    DECLARE_HROBE_OPCODE_VARIANTS(add);
    DECLARE_HROBE_OPCODE_VARIANTS(cmp);
    DECLARE_HROBE_OPCODE_VARIANTS(mov);
    void opcode_bx_Rs(uint16_t opcode);
    void opcode_bx_Hs(uint16_t opcode);
}
