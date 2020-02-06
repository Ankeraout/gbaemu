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

    adr r12, next + 1
    bx r12

code16
next:
    swi 0x0d

    adr r1, next2
    bx r1

align 4
code32
next2:
    mov r1, 0x000000ff
    orr r1, 0x00007f00
    mov r2, r0
    adr r0, msg1
    bl libtext_puts

    mov r0, r2
    bl libtext_putHex

    ldr r1, [expectedChecksum]
    teq r0, r1

    bne invalid

valid:
    mov r1, 0x000003e0
    adr r0, msg2
    bl libtext_puts
    b halt

invalid:
    mov r1, 0x0000001f
    adr r0, msg3
    bl libtext_puts

halt:
    b $

msg0 db "Calling GetBiosChecksum...", 10, 0
msg1 db "Result: 0x", 0
msg2 db 10, "Valid checksum", 10, 0
msg3 db 10, "Invalid checksum", 10, 0

align 4
expectedChecksum dw 0xbaae187f
