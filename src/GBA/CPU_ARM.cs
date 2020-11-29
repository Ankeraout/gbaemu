namespace gbaemu.GBA {
    public partial class CPU {
        private delegate void ARMOpcodeHandler(CPU cpu, uint opcode);

        private static void InitARMDecodeTable() {
            
        }

        private static ARMOpcodeHandler GetARMOpcodeHandler(uint opcode) {
            return null;
        }

        private ARMOpcodeHandler DecodeARMOpcode(uint opcode) {
            return null;
        }
    }
}