#include "core/cpu/cpu.h"
#include "core/cpu/op_arm_b.h"
#include "core/cpu/op_arm_blockdatatransfer.h"
#include "core/cpu/op_arm_bx.h"
#include "core/cpu/op_arm_dataprocessing.h"
#include "core/cpu/op_arm_halfwordsigneddatatransfer.h"
#include "core/cpu/op_arm_mul.h"
#include "core/cpu/op_arm_mull.h"
#include "core/cpu/op_arm_psrtransfer.h"
#include "core/cpu/op_arm_singledatatransfer.h"
#include "core/cpu/op_arm_swi.h"
#include "core/cpu/op_arm_swp.h"
#include "core/cpu/op_arm_und.h"
#include "core/cpu/op_thumb_und.h"

static struct {
    void (*arm[4096])(uint32_t p_opcode);
    void (*thumb[1024])(uint16_t p_opcode);
} s_cpuDecodeTable;

static void cpuInitArmDecodeTable(void);
static void cpuInitThumbDecodeTable(void);

void cpuDecode(void) {
    if(g_cpuPipelineState >= E_CPUPIPELINESTATE_DECODE) {
        if(g_cpuFlagT) {
            // Take bits 6 to 15
            uint16_t l_decodeIndex = g_cpuFetchedOpcode.thumb >> 6;

            g_cpuDecodedOpcode.thumb.opcode = g_cpuFetchedOpcode.thumb;
            g_cpuDecodedOpcode.thumb.handler = s_cpuDecodeTable
                .thumb[l_decodeIndex];
        } else {
            uint16_t l_decodeIndex;

            // Take bits 20 to 27
            l_decodeIndex = (g_cpuFetchedOpcode.arm >> 16) & 0x00000ff0;

            // Take bits 4 to 7
            l_decodeIndex |= (g_cpuFetchedOpcode.arm >> 4) & 0x0000000f;

            g_cpuDecodedOpcode.arm.opcode = g_cpuFetchedOpcode.arm;
            g_cpuDecodedOpcode.arm.handler = s_cpuDecodeTable
                .arm[l_decodeIndex];
        }
    }
}

void cpuDecoderInit(void) {
    cpuInitArmDecodeTable();
    cpuInitThumbDecodeTable();
}

static void cpuInitArmDecodeTable(void) {
    for(int l_index = 0; l_index < 4096; l_index++) {
        void (*l_result)(uint32_t) = cpuOpcodeArmUnd;

        const bool l_isBranchExchange = l_index == 0x121;
        const bool l_isBranch = (l_index & 0xe00) == 0xa00;
        const bool l_isBranchLink = (l_index & 0x100) == 0x100;
        const bool l_isDataProcImmediate = (l_index & 0xe00) == 0x200;
        const bool l_isDataProcRegister = (l_index & 0xe00) == 0x000;
        const bool l_isDataProcInvalidShift = (l_index & 0x009) == 0x009;
        const bool l_isDataProcInvalidTest = (l_index & 0x190) == 0x100;
        const bool l_isMrs = (l_index & 0xfbf) == 0x100;
        const bool l_isMsrImmediate = (l_index & 0xfb0) == 0x320;
        const bool l_isMsrRegister = (l_index & 0xfbf) == 0x120;
        const bool l_isMul = (l_index & 0xfcf) == 0x009;
        const bool l_isMull = (l_index & 0xf8f) == 0x089;
        const bool l_isSingleDataTransfer = (l_index & 0xc00) == 0x400;
        const bool l_isInvalidSingleDataTransfer = (l_index & 0xe01) == 0x601;
        const bool l_isHalfwordSignedDataTransfer = (l_index & 0xe09) == 0x009;
        const bool l_isHalfwordSignedDataTransferSwap = (l_index & 0xe0f) == 0x000;
        const bool l_isBlockDataTransfer = (l_index & 0xe00) == 0x800;
        const bool l_isSwap = (l_index & 0xfbf) == 0x109;
        const bool l_isSwi = (l_index & 0xf00) == 0xf00;

        if(l_isBranchExchange) {
            l_result = cpuOpcodeArmBx;
        } else if(l_isBranch) {
            if(!l_isBranchLink) {
                l_result = cpuOpcodeArmB;
            } else {
                l_result = cpuOpcodeArmBl;
            }
        } else if(
            !l_isDataProcInvalidTest
            && (
                l_isDataProcImmediate
                || (l_isDataProcRegister && !l_isDataProcInvalidShift)
            )
        ) {
            switch((l_index >> 5) & 0xf) {
                case 0: l_result = cpuOpcodeArmAnd; break;
                case 1: l_result = cpuOpcodeArmEor; break;
                case 2: l_result = cpuOpcodeArmSub; break;
                case 3: l_result = cpuOpcodeArmRsb; break;
                case 4: l_result = cpuOpcodeArmAdd; break;
                case 5: l_result = cpuOpcodeArmAdc; break;
                case 6: l_result = cpuOpcodeArmSbc; break;
                case 7: l_result = cpuOpcodeArmRsc; break;
                case 8: l_result = cpuOpcodeArmTst; break;
                case 9: l_result = cpuOpcodeArmTeq; break;
                case 10: l_result = cpuOpcodeArmCmp; break;
                case 11: l_result = cpuOpcodeArmCmn; break;
                case 12: l_result = cpuOpcodeArmOrr; break;
                case 13: l_result = cpuOpcodeArmMov; break;
                case 14: l_result = cpuOpcodeArmBic; break;
                case 15: l_result = cpuOpcodeArmMvn; break;
            }
        } else if(
            l_isMsrImmediate
            || l_isMsrRegister
            || l_isMrs
        ) {
            l_result = l_isMrs ? cpuOpcodeArmMrs : cpuOpcodeArmMsr;
        } else if(l_isMul) {
            l_result = cpuOpcodeArmMul;
        } else if(l_isMull) {
            l_result = cpuOpcodeArmMull;
        } else if(l_isSingleDataTransfer && !l_isInvalidSingleDataTransfer) {
            l_result = cpuOpcodeArmSingleDataTransfer;
        } else if(
            l_isHalfwordSignedDataTransfer
            && !l_isHalfwordSignedDataTransferSwap
        ) {
            l_result = cpuOpcodeArmHalfwordSignedDataTransfer;
        } else if(l_isBlockDataTransfer) {
            l_result = cpuOpcodeArmLdmStm;
        } else if(l_isSwap) {
            l_result = cpuOpcodeArmSwp;
        } else if(l_isSwi) {
            l_result = cpuOpcodeArmSwi;
        }

        s_cpuDecodeTable.arm[l_index] = l_result;
    }
}

static void cpuInitThumbDecodeTable(void) {
    for(int l_index = 0; l_index < 1024; l_index++) {
        s_cpuDecodeTable.thumb[l_index] = cpuOpcodeThumbUnd;
    }
}
