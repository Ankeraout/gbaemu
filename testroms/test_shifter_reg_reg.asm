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
    mov r1, 1
    mov r2, 0
    mov r3, 1
    mov r4, 32
    mov r5, 33

    mov r12, testStatus
    mov r0, TEST_STARTED
    strb r0, [r12]

    lsls r0, r1, r2
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    lsls r0, r1, r3
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    lsls r0, r1, r4
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    lsls r0, r1, r5
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    lsrs r0, r1, r2
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    lsrs r0, r1, r3
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    lsrs r0, r1, r4
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    lsrs r0, r1, r5
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    asrs r0, r1, r2
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    asrs r0, r1, r3
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    asrs r0, r1, r4
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    asrs r0, r1, r5
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    rors r0, r1, r2
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    rors r0, r1, r3
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    rors r0, r1, r4
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    rors r0, r1, r5
    str r0, [sp, -4]!
    mrs r0, cpsr
    and r0, 0xf0000000
    str r0, [sp, -4]!

    mrs r0, cpsr
    bic r0, 0xf0000000
    orr r0, 0x60000000
    msr cpsr, r0
    imm32 r0, 0x002d99b6
    imm32 r1, 0x814fb596
    mov r2, 0x1c
    orrs r4, r0, r1, lsl r2
    str r4, [sp, -4]!
    mrs r0, cpsr
    str r0, [sp, -4]!

    mov r12, testStatus
    mov r0, TEST_FINISHED
    strb r0, [r12]
