#pragma once

#include <cstdint>

#define DECLARE_ALU_OPCODE(name) \
    void opcode_ ## name(uint16_t opcode)

namespace gbaemu::gba::cpu::impl::thumb::alu {
    DECLARE_ALU_OPCODE(and);
    DECLARE_ALU_OPCODE(eor);
    DECLARE_ALU_OPCODE(lsl);
    DECLARE_ALU_OPCODE(lsr);
    DECLARE_ALU_OPCODE(asr);
    DECLARE_ALU_OPCODE(adc);
    DECLARE_ALU_OPCODE(sbc);
    DECLARE_ALU_OPCODE(ror);
    DECLARE_ALU_OPCODE(tst);
    DECLARE_ALU_OPCODE(neg);
    DECLARE_ALU_OPCODE(cmp);
    DECLARE_ALU_OPCODE(cmn);
    DECLARE_ALU_OPCODE(orr);
    DECLARE_ALU_OPCODE(mul);
    DECLARE_ALU_OPCODE(bic);
    DECLARE_ALU_OPCODE(mvn);
}
