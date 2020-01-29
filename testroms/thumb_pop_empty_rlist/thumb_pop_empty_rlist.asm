org 0x08000000

include '../header.inc'

b _start

include '../libtext/libtext.inc'

_start:
    bl libtext_init

    adr r0, test1_thumb
    orr r0, 1
    adr r14, test1_arm
    sub sp, 0x40
    mov r12, sp
    adr r1, test1_thumb_2
    str r1, [sp]
    mov r3, 0
    bx r0

test1_arm:
    mov r2, sp
    mov r0, 0
    mov r1, 0
    bl libtext_putHex2

    mov r2, r3
    mov r0, 0
    mov r1, 8
    bl libtext_putHex2
    
halt:
    b halt

code16
test1_thumb:
    dh 0xbc00 ; pop {}
    mov r3, 1
    bx r14

test1_thumb_2:
    mov r3, 2
    bx r14
