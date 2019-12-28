#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/decoder/arm.hpp>

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
        &r8_15_usr[7],  &r8_15_usr[7],  &r8_15_usr[7],  &r8_15_usr[7],  &r8_15_usr[7],  &r8_15_usr[7],  &r8_15_usr[7],
    };

    void init() {
        // Reset general-purpose registers
        for(int i = 0; i < 7; i++) {
            for(int j = 0; j < 16; j++) {
                *registerMapping[i * 16 + j] = 0;
            }
        }

        // Reset CPSR
        cpsr.value = 0;
        cpsr.fields.mode = PSR_MODE_SVC;
        cpsr.fields.flagF = 1;
        cpsr.fields.flagI = 1;
        cpsr.fields.flagT = 0;
    }

    uint32_t registerRead(int reg) {
        return *registerMapping[reg * 7 + modeMapping[cpsr.fields.mode]];
    }

    void registerWrite(int reg, uint32_t value) {
        *registerMapping[reg * 7 + modeMapping[cpsr.fields.mode]] = value;
    }

    static inline void execute() {
        if(pipeline.pipelineStage == PIPELINE_FETCH_DECODE_EXECUTE) {
            pipeline.decodedOpcode.function(pipeline.decodedOpcode.opcode);
        }
    }

    static inline void decode() {
        if(pipeline.pipelineStage >= PIPELINE_FETCH_DECODE) {
            switch(cpsr.fields.flagT) {
                case CPU_MODE_ARM:
                    pipeline.decodedOpcode = gbaemu::gba::cpu::decoder::arm::decode(pipeline.fetchedOpcodeARM);
                    break;

                case CPU_MODE_THUMB:

                    break;
            }
        }
    }

    static inline void fetch() {
        switch(cpsr.fields.flagT) {
            case CPU_MODE_ARM:
            pipeline.fetchedOpcodeARM = 0;
            break;

            case CPU_MODE_THUMB:
            pipeline.fetchedOpcodeThumb = 0;
            break;
        }
    }

    void cycle() {
        execute();
        decode();
        fetch();

        switch(pipeline.pipelineStage) {
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

    void resetPipeline() {
        pipeline.pipelineStage = PIPELINE_FETCH;
    }

    psr_t readCPSR() {
        return cpsr;
    }

    void writeCPSR(psr_t psr) {
        cpsr = psr;
    }
}
