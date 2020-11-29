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
    }
}


