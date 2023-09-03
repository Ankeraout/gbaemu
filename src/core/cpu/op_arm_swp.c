#include <stdbool.h>
#include <stdint.h>

#include "core/bus.h"
#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

void cpuOpcodeArmSwp(uint32_t p_opcode) {
    const bool l_isByte = (p_opcode & (1 << 22)) != 0;
    const uint32_t l_rn = (p_opcode >> 16) & 0xf;
    const uint32_t l_rd = (p_opcode >> 12) & 0xf;
    const uint32_t l_rm = p_opcode & 0xf;

    const uint32_t l_rnValue = g_cpuRegisterR[l_rn];
    const uint32_t l_rmValue = g_cpuRegisterR[l_rm];

    if(l_isByte) {
        cpuWriteRegister(l_rd, busRead8(l_rnValue));
        busWrite8(l_rnValue, l_rmValue);
    } else {
        const uint32_t l_rotation = (l_rnValue & 0x03) << 3;

        cpuWriteRegister(l_rd, rotateRight(busRead32(l_rnValue), l_rotation));
        busWrite32(l_rnValue, l_rmValue);
    }
}
