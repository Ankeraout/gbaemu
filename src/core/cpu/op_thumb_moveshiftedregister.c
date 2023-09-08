#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeThumbMoveShiftedRegister(uint16_t p_opcode) {
    const enum te_shiftType l_operation = (p_opcode >> 11) & 0x3;
    const uint16_t l_immediate = (p_opcode >> 6) & 0x1f;
    const uint16_t l_rs = (p_opcode >> 3) & 0x7;
    const uint16_t l_rd = p_opcode & 0x7;

    const uint32_t l_rsValue = g_cpuRegisterR[l_rs];

    uint32_t l_result;

    switch(l_operation) {
        case E_SHIFTTYPE_LSL:
            if(l_immediate == 0) {
                l_result = l_rsValue;
            } else {
                g_cpuFlagC =
                    ((l_rsValue >> (32 - l_immediate)) & 0x00000001)
                    != 0;
                l_result = l_rsValue << l_immediate;
            }

            break;

        case E_SHIFTTYPE_LSR:
            if(l_immediate == 0) {
                g_cpuFlagC = (l_rsValue & 0x80000000) != 0;
                l_result = 0;
            } else {
                g_cpuFlagC =
                    ((l_rsValue >> (l_immediate - 1)) & 0x00000001)
                    != 0;
                l_result = l_rsValue >> l_immediate;
            }

            break;

        case E_SHIFTTYPE_ASR:
            if(l_immediate == 0) {
                g_cpuFlagC = (l_rsValue & 0x80000000) != 0;
                l_result =
                    (uint32_t)(((int32_t)l_rsValue) >> 31);
            } else {
                g_cpuFlagC =
                    ((l_rsValue >> (l_immediate - 1)) & 0x00000001)
                    != 0;
                l_result =
                    (uint32_t)(((int32_t)l_rsValue) >> l_immediate);
            }

            break;

        default: // ROR is not implemented (ADD/SUB format)
            /*
                This code should never be executed.
                This assignment to l_result only fixes a use-before-assign
                warning, as the compiler has no way to guess that l_operation
                cannot be E_SHIFTTYPE_ROR.
            */
            l_result = 0;
            break;
    }

    g_cpuRegisterR[l_rd] = l_result;

    setFlagsArithmetical(l_result);
}
