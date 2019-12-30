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

namespace gbaemu::gba::cpu::impl::dataproc {
    DECLARE_DATAPROC_OPCODE(cmp);
}

#undef DECLARE_DATAPROC_OPCODE
#undef DECLARE_DATAPROC_OPCODE_SINGLE
