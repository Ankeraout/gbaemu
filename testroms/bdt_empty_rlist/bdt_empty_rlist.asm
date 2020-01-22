org 0x08000000

include '../header.inc'

_start:
    mov r0, 0x04000000
    mov r1, 0x00000400
    orr r1, 0x00000004
    strh r1, [r0]

    mov r12, 0

    ; Empty Rlist: R15 is loaded
test1:
    adr r0, testdata1
    dw 0xe8b00000 ;ldmia r0!, {}

test1nok:
    b test2

test1ok:
    orr r12, 0x000f

    ; Empty Rlist: Rb = Rb +/- 40h
test2:
    adr r1, testdata1 + 0x00000040
    teq r0, r1
    orreq r12, 0x01e0

    ; Writeback with Rb included in Rlist: Store OLD base if Rb is FIRST entry in Rlist
test3:
    mov r0, 0x03000000
    mov r1, r0
    dw 0xe8a00001 ; stmia r0!, {r0}
    teq r0, r1
    orreq r12, 0x3c00

    ; Writeback with Rb included in Rlist: Store NEW base if Rb is not FIRST entry in Rlist
test4:
    mov r0, 0x03000000
    mov r1, r0
    add r0, 8
    dw 0xe8a10003 ; stmia r1!, {r0, r1}
    teq r0, r1
    lsleq r12, 1

showResult:
    mov r0, 0x05000000
    strh r12, [r0]

halt:
    b halt

testdata1:
    dw test1ok
