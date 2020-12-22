include "header.inc"

code32

macro imm32 reg, immediate {
    mov reg, (immediate) and 0x000000ff
    orr reg, (immediate) and 0x0000ff00
    orr reg, (immediate) and 0x00ff0000
    orr reg, (immediate) and 0xff000000
}

_start:
    ; Init LCD
    mov r0, 0x04000000
    mov r1, 0x0400
    orr r1, 0x0003
    strh r1, [r0]

    ; Init colors
    adr r0, colors
    mov r1, 0x02000000
    ldr r2, [r0], 4
    str r2, [r1], 4
    ldr r2, [r0]
    str r2, [r1]

    ; Perform DMA 3
    mov r0, 0x04000000
    orr r0, 0x000000d4
    mov r1, 0x02000000
    str r1, [r0]
    mov r1, 0x06000000
    str r1, [r0, 4]
    mov r1, 0x9600
    strh r1, [r0, 8]
    mov r1, 0x8100
    strh r1, [r0, 10]

    ; Perform DMA 2
    mov r0, 0x04000000
    orr r0, 0x000000c8
    mov r1, 0x02000000
    orr r1, 0x00000002
    str r1, [r0]
    mov r1, 0x06000000
    str r1, [r0, 4]
    mov r1, 0x0000
    strh r1, [r0, 8]
    mov r1, 0x8100
    strh r1, [r0, 10]

    ; Perform DMA 1
    mov r0, 0x04000000
    orr r0, 0x000000bc
    mov r1, 0x02000000
    orr r1, 0x00000004
    str r1, [r0]
    mov r1, 0x06000000
    str r1, [r0, 4]
    mov r1, 0x2a00
    orr r1, 0x00aa
    strh r1, [r0, 8]
    mov r1, 0x8100
    strh r1, [r0, 10]

    ; Perform DMA 0
    mov r0, 0x04000000
    orr r0, 0x000000b0
    mov r1, 0x02000000
    orr r1, 0x00000006
    str r1, [r0]
    mov r1, 0x06000000
    str r1, [r0, 4]
    mov r1, 0x1500
    orr r1, 0x0055
    strh r1, [r0, 8]
    mov r1, 0x8100
    strh r1, [r0, 10]

    b $

colors:
    dh 0x7fe0
    dh 0x03e0
    dh 0x03ff
    dh 0x001f
