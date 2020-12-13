namespace gbaemu.GBA {
    public partial class CPU {
        private delegate void ThumbOpcodeHandler(CPU cpu, ushort opcode);

        private static ThumbOpcodeHandler[] thumbOpcodeHandlerTable;

        private static void InitThumbDecodeTable() {
            thumbOpcodeHandlerTable = new ThumbOpcodeHandler[1024];

            for(int i = 0; i < 1024; i++) {
                ushort opcode = (ushort)(i << 6);

                if((opcode & 0xff00) == 0xb000) {
                    thumbOpcodeHandlerTable[i] = null;
                } else if((opcode & 0xff00) == 0xbf00) {
                    thumbOpcodeHandlerTable[i] = OpcodeThumbSwi;
                }
            }
        }

        private ThumbOpcodeHandler DecodeThumbOpcode(ushort opcode) {
            return thumbOpcodeHandlerTable[opcode >> 6];
        }

        private static void OpcodeThumbSwi(CPU cpu, ushort opcode) {
            cpu.RaiseSWI();
        }
    }
}
