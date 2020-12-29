#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "core/bus.h"
#include "core/io.h"

typedef enum {
    GBA_CPU_MODE_USR_OLD = 0x00,
    GBA_CPU_MODE_FIQ_OLD = 0x01,
    GBA_CPU_MODE_IRQ_OLD = 0x02,
    GBA_CPU_MODE_SVC_OLD = 0x03,
    GBA_CPU_MODE_USR = 0x10,
    GBA_CPU_MODE_FIQ = 0x11,
    GBA_CPU_MODE_IRQ = 0x12,
    GBA_CPU_MODE_SVC = 0x13,
    GBA_CPU_MODE_ABT = 0x17,
    GBA_CPU_MODE_UND = 0x1b,
    GBA_CPU_MODE_SYS = 0x1f
} gba_cpu_mode_t;

typedef enum {
    GBA_CPU_PIPELINESTATE_FLUSH,
    GBA_CPU_PIPELINESTATE_FETCH,
    GBA_CPU_PIPELINESTATE_DECODE,
    GBA_CPU_PIPELINESTATE_EXECUTE,
} gba_cpu_pipelineState_t;

typedef enum {
    GBA_CPU_CONDITION_EQ,
    GBA_CPU_CONDITION_NE,
    GBA_CPU_CONDITION_CS,
    GBA_CPU_CONDITION_CC,
    GBA_CPU_CONDITION_MI,
    GBA_CPU_CONDITION_PL,
    GBA_CPU_CONDITION_VS,
    GBA_CPU_CONDITION_VC,
    GBA_CPU_CONDITION_HI,
    GBA_CPU_CONDITION_LS,
    GBA_CPU_CONDITION_GE,
    GBA_CPU_CONDITION_LT,
    GBA_CPU_CONDITION_GT,
    GBA_CPU_CONDITION_LE,
    GBA_CPU_CONDITION_AL,
    GBA_CPU_CONDITION_NV
} gba_cpu_condition_t;

typedef void gba_cpu_opcodeHandlerArm_t(uint32_t opcode);
typedef void gba_cpu_opcodeHandlerThumb_t(uint16_t opcode);

gba_cpu_pipelineState_t gba_cpu_pipelineState;
uint32_t gba_cpu_r[16];
uint32_t gba_cpu_r_usr[7];
uint32_t gba_cpu_r_irq[2];
uint32_t gba_cpu_r_fiq[7];
uint32_t gba_cpu_r_svc[2];
uint32_t gba_cpu_r_abt[2];
uint32_t gba_cpu_r_und[2];
uint32_t gba_cpu_spsr_irq;
uint32_t gba_cpu_spsr_fiq;
uint32_t gba_cpu_spsr_svc;
uint32_t gba_cpu_spsr_abt;
uint32_t gba_cpu_spsr_und;
gba_cpu_mode_t gba_cpu_mode;
bool gba_cpu_flagN;
bool gba_cpu_flagZ;
bool gba_cpu_flagC;
bool gba_cpu_flagV;
bool gba_cpu_flagI;
bool gba_cpu_flagF;
bool gba_cpu_flagT;
uint32_t gba_cpu_fetchedOpcodeArm;
uint16_t gba_cpu_fetchedOpcodeThumb;
uint32_t gba_cpu_decodedOpcodeArmValue;
uint16_t gba_cpu_decodedOpcodeThumbValue;
gba_cpu_opcodeHandlerArm_t *gba_cpu_decodedOpcodeArmHandler;
gba_cpu_opcodeHandlerThumb_t *gba_cpu_decodedOpcodeThumbHandler;
gba_cpu_opcodeHandlerArm_t *gba_cpu_decodeTable_arm[4096];
gba_cpu_opcodeHandlerThumb_t *gba_cpu_decodeTable_thumb[1024];
uint32_t gba_cpu_shifterResult;
bool gba_cpu_shifterCarry;

void gba_cpu_init();
void gba_cpu_reset(bool skipBoot);
void gba_cpu_cycle();
static inline uint32_t gba_cpu_getCpsr();
static inline void gba_cpu_setCpsr(uint32_t value);
static inline uint32_t gba_cpu_getSpsr();
static inline void gba_cpu_setSpsr(uint32_t value);
static inline void gba_cpu_changeMode(gba_cpu_mode_t newMode);
static inline bool gba_cpu_checkCondition(gba_cpu_condition_t condition);
static inline void gba_cpu_performJump(uint32_t address);
static inline void gba_cpu_raiseIrq();
static inline void gba_cpu_raiseSwi();
static inline void gba_cpu_raiseUnd();
static inline void gba_cpu_execute();
static inline void gba_cpu_decode();
static inline void gba_cpu_fetch();
static inline void gba_cpu_initDecodeThumb();
static inline void gba_cpu_initDecodeArm();
static inline void gba_cpu_arm_shift(uint32_t opcode);
static inline uint32_t gba_cpu_util_ror32(uint32_t value, int bits);
static inline void gba_cpu_writeRegister(int r, uint32_t value);
static inline void gba_cpu_setFlags_logical(uint32_t result);
static inline void gba_cpu_setFlags_arithmetical(uint32_t result);
static inline bool gba_cpu_getCarry_sbc(uint32_t left, uint32_t right, bool carry);
static inline bool gba_cpu_getCarry_sub(uint32_t left, uint32_t right);
static inline bool gba_cpu_getOverflow_sub(uint32_t left, uint32_t right, uint32_t result);
static inline bool gba_cpu_getOverflow_add(uint32_t left, uint32_t right, uint32_t result);
static inline unsigned int gba_cpu_util_hammingWeight16(uint16_t value);
static inline unsigned int gba_cpu_util_hammingWeight8(uint8_t value);
static inline void gba_cpu_arm_halfwordSignedDataTransfer(uint32_t opcode);
static inline void gba_cpu_arm_blockDataTransfer(uint32_t opcode);
static inline void gba_cpu_arm_swp(uint32_t opcode);
static inline void gba_cpu_arm_mul(uint32_t opcode);
static inline void gba_cpu_arm_mull(uint32_t opcode);
static inline void gba_cpu_arm_bx(uint32_t opcode);
static inline void gba_cpu_arm_swi(uint32_t opcode);
static inline void gba_cpu_arm_singleDataTransfer(uint32_t opcode);
static inline void gba_cpu_arm_psrTransfer_msr(uint32_t opcode, bool spsr, uint32_t operand);
static inline void gba_cpu_arm_psrTransfer(uint32_t opcode);
static inline void gba_cpu_arm_and(uint32_t opcode);
static inline void gba_cpu_arm_eor(uint32_t opcode);
static inline void gba_cpu_arm_sub(uint32_t opcode);
static inline void gba_cpu_arm_rsb(uint32_t opcode);
static inline void gba_cpu_arm_add(uint32_t opcode);
static inline void gba_cpu_arm_adc(uint32_t opcode);
static inline void gba_cpu_arm_sbc(uint32_t opcode);
static inline void gba_cpu_arm_rsc(uint32_t opcode);
static inline void gba_cpu_arm_tst(uint32_t opcode);
static inline void gba_cpu_arm_teq(uint32_t opcode);
static inline void gba_cpu_arm_cmp(uint32_t opcode);
static inline void gba_cpu_arm_cmn(uint32_t opcode);
static inline void gba_cpu_arm_orr(uint32_t opcode);
static inline void gba_cpu_arm_mov(uint32_t opcode);
static inline void gba_cpu_arm_bic(uint32_t opcode);
static inline void gba_cpu_arm_mvn(uint32_t opcode);
static inline void gba_cpu_arm_b(uint32_t opcode);
static inline void gba_cpu_thumb_sub(uint16_t opcode);
static inline void gba_cpu_thumb_add(uint16_t opcode);
static inline void gba_cpu_thumb_lsl(uint16_t opcode);
static inline void gba_cpu_thumb_lsr(uint16_t opcode);
static inline void gba_cpu_thumb_asr(uint16_t opcode);
static inline void gba_cpu_thumb_mov(uint16_t opcode);
static inline void gba_cpu_thumb_cmp(uint16_t opcode);
static inline void gba_cpu_thumb_add2(uint16_t opcode);
static inline void gba_cpu_thumb_sub2(uint16_t opcode);
static inline void gba_cpu_thumb_ldrStrh(uint16_t opcode);
static inline void gba_cpu_thumb_ldrStr(uint16_t opcode);
static inline void gba_cpu_thumb_ldr(uint16_t opcode);
static inline void gba_cpu_thumb_bx(uint16_t opcode);
static inline void gba_cpu_thumb_add3(uint16_t opcode);
static inline void gba_cpu_thumb_cmp3(uint16_t opcode);
static inline void gba_cpu_thumb_mov2(uint16_t opcode);
static inline void gba_cpu_thumb_and(uint16_t opcode);
static inline void gba_cpu_thumb_eor(uint16_t opcode);
static inline void gba_cpu_thumb_lsl2(uint16_t opcode);
static inline void gba_cpu_thumb_lsr2(uint16_t opcode);
static inline void gba_cpu_thumb_asr2(uint16_t opcode);
static inline void gba_cpu_thumb_adc(uint16_t opcode);
static inline void gba_cpu_thumb_sbc(uint16_t opcode);
static inline void gba_cpu_thumb_ror(uint16_t opcode);
static inline void gba_cpu_thumb_tst(uint16_t opcode);
static inline void gba_cpu_thumb_neg(uint16_t opcode);
static inline void gba_cpu_thumb_cmp2(uint16_t opcode);
static inline void gba_cpu_thumb_cmn(uint16_t opcode);
static inline void gba_cpu_thumb_orr(uint16_t opcode);
static inline void gba_cpu_thumb_mul(uint16_t opcode);
static inline void gba_cpu_thumb_bic(uint16_t opcode);
static inline void gba_cpu_thumb_mvn(uint16_t opcode);
static inline void gba_cpu_thumb_ldrStr2(uint16_t opcode);
static inline void gba_cpu_thumb_ldrStr3(uint16_t opcode);
static inline void gba_cpu_thumb_ldrStrh2(uint16_t opcode);
static inline void gba_cpu_thumb_add5(uint16_t opcode);
static inline void gba_cpu_thumb_pushPop(uint16_t opcode);
static inline void gba_cpu_thumb_add4(uint16_t opcode);
static inline void gba_cpu_thumb_swi(uint16_t opcode);
static inline void gba_cpu_thumb_b(uint16_t opcode);
static inline void gba_cpu_thumb_ldmStm(uint16_t opcode);
static inline void gba_cpu_thumb_bl(uint16_t opcode);
static inline void gba_cpu_thumb_b2(uint16_t opcode);

