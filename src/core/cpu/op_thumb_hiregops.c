#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

static inline void add(uint16_t p_rs, uint16_t p_opcode);
static inline void cmp(uint16_t p_rs, uint16_t p_opcode);
static inline void mov(uint16_t p_rs, uint16_t p_opcode);
static inline void bx(uint16_t p_rs);

void cpuOpcodeThumbHighRegisterOperations(uint16_t p_opcode) {
    const bool l_h2 = (p_opcode & (1 << 6)) != 0;
    const uint16_t l_rs = ((p_opcode >> 3) & 0x7) | (l_h2 ? 8 : 0);
    const uint16_t l_op = (p_opcode >> 8) & 0x3;

    switch(l_op) {
        case 0: add(l_rs, p_opcode); break;
        case 1: cmp(l_rs, p_opcode); break;
        case 2: mov(l_rs, p_opcode); break;
        case 3: bx(l_rs); break;
    }
}

static inline void add(uint16_t p_rs, uint16_t p_opcode) {
    const bool l_h1 = (p_opcode & (1 << 7)) != 0;
    const uint16_t l_rd = (p_opcode & 0x7) | (l_h1 ? 8 : 0);

    cpuWriteRegister(l_rd, g_cpuRegisterR[l_rd] + g_cpuRegisterR[p_rs]);
}

static inline void cmp(uint16_t p_rs, uint16_t p_opcode) {
    const bool l_h1 = (p_opcode & (1 << 7)) != 0;
    const uint16_t l_rd = (p_opcode & 0x7) | (l_h1 ? 8 : 0);

    const uint32_t l_operand = g_cpuRegisterR[l_rd];
    const uint32_t l_operand2 = g_cpuRegisterR[p_rs];

    const uint32_t l_result = l_operand - l_operand2;

    g_cpuFlagC = getCarrySub(l_operand, l_operand2);
    g_cpuFlagV = getOverflowSub(l_operand, l_operand2, l_result);
    setFlagsArithmetical(l_result);
}

static inline void mov(uint16_t p_rs, uint16_t p_opcode) {
    const bool l_h1 = (p_opcode & (1 << 7)) != 0;
    const uint16_t l_rd = (p_opcode & 0x7) | (l_h1 ? 8 : 0);

    cpuWriteRegister(l_rd, g_cpuRegisterR[p_rs]);
}

static inline void bx(uint16_t p_rs) {
    const uint32_t l_rsValue = g_cpuRegisterR[p_rs];

    if((l_rsValue & 0x00000001) != 0) {
        g_cpuFlagT = true;
        cpuJump(l_rsValue & 0xfffffffe);
    } else {
        g_cpuFlagT = false;
        cpuJump(l_rsValue & 0xfffffffc);
    }
}
