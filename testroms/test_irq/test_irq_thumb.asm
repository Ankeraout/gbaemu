org 0x08000000

include "../header.inc"

b _start

include "../libtext/libtext.inc"

_start:
    bl libtext_init

    mov r1, 0x000000ff
    orr r1, 0x00007f00
    adr r0, msg0
    bl libtext_puts

    ; Install IRQ handler
    mov r0, 0x04000000
    sub r0, 4
    adr r1, irq_handler
    str r1, [r0]

    ; Enable V-blank interrupt
    mov r0, 0x04000000
    orr r0, 0x00000200
    mov r1, 0x00000001
    strh r1, [r0]

    ; Enable V-blank IRQ
    mov r0, 0x04000000
    orr r0, 0x00000004
    ldrh r1, [r0]
    orr r1, 0x0008
    strh r1, [r0]

    ; Set CPSR status
    mrs r0, cpsr
    bic r0, 0x40000000
    msr cpsr, r0

    ; Enable IME
    mov r0, 0x04000000
    orr r0, 0x00000200
    mov r1, 1

    adr r12, next + 1
    bx r12

code16
next:
    str r1, [r0, 8]

    ; Wait for the interrupt to occur
loop:
    bne loop

    adr r0, next2
    bx r0

align 4
code32
next2:
    ; Write the second message
    mov r1, 0x000003e0
    adr r0, msg1
    bl libtext_puts

    b $

irq_handler:
    mrs r0, spsr
    orr r0, 0x40000000
    msr spsr, r0
    mov r0, 0x04000000
    orr r0, 0x00000200
    eor r1, r1
    str r1, [r0, 8]
    bx r14

msg0 db "Waiting for interrupt...", 10, 0
msg1 db "Interrupt occurred!", 10, 0

align 4
