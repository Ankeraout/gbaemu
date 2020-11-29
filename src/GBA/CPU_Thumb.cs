namespace gbaemu.GBA {
    public partial class CPU {
        private delegate void ThumbOpcodeHandler(CPU cpu, ushort opcode);

        private static void InitThumbDecodeTable() {

        }

        private static ThumbOpcodeHandler GetThumbOpcodeHandler(ushort opcode) {
            return null;
        }

        private ThumbOpcodeHandler DecodeThumbOpcode(ushort opcode) {
            return null;
        }
    }
}
