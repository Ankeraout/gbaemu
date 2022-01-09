//==============================================================================
// Included files
//==============================================================================
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "bitops.h"
#include "bus.h"
#include "common.h"
#include "cpu.h"

//==============================================================================
// Private constants
//==============================================================================
/**
 * @brief This constant defines the mask to apply to a memory address in order
 *        to align it to a 16-bit boundary.
 */
#define C_ADDRESS_MASK_16 0xfffffffe

/**
 * @brief This constant defines the mask to apply to a memory address in order
 *        to align it to a 32-bit boundary.
 */
#define C_ADDRESS_MASK_32 0xfffffffc

/**
 * @brief This constant defines the size of a Thumb opcode in bytes.
 */
#define C_INSTRUCTION_SIZE_THUMB 0x00000002

/**
 * @brief This constant defines the size of an ARM opcode in bytes.
 */
#define C_INSTRUCTION_SIZE_ARM 0x00000004

/**
 * @brief This constant defines the mask for the N (negative) flag in the PSR
 *        registers.
 */
#define C_PSR_MASK_FLAG_N 0x80000000

/**
 * @brief This constant defines the mask for the Z (zero) flag in the PSR
 *        registers.
 */
#define C_PSR_MASK_FLAG_Z 0x40000000

/**
 * @brief This constant defines the mask for the C (carry) flag in the PSR
 *        registers.
 */
#define C_PSR_MASK_FLAG_C 0x20000000

/**
 * @brief This constant defines the mask for the V (overflow) flag in the PSR
 *        registers.
 */
#define C_PSR_MASK_FLAG_V 0x10000000

/**
 * @brief This constant defines the mask for the I (interrupt) flag in the PSR
 *        registers.
 */
#define C_PSR_MASK_FLAG_I 0x00000080

/**
 * @brief This constant defines the mask for the F (FIQ, Fast Interrupt Request)
 *        flag in the PSR registers.
 */
#define C_PSR_MASK_FLAG_F 0x00000040

/**
 * @brief This constant defines the mask for the T (Thumb) flag in the PSR
 *        registers.
 */
#define C_PSR_MASK_FLAG_T 0x00000020

/**
 * @brief This constant defines the mask for the mode field in the PSR
 *        registers.
 */
#define C_PSR_MASK_MODE 0x0000001f

/**
 * @brief This constant defines the vector for the reset exception.
 */
#define C_EXCEPTION_VECTOR_RESET 0x00000000

/**
 * @brief This constant defines the vector for the undefined instruction
 *        exception.
 */
#define C_EXCEPTION_VECTOR_UND 0x00000004

/**
 * @brief This constant defines the vector for the software interrupt exception.
 */
#define C_EXCEPTION_VECTOR_SWI 0x00000008

/**
 * @brief This constant defines the vector for the hardware interrupt exception.
 */
#define C_EXCEPTION_VECTOR_IRQ 0x00000018

//==============================================================================
// Private types
//==============================================================================
/**
 * @brief This enumeration defines the different possible CPU modes.
 */
typedef enum {
    E_CPUMODE_OLD_USR = 0x00, ///< This constant designates the old USR mode.
    E_CPUMODE_OLD_FIQ = 0x01, ///< This constant designates the old FIQ mode.
    E_CPUMODE_OLD_IRQ = 0x02, ///< This constant designates the old IRQ mode.
    E_CPUMODE_OLD_SVC = 0x03, ///< This constant designates the old SVC mode.
    E_CPUMODE_USR = 0x10, ///< This constant designates the USR mode.
    E_CPUMODE_FIQ = 0x11, ///< This constant designates the FIQ mode.
    E_CPUMODE_IRQ = 0x12, ///< This constant designates the IRQ mode.
    E_CPUMODE_SVC = 0x13, ///< This constant designates the SVC mode.
    E_CPUMODE_ABT = 0x17, ///< This constant designates the ABT mode.
    E_CPUMODE_UND = 0x1b, ///< This constant designates the UND mode.
    E_CPUMODE_SYS = 0x1f ///< This constant designates the SYS mode.
} t_cpuMode;

/**
 * @brief This enumeration defines the different possible CPU pipeline states.
 */
typedef enum {
    E_CPUPIPELINESTATE_FLUSH, /**< This constant indicates that the CPU pipeline
                                   is currently empty. During the next cycle,
                                   the CPU will fetch an opcode from memory.*/
    E_CPUPIPELINESTATE_FETCH, /**< This constant indicates that the CPU pipeline
                                   currently contains one fetched instruction.
                                   During the next cycle, the CPU will fetch an
                                   opcode from memory and decode the previously
                                   fetched instruction. */
    E_CPUPIPELINESTATE_DECODE, /**< This constant indicates that the CPU
                                    pipeline currently contains one fetched and
                                    one decoded instructions. During the next
                                    cycle, the CPU will fetch an opcode from
                                    memory, decode the previously fetched
                                    instruction and execute the previously
                                    decoded instruction. */
    E_CPUPIPELINESTATE_EXECUTE /**< This constant indicates that the CPU
                                    pipeline is currently filled at all steps,
                                    and that the CPU is fetching, decoding and
                                    executing opcodes during the same cycle. */
} t_cpuPipelineState;

/**
 * @brief This enumeration defines the different possible ARM opcode execution
 *        conditions.
 */
typedef enum {
    E_CPUCONDITION_EQ, ///< This constant indicates the equality condition (Z).
    E_CPUCONDITION_NE, /**< This constant indicates the inequality condition
                            (!Z) */
    E_CPUCONDITION_CS, /**< This constant indicates the unsigned higher or same
                            condition (C). */
    E_CPUCONDITION_CC, /**< This constant indicates the unsigned lower condition
                            (!C). */
    E_CPUCONDITION_MI, /**< This constant indicates the signed negative
                            condition (N). */
    E_CPUCONDITION_PL, /**< This constant indicates the signed positive or zero
                            condition (!N). */
    E_CPUCONDITION_VS, /**< This constant indicates the signed overflow
                            condition (V). */
    E_CPUCONDITION_VC, /**< This constant indicates the signed no overflow
                            condition (!V). */
    E_CPUCONDITION_HI, /**< This constant indicates the unsigned higher
                            condition (C && !V). */
    E_CPUCONDITION_LS, /**< This constant indicates the unsigned lower or same
                            condition (!C || V). */
    E_CPUCONDITION_GE, /**< This constant indicates the signed greater or equal
                            condition (N == V). */
    E_CPUCONDITION_LT, /**< This constant indicates the signed less than
                            condition (N != V). */
    E_CPUCONDITION_GT, /**< This constant indicates the signed greater than
                            condition (!Z && N == V). */
    E_CPUCONDITION_LE, /**< This constant indicates the signed less or equal
                            condition (Z || N != V). */
    E_CPUCONDITION_AL, /**< This constant indicates the "always" condition
                            (true). */
    E_CPUCONDITION_NV /**< This constant indicates the "never" condition
                           (false). */
} t_cpuCondition;

/**
 * @brief This enumeration defines constants for the registers that have a
 *        special purpose.
 */
enum {
    E_CPUREGISTER_IP = 12, ///< This constant designates the IP (R12) register.
    E_CPUREGISTER_SP = 13, ///< This constant designates the SP (R13) register.
    E_CPUREGISTER_LR = 14, ///< This constant designates the LR (R14) register.
    E_CPUREGISTER_PC = 15  ///< This constant designates the PC (R15) register.
};

/**
 * @brief This enumeration defines the different shift types.
 */
typedef enum {
    E_SHIFTTYPE_LSL, /**< This constant means that the shift is a logical left
                          shift. */
    E_SHIFTTYPE_LSR, /**< This constant means that the shift is a logical right
                          shift. */
    E_SHIFTTYPE_ASR, /**< This constant means that the shift is an arithmetical
                          right shift. */
    E_SHIFTTYPE_ROR /**< This constant means that the shift is a right rotation.
                         */
} t_shiftType;

//==============================================================================
// Private variables
//==============================================================================
/**
 * @brief This variable contains the status of the Z (zero) flag.
 */
static bool s_cpuFlagZ;

/**
 * @brief This variable contains the status of the C (carry) flag.
 */
static bool s_cpuFlagC;

/**
 * @brief This variable contains the status of the N (negative) flag.
 */
static bool s_cpuFlagN;

/**
 * @brief This variable contains the status of the V (overflow) flag.
 */
static bool s_cpuFlagV;

/**
 * @brief This variable contains the status of the I (IRQ) flag.
 */
static bool s_cpuFlagI;

/**
 * @brief This variable contains the status of the F (FIQ) flag.
 */
