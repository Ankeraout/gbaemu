#ifndef __INCLUDE_CORE_CPU_DECODER_H__
#define __INCLUDE_CORE_CPU_DECODER_H__

#include <stdint.h>

union tu_cpuDecodedOpcode {
    struct {
        void (*handler)(uint32_t p_opcode);
        uint32_t opcode;
    } arm;

    struct {
        void (*handler)(uint16_t p_opcode);
        uint16_t opcode;
    } thumb;
};

void cpuDecoderInit(void);
void cpuDecode(void);

#endif
