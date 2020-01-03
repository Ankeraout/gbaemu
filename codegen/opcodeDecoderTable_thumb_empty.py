#!/usr/bin/env python3
#encoding: utf-8

print('    const thumbOpcodeCallback_t opcodeDecoderTable[] = {')

for i in range(0, 1024):
    if i == 1023:
        print('        NULL, // 0xffc')
    else:
        print('        NULL, // 0x%03x' % (i << 2))

print('    };')
