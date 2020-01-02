#!/usr/bin/env python3
#encoding: utf-8

print('    const thumbOpcodeCallback_t opcodeDecoderTable[] = {')

for i in range(0, 256):
    if i == 255:
        print('        NULL, // 0xff')
    else:
        print('        NULL, // 0x%02x' % i)

print('    };')