void gba_cpu_init() {
    gba_cpu_initDecodeArm();
    gba_cpu_initDecodeThumb();
}

void gba_cpu_reset(bool skipBoot) {
    gba_cpu_pipelineState = GBA_CPU_PIPELINESTATE_FETCH;

    for(int i = 0; i < 2; i++) {
        gba_cpu_r_irq[i] = 0;
        gba_cpu_r_svc[i] = 0;
        gba_cpu_r_abt[i] = 0;
        gba_cpu_r_und[i] = 0;
    }

    for(int i = 0; i < 7; i++) {
        gba_cpu_r_fiq[i] = 0;
        gba_cpu_r_usr[i] = 0;
    }

    for(int i = 0; i < 16; i++) {
        gba_cpu_r[i] = 0;
    }

    gba_cpu_flagN = false;
    gba_cpu_flagZ = false;
    gba_cpu_flagC = false;
    gba_cpu_flagV = false;
    gba_cpu_flagI = false;
    gba_cpu_flagF = false;
    gba_cpu_flagT = false;

    if(skipBoot) {
        gba_cpu_r[13] = 0x03007f00;
        gba_cpu_r_irq[0] = 0x03007fa0;
        gba_cpu_r_svc[0] = 0x03007fe0;

        gba_cpu_r[15] = 0x08000000;
        gba_cpu_mode = GBA_CPU_MODE_SYS;
    } else {
        gba_cpu_mode = GBA_CPU_MODE_SVC;
        gba_cpu_flagI = true;
        gba_cpu_flagF = true;
    }

    gba_cpu_spsr_abt = 0;
    gba_cpu_spsr_fiq = 0;
    gba_cpu_spsr_irq = 0;
    gba_cpu_spsr_svc = 0;
    gba_cpu_spsr_und = 0;
}

void gba_cpu_cycle() {
    uint32_t fetchAddress = gba_cpu_r[15];

    gba_cpu_execute();
    gba_cpu_decode();
    gba_cpu_fetch(fetchAddress);

    switch(gba_cpu_pipelineState) {
        case GBA_CPU_PIPELINESTATE_FLUSH:
            gba_cpu_pipelineState = GBA_CPU_PIPELINESTATE_FETCH;
            break;

        case GBA_CPU_PIPELINESTATE_FETCH:
            gba_cpu_pipelineState = GBA_CPU_PIPELINESTATE_DECODE;
            break;

        case GBA_CPU_PIPELINESTATE_DECODE:
            gba_cpu_pipelineState = GBA_CPU_PIPELINESTATE_EXECUTE;
            break;

        case GBA_CPU_PIPELINESTATE_EXECUTE:
            gba_cpu_pipelineState = GBA_CPU_PIPELINESTATE_EXECUTE;
            break;
    }
}

static inline uint32_t gba_cpu_getCpsr() {
    return 0
        | (gba_cpu_flagN ? 1 << 31 : 0)
        | (gba_cpu_flagZ ? 1 << 30 : 0)
        | (gba_cpu_flagC ? 1 << 29 : 0)
        | (gba_cpu_flagV ? 1 << 28 : 0)
        | (gba_cpu_flagI ? 1 << 7 : 0)
        | (gba_cpu_flagF ? 1 << 6 : 0)
        | (gba_cpu_flagT ? 1 << 5 : 0)
        | gba_cpu_mode;
}

static inline void gba_cpu_setCpsr(uint32_t value) {
    gba_cpu_flagN = (value & (1 << 31)) != 0;
    gba_cpu_flagZ = (value & (1 << 30)) != 0;
    gba_cpu_flagC = (value & (1 << 29)) != 0;
    gba_cpu_flagV = (value & (1 << 28)) != 0;
    gba_cpu_flagI = (value & (1 << 7)) != 0;
    gba_cpu_flagF = (value & (1 << 6)) != 0;
    gba_cpu_flagT = (value & (1 << 5)) != 0;

    gba_cpu_changeMode(value & 0x1f);
}

static inline uint32_t gba_cpu_getSpsr() {
    switch(gba_cpu_mode) {
        case GBA_CPU_MODE_FIQ_OLD:
        case GBA_CPU_MODE_FIQ:
            return gba_cpu_spsr_fiq;

        case GBA_CPU_MODE_IRQ_OLD:
        case GBA_CPU_MODE_IRQ:
            return gba_cpu_spsr_irq;

        case GBA_CPU_MODE_SVC_OLD:
        case GBA_CPU_MODE_SVC:
            return gba_cpu_spsr_svc;

        case GBA_CPU_MODE_ABT:
            return gba_cpu_spsr_abt;

        case GBA_CPU_MODE_UND:
            return gba_cpu_spsr_und;
            
        default:
            return gba_cpu_getCpsr();
    }
}

static inline void gba_cpu_setSpsr(uint32_t value) {
    switch(gba_cpu_mode) {
        case GBA_CPU_MODE_FIQ_OLD:
        case GBA_CPU_MODE_FIQ:
            gba_cpu_spsr_fiq = value;
            break;

        case GBA_CPU_MODE_IRQ_OLD:
        case GBA_CPU_MODE_IRQ:
            gba_cpu_spsr_irq = value;
            break;

        case GBA_CPU_MODE_SVC_OLD:
        case GBA_CPU_MODE_SVC:
            gba_cpu_spsr_svc = value;
            break;

        case GBA_CPU_MODE_ABT:
            gba_cpu_spsr_abt = value;
            break;

        case GBA_CPU_MODE_UND:
            gba_cpu_spsr_und = value;
            break;
            
        default:
            gba_cpu_setCpsr(value);
            break;
    }
}

static inline void gba_cpu_changeMode(gba_cpu_mode_t newMode) {
    gba_cpu_mode_t oldMode = gba_cpu_mode;

    switch(oldMode) {
        case GBA_CPU_MODE_USR_OLD:
        case GBA_CPU_MODE_USR:
        case GBA_CPU_MODE_SYS:
            gba_cpu_r_usr[0] = gba_cpu_r[8];
            gba_cpu_r_usr[1] = gba_cpu_r[9];
            gba_cpu_r_usr[2] = gba_cpu_r[10];
            gba_cpu_r_usr[3] = gba_cpu_r[11];
            gba_cpu_r_usr[4] = gba_cpu_r[12];
            gba_cpu_r_usr[5] = gba_cpu_r[13];
            gba_cpu_r_usr[6] = gba_cpu_r[14];
            break;

        case GBA_CPU_MODE_FIQ_OLD:
        case GBA_CPU_MODE_FIQ:
            gba_cpu_r_fiq[0] = gba_cpu_r[8];
            gba_cpu_r_fiq[1] = gba_cpu_r[9];
            gba_cpu_r_fiq[2] = gba_cpu_r[10];
            gba_cpu_r_fiq[3] = gba_cpu_r[11];
            gba_cpu_r_fiq[4] = gba_cpu_r[12];
            gba_cpu_r_fiq[5] = gba_cpu_r[13];
            gba_cpu_r_fiq[6] = gba_cpu_r[14];
            break;

        case GBA_CPU_MODE_IRQ_OLD:
        case GBA_CPU_MODE_IRQ:
            gba_cpu_r_usr[0] = gba_cpu_r[8];
            gba_cpu_r_usr[1] = gba_cpu_r[9];
            gba_cpu_r_usr[2] = gba_cpu_r[10];
            gba_cpu_r_usr[3] = gba_cpu_r[11];
            gba_cpu_r_usr[4] = gba_cpu_r[12];
            gba_cpu_r_irq[0] = gba_cpu_r[13];
            gba_cpu_r_irq[1] = gba_cpu_r[14];
            break;

        case GBA_CPU_MODE_SVC_OLD:
        case GBA_CPU_MODE_SVC:
            gba_cpu_r_usr[0] = gba_cpu_r[8];
            gba_cpu_r_usr[1] = gba_cpu_r[9];
            gba_cpu_r_usr[2] = gba_cpu_r[10];
            gba_cpu_r_usr[3] = gba_cpu_r[11];
            gba_cpu_r_usr[4] = gba_cpu_r[12];
            gba_cpu_r_svc[0] = gba_cpu_r[13];
            gba_cpu_r_svc[1] = gba_cpu_r[14];
            break;

        case GBA_CPU_MODE_ABT:
            gba_cpu_r_usr[0] = gba_cpu_r[8];
            gba_cpu_r_usr[1] = gba_cpu_r[9];
            gba_cpu_r_usr[2] = gba_cpu_r[10];
            gba_cpu_r_usr[3] = gba_cpu_r[11];
            gba_cpu_r_usr[4] = gba_cpu_r[12];
            gba_cpu_r_abt[0] = gba_cpu_r[13];
            gba_cpu_r_abt[1] = gba_cpu_r[14];
            break;

        case GBA_CPU_MODE_UND:
            gba_cpu_r_usr[0] = gba_cpu_r[8];
            gba_cpu_r_usr[1] = gba_cpu_r[9];
            gba_cpu_r_usr[2] = gba_cpu_r[10];
            gba_cpu_r_usr[3] = gba_cpu_r[11];
            gba_cpu_r_usr[4] = gba_cpu_r[12];
            gba_cpu_r_und[0] = gba_cpu_r[13];
            gba_cpu_r_und[1] = gba_cpu_r[14];
            break;
    }

    switch(newMode) {
        case GBA_CPU_MODE_USR_OLD:
        case GBA_CPU_MODE_USR:
        case GBA_CPU_MODE_SYS:
            gba_cpu_r[8] = gba_cpu_r_usr[0];
            gba_cpu_r[9] = gba_cpu_r_usr[1];
            gba_cpu_r[10] = gba_cpu_r_usr[2];
            gba_cpu_r[11] = gba_cpu_r_usr[3];
            gba_cpu_r[12] = gba_cpu_r_usr[4];
            gba_cpu_r[13] = gba_cpu_r_usr[5];
            gba_cpu_r[14] = gba_cpu_r_usr[6];
            break;

        case GBA_CPU_MODE_FIQ_OLD:
        case GBA_CPU_MODE_FIQ:
            gba_cpu_r[8] = gba_cpu_r_fiq[0];
            gba_cpu_r[9] = gba_cpu_r_fiq[1];
            gba_cpu_r[10] = gba_cpu_r_fiq[2];
            gba_cpu_r[11] = gba_cpu_r_fiq[3];
            gba_cpu_r[12] = gba_cpu_r_fiq[4];
            gba_cpu_r[13] = gba_cpu_r_fiq[5];
            gba_cpu_r[14] = gba_cpu_r_fiq[6];
            break;

        case GBA_CPU_MODE_IRQ_OLD:
        case GBA_CPU_MODE_IRQ:
            gba_cpu_r[8] = gba_cpu_r_usr[0];
            gba_cpu_r[9] = gba_cpu_r_usr[1];
            gba_cpu_r[10] = gba_cpu_r_usr[2];
            gba_cpu_r[11] = gba_cpu_r_usr[3];
            gba_cpu_r[12] = gba_cpu_r_usr[4];
            gba_cpu_r[13] = gba_cpu_r_irq[0];
            gba_cpu_r[14] = gba_cpu_r_irq[1];
            break;

        case GBA_CPU_MODE_SVC_OLD:
        case GBA_CPU_MODE_SVC:
            gba_cpu_r[8] = gba_cpu_r_usr[0];
            gba_cpu_r[9] = gba_cpu_r_usr[1];
            gba_cpu_r[10] = gba_cpu_r_usr[2];
            gba_cpu_r[11] = gba_cpu_r_usr[3];
            gba_cpu_r[12] = gba_cpu_r_usr[4];
            gba_cpu_r[13] = gba_cpu_r_svc[0];
            gba_cpu_r[14] = gba_cpu_r_svc[1];
            break;

        case GBA_CPU_MODE_ABT:
            gba_cpu_r[8] = gba_cpu_r_usr[0];
            gba_cpu_r[9] = gba_cpu_r_usr[1];
            gba_cpu_r[10] = gba_cpu_r_usr[2];
            gba_cpu_r[11] = gba_cpu_r_usr[3];
            gba_cpu_r[12] = gba_cpu_r_usr[4];
            gba_cpu_r[13] = gba_cpu_r_abt[0];
            gba_cpu_r[14] = gba_cpu_r_abt[1];
            break;

        case GBA_CPU_MODE_UND:
            gba_cpu_r[8] = gba_cpu_r_usr[0];
            gba_cpu_r[9] = gba_cpu_r_usr[1];
            gba_cpu_r[10] = gba_cpu_r_usr[2];
            gba_cpu_r[11] = gba_cpu_r_usr[3];
            gba_cpu_r[12] = gba_cpu_r_usr[4];
            gba_cpu_r[13] = gba_cpu_r_und[0];
            gba_cpu_r[14] = gba_cpu_r_und[1];
            break;
            // TODO: undefined mode
    }

    gba_cpu_mode = newMode;
}

