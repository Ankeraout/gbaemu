#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "core/bus.h"
#include "core/irq.h"
#include "core/cpu/cpu.h"
#include "core/cpu/decoder.h"
#include "core/cpu/op_arm_bx.h"
#include "core/cpu/op_arm_und.h"
#include "core/cpu/op_thumb_und.h"

#define C_ADDRESS_MASK_16 0xfffffffe
#define C_ADDRESS_MASK_32 0xfffffffc
#define C_PSR_MASK_FLAG_N 0x80000000
#define C_PSR_MASK_FLAG_Z 0x40000000
#define C_PSR_MASK_FLAG_C 0x20000000
#define C_PSR_MASK_FLAG_V 0x10000000
#define C_PSR_MASK_FLAG_I 0x00000080
#define C_PSR_MASK_FLAG_F 0x00000040
#define C_PSR_MASK_FLAG_T 0x00000020
#define C_PSR_MASK_MODE 0x0000001f
#define C_EXCEPTION_VECTOR_RESET 0x00000000
#define C_EXCEPTION_VECTOR_UND 0x00000004
#define C_EXCEPTION_VECTOR_SWI 0x00000008
#define C_EXCEPTION_VECTOR_IRQ 0x00000018

bool g_cpuFlagZ;
bool g_cpuFlagC;
bool g_cpuFlagN;
bool g_cpuFlagV;
bool g_cpuFlagI;
bool g_cpuFlagF;
bool g_cpuFlagT;
uint32_t g_cpuRegisterR[16];
static uint32_t s_cpuRegisterUsrR[7];
static uint32_t s_cpuRegisterIrqR[2];
static uint32_t s_cpuRegisterFiqR[7];
static uint32_t s_cpuRegisterSvcR[2];
static uint32_t s_cpuRegisterAbtR[2];
static uint32_t s_cpuRegisterUndR[2];
static uint32_t s_cpuRegisterSpsrIrq;
static uint32_t s_cpuRegisterSpsrFiq;
static uint32_t s_cpuRegisterSpsrSvc;
static uint32_t s_cpuRegisterSpsrAbt;
static uint32_t s_cpuRegisterSpsrUnd;
enum te_cpuPipelineState g_cpuPipelineState;
static enum te_cpuMode s_cpuMode;
union tu_cpuFetchedOpcode g_cpuFetchedOpcode;
union tu_cpuDecodedOpcode g_cpuDecodedOpcode;

static void cpuChangeMode(enum te_cpuMode p_newMode);
static void cpuRaiseIrq(void);
static void cpuFetch(uint32_t p_fetchAddress);
static void cpuExecute(void);
static inline uint32_t *getRegisterPointerUsr(uint32_t p_register);

void cpuInit(void) {
    cpuDecoderInit();
}

void cpuReset(bool p_skipBoot) {
    // Reset CPU pipeline state
    g_cpuPipelineState = E_CPUPIPELINESTATE_FLUSH;

    // Reset flags
    g_cpuFlagZ = false;
    g_cpuFlagC = false;
    g_cpuFlagN = false;
    g_cpuFlagV = false;
    g_cpuFlagT = false;

    // Reset registers
    memset(g_cpuRegisterR, 0, sizeof(g_cpuRegisterR));
    memset(s_cpuRegisterIrqR, 0, sizeof(s_cpuRegisterIrqR));
    memset(s_cpuRegisterSvcR, 0, sizeof(s_cpuRegisterSvcR));
    memset(s_cpuRegisterAbtR, 0, sizeof(s_cpuRegisterAbtR));
    memset(s_cpuRegisterUndR, 0, sizeof(s_cpuRegisterUndR));
    memset(s_cpuRegisterFiqR, 0, sizeof(s_cpuRegisterFiqR));
    memset(s_cpuRegisterUsrR, 0, sizeof(s_cpuRegisterUsrR));
    s_cpuRegisterSpsrAbt = 0;
    s_cpuRegisterSpsrFiq = 0;
    s_cpuRegisterSpsrIrq = 0;
    s_cpuRegisterSpsrSvc = 0;
    s_cpuRegisterSpsrUnd = 0;

    if(p_skipBoot) {
        g_cpuFlagI = false;
        g_cpuFlagF = false;
        g_cpuRegisterR[13] = 0x03007f00;
        g_cpuRegisterR[15] = 0x08000000;
        s_cpuRegisterIrqR[0] = 0x03007fa0;
        s_cpuRegisterSvcR[0] = 0x03007fe0;
        s_cpuMode = E_CPUMODE_SYS;
    } else {
        g_cpuFlagI = false;
        g_cpuFlagF = false;
        s_cpuMode = E_CPUMODE_SVC;
    }
}