static bool s_cpuFlagF;

/**
 * @brief This variable contains the status of the T (Thumb) flag.
 */
static bool s_cpuFlagT;

/**
 * @brief This variable contains the values of the 16 general-purpose registers.
 */
static uint32_t s_cpuRegisterR[16];

/**
 * @brief This variable contains the saved values of the R8_usr to R14_usr
 *        registers when the CPU is in FIQ mode.
 */
static uint32_t s_cpuRegisterUsrR[7];

/**
 * @brief This variable contains the saved values of the R13_irq and R14_irq
 *        registers when the CPU is not in IRQ mode.
 */
static uint32_t s_cpuRegisterIrqR[2];

/**
 * @brief This variable contains the saved values of the R8_fiq to R14_fiq
 *        registers when the CPU is not in FIQ mode.
 */
static uint32_t s_cpuRegisterFiqR[7];

/**
 * @brief This variable contains the saved values of the R13_svc and R14_svc
 *        registers when the CPU is not in IRQ mode.
 */
static uint32_t s_cpuRegisterSvcR[2];

/**
 * @brief This variable contains the saved values of the R13_abt and R14_abt
 *        registers when the CPU is not in IRQ mode.
 */
static uint32_t s_cpuRegisterAbtR[2];

/**
 * @brief This variable contains the saved values of the R13_und and R14_und
 *        registers when the CPU is not in IRQ mode.
 */
static uint32_t s_cpuRegisterUndR[2];

/**
 * @brief This variable contains the saved value of the SPSR_irq register when
 *        the CPU is not in IRQ mode.
 */
static uint32_t s_cpuRegisterSpsrIrq;

/**
 * @brief This variable contains the saved value of the SPSR_fiq register when
 *        the CPU is not in FIQ mode.
 */
static uint32_t s_cpuRegisterSpsrFiq;

/**
 * @brief This variable contains the saved value of the SPSR_svc register when
 *        the CPU is not in SVC mode.
 */
static uint32_t s_cpuRegisterSpsrSvc;

/**
 * @brief This variable contains the saved value of the SPSR_abt register when
 *        the CPU is not in ABT mode.
 */
static uint32_t s_cpuRegisterSpsrAbt;

/**
 * @brief This variable contains the saved value of the SPSR_und register when
 *        the CPU is not in UND mode.
 */
static uint32_t s_cpuRegisterSpsrUnd;

/**
 * @brief This variable contains the result of the shifter operation.
 */
static uint32_t s_cpuShifterResult;

/**
 * @brief This variable contains the carry flag resulting of the last shifter
 *        operation.
 */
static bool s_cpuShifterCarry;

/**
 * @brief This variable contains the status of the CPU pipeline.
 */
static t_cpuPipelineState s_cpuPipelineState;

/**
 * @brief This variable contains the current mode of the CPU.
 */
static t_cpuMode s_cpuMode;

/**
 * @brief This union contains the fetched opcode.
 */
static union {
    uint32_t arm; ///< This field contains the fetched opcode in ARM mode.
    uint16_t thumb; ///< This field contains the fetched opcode in Thumb mode.
} s_cpuFetchedOpcode;

/**
 * @brief This union contains the decoded opcode.
 */
static union {
    struct {
        void (*handler)(uint32_t p_opcode); /**< This field contains the decoded
                                                 opcode handler in ARM mode. */
        uint32_t opcode; /**< This field contains the decoded opcode value in
                              ARM mode. */
    } arm; ///< This field contains the decoded opcode in ARM mode.

    struct {
        void (*handler)(uint16_t p_opcode); /**< This field contains the decoded
                                                 opcode handler in Thumb mode.
                                                 */
        uint16_t opcode; /**< This field contains the decoded opcode value in
                              Thumb mode. */
    } thumb; ///< This field contains the decoded opcode in Thumb mode.
} s_cpuDecodedOpcode;

/**
 * @brief This structure contains the opcode decoding tables.
 */
static struct {
    void (*arm[4096])(uint32_t p_opcode); /**< This field contains the ARM
                                               opcode decoding table. */
    void (*thumb[1024])(uint16_t p_opcode); /**< This field contains the Thumb
                                                 opcode decoding table. */
} s_cpuDecodeTable;

//==============================================================================
// Private functions declaration
//==============================================================================
/**
 * @brief Checks if the given condition is met with the current status of the
 *        flags of the CPU.
 *
 * @param[in] p_condition The condition to check.
 *
 * @returns A boolean value that indicates whether or not the condition is met.
 * @retval true if the condition is met.
 * @retval false if the condition is not met.
 */
static bool cpuCheckCondition(t_cpuCondition p_condition);

/**
 * @brief Changes the current CPU mode.
 *
 * @param[in] p_newMode The new mode of the CPU.
 */
static void cpuChangeMode(t_cpuMode p_newMode);

/**
 * @brief Makes the CPU jump to the given address.
 * @details This function does not affect the CPU flags. It flushes the CPU
 *          pipeline in order to ensure that no instruction of the previous
 *          code section will execute.
 *
 * @param[in] p_address The address of the next instruction to execute.
 */
static void cpuJump(uint32_t p_address);

/**
 * @brief Sets the IRQ mode.
 */
static void cpuRaiseIrq(void);

/**
 * @brief Sets the SVC mode.
 * @details This function shall be called when a SWI opcode is executed.
 */
static void cpuRaiseSwi(void);

/**
 * @brief Sets the UND mode.
 * @details This function shall be called when an undefined opcode is executed.
 */
static void cpuRaiseUnd(void);

/**
 * @brief Performs the fetch operation of the CPU cycle.
 * @details If the CPU pipeline is at least in "FETCH" state, read the opcode at
 *          R15 and increase R15.
 *
 * @param[in] p_fetchAddress The memory address where the opcode shall be
 *                           fetched.
 */
static void cpuFetch(uint32_t p_fetchAddress);

/**
 * @brief Performs the decode operation of the CPU cycle.
 * @details If the CPU pipeline is at least in "DECODE" state, decode the
 *          previously fetched opcode.
 */
static void cpuDecode(void);

/**
 * @brief Performs the execute operation of the CPU cycle.
 * @details If the CPU pipeline is at least in "EXECUTE" state, check if an IRQ
 *          line is high. If so, go into the IRQ mode, otherwise execute the
 *          previously decoded opcode.
 */
static void cpuExecute(void);

/**
 * @brief Returns the CPSR as an unsigned 32-bit value.
 *
 * @returns The CPSR as an unsigned 32-bit value.
 */
static uint32_t cpuGetCpsr(void);

/**
 * @brief Returns the SPSR as an unsigned 32-bit value.
 *
 * @returns The SPSR as an unsigned 32-bit value.
 */
static uint32_t cpuGetSpsr(void);

/**
 * @brief Sets the CPSR register with the given value.
 *
 * @param[in] p_value The value of the new CPSR.
 */
static void cpuSetCpsr(uint32_t p_value);

/**
 * @brief Sets the SPSR register with the given value.
 *
 * @param[in] p_value The value of the new SPSR.
 */
static void cpuSetSpsr(uint32_t p_value);

/**
 * @brief Initializes the ARM opcode decoding table.
 */
static void cpuInitArmDecodeTable(void);

/**
 * @brief Initializes the Thumb opcode decoding table.
 */
static void cpuInitThumbDecodeTable(void);

/**
 * @brief Gets the operand of a data processing opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static inline void cpuUtilArmDataProcessingGetOperand(uint32_t p_opcode);

/**
 * @brief Sets the Z and N flags of the CPU according to the given operation
 *        result.
 *
 * @param[in] p_result The result of the operation.
 */
static inline void cpuUtilSetFlagsArithmetical(uint32_t p_result);

/**
 * @brief Sets the Z and N flags of the CPU according to the given operation
 *        result. The C flag will be set to the shifter carry flag value.
 *
 * @param[in] p_result The result of the operation.
 */
static inline void cpuUtilSetFlagsLogical(uint32_t p_result);

/**
 * @brief Sets the C flag according to the operands of the SUB opcode.
 *
 * @param[in] p_leftOperand The left operand of the SUB operation.
 * @param[in] p_rightOperand The right operand of the SUB operation.
 */
static inline void cpuUtilSetCarryFlagSub(
    uint32_t p_leftOperand,
    uint32_t p_rightOperand
);

/**
 * @brief Sets the C flag according to the operands of the SUB opcode.
 *
 * @param[in] p_leftOperand The left operand of the SUB operation.
 * @param[in] p_rightOperand The right operand of the SUB operation.
 * @param[in] p_result The result of the SUB operation.
 */
