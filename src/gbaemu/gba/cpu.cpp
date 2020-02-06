#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/io.hpp>
#include <gbaemu/gba/mmu.hpp>
#include <gbaemu/gba/cpu/decoder/arm.hpp>
#include <gbaemu/gba/cpu/decoder/thumb.hpp>

#define PC r8_15_usr[7]

namespace gbaemu::gba::cpu {
    uint32_t r0_7[8];
    uint32_t r8_15_usr[8];
    uint32_t r8_14_fiq[7];
    uint32_t r13_14_svc[2];
    uint32_t r13_14_abt[2];
    uint32_t r13_14_irq[2];
    uint32_t r13_14_und[2];
    psr_t cpsr;
    psr_t spsr[5];
    pipeline_t pipeline;
    shifter_t shifter;
    
    static inline const int modeMapping[] = {
        MODE_INV, MODE_INV, MODE_INV, MODE_INV,
        MODE_INV, MODE_INV, MODE_INV, MODE_INV,
        MODE_INV, MODE_INV, MODE_INV, MODE_INV,
        MODE_INV, MODE_INV, MODE_INV, MODE_INV,
        MODE_USR, MODE_FIQ, MODE_IRQ, MODE_SVC,
        MODE_INV, MODE_INV, MODE_INV, MODE_ABT,
        MODE_INV, MODE_INV, MODE_INV, MODE_UND,
        MODE_INV, MODE_INV, MODE_INV, MODE_SYS,
    };

    static inline uint32_t *const registerMapping[] = {
        &r0_7[0],       &r0_7[0],       &r0_7[0],       &r0_7[0],       &r0_7[0],       &r0_7[0],       &r0_7[0],
        &r0_7[1],       &r0_7[1],       &r0_7[1],       &r0_7[1],       &r0_7[1],       &r0_7[1],       &r0_7[1],
        &r0_7[2],       &r0_7[2],       &r0_7[2],       &r0_7[2],       &r0_7[2],       &r0_7[2],       &r0_7[2],
        &r0_7[3],       &r0_7[3],       &r0_7[3],       &r0_7[3],       &r0_7[3],       &r0_7[3],       &r0_7[3],
        &r0_7[4],       &r0_7[4],       &r0_7[4],       &r0_7[4],       &r0_7[4],       &r0_7[4],       &r0_7[4],
        &r0_7[5],       &r0_7[5],       &r0_7[5],       &r0_7[5],       &r0_7[5],       &r0_7[5],       &r0_7[5],
        &r0_7[6],       &r0_7[6],       &r0_7[6],       &r0_7[6],       &r0_7[6],       &r0_7[6],       &r0_7[6],
        &r0_7[7],       &r0_7[7],       &r0_7[7],       &r0_7[7],       &r0_7[7],       &r0_7[7],       &r0_7[7],
        &r8_15_usr[0],  &r8_14_fiq[0],  &r8_15_usr[0],  &r8_15_usr[0],  &r8_15_usr[0],  &r8_15_usr[0],  &r8_15_usr[0],
        &r8_15_usr[1],  &r8_14_fiq[1],  &r8_15_usr[1],  &r8_15_usr[1],  &r8_15_usr[1],  &r8_15_usr[1],  &r8_15_usr[1],
        &r8_15_usr[2],  &r8_14_fiq[2],  &r8_15_usr[2],  &r8_15_usr[2],  &r8_15_usr[2],  &r8_15_usr[2],  &r8_15_usr[2],
        &r8_15_usr[3],  &r8_14_fiq[3],  &r8_15_usr[3],  &r8_15_usr[3],  &r8_15_usr[3],  &r8_15_usr[3],  &r8_15_usr[3],
        &r8_15_usr[4],  &r8_14_fiq[4],  &r8_15_usr[4],  &r8_15_usr[4],  &r8_15_usr[4],  &r8_15_usr[4],  &r8_15_usr[4],
        &r8_15_usr[5],  &r8_14_fiq[5],  &r13_14_svc[0], &r13_14_abt[0], &r13_14_irq[0], &r13_14_und[0], &r8_15_usr[5],
        &r8_15_usr[6],  &r8_14_fiq[6],  &r13_14_svc[1], &r13_14_abt[1], &r13_14_irq[1], &r13_14_und[1], &r8_15_usr[6],
        &r8_15_usr[7],  &r8_15_usr[7],  &r8_15_usr[7],  &r8_15_usr[7],  &r8_15_usr[7],  &r8_15_usr[7],  &r8_15_usr[7]
    };

