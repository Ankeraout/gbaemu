namespace gbaemu {
    public static class BitUtils {
        public static bool BitTest32(uint value, int bitNumber) {
            return (value & (1 << bitNumber)) != 0;
        }

        public static uint RotateRight32(uint value, int bits) {
            return (value >> bits) | (value << (32 - bits));
        }
    }
}
