using System;

namespace gbaemu.GBA {
    public class Cartridge : BusPeripheral {
        private byte[] romData;

        private static uint ceilPo2(uint v) {
            return (uint)(Math.Pow(2, Math.Ceiling(Math.Log2(v))));
        }
        
        public Cartridge() {
            romData = new byte[0];
        }

        public Cartridge(byte[] _romData) {
            romData = new byte[ceilPo2((uint)_romData.Length)];
            _romData.CopyTo(romData, 0);
        }

        public override byte Read8(uint addr) {
            switch((addr >> 24) & 0xf) {
                case 0x8:
                case 0x9:
                case 0xa:
                case 0xb:
                case 0xc:
                case 0xd:
                    if((addr & 0x01ffffff) < romData.Length) {
                        return romData[addr & 0x01ffffff];
                    }

                    break;
            }

            return 0;
        }

        public override void Write8(uint addr, byte val) {
            
        }
    }
}