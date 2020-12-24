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
    mov r12, testStatus
    mov r0, TEST_STARTED
    strb r0, [r12]

    mov r12, 0x02000000

    str pc, [r12]
    mov r0, pc
    ldr r1, [r12]
    str r0, [sp, -4]!
    str r1, [sp, -4]!

    mov r12, testStatus
    mov r0, TEST_FINISHED
    strb r0, [r12]