static inline bool gba_cpu_checkCondition(gba_cpu_condition_t condition) {
    switch(condition) {
        case GBA_CPU_CONDITION_EQ: return gba_cpu_flagZ;
        case GBA_CPU_CONDITION_NE: return !gba_cpu_flagZ;
        case GBA_CPU_CONDITION_CS: return gba_cpu_flagC;
        case GBA_CPU_CONDITION_CC: return !gba_cpu_flagC;
        case GBA_CPU_CONDITION_MI: return gba_cpu_flagN;
        case GBA_CPU_CONDITION_PL: return !gba_cpu_flagN;
        case GBA_CPU_CONDITION_VS: return gba_cpu_flagV;
        case GBA_CPU_CONDITION_VC: return !gba_cpu_flagV;
        case GBA_CPU_CONDITION_HI: return gba_cpu_flagC && !gba_cpu_flagZ;
        case GBA_CPU_CONDITION_LS: return (!gba_cpu_flagC) || gba_cpu_flagZ;
        case GBA_CPU_CONDITION_GE: return gba_cpu_flagN == gba_cpu_flagV;
        case GBA_CPU_CONDITION_LT: return gba_cpu_flagN == gba_cpu_flagV;
        case GBA_CPU_CONDITION_GT: return (!gba_cpu_flagZ) && (gba_cpu_flagN == gba_cpu_flagV);
        case GBA_CPU_CONDITION_LE: return gba_cpu_flagZ || (gba_cpu_flagN != gba_cpu_flagV);
        case GBA_CPU_CONDITION_AL: return true;
        default: return false;
    }
}

static inline void gba_cpu_performJump(uint32_t address) {
    address -= gba_cpu_flagT ? 2 : 4;
    address &= gba_cpu_flagT ? 0xfffffffe : 0xfffffffc;

    gba_cpu_r[15] = address;
    gba_cpu_pipelineState = GBA_CPU_PIPELINESTATE_FLUSH;
}

static inline void gba_cpu_raiseIrq() {
    gba_cpu_spsr_irq = gba_cpu_getCpsr();
    gba_cpu_changeMode(GBA_CPU_MODE_IRQ);
    gba_cpu_r[14] = gba_cpu_r[15] + (gba_cpu_flagT ? 4 : 0);
    gba_cpu_flagT = false;
    gba_cpu_flagI = true;
    gba_cpu_performJump(0x00000018);
}

static inline void gba_cpu_raiseSwi() {
    gba_cpu_spsr_svc = gba_cpu_getCpsr();
    gba_cpu_changeMode(GBA_CPU_MODE_SVC);
    gba_cpu_r[14] = gba_cpu_r[15] - (gba_cpu_flagT ? 2 : 4);
    gba_cpu_flagT = false;
    gba_cpu_flagI = true;
    gba_cpu_performJump(0x00000008);
}

static inline void gba_cpu_raiseUnd() {
    gba_cpu_spsr_und = gba_cpu_getCpsr();
    gba_cpu_changeMode(GBA_CPU_MODE_UND);
    gba_cpu_r[14] = gba_cpu_r[15] - (gba_cpu_flagT ? 2 : 4);
    gba_cpu_flagT = false;
    gba_cpu_flagI = true;
    gba_cpu_performJump(0x00000004);
}

static inline void gba_cpu_execute() {
    if(gba_cpu_pipelineState == GBA_CPU_PIPELINESTATE_EXECUTE) {
        // Check for interrupts
        if(
            (gba_io_read16(0x04000208) & 0x0001)
            && (gba_io_read16(0x04000200) & gba_io_read16(0x04000202) & 0x3fff)
            && !gba_cpu_flagI
        ) {
            gba_cpu_raiseIrq();
        }
        
        if(gba_cpu_flagT) {
            if(gba_cpu_decodedOpcodeThumbHandler) {
                gba_cpu_decodedOpcodeThumbHandler(gba_cpu_decodedOpcodeThumbValue);
            } else {
                gba_cpu_raiseUnd();
            }
        } else {
            if(gba_cpu_decodedOpcodeArmHandler) {
                if(gba_cpu_checkCondition(gba_cpu_decodedOpcodeArmValue >> 28)) {
                    gba_cpu_decodedOpcodeArmHandler(gba_cpu_decodedOpcodeArmValue);
                }
            } else {
                gba_cpu_raiseUnd();
            }
        }
    }
}

static inline void gba_cpu_decode() {
    if(gba_cpu_pipelineState >= GBA_CPU_PIPELINESTATE_DECODE) {
        if(gba_cpu_flagT) {
            gba_cpu_decodedOpcodeThumbValue = gba_cpu_fetchedOpcodeThumb;
            gba_cpu_decodedOpcodeThumbHandler = gba_cpu_decodeTable_thumb[gba_cpu_fetchedOpcodeThumb >> 6];
        } else {
            gba_cpu_decodedOpcodeArmValue = gba_cpu_fetchedOpcodeArm;
            gba_cpu_decodedOpcodeArmHandler = gba_cpu_decodeTable_arm[((gba_cpu_fetchedOpcodeArm >> 16) & 0xff0) | ((gba_cpu_fetchedOpcodeArm >> 4) & 0x0f)];
        }
    }
}

static inline void gba_cpu_fetch(uint32_t fetchAddress) {
    if(gba_cpu_flagT) {
        gba_cpu_fetchedOpcodeThumb = gba_bus_read16(fetchAddress);
        gba_cpu_r[15] += 2;
    } else {
        gba_cpu_fetchedOpcodeArm = gba_bus_read32(fetchAddress);
        gba_cpu_r[15] += 4;
    }
}

