#include "core/cpu/cpu.h"
#include "core/cpu/op_arm_bx.h"
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
        s_cpuDecodeTable.arm[l_index] = cpuOpcodeArmUnd;
    }

    s_cpuDecodeTable.arm[0x121] = cpuOpcodeArmBx;
}

static void cpuInitThumbDecodeTable(void) {
    for(int l_index = 0; l_index < 1024; l_index++) {
        s_cpuDecodeTable.thumb[l_index] = cpuOpcodeThumbUnd;
    }
}
