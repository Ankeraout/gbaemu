#pragma once

#include <cstdint>

#define DECLARE_OPCODE_CB(cond) void opcode_b ## cond(uint16_t opcode)

namespace gbaemu::gba::cpu::impl::thumb::cb {
    DECLARE_OPCODE_CB(eq);
    DECLARE_OPCODE_CB(ne);
    DECLARE_OPCODE_CB(cs);
    DECLARE_OPCODE_CB(cc);
    DECLARE_OPCODE_CB(mi);
    DECLARE_OPCODE_CB(pl);
    DECLARE_OPCODE_CB(vs);
    DECLARE_OPCODE_CB(vc);
    DECLARE_OPCODE_CB(hi);
    DECLARE_OPCODE_CB(ls);
    DECLARE_OPCODE_CB(ge);
    DECLARE_OPCODE_CB(lt);
    DECLARE_OPCODE_CB(gt);
    DECLARE_OPCODE_CB(le);
}
