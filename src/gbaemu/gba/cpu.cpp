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
    uint32_t r0_8[8];
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
        &r0_8[0],       &r0_8[0],       &r0_8[0],       &r0_8[0],       &r0_8[0],       &r0_8[0],       &r0_8[0],
        &r0_8[1],       &r0_8[1],       &r0_8[1],       &r0_8[1],       &r0_8[1],       &r0_8[1],       &r0_8[1],
        &r0_8[2],       &r0_8[2],       &r0_8[2],       &r0_8[2],       &r0_8[2],       &r0_8[2],       &r0_8[2],
        &r0_8[3],       &r0_8[3],       &r0_8[3],       &r0_8[3],       &r0_8[3],       &r0_8[3],       &r0_8[3],
        &r0_8[4],       &r0_8[4],       &r0_8[4],       &r0_8[4],       &r0_8[4],       &r0_8[4],       &r0_8[4],
        &r0_8[5],       &r0_8[5],       &r0_8[5],       &r0_8[5],       &r0_8[5],       &r0_8[5],       &r0_8[5],
        &r0_8[6],       &r0_8[6],       &r0_8[6],       &r0_8[6],       &r0_8[6],       &r0_8[6],       &r0_8[6],
        &r0_8[7],       &r0_8[7],       &r0_8[7],       &r0_8[7],       &r0_8[7],       &r0_8[7],       &r0_8[7],
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

    void init() {
        // Reset CPSR
        cpsr.value = 0;
        cpsr.fields.mode = PSR_MODE_SVC;
        cpsr.fields.flagF = 1;
        cpsr.fields.flagI = 1;
        cpsr.fields.flagT = 0;

        // Reset general-purpose registers
        for(int i = 0; i < 7; i++) {
            for(int j = 0; j < 16; j++) {
                *registerMapping[i * 16 + j] = 0;
            }
        }

        registerWrite(0, 0x00000ca5);

        for(int mode = 0; mode < 7; mode++) {
            registerWrite(13, mode, 0x03007f00);
        }
        
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
                    //printf("E [%08x] %08x\n", PC - 8, pipeline.decodedOpcodeARM_value);
                    if(checkCondition(pipeline.decodedOpcodeARM_value)) {
                        if(!pipeline.decodedOpcodeARM) {
                            printf("Warning: undefined opcode %08x at %08x\n", pipeline.decodedOpcodeARM_value, PC - 8);
                            raiseUnd();
                        } else {
                            pipeline.decodedOpcodeARM(pipeline.decodedOpcodeARM_value);
                        }
                    }

                    break;

                case CPU_MODE_THUMB:
                    //printf("E [%08x] %04x\n", PC - 4, pipeline.decodedOpcodeThumb_value);
                    if(!pipeline.decodedOpcodeThumb) {
                        printf("Warning: undefined opcode %04x at %08x\n", pipeline.decodedOpcodeARM_value, PC - 4);
                        raiseUnd();
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
        if((io::get(io::IME) & 0x00000001) && (io::get(io::IF) & io::get(io::IE) & 0x3fff)) {
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
            return !cpsr.fields.flagC && cpsr.fields.flagZ;
            
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

        fflush(stdout);
    }

    psr_t readSPSR() {
        if((cpsr.fields.mode == PSR_MODE_USR) || (cpsr.fields.mode == PSR_MODE_SYS)) {
            fprintf(stderr, "Attempted to read SPSR in user or system mode.\n");
            exit(1);
        }

        return spsr[modeMapping[cpsr.fields.mode] - 1];
    }

    void writeSPSR(psr_t value) {
        if((cpsr.fields.mode == PSR_MODE_USR) || (cpsr.fields.mode == PSR_MODE_SYS)) {
            fprintf(stderr, "Attempted to write SPSR in user or system mode.\n");
            exit(1);
        }

        spsr[modeMapping[cpsr.fields.mode] - 1] = value;
    }

    void writeSPSR(uint32_t value) {
        if((cpsr.fields.mode == PSR_MODE_USR) || (cpsr.fields.mode == PSR_MODE_SYS)) {
            fprintf(stderr, "Attempted to write SPSR in user or system mode.\n");
            exit(1);
        }

        spsr[modeMapping[cpsr.fields.mode] - 1].value = value;
    }

    void registerWrite(int reg, int mode, uint32_t value) {
        *registerMapping[reg * 7 + modeMapping[mode]] = value;
    }

    uint32_t registerRead(int reg, int mode) {
        return *registerMapping[reg * 7 + modeMapping[mode]];
    }

    void writeSPSR(psr_t value, int mode) {
        if((mode == MODE_USR) || (mode == MODE_SYS)) {
            fprintf(stderr, "Attempted to write user or system SPSR.\n");
            exit(1);
        }

        spsr[modeMapping[mode] - 1] = value;
    }

    void writeSPSR(uint32_t value, int mode) {
        if((mode == MODE_USR) || (mode == MODE_SYS)) {
            fprintf(stderr, "Attempted to write user or system SPSR.\n");
            exit(1);
        }

        spsr[modeMapping[mode] - 1].value = value;
    }

    void raiseUnd() {
        writeSPSR(cpsr, PSR_MODE_UND);
        cpsr.fields.mode = PSR_MODE_UND;
        registerWrite(CPU_REG_LR, registerRead(CPU_REG_PC) - (cpsr.fields.flagT ? 2 : 4));
        cpsr.fields.flagT = 0;
        cpsr.fields.flagI = 1;
        registerWrite(CPU_REG_PC, 0x00000004);
    }

    void raiseIRQ() {
        writeSPSR(cpsr, PSR_MODE_IRQ);
        cpsr.fields.mode = PSR_MODE_IRQ;
        registerWrite(CPU_REG_LR, registerRead(CPU_REG_PC) - (cpsr.fields.flagT ? 2 : 4));
        cpsr.fields.flagT = 0;
        cpsr.fields.flagI = 1;
        registerWrite(CPU_REG_PC, 0x00000018);
    }

    void if_writeCallback(uint16_t value) {
        io::set(io::IF, io::get(io::IF) & ~value);
    }
}
