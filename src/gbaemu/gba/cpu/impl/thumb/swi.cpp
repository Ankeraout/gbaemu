#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/swi.hpp>

namespace gbaemu::gba::cpu::impl::thumb::swi {
    void opcode_swi(uint16_t opcode) {
        registerWrite(CPU_REG_LR, registerRead(CPU_REG_PC) - 2);

        if((cpsr.fields.mode != PSR_MODE_SYS) && (cpsr.fields.mode != PSR_MODE_USR)) {
            writeSPSR(cpsr);
        }
        
        performJump(0x08);
        cpsr.fields.mode = PSR_MODE_SVC;
        cpsr.fields.flagT = 0;
    }
}