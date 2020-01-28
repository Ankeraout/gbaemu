org 0x08000000

include '../header.inc'

b _start

include '../libtext.inc'

_start:
    bl libtext_init

    mov r0, 0
    mov r1, 0
    mov r2, 0xde000000
    orr r2, 0x00ad0000
    orr r2, 0x0000be00
    orr r2, 0x000000ef
    bl libtext_putHex

    b $