void cpuDebug(void) {
    for(int l_i = 0; l_i < 12; l_i++) {
        printf("R%d=%08x ", l_i, g_cpuRegisterR[l_i]);
    }

    printf("IP=%08x SP=%08x LR=%08x PC=%08x CPSR=%08x SPSR=%08x", g_cpuRegisterR[12], g_cpuRegisterR[13], g_cpuRegisterR[14], g_cpuRegisterR[15], cpuGetCpsr(), cpuGetSpsr());

    if(g_cpuPipelineState == E_CPUPIPELINESTATE_EXECUTE) {
        if(g_cpuFlagT) {
            printf(" %04x\n", g_cpuDecodedOpcode.thumb.opcode);
        } else {
            printf(" %08x\n", g_cpuDecodedOpcode.arm.opcode);
        }
    } else {
        printf("\n");
    }
}

void coreStep(void) {
    const uint32_t l_fetchAddress = g_cpuRegisterR[E_CPUREGISTER_PC];

    cpuExecute();
    cpuDecode();
    cpuFetch(l_fetchAddress);

    if(g_cpuPipelineState != E_CPUPIPELINESTATE_EXECUTE) {
        g_cpuPipelineState++;
    }
}

void cpuSetCpsr(uint32_t p_value) {
    g_cpuFlagN = (p_value & C_PSR_MASK_FLAG_N) != 0;
    g_cpuFlagZ = (p_value & C_PSR_MASK_FLAG_Z) != 0;
    g_cpuFlagC = (p_value & C_PSR_MASK_FLAG_C) != 0;
    g_cpuFlagV = (p_value & C_PSR_MASK_FLAG_V) != 0;
    g_cpuFlagI = (p_value & C_PSR_MASK_FLAG_I) != 0;
    g_cpuFlagF = (p_value & C_PSR_MASK_FLAG_F) != 0;
    g_cpuFlagT = (p_value & C_PSR_MASK_FLAG_T) != 0;

    cpuChangeMode(p_value & C_PSR_MASK_MODE);
}

void cpuSetSpsr(uint32_t p_value) {
    switch(s_cpuMode) {
        case E_CPUMODE_OLD_USR:
        case E_CPUMODE_USR:
        case E_CPUMODE_SYS:
            cpuSetCpsr(p_value);
            break;

        case E_CPUMODE_OLD_FIQ:
        case E_CPUMODE_FIQ:
            s_cpuRegisterSpsrFiq = p_value;
            break;

        case E_CPUMODE_OLD_IRQ:
        case E_CPUMODE_IRQ:
            s_cpuRegisterSpsrIrq = p_value;
            break;

        case E_CPUMODE_OLD_SVC:
        case E_CPUMODE_SVC:
            s_cpuRegisterSpsrSvc = p_value;
            break;

        case E_CPUMODE_ABT:
            s_cpuRegisterSpsrAbt = p_value;
            break;

        case E_CPUMODE_UND:
            s_cpuRegisterSpsrUnd = p_value;
            break;
    }
}

