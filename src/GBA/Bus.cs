namespace gbaemu.GBA {
    public class Bus : BusPeripheral {
        private GBA gba;

        public Bus(GBA _gba) {
            gba = _gba;
        }

        public override byte Read8(uint addr) {
            switch((addr >> 24) & 0xf) {
                case 0x0:
                case 0x1:
                    return gba.Bios.Read8(addr);

                case 0x2:
                    return gba.Ewram.Read8(addr);

                case 0x3:
                    return gba.Iwram.Read8(addr);

                case 0x4:
                    return gba.Io.Read8(addr);

                case 0x5:
                case 0x6:
                case 0x7:
                    return gba.Ppu.Read8(addr);

                case 0x8:
                case 0x9:
                case 0xa:
                case 0xb:
                case 0xc:
                case 0xd:
                case 0xe:
                case 0xf:
                    return gba.Cartridge.Read8(addr);
            }

            return 0x00;
        }

        public new ushort Read16(uint addr) {
            addr &= 0xfffffffe;

            switch((addr >> 24) & 0xf) {
                case 0x0:
                case 0x1:
                    return gba.Bios.Read16(addr);

                case 0x2:
                    return gba.Ewram.Read16(addr);

                case 0x3:
                    return gba.Iwram.Read16(addr);

                case 0x4:
                    return gba.Io.Read16(addr);

                case 0x5:
                case 0x6:
                case 0x7:
                    return gba.Ppu.Read16(addr);

                case 0x8:
                case 0x9:
                case 0xa:
                case 0xb:
                case 0xc:
                case 0xd:
                case 0xe:
                case 0xf:
                    return gba.Cartridge.Read16(addr);
            }

            return 0x0000;
        }

        public new uint Read32(uint addr) {
            addr &= 0xfffffffc;

            switch((addr >> 24) & 0xf) {
                case 0x0:
                case 0x1:
                    return gba.Bios.Read32(addr);

                case 0x2:
                    return gba.Ewram.Read32(addr);

                case 0x3:
                    return gba.Iwram.Read32(addr);

                case 0x4:
                    return gba.Io.Read32(addr);

                case 0x5:
                case 0x6:
                case 0x7:
                    return gba.Ppu.Read32(addr);

                case 0x8:
                case 0x9:
                case 0xa:
                case 0xb:
                case 0xc:
                case 0xd:
                case 0xe:
                case 0xf:
                    return gba.Cartridge.Read32(addr);
            }

            return 0x00000000;
        }

        public override void Write8(uint addr, byte val) {
            switch((addr >> 24) & 0xf) {
                case 0x0:
                case 0x1:
                    gba.Bios.Write8(addr, val);
                    break;

                case 0x2:
                    gba.Ewram.Write8(addr, val);
                    break;

                case 0x3:
                    gba.Iwram.Write8(addr, val);
                    break;

                case 0x4:
                    gba.Io.Write8(addr, val);
                    break;

                case 0x5:
                case 0x6:
                case 0x7:
                    gba.Ppu.Write8(addr, val);
                    break;

                case 0x8:
                case 0x9:
                case 0xa:
                case 0xb:
                case 0xc:
                case 0xd:
                case 0xe:
                case 0xf:
                    gba.Cartridge.Write8(addr, val);
                    break;
            }
        }
        
        public new void Write16(uint addr, ushort val) {
            addr &= 0xfffffffe;

            switch((addr >> 24) & 0xf) {
                case 0x0:
                case 0x1:
                    gba.Bios.Write16(addr, val);
                    break;

                case 0x2:
                    gba.Ewram.Write16(addr, val);
                    break;

                case 0x3:
                    gba.Iwram.Write16(addr, val);
                    break;

                case 0x4:
                    gba.Io.Write16(addr, val);
                    break;

                case 0x5:
                case 0x6:
                case 0x7:
                    gba.Ppu.Write16(addr, val);
                    break;

                case 0x8:
                case 0x9:
                case 0xa:
                case 0xb:
                case 0xc:
                case 0xd:
                case 0xe:
                case 0xf:
                    gba.Cartridge.Write16(addr, val);
                    break;
            }
        }
        
        public new void Write32(uint addr, uint val) {
            addr &= 0xfffffffc;

            switch((addr >> 24) & 0xf) {
                case 0x0:
                case 0x1:
                    gba.Bios.Write32(addr, val);
                    break;

                case 0x2:
                    gba.Ewram.Write32(addr, val);
                    break;

                case 0x3:
                    gba.Iwram.Write32(addr, val);
                    break;

                case 0x4:
                    gba.Io.Write32(addr, val);
                    break;

                case 0x5:
                case 0x6:
                case 0x7:
                    gba.Ppu.Write32(addr, val);
                    break;

                case 0x8:
                case 0x9:
                case 0xa:
                case 0xb:
                case 0xc:
                case 0xd:
                case 0xe:
                case 0xf:
                    gba.Cartridge.Write32(addr, val);
                    break;
            }
        }
    }
}
