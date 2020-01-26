org 0x08000000

include '../header.inc'

_start:
    mrs r0, cpsr
    bic r0, 0x0000001f
    msr cpsr, r0

    mov r0, 0x05000000
    mov r1, 0x03e0
    str r1, [r0]

halt:
    b halt
