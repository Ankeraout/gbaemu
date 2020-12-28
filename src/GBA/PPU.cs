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

        private void SortLayers(out int[] layers) {
            int[] intLayers = new int[4] {3, 2, 1, 0};
            int[] priorities = new int[4];

            for(int i = 0; i < 4; i++) {
                priorities[i] = GBA.Io.GetRegister(0x04000008 + ((uint)i << 1)).Value & 0x0003;
            }
            
            for(int i = 0; i < 3; i++) {
                int layerPriority = priorities[i];
                int maxPriority = layerPriority;
                int maxPriorityIndex = i;

                for(int j = i + 1; j < 4; j++) {
                    int currentLayerPriority = priorities[j];

                    if(currentLayerPriority > maxPriority) {
                        maxPriorityIndex = j;
                        maxPriority = currentLayerPriority;
                    }
                }

                int exchange = intLayers[i];
                intLayers[i] = intLayers[maxPriorityIndex];
                intLayers[maxPriorityIndex] = exchange;
            }

            layers = intLayers;
        }

        private void DrawLayer(int layer) {
            IO.Register bgcnt = GBA.Io.GetRegister(0x04000008 + ((uint)layer << 1));

            uint hofs = GBA.Io.GetRegister(0x04000010 + ((uint)layer << 2)).Value;
            uint vofs = GBA.Io.GetRegister(0x04000012 + ((uint)layer << 2)).Value;
            uint mapBase = (uint)(bgcnt.Value & 0x1f00) << 3;
            uint tileBase = (uint)(bgcnt.Value & 0x000c) << 12;
            uint yLayer = currentRow + vofs;

            uint mapOffsetY = 0x00000000;

            if(BitUtils.BitTest16(bgcnt.Value, 15)) {
                yLayer &= 0x000001ff;

                if(((bgcnt.Value & 0xc000) == 0xc000) && (yLayer >= 0x100)) {
                    mapOffsetY = 0x00001000;
                }
            } else {
                yLayer &= 0x000000ff;
            }

            uint yChunk = yLayer & 0x000000ff;

            uint yMap = yChunk >> 3;
            uint yTile = yChunk & 7;

            for(uint x = 0; x < 240; x++) {
                uint xLayer = x + hofs;

                uint mapOffsetX = 0x00000000;

                if(BitUtils.BitTest16(bgcnt.Value, 14)) {
                    xLayer &= 0x000001ff;

                    if(BitUtils.BitTest16(bgcnt.Value, 14) && (xLayer >= 0x100)) {
                        mapOffsetX = 0x00000800;
                    }
                } else {
                    xLayer &= 0x000000ff;
                }

                uint xChunk = xLayer & 0x000000ff;

                uint xMap = xChunk >> 3;
                uint xTile = xChunk & 7;

                uint mapOffset = mapOffsetX + mapOffsetY;

                uint mapAddress = mapOffset + (yMap << 6) + (xMap << 1);
                uint mapValue = vram[mapAddress] | ((uint)vram[mapAddress + 1] << 8);

                uint tileNumber = mapValue & 0x03ff;
                bool flipHorizontal = BitUtils.BitTest32(mapValue, 10);
                bool flipVertical = BitUtils.BitTest32(mapValue, 11);

                uint yTileReal = flipVertical ? 7 - yTile : yTile;
                uint xTileReal = flipHorizontal ? 7 - xTile : xTile;

                uint colorAddress;

                if(BitUtils.BitTest16(bgcnt.Value, 7)) {
                    uint tileDataAddress = (tileNumber << 6) | (yTileReal << 3) | xTileReal;
                    uint tileValue = vram[tileBase + tileDataAddress];
                    colorAddress = tileValue << 1;
                } else {
                    uint tileDataAddress = (tileNumber << 5) | (yTileReal << 2) | (xTileReal >> 1);
                    
                    uint tileValue = vram[tileBase + tileDataAddress];

                    if(BitUtils.BitTest32(xTileReal, 0) ^ flipHorizontal) {
                        tileValue >>= 4;
                    } else {
                        tileValue &= 0x0f;
                    }

                    uint paletteNumber = mapValue >> 12;

                    colorAddress = (paletteNumber << 5) | (tileValue << 1);
                }

                ushort pixelColor = (ushort)(palette[colorAddress] | (palette[colorAddress + 1] << 8));

                frameBuffer[currentRow * 240 + x] = ColorToRGB(pixelColor);
            }
        }

        private void DrawMode0() {
            int[] layers;

            SortLayers(out layers);
            
            IO.Register dispcnt = GBA.Io.GetRegister(0x04000000);

            for(int i = 0; i < 4; i++) {
                if((dispcnt.Value & (1 << (8 + i))) != 0) {
                    DrawLayer(i);
                }
            }
        }

        private void DrawMode1() {
            DrawMode0();
        }

        private void DrawMode2() {
            DrawMode0();
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
            IO.Register dispcnt = GBA.Io.GetRegister(0x04000000);

            uint offset = BitUtils.BitTest16(dispcnt.Value, 4) ? 0x00005000U : 0x00000000U;

            uint currentRow = this.currentRow - 16;

            if(currentRow < 128) {
                for(int x = 0; x < 160; x++) {
                    frameBuffer[currentRow * 240 + x + 40] = ColorToRGB(Read16(0x06000000 | (uint)((currentRow * 160 + x + offset) << 1)));
                }
            }
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
        }
    }
}
