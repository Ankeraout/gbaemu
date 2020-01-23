org 0x08000000

include '../header.inc'

_start:
    mov r0, 0x04000000
    mov r1, 0x00000400
    orr r1, 0x00000004
    strh r1, [r0]
    
    mov r12, 0
    mov r11, 0x03000000

test1:
    ldr r0, [deadbeef]
    str r0, [r11]
    str r0, [r11, 4]
    str r0, [r11, 0x40]

    dw 0xe98b0000 ; stmib r11, {}

    ldr r1, [r11]
    teq r0, r1
    orrne r12, 0x001f

    ldr r1, [r11, 4]
    teq r0, r1
    orrne r12, 0x03e0

    ldr r1, [r11, 0x40]
    teq r0, r1
    orrne r12, 0x7c00

showResult:
    mov r0, 0x05000000
    strh r12, [r0]

halt:
    b halt

deadbeef:
    dw 0xdeadbeef