    void raiseIRQ();

    void setInitialRegisterValues() {
        // Reset general-purpose registers
        for(int i = 0; i < 7; i++) {
            for(int j = 0; j < 16; j++) {
                *registerMapping[i * 16 + j] = 0;
            }
        }

        // USR/SYS
        registerWrite(13, PSR_MODE_USR, 0x03007f00);
        registerWrite(14, PSR_MODE_USR, 0x08000000);

        // FIQ
        registerWrite(8, PSR_MODE_FIQ, 0x40988194);
        registerWrite(9, PSR_MODE_FIQ, 0x04100084);
        registerWrite(10, PSR_MODE_FIQ, 0x808c1042);
        registerWrite(11, PSR_MODE_FIQ, 0x16a0439b);
        registerWrite(12, PSR_MODE_FIQ, 0x44820443);
        registerWrite(13, PSR_MODE_FIQ, 0x00410c81);
        registerWrite(14, PSR_MODE_FIQ, 0xa928314e);

        // SVC
        registerWrite(13, PSR_MODE_SVC, 0x03007fe0);

        // ABT
        registerWrite(13, PSR_MODE_ABT, 0x04014520);
        registerWrite(14, PSR_MODE_FIQ, 0x0b3478a4);

        // IRQ
        registerWrite(13, PSR_MODE_IRQ, 0x03007fa0);

        // UND
        registerWrite(13, PSR_MODE_FIQ, 0x490a068c);
        registerWrite(14, PSR_MODE_FIQ, 0x41400407);
    }

    void init() {
        // Reset CPSR
        cpsr.value = 0;
        cpsr.fields.mode = PSR_MODE_SYS;

        setInitialRegisterValues();
        
        // Reset pipeline
        pipeline.pipelineStage = PIPELINE_FETCH;
    }

    uint32_t registerRead(int reg) {
        return *registerMapping[reg * 7 + modeMapping[cpsr.fields.mode]];
    }

    void registerWrite(int reg, uint32_t value) {
        if(reg == 15) {
            performJump(value);
        } else {
            *registerMapping[reg * 7 + modeMapping[cpsr.fields.mode]] = value;
        }
    }

    static inline void execute() {
        if(pipeline.pipelineStage == PIPELINE_FETCH_DECODE_EXECUTE) {
            switch(cpsr.fields.flagT) {
                case CPU_MODE_ARM:
                    //printf("[%08x] %08x R0=%08x R1=%08x R2=%08x R3=%08x R4=%08x R5=%08x R6=%08x R7=%08x R8=%08x R9=%08x R10=%08x R11=%08x R12=%08x SP=%08x LR=%08x CPSR=%08x\n", PC - 8, pipeline.decodedOpcodeARM_value, registerRead(0), registerRead(1), registerRead(2), registerRead(3), registerRead(4), registerRead(5), registerRead(6), registerRead(7), registerRead(8), registerRead(9), registerRead(10), registerRead(11), registerRead(12), registerRead(13), registerRead(14), cpsr.value);
                    if(checkCondition(pipeline.decodedOpcodeARM_value)) {
                        if(!pipeline.decodedOpcodeARM) {
                            printf("Warning: undefined opcode %08x at %08x\n", pipeline.decodedOpcodeARM_value, PC - 8);
                            raiseUnd();
                            gbaemu::quit();
                        } else {
                            pipeline.decodedOpcodeARM(pipeline.decodedOpcodeARM_value);
                        }
                    }

                    break;

                case CPU_MODE_THUMB:
                    //printf("[%08x] %04x     R0=%08x R1=%08x R2=%08x R3=%08x R4=%08x R5=%08x R6=%08x R7=%08x R8=%08x R9=%08x R10=%08x R11=%08x R12=%08x SP=%08x LR=%08x CPSR=%08x\n", PC - 4, pipeline.decodedOpcodeThumb_value, registerRead(0), registerRead(1), registerRead(2), registerRead(3), registerRead(4), registerRead(5), registerRead(6), registerRead(7), registerRead(8), registerRead(9), registerRead(10), registerRead(11), registerRead(12), registerRead(13), registerRead(14), cpsr.value);
                    if(!pipeline.decodedOpcodeThumb) {
                        printf("Warning: undefined opcode %04x at %08x\n", pipeline.decodedOpcodeThumb_value, PC - 4);
                        raiseUnd();
                        gbaemu::quit();
                    } else {
                        pipeline.decodedOpcodeThumb(pipeline.decodedOpcodeThumb_value);
                    }

                    break;
            }
        }
    }

