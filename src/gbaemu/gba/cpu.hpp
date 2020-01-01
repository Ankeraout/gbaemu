#pragma once

#include <gbaemu/gba/cpu/decoder.hpp>

namespace gbaemu::gba::cpu {
    typedef union {
        uint32_t value;

        struct {
            uint32_t mode : 5;
            uint32_t flagT : 1;
            uint32_t flagF : 1;
            uint32_t flagI : 1;
            uint32_t reserved : 19;
            uint32_t flagQ : 1;
            uint32_t flagV : 1;
            uint32_t flagC : 1;
            uint32_t flagZ : 1;
            uint32_t flagN : 1;
        } __attribute__((packed)) fields;
    } psr_t;

    typedef enum {
        PSR_MODE_USR = 0b10000,
        PSR_MODE_FIQ = 0b10001,
        PSR_MODE_IRQ = 0b10010,
        PSR_MODE_SVC = 0b10011,
        PSR_MODE_ABT = 0b10111,
        PSR_MODE_UND = 0b11011,
        PSR_MODE_SYS = 0b11111,
    } mode_t;
    
    typedef struct {
        uint32_t opcode;
        gbaemu::gba::cpu::decoder::opcodeCallback_t function;
    } instruction_t;

    typedef enum {
        PIPELINE_FLUSH,
        PIPELINE_FETCH,
        PIPELINE_FETCH_DECODE,
        PIPELINE_FETCH_DECODE_EXECUTE
    } pipelineStage_t;

    typedef struct {
        uint32_t fetchedOpcodeARM;
        uint16_t fetchedOpcodeThumb;
        instruction_t decodedOpcode;
        pipelineStage_t pipelineStage;
    } pipeline_t;

    enum {
        MODE_USR,
        MODE_FIQ,
        MODE_SVC,
        MODE_ABT,
        MODE_IRQ,
        MODE_UND,
        MODE_SYS,
        MODE_INV
    };

    enum {
        CPU_MODE_ARM,
        CPU_MODE_THUMB
    };

    enum {
        CPU_CONDITION_EQ,
        CPU_CONDITION_NE,
        CPU_CONDITION_CS,
        CPU_CONDITION_CC,
        CPU_CONDITION_MI,
        CPU_CONDITION_PL,
        CPU_CONDITION_VS,
        CPU_CONDITION_VC,
        CPU_CONDITION_HI,
        CPU_CONDITION_LS,
        CPU_CONDITION_GE,
        CPU_CONDITION_LT,
        CPU_CONDITION_GT,
        CPU_CONDITION_LE,
        CPU_CONDITION_AL,
        CPU_CONDITION_NV,
    };

    enum {
        CPU_REG_R0,
        CPU_REG_R1,
        CPU_REG_R2,
        CPU_REG_R3,
        CPU_REG_R4,
        CPU_REG_R5,
        CPU_REG_R6,
        CPU_REG_R7,
        CPU_REG_R8,
        CPU_REG_R9,
        CPU_REG_R10,
        CPU_REG_R11,
        CPU_REG_R12,
        CPU_REG_SP,
        CPU_REG_LR,
        CPU_REG_PC,
    };
    
    typedef struct {
        bool flagC;
        uint32_t result;
    } shifter_t;
    
    extern psr_t cpsr;
    extern shifter_t shifter;

    extern void init();
    extern void cycle();
    extern bool checkCondition(uint32_t opcode);

    // Instruction API
    extern uint32_t registerRead(int reg);
    extern void registerWrite(int reg, uint32_t value);
    extern void performJump(uint32_t address);
    extern psr_t readSPSR();
    extern void writeSPSR(psr_t value);
    extern void writeSPSR(uint32_t value);
    extern void writeCPSR(psr_t value);
    extern void writeCPSR(uint32_t value);

    // Debug API
    extern void displayState();
}
