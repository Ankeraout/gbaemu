#!/usr/bin/env python3
#encoding: utf-8

def write32(f, v):
    ba = bytearray(4)
    ba[0] = v & 0x000000ff
    ba[1] = (v & 0x0000ff00) >> 8
    ba[2] = (v & 0x00ff0000) >> 16
    ba[3] = (v & 0xff000000) >> 24
    f.write(ba)

f = open('GBA.ROM', mode='wb')

for i in range(0, 4096):
    vh = i & 0xff0
    vl = i & 0x00f
    v = 0xe0000001 | (vh << 16) | (vl << 4)

    write32(f, v)

f.close()
