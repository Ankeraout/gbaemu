#pragma once

#include <cstdint>

#define DECLARE_HSDT_OPCODE_SINGLE(name) \
    void opcode_ ## name(uint32_t opcode)

#define DECLARE_HSDT_OPCODE(opcode) \
    DECLARE_HSDT_OPCODE_SINGLE(opcode); \
    DECLARE_HSDT_OPCODE_SINGLE(opcode ## u); \
    DECLARE_HSDT_OPCODE_SINGLE(opcode ## p); \
    DECLARE_HSDT_OPCODE_SINGLE(opcode ## pw); \
    DECLARE_HSDT_OPCODE_SINGLE(opcode ## pu); \
    DECLARE_HSDT_OPCODE_SINGLE(opcode ## puw)

#define DECLARE_HSDT_STR_PREFIX2(name) \
    DECLARE_HSDT_OPCODE(name ## h) \

#define DECLARE_HSDT_LDR_PREFIX2(name) \
    DECLARE_HSDT_OPCODE(name ## h); \
    DECLARE_HSDT_OPCODE(name ## s); \
    DECLARE_HSDT_OPCODE(name ## sh)

#define DECLARE_HSDT_STR_PREFIX \
    DECLARE_HSDT_STR_PREFIX2(str); \
    DECLARE_HSDT_STR_PREFIX2(stri)

#define DECLARE_HSDT_LDR_PREFIX \
    DECLARE_HSDT_LDR_PREFIX2(ldr); \
    DECLARE_HSDT_LDR_PREFIX2(ldri)

namespace gbaemu::gba::cpu::impl::arm::hsdt {
    DECLARE_HSDT_LDR_PREFIX;
    DECLARE_HSDT_STR_PREFIX;
}
