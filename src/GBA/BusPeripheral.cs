namespace gbaemu.GBA {
    public abstract class BusPeripheral : IBusPeripheral {
        public abstract byte Read8(uint addr);

        public ushort Read16(uint addr) {
            return (ushort)(Read8(addr) | (Read8(addr + 1) << 8));
        }

        public uint Read32(uint addr) {
            return (uint)(Read16(addr) | (Read16(addr + 2) << 16));
        }

        public abstract void Write8(uint addr, byte val);

        public void Write16(uint addr, ushort val) {
            Write8(addr, (byte)(val & 0xff));
            Write8(addr + 1, (byte)(val >> 8));
        }

        public void Write32(uint addr, uint val) {
            Write16(addr, (ushort)(val & 0xffff));
            Write16(addr + 2, (ushort)(val >> 16));
        }
    }
}
