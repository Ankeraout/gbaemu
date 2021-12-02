#ifndef __GBA_CPU_H__
#define __GBA_CPU_H__

#include <stdbool.h>
#include <stdint.h>

#include "core/gba.h"

/**
 * @brief This enumeration represents the different possible CPU modes and
 *        their number in CPSR.
 */
enum gba_cpu_mode_e {
    GBA_CPU_MODE_USR_OLD = 0x00, ///< This value represents the old USR mode.
    GBA_CPU_MODE_FIQ_OLD = 0x01, ///< This value represents the old FIQ mode.
    GBA_CPU_MODE_IRQ_OLD = 0x02, ///< This value represents the old IRQ mode.
    GBA_CPU_MODE_SVC_OLD = 0x03, ///< This value represents the old SVC mode.
    GBA_CPU_MODE_USR = 0x10, ///< This value represents the USR mode.
    GBA_CPU_MODE_FIQ = 0x11, ///< This value represents the FIQ mode.
    GBA_CPU_MODE_IRQ = 0x12, ///< This value represents the IRQ mode.
    GBA_CPU_MODE_SVC = 0x13, ///< This value represents the SVC mode.
    GBA_CPU_MODE_ABT = 0x17, ///< This value represents the ABT mode.
    GBA_CPU_MODE_UND = 0x1b, ///< This value represents the UND mode.
    GBA_CPU_MODE_SYS = 0x1f ///< This value represents the SYS mode.
};

/**
 * @brief This enumeration represents the status of the pipeline of the CPU
 *        during one cycle.
 */
enum gba_cpu_pipelineState_e {
    GBA_CPU_PIPELINESTATE_EMPTY, /**< This value indicates that the pipeline is
                                      empty. This state occurs when the CPU is
                                      reset or when the last executed
                                      instruction changed the value of R15. */
    GBA_CPU_PIPELINESTATE_FETCH, /**< This value indicates that the CPU has
                                      fetched an opcode from the bus. */
    GBA_CPU_PIPELINESTATE_DECODE, /**< This value indicates that the CPU has
                                       fetched an opcode from the bus and that
                                       one opcode was decoded. */
    GBA_CPU_PIPELINESTATE_EXECUTE, /**< This value indicates that the CPU has
                                        fetched an opcode from the bus, that
                                        one opcode was decoded, and that one
                                        opcode was executed. */
};

/**
 * This enumeration represents the different opcode condition values.
 */
enum gba_cpu_condition_e {
    GBA_CPU_CONDITION_EQ, ///< This condition represents the equality or zero (Z set).
    GBA_CPU_CONDITION_NE, ///< This condition represents non-equality or non-zero (Z cleared).
    GBA_CPU_CONDITION_CS, ///< This condition is true when the carry flag is set.
    GBA_CPU_CONDITION_HS = GBA_CPU_CONDITION_CS, ///< This condition is true when the number was higher or same (unsigned arithmetic) (C set).
    GBA_CPU_CONDITION_CC, ///< This condition is true when the carry flag is cleared.
    GBA_CPU_CONDITION_LO = GBA_CPU_CONDITION_CC, ///< This condition is true when the number was lower (unsigned arithmetic) (C cleared).
    GBA_CPU_CONDITION_MI, ///< This condition is true when the number was negative (N set).
    GBA_CPU_CONDITION_PL, ///< This condition is true when the number was positive or null (N cleared).
    GBA_CPU_CONDITION_VS, ///< This condition is true when an overflow occurred (V set).
    GBA_CPU_CONDITION_VC, ///< This condition is true when an overflow did not occur (V cleared).
    GBA_CPU_CONDITION_HI, ///< This condition is true when the number was higher (unsigned arithmetic) (!C || Z).
    GBA_CPU_CONDITION_LS, ///< This condition is true when the number was lower or same (unsigned arithmetic) (C && !Z).
    GBA_CPU_CONDITION_GE, ///< This condition is true when the number was greater (signed arithmetic) (N == V).
    GBA_CPU_CONDITION_LT, ///< This condition is true when the number was less (signed arithmetic) (N != V).
    GBA_CPU_CONDITION_GT, ///< This condition is true when the number was greater (signed arithmetic) (!Z && (N == V)).
    GBA_CPU_CONDITION_LE, ///< This condition is true when the number was less or equal (signed arithmetic) (Z || (N != V)).
    GBA_CPU_CONDITION_AL, ///< This condition is always true.
    GBA_CPU_CONDITION_NV ///< This condition is always false (unimplemented?)
};

/**
 * @brief This type represents an opcode handler for ARM instructions.
 */
typedef void gba_cpu_opcodeHandlerArm_t(uint32_t opcode);

