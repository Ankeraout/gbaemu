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
    ldr r0, [op1]
    ldr r1, [op2]
    mov r2, r0
    mov r3, r1

    tst r0, r1

    ; Check if r0 was changed
    teq r0, r2
    bne testFailed

    ; Check if r1 was changed
    teq r1, r3
    bne testFailed

testPassed:
    mov r12, 0x03e0
    b showResult

testFailed:
    mov r12, 0x001f

showResult:
    mov r0, 0x05000000
    strh r12, [r0]

halt:
    b halt

op1:
    dw 0x8b849c34

op2:
    dw 0xdf7400c4