static inline void cpuUtilSetOverflowFlagSub(
    uint32_t p_leftOperand,
    uint32_t p_rightOperand,
    uint32_t p_result
);

/**
 * @brief Sets the C flag according to the operands of the SBC opcode.
 *
 * @param[in] p_leftOperand The left operand of the SBC operation.
 * @param[in] p_rightOperand The right operand of the SBC operation.
 */
static inline void cpuUtilSetCarryFlagSbc(
    uint32_t p_leftOperand,
    uint32_t p_rightOperand
);

/**
 * @brief Sets the C flag according to the operands of the ADD opcode.
 *
 * @param[in] p_leftOperand The left operand of the ADD operation.
 * @param[in] p_rightOperand The right operand of the ADD operation.
 * @param[in] p_result The result of the ADD operation.
 */
static inline void cpuUtilSetOverflowFlagAdd(
    uint32_t p_leftOperand,
    uint32_t p_rightOperand,
    uint32_t p_result
);

/**
 * @brief Sets CPSR to the value of SPSR.
 */
static inline void cpuRestoreCpsr(void);

/**
 * @brief Sets the given register to the given value.
 *
 * @param[in] p_register The register to write.
 * @param[in] p_value The value to write in the register.
 */
static inline void cpuWriteRegister(uint32_t p_register, uint32_t p_value);

/**
 * @brief Executes the ARM ADC opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmAdc(uint32_t p_opcode);

/**
 * @brief Executes the ARM ADD opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmAdd(uint32_t p_opcode);

/**
 * @brief Executes the ARM AND opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmAnd(uint32_t p_opcode);

/**
 * @brief Executes the ARM B opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmB(uint32_t p_opcode);

/**
 * @brief Executes the ARM BIC opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmBic(uint32_t p_opcode);

/**
 * @brief Executes the ARM BL opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmBl(uint32_t p_opcode);

/**
 * @brief Executes the ARM BX opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmBx(uint32_t p_opcode);

/**
 * @brief Executes the ARM CMN opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmCmn(uint32_t p_opcode);

/**
 * @brief Executes the ARM CMP opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmCmp(uint32_t p_opcode);

/**
 * @brief Executes the ARM EOR opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmEor(uint32_t p_opcode);

/**
 * @brief Executes the ARM MLA opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmMla(uint32_t p_opcode);

/**
 * @brief Executes the ARM MOV opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmMov(uint32_t p_opcode);

/**
 * @brief Executes the ARM MRS opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmMrs(uint32_t p_opcode);

/**
 * @brief Executes the ARM MSR opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmMsr(uint32_t p_opcode);

/**
 * @brief Executes the ARM MUL opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmMul(uint32_t p_opcode);

/**
 * @brief Executes the ARM MVN opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmMvn(uint32_t p_opcode);

/**
 * @brief Executes the ARM ORR opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmOrr(uint32_t p_opcode);

/**
 * @brief Executes the ARM RSB opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmRsb(uint32_t p_opcode);

/**
 * @brief Executes the ARM RSC opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmRsc(uint32_t p_opcode);

/**
 * @brief Executes the ARM SBC opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmSbc(uint32_t p_opcode);

/**
 * @brief Executes the ARM SMLAL opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmSmlal(uint32_t p_opcode);

/**
 * @brief Executes the ARM SMULL opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmSmull(uint32_t p_opcode);

/**
 * @brief Executes the ARM SUB opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmSub(uint32_t p_opcode);

/**
 * @brief Executes the ARM TEQ opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmTeq(uint32_t p_opcode);

/**
 * @brief Executes the ARM TST opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmTst(uint32_t p_opcode);

/**
 * @brief Executes the ARM UMLAL opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmUmlal(uint32_t p_opcode);

/**
 * @brief Executes the ARM UMULL opcode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmUmull(uint32_t p_opcode);

/**
 * @brief Puts the CPU in UND mode.
 *
 * @param[in] p_opcode The opcode as a 32-bit word.
 */
static void cpuOpcodeArmUnd(uint32_t p_opcode);

/**
 * @brief Puts the CPU in UND mode.
 *
 * @param[in] p_opcode The opcode as a 16-bit halfword.
 */
static void cpuOpcodeThumbUnd(uint16_t p_opcode);

//==============================================================================
// Public functions definition
//==============================================================================
void cpuInit(void) {
    cpuInitArmDecodeTable();
    cpuInitThumbDecodeTable();
}

void cpuReset(bool p_skipBoot) {
    // Reset CPU pipeline state
    s_cpuPipelineState = E_CPUPIPELINESTATE_FLUSH;

    // Reset flags
    s_cpuFlagZ = false;
    s_cpuFlagC = false;
    s_cpuFlagN = false;
    s_cpuFlagV = false;
    s_cpuFlagT = false;

    // Reset registers
    memset(s_cpuRegisterR, 0, sizeof(s_cpuRegisterR));
    memset(s_cpuRegisterIrqR, 0, sizeof(s_cpuRegisterIrqR));
    memset(s_cpuRegisterSvcR, 0, sizeof(s_cpuRegisterSvcR));
    memset(s_cpuRegisterAbtR, 0, sizeof(s_cpuRegisterAbtR));
    memset(s_cpuRegisterUndR, 0, sizeof(s_cpuRegisterUndR));
    memset(s_cpuRegisterFiqR, 0, sizeof(s_cpuRegisterFiqR));
    memset(s_cpuRegisterUsrR, 0, sizeof(s_cpuRegisterUsrR));
    s_cpuRegisterSpsrAbt = 0;
    s_cpuRegisterSpsrFiq = 0;
    s_cpuRegisterSpsrIrq = 0;
    s_cpuRegisterSpsrSvc = 0;
    s_cpuRegisterSpsrUnd = 0;

    if(p_skipBoot) {
        s_cpuFlagI = false;
        s_cpuFlagF = false;
        s_cpuRegisterR[13] = 0x03007f00;
        s_cpuRegisterR[15] = 0x08000000;
        s_cpuRegisterIrqR[0] = 0x03007fa0;
        s_cpuRegisterSvcR[0] = 0x03007fe0;
        s_cpuMode = E_CPUMODE_SYS;
    } else {
        s_cpuFlagI = false;
        s_cpuFlagF = false;
        s_cpuMode = E_CPUMODE_SVC;
    }
}

void cpuCycle(void) {

}

//==============================================================================
// Private functions definition
//==============================================================================
static bool cpuCheckCondition(t_cpuCondition p_condition) {
    switch(p_condition) {
        case E_CPUCONDITION_EQ: return s_cpuFlagZ;
        case E_CPUCONDITION_NE: return !s_cpuFlagZ;
        case E_CPUCONDITION_CS: return s_cpuFlagC;
        case E_CPUCONDITION_CC: return !s_cpuFlagC;
        case E_CPUCONDITION_MI: return s_cpuFlagN;
        case E_CPUCONDITION_PL: return !s_cpuFlagN;
        case E_CPUCONDITION_VS: return s_cpuFlagV;
        case E_CPUCONDITION_VC: return !s_cpuFlagV;
        case E_CPUCONDITION_HI: return s_cpuFlagC && !s_cpuFlagZ;
        case E_CPUCONDITION_LS: return (!s_cpuFlagC) || s_cpuFlagZ;
        case E_CPUCONDITION_GE: return s_cpuFlagN == s_cpuFlagV;
        case E_CPUCONDITION_LT: return s_cpuFlagN != s_cpuFlagV;
        case E_CPUCONDITION_GT:
            return (!s_cpuFlagZ) && (s_cpuFlagN == s_cpuFlagV);
        case E_CPUCONDITION_LE: return s_cpuFlagZ || (s_cpuFlagN != s_cpuFlagV);
        case E_CPUCONDITION_AL: return true;
        default: return false;
    }
}

