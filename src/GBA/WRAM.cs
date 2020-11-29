namespace gbaemu.GBA {
    public class WRAM : BusPeripheral {
        private byte[] data;
        private uint addressMask;

        public WRAM(uint size) {
            data = new byte[size];
            addressMask = size - 1;
        }

        public override byte Read8(uint addr)
        {
            return data[addr & addressMask];
        }

        public override void Write8(uint addr, byte val)
        {
            data[addr & addressMask] = val;
        }
    }
}
