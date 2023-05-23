#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "core/bitops.h"
#include "core/bus.h"

#define C_ADDRESS_MASK_16 0xfffffffe
#define C_ADDRESS_MASK_32 0xfffffffc
#define C_INSTRUCTION_SIZE_THUMB 0x00000002
#define C_INSTRUCTION_SIZE_ARM 0x00000004
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
} t_cpuMode;

typedef enum {
    E_CPUPIPELINESTATE_FLUSH,
    E_CPUPIPELINESTATE_FETCH,
    E_CPUPIPELINESTATE_DECODE,
    E_CPUPIPELINESTATE_EXECUTE
} t_cpuPipelineState;

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
} t_cpuCondition;

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
} t_shiftType;

static bool s_cpuFlagZ;
static bool s_cpuFlagC;
static bool s_cpuFlagN;
static bool s_cpuFlagV;
static bool s_cpuFlagI;
static bool s_cpuFlagF;
static bool s_cpuFlagT;
static uint32_t s_cpuRegisterR[16];
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
static uint32_t s_cpuShifterResult;
static bool s_cpuShifterCarry;
static t_cpuPipelineState s_cpuPipelineState;
static t_cpuMode s_cpuMode;

static union {
    uint32_t arm;
    uint16_t thumb;
} s_cpuFetchedOpcode;

static union {
    struct {
        void (*handler)(uint32_t p_opcode);
        uint32_t opcode;
    } arm;

    struct {
        void (*handler)(uint16_t p_opcode);
        uint16_t opcode;
    } thumb;
} s_cpuDecodedOpcode;

static struct {
    void (*arm[4096])(uint32_t p_opcode);
    void (*thumb[1024])(uint16_t p_opcode);
} s_cpuDecodeTable;

static bool cpuCheckCondition(t_cpuCondition p_condition);
static void cpuChangeMode(t_cpuMode p_newMode);
static void cpuJump(uint32_t p_address);
static void cpuRaiseIrq(void);
static void cpuRaiseSwi(void);
static void cpuRaiseUnd(void);
static void cpuFetch(uint32_t p_fetchAddress);
static void cpuDecode(void);
static void cpuExecute(void);
static uint32_t cpuGetCpsr(void);
static uint32_t cpuGetSpsr(void);
static void cpuSetCpsr(uint32_t p_value);
static void cpuSetSpsr(uint32_t p_value);
static void cpuInitArmDecodeTable(void);
static void cpuInitThumbDecodeTable(void);
static inline void cpuRestoreCpsr(void);
static inline void cpuWriteRegister(uint32_t p_register, uint32_t p_value);
static void cpuOpcodeArmB(uint32_t p_opcode);
static void cpuOpcodeArmBl(uint32_t p_opcode);
static void cpuOpcodeArmBx(uint32_t p_opcode);
static void cpuOpcodeArmUnd(uint32_t p_opcode);
static void cpuOpcodeThumbUnd(uint16_t p_opcode);

void cpuInit(void) {
    memset(&s_cpuDecodeTable, 0, sizeof(s_cpuDecodeTable));
    cpuInitArmDecodeTable();
    cpuInitThumbDecodeTable();
}

void cpuReset(bool p_skipBoot) {
    // Reset CPU pipeline state
    s_cpuPipelineState = E_CPUPIPELINESTATE_FLUSH;

    // Reset flags
    s_cpuFlagZ = false;
    s_cpuFlagC = false;
    s_cpuFlagN = false;
    s_cpuFlagV = false;
    s_cpuFlagT = false;

    // Reset registers
    memset(s_cpuRegisterR, 0, sizeof(s_cpuRegisterR));
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
        s_cpuFlagI = false;
        s_cpuFlagF = false;
        s_cpuRegisterR[13] = 0x03007f00;
        s_cpuRegisterR[15] = 0x08000000;
        s_cpuRegisterIrqR[0] = 0x03007fa0;
        s_cpuRegisterSvcR[0] = 0x03007fe0;
        s_cpuMode = E_CPUMODE_SYS;
    } else {
        s_cpuFlagI = false;
        s_cpuFlagF = false;
        s_cpuMode = E_CPUMODE_SVC;
    }
}