static void cpuChangeMode(t_cpuMode p_newMode) {
    // Save banked registers
    switch(s_cpuMode) {
        case E_CPUMODE_OLD_USR:
        case E_CPUMODE_USR:
        case E_CPUMODE_SYS:
            s_cpuRegisterUsrR[0] = s_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = s_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = s_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = s_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = s_cpuRegisterR[12];
            s_cpuRegisterUsrR[5] = s_cpuRegisterR[13];
            s_cpuRegisterUsrR[6] = s_cpuRegisterR[14];
            break;

        case E_CPUMODE_OLD_FIQ:
        case E_CPUMODE_FIQ:
            s_cpuRegisterFiqR[0] = s_cpuRegisterR[8];
            s_cpuRegisterFiqR[1] = s_cpuRegisterR[9];
            s_cpuRegisterFiqR[2] = s_cpuRegisterR[10];
            s_cpuRegisterFiqR[3] = s_cpuRegisterR[11];
            s_cpuRegisterFiqR[4] = s_cpuRegisterR[12];
            s_cpuRegisterFiqR[5] = s_cpuRegisterR[13];
            s_cpuRegisterFiqR[6] = s_cpuRegisterR[14];
            break;

        case E_CPUMODE_OLD_IRQ:
        case E_CPUMODE_IRQ:
            s_cpuRegisterUsrR[0] = s_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = s_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = s_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = s_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = s_cpuRegisterR[12];
            s_cpuRegisterIrqR[0] = s_cpuRegisterR[13];
            s_cpuRegisterIrqR[0] = s_cpuRegisterR[14];
            break;

        case E_CPUMODE_OLD_SVC:
        case E_CPUMODE_SVC:
            s_cpuRegisterUsrR[0] = s_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = s_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = s_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = s_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = s_cpuRegisterR[12];
            s_cpuRegisterSvcR[0] = s_cpuRegisterR[13];
            s_cpuRegisterSvcR[1] = s_cpuRegisterR[14];
            break;

        case E_CPUMODE_ABT:
            s_cpuRegisterUsrR[0] = s_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = s_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = s_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = s_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = s_cpuRegisterR[12];
            s_cpuRegisterAbtR[0] = s_cpuRegisterR[13];
            s_cpuRegisterAbtR[1] = s_cpuRegisterR[14];
            break;

        case E_CPUMODE_UND:
            s_cpuRegisterUsrR[0] = s_cpuRegisterR[8];
            s_cpuRegisterUsrR[1] = s_cpuRegisterR[9];
            s_cpuRegisterUsrR[2] = s_cpuRegisterR[10];
            s_cpuRegisterUsrR[3] = s_cpuRegisterR[11];
            s_cpuRegisterUsrR[4] = s_cpuRegisterR[12];
            s_cpuRegisterUndR[0] = s_cpuRegisterR[13];
            s_cpuRegisterUndR[1] = s_cpuRegisterR[14];
            break;
    }

    // Restore banked registers
    switch(p_newMode) {
        case E_CPUMODE_OLD_USR:
        case E_CPUMODE_USR:
        case E_CPUMODE_SYS:
            s_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            s_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            s_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            s_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            s_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            s_cpuRegisterR[13] = s_cpuRegisterUsrR[5];
            s_cpuRegisterR[14] = s_cpuRegisterUsrR[6];
            break;

        case E_CPUMODE_OLD_FIQ:
        case E_CPUMODE_FIQ:
            s_cpuRegisterR[8] = s_cpuRegisterFiqR[0];
            s_cpuRegisterR[9] = s_cpuRegisterFiqR[1];
            s_cpuRegisterR[10] = s_cpuRegisterFiqR[2];
            s_cpuRegisterR[11] = s_cpuRegisterFiqR[3];
            s_cpuRegisterR[12] = s_cpuRegisterFiqR[4];
            s_cpuRegisterR[13] = s_cpuRegisterFiqR[5];
            s_cpuRegisterR[14] = s_cpuRegisterFiqR[6];
            break;

        case E_CPUMODE_OLD_IRQ:
        case E_CPUMODE_IRQ:
            s_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            s_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            s_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            s_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            s_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            s_cpuRegisterR[13] = s_cpuRegisterIrqR[0];
            s_cpuRegisterR[14] = s_cpuRegisterIrqR[1];
            break;

        case E_CPUMODE_OLD_SVC:
        case E_CPUMODE_SVC:
            s_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            s_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            s_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            s_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            s_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            s_cpuRegisterR[13] = s_cpuRegisterSvcR[0];
            s_cpuRegisterR[14] = s_cpuRegisterSvcR[1];
            break;

        case E_CPUMODE_ABT:
            s_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            s_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            s_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            s_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            s_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            s_cpuRegisterR[13] = s_cpuRegisterAbtR[0];
            s_cpuRegisterR[14] = s_cpuRegisterAbtR[1];
            break;

        case E_CPUMODE_UND:
            s_cpuRegisterR[8] = s_cpuRegisterUsrR[0];
            s_cpuRegisterR[9] = s_cpuRegisterUsrR[1];
            s_cpuRegisterR[10] = s_cpuRegisterUsrR[2];
            s_cpuRegisterR[11] = s_cpuRegisterUsrR[3];
            s_cpuRegisterR[12] = s_cpuRegisterUsrR[4];
            s_cpuRegisterR[13] = s_cpuRegisterUndR[0];
            s_cpuRegisterR[14] = s_cpuRegisterUndR[1];
            break;
    }

    // Set CPU mode
    s_cpuMode = p_newMode;
}

static void cpuJump(uint32_t p_address) {
    uint32_t l_address;

    if(s_cpuFlagT) {
        l_address = p_address & C_ADDRESS_MASK_16;
    } else {
        l_address = p_address & C_ADDRESS_MASK_32;
    }

    s_cpuRegisterR[15] = l_address;
    s_cpuPipelineState = E_CPUPIPELINESTATE_FLUSH;
}

static void cpuRaiseIrq(void) {
    cpuChangeMode(E_CPUMODE_IRQ);
    s_cpuRegisterSpsrIrq = cpuGetCpsr();
    s_cpuRegisterR[14] = s_cpuRegisterR[15];
    s_cpuFlagT = false;
    s_cpuFlagI = true;
    cpuJump(C_EXCEPTION_VECTOR_IRQ);
}

static void cpuRaiseSwi(void) {
    cpuChangeMode(E_CPUMODE_SVC);
    s_cpuRegisterSpsrSvc = cpuGetCpsr();
    s_cpuRegisterR[14] = s_cpuRegisterR[15];
    s_cpuFlagT = false;
    s_cpuFlagI = true;
    cpuJump(C_EXCEPTION_VECTOR_SWI);
}

static void cpuRaiseUnd(void) {
    cpuChangeMode(E_CPUMODE_UND);
    s_cpuRegisterSpsrUnd = cpuGetCpsr();
    s_cpuRegisterR[14] = s_cpuRegisterR[15];
    s_cpuFlagT = false;
    s_cpuFlagI = true;
    cpuJump(C_EXCEPTION_VECTOR_UND);
}

static void cpuFetch(uint32_t p_fetchAddress) {
    if(s_cpuPipelineState >= E_CPUPIPELINESTATE_FETCH) {
        if(s_cpuFlagT) {
            s_cpuFetchedOpcode.thumb = busRead16(p_fetchAddress);
            s_cpuRegisterR[15] += C_INSTRUCTION_SIZE_THUMB;
        } else {
            s_cpuFetchedOpcode.arm = busRead32(p_fetchAddress);
            s_cpuRegisterR[15] += C_INSTRUCTION_SIZE_ARM;
        }
    }
}

static void cpuDecode(void) {
    if(s_cpuPipelineState >= E_CPUPIPELINESTATE_DECODE) {
        if(s_cpuFlagT) {
            // Take bits 6 to 15
            uint16_t l_decodeIndex = s_cpuFetchedOpcode.thumb >> 6;

            s_cpuDecodedOpcode.thumb.opcode = s_cpuFetchedOpcode.thumb;
            s_cpuDecodedOpcode.thumb.handler = s_cpuDecodeTable
                .thumb[l_decodeIndex];
        } else {
            uint16_t l_decodeIndex;

            // Take bits 20 to 27
            l_decodeIndex = (s_cpuFetchedOpcode.arm >> 16) & 0x00000ff0;

            // Take bits 4 to 7
            l_decodeIndex |= (s_cpuFetchedOpcode.arm >> 4) & 0x0000000f;

            s_cpuDecodedOpcode.arm.opcode = s_cpuFetchedOpcode.arm;
            s_cpuDecodedOpcode.arm.handler = s_cpuDecodeTable
                .arm[l_decodeIndex];
        }
    }
}

static void cpuExecute(void) {
    if(s_cpuPipelineState >= E_CPUPIPELINESTATE_EXECUTE) {
        // TODO: Check for hardware interrupts

        if(s_cpuFlagT) {
            s_cpuDecodedOpcode.thumb.handler(s_cpuDecodedOpcode.thumb.opcode);
        } else {
            s_cpuDecodedOpcode.arm.handler(s_cpuDecodedOpcode.arm.opcode);
        }
    }
}