uint32_t cpuGetCpsr(void) {
    uint32_t l_cpsr = 0;

    if(g_cpuFlagN) l_cpsr |= C_PSR_MASK_FLAG_N;
    if(g_cpuFlagZ) l_cpsr |= C_PSR_MASK_FLAG_Z;
    if(g_cpuFlagC) l_cpsr |= C_PSR_MASK_FLAG_C;
    if(g_cpuFlagV) l_cpsr |= C_PSR_MASK_FLAG_V;
    if(g_cpuFlagI) l_cpsr |= C_PSR_MASK_FLAG_I;
    if(g_cpuFlagF) l_cpsr |= C_PSR_MASK_FLAG_F;
    if(g_cpuFlagT) l_cpsr |= C_PSR_MASK_FLAG_T;

    l_cpsr |= s_cpuMode;

    return l_cpsr;
}

uint32_t cpuGetSpsr(void) {
    uint32_t l_spsr = 0;

    switch(s_cpuMode) {
        case E_CPUMODE_OLD_USR:
        case E_CPUMODE_USR:
        case E_CPUMODE_SYS:
            l_spsr = cpuGetCpsr();
            break;

        case E_CPUMODE_OLD_FIQ:
        case E_CPUMODE_FIQ:
            l_spsr = s_cpuRegisterSpsrFiq;
            break;

        case E_CPUMODE_OLD_IRQ:
        case E_CPUMODE_IRQ:
            l_spsr = s_cpuRegisterSpsrIrq;
            break;

        case E_CPUMODE_OLD_SVC:
        case E_CPUMODE_SVC:
            l_spsr = s_cpuRegisterSpsrSvc;
            break;

        case E_CPUMODE_ABT:
            l_spsr = s_cpuRegisterSpsrAbt;
            break;

        case E_CPUMODE_UND:
            l_spsr = s_cpuRegisterSpsrUnd;
            break;
    }

    return l_spsr;
}

void cpuJump(uint32_t p_address) {
    uint32_t l_address;

    if(g_cpuFlagT) {
        l_address = p_address & C_ADDRESS_MASK_16;
    } else {
        l_address = p_address & C_ADDRESS_MASK_32;
    }

    g_cpuRegisterR[15] = l_address;
    g_cpuPipelineState = E_CPUPIPELINESTATE_FLUSH;
}

void cpuRaiseSwi(void) {
    cpuChangeMode(E_CPUMODE_SVC);
    s_cpuRegisterSpsrSvc = cpuGetCpsr();
    g_cpuRegisterR[14] = g_cpuRegisterR[15];
    g_cpuFlagT = false;
    g_cpuFlagI = true;
    cpuJump(C_EXCEPTION_VECTOR_SWI);
}

void cpuRaiseUnd(void) {
    cpuChangeMode(E_CPUMODE_UND);
    s_cpuRegisterSpsrUnd = cpuGetCpsr();
    g_cpuRegisterR[14] = g_cpuRegisterR[15];
    g_cpuFlagT = false;
    g_cpuFlagI = true;
    cpuJump(C_EXCEPTION_VECTOR_UND);
}

bool cpuCheckCondition(enum te_cpuCondition p_condition) {
    switch(p_condition) {
        case E_CPUCONDITION_EQ: return g_cpuFlagZ;
        case E_CPUCONDITION_NE: return !g_cpuFlagZ;
        case E_CPUCONDITION_CS: return g_cpuFlagC;
        case E_CPUCONDITION_CC: return !g_cpuFlagC;
        case E_CPUCONDITION_MI: return g_cpuFlagN;
        case E_CPUCONDITION_PL: return !g_cpuFlagN;
        case E_CPUCONDITION_VS: return g_cpuFlagV;
        case E_CPUCONDITION_VC: return !g_cpuFlagV;
        case E_CPUCONDITION_HI: return g_cpuFlagC && !g_cpuFlagZ;
        case E_CPUCONDITION_LS: return (!g_cpuFlagC) || g_cpuFlagZ;
        case E_CPUCONDITION_GE: return g_cpuFlagN == g_cpuFlagV;
        case E_CPUCONDITION_LT: return g_cpuFlagN != g_cpuFlagV;
        case E_CPUCONDITION_GT:
            return (!g_cpuFlagZ) && (g_cpuFlagN == g_cpuFlagV);
        case E_CPUCONDITION_LE: return g_cpuFlagZ || (g_cpuFlagN != g_cpuFlagV);
        case E_CPUCONDITION_AL: return true;
        default: return false;
    }
}

