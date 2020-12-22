namespace gbaemu.GBA {
    public class PPU : BusPeripheral {
        public GBA GBA {get; private set;}

        private byte[] palette;
        private byte[] vram;
        private byte[] oam;

        public delegate void OnFrame(uint[] color);
        public event OnFrame OnFrameEvent;
        public delegate void OnVblank();
        public event OnVblank OnVblankEvent;
        public delegate void OnHblank();
        public event OnHblank OnHblankEvent;

        private uint[] frameBuffer;
        private uint currentRow;
        private uint currentCol;
        private uint currentCycle;

        public PPU(GBA gba) {
            GBA = gba;
            frameBuffer = new uint[240 * 160];
            palette = new byte[1024];
            vram = new byte[98304];
            oam = new byte[1024];
        }

        public override byte Read8(uint addr) {
            switch((addr & 0x0f000000) >> 24) {
                case 0x5:
                    return palette[addr & 0x000003ff];

                case 0x6:
                    addr &= 0x0001ffff;

                    if(addr >= 0x00018000) {
                        addr &= 0x00017fff;
                    }

                    return vram[addr];

                case 0x7:
                    return oam[addr & 0x000003ff];
            }

            return 0xff;
        }

        public override void Write8(uint addr, byte val) {
            Write16(addr, (ushort)((val << 8) | val));
        }

        public new void Write16(uint addr, ushort val) {
            addr &= 0xfffffffe;

            switch((addr & 0x0f000000) >> 24) {
                case 0x5:
                    addr &= 0x000003fe;
                    palette[addr] = (byte)val;
                    palette[addr + 1] = (byte)(val >> 8);
                    break;
                    
                case 0x6:
                    addr &= 0x0001ffff;

                    if(addr >= 0x00018000) {
                        addr &= 0x00017fff;
                    }
                    
                    vram[addr] = (byte)val;
                    vram[addr + 1] = (byte)(val >> 8);
                    break;

                case 0x7:
                    addr &= 0x000003fe;
                    oam[addr] = (byte)val;
                    oam[addr + 1] = (byte)(val >> 8);
                    break;
            }
        }

        private uint ColorToRGB(ushort color) {
            uint blue = (uint)(color & 0x7c00) >> 10;
            uint green = (uint)(color & 0x03e0) >> 5;
            uint red = (uint)color & 0x001f;

            return 0xff000000 | (red << 3) | (green << 11) | (blue << 19);
        }

        private ushort GetPaletteColor(byte paletteIndex) {
            return Read16(0x05000000U | (uint)(paletteIndex << 1));
        }

        private void DrawMode0() {

        }

        private void DrawMode1() {
            
        }

        private void DrawMode2() {
            
        }

        private void DrawMode3() {
            for(int x = 0; x < 240; x++) {
                frameBuffer[currentRow * 240 + x] = ColorToRGB(Read16(0x06000000 | (uint)((currentRow * 240 + x) << 1)));
            }
        }

        private void DrawMode4() {
            IO.Register dispcnt = GBA.Io.GetRegister(0x04000000);

            uint offset = BitUtils.BitTest16(dispcnt.Value, 4) ? 0x0000a000U : 0x00000000U;

            for(int x = 0; x < 240; x++) {
                frameBuffer[currentRow * 240 + x] = ColorToRGB(GetPaletteColor(vram[currentRow * 240 + x + offset]));
            }
        }

        private void DrawMode5() {
            
        }

        private void DrawLine() {
            IO.Register dispcnt = GBA.Io.GetRegister(0x04000000);

            switch(dispcnt.Value & 0x07) {
                case 0: DrawMode0(); break;
                case 1: DrawMode1(); break;
                case 2: DrawMode2(); break;
                case 3: DrawMode3(); break;
                case 4: DrawMode4(); break;
                case 5: DrawMode5(); break;
                default: break;
            }
        }

        public void Cycle() {
            if((currentCycle & 0x03) == 0x03) {
                IO.Register dispstat = GBA.Io.GetRegister(0x04000004);

                currentCol++;

                if(currentCol == 308) {
                    currentCol = 0;
                    currentRow++;

                    dispstat.Value = BitUtils.BitClear16(dispstat.Value, 1);

                    if((dispstat.Value >> 8) == currentRow) {
                        dispstat.Value = BitUtils.BitSet16(dispstat.Value, 2);

                        if(BitUtils.BitTest16(dispstat.Value, 5)) {
                            GBA.SetInterruptFlag(1 << 2);
                        }
                    } else {
                        dispstat.Value = BitUtils.BitClear16(dispstat.Value, 2);
                    }

                    if(currentRow == 228) {
                        currentRow = 0;
                        currentCycle = 0;
                        dispstat.Value = BitUtils.BitClear16(dispstat.Value, 0);
                    } else if(currentRow == 160) {
                        dispstat.Value = BitUtils.BitSet16(dispstat.Value, 0);

                        if(BitUtils.BitTest16(dispstat.Value, 3)) {
                            GBA.SetInterruptFlag(1 << 0);
                            
                            if(OnVblankEvent != null) {
                                OnVblankEvent.Invoke();
                            }
                        }

                        OnFrameEvent(frameBuffer);
                    }

                    GBA.Io.GetRegister(0x04000006).Value = (ushort)currentRow;
                } else if(currentCol == 240) {
                    dispstat.Value = BitUtils.BitSet16(dispstat.Value, 1);

                    if(BitUtils.BitTest16(dispstat.Value, 4)) {
                        GBA.SetInterruptFlag(1 << 1);

                        if(OnHblankEvent != null) {
                            OnHblankEvent.Invoke();
                        }
                    }

                    if(currentRow < 160) {
                        DrawLine();
                    }
                }
            }

            currentCycle++;

            if(currentCycle == 280896) {
                currentCycle = 0;
            }
        }
    }
}
