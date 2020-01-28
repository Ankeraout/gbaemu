org 0x08000000

include '../header.inc'

_start:
    mov r0, 0x04000000
    mov r1, 0x00000400
    orr r1, 0x00000004
    strh r1, [r0]

    adr r0, test1_thumb
    orr r0, 1
    adr r14, test1_arm
    mov r12, sp
    mov r1, 0
    bx r0

test1_arm:
    teq sp, r12
    orreq r1, 0x03e0
    orrne r1, 0x001f

displayResult:
    mov r0, 0x05000000
    strh r1, [r0]
    
halt:
    b halt

code16
test1_thumb:
    dh 0xb400 ; push {}
    bx r14
