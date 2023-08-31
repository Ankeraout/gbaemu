#ifndef __INCLUDE_CORE_CPU_OP_ARM_DATAPROCESSING_H__
#define __INCLUDE_CORE_CPU_OP_ARM_DATAPROCESSING_H__

#include <stdint.h>

void cpuOpcodeArmAnd(uint32_t p_opcode);
void cpuOpcodeArmEor(uint32_t p_opcode);
void cpuOpcodeArmSub(uint32_t p_opcode);
void cpuOpcodeArmRsb(uint32_t p_opcode);
void cpuOpcodeArmAdd(uint32_t p_opcode);
void cpuOpcodeArmAdc(uint32_t p_opcode);
void cpuOpcodeArmSbc(uint32_t p_opcode);
void cpuOpcodeArmRsc(uint32_t p_opcode);
void cpuOpcodeArmTst(uint32_t p_opcode);
void cpuOpcodeArmTeq(uint32_t p_opcode);
void cpuOpcodeArmCmp(uint32_t p_opcode);
void cpuOpcodeArmCmn(uint32_t p_opcode);
void cpuOpcodeArmOrr(uint32_t p_opcode);
void cpuOpcodeArmMov(uint32_t p_opcode);
void cpuOpcodeArmBic(uint32_t p_opcode);
void cpuOpcodeArmMvn(uint32_t p_opcode);

#endif
