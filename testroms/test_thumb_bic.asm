include "header.inc"

code32

testStatus equ 0x03000000

macro imm32 reg, immediate {
    mov reg, (immediate) and 0x000000ff
    orr reg, (immediate) and 0x0000ff00
    orr reg, (immediate) and 0x00ff0000
    orr reg, (immediate) and 0xff000000
}

TEST_STARTED equ 0x00000011
TEST_FINISHED equ 0x00000022

_start:
    ; Notify the supervisor that the test started
    mov r12, testStatus
    mov r0, TEST_STARTED
    strb r0, [r12]

    ; Prepare operands
    mov r0, 0x00040000
    mov r1, 0x00000003
    orr r0, r1

    ; Set thumb mode
    adr r14, thumbMode
    orr r14, 1
    bx r14

thumbMode:
    code16

    bics r0, r1
    push {r0}

    ; Notify the supervisor that the test finished
    mov r0, r12
    mov r1, TEST_FINISHED
    strb r1, [r0]

    ; Halt
    b $
