include "header.inc"

code32

_start:
    mov r0, 0xde000000
    orr r0, 0x00ad0000
    orr r0, 0x0000be00
    orr r0, 0x000000ef

    mov r1, 0x03000000
    str r0, [r1 + 0x300]

    b $