static uint32_t cpuGetCpsr(void) {
    uint32_t l_cpsr = 0;

    if(s_cpuFlagN) l_cpsr |= C_PSR_MASK_FLAG_N;
    if(s_cpuFlagZ) l_cpsr |= C_PSR_MASK_FLAG_Z;
    if(s_cpuFlagC) l_cpsr |= C_PSR_MASK_FLAG_C;
    if(s_cpuFlagV) l_cpsr |= C_PSR_MASK_FLAG_V;
    if(s_cpuFlagI) l_cpsr |= C_PSR_MASK_FLAG_I;
    if(s_cpuFlagF) l_cpsr |= C_PSR_MASK_FLAG_F;
    if(s_cpuFlagT) l_cpsr |= C_PSR_MASK_FLAG_T;

    l_cpsr |= s_cpuMode;

    return l_cpsr;
}

static uint32_t cpuGetSpsr(void) {
    uint32_t l_spsr = 0;

    switch(s_cpuMode) {
        case E_CPUMODE_OLD_USR:
        case E_CPUMODE_USR:
        case E_CPUMODE_SYS:
            l_spsr = cpuGetCpsr();
            break;

        case E_CPUMODE_OLD_FIQ:
        case E_CPUMODE_FIQ:
            l_spsr = s_cpuRegisterSpsrFiq;
            break;

        case E_CPUMODE_OLD_IRQ:
        case E_CPUMODE_IRQ:
            l_spsr = s_cpuRegisterSpsrIrq;
            break;

        case E_CPUMODE_OLD_SVC:
        case E_CPUMODE_SVC:
            l_spsr = s_cpuRegisterSpsrSvc;
            break;

        case E_CPUMODE_ABT:
            l_spsr = s_cpuRegisterSpsrAbt;
            break;

        case E_CPUMODE_UND:
            l_spsr = s_cpuRegisterSpsrUnd;
            break;
    }

    return l_spsr;
}

static void cpuSetCpsr(uint32_t p_value) {
    s_cpuFlagN = (p_value & C_PSR_MASK_FLAG_N) != 0;
    s_cpuFlagZ = (p_value & C_PSR_MASK_FLAG_Z) != 0;
    s_cpuFlagC = (p_value & C_PSR_MASK_FLAG_C) != 0;
    s_cpuFlagV = (p_value & C_PSR_MASK_FLAG_V) != 0;
    s_cpuFlagI = (p_value & C_PSR_MASK_FLAG_I) != 0;
    s_cpuFlagF = (p_value & C_PSR_MASK_FLAG_F) != 0;
    s_cpuFlagT = (p_value & C_PSR_MASK_FLAG_T) != 0;

    cpuChangeMode(p_value & C_PSR_MASK_MODE);
}

static void cpuSetSpsr(uint32_t p_value) {
    switch(s_cpuMode) {
        case E_CPUMODE_OLD_USR:
        case E_CPUMODE_USR:
        case E_CPUMODE_SYS:
            cpuSetCpsr(p_value);
            break;

        case E_CPUMODE_OLD_FIQ:
        case E_CPUMODE_FIQ:
            s_cpuRegisterSpsrFiq = p_value;
            break;

        case E_CPUMODE_OLD_IRQ:
        case E_CPUMODE_IRQ:
            s_cpuRegisterSpsrIrq = p_value;
            break;

        case E_CPUMODE_OLD_SVC:
        case E_CPUMODE_SVC:
            s_cpuRegisterSpsrSvc = p_value;
            break;

        case E_CPUMODE_ABT:
            s_cpuRegisterSpsrAbt = p_value;
            break;

        case E_CPUMODE_UND:
            s_cpuRegisterSpsrUnd = p_value;
            break;
    }
}

static void cpuInitArmDecodeTable(void) {
    for(uint32_t l_tableIndex = 0; l_tableIndex < 4096; l_tableIndex++) {
        uint32_t l_opcode = (
            ((l_tableIndex & 0x00000ff0) << 16)
            | ((l_tableIndex & 0x0000000f) << 4)
        );

        void (*l_opcodeHandler)(uint32_t p_opcode);

        if((l_opcode & 0x0ffffff0) == 0x01200010) { // BX
            l_opcodeHandler = cpuOpcodeArmBx;
        } else if((l_opcode & 0x0e000000) == 0x0e000000) { // B/BL
            if((l_opcode & 0x0f000000) == 0x0e000000) {
                l_opcodeHandler = cpuOpcodeArmB;
            } else {
                l_opcodeHandler = cpuOpcodeArmBl;
            }
        } else if(
            ((l_opcode & 0x0c000000) == 0x00000000) // Data processing
            && ((l_opcode & 0x00000090) != 0x00000090) // b4 and b7 set
            && ((l_opcode & 0x01900000) != 0x01000000) // test opcode, S clear
        ) {
            switch((l_opcode & 0x01e00000) >> 21) {
                case 0b0000: l_opcodeHandler = cpuOpcodeArmAnd; break;
                case 0b0001: l_opcodeHandler = cpuOpcodeArmEor; break;
                case 0b0010: l_opcodeHandler = cpuOpcodeArmSub; break;
                case 0b0011: l_opcodeHandler = cpuOpcodeArmRsb; break;
                case 0b0100: l_opcodeHandler = cpuOpcodeArmAdd; break;
                case 0b0101: l_opcodeHandler = cpuOpcodeArmAdc; break;
                case 0b0110: l_opcodeHandler = cpuOpcodeArmSbc; break;
                case 0b0111: l_opcodeHandler = cpuOpcodeArmRsc; break;
                case 0b1000: l_opcodeHandler = cpuOpcodeArmTst; break;
                case 0b1001: l_opcodeHandler = cpuOpcodeArmTeq; break;
                case 0b1010: l_opcodeHandler = cpuOpcodeArmCmp; break;
                case 0b1011: l_opcodeHandler = cpuOpcodeArmCmn; break;
                case 0b1100: l_opcodeHandler = cpuOpcodeArmOrr; break;
                case 0b1101: l_opcodeHandler = cpuOpcodeArmMov; break;
                case 0b1110: l_opcodeHandler = cpuOpcodeArmBic; break;
                case 0b1111: l_opcodeHandler = cpuOpcodeArmMvn; break;
            }
        } else if((l_opcode & 0x0fb000f0) == 0x01000000) { // MRS
            l_opcodeHandler = cpuOpcodeArmMrs;
        } else if((l_opcode & 0x0db00000) == 0x01200000) { // MSR
            l_opcodeHandler = cpuOpcodeArmMsr;
        } else if((l_opcode & 0x0fe000f0) == 0x00000090) { // MUL
            l_opcodeHandler = cpuOpcodeArmMul;
        } else if((l_opcode & 0x0fe000f0) == 0x00200090) { // MLA
            l_opcodeHandler = cpuOpcodeArmMla;
        } else if((l_opcode & 0x0fe000f0) == 0x00800090) { // UMULL
            l_opcodeHandler = cpuOpcodeArmUmull;
        } else if((l_opcode & 0x0fe000f0) == 0x00a00090) { // UMLAL
            l_opcodeHandler = cpuOpcodeArmUmlal;
        } else if((l_opcode & 0x0fe000f0) == 0x00c00090) { // SMULL
            l_opcodeHandler = cpuOpcodeArmSmull;
        } else if((l_opcode & 0x0fe000f0) == 0x00e00090) { // SMLAL
            l_opcodeHandler = cpuOpcodeArmSmlal;
        } else {
            l_opcodeHandler = cpuOpcodeArmUnd;
        }

        s_cpuDecodeTable.arm[l_opcode] = l_opcodeHandler;
    }
}

static void cpuInitThumbDecodeTable(void) {
    for(uint16_t l_tableIndex = 0; l_tableIndex < 1024; l_tableIndex++) {
        uint16_t l_opcode = l_tableIndex << 6;

        void (*l_opcodeHandler)(uint16_t p_opcode);

        l_opcodeHandler = cpuOpcodeThumbUnd;

        s_cpuDecodeTable.thumb[l_opcode] = l_opcodeHandler;
    }
}

