namespace gbaemu {
    public static class BitUtils {
        public static bool BitTest32(uint value, int bitNumber) {
            return (value & (1 << bitNumber)) != 0;
        }
    }
}