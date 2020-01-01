#!/usr/bin/env python3
#encoding: utf-8

variants = [
    "p",
    "pw",
    "pb",
    "pbw",
    "pu",
    "puw",
    "pub",
    "pubw"
]

functions = [
    "str",
    "ldr"
]

shifts = [
    "LSL_Rm_0",
    "LSL_Rm_1",
    "LSR_Rm_0",
    "LSR_Rm_1",
    "ASR_Rm_0",
    "ASR_Rm_1",
    "ROR_Rm_0",
    "ROR_Rm_1",
    "LSL_Rm_0",
    None,
    "LSR_Rm_0",
    None,
    "ASR_Rm_0",
    None,
    "ROR_Rm_0",
    None
]

tableIndex = 0x700

for i in range(0, 8):
    variant = variants[i]

    for j in range(0, 2):
        function = functions[j]

        for k in range(0, 16):
            shift = shifts[k]

            if shift == None:
                print('        NULL, // 0x%03x' % tableIndex)
            else:
                print('        gbaemu::gba::cpu::impl::sdt::opcode_%s%s_%s, // 0x%03x' % (function, variant, shift, tableIndex))

            tableIndex += 1
