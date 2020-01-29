org 0x08000000

include '../header.inc'

b _start

include '../libtext.inc'

_start:
    bl libtext_init

    adr r0, test1_thumb
    orr r0, 1
    adr r14, test1_arm
    mov r12, sp
    mov r1, 0
    bx r0

test1_arm:
    mov r2, sp
    mov r0, 0
    mov r1, 0
    bl libtext_putHex

    ldr r2, [sp]
    mov r0, 0
    mov r1, 8
    bl libtext_putHex
    
halt:
    b halt

code16
test1_thumb:
    dh 0xb400 ; push {}
    bx r14
