#pragma once

#include <cstdint>

#define DECLARE_BDT_OPCODE_SINGLE(name) \
    void opcode_ ## name(uint32_t opcode)

#define DECLARE_BDT_OPCODE(name) \
    DECLARE_BDT_OPCODE_SINGLE(name ## da); \
    DECLARE_BDT_OPCODE_SINGLE(name ## das); \
    DECLARE_BDT_OPCODE_SINGLE(name ## ia); \
    DECLARE_BDT_OPCODE_SINGLE(name ## ias); \
    DECLARE_BDT_OPCODE_SINGLE(name ## db); \
    DECLARE_BDT_OPCODE_SINGLE(name ## dbw); \
    DECLARE_BDT_OPCODE_SINGLE(name ## dbs); \
    DECLARE_BDT_OPCODE_SINGLE(name ## dbsw); \
    DECLARE_BDT_OPCODE_SINGLE(name ## ib); \
    DECLARE_BDT_OPCODE_SINGLE(name ## ibw); \
    DECLARE_BDT_OPCODE_SINGLE(name ## ibs); \
    DECLARE_BDT_OPCODE_SINGLE(name ## ibsw)

namespace gbaemu::gba::cpu::impl::arm::bdt {
    DECLARE_BDT_OPCODE(ldm);
    DECLARE_BDT_OPCODE(stm);
}
