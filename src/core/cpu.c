#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

void gba_cpu_init() {

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

        case GBA_CPU_MODE_IRQ_OLD:
        case GBA_CPU_MODE_IRQ:
            gba_cpu_spsr_irq = value;

        case GBA_CPU_MODE_SVC_OLD:
        case GBA_CPU_MODE_SVC:
            gba_cpu_spsr_svc = value;

        case GBA_CPU_MODE_ABT:
            gba_cpu_spsr_abt = value;

        case GBA_CPU_MODE_UND:
            gba_cpu_spsr_und = value;
            
        default:
            gba_cpu_setCpsr(value);
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
    // Check for interrupts
    if(
        (gba_io_read16(0x04000208) & 0x0001)
        && (gba_io_read16(0x04000200) & gba_io_read16(0x04000202) & 0x3fff)
        && !gba_cpu_flagI
    ) {
        gba_cpu_raiseIrq();
    }

    if(gba_cpu_pipelineState == GBA_CPU_PIPELINESTATE_EXECUTE) {
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
                    gba_cpu_decodeTable_thumb[i] = NULL;
                } else {
                    gba_cpu_decodeTable_thumb[i] = NULL;
                }
            } else {
                switch((opcode & 0x1800) >> 11) {
                    case 0: gba_cpu_decodeTable_thumb[i] = NULL; break;
                    case 1: gba_cpu_decodeTable_thumb[i] = NULL; break;
                    case 2: gba_cpu_decodeTable_thumb[i] = NULL; break;
                }
            }

            break;

            case 1:
            switch((opcode & 0x1800) >> 11) {
                case 0: gba_cpu_decodeTable_thumb[i] = NULL; break;
                case 1: gba_cpu_decodeTable_thumb[i] = NULL; break;
                case 2: gba_cpu_decodeTable_thumb[i] = NULL; break;
                case 3: gba_cpu_decodeTable_thumb[i] = NULL; break;
            }

            break;

            case 2:
            if(opcode & (1 << 12)) {
                if(opcode & (1 << 9)) {
                    gba_cpu_decodeTable_thumb[i] = NULL;
                } else {
                    gba_cpu_decodeTable_thumb[i] = NULL;
                }
            } else {
                if(opcode & (1 << 11)) {
                    gba_cpu_decodeTable_thumb[i] = NULL;
                } else {
                    if(opcode & (1 << 10)) {
                        if((opcode & 0x0380) == 0x0300) {
                            gba_cpu_decodeTable_thumb[i] = NULL;
                        } else if((opcode & 0x0300) != 0x0300) {
                            if((opcode & 0x00c0) != 0x0000) {
                                switch((opcode & 0x0300) >> 8) {
                                    case 0: gba_cpu_decodeTable_thumb[i] = NULL; break;
                                    case 1: gba_cpu_decodeTable_thumb[i] = NULL; break;
                                    case 2: gba_cpu_decodeTable_thumb[i] = NULL; break;
                                }
                            }
                        }
                    } else {
                        switch((opcode & 0x03c0) >> 6) {
                            case 0x0: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0x1: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0x2: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0x3: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0x4: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0x5: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0x6: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0x7: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0x8: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0x9: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0xa: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0xb: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0xc: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0xd: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0xe: gba_cpu_decodeTable_thumb[i] = NULL; break;
                            case 0xf: gba_cpu_decodeTable_thumb[i] = NULL; break;
                        }
                    }
                }
            }

            break;

            case 3:
                gba_cpu_decodeTable_thumb[i] = NULL;
                break;

            case 4:
                if(opcode & (1 << 12)) {
                    gba_cpu_decodeTable_thumb[i] = NULL;
                } else {
                    gba_cpu_decodeTable_thumb[i] = NULL;
                }

                break;

            case 5:
                if(opcode & (1 << 12)) {
                    if((opcode & 0x0f00) == 0x0000) {
                        gba_cpu_decodeTable_thumb[i] = NULL;
                    } else if((opcode & 0x0600) == 0x0400) {
                        gba_cpu_decodeTable_thumb[i] = NULL;
                    }
                } else {
                    gba_cpu_decodeTable_thumb[i] = NULL;
                }

                break;

            case 6:
                if(opcode & (1 << 12)) {
                    if((opcode & 0x0f00) == 0x0f00) {
                        gba_cpu_decodeTable_thumb[i] = NULL;
                    } else {
                        gba_cpu_decodeTable_thumb[i] = NULL;
                    }
                } else {
                    gba_cpu_decodeTable_thumb[i] = NULL;
                }

                break;

            case 7:
                if(opcode & (1 << 12)) {
                    gba_cpu_decodeTable_thumb[i] = NULL;
                } else {
                    if(!(opcode & (1 << 11))) {
                        gba_cpu_decodeTable_thumb[i] = NULL;
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
                    gba_cpu_decodeTable_arm[i] = NULL;
                } else if((opcode & 0x02000090) == 0x00000090) {
                    if((opcode & 0x0fb000f0) == 0x01000090) {
                        gba_cpu_decodeTable_arm[i] = NULL;
                    } else if((opcode & 0x0fc000f0) == 0x00000090) {
                        gba_cpu_decodeTable_arm[i] = NULL;
                    } else if((opcode & 0x0f8000f0) == 0x00800090) {
                        gba_cpu_decodeTable_arm[i] = NULL;
                    } else if((opcode & 0x0e000090) == 0x00000090) {
                        gba_cpu_decodeTable_arm[i] = NULL;
                    }
                } else if((opcode & 0x01900000) == 0x01000000) {
                    if((opcode & (1 << 25)) || ((opcode & 0x000000f0) == 0x00000000)) {
                        gba_cpu_decodeTable_arm[i] = NULL;
                    }
                } else {
                    switch((opcode & 0x01e00000) >> 21) {
                        case 0x0: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0x1: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0x2: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0x3: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0x4: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0x5: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0x6: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0x7: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0x8: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0x9: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0xa: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0xb: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0xc: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0xd: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0xe: gba_cpu_decodeTable_arm[i] = NULL; break;
                        case 0xf: gba_cpu_decodeTable_arm[i] = NULL; break;
                    }
                }

                break;

            case 0x1:
            if((opcode & 0x02000010) != 0x02000010) {
                gba_cpu_decodeTable_arm[i] = NULL;
            }

            break;

            case 0x2:
            if(opcode & (1 << 25)) {
                gba_cpu_decodeTable_arm[i] = NULL;
            } else {
                gba_cpu_decodeTable_arm[i] = NULL;
            }

            break;

            case 0x3:
            if((opcode & 0x03000000) == 0x03000000) {
                gba_cpu_decodeTable_arm[i] = NULL;
            }
        }
    }
}
