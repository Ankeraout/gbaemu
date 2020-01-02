#pragma once

#include <cstdint>

#include <gbaemu/gba/cpu/impl/arm/shift_inline.hpp>

#define DECLARE_SDT_OPCODE_SINGLE(name, suffix) \
    extern void opcode_ ## name ## _ ## suffix (uint32_t opcode)

#define DECLARE_SDT_OPCODE(name) \
    DECLARE_SDT_OPCODE_SINGLE(name, LSL_Rm_0); \
    DECLARE_SDT_OPCODE_SINGLE(name, LSL_Rm_1); \
    DECLARE_SDT_OPCODE_SINGLE(name, LSR_Rm_0); \
    DECLARE_SDT_OPCODE_SINGLE(name, LSR_Rm_1); \
    DECLARE_SDT_OPCODE_SINGLE(name, ASR_Rm_0); \
    DECLARE_SDT_OPCODE_SINGLE(name, ASR_Rm_1); \
    DECLARE_SDT_OPCODE_SINGLE(name, ROR_Rm_0); \
    DECLARE_SDT_OPCODE_SINGLE(name, ROR_Rm_1); \
    DECLARE_SDT_OPCODE_SINGLE(name, Immediate)

#define DECLARE_SDT_PREFIX(opcode) \
    DECLARE_SDT_OPCODE(opcode); \
    DECLARE_SDT_OPCODE(opcode ## b); \
    DECLARE_SDT_OPCODE(opcode ## u); \
    DECLARE_SDT_OPCODE(opcode ## ub); \
    DECLARE_SDT_OPCODE(opcode ## p); \
    DECLARE_SDT_OPCODE(opcode ## pw); \
    DECLARE_SDT_OPCODE(opcode ## pb); \
    DECLARE_SDT_OPCODE(opcode ## pbw); \
    DECLARE_SDT_OPCODE(opcode ## pu); \
    DECLARE_SDT_OPCODE(opcode ## puw); \
    DECLARE_SDT_OPCODE(opcode ## pub); \
    DECLARE_SDT_OPCODE(opcode ## pubw); \

namespace gbaemu::gba::cpu::impl::arm::sdt {
    DECLARE_SDT_PREFIX(ldr)
    DECLARE_SDT_PREFIX(str)
}
