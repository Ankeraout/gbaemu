#pragma once

#include <cstdint>

#define DECLARE_BDT_OPCODE_SINGLE(name) \
    void opcode_ ## name(uint32_t opcode)

#define DECLARE_BDT_OPCODE_VARIANTS(name) \
    DECLARE_BDT_OPCODE_SINGLE(name); \
    DECLARE_BDT_OPCODE_SINGLE(name ## w); \
    DECLARE_BDT_OPCODE_SINGLE(name ## s); \
    DECLARE_BDT_OPCODE_SINGLE(name ## sw)

#define DECLARE_BDT_OPCODE(name) \
    DECLARE_BDT_OPCODE_VARIANTS(name ## da); \
    DECLARE_BDT_OPCODE_VARIANTS(name ## ia); \
    DECLARE_BDT_OPCODE_VARIANTS(name ## db); \
    DECLARE_BDT_OPCODE_VARIANTS(name ## ib)

namespace gbaemu::gba::cpu::impl::arm::bdt {
    DECLARE_BDT_OPCODE(ldm);
    DECLARE_BDT_OPCODE(stm);
}
