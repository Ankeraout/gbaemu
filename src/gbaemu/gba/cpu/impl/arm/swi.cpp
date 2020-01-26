#include <cstdint>

#include <gbaemu/gbaemu.hpp>
#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/arm/swi.hpp>

namespace gbaemu::gba::cpu::impl::arm::swi {
    void swi(uint32_t opcode) {
        UNUSED(opcode);

        writeSPSR(cpsr, PSR_MODE_SVC);
        cpsr.fields.mode = PSR_MODE_SVC;

        registerWrite(CPU_REG_LR, registerRead(CPU_REG_PC) - 4);
        performJump(0x08);
    }
}