static inline void gba_cpu_initDecodeThumb() {
    for(int i = 0; i < 1024; i++) {
        uint16_t opcode = i << 6;

        gba_cpu_decodeTable_thumb[i] = NULL;

        switch((opcode & 0xe000) >> 13) {
            case 0:
            if((opcode & 0x1800) == 0x1800) {
                if(opcode & (1 << 9)) {
                    gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_sub;
                } else {
                    gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_add;
                }
            } else {
                switch((opcode & 0x1800) >> 11) {
                    case 0: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_lsl; break;
                    case 1: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_lsr; break;
                    case 2: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_asr; break;
                }
            }

            break;

            case 1:
            switch((opcode & 0x1800) >> 11) {
                case 0: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_mov; break;
                case 1: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_cmp; break;
                case 2: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_add2; break;
                case 3: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_sub2; break;
            }

            break;

            case 2:
            if(opcode & (1 << 12)) {
                if(opcode & (1 << 9)) {
                    gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_ldrStrh;
                } else {
                    gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_ldrStr;
                }
            } else {
                if(opcode & (1 << 11)) {
                    gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_ldr;
                } else {
                    if(opcode & (1 << 10)) {
                        if((opcode & 0x0380) == 0x0300) {
                            gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_bx;
                        } else if((opcode & 0x0300) != 0x0300) {
                            if((opcode & 0x00c0) != 0x0000) {
                                switch((opcode & 0x0300) >> 8) {
                                    case 0: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_add3; break;
                                    case 1: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_cmp3; break;
                                    case 2: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_mov2; break;
                                }
                            }
                        }
                    } else {
                        switch((opcode & 0x03c0) >> 6) {
                            case 0x0: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_and; break;
                            case 0x1: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_eor; break;
                            case 0x2: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_lsl2; break;
                            case 0x3: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_lsr2; break;
                            case 0x4: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_asr2; break;
                            case 0x5: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_adc; break;
                            case 0x6: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_sbc; break;
                            case 0x7: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_ror; break;
                            case 0x8: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_tst; break;
                            case 0x9: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_neg; break;
                            case 0xa: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_cmp2; break;
                            case 0xb: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_cmn; break;
                            case 0xc: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_orr; break;
                            case 0xd: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_mul; break;
                            case 0xe: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_bic; break;
                            case 0xf: gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_mvn; break;
                        }
                    }
                }
            }

            break;

            case 3:
                gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_ldrStr2;
                break;

            case 4:
                if(opcode & (1 << 12)) {
                    gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_ldrStr3;
                } else {
                    gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_ldrStrh2;
                }

                break;

            case 5:
                if(opcode & (1 << 12)) {
                    if((opcode & 0x0f00) == 0x0000) {
                        gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_add5;
                    } else if((opcode & 0x0600) == 0x0400) {
                        gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_pushPop;
                    }
                } else {
                    gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_add4;
                }

                break;

            case 6:
                if(opcode & (1 << 12)) {
                    if((opcode & 0x0f00) == 0x0f00) {
                        gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_swi;
                    } else {
                        gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_b;
                    }
                } else {
                    gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_ldmStm;
                }

                break;

            case 7:
                if(opcode & (1 << 12)) {
                    gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_bl;
                } else {
                    if(!(opcode & (1 << 11))) {
                        gba_cpu_decodeTable_thumb[i] = gba_cpu_thumb_b2;
                    }
                }

                break;
        }
    }
}

static inline void gba_cpu_initDecodeArm() {
    for(int i = 0; i < 4096; i++) {
        uint32_t opcode = ((i & 0xff0) << 16) | ((i & 0x00f) << 4);

        gba_cpu_decodeTable_arm[i] = NULL;

        switch((opcode & 0x0c000000) >> 26) {
            case 0x0:
                if((opcode & 0x0ff000f0) == 0x01200010) {
                    gba_cpu_decodeTable_arm[i] = gba_cpu_arm_bx;
                } else if((opcode & 0x02000090) == 0x00000090) {
                    if((opcode & 0x0fb000f0) == 0x01000090) {
                        gba_cpu_decodeTable_arm[i] = gba_cpu_arm_swp;
                    } else if((opcode & 0x0fc000f0) == 0x00000090) {
                        gba_cpu_decodeTable_arm[i] = gba_cpu_arm_mul;
                    } else if((opcode & 0x0f8000f0) == 0x00800090) {
                        gba_cpu_decodeTable_arm[i] = gba_cpu_arm_mull;
                    } else if((opcode & 0x0e000090) == 0x00000090) {
                        gba_cpu_decodeTable_arm[i] = gba_cpu_arm_halfwordSignedDataTransfer;
                    }
                } else if((opcode & 0x01900000) == 0x01000000) {
                    if((opcode & (1 << 25)) || ((opcode & 0x000000f0) == 0x00000000)) {
                        gba_cpu_decodeTable_arm[i] = gba_cpu_arm_psrTransfer;
                    }
                } else {
                    switch((opcode & 0x01e00000) >> 21) {
                        case 0x0: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_and; break;
                        case 0x1: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_eor; break;
                        case 0x2: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_sub; break;
                        case 0x3: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_rsb; break;
                        case 0x4: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_add; break;
                        case 0x5: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_adc; break;
                        case 0x6: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_sbc; break;
                        case 0x7: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_rsc; break;
                        case 0x8: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_tst; break;
                        case 0x9: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_teq; break;
                        case 0xa: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_cmp; break;
                        case 0xb: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_cmn; break;
                        case 0xc: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_orr; break;
                        case 0xd: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_mov; break;
                        case 0xe: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_bic; break;
                        case 0xf: gba_cpu_decodeTable_arm[i] = gba_cpu_arm_mvn; break;
                    }
                }

                break;

            case 0x1:
            if((opcode & 0x02000010) != 0x02000010) {
                gba_cpu_decodeTable_arm[i] = gba_cpu_arm_singleDataTransfer;
            }

            break;

            case 0x2:
            if(opcode & (1 << 25)) {
                gba_cpu_decodeTable_arm[i] = gba_cpu_arm_b;
            } else {
                gba_cpu_decodeTable_arm[i] = gba_cpu_arm_blockDataTransfer;
            }

            break;

            case 0x3:
            if((opcode & 0x03000000) == 0x03000000) {
                gba_cpu_decodeTable_arm[i] = gba_cpu_arm_swi;
            }

            break;
        }
    }
}

static inline void gba_cpu_arm_shift(uint32_t opcode) {
    if(!(opcode & 0x02000000)) {
        uint32_t rm = opcode & 0x0000000f;
        uint32_t rm_v = gba_cpu_r[rm];

        if(!(opcode & 0x00000010)) {
            int immediate = (opcode & 0x00000f80) >> 7;

            switch((opcode & 0x060) >> 5) {
                case 0: // LSL
                    if(immediate == 0) {
                        gba_cpu_shifterResult = rm_v;
                        gba_cpu_shifterCarry = gba_cpu_flagC;
                    } else {
                        gba_cpu_shifterResult = rm_v << immediate;
                        gba_cpu_shifterCarry = (rm_v >> (32 - immediate)) & (1 << 0);
                    }

                    break;

                case 1: // LSR
                    if(immediate == 0) {
                        gba_cpu_shifterResult = 0;
                        gba_cpu_shifterCarry = rm_v >> 31;
                    } else {
                        gba_cpu_shifterResult = rm_v >> immediate;
                        gba_cpu_shifterCarry = (rm_v >> (immediate - 1)) & (1 << 0);
                    }

                    break;

                case 2: // ASR
                    if(immediate == 0) {
                        gba_cpu_shifterResult = (int)rm_v >> 31;
                        gba_cpu_shifterCarry = rm_v >> 31;
                    } else {
                        gba_cpu_shifterResult = (int)rm_v >> immediate;
                        gba_cpu_shifterCarry = (rm_v >> (immediate - 1)) & (1 << 0);
                    }

                    break;

                case 3: // ROR/RRX
                    if(immediate == 0) {
                        // RRX
                        gba_cpu_shifterResult = (gba_cpu_flagC ? 1 << 31 : 0) | (rm_v >> 1);
                        gba_cpu_shifterCarry = rm_v & (1 << 0);
                    } else {
                        // ROR
                        gba_cpu_shifterResult = gba_cpu_util_ror32(rm_v, immediate);
                        gba_cpu_shifterCarry = (rm_v >> (immediate - 1)) & (1 << 0);
                    }

                    break;
            }
        } else {
            if(rm == 15) {
                rm_v += 4;
            }

            if((opcode & 0x00000080) != 0) {
                gba_cpu_raiseUnd();
            }

            uint32_t rs = (opcode & 0x00000f00) >> 8;
            uint32_t shift = gba_cpu_r[rs] & 0x000000ff;

            switch((opcode & 0x060) >> 5) {
                case 0: // LSL
                    if(shift == 0) {
                        gba_cpu_shifterResult = rm_v;
                        gba_cpu_shifterCarry = gba_cpu_flagC;
                    } else if(shift < 32) {
                        gba_cpu_shifterResult = rm_v << shift;
                        gba_cpu_shifterCarry = (rm_v >> (32 - shift)) & (1 << 0);
                    } else if(shift == 32) {
                        gba_cpu_shifterResult = 0;
                        gba_cpu_shifterCarry = rm_v & (1 << 0);
                    } else {
                        gba_cpu_shifterResult = 0;
                        gba_cpu_shifterCarry = false;
                    }

                    break;

                case 1: // LSR
                    if(shift == 0) {
                        gba_cpu_shifterResult = rm_v;
                        gba_cpu_shifterCarry = gba_cpu_flagC;
                    } else if(shift < 32) {
                        gba_cpu_shifterResult = rm_v >> shift;
                        gba_cpu_shifterCarry = (rm_v >> (shift - 1)) & (1 << 0);
                    } else if(shift == 32) {
                        gba_cpu_shifterResult = 0;
                        gba_cpu_shifterCarry = rm_v >> 31;
                    } else {
                        gba_cpu_shifterResult = 0;
                        gba_cpu_shifterCarry = false;
                    }

                    break;

                case 2: // ASR
                    if(shift == 0) {
                        gba_cpu_shifterResult = rm_v;
                        gba_cpu_shifterCarry = gba_cpu_flagC;
                    } else if(shift < 32) {
                        gba_cpu_shifterResult = (int)rm_v >> shift;
                        gba_cpu_shifterCarry = (rm_v >> (shift - 1)) & (1 << 0);
                    } else {
                        gba_cpu_shifterResult = (int)rm_v >> 31;
                        gba_cpu_shifterCarry = rm_v >> 31;
                    }

                    break;
                    
                case 3: // ROR
                    {
                        int rotation = shift & 0x1f;

                        if(shift == 0) {
                            gba_cpu_shifterResult = rm_v;
                            gba_cpu_shifterCarry = gba_cpu_flagC;
                        } else if(rotation == 0) {
                            gba_cpu_shifterResult = rm_v;
                            gba_cpu_shifterCarry = rm_v >> 31;
                        } else {
                            gba_cpu_shifterResult = gba_cpu_util_ror32(rm_v, rotation);
                            gba_cpu_shifterCarry = (rm_v >> (rotation - 1)) & (1 << 0);
                        }

                        break;
                    }
            }
        }
    } else {
        uint32_t rotation = (opcode >> 7) & 0x1e;
        uint32_t immediate = opcode & 0xff;

        if(rotation) {
            gba_cpu_shifterResult = gba_cpu_util_ror32(immediate, rotation);
            gba_cpu_shifterCarry = gba_cpu_shifterResult >> 31;
        } else {
            gba_cpu_shifterResult = immediate;
            gba_cpu_shifterCarry = gba_cpu_flagC;
        }
    }
}

