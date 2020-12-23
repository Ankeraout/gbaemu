include "header.inc"

code32

testStatus equ 0x03000000

macro imm32 reg, immediate {
    mov reg, (immediate) and 0x000000ff
    orr reg, (immediate) and 0x0000ff00
    orr reg, (immediate) and 0x00ff0000
    orr reg, (immediate) and 0xff000000
}

_start:
    ; Set mode 0
    mov r12, 0x04000000
    mov r0, 0x00000100
    strh r0, [r12]

    ; Set BG0CNT (tile map = 0x06000000, tile data = 0x06004000)
    mov r0, 0x0000c000
    orr r0, 0x00000004
    strh r0, [r12, 8]

    ; DMA 3 copy tile data
    adr r0, tileData
    str r0, [r12, 0xd4] ; sad = tileData
    mov r0, 0x06000000
    orr r0, 0x00004000
    str r0, [r12, 0xd8] ; dad = tile data
    mov r0, 32
    strh r0, [r12, 0xdc] ; cnt_l = 32 * 16 bits = 64 bytes
    mov r0, 0x8000
    strh r0, [r12, 0xde] ; cnt_h = enable, sac = increment, dac = increment, 16 bits

    ; DMA 3 copy palette data
    adr r0, paletteData
    str r0, [r12, 0xd4] ; sad = tileData
    mov r0, 0x05000000
    str r0, [r12, 0xd8] ; dad = palette data
    mov r0, 9
    strh r0, [r12, 0xdc] ; cnt_l = 9 * 16 bits = 18 bytes
    mov r0, 0x8000
    strh r0, [r12, 0xde] ; cnt_h = enable, sac = increment, dac = increment, 16 bits

    ; Scroll to the tile
    mov r0, 256
    strh r0, [r12, 0x10]
    strh r0, [r12, 0x12]

    ; Put tile on screen
    mov r12, 0x06000000
    orr r12, 0x00001800
    mov r0, 0x0001
    strh r0, [r12, 0x40]

    ; Halt
    b $

tileData:
tile0:
    db 32 dup 0

tile1:
    db 0x10, 0x11, 0x11, 0x01
    db 0x02, 0x00, 0x00, 0x20
    db 0x03, 0x00, 0x00, 0x30
    db 0x04, 0x00, 0x00, 0x40
    db 0x05, 0x00, 0x00, 0x50
    db 0x06, 0x00, 0x00, 0x60
    db 0x07, 0x00, 0x00, 0x70
    db 0x80, 0x88, 0x88, 0x08

paletteData:
color0: dh 0x0000
color1: dh 0x001f
color2: dh 0x03ff
color3: dh 0x03e0
color4: dh 0x7fe0
color5: dh 0x7c00
color6: dh 0x7c1f
color7: dh 0x001f
color8: dh 0x03ff