void cpuDebug(void) {
    for(int l_i = 0; l_i < 4; l_i++) {
        for(int l_j = 0; l_j < 4; l_j++) {
            int l_k = (l_i << 2) | l_j;

            printf("R%02d=0x%08x ", l_k, s_cpuRegisterR[l_k]);
        }

        printf("\n");
    }

    printf("CPSR=0x%08x SPSR=0x%08x\n", cpuGetCpsr(), cpuGetSpsr());
}

static bool cpuCheckCondition(t_cpuCondition p_condition) {
    switch(p_condition) {
        case E_CPUCONDITION_EQ: return s_cpuFlagZ;
        case E_CPUCONDITION_NE: return !s_cpuFlagZ;
        case E_CPUCONDITION_CS: return s_cpuFlagC;
        case E_CPUCONDITION_CC: return !s_cpuFlagC;
        case E_CPUCONDITION_MI: return s_cpuFlagN;
        case E_CPUCONDITION_PL: return !s_cpuFlagN;
        case E_CPUCONDITION_VS: return s_cpuFlagV;
        case E_CPUCONDITION_VC: return !s_cpuFlagV;
        case E_CPUCONDITION_HI: return s_cpuFlagC && !s_cpuFlagZ;
        case E_CPUCONDITION_LS: return (!s_cpuFlagC) || s_cpuFlagZ;
        case E_CPUCONDITION_GE: return s_cpuFlagN == s_cpuFlagV;
        case E_CPUCONDITION_LT: return s_cpuFlagN != s_cpuFlagV;
        case E_CPUCONDITION_GT:
            return (!s_cpuFlagZ) && (s_cpuFlagN == s_cpuFlagV);
        case E_CPUCONDITION_LE: return s_cpuFlagZ || (s_cpuFlagN != s_cpuFlagV);
        case E_CPUCONDITION_AL: return true;
        default: return false;
    }
}