static inline uint32_t gba_cpu_util_ror32(uint32_t value, int bits) {
    return (value >> bits) | (value << (32 - bits));
}


static inline void gba_cpu_writeRegister(int rd, uint32_t value) {
    if(rd == 15) {
        gba_cpu_performJump(value);
    } else {
        gba_cpu_r[rd] = value;
    }
}

static inline void gba_cpu_setFlags_logical(uint32_t result) {
    gba_cpu_setFlags_arithmetical(result);
    gba_cpu_flagC = gba_cpu_shifterCarry;
}

static inline void gba_cpu_setFlags_arithmetical(uint32_t result) {
    gba_cpu_flagN = result >> 31;
    gba_cpu_flagZ = result == 0;
}

static inline bool gba_cpu_getCarry_sbc(uint32_t left, uint32_t right, bool carry) {
    return (left >= right) && ((left - right) >= carry);
}

static inline bool gba_cpu_getCarry_sub(uint32_t left, uint32_t right) {
    return left >= right;
}

static inline bool gba_cpu_getOverflow_sub(uint32_t left, uint32_t right, uint32_t result) {
    return ((left ^ right) >> 31) && ((left ^ result) >> 31);
}

static inline bool gba_cpu_getOverflow_add(uint32_t left, uint32_t right, uint32_t result) {
    return (!((left ^ right) >> 31)) && ((left ^ result) >> 31);
}

static inline unsigned int gba_cpu_util_hammingWeight16(uint16_t value) {
    unsigned int count = 0;

    for(int i = 0; i < 16; i++) {
        if(value & (1 << i)) {
            count++;
        }
    }

    return count;
}

static inline unsigned int gba_cpu_util_hammingWeight8(uint8_t value) {
    unsigned int count = 0;

    for(int i = 0; i < 8; i++) {
        if(value & (1 << i)) {
            count++;
        }
    }

    return count;
}

static inline void gba_cpu_arm_halfwordSignedDataTransfer(uint32_t opcode) {
    bool p = (opcode & (1 << 24)) != 0;
    bool u = (opcode & (1 << 23)) != 0;
    bool i = (opcode & (1 << 22)) != 0;
    bool w = (opcode & (1 << 21)) != 0;
    bool l = (opcode & (1 << 20)) != 0;
    bool s = (opcode & (1 << 6)) != 0;
    bool h = (opcode & (1 << 5)) != 0;
    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rd = (opcode & 0x0000f000) >> 12;

    if(!s && !h) {
        gba_cpu_raiseUnd();
    }

    uint32_t addr = gba_cpu_r[rn];
    uint32_t offset;

    if(i) {
        offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    } else {
        if(((opcode & 0x00000f00) >> 8) != 0) {
            gba_cpu_raiseUnd();
        }

        uint32_t rm = opcode & 0x0000000f;

        offset = gba_cpu_r[rm];
    }

    if(p) {
        if(u) {
            addr += offset;
        } else {
            addr -= offset;
        }
    } else {
        w = true;
    }

    if(l) {
        if(w) {
            if(p) {
                gba_cpu_r[rn] = addr;
            } else {
                if(u) {
                    gba_cpu_r[rn] = addr + offset;
                } else {
                    gba_cpu_r[rn] = addr - offset;
                }
            }
        }

        if(!s && h) { // LDRH
            if(addr & (1 << 0)) {
                gba_cpu_writeRegister(rd, gba_cpu_util_ror32(gba_bus_read16(addr), 8));
            } else {
                gba_cpu_writeRegister(rd, gba_bus_read16(addr));
            }
        } else if((!h) || (addr & (1 << 0))) { // LDRSB
            uint32_t readValue = gba_bus_read8(addr);

            if(readValue & (1 << 7)) {
                readValue |= 0xffffff00;
            }

            gba_cpu_writeRegister(rd, readValue);
        } else { // LDRSH
            uint32_t readValue = gba_bus_read16(addr);

            if(readValue & (1 << 15)) {
                readValue |= 0xffff0000;
            }

            gba_cpu_writeRegister(rd, readValue);
        }
    } else {
        uint32_t rd_v = gba_cpu_r[rd];

        if(rd == 15) {
            rd_v += 4;
        }

        if(s) {
            gba_cpu_raiseUnd();
        } else { // STRH
            gba_bus_write16(addr, rd_v);
        }

        if(w) {
            if(p) {
                gba_cpu_r[rn] = addr;
            } else {
                if(u) {
                    gba_cpu_r[rn] = addr + offset;
                } else {
                    gba_cpu_r[rn] = addr - offset;
                }
            }
        }
    }
}

static inline void gba_cpu_arm_blockDataTransfer(uint32_t opcode) {
    bool p = (opcode & (1 << 24)) != 0;
    bool u = (opcode & (1 << 23)) != 0;
    bool s = (opcode & (1 << 22)) != 0;
    bool w = (opcode & (1 << 21)) != 0;
    bool l = (opcode & (1 << 20)) != 0;
    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];
    uint32_t registerCount = gba_cpu_util_hammingWeight16(opcode);
    uint16_t rlist = opcode;

    uint32_t addr = rn_v;

    if(!rlist) {
        registerCount = 16;
    }

    if(p) {
        if(u) {
            addr += 4;
        } else {
            addr -= 4 * registerCount;
        }
    } else {
        if(!u) {
            addr -= 4 * (registerCount - 1);
        }
    }

    if(!rlist) {
        if(l) {
            gba_cpu_performJump(gba_bus_read32(addr));
        } else {
            gba_bus_write32(addr, gba_cpu_r[15] + 4);
        }

        if(u) {
            gba_cpu_r[rn] += 0x40;
        } else {
            gba_cpu_r[rn] -= 0x40;
        }
    } else {
        if(l) { // LDM
            if(w) {
                if(u) {
                    gba_cpu_r[rn] = rn_v + 4 * registerCount;
                } else {
                    gba_cpu_r[rn] = rn_v - 4 * registerCount;
                }
            }

            for(int i = 0; i < 16; i++) {
                if(opcode & (1 << i)) {
                    if(s) {
                        bool r15 = i == 15;
                        bool notUserMode = gba_cpu_mode != GBA_CPU_MODE_USR && gba_cpu_mode != GBA_CPU_MODE_USR_OLD && gba_cpu_mode != GBA_CPU_MODE_SYS;
                        bool registerBanked = ((gba_cpu_mode == GBA_CPU_MODE_FIQ || gba_cpu_mode == GBA_CPU_MODE_FIQ_OLD) && (i >= 8)) || (i == 13) || (i == 14);

                        if(r15) {
                            gba_cpu_setCpsr(gba_cpu_getSpsr());
                            gba_cpu_performJump(gba_bus_read32(addr));
                        } else if(notUserMode && registerBanked) {
                            gba_cpu_r_usr[i - 8] = gba_bus_read32(addr);
                        } else {
                            gba_cpu_r[i] = gba_bus_read32(addr);
                        }
                    } else {
                        gba_cpu_writeRegister(i, gba_bus_read32(addr));
                    }

                    addr += 4;
                }
            }
        } else { // STM
            bool firstCycle = true;
            bool secondCycle = false;

            for(int i = 0; i < 16; i++) {
                if(rlist & (1 << i)) {
                    if(firstCycle) {
                        firstCycle = false;
                        secondCycle = true;
                    } else if(secondCycle) {
                        if(w) {
                            if(u) {
                                gba_cpu_r[rn] = rn_v + registerCount * 4;
                            } else {
                                gba_cpu_r[rn] = rn_v - registerCount * 4;
                            }
                        }

                        secondCycle = false;
                    }

                    if(s) {
                        bool notUserMode = gba_cpu_mode != GBA_CPU_MODE_USR && gba_cpu_mode != GBA_CPU_MODE_USR_OLD && gba_cpu_mode != GBA_CPU_MODE_SYS;
                        bool registerBanked = ((gba_cpu_mode == GBA_CPU_MODE_FIQ || gba_cpu_mode == GBA_CPU_MODE_FIQ_OLD) && (i >= 8)) || (i == 13) || (i == 14);

                        if(notUserMode && registerBanked) {
                            gba_bus_write32(addr, gba_cpu_r_usr[i - 8]);
                        } else {
                            gba_bus_write32(addr, gba_cpu_r[i]);
                        }
                    } else {
                        if(i == 15 && !secondCycle) {
                            gba_bus_write32(addr, gba_cpu_r[15] + 4);
                        } else {
                            gba_bus_write32(addr, gba_cpu_r[i]);
                        }
                    }

                    addr += 4;
                }
            }

            if(w && secondCycle) {
                if(u) {
                    gba_cpu_r[rn] = rn_v + 4;
                } else {
                    gba_cpu_r[rn] = rn_v - 4;
                }
            }
        }
    }
}

