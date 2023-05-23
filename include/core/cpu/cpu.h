#ifndef __INCLUDE_CORE_CPU_H__
#define __INCLUDE_CORE_CPU_H__

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    E_CPUMODE_OLD_USR = 0x00,
    E_CPUMODE_OLD_FIQ = 0x01,
    E_CPUMODE_OLD_IRQ = 0x02,
    E_CPUMODE_OLD_SVC = 0x03,
    E_CPUMODE_USR = 0x10,
    E_CPUMODE_FIQ = 0x11,
    E_CPUMODE_IRQ = 0x12,
    E_CPUMODE_SVC = 0x13,
    E_CPUMODE_ABT = 0x17,
    E_CPUMODE_UND = 0x1b,
    E_CPUMODE_SYS = 0x1f
} te_cpuMode;

typedef enum {
    E_CPUCONDITION_EQ,
    E_CPUCONDITION_NE,
    E_CPUCONDITION_CS,
    E_CPUCONDITION_CC,
    E_CPUCONDITION_MI,
    E_CPUCONDITION_PL,
    E_CPUCONDITION_VS,
    E_CPUCONDITION_VC,
    E_CPUCONDITION_HI,
    E_CPUCONDITION_LS,
    E_CPUCONDITION_GE,
    E_CPUCONDITION_LT,
    E_CPUCONDITION_GT,
    E_CPUCONDITION_LE,
    E_CPUCONDITION_AL,
    E_CPUCONDITION_NV
} te_cpuCondition;

enum {
    E_CPUREGISTER_IP = 12,
    E_CPUREGISTER_SP = 13,
    E_CPUREGISTER_LR = 14,
    E_CPUREGISTER_PC = 15
};

typedef enum {
    E_SHIFTTYPE_LSL,
    E_SHIFTTYPE_LSR,
    E_SHIFTTYPE_ASR,
    E_SHIFTTYPE_ROR
} te_shiftType;

extern bool s_cpuFlagZ;
extern bool s_cpuFlagC;
extern bool s_cpuFlagN;
extern bool s_cpuFlagV;
extern bool s_cpuFlagI;
extern bool s_cpuFlagF;
extern bool s_cpuFlagT;
extern uint32_t s_cpuRegisterR[16];

void cpuDebug(void);
void cpuInit(void);
void cpuReset(bool p_skipBoot);
void cpuJump(uint32_t p_address);
void cpuRaiseSwi(void);
void cpuRaiseUnd(void);
uint32_t cpuGetCpsr(void);
uint32_t cpuGetSpsr(void);
void cpuSetCpsr(uint32_t p_value);
void cpuSetSpsr(uint32_t p_value);

static inline void cpuWriteRegister(uint32_t p_register, uint32_t p_value) {
    if(p_register == E_CPUREGISTER_PC) {
        cpuJump(p_value);
    } else {
        s_cpuRegisterR[p_register] = p_value;
    }
}

static inline void cpuRestoreCpsr(void) {
    cpuSetCpsr(cpuGetSpsr());
}

#endif