void cpuWriteRegisterUsr(uint32_t p_register, uint32_t p_value) {
    if(p_register == 15) {
        cpuJump(p_value);
    } else {
        *(getRegisterPointerUsr(p_register)) = p_value;
    }
}

uint32_t cpuReadRegisterUsr(uint32_t p_register) {
    return *(getRegisterPointerUsr(p_register));
}

static void cpuChangeMode(enum te_cpuMode p_newMode) {
    // Save banked registers
    switch(s_cpuMode) {
        case E_CPUMODE_OLD_USR:
        case E_CPUMODE_USR:
        case E_CPUMODE_SYS:
            s_cpuRegisterUsrR[0] = g_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = g_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = g_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = g_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = g_cpuRegisterR[12];
            s_cpuRegisterUsrR[5] = g_cpuRegisterR[13];
            s_cpuRegisterUsrR[6] = g_cpuRegisterR[14];
            break;

        case E_CPUMODE_OLD_FIQ:
        case E_CPUMODE_FIQ:
            s_cpuRegisterFiqR[0] = g_cpuRegisterR[8];
            s_cpuRegisterFiqR[1] = g_cpuRegisterR[9];
            s_cpuRegisterFiqR[2] = g_cpuRegisterR[10];
            s_cpuRegisterFiqR[3] = g_cpuRegisterR[11];
            s_cpuRegisterFiqR[4] = g_cpuRegisterR[12];
            s_cpuRegisterFiqR[5] = g_cpuRegisterR[13];
            s_cpuRegisterFiqR[6] = g_cpuRegisterR[14];
            break;

        case E_CPUMODE_OLD_IRQ:
        case E_CPUMODE_IRQ:
            s_cpuRegisterUsrR[0] = g_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = g_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = g_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = g_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = g_cpuRegisterR[12];
            s_cpuRegisterIrqR[0] = g_cpuRegisterR[13];
            s_cpuRegisterIrqR[0] = g_cpuRegisterR[14];
            break;

        case E_CPUMODE_OLD_SVC:
        case E_CPUMODE_SVC:
            s_cpuRegisterUsrR[0] = g_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = g_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = g_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = g_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = g_cpuRegisterR[12];
            s_cpuRegisterSvcR[0] = g_cpuRegisterR[13];
            s_cpuRegisterSvcR[1] = g_cpuRegisterR[14];
            break;

        case E_CPUMODE_ABT:
            s_cpuRegisterUsrR[0] = g_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = g_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = g_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = g_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = g_cpuRegisterR[12];
            s_cpuRegisterAbtR[0] = g_cpuRegisterR[13];
            s_cpuRegisterAbtR[1] = g_cpuRegisterR[14];
            break;

        case E_CPUMODE_UND:
            s_cpuRegisterUsrR[0] = g_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = g_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = g_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = g_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = g_cpuRegisterR[12];
            s_cpuRegisterUndR[0] = g_cpuRegisterR[13];
            s_cpuRegisterUndR[1] = g_cpuRegisterR[14];
            break;
    }

    // Restore banked registers
    switch(p_newMode) {
        case E_CPUMODE_OLD_USR:
        case E_CPUMODE_USR:
        case E_CPUMODE_SYS:
            g_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            g_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            g_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            g_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            g_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            g_cpuRegisterR[13] = s_cpuRegisterUsrR[5];
            g_cpuRegisterR[14] = s_cpuRegisterUsrR[6];
            break;

        case E_CPUMODE_OLD_FIQ:
        case E_CPUMODE_FIQ:
            g_cpuRegisterR[8] = s_cpuRegisterFiqR[0];
            g_cpuRegisterR[9] = s_cpuRegisterFiqR[1];
            g_cpuRegisterR[10] = s_cpuRegisterFiqR[2];
            g_cpuRegisterR[11] = s_cpuRegisterFiqR[3];
            g_cpuRegisterR[12] = s_cpuRegisterFiqR[4];
            g_cpuRegisterR[13] = s_cpuRegisterFiqR[5];
            g_cpuRegisterR[14] = s_cpuRegisterFiqR[6];
            break;

        case E_CPUMODE_OLD_IRQ:
        case E_CPUMODE_IRQ:
            g_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            g_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            g_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            g_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            g_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            g_cpuRegisterR[13] = s_cpuRegisterIrqR[0];
            g_cpuRegisterR[14] = s_cpuRegisterIrqR[1];
            break;

        case E_CPUMODE_OLD_SVC:
        case E_CPUMODE_SVC:
            g_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            g_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            g_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            g_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            g_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            g_cpuRegisterR[13] = s_cpuRegisterSvcR[0];
            g_cpuRegisterR[14] = s_cpuRegisterSvcR[1];
            break;

        case E_CPUMODE_ABT:
            g_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            g_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            g_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            g_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            g_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            g_cpuRegisterR[13] = s_cpuRegisterAbtR[0];
            g_cpuRegisterR[14] = s_cpuRegisterAbtR[1];
            break;

        case E_CPUMODE_UND:
            g_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            g_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            g_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            g_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            g_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            g_cpuRegisterR[13] = s_cpuRegisterUndR[0];
            g_cpuRegisterR[14] = s_cpuRegisterUndR[1];
            break;
    }

    // Set CPU mode
    s_cpuMode = p_newMode;
}

