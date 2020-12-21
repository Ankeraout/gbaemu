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

    imm32 r0, main_thumb + 1
    imm32 r2, return_arm

    bx r0

main_thumb:
    code16
    swi 0x0d
    bx r2

return_arm:
    code32
    str r0, [sp, -4]!

    mov r12, testStatus
    mov r0, TEST_FINISHED
    strb r0, [r12]