    static inline void decode() {
        if(pipeline.pipelineStage >= PIPELINE_FETCH_DECODE) {
            switch(cpsr.fields.flagT) {
                case CPU_MODE_ARM:
                    pipeline.decodedOpcodeARM = gbaemu::gba::cpu::decoder::arm::decode(pipeline.fetchedOpcodeARM);
                    pipeline.decodedOpcodeARM_value = pipeline.fetchedOpcodeARM;
                    break;

                case CPU_MODE_THUMB:
                    pipeline.decodedOpcodeThumb = gbaemu::gba::cpu::decoder::thumb::decode(pipeline.fetchedOpcodeThumb);
                    pipeline.decodedOpcodeThumb_value = pipeline.fetchedOpcodeThumb;
                    break;
            }
        }
    }

    static inline void fetch(uint32_t fetchOffset) {
        switch(cpsr.fields.flagT) {
            case CPU_MODE_ARM:
            pipeline.fetchedOpcodeARM = mmu::read32(fetchOffset);
            //printf("F [%08x] %08x\n", fetchOffset, pipeline.fetchedOpcodeARM);
            break;

            case CPU_MODE_THUMB:
            pipeline.fetchedOpcodeThumb = mmu::read16(fetchOffset);
            //printf("F [%08x] %04x\n", fetchOffset, pipeline.fetchedOpcodeThumb);
            break;
        }

        PC += 4 >> cpsr.fields.flagT;
    }

    void cycle() {
        if(modeMapping[cpsr.fields.mode] == MODE_INV) {
            fprintf(stderr, "Error: PSR mode 0x%02x not allowed.\n", cpsr.fields.mode);
            exit(0);
        }

        if((io::get(io::IME) & 0x00000001) && (io::get(io::IF) & io::get(io::IE) & 0x3fff) && (!cpsr.fields.flagI)) {
            raiseIRQ();
        }

        uint32_t cycleFetchOffset = PC;

        execute();
        decode();
        fetch(cycleFetchOffset);

        switch(pipeline.pipelineStage) {
            case PIPELINE_FLUSH:
            pipeline.pipelineStage = PIPELINE_FETCH;
            break;

            case PIPELINE_FETCH:
            pipeline.pipelineStage = PIPELINE_FETCH_DECODE;
            break;

            default:
            pipeline.pipelineStage = PIPELINE_FETCH_DECODE_EXECUTE;
            break;
        }
    }

    bool checkCondition(uint32_t opcode) {
        switch((opcode >> 28) & 0x0f) {
            case CPU_CONDITION_EQ:
            return cpsr.fields.flagZ;

            case CPU_CONDITION_NE:
            return !cpsr.fields.flagZ;
            
            case CPU_CONDITION_CS:
            return cpsr.fields.flagC;
            
            case CPU_CONDITION_CC:
            return !cpsr.fields.flagC;
            
            case CPU_CONDITION_MI:
            return cpsr.fields.flagN;
            
            case CPU_CONDITION_PL:
            return !cpsr.fields.flagN;
            
            case CPU_CONDITION_VS:
            return cpsr.fields.flagV;
            
            case CPU_CONDITION_VC:
            return !cpsr.fields.flagV;
            
            case CPU_CONDITION_HI:
            return cpsr.fields.flagC && !cpsr.fields.flagZ;
            
            case CPU_CONDITION_LS:
            return !cpsr.fields.flagC || cpsr.fields.flagZ;
            
            case CPU_CONDITION_GE:
            return cpsr.fields.flagN == cpsr.fields.flagV;
            
            case CPU_CONDITION_LT:
            return cpsr.fields.flagN != cpsr.fields.flagV;
            
            case CPU_CONDITION_GT:
            return !cpsr.fields.flagZ && (cpsr.fields.flagN == cpsr.fields.flagV);
            
            case CPU_CONDITION_LE:
            return cpsr.fields.flagZ || (cpsr.fields.flagN != cpsr.fields.flagV);
            
            case CPU_CONDITION_AL:
            return true;
            
            default:
            return false;
        }
    }

