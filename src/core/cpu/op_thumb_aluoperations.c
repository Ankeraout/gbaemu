#include <stdint.h>

#include "core/cpu/bitops.h"
#include "core/cpu/cpu.h"

static inline void and(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void eor(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void lsl(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void lsr(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void asr(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void adc(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void sbc(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void ror(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void tst(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void neg(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void cmp(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void cmn(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void orr(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void mul(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void bic(const uint32_t p_rsValue, const uint16_t p_rd);
static inline void mvn(const uint32_t p_rsValue, const uint16_t p_rd);

void cpuOpcodeThumbAluOperations(uint16_t p_opcode) {
    const uint16_t l_op = (p_opcode >> 6) & 0xf;
    const uint16_t l_rs = (p_opcode >> 3) & 0x7;
    const uint16_t l_rd = p_opcode & 0x7;

    const uint32_t l_rsValue = g_cpuRegisterR[l_rs];

    switch(l_op) {
        case 0: and(l_rsValue, l_rd); break;
        case 1: eor(l_rsValue, l_rd); break;
        case 2: lsl(l_rsValue, l_rd); break;
        case 3: lsr(l_rsValue, l_rd); break;
        case 4: asr(l_rsValue, l_rd); break;
        case 5: adc(l_rsValue, l_rd); break;
        case 6: sbc(l_rsValue, l_rd); break;
        case 7: ror(l_rsValue, l_rd); break;
        case 8: tst(l_rsValue, l_rd); break;
        case 9: neg(l_rsValue, l_rd); break;
        case 10: cmp(l_rsValue, l_rd); break;
        case 11: cmn(l_rsValue, l_rd); break;
        case 12: orr(l_rsValue, l_rd); break;
        case 13: mul(l_rsValue, l_rd); break;
        case 14: bic(l_rsValue, l_rd); break;
        case 15: mvn(l_rsValue, l_rd); break;
    }
}

static inline void and(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    const uint32_t l_result = p_rsValue & l_rdValue;

    setFlagsArithmetical(l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void eor(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    const uint32_t l_result = p_rsValue ^ l_rdValue;

    setFlagsArithmetical(l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void lsl(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    uint32_t l_result;

    if(p_rsValue == 0) {
        g_cpuShifterCarry = g_cpuFlagC;
        l_result = l_rdValue;
    } else if(p_rsValue < 32) {
        g_cpuShifterCarry =
            ((l_rdValue >> (32 - p_rsValue)) & 0x00000001) != 0;
        l_result = (uint32_t)(l_rdValue << p_rsValue);
    } else if(p_rsValue == 32) {
        g_cpuShifterCarry = (l_rdValue & 0x00000001) != 0;
        l_result = 0;
    } else {
        g_cpuShifterCarry = false;
        l_result = 0;
    }

    setFlagsLogical(l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void lsr(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    uint32_t l_result;

    if(p_rsValue == 0) {
        g_cpuShifterCarry = g_cpuFlagC;
        l_result = l_rdValue;
    } else if(p_rsValue < 32) {
        g_cpuShifterCarry =
            ((l_rdValue >> (p_rsValue - 1)) & 0x00000001) != 0;
        l_result = (uint32_t)(l_rdValue >> p_rsValue);
    } else if(p_rsValue == 32) {
        g_cpuShifterCarry = (l_rdValue & 0x80000000) != 0;
        l_result = 0;
    } else {
        g_cpuShifterCarry = false;
        l_result = 0;
    }

    setFlagsLogical(l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void asr(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    uint32_t l_result;

    if(p_rsValue == 0) {
        g_cpuShifterCarry = g_cpuFlagC;
        l_result = l_rdValue;
    } else if(p_rsValue < 32) {
        g_cpuShifterCarry =
            ((l_rdValue >> (p_rsValue - 1)) & 0x00000001) != 0;
        l_result =
            (uint32_t)(((int32_t)l_rdValue) >> p_rsValue);
    } else {
        g_cpuShifterCarry = (l_rdValue & 0x80000000) != 0;
        l_result =
            (uint32_t)(((int32_t)l_rdValue) >> 31);
    }

    setFlagsLogical(l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void adc(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    const uint32_t l_carry = g_cpuFlagC ? 1 : 0;
    const uint64_t l_result =
        (uint64_t)p_rsValue
        + (uint64_t)l_rdValue
        + (uint64_t)l_carry;

    setFlagsArithmetical(l_result);
    g_cpuFlagC = l_result > UINT32_MAX;
    g_cpuFlagV = getOverflowAdd(p_rsValue, l_rdValue, l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void sbc(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    const uint32_t l_carry = g_cpuFlagC ? 1 : 0;
    const uint32_t l_result = l_rdValue - p_rsValue + l_carry - 1;

    setFlagsArithmetical(l_result);
    g_cpuFlagC = getCarrySbc(l_rdValue, p_rsValue, g_cpuFlagC);
    g_cpuFlagV = getOverflowSub(l_rdValue, p_rsValue, l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void ror(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    uint32_t l_result;

    const uint32_t l_rotation = p_rsValue & 0x0000001f;

    if(p_rsValue == 0) {
        g_cpuShifterCarry = g_cpuFlagC;
        l_result = l_rdValue;
    } else if(l_rotation == 0) {
        g_cpuShifterCarry = (l_rdValue & 0x80000000) != 0;
        l_result = l_rdValue;
    } else {
        g_cpuShifterCarry =
            ((l_rdValue >> (l_rotation - 1)) & 0x00000001) != 0;
        l_result = rotateRight(l_rdValue, l_rotation);
    }

    setFlagsLogical(l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void tst(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    const uint32_t l_result = p_rsValue & l_rdValue;

    setFlagsArithmetical(l_result);
}

static inline void neg(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_result = -p_rsValue;

    setFlagsArithmetical(l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void cmp(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    const uint32_t l_result = l_rdValue - p_rsValue;

    setFlagsArithmetical(l_result);
    g_cpuFlagC = getCarrySub(l_rdValue, p_rsValue);
    g_cpuFlagV = getOverflowSub(l_rdValue, p_rsValue, l_result);
}

static inline void cmn(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    const uint32_t l_result = p_rsValue + l_rdValue;

    setFlagsArithmetical(l_result);
    g_cpuFlagC = l_result < p_rsValue;
    g_cpuFlagV = getOverflowAdd(p_rsValue, l_rdValue, l_result);
}

static inline void orr(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    const uint32_t l_result = p_rsValue | l_rdValue;

    setFlagsArithmetical(l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void mul(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    const uint32_t l_result = p_rsValue * l_rdValue;

    setFlagsArithmetical(l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void bic(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_rdValue = g_cpuRegisterR[p_rd];
    const uint32_t l_result = l_rdValue & ~p_rsValue;

    setFlagsArithmetical(l_result);
    g_cpuRegisterR[p_rd] = l_result;
}

static inline void mvn(const uint32_t p_rsValue, const uint16_t p_rd) {
    const uint32_t l_result = ~p_rsValue;

    setFlagsArithmetical(l_result);
    g_cpuRegisterR[p_rd] = l_result;
}