static void cpuRaiseIrq(void) {
    cpuChangeMode(E_CPUMODE_IRQ);
    s_cpuRegisterSpsrIrq = cpuGetCpsr();
    g_cpuRegisterR[14] = g_cpuRegisterR[15];
    g_cpuFlagT = false;
    g_cpuFlagI = true;
    cpuJump(C_EXCEPTION_VECTOR_IRQ);
}

static void cpuFetch(uint32_t p_fetchAddress) {
    if(g_cpuPipelineState >= E_CPUPIPELINESTATE_FETCH) {
        if(g_cpuFlagT) {
            g_cpuFetchedOpcode.thumb = busRead16(p_fetchAddress);
            g_cpuRegisterR[15] += C_INSTRUCTION_SIZE_THUMB;
        } else {
            g_cpuFetchedOpcode.arm = busRead32(p_fetchAddress);
            g_cpuRegisterR[15] += C_INSTRUCTION_SIZE_ARM;
        }
    }
}

static void cpuExecute(void) {
    if(g_cpuPipelineState >= E_CPUPIPELINESTATE_EXECUTE) {
        if(irqGetSignal()) {
            cpuRaiseIrq();
        } else {
            if(g_cpuFlagT) {
                g_cpuDecodedOpcode.thumb.handler(g_cpuDecodedOpcode.thumb.opcode);
            } else if(cpuCheckCondition(g_cpuDecodedOpcode.arm.opcode >> 28)) {
                g_cpuDecodedOpcode.arm.handler(g_cpuDecodedOpcode.arm.opcode);
            }
        }
    }
}

static inline uint32_t *getRegisterPointerUsr(uint32_t p_register) {
    const bool l_isUserMode =
        s_cpuMode == E_CPUMODE_OLD_USR
        || s_cpuMode == E_CPUMODE_USR
        || s_cpuMode == E_CPUMODE_SYS;
    const bool l_isCommonRegister =
        (p_register <= 7)
        || (p_register == 15)
        || (
            s_cpuMode != E_CPUMODE_FIQ
            && s_cpuMode != E_CPUMODE_OLD_FIQ
            && (p_register <= 12)
        );

    if(l_isUserMode || l_isCommonRegister) {
        return &g_cpuRegisterR[p_register];
    } else {
        return &s_cpuRegisterUsrR[p_register - 8];
    }
}
