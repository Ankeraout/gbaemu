namespace gbaemu.GBA {
    public interface IBusPeripheral {
        public byte Read8(uint addr);
        public ushort Read16(uint addr);
        public uint Read32(uint addr);
        public void Write8(uint addr, byte val);
        public void Write16(uint addr, ushort val);
        public void Write32(uint addr, uint val);
    }
}