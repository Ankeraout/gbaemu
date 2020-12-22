namespace gbaemu.GBA {
    public class IO : IBusPeripheral {
        public GBA GBA {get; private set;}
        public delegate void WriteCallback(uint address, ushort value);

        public class Register {
            public WriteCallback WriteCallback;
            public ushort Value;
            public ushort WriteMask;
            public ushort ReadMask;

            public Register(ushort value, WriteCallback writeCallback, ushort readMask, ushort writeMask) {
                Value = value;
                WriteCallback = writeCallback;
                ReadMask = readMask;
                WriteMask = writeMask;
            }
        }

        private Register internalMemoryControlHigh;
        private Register internalMemoryControlLow;
        private Register nullRegister;

        private Register[] registers;

        public IO(GBA gba) {
            GBA = gba;
            InitRegisters();
        }

        private void InitRegisters() {
            registers = new Register[512];

            internalMemoryControlHigh = new Register(0x0000, null, 0xffff, 0xffff);
            internalMemoryControlLow = new Register(0x0000, null, 0xffff, 0xffff);
            nullRegister = new Register(0x0000, null, 0x0000, 0xffff);

            for(int i = 0; i < 512; i++) {
                switch(0x04000000 | (i << 1)) {
                    case 0x04000000: registers[i] = new Register(0x0000, null, 0xffff, 0xffff); break; // DISPCNT
                    case 0x04000002: registers[i] = new Register(0x0000, null, 0xffff, 0xffff); break; // GREENSWP
                    case 0x04000004: registers[i] = new Register(0x0000, null, 0xffff, 0xffff); break; // DISPSTAT
                    case 0x04000006: registers[i] = new Register(0x0000, null, 0xffff, 0x0000); break; // VCOUNT
                    case 0x04000008: registers[i] = new Register(0x0000, null, 0xffff, 0xffff); break; // BG0CNT
                    case 0x0400000a: registers[i] = new Register(0x0000, null, 0xffff, 0xffff); break; // BG1CNT
                    case 0x0400000c: registers[i] = new Register(0x0000, null, 0xffff, 0xffff); break; // BG2CNT
                    case 0x0400000e: registers[i] = new Register(0x0000, null, 0xffff, 0xffff); break; // BG3CNT
                    case 0x04000010: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG0HOFS
                    case 0x04000012: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG0VOFS
                    case 0x04000014: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG1HOFS
                    case 0x04000016: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG1VOFS
                    case 0x04000018: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG2HOFS
                    case 0x0400001a: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG2VOFS
                    case 0x0400001c: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG3HOFS
                    case 0x0400001e: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG3VOFS
                    case 0x04000020: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG2PA
                    case 0x04000022: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG2PB
                    case 0x04000024: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG2PC
                    case 0x04000026: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG2PD
                    case 0x04000028: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG2X_L
                    case 0x0400002a: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG2X_H
                    case 0x0400002c: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG2Y_L
                    case 0x0400002e: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG2Y_H
                    case 0x04000030: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG3PA
                    case 0x04000032: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG3PB
                    case 0x04000034: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG3PC
                    case 0x04000036: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG3PD
                    case 0x04000038: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG3X_L
                    case 0x0400003a: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG3X_H
                    case 0x0400003c: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG3Y_L
                    case 0x0400003e: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BG3Y_H
                    case 0x04000040: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // WIN0H
                    case 0x04000042: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // WIN1H
                    case 0x04000044: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // WIN0V
                    case 0x04000046: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // WIN1V
                    case 0x04000048: registers[i] = new Register(0x0000, null, 0xffff, 0xffff); break; // WININ
                    case 0x0400004a: registers[i] = new Register(0x0000, null, 0xffff, 0xffff); break; // WINOUT
                    case 0x0400004c: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // MOSAIC
                    case 0x04000050: registers[i] = new Register(0x0000, null, 0xffff, 0xffff); break; // BLDCNT
                    case 0x04000052: registers[i] = new Register(0x0000, null, 0xffff, 0xffff); break; // BLDALPHA
                    case 0x04000054: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // BLDY
                    case 0x040000b0: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // DMA0SAD_L
                    case 0x040000b2: registers[i] = new Register(0x0000, null, 0x0000, 0x07ff); break; // DMA0SAD_H
                    case 0x040000b4: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // DMA0DAD_L
                    case 0x040000b6: registers[i] = new Register(0x0000, null, 0x0000, 0x07ff); break; // DMA0DAD_H
                    case 0x040000b8: registers[i] = new Register(0x0000, null, 0x0000, 0x3fff); break; // DMA0CNT_L
                    case 0x040000ba: registers[i] = new Register(0x0000, GBA.Dma.Channel0.CntHWriteCallback, 0xffe0, 0xffe0); break; // DMA0CNT_H
                    case 0x040000bc: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // DMA1SAD_L
                    case 0x040000be: registers[i] = new Register(0x0000, null, 0x0000, 0x07ff); break; // DMA1SAD_H
                    case 0x040000c0: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // DMA1DAD_L
                    case 0x040000c2: registers[i] = new Register(0x0000, null, 0x0000, 0x07ff); break; // DMA1DAD_H
                    case 0x040000c4: registers[i] = new Register(0x0000, null, 0x0000, 0x3fff); break; // DMA1CNT_L
                    case 0x040000c6: registers[i] = new Register(0x0000, GBA.Dma.Channel1.CntHWriteCallback, 0xffe0, 0xffe0); break; // DMA1CNT_H
                    case 0x040000c8: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // DMA2SAD_L
                    case 0x040000ca: registers[i] = new Register(0x0000, null, 0x0000, 0x07ff); break; // DMA2SAD_H
                    case 0x040000cc: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // DMA2DAD_L
                    case 0x040000ce: registers[i] = new Register(0x0000, null, 0x0000, 0x07ff); break; // DMA2DAD_H
                    case 0x040000d0: registers[i] = new Register(0x0000, null, 0x0000, 0x3fff); break; // DMA2CNT_L
                    case 0x040000d2: registers[i] = new Register(0x0000, GBA.Dma.Channel2.CntHWriteCallback, 0xffe0, 0xffe0); break; // DMA2CNT_H
                    case 0x040000d4: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // DMA3SAD_L
                    case 0x040000d6: registers[i] = new Register(0x0000, null, 0x0000, 0x0fff); break; // DMA3SAD_H
                    case 0x040000d8: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // DMA3DAD_L
                    case 0x040000da: registers[i] = new Register(0x0000, null, 0x0000, 0x07ff); break; // DMA3DAD_H
                    case 0x040000dc: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break; // DMA3CNT_L
                    case 0x040000de: registers[i] = new Register(0x0000, GBA.Dma.Channel3.CntHWriteCallback, 0xffe0, 0xffe0); break; // DMA3CNT_H
                    case 0x04000130: registers[i] = new Register(0xffff, null, 0x03ff, 0x0000); break; // KEYINPUT
                    case 0x04000132: registers[i] = new Register(0x0000, null, 0xc3ff, 0xc3ff); break; // KEYCNT
                    case 0x04000200: registers[i] = new Register(0x0000, null, 0x3fff, 0x3fff); break; // IE
                    case 0x04000202: registers[i] = new Register(0x0000, GBA.WriteToIF, 0x3fff, 0x0000); break; // IF
                    case 0x04000208: registers[i] = new Register(0x0000, null, 0x0001, 0x0001); break; // IME
                    default: registers[i] = new Register(0x0000, null, 0x0000, 0xffff); break;
                }
            }
        }

        private uint ConvertAddress(uint addr) {
            if((addr & 0x0000ffff) == 0x00000800) {
                return 0x04000800;
            } else {
                return addr;
            }
        }

        public Register GetRegister(uint addr) {
            if(addr == 0x04000800) {
                return internalMemoryControlLow;
            } else if(addr == 0x04000802) {
                return internalMemoryControlHigh;
            } else if(addr < 0x04000400) {
                return registers[(addr & 0x000003ff) >> 1];
            } else {
                return nullRegister;
            }
        }

        public void Write8(uint addr, byte val) {
            ushort value = Read16(addr);

            if(BitUtils.BitTest32(addr, 0)) {
                value &= 0x00ff;
                value |= (ushort)(val << 8);
            } else {
                value &= 0xff00;
                value |= val;
            }

            Write16(addr, val);
        }

        public void Write16(uint addr, ushort val) {
            Register register = GetRegister(ConvertAddress(addr));
            
            register.Value &= (ushort)~register.WriteMask;
            register.Value |= (ushort)(val & register.WriteMask);

            if(register.WriteCallback != null) {
                register.WriteCallback(addr, val);
            }
        }

        public void Write32(uint addr, uint val) {
            Write16(addr, (ushort)val);
            Write16(addr + 2, (ushort)(val >> 16));
        }

        public byte Read8(uint addr) {
            ushort value = Read16(addr);

            if(BitUtils.BitTest32(addr, 0)) {
                return (byte)(value >> 8);
            } else {
                return (byte)value;
            }
        }

        public ushort Read16(uint addr) {
            Register register = GetRegister(ConvertAddress(addr));
            return (ushort)(register.Value & register.ReadMask);
        }

        public uint Read32(uint addr) {
            return (uint)(Read16(addr) | (Read16(addr + 2) << 16));
        }
    }
}
