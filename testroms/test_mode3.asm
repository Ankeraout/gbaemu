include "header.inc"

code32

testStatus equ 0x03000000

macro imm32 reg, immediate {
    mov reg, (immediate) and 0x000000ff
    orr reg, (immediate) and 0x0000ff00
    orr reg, (immediate) and 0x00ff0000
    orr reg, (immediate) and 0xff000000
}

_start:
    ; Set mode 3
    mov r12, 0x04000000
    ldrh r0, [r12]
    bic r0, 0x0087
    orr r0, 0x0003
    orr r0, 0x0400
    strh r0, [r12]

    ; Plot a red dot
    mov r12, 0x06000000
    mov r0, 0x001f
    strh r0, [r12], 2

    ; Plot a green dot
    lsl r0, 5
    strh r0, [r12], 2

    ; Plot a blue dot
    lsl r0, 5
    strh r0, [r12], 2

    ; Halt
    b $
