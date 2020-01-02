#pragma once

#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/logic_inline.hpp>

#define HEADER_SHIFT_RM_0 \
    uint32_t Rm = opcode & 0x0000000f; \
    uint32_t immediate = (opcode & 0x00000f80) >> 7; \
    uint32_t Rm_v = registerRead(Rm)

#define HEADER_SHIFT_RM_1 \
    uint32_t Rm = opcode & 0x0000000f; \
    uint32_t Rs = (opcode & 0x00000f00) >> 8; \
    uint32_t Rm_v = registerRead(Rm); \
    uint32_t Rs_v = registerRead(Rs); \
    \
    uint32_t shift = Rs_v; \
    \
    if(Rs == CPU_REG_PC) { \
        shift += 4; \
    } \
    \
    shift &= 0xff; \
    \
    uint32_t value = Rm_v; \
    \
    if(Rm == CPU_REG_PC) { \
        value += 4; \
    }

namespace gbaemu::gba::cpu::impl::arm::shift {
    static inline void shiftLSL_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;

        if(immediate) {
            shifter.result = Rm_v << immediate;
            shifter.flagC = (Rm_v >> (32 - immediate)) & 0x00000001;
        } else {
            shifter.result = registerRead(Rm);
            shifter.flagC = cpsr.fields.flagC;
        }
    }

    static inline void shiftLSL_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;

        if(!shift) {
            shifter.result = value;
            shifter.flagC = cpsr.fields.flagC;
        } else if(shift < 32) {
            shifter.result = value << shift;
            shifter.flagC = (value >> (32 - shift)) & 0x00000001;
        } else if(shift == 32) {
            shifter.result = 0;
            shifter.flagC = value & 0x00000001;
        } else {
            shifter.result = 0;
            shifter.flagC = false;
        }
    }

    static inline void shiftLSR_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;

        if(immediate) {
            shifter.result = Rm_v >> immediate;
            shifter.flagC = (Rm_v >> (immediate - 1)) & 0x00000001;
        } else {
            shifter.result = 0;
            shifter.flagC = SIGN32(Rm_v);
        }
    }

    static inline void shiftLSR_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;

        if(!shift) {
            shifter.result = value;
            shifter.flagC = cpsr.fields.flagC;
        } else if(shift < 32) {
            shifter.result = value >> shift;
            shifter.flagC = (value >> (shift - 1)) & 0x00000001;
        } else if(shift == 32) {
            shifter.result = 0;
            shifter.flagC = SIGN32(value);
        } else {
            shifter.result = 0;
            shifter.flagC = false;
        }
    }

    static inline void shiftASR_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;

        if(immediate) {
            shifter.result = Rm_v >> immediate;
            shifter.flagC = (Rm_v >> (immediate - 1)) & 0x00000001;
        } else {
            shifter.result = SIGN32(Rm_v);
            shifter.flagC = shifter.flagC;
        }
    }

    static inline void shiftASR_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;

        if(!shift) {
            shifter.result = value;
            shifter.flagC = cpsr.fields.flagC;
        } else if(shift < 32) {
            shifter.result = value >> shift;
            shifter.flagC = (value >> (shift - 1)) & 0x00000001;
        } else if(SIGN32(Rm_v)) {
            shifter.result = 0xffffffff;
            shifter.flagC = true;
        } else {
            shifter.result = 0;
            shifter.flagC = false;
        }
    }

    static inline void shiftROR_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;

        if(immediate) {
            shifter.result = ROR(Rm_v, immediate);
            shifter.flagC = (Rm_v >> (immediate - 1)) & 0x00000001;
        } else {
            // RRX
            shifter.result = (cpsr.fields.flagC << 31) | (Rm_v >> 1);
            shifter.flagC = Rm_v & 0x00000001;
        }
    }

    static inline void shiftROR_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;

        uint32_t rotation = shift & 0x1f;

        if(!shift) {
            shifter.result = value;
            shifter.flagC = cpsr.fields.flagC;
        } else if(rotation) {
            shifter.result = ROR(value, rotation);
            shifter.flagC = (value >> (rotation - 1)) & 0x00000001;
        } else {
            shifter.result = value;
            shifter.flagC = SIGN32(value);
        }
    }

    static inline void shiftImmediate(uint32_t opcode) {
        unsigned int rotation = (opcode & 0x00000f00) >> 7;
        unsigned int immediate = opcode & 0x000000ff;

        if(rotation) {
            shifter.result = ROR(immediate, rotation);
            shifter.flagC = SIGN32(shifter.result);
        } else {
            shifter.result = immediate;
            shifter.flagC = cpsr.fields.flagC;
        }
    }
}
