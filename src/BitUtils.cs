namespace gbaemu {
    public static class BitUtils {
        public static ushort BitClear16(ushort value, int bitNumber) {
            return (ushort)(value & ~(1 << bitNumber));
        }

        public static ushort BitSet16(ushort value, int bitNumber) {
            return (ushort)(value | (1 << bitNumber));
        }

        public static uint BitSet32(uint value, int bitNumber) {
            return (uint)(value | (1U << bitNumber));
        }

        public static bool BitTest8(byte value, int bitNumber) {
            return (value & (1 << bitNumber)) != 0;
        }

        public static bool BitTest16(ushort value, int bitNumber) {
            return (value & (1 << bitNumber)) != 0;
        }

        public static bool BitTest32(uint value, int bitNumber) {
            return (value & (1 << bitNumber)) != 0;
        }

        public static bool BitTest64(ulong value, int bitNumber) {
            return (value & (1UL << bitNumber)) != 0;
        }

        public static uint RotateRight32(uint value, int bits) {
            return (value >> bits) | (value << (32 - bits));
        }

        public static uint HammingWeight8(byte v) {
            uint w = 0;

            for(int i = 0; i < 8; i++) {
                if(BitTest8(v, i)) {
                    w++;
                }
            }

            return w;
        }

        public static uint HammingWeight16(ushort v) {
            uint w = 0;

            for(int i = 0; i < 16; i++) {
                if(BitTest16(v, i)) {
                    w++;
                }
            }

            return w;
        }
    }
}
