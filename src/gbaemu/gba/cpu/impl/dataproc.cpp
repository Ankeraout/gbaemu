#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/dataproc.hpp>

#define HEADER_SHIFT_RM_0 \
    uint32_t Rm = opcode & 0x0000000f; \
    uint32_t immediate = (opcode & 0x00000f80) >> 7;

#define HEADER_SHIFT_RM_1 \
    uint32_t Rm = opcode & 0x0000000f; \
    uint32_t Rs = (opcode & 0x00000f00) >> 8; \
    uint32_t Rm_v = registerRead(Rm); \
    uint32_t Rs_v = registerRead(Rs);

#define ROR(value, rotation) ((((uint32_t)(value)) >> (rotation)) | (((uint32_t)(value)) << ((-(rotation)) & 0x1f)))

namespace gbaemu::gba::cpu::impl::dataproc {
    static inline void shiftLSL_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;

        if(immediate) {
            uint32_t Rm_v = registerRead(Rm);

            shifter.result = Rm_v << immediate;
            shifter.flagC = (Rm_v >> (32 - immediate)) & 0x00000001;
        } else {
            shifter.result = registerRead(Rm);
            shifter.flagC = cpsr.fields.flagC;
        }
    }

    static inline void shiftLSL_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;
    }

    static inline void shiftLSR_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;
    }

    static inline void shiftLSR_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;
    }

    static inline void shiftASR_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;
    }

    static inline void shiftASR_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;
    }

    static inline void shiftROR_Rm_0(uint32_t opcode) {
        HEADER_SHIFT_RM_0;
    }

    static inline void shiftROR_Rm_1(uint32_t opcode) {
        HEADER_SHIFT_RM_1;
    }

    static inline void shiftImmediate(uint32_t opcode) {
        unsigned int rotation = (opcode & 0x00000f00) >> 8;
        unsigned int immediate = opcode & 0x000000ff;

        if(rotation) {
            shifter.result = ROR(immediate, rotation);
            shifter.flagC = shifter.result >> 31;
        } else {
            shifter.result = immediate;
            shifter.flagC = cpsr.fields.flagC;
        }
    }
}