static inline void gba_cpu_arm_swp(uint32_t opcode) {
    bool b = (opcode & (1 << 22)) != 0;
    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rd = (opcode & 0x0000f000) >> 12;
    uint32_t rm = opcode & 0x0000000f;

    uint32_t rn_v = gba_cpu_r[rn];

    if(b) {
        uint8_t tmp = gba_bus_read8(rn_v);
        gba_bus_write8(rn_v, gba_cpu_r[rm]);
        gba_cpu_r[rd] = tmp;
    } else {
        uint32_t tmp = gba_cpu_util_ror32(gba_bus_read32(rn_v), (rn_v & 3) << 3);
        gba_bus_write32(rn_v, gba_cpu_r[rm]);
        gba_cpu_r[rd] = tmp;
    }
}

static inline void gba_cpu_arm_mul(uint32_t opcode) {
    bool a = (opcode & (1 << 21)) != 0;
    bool s = (opcode & (1 << 20)) != 0;
    uint32_t rd = (opcode & 0x000f0000) >> 16;
    uint32_t rs = (opcode & 0x00000f00) >> 8;
    uint32_t rm = opcode & 0x0000000f;

    uint32_t result = gba_cpu_r[rm] * gba_cpu_r[rs];

    if(a) {
        uint32_t rn = (opcode & 0x0000f000) >> 12;
        result += gba_cpu_r[rn];
    }

    gba_cpu_r[rd] = result;

    if(s) {
        gba_cpu_setFlags_logical(result);
    }
}

static inline void gba_cpu_arm_mull(uint32_t opcode) {
    bool u = (opcode & (1 << 22)) != 0;
    bool a = (opcode & (1 << 21)) != 0;
    bool s = (opcode & (1 << 20)) != 0;
    uint32_t rdhi = (opcode & 0x000f0000) >> 16;
    uint32_t rdlo = (opcode & 0x0000f000) >> 12;
    uint32_t rs = (opcode & 0x00000f00) >> 8;
    uint32_t rm = opcode & 0x0000000f;

    uint64_t result;

    if(u) {
        int64_t rm_v = (int32_t)gba_cpu_r[rm];
        int64_t rs_v = (int32_t)gba_cpu_r[rs];

        result = rm_v * rs_v;
    } else {
        uint64_t rm_v = gba_cpu_r[rm];
        uint64_t rs_v = gba_cpu_r[rs];

        result = rm_v * rs_v;
    }

    if(a) {
        result += ((uint64_t)gba_cpu_r[rdhi] << 32) | gba_cpu_r[rdlo];
    }

    gba_cpu_r[rdhi] = result >> 32;
    gba_cpu_r[rdlo] = result;

    if(s) {
        gba_cpu_flagZ = result == 0;
        gba_cpu_flagN = result >> 63;
    }
}

static inline void gba_cpu_arm_bx(uint32_t opcode) {
    if((opcode & 0x0ffffff0) != 0x012fff10) {
        gba_cpu_raiseUnd();
    } else {
        uint32_t dest = gba_cpu_r[opcode & 0x0000000f];

        gba_cpu_flagT = dest & (1 << 0);
        gba_cpu_performJump(dest);
    }
}

static inline void gba_cpu_arm_swi(uint32_t opcode) {
    UNUSED(opcode);

    gba_cpu_raiseSwi();
}

static inline void gba_cpu_arm_singleDataTransfer(uint32_t opcode) {
    bool i = (opcode & (1 << 25)) != 0;
    bool p = (opcode & (1 << 24)) != 0;
    bool u = (opcode & (1 << 23)) != 0;
    bool b = (opcode & (1 << 22)) != 0;
    bool w = (opcode & (1 << 21)) != 0;
    bool l = (opcode & (1 << 20)) != 0;
    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rd = (opcode & 0x0000f000) >> 12;
    uint32_t rn_v = gba_cpu_r[rn];
    uint32_t rd_v = gba_cpu_r[rd];

    if((!l) && (rd == 15)) {
        rd_v += 4;
    }

    if(i) {
        gba_cpu_arm_shift(opcode ^ 0x02000000);
    } else {
        gba_cpu_shifterResult = opcode & 0x00000fff;
    }

    uint32_t offset = rn_v;

    if(p) {
        if(u) {
            offset += gba_cpu_shifterResult;
        } else {
            offset -= gba_cpu_shifterResult;
        }
    } else {
        w = true;
    }

    if(w) {
        if(u) {
            if(p) {
                gba_cpu_writeRegister(rn, offset);
            } else {
                gba_cpu_writeRegister(rn, offset + gba_cpu_shifterResult);
            }
        } else {
            if(p) {
                gba_cpu_writeRegister(rn, offset);
            } else {
                gba_cpu_writeRegister(rn, offset - gba_cpu_shifterResult);
            }
        }
    }

    if(l) {
        if(b) {
            gba_cpu_writeRegister(rd, gba_bus_read8(offset));
        } else {
            gba_cpu_writeRegister(rd, gba_cpu_util_ror32(gba_bus_read32(offset), (offset & 0x00000003) << 3));
        }
    } else {
        if(b) {
            gba_bus_write8(offset, rd_v);
        } else {
            gba_bus_write32(offset, rd_v);
        }
    }
}

static inline void gba_cpu_arm_psrTransfer_msr(uint32_t opcode, bool spsr, uint32_t operand) {
    uint32_t mask = 0;

    if(opcode & (1 << 19)) mask |= 0xff000000;
    if(opcode & (1 << 18)) mask |= 0x00ff0000;
    if(opcode & (1 << 17)) mask |= 0x0000ff00;
    if(opcode & (1 << 16)) mask |= 0x000000ff;

    uint32_t registerValue;

    if(spsr) {
        registerValue = gba_cpu_getSpsr();
    } else {
        registerValue = gba_cpu_getCpsr();
    }

    registerValue &= ~mask;
    registerValue |= operand & mask;

    if(spsr) {
        gba_cpu_setSpsr(registerValue);
    } else {
        gba_cpu_setCpsr(registerValue);
    }
}

static inline void gba_cpu_arm_psrTransfer(uint32_t opcode) {
    bool msr = (opcode & (1 << 21)) != 0;
    bool spsr = (opcode & (1 << 22)) != 0;

    if(msr) {
        bool immediate = (opcode & (1 << 25)) != 0;

        if(immediate) {
            uint32_t shift = (opcode & 0x00000f00) >> 8;
            uint32_t imm = opcode & 0x000000ff;

            uint32_t operand = gba_cpu_util_ror32(imm, shift << 1);

            gba_cpu_arm_psrTransfer_msr(opcode, spsr, operand);
        } else {
            if((opcode & 0x00000ff0) != 0x00000000) {
                gba_cpu_raiseUnd();
            }

            uint32_t rm = opcode & 0x0000000f;

            gba_cpu_arm_psrTransfer_msr(opcode, spsr, gba_cpu_r[rm]);
        }
    } else {
        if((opcode & 0x000f0fff) != 0x000f0000) {
            gba_cpu_raiseUnd();
        }

        uint32_t rd = (opcode & 0x0000f000) >> 12;

        if(spsr) {
            gba_cpu_r[rd] = gba_cpu_getSpsr();
        } else {
            gba_cpu_r[rd] = gba_cpu_getCpsr();
        }
    }
}