static inline void cpuUtilArmDataProcessingGetOperand(uint32_t p_opcode) {
    bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

    if(l_isImmediate) {
        uint32_t l_immediate = (uint32_t)(p_opcode & 0x000000ff);
        uint32_t l_shift = (uint32_t)((p_opcode >> 8) & 0x0000000f);

        l_shift <<= 1;

        if(l_shift == 0) {
            s_cpuShifterResult = l_immediate;
            s_cpuShifterCarry = s_cpuFlagC;
        } else {
            s_cpuShifterResult = rotateRight(l_immediate, l_shift);
            s_cpuShifterCarry = ((uint32_t)s_cpuShifterResult >> 31) != 0;
        }
    } else {
        t_shiftType l_shiftType = ((uint32_t)(p_opcode & 0x00000060)) >> 5;
        bool l_shiftByRegister = ((uint32_t)(p_opcode & 0x00000010)) != 0;
        uint32_t l_operandRegister = (uint32_t)(p_opcode & 0x0000000f);
        uint32_t l_operand = s_cpuRegisterR[l_operandRegister];

        if(l_operandRegister == E_CPUREGISTER_PC) {
            l_operand += C_INSTRUCTION_SIZE_ARM;
        }

        if(l_shiftByRegister) {
            uint32_t l_shiftRegister = ((uint32_t)(p_opcode & 0x00000f00)) >> 8;
            uint32_t l_shift = s_cpuRegisterR[l_shiftRegister];

            switch(l_shiftType) {
                case E_SHIFTTYPE_LSL:
                    if(l_shift == 0) {
                        s_cpuShifterResult = l_operand;
                        s_cpuShifterCarry = s_cpuFlagC;
                    } else if(l_shift < 32) {
                        s_cpuShifterResult = (uint32_t)(l_operand << l_shift);
                        s_cpuShifterCarry =
                            ((l_operand >> (32 - l_shift)) & 0x00000001) != 0;
                    } else if(l_shift == 32) {
                        s_cpuShifterResult = 0;
                        s_cpuShifterCarry = (l_operand & 0x00000001) != 0;
                    } else {
                        s_cpuShifterResult = 0;
                        s_cpuShifterCarry = false;
                    }

                    break;

                case E_SHIFTTYPE_LSR:
                    if(l_shift == 0) {
                        s_cpuShifterResult = l_operand;
                        s_cpuShifterCarry = s_cpuFlagC;
                    } else if(l_shift < 32) {
                        s_cpuShifterResult = (uint32_t)(l_operand >> l_shift);
                        s_cpuShifterCarry =
                            ((l_operand >> (l_shift - 1)) & 0x00000001) != 0;
                    } else if(l_shift == 32) {
                        s_cpuShifterResult = 0;
                        s_cpuShifterCarry = (l_operand & 0x80000000) != 0;
                    } else {
                        s_cpuShifterResult = 0;
                        s_cpuShifterCarry = false;
                    }

                    break;

                case E_SHIFTTYPE_ASR:
                    if(l_shift == 0) {
                        s_cpuShifterResult = l_operand;
                        s_cpuShifterCarry = s_cpuFlagC;
                    } else if(l_shift < 32) {
                        s_cpuShifterResult =
                            (uint32_t)(((int32_t)l_operand) >> l_shift);
                        s_cpuShifterCarry =
                            ((l_operand >> (l_shift - 1)) & 0x00000001) != 0;
                    } else {
                        s_cpuShifterResult =
                            (uint32_t)(((int32_t)l_operand) >> 31);
                        s_cpuShifterCarry = (l_operand & 0x80000000) != 0;
                    }

                    break;

                case E_SHIFTTYPE_ROR:
                    uint32_t l_rotation = l_shift & 0x0000001f;

                    if(l_shift == 0) {
                        s_cpuShifterResult = l_operand;
                        s_cpuShifterCarry = s_cpuFlagC;
                    } else if(l_rotation == 0) {
                        s_cpuShifterResult = l_operand;
                        s_cpuShifterCarry = (l_operand & 0x80000000) != 0;
                    } else {
                        s_cpuShifterResult = rotateRight(l_operand, l_rotation);
                        s_cpuShifterCarry =
                            ((l_operand >> (l_rotation - 1)) & 0x00000001) != 0;
                    }

                    break;
            }
        } else {
            uint32_t l_shiftAmount = (uint32_t)((p_opcode & 0x00000f80) >> 7);

            switch(l_shiftType) {
                case E_SHIFTTYPE_LSL:
                    if(l_shiftAmount == 0) {
                        s_cpuShifterResult = l_operand;
                        s_cpuShifterCarry = s_cpuFlagC;
                    } else {
                        s_cpuShifterResult = l_operand << l_shiftAmount;
                        s_cpuShifterCarry =
                            ((l_operand >> (32 - l_shiftAmount)) & 0x00000001)
                            != 0;
                    }

                    break;

                case E_SHIFTTYPE_LSR:
                    if(l_shiftAmount == 0) {
                        s_cpuShifterResult = 0;
                        s_cpuShifterCarry = (l_operand & 0x80000000) != 0;
                    } else {
                        s_cpuShifterResult = l_operand >> l_shiftAmount;
                        s_cpuShifterCarry =
                            ((l_operand >> (l_shiftAmount - 1)) & 0x00000001)
                            != 0;
                    }

                    break;

                case E_SHIFTTYPE_ASR:
                    if(l_shiftAmount == 0) {
                        s_cpuShifterResult =
                            (uint32_t)(((int32_t)l_operand) >> 31);
                        s_cpuShifterCarry = (l_operand & 0x80000000) != 0;
                    } else {
                        s_cpuShifterResult =
                            (uint32_t)(((int32_t)l_operand) >> l_shiftAmount);
                        s_cpuShifterCarry =
                            ((l_operand >> (l_shiftAmount - 1)) & 0x00000001)
                            != 0;
                    }

                    break;

                case E_SHIFTTYPE_ROR:
                    if(l_shiftAmount == 0) {
                        s_cpuShifterResult = l_operand >> 1;

                        if(s_cpuFlagC) {
                            s_cpuShifterResult |= 0x80000000;
                        }
                    } else {
                        s_cpuShifterResult =
                            rotateRight(l_operand, l_shiftAmount);
                        s_cpuShifterCarry =
                            ((l_operand >> (l_shiftAmount - 1)) & 0x00000001)
                            != 0;
                    }

                    break;

            }
        }
    }
}

static inline void cpuUtilSetFlagsArithmetical(uint32_t p_result) {
    s_cpuFlagZ = p_result == 0;
    s_cpuFlagN = (p_result & 0x80000000) != 0;
}

static inline void cpuUtilSetFlagsLogical(uint32_t p_result) {
    cpuUtilSetFlagsArithmetical(p_result);
    s_cpuFlagC = s_cpuShifterCarry;
}

static inline void cpuUtilSetCarryFlagSub(
    uint32_t p_leftOperand,
    uint32_t p_rightOperand
) {
    s_cpuFlagC = (p_leftOperand >= p_rightOperand);
}

static inline void cpuUtilSetOverflowFlagSub(
    uint32_t p_leftOperand,
    uint32_t p_rightOperand,
    uint32_t p_result
) {
    bool l_operandsHaveDifferentSign =
        ((p_leftOperand ^ p_rightOperand) & 0x80000000) != 0x00000000;

    if(l_operandsHaveDifferentSign) {
        bool l_resultHasDifferentSign =
            ((p_leftOperand ^ p_result) & 0x80000000) != 0x00000000;

        if(l_resultHasDifferentSign) {
            s_cpuFlagV = true;
        } else {
            s_cpuFlagV = false;
        }
    } else {
        s_cpuFlagV = false;
    }
}

static inline void cpuUtilSetCarryFlagSbc(
    uint32_t p_leftOperand,
    uint32_t p_rightOperand
) {
    uint32_t l_carryValue;

    if(s_cpuFlagC) {
        l_carryValue = 0;
    } else {
        l_carryValue = 1;
    }

    s_cpuFlagC = (p_leftOperand >= p_rightOperand) && (
        ((p_leftOperand - p_rightOperand) >= l_carryValue)
    );
}

static inline void cpuUtilSetOverflowFlagAdd(
    uint32_t p_leftOperand,
    uint32_t p_rightOperand,
    uint32_t p_result
) {
    bool l_operandsHaveSameSign =
        ((p_leftOperand ^ p_rightOperand) & 0x80000000) == 0x00000000;

    if(l_operandsHaveSameSign) {
        bool l_resultHasDifferentSign =
            ((p_leftOperand ^ p_result) & 0x80000000) != 0x00000000;

        if(l_resultHasDifferentSign) {
            s_cpuFlagV = true;
        } else {
            s_cpuFlagV = false;
        }
    } else {
        s_cpuFlagV = false;
    }
}

static inline void cpuRestoreCpsr(void) {
    cpuSetCpsr(cpuGetSpsr());
}

static inline void cpuWriteRegister(uint32_t p_register, uint32_t p_value) {
    if(p_register == E_CPUREGISTER_PC) {
        cpuJump(p_value);
    } else {
        s_cpuRegisterR[p_register] = p_value;
    }
}