/**
 * @brief This type represents an opcode handler for Thumb instructions.
 */
typedef void gba_cpu_opcodeHandlerThumb_t(uint16_t opcode);

/**
 * @brief This structure represents the CPU pipeline.
 */
struct gba_cpu_pipeline_s {
    enum gba_cpu_pipelineState_e state; ///< The state of the pipeline.

    /**
     * @brief This union represents the data stored in the different stages of
     *        the pipeline.
     */
    union {
        /**
         * @brief This structure represents the data stored in the different
         *        stages of the pipeline when the CPU is in ARM execution mode.
         */
        struct {
            uint32_t fetchedOpcode; ///< The fetched opcode.
            uint32_t decodedOpcode_value; ///< The value of the decoded opcode.
            gba_cpu_opcodeHandlerArm_t *decodedOpcode_handler; ///< The handler of the decoded opcode.
        } arm;

        /**
         * @brief This structure represents the data stored in the different
         *        stages of the pipeline when the CPU is in Thumb execution
         *        mode.
         */
        struct {
            uint16_t fetchedOpcode; ///< The fetched opcode.
            uint16_t decodedOpcode_value; ///< The value of the decoded opcode.
            gba_cpu_opcodeHandlerThumb_t *decodedOpcode_handler; ///< The handler of the decoded opcode.
        } thumb;
    } opcode;
};

/**
 * @brief This structure represents the flags of the CPU.
 */
struct gba_cpu_flags_s {
    bool n; ///< The negative flag. This flag is set when the result is negative (31st bit is set).
    bool z; ///< The zero flag. This flag is set when the result is null.
    bool c; ///< The carry flag. This flag is set when the operation left a carry.
    bool v; ///< The overflow flag. This flag is set when the operation overflowed.
    bool i; ///< The IRQ enable flag. If this flag is set, IRQs can occur.
    bool f; ///< The FIQ enable flag. If this flag is set, FIQs can occur.
    bool t; ///< The Thumb execution mode flag. If this flag is set, the CPU will fetch and decode Thumb opcodes.
};

/**
 * @brief This structure represents the registers of the CPU.
 */
struct gba_cpu_registers_s {
    uint32_t r[16]; ///< This array represents the 16 general-purpose registers of the CPU.
    uint32_t r_usr[7]; ///< This array contains the saved R8-14 registers in user mode.
    uint32_t r_irq[2]; ///< This array contains the saved R13-R14 registers in IRQ mode.
    uint32_t spsr_irq; ///< This value contains the SPSR register in IRQ mode.
    uint32_t r_fiq[7]; ///< This array contains the saved R8-R14 registers in FIQ mode.
    uint32_t spsr_fiq; ///< This value contains the SPSR register in FIQ mode.
    uint32_t r_svc[2]; ///< This array contains the saved R13-R14 registers in SVC mode.
    uint32_t spsr_svc; ///< This value contains the SPSR register in SVC mode.
    uint32_t r_abt[2]; ///< This array contains the saved R13-R14 registers in ABT mode.
    uint32_t spsr_abt; ///< This value contains the SPSR register in ABT mode.
    uint32_t r_und[2]; ///< This array contains the saved R13-R14 registers in UND mode.
    uint32_t spsr_und; ///< This value contains the SPSR register in UND mode.
};

/**
 * @brief This structure represents the CPU shifter.
 */
struct gba_cpu_shifter_s {
    uint32_t result; ///< This value contains the result of the shifter.
    bool carry; ///< This value contains the carry flag of the shifter.
};

/**
 * @brief This structure represents the GameBoy Advance CPU.
 */
struct gba_cpu_s {
    enum gba_cpu_mode_e mode; ///< This value represents the current execution mode.
    struct gba_cpu_pipeline_s pipeline; ///< This structure represents the pipeline.
    struct gba_cpu_flags_s flags; ///< This structure contains the flags.
    struct gba_cpu_shifter_s shifter; ///< This structure represents the shifter.
};

/**
 * @brief Initializes the GameBoy Advance CPU.
 * @details This function will initialize the functional fields of the CPU
 *          structure.
 */
void gba_cpu_init();

/**
 * @brief Resets the GameBoy Advance CPU.
 * @details This function will reset the CPU registers to their initial value.
 *
 * @param[in] skipBoot A boolean value that indicates whether the boot intro
 *                     shall be skipped.
 */
void gba_cpu_reset(bool skipBoot);

/**
 * @brief Performs one CPU step.
 * @details A CPU step is done when the decoded instruction in the pipeline is
 *          completely executed. The execution of a single instruction can take
 *          more than one CPU cycle.
 */
void gba_cpu_step();

#endif
