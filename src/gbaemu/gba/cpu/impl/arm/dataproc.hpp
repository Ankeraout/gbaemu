#pragma once

#include <cstdint>

#define DECLARE_DATAPROC_OPCODE_SINGLE(name, suffix) \
    void opcode_ ## name ## _ ## suffix (uint32_t opcode)

#define DECLARE_DATAPROC_OPCODE(name) \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSL_Rm_0); \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSL_Rm_1); \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSR_Rm_0); \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, LSR_Rm_1); \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ASR_Rm_0); \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ASR_Rm_1); \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ROR_Rm_0); \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, ROR_Rm_1); \
    DECLARE_DATAPROC_OPCODE_SINGLE(name, Immediate)

#define DECLARE_DATAPROC_OPCODE_S(name) \
    DECLARE_DATAPROC_OPCODE(name); \
    DECLARE_DATAPROC_OPCODE(name ## s)

namespace gbaemu::gba::cpu::impl::arm::dataproc {
    DECLARE_DATAPROC_OPCODE_S(and);
    DECLARE_DATAPROC_OPCODE_S(eor);
    DECLARE_DATAPROC_OPCODE_S(sub);
    DECLARE_DATAPROC_OPCODE_S(rsb);
    DECLARE_DATAPROC_OPCODE_S(add);
    DECLARE_DATAPROC_OPCODE_S(adc);
    DECLARE_DATAPROC_OPCODE_S(sbc);
    DECLARE_DATAPROC_OPCODE_S(rsc);
    DECLARE_DATAPROC_OPCODE(tst);
    DECLARE_DATAPROC_OPCODE(teq);
    DECLARE_DATAPROC_OPCODE(cmp);
    DECLARE_DATAPROC_OPCODE(cmn);
    DECLARE_DATAPROC_OPCODE_S(orr);
    DECLARE_DATAPROC_OPCODE_S(mov);
    DECLARE_DATAPROC_OPCODE_S(bic);
    DECLARE_DATAPROC_OPCODE_S(mvn);
}

#undef DECLARE_DATAPROC_OPCODE_S
#undef DECLARE_DATAPROC_OPCODE
#undef DECLARE_DATAPROC_OPCODE_SINGLE