static void cpuChangeMode(t_cpuMode p_newMode) {
    // Save banked registers
    switch(s_cpuMode) {
        case E_CPUMODE_OLD_USR:
        case E_CPUMODE_USR:
        case E_CPUMODE_SYS:
            s_cpuRegisterUsrR[0] = s_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = s_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = s_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = s_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = s_cpuRegisterR[12];
            s_cpuRegisterUsrR[5] = s_cpuRegisterR[13];
            s_cpuRegisterUsrR[6] = s_cpuRegisterR[14];
            break;

        case E_CPUMODE_OLD_FIQ:
        case E_CPUMODE_FIQ:
            s_cpuRegisterFiqR[0] = s_cpuRegisterR[8];
            s_cpuRegisterFiqR[1] = s_cpuRegisterR[9];
            s_cpuRegisterFiqR[2] = s_cpuRegisterR[10];
            s_cpuRegisterFiqR[3] = s_cpuRegisterR[11];
            s_cpuRegisterFiqR[4] = s_cpuRegisterR[12];
            s_cpuRegisterFiqR[5] = s_cpuRegisterR[13];
            s_cpuRegisterFiqR[6] = s_cpuRegisterR[14];
            break;

        case E_CPUMODE_OLD_IRQ:
        case E_CPUMODE_IRQ:
            s_cpuRegisterUsrR[0] = s_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = s_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = s_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = s_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = s_cpuRegisterR[12];
            s_cpuRegisterIrqR[0] = s_cpuRegisterR[13];
            s_cpuRegisterIrqR[0] = s_cpuRegisterR[14];
            break;

        case E_CPUMODE_OLD_SVC:
        case E_CPUMODE_SVC:
            s_cpuRegisterUsrR[0] = s_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = s_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = s_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = s_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = s_cpuRegisterR[12];
            s_cpuRegisterSvcR[0] = s_cpuRegisterR[13];
            s_cpuRegisterSvcR[1] = s_cpuRegisterR[14];
            break;

        case E_CPUMODE_ABT:
            s_cpuRegisterUsrR[0] = s_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = s_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = s_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = s_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = s_cpuRegisterR[12];
            s_cpuRegisterAbtR[0] = s_cpuRegisterR[13];
            s_cpuRegisterAbtR[1] = s_cpuRegisterR[14];
            break;

        case E_CPUMODE_UND:
            s_cpuRegisterUsrR[0] = s_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = s_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = s_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = s_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = s_cpuRegisterR[12];
            s_cpuRegisterUndR[0] = s_cpuRegisterR[13];
            s_cpuRegisterUndR[1] = s_cpuRegisterR[14];
            break;
    }

    // Restore banked registers
    switch(p_newMode) {
        case E_CPUMODE_OLD_USR:
        case E_CPUMODE_USR:
        case E_CPUMODE_SYS:
            s_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            s_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            s_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            s_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            s_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            s_cpuRegisterR[13] = s_cpuRegisterUsrR[5];
            s_cpuRegisterR[14] = s_cpuRegisterUsrR[6];
            break;

        case E_CPUMODE_OLD_FIQ:
        case E_CPUMODE_FIQ:
            s_cpuRegisterR[8] = s_cpuRegisterFiqR[0];
            s_cpuRegisterR[9] = s_cpuRegisterFiqR[1];
            s_cpuRegisterR[10] = s_cpuRegisterFiqR[2];
            s_cpuRegisterR[11] = s_cpuRegisterFiqR[3];
            s_cpuRegisterR[12] = s_cpuRegisterFiqR[4];
            s_cpuRegisterR[13] = s_cpuRegisterFiqR[5];
            s_cpuRegisterR[14] = s_cpuRegisterFiqR[6];
            break;

        case E_CPUMODE_OLD_IRQ:
        case E_CPUMODE_IRQ:
            s_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            s_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            s_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            s_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            s_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            s_cpuRegisterR[13] = s_cpuRegisterIrqR[0];
            s_cpuRegisterR[14] = s_cpuRegisterIrqR[1];
            break;

        case E_CPUMODE_OLD_SVC:
        case E_CPUMODE_SVC:
            s_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            s_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            s_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            s_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            s_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            s_cpuRegisterR[13] = s_cpuRegisterSvcR[0];
            s_cpuRegisterR[14] = s_cpuRegisterSvcR[1];
            break;

        case E_CPUMODE_ABT:
            s_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            s_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            s_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            s_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            s_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            s_cpuRegisterR[13] = s_cpuRegisterAbtR[0];
            s_cpuRegisterR[14] = s_cpuRegisterAbtR[1];
            break;

        case E_CPUMODE_UND:
            s_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            s_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            s_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            s_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            s_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            s_cpuRegisterR[13] = s_cpuRegisterUndR[0];
            s_cpuRegisterR[14] = s_cpuRegisterUndR[1];
            break;
    }

    // Set CPU mode
    s_cpuMode = p_newMode;
}

static void cpuJump(uint32_t p_address) {
    uint32_t l_address;

    if(s_cpuFlagT) {
        l_address = p_address & C_ADDRESS_MASK_16;
    } else {
        l_address = p_address & C_ADDRESS_MASK_32;
    }

    s_cpuRegisterR[15] = l_address;
    s_cpuPipelineState = E_CPUPIPELINESTATE_FLUSH;
}

static void cpuRaiseIrq(void) {
    cpuChangeMode(E_CPUMODE_IRQ);
    s_cpuRegisterSpsrIrq = cpuGetCpsr();
    s_cpuRegisterR[14] = s_cpuRegisterR[15];
    s_cpuFlagT = false;
    s_cpuFlagI = true;
    cpuJump(C_EXCEPTION_VECTOR_IRQ);
}