static inline void gba_cpu_arm_and(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = rn_v & gba_cpu_shifterResult;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_setFlags_logical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_eor(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = rn_v ^ gba_cpu_shifterResult;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_setFlags_logical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_sub(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = rn_v - gba_cpu_shifterResult;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_flagC = gba_cpu_getCarry_sub(rn_v, gba_cpu_shifterResult);
            gba_cpu_flagV = gba_cpu_getOverflow_sub(rn_v, gba_cpu_shifterResult, result);
            gba_cpu_setFlags_arithmetical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_rsb(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = gba_cpu_shifterResult - rn_v;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_flagC = gba_cpu_getCarry_sub(gba_cpu_shifterResult, rn_v);
            gba_cpu_flagV = gba_cpu_getOverflow_sub(gba_cpu_shifterResult, rn_v, result);
            gba_cpu_setFlags_arithmetical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_add(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = rn_v + gba_cpu_shifterResult;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_flagC = result < rn_v;
            gba_cpu_flagV = gba_cpu_getOverflow_add(rn_v, gba_cpu_shifterResult, result);
            gba_cpu_setFlags_arithmetical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_adc(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint64_t result = (uint64_t)rn_v + (uint64_t)gba_cpu_shifterResult + gba_cpu_flagC;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_flagC = result > UINT32_MAX;
            gba_cpu_flagV = gba_cpu_getOverflow_add(rn_v, gba_cpu_shifterResult, result);
            gba_cpu_setFlags_arithmetical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_sbc(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint64_t result = (uint64_t)rn_v - (uint64_t)gba_cpu_shifterResult - !gba_cpu_flagC;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_flagC = gba_cpu_getCarry_sbc(rn_v, gba_cpu_shifterResult, gba_cpu_flagC);
            gba_cpu_flagV = gba_cpu_getOverflow_sub(rn_v, gba_cpu_shifterResult, result);
            gba_cpu_setFlags_arithmetical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_rsc(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint64_t result = (uint64_t)gba_cpu_shifterResult - (uint64_t)rn_v - !gba_cpu_flagC;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_flagC = gba_cpu_getCarry_sbc(gba_cpu_shifterResult, rn_v, gba_cpu_flagC);
            gba_cpu_flagV = gba_cpu_getOverflow_sub(gba_cpu_shifterResult, rn_v, result);
            gba_cpu_setFlags_arithmetical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_tst(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = rn_v & gba_cpu_shifterResult;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_setFlags_logical(result);
        }
    }
}

static inline void gba_cpu_arm_teq(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = rn_v ^ gba_cpu_shifterResult;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_setFlags_logical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_cmp(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = rn_v - gba_cpu_shifterResult;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_flagC = gba_cpu_getCarry_sub(rn_v, gba_cpu_shifterResult);
            gba_cpu_flagV = gba_cpu_getOverflow_sub(rn_v, gba_cpu_shifterResult, result);
            gba_cpu_setFlags_arithmetical(result);
        }
    }
}

static inline void gba_cpu_arm_cmn(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = rn_v + gba_cpu_shifterResult;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_flagC = result < rn_v;
            gba_cpu_flagV = gba_cpu_getOverflow_add(rn_v, gba_cpu_shifterResult, result);
            gba_cpu_setFlags_arithmetical(result);
        }
    }
}

static inline void gba_cpu_arm_orr(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = rn_v | gba_cpu_shifterResult;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_setFlags_logical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_mov(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_setFlags_logical(gba_cpu_shifterResult);
        }
    }

    gba_cpu_writeRegister(rd, gba_cpu_shifterResult);
}

static inline void gba_cpu_arm_bic(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rn = (opcode & 0x000f0000) >> 16;
    uint32_t rn_v = gba_cpu_r[rn];

    if(rn == 15) {
        if((!(opcode & (1 << 25))) && (opcode & (1 << 4))) {
            rn_v += 4;
        }
    }

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = rn_v & ~gba_cpu_shifterResult;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_setFlags_logical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_mvn(uint32_t opcode) {
    gba_cpu_arm_shift(opcode);

    uint32_t rd = (opcode & 0x0000f000) >> 12;
    bool s = (opcode & (1 << 20)) != 0;

    uint32_t result = ~gba_cpu_shifterResult;

    if(s) {
        if(rd == 15) {
            gba_cpu_setCpsr(gba_cpu_getSpsr());
        } else {
            gba_cpu_setFlags_logical(result);
        }
    }

    gba_cpu_writeRegister(rd, result);
}

static inline void gba_cpu_arm_b(uint32_t opcode) {
    bool l = (opcode & (1 << 24)) != 0;

    if(l) {
        gba_cpu_r[14] = gba_cpu_r[15] - 4;
    }

    uint32_t offset = (opcode & 0x00ffffff) << 2;

    if(offset & (1 << 25)) {
        offset |= 0xfc000000;
    }

    gba_cpu_performJump(gba_cpu_r[15] + offset);
}

static inline void gba_cpu_thumb_sub(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;
    bool i = (opcode & (1 << 10)) != 0;
    uint32_t rs_v = gba_cpu_r[rs];

    uint32_t op2;

    if(i) {
        op2 = (opcode & 0x01c0) >> 6;
    } else {
        uint16_t rn = (opcode & 0x01c0) >> 6;
        op2 = gba_cpu_r[rn];
    }

    uint32_t result = rs_v - op2;

    gba_cpu_flagC = gba_cpu_getCarry_sub(rs_v, op2);
    gba_cpu_flagV = gba_cpu_getOverflow_sub(rs_v, op2, result);
    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_add(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;
    bool i = (opcode & (1 << 10)) != 0;
    uint32_t rs_v = gba_cpu_r[rs];

    uint32_t op2;

    if(i) {
        op2 = (opcode & 0x01c0) >> 6;
    } else {
        uint16_t rn = (opcode & 0x01c0) >> 6;
        op2 = gba_cpu_r[rn];
    }

    uint32_t result = rs_v + op2;

    gba_cpu_flagC = result < op2;
    gba_cpu_flagV = gba_cpu_getOverflow_add(rs_v, op2, result);
    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_lsl(uint16_t opcode) {
    uint16_t offset = (opcode & 0x07c0) >> 6;
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];

    if(offset) {
        gba_cpu_r[rd] = rs_v << offset;
        gba_cpu_flagC = rs_v >> (32 - offset);
    } else {
        gba_cpu_r[rd] = rs_v;
    }

    gba_cpu_setFlags_arithmetical(gba_cpu_r[rd]);
}

static inline void gba_cpu_thumb_lsr(uint16_t opcode) {
    uint16_t offset = (opcode & 0x07c0) >> 6;
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];

    if(offset) {
        gba_cpu_r[rd] = rs_v >> offset;
        gba_cpu_flagC = (rs_v >> (offset - 1)) & (1 << 0);
    } else {
        gba_cpu_r[rd] = 0;
        gba_cpu_flagC = rs_v >> 31;
    }

    gba_cpu_setFlags_arithmetical(gba_cpu_r[rd]);
}

static inline void gba_cpu_thumb_asr(uint16_t opcode) {
    uint16_t offset = (opcode & 0x07c0) >> 6;
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];

    if(offset) {
        gba_cpu_r[rd] = (int)rs_v >> offset;
        gba_cpu_flagC = (rs_v >> (offset - 1)) & (1 << 0);
    } else {
        gba_cpu_r[rd] = (int)rs_v >> 31;
        gba_cpu_flagC = rs_v >> 31;
    }

    gba_cpu_setFlags_arithmetical(gba_cpu_r[rd]);
}

static inline void gba_cpu_thumb_mov(uint16_t opcode) {
    uint16_t rd = (opcode & 0x0700) >> 8;
    uint16_t offset = opcode & 0x00ff;

    gba_cpu_r[rd] = offset;

    gba_cpu_flagZ = offset == 0;
    gba_cpu_flagN = false;
}

static inline void gba_cpu_thumb_cmp(uint16_t opcode) {
    uint16_t rd = (opcode & 0x0700) >> 8;
    uint16_t offset = opcode & 0x00ff;

    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rd_v - offset;

    gba_cpu_flagC = gba_cpu_getCarry_sub(rd_v, offset);
    gba_cpu_flagV = gba_cpu_getOverflow_sub(rd_v, offset, result);
    gba_cpu_setFlags_arithmetical(result);
}

static inline void gba_cpu_thumb_add2(uint16_t opcode) {
    uint16_t rd = (opcode & 0x0700) >> 8;
    uint16_t offset = opcode & 0x00ff;

    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rd_v + offset;

    gba_cpu_flagC = result < offset;
    gba_cpu_flagV = gba_cpu_getOverflow_add(rd_v, offset, result);
    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_sub2(uint16_t opcode) {
    uint16_t rd = (opcode & 0x0700) >> 8;
    uint16_t offset = opcode & 0x00ff;

    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rd_v - offset;

    gba_cpu_flagC = gba_cpu_getCarry_sub(rd_v, offset);
    gba_cpu_flagV = gba_cpu_getOverflow_sub(rd_v, offset, result);
    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_ldrStrh(uint16_t opcode) {
    bool h = (opcode & (1 << 11)) != 0;
    bool s = (opcode & (1 << 10)) != 0;
    uint16_t ro = (opcode & 0x01c0) >> 6;
    uint16_t rb = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t ro_v = gba_cpu_r[ro];
    uint32_t rb_v = gba_cpu_r[rb];

    uint32_t offset = ro_v + rb_v;

    if(s) {
        if(h) {
            uint32_t result = gba_bus_read16(offset);

            if(result & (1 << 15)) {
                result |= 0xffff0000;
            }

            gba_cpu_r[rd] = result;
        } else {
            uint32_t result = gba_bus_read8(offset);

            if(result & (1 << 7)) {
                result |= 0xffffff00;
            }

            gba_cpu_r[rd] = result;
        }
    } else {
        if(h) {
            gba_cpu_r[rd] = gba_bus_read16(offset);
        } else {
            uint32_t rd_v = gba_cpu_r[rd];
            gba_bus_write16(offset, rd_v);
        }
    }
}

static inline void gba_cpu_thumb_ldrStr(uint16_t opcode) {
    bool l = (opcode & (1 << 11)) != 0;
    bool b = (opcode & (1 << 10)) != 0;
    uint16_t ro = (opcode & 0x01c0) >> 6;
    uint16_t rb = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t ro_v = gba_cpu_r[ro];
    uint32_t rb_v = gba_cpu_r[rb];

    uint32_t offset = ro_v + rb_v;

    if(l) {
        if(b) {
            gba_cpu_r[rd] = gba_bus_read8(offset);
        } else {
            gba_cpu_r[rd] = gba_cpu_util_ror32(gba_bus_read32(offset), (offset & 0x00000003) << 3);
        }
    } else {
        uint32_t rd_v = gba_cpu_r[rd];

        if(b) {
            gba_bus_write8(offset, rd_v);
        } else {
            gba_bus_write32(offset, rd_v);
        }
    }
}

static inline void gba_cpu_thumb_ldr(uint16_t opcode) {
    uint16_t rd = (opcode & 0x0700) >> 8;
    uint16_t immediate = opcode & 0x00ff;
    uint32_t result = (gba_cpu_r[15] & 0xfffffffc) + (immediate << 2);

    gba_cpu_r[rd] = gba_bus_read32(result);
}

static inline void gba_cpu_thumb_bx(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;

    if(opcode & (1 << 6)) {
        rs += 8;
    }

    uint32_t dest = gba_cpu_r[rs];

    gba_cpu_flagT = dest & (1 << 0);
    gba_cpu_performJump(dest);
}

static inline void gba_cpu_thumb_add3(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    if(opcode & (1 << 7)) {
        rd += 8;
    }

    if(opcode & (1 << 6)) {
        rs += 8;
    }

    gba_cpu_writeRegister(rd, gba_cpu_r[rd] + gba_cpu_r[rs]);
}

static inline void gba_cpu_thumb_cmp3(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    if(opcode & (1 << 7)) {
        rd += 8;
    }

    if(opcode & (1 << 6)) {
        rs += 8;
    }

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rd_v - rs_v;
    gba_cpu_flagC = gba_cpu_getCarry_sub(rd_v, rs_v);
    gba_cpu_flagV = gba_cpu_getOverflow_sub(rd_v, rs_v, result);
    gba_cpu_setFlags_arithmetical(result);
}

static inline void gba_cpu_thumb_mov2(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    if(opcode & (1 << 7)) {
        rd += 8;
    }

    if(opcode & (1 << 6)) {
        rs += 8;
    }

    gba_cpu_writeRegister(rd, gba_cpu_r[rs]);
}

static inline void gba_cpu_thumb_and(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rs_v & rd_v;

    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_eor(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rs_v ^ rd_v;

    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_lsl2(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result;

    if(rs_v == 0) {
        result = rd_v;
    } else if(rs_v < 32) {
        result = rd_v << rs_v;
        gba_cpu_flagC = (rd_v >> (32 - rs_v)) & (1 << 0);
    } else if(rs_v == 32) {
        result = 0;
        gba_cpu_flagC = rd_v & (1 << 0);
    } else {
        result = 0;
        gba_cpu_flagC = false;
    }

    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_lsr2(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result;

    if(rs_v == 0) {
        result = rd_v;
    } else if(rs_v < 32) {
        result = rd_v >> rs_v;
        gba_cpu_flagC = (rd_v >> (rs_v - 1)) & (1 << 0);
    } else if(rs_v == 32) {
        result = 0;
        gba_cpu_flagC = rd_v >> 31;
    } else {
        result = 0;
        gba_cpu_flagC = false;
    }

    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_asr2(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result;

    if(rs_v == 0) {
        result = rd_v;
    } else if(rs_v < 32) {
        result = (int)rd_v >> rs_v;
        gba_cpu_flagC = (rd_v >> (rs_v - 1)) & (1 << 0);
    } else {
        result = (int)rd_v >> 31;
        gba_cpu_flagC = rd_v >> 31;
    }

    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_adc(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint64_t result = (uint64_t)rd_v + (uint64_t)rs_v + gba_cpu_flagC;

    gba_cpu_flagC = result > UINT32_MAX;
    gba_cpu_flagV = gba_cpu_getOverflow_add(rd_v, rs_v, result);
    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_sbc(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint64_t result = (uint64_t)rd_v - (uint64_t)rs_v - !gba_cpu_flagC;

    gba_cpu_flagC = gba_cpu_getCarry_sbc(rd_v, rs_v, gba_cpu_flagC);
    gba_cpu_flagV = gba_cpu_getOverflow_sub(rd_v, rs_v, result);
    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_ror(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result;
    int rotation = rs_v & 0x1f;

    if(rs_v == 0) {
        result = rd_v;
    } else if(rotation == 0) {
        result = rd_v;
        gba_cpu_flagC = rd_v >> 31;
    } else {
        result = gba_cpu_util_ror32(rd_v, rotation);
        gba_cpu_flagC = (rd_v >> (rotation - 1)) & (1 << 0);
    }

    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_tst(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rs_v & rd_v;

    gba_cpu_setFlags_arithmetical(result);
}

static inline void gba_cpu_thumb_neg(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];

    uint32_t result = -rs_v;
    gba_cpu_flagC = gba_cpu_getCarry_sub(0, rs_v);
    gba_cpu_flagV = gba_cpu_getOverflow_sub(0, rs_v, result);

    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_cmp2(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rd_v - rs_v;

    gba_cpu_flagC = gba_cpu_getCarry_sub(rd_v, rs_v);
    gba_cpu_flagV = gba_cpu_getOverflow_sub(rd_v, rs_v, result);
    gba_cpu_setFlags_arithmetical(result);
}

static inline void gba_cpu_thumb_cmn(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rd_v + rs_v;

    gba_cpu_flagC = result < rd_v;
    gba_cpu_flagV = gba_cpu_getOverflow_add(rd_v, rs_v, result);
    gba_cpu_setFlags_arithmetical(result);
}

static inline void gba_cpu_thumb_orr(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rs_v | rd_v;

    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_mul(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rs_v * rd_v;

    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_bic(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];
    uint32_t rd_v = gba_cpu_r[rd];

    uint32_t result = rs_v & ~rd_v;

    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_mvn(uint16_t opcode) {
    uint16_t rs = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rs_v = gba_cpu_r[rs];

    uint32_t result = ~rs_v;

    gba_cpu_setFlags_arithmetical(result);

    gba_cpu_r[rd] = result;
}

static inline void gba_cpu_thumb_ldrStr2(uint16_t opcode) {
    bool b = (opcode & (1 << 12)) != 0;
    bool l = (opcode & (1 << 11)) != 0;
    uint16_t offset = (opcode & 0x07c0) >> 6;
    uint16_t rb = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rb_v = gba_cpu_r[rb];

    if(b) {
        uint32_t addr = rb_v + offset;

        if(l) {
            gba_cpu_r[rd] = gba_bus_read8(addr);
        } else {
            uint32_t rd_v = gba_cpu_r[rd];
            gba_bus_write8(addr, rd_v);
        }
    } else {
        uint32_t addr = rb_v + (offset << 2);

        if(l) {
            gba_cpu_r[rd] = gba_cpu_util_ror32(gba_bus_read32(addr), (addr & 0x00000003) << 3);
        } else {
            uint32_t rd_v = gba_cpu_r[rd];
            gba_bus_write32(addr, rd_v);
        }
    }
}

static inline void gba_cpu_thumb_ldrStr3(uint16_t opcode) {
    bool l = (opcode & (1 << 11)) != 0;

    uint16_t rd = (opcode & 0x0700) >> 8;
    uint16_t immediate = opcode & 0x00ff;

    uint32_t offset = gba_cpu_r[13] + (immediate << 2);

    if(l) {
        gba_cpu_r[rd] = gba_cpu_util_ror32(gba_bus_read32(offset), (offset & 0x00000003) << 3);
    } else {
        uint32_t rd_v = gba_cpu_r[rd];
        gba_bus_write32(offset, rd_v);
    }
}

static inline void gba_cpu_thumb_ldrStrh2(uint16_t opcode) {
    bool l = (opcode & (1 << 11)) != 0;
    uint16_t offset = (opcode & 0x07c0) >> 6;
    uint16_t rb = (opcode & 0x0038) >> 3;
    uint16_t rd = opcode & 0x0007;

    uint32_t rb_v = gba_cpu_r[rb];

    uint32_t addr = rb_v + (offset << 1);

    if(l) {
        gba_cpu_r[rd] = gba_bus_read16(addr);
    } else {
        uint32_t rd_v = gba_cpu_r[rd];
        gba_bus_write16(addr, rd_v);
    }
}

static inline void gba_cpu_thumb_add5(uint16_t opcode) {
    bool s = (opcode & (1 << 7)) != 0;
    uint16_t immediate = (opcode & 0x007f) << 2;

    if(s) {
        gba_cpu_r[13] -= immediate;
    } else {
        gba_cpu_r[13] += immediate;
    }
}

static inline void gba_cpu_thumb_pushPop(uint16_t opcode) {
    bool l = (opcode & (1 << 11)) != 0;
    bool r = (opcode & (1 << 8)) != 0;
    uint8_t rlist = opcode;
    uint32_t registerCount = gba_cpu_util_hammingWeight8(rlist);

    if(r) {
        registerCount++;
    }

    uint32_t addr;

    if(l) {
        addr = gba_cpu_r[13];
        gba_cpu_r[13] += registerCount * 4;
    } else {
        gba_cpu_r[13] -= registerCount * 4;
        addr = gba_cpu_r[13];
    }

    for(int i = 0; i < 8; i++) {
        if(rlist & (1 << i)) {
            if(l) {
                gba_cpu_r[i] = gba_bus_read32(addr);
            } else {
                gba_bus_write32(addr, gba_cpu_r[i]);
            }

            addr += 4;
        }
    }

    if(r) {
        if(l) {
            gba_cpu_performJump(gba_bus_read32(addr));
        } else {
            gba_bus_write32(addr, gba_cpu_r[14]);
        }
    }
}

static inline void gba_cpu_thumb_add4(uint16_t opcode) {
    bool sp = (opcode & (1 << 11));
    uint16_t rd = (opcode & 0x07c0) >> 8;
    uint16_t immediate = opcode & 0x00ff;

    if(sp) {
        gba_cpu_r[rd] = gba_cpu_r[13] + (immediate << 2);
    } else {
        gba_cpu_r[rd] = (gba_cpu_r[15] & 0xfffffffc) + (immediate << 2);
    }
}

static inline void gba_cpu_thumb_swi(uint16_t opcode) {
    UNUSED(opcode);

    gba_cpu_raiseSwi();
}

static inline void gba_cpu_thumb_b(uint16_t opcode) {
    gba_cpu_condition_t condition = (opcode & 0x0f00) >> 8;
    int8_t immediate = opcode;

    if(gba_cpu_checkCondition(condition)) {
        gba_cpu_performJump(gba_cpu_r[15] + (immediate << 1));
    }
}

static inline void gba_cpu_thumb_ldmStm(uint16_t opcode) {
    bool l = (opcode & (1 << 11)) != 0;
    uint16_t rb = (opcode & 0x0700) >> 8;
    uint8_t rlist = opcode;

    for(int i = 0; i < 7; i++) {
        if(rlist & (1 << i)) {
            if(l) {
                gba_cpu_r[i] = gba_bus_read32(gba_cpu_r[rb]);
            } else {
                gba_bus_write32(gba_cpu_r[rb], gba_cpu_r[i]);
            }

            gba_cpu_r[rb] += 4;
        }
    }
}

static inline void gba_cpu_thumb_bl(uint16_t opcode) {
    bool h = (opcode & (1 << 11)) != 0;
    uint32_t offset = opcode & 0x07ff;

    if(h) {
        gba_cpu_r[14] += (offset << 1);
        
        uint32_t pc_v = gba_cpu_r[15];
        
        gba_cpu_performJump(gba_cpu_r[14]);

        gba_cpu_r[14] = (pc_v - 2) | 0x00000001;
    } else {
        if(opcode & (1 << 10)) {
            offset |= 0xfffff800;
        }

        gba_cpu_r[14] = gba_cpu_r[15] + (offset << 12);
    }
}

static inline void gba_cpu_thumb_b2(uint16_t opcode) {
    uint32_t immediate = opcode & 0x07ff;

    if(opcode & (1 << 10)) {
        immediate |= 0xfffff800;
    }

    gba_cpu_performJump(gba_cpu_r[15] + (immediate << 1));
}
