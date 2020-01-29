libtext_init:
    push {r0, r1}
    
    mov r0, 0x04000000
    mov r1, 0x00000400
    orr r1, 0x00000003
    strh r1, [r0]

    mov r0, 0
    mov r1, 0x03000000 ; cur_x
    str r0, [r1]
    add r1, 4 ; cur_y
    str r0, [r1]

    pop {r0, r1}
    bx r14

; args:
; r0 - str
; r1 - color
libtext_puts:
    push {r0-r2, r14}
    mov r2, r0

libtext_puts_loop:
    ldrb r0, [r2]
    add r2, 1
    tst r0, r0
    beq libtext_puts_loop_end
    bl libtext_putc
    b libtext_puts_loop

libtext_puts_loop_end:
    pop {r0-r2, r14}
    bx r14

; args:
; r0 - character to print
; r1 - color
libtext_putc:
    push {r0-r7}

    ; If character is NUL, do nothing
    tst r0, r0
    beq libtext_putc_return

    ; If character is CR, reset cur_x
    cmp r0, 13
    beq libtext_putc_cr

    ; If character is LF, start a new line
    cmp r0, 10
    beq libtext_putc_lf

    ; If character is TAB, do a tab
    cmp r0, 9
    beq libtext_putc_tab

    ; Compute the address of the first pixel of the character
    mov r5, 0x03000000
    add r6, r5, 4
    ldr r5, [r5]
    ldr r6, [r6]
    lsl r5, 3
    lsl r6, 3
    mov r4, 480
    umull r3, r7, r4, r6
    add r3, r5, lsl 1
    orr r3, 0x06000000

    ; Compute the address of the character data
    adr r4, libtext_fontData
    add r4, r0, lsl 3

    mov r2, 0

libtext_putc_loop0:
    ldrb r5, [r4]
    add r4, 1
    mov r0, 0

libtext_putc_loop1:
    ands r6, r5, 0x80
    lsl r5, 1
    strhne r1, [r3]
    add r3, 2
    add r0, 1
    cmp r0, 8
    bne libtext_putc_loop1

    add r3, 464
    add r2, 1
    cmp r2, 8
    bne libtext_putc_loop0

    ; Increment cursor position
    mov r1, 0x03000000
    ldr r0, [r1]
    add r0, 1
    str r0, [r1]

libtext_putc_check_cursor:
    ; Load cursor position
    mov r0, 0x03000000
    ldr r0, [r0]
    add r1, r0, 4
    ldr r1, [r1]

    ; If x >= 30, go to next line
    cmp r0, 30
    addge r1, 1
    movge r0, 0

    ; If y >= 20, go back to top
    cmp r1, 20
    movge r1, 0

libtext_putc_return:
    pop {r0-r7}
    bx r14

libtext_putc_cr:
    mov r0, 0x03000000
    mov r2, 0
    str r2, [r0]
    b libtext_putc_return

libtext_putc_lf:
    mov r1, 0x03000000
    mov r0, 0
    str r0, [r1]
    add r1, 4
    ldr r0, [r1]
    add r0, 1
    str r0, [r1]
    b libtext_putc_check_cursor

libtext_putc_tab:
    mov r1, 0x03000000
    ldr r0, [r1]
    and r2, r0, 0x00000003
    add r0, 4
    sub r0, r2
    str r0, [r1]
    b libtext_putc_check_cursor

; args:
; r0 - Hexadecimal value
; r1 - color
libtext_putHex:
    push {r0-r3, r14}

    mov r2, r0
    mov r3, 32

libtext_putHex_loop:
    sub r3, 4
    mov r0, r2, lsr r3
    and r0, 0x0000000f
    cmp r0, 0x0000000a
    addge r0, 'A' - 10
    addlt r0, '0'
    bl libtext_putc
    tst r3, r3
    bne libtext_putHex_loop

    pop {r0-r3, r14}
    bx r14

; args:
; r0 - x
; r1 - y
; r2 - value to print
libtext_putHex2:
    push {r0-r3, r14}

    mov r3, r2
    lsr r2, r3, 28
    bl libhex_putChar
    add r0, 8
    lsr r2, r3, 24
    bl libhex_putChar
    add r0, 8
    lsr r2, r3, 20
    bl libhex_putChar
    add r0, 8
    lsr r2, r3, 16
    bl libhex_putChar
    add r0, 8
    lsr r2, r3, 12
    bl libhex_putChar
    add r0, 8
    lsr r2, r3, 8
    bl libhex_putChar
    add r0, 8
    lsr r2, r3, 4
    bl libhex_putChar
    add r0, 8
    mov r2, r3
    bl libhex_putChar

    pop {r0-r3, r14}
    bx r14

; args:
; r0 - x
; r1 - y
; r2 - char
;
; allocated:
; r3 - vram ptr
; r4 - font ptr
; r5 - font value
; r6 - r5 and 1
libhex_putChar:
    push {r0-r6}

    mov r3, r1
    mov r4, 0
    mov r5, 480
    umull r3, r4, r1, r5
    add r3, r0, lsl 1
    orr r3, 0x06000000

    adr r4, libtext_fontData
    and r2, 0x0000000f
    cmp r2, 0x0000000a
    addge r2, 'A' - 10
    addlt r2, '0'
    add r4, r2, lsl 3
    
    mov r1, 0 ; y

libhex_putChar_loop0:
    ldrb r5, [r4]
    add r4, 1
    mov r0, 0

libhex_putChar_loop1:
    ands r6, r5, 0x80
    movne r6, 0x00007f00
    orrne r6, 0x000000ff
    moveq r6, 0x00000000
    lsl r5, 1
    strhne r6, [r3]
    add r3, 2
    add r0, 1
    cmp r0, 8
    bne libhex_putChar_loop1

    add r3, 464
    add r1, 1
    cmp r1, 8
    bne libhex_putChar_loop0

    pop {r0-r6}
    bx r14

libtext_fontData:
    file '../libtext/font.raw'