static void cpuRaiseSwi(void) {
    cpuChangeMode(E_CPUMODE_SVC);
    s_cpuRegisterSpsrSvc = cpuGetCpsr();
    s_cpuRegisterR[14] = s_cpuRegisterR[15];
    s_cpuFlagT = false;
    s_cpuFlagI = true;
    cpuJump(C_EXCEPTION_VECTOR_SWI);
}

static void cpuRaiseUnd(void) {
    cpuChangeMode(E_CPUMODE_UND);
    s_cpuRegisterSpsrUnd = cpuGetCpsr();
    s_cpuRegisterR[14] = s_cpuRegisterR[15];
    s_cpuFlagT = false;
    s_cpuFlagI = true;
    cpuJump(C_EXCEPTION_VECTOR_UND);
}

static void cpuFetch(uint32_t p_fetchAddress) {
    if(s_cpuPipelineState >= E_CPUPIPELINESTATE_FETCH) {
        if(s_cpuFlagT) {
            s_cpuFetchedOpcode.thumb = busRead16(p_fetchAddress);
            s_cpuRegisterR[15] += C_INSTRUCTION_SIZE_THUMB;
        } else {
            s_cpuFetchedOpcode.arm = busRead32(p_fetchAddress);
            s_cpuRegisterR[15] += C_INSTRUCTION_SIZE_ARM;
        }
    }
}

static void cpuDecode(void) {
    if(s_cpuPipelineState >= E_CPUPIPELINESTATE_DECODE) {
        if(s_cpuFlagT) {
            // Take bits 6 to 15
            uint16_t l_decodeIndex = s_cpuFetchedOpcode.thumb >> 6;

            s_cpuDecodedOpcode.thumb.opcode = s_cpuFetchedOpcode.thumb;
            s_cpuDecodedOpcode.thumb.handler = s_cpuDecodeTable
                .thumb[l_decodeIndex];
        } else {
            uint16_t l_decodeIndex;

            // Take bits 20 to 27
            l_decodeIndex = (s_cpuFetchedOpcode.arm >> 16) & 0x00000ff0;

            // Take bits 4 to 7
            l_decodeIndex |= (s_cpuFetchedOpcode.arm >> 4) & 0x0000000f;

            s_cpuDecodedOpcode.arm.opcode = s_cpuFetchedOpcode.arm;
            s_cpuDecodedOpcode.arm.handler = s_cpuDecodeTable
                .arm[l_decodeIndex];
        }
    }
}

static void cpuExecute(void) {
    if(s_cpuPipelineState >= E_CPUPIPELINESTATE_EXECUTE) {
        // TODO: Check for hardware interrupts

        if(s_cpuFlagT) {
            s_cpuDecodedOpcode.thumb.handler(s_cpuDecodedOpcode.thumb.opcode);
        } else if(cpuCheckCondition(s_cpuDecodedOpcode.arm.opcode >> 28)) {
            s_cpuDecodedOpcode.arm.handler(s_cpuDecodedOpcode.arm.opcode);
        }
    }
}

static uint32_t cpuGetCpsr(void) {
    uint32_t l_cpsr = 0;

    if(s_cpuFlagN) l_cpsr |= C_PSR_MASK_FLAG_N;
    if(s_cpuFlagZ) l_cpsr |= C_PSR_MASK_FLAG_Z;
    if(s_cpuFlagC) l_cpsr |= C_PSR_MASK_FLAG_C;
    if(s_cpuFlagV) l_cpsr |= C_PSR_MASK_FLAG_V;
    if(s_cpuFlagI) l_cpsr |= C_PSR_MASK_FLAG_I;
    if(s_cpuFlagF) l_cpsr |= C_PSR_MASK_FLAG_F;
    if(s_cpuFlagT) l_cpsr |= C_PSR_MASK_FLAG_T;

    l_cpsr |= s_cpuMode;

    return l_cpsr;
}