static void cpuOpcodeArmAdc(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint64_t l_carryFlagValue;

    if(s_cpuFlagC) {
        l_carryFlagValue = 1;
    } else {
        l_carryFlagValue = 0;
    }

    uint64_t l_result =
        (uint64_t)l_firstOperand
        + (uint64_t)s_cpuShifterResult
        + l_carryFlagValue;

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            s_cpuFlagC = l_result > UINT32_MAX;

            cpuUtilSetOverflowFlagAdd(
                l_firstOperand,
                s_cpuShifterResult,
                (uint32_t)l_result
            );

            cpuUtilSetFlagsArithmetical((uint32_t)l_result);
        }
    }

    cpuWriteRegister(l_destinationRegister, (uint32_t)l_result);
}

static void cpuOpcodeArmAdd(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = l_firstOperand + s_cpuShifterResult;

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            s_cpuFlagC = l_result < l_firstOperand;

            cpuUtilSetOverflowFlagAdd(
                l_firstOperand,
                s_cpuShifterResult,
                l_result
            );

            cpuUtilSetFlagsArithmetical(l_result);
        }
    }

    cpuWriteRegister(l_destinationRegister, (uint32_t)l_result);
}

static void cpuOpcodeArmAnd(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = l_firstOperand & s_cpuShifterResult;

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            cpuUtilSetFlagsLogical(l_result);
        }
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmB(uint32_t p_opcode) {
    uint32_t l_offset = (uint32_t)(p_opcode & 0x00ffffff);

    l_offset <<= 2;
    l_offset = signExtend26to32(l_offset);

    cpuJump(s_cpuRegisterR[E_CPUREGISTER_PC] + l_offset);
}

static void cpuOpcodeArmBic(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = l_firstOperand & (~s_cpuShifterResult);

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            cpuUtilSetFlagsLogical(l_result);
        }
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmBl(uint32_t p_opcode) {
    uint32_t l_offset = (uint32_t)(p_opcode & 0x00ffffff);

    l_offset <<= 2;
    l_offset = signExtend26to32(l_offset);

    s_cpuRegisterR[E_CPUREGISTER_LR] =
        s_cpuRegisterR[E_CPUREGISTER_PC] - C_INSTRUCTION_SIZE_ARM;

    cpuJump(s_cpuRegisterR[E_CPUREGISTER_PC] + l_offset);
}

static void cpuOpcodeArmBx(uint32_t p_opcode) {
    if((p_opcode & 0x0ffffff0) != 0x012fff10) {
        cpuRaiseUnd();
        return;
    }

    uint32_t l_register = p_opcode & 0x0000000f;
    uint32_t l_jumpAddress = s_cpuRegisterR[l_register];

    if((l_jumpAddress & 0x00000001) == 0x00000000) {
        s_cpuFlagT = false;
        l_jumpAddress &= 0xfffffffe;
    } else {
        s_cpuFlagT = true;
        l_jumpAddress &= 0xfffffffc;
    }

    cpuJump(l_jumpAddress);
}

static void cpuOpcodeArmCmn(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = l_firstOperand + s_cpuShifterResult;

    if(l_destinationRegister == E_CPUREGISTER_PC) {
        cpuRestoreCpsr();
    } else {
        s_cpuFlagC = l_result < l_firstOperand;

        cpuUtilSetOverflowFlagAdd(
            l_firstOperand,
            s_cpuShifterResult,
            l_result
        );

        cpuUtilSetFlagsArithmetical(l_result);
    }

    cpuWriteRegister(l_destinationRegister, (uint32_t)l_result);
}

