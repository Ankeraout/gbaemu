#!/usr/bin/env python3
#encoding: utf-8

print('    const opcodeCallback_t opcodeDecoderTable[] = {')

for i in range(0, 4096):
    if i == 4095:
        print('        NULL, // 0xfff')
    else:
        print('        NULL, // 0x%03x' % i)

print('    };')