    void performJump(uint32_t address) {
        address -= (cpsr.fields.flagT) ? 2 : 4;
        address &= (cpsr.fields.flagT) ? 0xfffffffe : 0xfffffffc;

        PC = address;
        pipeline.pipelineStage = PIPELINE_FLUSH;
    }

    void displayState() {
        printf("CPU state\n");
        printf("=========\n");
        printf("\n");
        printf("Pipeline state: %u\n", pipeline.pipelineStage);
        printf("CPSR: %08x\n", cpsr.value);
        printf("Mode: %02x\n", cpsr.fields.mode);
        
        for(int i = 0; i < 16; i++) {
            printf("R%d = %08x\n", i, registerRead(i));
        }

        printf("\nStack trace:\n");

        if((registerRead(CPU_REG_SP) & 0xfffffe00) != 0x03007e00) {
            printf("<too large to display>\n");
        } else {
            for(uint32_t base = 0x03007f00; base >= registerRead(CPU_REG_SP); base -= 4) {
                printf(" - %08x: %08x\n", base, mmu::read32(base));
            }
        }

        fflush(stdout);
    }

    psr_t readSPSR() {
        if((cpsr.fields.mode == PSR_MODE_USR) || (cpsr.fields.mode == PSR_MODE_SYS)) {
            return cpsr;
        }

        return spsr[modeMapping[cpsr.fields.mode] - 1];
    }

    void writeSPSR(psr_t value) {
        if((cpsr.fields.mode != PSR_MODE_USR) && (cpsr.fields.mode != PSR_MODE_SYS)) {
            spsr[modeMapping[cpsr.fields.mode] - 1] = value;
        }
    }

    void writeSPSR(uint32_t value) {
        if((cpsr.fields.mode != PSR_MODE_USR) && (cpsr.fields.mode != PSR_MODE_SYS)) {
            spsr[modeMapping[cpsr.fields.mode] - 1].value = value;
        }
    }

    void registerWrite(int reg, int mode, uint32_t value) {
        *registerMapping[reg * 7 + modeMapping[mode]] = value;
    }

    uint32_t registerRead(int reg, int mode) {
        return *registerMapping[reg * 7 + modeMapping[mode]];
    }

    void writeSPSR(psr_t value, int mode) {
        if((mode != MODE_USR) && (mode != MODE_SYS)) {
            spsr[modeMapping[mode] - 1] = value;
        }
    }

    void writeSPSR(uint32_t value, int mode) {
        if((mode != MODE_USR) && (mode != MODE_SYS)) {
            spsr[modeMapping[mode] - 1].value = value;
        }
    }

    void raiseUnd() {
        writeSPSR(cpsr, PSR_MODE_UND);
        cpsr.fields.mode = PSR_MODE_UND;
        registerWrite(CPU_REG_LR, registerRead(CPU_REG_PC) - (cpsr.fields.flagT ? 2 : 4));
        cpsr.fields.flagT = 0;
        cpsr.fields.flagI = 1;
        performJump(0x00000004);
    }

    void raiseIRQ() {
        writeSPSR(cpsr, PSR_MODE_IRQ);
        cpsr.fields.mode = PSR_MODE_IRQ;
        registerWrite(CPU_REG_LR, registerRead(CPU_REG_PC) - (cpsr.fields.flagT ? 0 : 4));
        cpsr.fields.flagT = 0;
        cpsr.fields.flagI = 1;
        performJump(0x00000018);
    }

    void raiseSWI() {
        writeSPSR(cpsr, PSR_MODE_SVC);
        cpsr.fields.mode = PSR_MODE_SVC;
        registerWrite(CPU_REG_LR, registerRead(CPU_REG_PC) - (cpsr.fields.flagT ? 2 : 4));
        cpsr.fields.flagT = 0;
        cpsr.fields.flagI = 1;
        performJump(0x00000008);
    }

    void if_writeCallback(uint16_t value) {
        io::set(io::IF, io::get(io::IF) & ~value);
    }

    void writeCPSR(uint32_t value) {
        uint32_t mask;

        if(cpsr.fields.mode == PSR_MODE_USR) {
            mask = 0xff000000;
        } else {
            mask = 0xffffffff;
        }

        cpsr.value &= ~mask;
        cpsr.value |= value & mask;
    }
}