static void cpuOpcodeArmCmp(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = l_firstOperand - s_cpuShifterResult;

    if(l_destinationRegister == E_CPUREGISTER_PC) {
        cpuRestoreCpsr();
    } else {
        cpuUtilSetCarryFlagSub(l_firstOperand, s_cpuShifterResult);
        cpuUtilSetOverflowFlagSub(
            l_firstOperand,
            s_cpuShifterResult,
            l_result
        );
        cpuUtilSetFlagsArithmetical(l_result);
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmEor(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = l_firstOperand ^ s_cpuShifterResult;

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            cpuUtilSetFlagsLogical(l_result);
        }
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmMla(uint32_t p_opcode) {
    uint32_t l_destinationRegister = (p_opcode & 0x000f0000) >> 16;
    uint32_t l_firstOperandRegister = p_opcode & 0x0000000f;
    uint32_t l_secondOperandRegister = (p_opcode & 0x00000f00) >> 8;
    uint32_t l_thirdOperandRegister = (p_opcode & 0x0000f000) >> 12;
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];
    uint32_t l_secondOperand = s_cpuRegisterR[l_secondOperandRegister];
    uint32_t l_thirdOperand = s_cpuRegisterR[l_thirdOperandRegister];
    bool l_setFlags = (p_opcode & 0x00100000) != 0;

    uint32_t l_result = l_firstOperand * l_secondOperand + l_thirdOperand;

    if(l_setFlags) {
        cpuUtilSetFlagsArithmetical(l_result);
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmMov(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            cpuUtilSetFlagsLogical(s_cpuShifterResult);
        }
    }

    cpuWriteRegister(l_destinationRegister, s_cpuShifterResult);
}

static void cpuOpcodeArmMrs(uint32_t p_opcode) {
    if((p_opcode & 0x0fbf0fff) != 0x010f0000) {
        cpuRaiseUnd();
    }

    bool l_getSpsr = (p_opcode & 0x00400000) != 0x00000000;
    uint32_t l_destinationRegister = (p_opcode & 0x0000f000) >> 12;
    uint32_t l_value;

    if(l_getSpsr) {
        l_value = cpuGetSpsr();
    } else {
        l_value = cpuGetCpsr();
    }

    cpuWriteRegister(l_destinationRegister, l_value);
}

static void cpuOpcodeArmMsr(uint32_t p_opcode) {
    bool l_destinationIsSpsr = ((p_opcode & 0x00400000) != 0x00000000);
    uint32_t l_value;

    if((p_opcode & 0x0fbffff0) == 0x0129f000) {
        uint32_t l_sourceRegister = p_opcode & 0x0000000f;

        l_value = s_cpuRegisterR[l_sourceRegister];
    } else { // Flags-only
        if((p_opcode & 0x0fbffff0) == 0x0128f000) {
            // The shift value is multiplied by 2 so we shift one less to the
            // right
            uint32_t l_shift = (p_opcode & 0x00000f00) >> 7;
            uint32_t l_immediate = p_opcode & 0x000000ff;

            l_value = rotateRight(l_immediate, l_shift);
        } else if((p_opcode & 0x0fbff000) == 0x0328f000) {
            uint32_t l_sourceRegister = p_opcode & 0x0000000f;

            l_value = s_cpuRegisterR[l_sourceRegister];
        } else {
            cpuRaiseUnd();
            return;
        }

        // Keep only flags
        l_value &= 0xf0000000;

        // Keep the 28 lower bits
        if(l_destinationIsSpsr) {
            l_value |= cpuGetSpsr() & 0x0fffffff;
        } else {
            l_value |= cpuGetCpsr() & 0x0fffffff;
        }
    }

    if(l_destinationIsSpsr) {
        cpuSetSpsr(l_value);
    } else {
        cpuSetCpsr(l_value);
    }
}

static void cpuOpcodeArmMul(uint32_t p_opcode) {
    uint32_t l_destinationRegister = (p_opcode & 0x000f0000) >> 16;
    uint32_t l_firstOperandRegister = p_opcode & 0x0000000f;
    uint32_t l_secondOperandRegister = (p_opcode & 0x00000f00) >> 8;
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];
    uint32_t l_secondOperand = s_cpuRegisterR[l_secondOperandRegister];
    bool l_setFlags = (p_opcode & 0x00100000) != 0;

    uint32_t l_result = l_firstOperand * l_secondOperand;

    if(l_setFlags) {
        cpuUtilSetFlagsArithmetical(l_result);
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmMvn(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = ~s_cpuShifterResult;

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            cpuUtilSetFlagsLogical(l_result);
        }
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmOrr(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = l_firstOperand | s_cpuShifterResult;

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            cpuUtilSetFlagsLogical(l_result);
        }
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmRsb(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = s_cpuShifterResult - l_firstOperand;

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            cpuUtilSetCarryFlagSub(s_cpuShifterResult, l_firstOperand);
            cpuUtilSetOverflowFlagSub(
                s_cpuShifterResult,
                l_firstOperand,
                l_result
            );
            cpuUtilSetFlagsArithmetical(l_result);
        }
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmRsc(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_carryFlagValue;

    if(s_cpuFlagC) {
        l_carryFlagValue = 1;
    } else {
        l_carryFlagValue = 0;
    }

    uint32_t l_result =
        s_cpuShifterResult - l_firstOperand + l_carryFlagValue - 1;

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            cpuUtilSetCarryFlagSbc(s_cpuShifterResult, l_firstOperand);
            cpuUtilSetOverflowFlagSub(
                s_cpuShifterResult,
                l_firstOperand,
                l_result
            );
            cpuUtilSetFlagsArithmetical(l_result);
        }
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmSbc(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_carryFlagValue;

    if(s_cpuFlagC) {
        l_carryFlagValue = 1;
    } else {
        l_carryFlagValue = 0;
    }

    uint32_t l_result =
        l_firstOperand - s_cpuShifterResult + l_carryFlagValue - 1;

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            cpuUtilSetCarryFlagSbc(l_firstOperand, s_cpuShifterResult);
            cpuUtilSetOverflowFlagSub(
                l_firstOperand,
                s_cpuShifterResult,
                l_result
            );
            cpuUtilSetFlagsArithmetical(l_result);
        }
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmSmlal(uint32_t p_opcode) {
    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_destinationLowRegister = (p_opcode & 0x0000f000) >> 12;
    uint32_t l_destinationHighRegister = (p_opcode & 0x000f0000) >> 16;
    uint32_t l_firstOperandRegister = (p_opcode & 0x00000f00) >> 8;
    uint32_t l_secondOperandRegister = p_opcode & 0x0000000f;
    int64_t l_firstOperand =
        signExtend32to64(s_cpuRegisterR[l_firstOperandRegister]);
    int64_t l_secondOperand =
        signExtend32to64(s_cpuRegisterR[l_secondOperandRegister]);
    uint64_t l_thirdOperandHigh = s_cpuRegisterR[l_destinationHighRegister];
    uint64_t l_thirdOperandLow = s_cpuRegisterR[l_destinationLowRegister];
    int64_t l_thirdOperand =
        (int64_t)(
            (l_thirdOperandHigh << (uint64_t)32)
            | l_thirdOperandLow
        );

    int64_t l_result = l_firstOperand * l_secondOperand + l_thirdOperand;
    uint32_t l_resultHigh = (uint32_t)(l_result >> 32);
    uint32_t l_resultLow = (uint32_t)l_result;

    s_cpuRegisterR[l_destinationHighRegister] = l_resultHigh;
    s_cpuRegisterR[l_destinationLowRegister] = l_resultLow;

    if(l_setFlags) {
        s_cpuFlagZ = (l_result == (int64_t)0);
        s_cpuFlagN = (l_result & 0x8000000000000000) != 0;
    }
}

static void cpuOpcodeArmSmull(uint32_t p_opcode) {
    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_destinationLowRegister = (p_opcode & 0x0000f000) >> 12;
    uint32_t l_destinationHighRegister = (p_opcode & 0x000f0000) >> 16;
    uint32_t l_firstOperandRegister = (p_opcode & 0x00000f00) >> 8;
    uint32_t l_secondOperandRegister = p_opcode & 0x0000000f;
    int64_t l_firstOperand =
        signExtend32to64(s_cpuRegisterR[l_firstOperandRegister]);
    int64_t l_secondOperand =
        signExtend32to64(s_cpuRegisterR[l_secondOperandRegister]);

    int64_t l_result = l_firstOperand * l_secondOperand;
    uint32_t l_resultHigh = (uint32_t)(l_result >> 32);
    uint32_t l_resultLow = (uint32_t)l_result;

    s_cpuRegisterR[l_destinationHighRegister] = l_resultHigh;
    s_cpuRegisterR[l_destinationLowRegister] = l_resultLow;

    if(l_setFlags) {
        s_cpuFlagZ = (l_result == (int64_t)0);
        s_cpuFlagN = (l_result & 0x8000000000000000) != 0;
    }
}

static void cpuOpcodeArmSub(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = l_firstOperand - s_cpuShifterResult;

    if(l_setFlags) {
        if(l_destinationRegister == E_CPUREGISTER_PC) {
            cpuRestoreCpsr();
        } else {
            cpuUtilSetCarryFlagSub(l_firstOperand, s_cpuShifterResult);
            cpuUtilSetOverflowFlagSub(
                l_firstOperand,
                s_cpuShifterResult,
                l_result
            );
            cpuUtilSetFlagsArithmetical(l_result);
        }
    }

    cpuWriteRegister(l_destinationRegister, l_result);
}

static void cpuOpcodeArmTeq(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = l_firstOperand ^ s_cpuShifterResult;

    if(l_destinationRegister == E_CPUREGISTER_PC) {
        cpuRestoreCpsr();
    } else {
        cpuUtilSetFlagsLogical(l_result);
    }
}

static void cpuOpcodeArmTst(uint32_t p_opcode) {
    cpuUtilArmDataProcessingGetOperand(p_opcode);

    uint32_t l_firstOperandRegister = (uint32_t)((p_opcode & 0x000f0000) >> 16);
    uint32_t l_firstOperand = s_cpuRegisterR[l_firstOperandRegister];

    if(l_firstOperandRegister == E_CPUREGISTER_PC) {
        bool l_isImmediate = (p_opcode & 0x02000000) != 0x00000000;

        if(!l_isImmediate) {
            bool l_shiftByRegister = (p_opcode & 0x00000010) != 0x00000000;

            if(l_shiftByRegister) {
                l_firstOperand += C_INSTRUCTION_SIZE_ARM;
            }
        }
    }

    uint32_t l_destinationRegister = (uint32_t)((p_opcode & 0x0000f000) >> 12);
    uint32_t l_result = l_firstOperand & s_cpuShifterResult;

    if(l_destinationRegister == E_CPUREGISTER_PC) {
        cpuRestoreCpsr();
    } else {
        cpuUtilSetFlagsLogical(l_result);
    }
}

static void cpuOpcodeArmUmlal(uint32_t p_opcode) {
    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_destinationLowRegister = (p_opcode & 0x0000f000) >> 12;
    uint32_t l_destinationHighRegister = (p_opcode & 0x000f0000) >> 16;
    uint32_t l_firstOperandRegister = (p_opcode & 0x00000f00) >> 8;
    uint32_t l_secondOperandRegister = p_opcode & 0x0000000f;
    uint64_t l_firstOperand = (uint64_t)s_cpuRegisterR[l_firstOperandRegister];
    uint64_t l_secondOperand = s_cpuRegisterR[l_secondOperandRegister];
    uint64_t l_thirdOperandHigh = s_cpuRegisterR[l_destinationHighRegister];
    uint64_t l_thirdOperandLow = s_cpuRegisterR[l_destinationLowRegister];
    uint64_t l_thirdOperand = (
            (l_thirdOperandHigh << (uint64_t)32)
            | l_thirdOperandLow
        );

    uint64_t l_result = l_firstOperand * l_secondOperand + l_thirdOperand;
    uint32_t l_resultHigh = (uint32_t)(l_result >> 32);
    uint32_t l_resultLow = (uint32_t)l_result;

    s_cpuRegisterR[l_destinationHighRegister] = l_resultHigh;
    s_cpuRegisterR[l_destinationLowRegister] = l_resultLow;

    if(l_setFlags) {
        s_cpuFlagZ = (l_result == (uint64_t)0);
        s_cpuFlagN = (l_result & 0x8000000000000000) != 0;
    }
}

static void cpuOpcodeArmUmull(uint32_t p_opcode) {
    bool l_setFlags = (p_opcode & 0x00100000) != 0;
    uint32_t l_destinationLowRegister = (p_opcode & 0x0000f000) >> 12;
    uint32_t l_destinationHighRegister = (p_opcode & 0x000f0000) >> 16;
    uint32_t l_firstOperandRegister = (p_opcode & 0x00000f00) >> 8;
    uint32_t l_secondOperandRegister = p_opcode & 0x0000000f;
    uint64_t l_firstOperand = (uint64_t)s_cpuRegisterR[l_firstOperandRegister];
    uint64_t l_secondOperand = s_cpuRegisterR[l_secondOperandRegister];

    uint64_t l_result = l_firstOperand * l_secondOperand;
    uint32_t l_resultHigh = (uint32_t)(l_result >> 32);
    uint32_t l_resultLow = (uint32_t)l_result;

    s_cpuRegisterR[l_destinationHighRegister] = l_resultHigh;
    s_cpuRegisterR[l_destinationLowRegister] = l_resultLow;

    if(l_setFlags) {
        s_cpuFlagZ = (l_result == (uint64_t)0);
        s_cpuFlagN = (l_result & 0x8000000000000000) != 0;
    }
}

static void cpuOpcodeArmUnd(uint32_t p_opcode) {
    M_UNUSED_PARAMETER(p_opcode);

    cpuRaiseUnd();
}

static void cpuOpcodeThumbUnd(uint16_t p_opcode) {
    M_UNUSED_PARAMETER(p_opcode);

    cpuRaiseUnd();
}