static uint32_t cpuGetSpsr(void) {
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

static void cpuSetCpsr(uint32_t p_value) {
    s_cpuFlagN = (p_value & C_PSR_MASK_FLAG_N) != 0;
    s_cpuFlagZ = (p_value & C_PSR_MASK_FLAG_Z) != 0;
    s_cpuFlagC = (p_value & C_PSR_MASK_FLAG_C) != 0;
    s_cpuFlagV = (p_value & C_PSR_MASK_FLAG_V) != 0;
    s_cpuFlagI = (p_value & C_PSR_MASK_FLAG_I) != 0;
    s_cpuFlagF = (p_value & C_PSR_MASK_FLAG_F) != 0;
    s_cpuFlagT = (p_value & C_PSR_MASK_FLAG_T) != 0;

    cpuChangeMode(p_value & C_PSR_MASK_MODE);
}

static void cpuSetSpsr(uint32_t p_value) {
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

static void cpuInitArmDecodeTable(void) {
    for(int l_index = 0; l_index < 4096; l_index++) {
        if((l_index & 0xe00) == 0xa00) {
            if((l_index & 0x100) == 0x000) {
                s_cpuDecodeTable.arm[l_index] = cpuOpcodeArmB;
            } else {
                s_cpuDecodeTable.arm[l_index] = cpuOpcodeArmBl;
            }
        } else {
            s_cpuDecodeTable.arm[l_index] = cpuOpcodeArmUnd;
        }
    }

    s_cpuDecodeTable.arm[0x121] = cpuOpcodeArmBx;
}

static void cpuInitThumbDecodeTable(void) {
    for(int l_index = 0; l_index < 1024; l_index++) {
        s_cpuDecodeTable.thumb[l_index] = cpuOpcodeThumbUnd;
    }
}

static inline void cpuRestoreCpsr(void) {
    cpuSetCpsr(cpuGetSpsr());
}

static inline void cpuWriteRegister(uint32_t p_register, uint32_t p_value) {
    if(p_register == E_CPUREGISTER_PC) {
        cpuJump(p_value);
    } else {
        s_cpuRegisterR[p_register] = p_value;
    }
}

void coreStep(void) {
    const uint32_t l_fetchAddress = s_cpuRegisterR[E_CPUREGISTER_PC];

    cpuExecute();
    cpuDecode();
    cpuFetch(l_fetchAddress);

    if(s_cpuPipelineState != E_CPUPIPELINESTATE_EXECUTE) {
        s_cpuPipelineState++;
    }
}

static void cpuOpcodeArmB(uint32_t p_opcode) {
    uint32_t l_offset = p_opcode & 0x00ffffff;

    l_offset <<= 2;
    l_offset = signExtend26to32(l_offset);

    cpuJump(s_cpuRegisterR[E_CPUREGISTER_PC] + l_offset);
}

static void cpuOpcodeArmBl(uint32_t p_opcode) {
    uint32_t l_offset = p_opcode & 0x00ffffff;

    l_offset <<= 2;
    l_offset = signExtend26to32(l_offset);

    s_cpuRegisterR[E_CPUREGISTER_LR] =
        s_cpuRegisterR[E_CPUREGISTER_PC] - C_INSTRUCTION_SIZE_ARM;

    cpuJump(s_cpuRegisterR[E_CPUREGISTER_PC] + l_offset);
}

static void cpuOpcodeArmBx(uint32_t p_opcode) {
    if((p_opcode & 0x0ffffff0) != 0x012fff10) {
        cpuRaiseUnd();
        return;
    }

    const uint32_t l_rn = p_opcode & 0x0000000f;
    const uint32_t l_rnValue = s_cpuRegisterR[l_rn];

    if((l_rn & 0x00000001) != 0) {
        s_cpuFlagT = true;
        cpuJump(l_rnValue & 0xfffffffe);
    } else {
        s_cpuFlagT = false;
        cpuJump(l_rnValue & 0xfffffffc);
    }
}

static void cpuOpcodeArmUnd(uint32_t p_opcode) {
    cpuRaiseUnd();
}

static void cpuOpcodeThumbUnd(uint16_t p_opcode) {
    cpuRaiseUnd();
}
