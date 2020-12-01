namespace gbaemu.GBA {
    public partial class CPU {
        private delegate void ThumbOpcodeHandler(CPU cpu, ushort opcode);

        private static ThumbOpcodeHandler[] thumbOpcodeHandlerTable;

        private static void InitThumbDecodeTable() {
            thumbOpcodeHandlerTable = new ThumbOpcodeHandler[1024];

            for(int i = 0; i < 1024; i++) {
                
            }
        }

        private ThumbOpcodeHandler DecodeThumbOpcode(ushort opcode) {
            return thumbOpcodeHandlerTable[opcode >> 6];
        }
    }
}
