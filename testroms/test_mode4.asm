include "header.inc"

code32

macro imm32 reg, immediate {
    mov reg, (immediate) and 0x000000ff
    orr reg, (immediate) and 0x0000ff00
    orr reg, (immediate) and 0x00ff0000
    orr reg, (immediate) and 0xff000000
}

_start:
    ; Set mode 4
    mov r12, 0x04000000
    ldrh r0, [r12]
    bic r0, 0x0087
    orr r0, 0x0004
    orr r0, 0x0400
    strh r0, [r12]

    ; Initialize palette
    mov r0, 0x05000000
    mov r1, 0x00007c00
    strh r1, [r0, 2]!
    lsr r1, 5
    strh r1, [r0, 2]!
    lsr r1, 5
    strh r1, [r0, 2]!

    ; Plot a red dot
    mov r12, 0x06000000
    imm32 r0, 0x01020300
    str r0, [r12]

    ; Halt
    b $
