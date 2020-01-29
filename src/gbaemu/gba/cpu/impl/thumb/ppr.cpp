#include <cstdint>
#include <cstdio>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/impl/logic_inline.hpp>
#include <gbaemu/gba/cpu/impl/thumb/ppr.hpp>
#include <gbaemu/gba/mmu.hpp>

namespace gbaemu::gba::cpu::impl::thumb::ppr {
    static inline unsigned int hammingWeight8(unsigned int v) {
        unsigned int hammingWeight = 0;

        for(unsigned int i = 0; i < 8; i++) {
            if(v & 0x01) {
                hammingWeight++;
            }

            v >>= 1;
        }

        return hammingWeight;
    }

    void opcode_push(uint16_t opcode) {
        uint32_t sp = registerRead(CPU_REG_SP);
        uint32_t Rlist = opcode & 0x00ff;
        unsigned int Rcount = (Rlist == 0) ? 16 : hammingWeight8(Rlist);

        sp -= (Rcount << 2);
        uint32_t accessAddress = sp;
        
        if(Rlist == 0) {
            mmu::write32(accessAddress, registerRead(CPU_REG_PC) + 2);
        } else {
            for(int i = 0; i < 8; i++) {
                if(Rlist & 0x0001) {
                    mmu::write32(accessAddress, registerRead(i));
                    accessAddress += 4;
                } Rlist >>= 1;
            }
        }
        
        registerWrite(CPU_REG_SP, sp);
    }
    
    void opcode_push_lr(uint16_t opcode) {
        uint32_t sp = registerRead(CPU_REG_SP);
        uint32_t Rlist = opcode & 0x00ff;
        unsigned int Rcount = hammingWeight8(Rlist) + 1;

        Rcount++; sp -= (Rcount << 2);
        uint32_t accessAddress = sp;

        for(int i = 0; i < 8; i++) {
            if(Rlist & 0x0001) {
                mmu::write32(accessAddress, registerRead(i));
                accessAddress += 4;
            } Rlist >>= 1;
        }
        
        mmu::write32(accessAddress, registerRead(CPU_REG_LR));
        registerWrite(CPU_REG_SP, sp);
    }

    void opcode_pop(uint16_t opcode) {
        uint32_t sp = registerRead(CPU_REG_SP);
        uint32_t Rlist = opcode & 0x00ff;
        unsigned int Rcount = (Rlist == 0) ? 16 : hammingWeight8(Rlist);
        uint32_t accessAddress = sp;
        sp += (Rcount << 2);

        if(Rlist == 0) {
            performJump(mmu::read32(accessAddress));
        } else {
            for(int i = 0; i < 8; i++) {
                if(Rlist & 0x0001) {
                    registerWrite(i, mmu::read32(accessAddress));
                    accessAddress += 4;
                } Rlist >>= 1;
            }
        }
        
        registerWrite(CPU_REG_SP, sp);
    }

    void opcode_pop_pc(uint16_t opcode) {
        uint32_t sp = registerRead(CPU_REG_SP);
        uint32_t Rlist = opcode & 0x00ff;

        for(int i = 0; i < 8; i++) {
            if(Rlist & 0x0001) {
                registerWrite(i, mmu::read32(sp));
                sp += 4;
            } Rlist >>= 1;
        }
        
        performJump(mmu::read32(sp));

        sp += 4;
        
        registerWrite(CPU_REG_SP, sp);
    }

}
