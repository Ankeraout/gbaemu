using System;

namespace gbaemu.GBA {
    public partial class CPU {
        private GBA gba;

        private enum Mode {
            USR_OLD = 0b00000,
            FIQ_OLD = 0b00001,
            IRQ_OLD = 0b00010,
            SVC_OLD = 0b00011,
            USR = 0b10000,
            FIQ = 0b10001,
            IRQ = 0b10010,
            SVC = 0b10011,
            ABT = 0b10111,
            UND = 0b11011,
            SYS = 0b11111
        };

        private enum PipelineState {
            Flush,
            Fetch,
            Decode,
            Execute
        }

        private PipelineState pipelineState;

        private uint[] r;
        private uint[] r_usr;
        private uint[] r_irq;
        private uint[] r_fiq;
        private uint[] r_svc;
        private uint[] r_abt;
        private uint[] r_und;
        public uint Cpsr {
            get {
                return 0
                    | (flagN ? (1U << 31) : 0)
                    | (flagZ ? (1U << 30) : 0)
                    | (flagC ? (1U << 29) : 0)
                    | (flagV ? (1U << 28) : 0)
                    | (flagI ? (1U << 7) : 0)
                    | (flagF ? (1U << 6) : 0)
                    | (flagT ? (1U << 5) : 0)
                    | (uint)mode;
            }

            private set {
                flagN = BitUtils.BitTest32(value, 31);
                flagZ = BitUtils.BitTest32(value, 30);
                flagC = BitUtils.BitTest32(value, 29);
                flagV = BitUtils.BitTest32(value, 28);
                flagI = BitUtils.BitTest32(value, 7);
                flagF = BitUtils.BitTest32(value, 6);
                flagT = BitUtils.BitTest32(value, 5);

                mode = (Mode)(value & 0b11111);
                
                CheckMode();
            }
        }

        private uint spsr_irq;
        private uint spsr_fiq;
        private uint spsr_svc;
        private uint spsr_abt;
        private uint spsr_und;
        private Mode mode;
        private bool flagN;
        private bool flagZ;
        private bool flagC;
        private bool flagV;
        private bool flagI;
        private bool flagF;
        private bool flagT;

        private uint fetchedOpcodeARM;
        private ushort fetchedOpcodeThumb;
        private uint decodedOpcodeARMValue;
        private ushort decodedOpcodeThumbValue;

        static CPU() {
            
        }

        public CPU(GBA _gba) {
            gba = _gba;

            r = new uint[16];
            r_usr = new uint[7];
            r_irq = new uint[2];
            r_fiq = new uint[7];
            r_svc = new uint[2];
            r_abt = new uint[2];
            r_und = new uint[2];

            Reset();
        }

        private void ChangeMode(Mode newMode) {
            Mode oldMode = mode;

            switch(oldMode) {
                case Mode.USR_OLD:
                case Mode.USR:
                case Mode.SYS:
                    r_usr[0] = r[8];
                    r_usr[1] = r[9];
                    r_usr[2] = r[10];
                    r_usr[3] = r[11];
                    r_usr[4] = r[12];
                    r_usr[5] = r[13];
                    r_usr[6] = r[14];
                    break;

                case Mode.FIQ_OLD:
                case Mode.FIQ:
                    r_fiq[0] = r[8];
                    r_fiq[1] = r[9];
                    r_fiq[2] = r[10];
                    r_fiq[3] = r[11];
                    r_fiq[4] = r[12];
                    r_fiq[5] = r[13];
                    r_fiq[6] = r[14];
                    break;

                case Mode.IRQ_OLD:
                case Mode.IRQ:
                    r_usr[0] = r[8];
                    r_usr[1] = r[9];
                    r_usr[2] = r[10];
                    r_usr[3] = r[11];
                    r_usr[4] = r[12];
                    r_irq[5] = r[13];
                    r_irq[6] = r[14];
                    break;

                case Mode.SVC_OLD:
                case Mode.SVC:
                    r_usr[0] = r[8];
                    r_usr[1] = r[9];
                    r_usr[2] = r[10];
                    r_usr[3] = r[11];
                    r_usr[4] = r[12];
                    r_svc[5] = r[13];
                    r_svc[6] = r[14];
                    break;

                case Mode.ABT:
                    r_usr[0] = r[8];
                    r_usr[1] = r[9];
                    r_usr[2] = r[10];
                    r_usr[3] = r[11];
                    r_usr[4] = r[12];
                    r_abt[5] = r[13];
                    r_abt[6] = r[14];
                    break;

                case Mode.UND:
                    r_usr[0] = r[8];
                    r_usr[1] = r[9];
                    r_usr[2] = r[10];
                    r_usr[3] = r[11];
                    r_usr[4] = r[12];
                    r_und[5] = r[13];
                    r_und[6] = r[14];
                    break;
            }

            switch(newMode) {
                case Mode.USR_OLD:
                case Mode.USR:
                case Mode.SYS:
                    r[8] = r_usr[0];
                    r[9] = r_usr[1];
                    r[10] = r_usr[2];
                    r[11] = r_usr[3];
                    r[12] = r_usr[4];
                    r[13] = r_usr[5];
                    r[14] = r_usr[6];
                    break;

                case Mode.FIQ_OLD:
                case Mode.FIQ:
                    r[8] = r_fiq[0];
                    r[9] = r_fiq[1];
                    r[10] = r_fiq[2];
                    r[11] = r_fiq[3];
                    r[12] = r_fiq[4];
                    r[13] = r_fiq[5];
                    r[14] = r_fiq[6];
                    break;

                case Mode.IRQ_OLD:
                case Mode.IRQ:
                    r[8] = r_usr[0];
                    r[9] = r_usr[1];
                    r[10] = r_usr[2];
                    r[11] = r_usr[3];
                    r[12] = r_usr[4];
                    r[13] = r_irq[0];
                    r[14] = r_irq[1];
                    break;

                case Mode.SVC_OLD:
                case Mode.SVC:
                    r[8] = r_usr[0];
                    r[9] = r_usr[1];
                    r[10] = r_usr[2];
                    r[11] = r_usr[3];
                    r[12] = r_usr[4];
                    r[13] = r_svc[0];
                    r[14] = r_svc[1];
                    break;

                case Mode.ABT:
                    r[8] = r_usr[0];
                    r[9] = r_usr[1];
                    r[10] = r_usr[2];
                    r[11] = r_usr[3];
                    r[12] = r_usr[4];
                    r[13] = r_abt[0];
                    r[14] = r_abt[1];
                    break;

                case Mode.UND:
                    r[8] = r_usr[0];
                    r[9] = r_usr[1];
                    r[10] = r_usr[2];
                    r[11] = r_usr[3];
                    r[12] = r_usr[4];
                    r[13] = r_und[0];
                    r[14] = r_und[1];
                    break;
            }

            mode = newMode;
        }

        public void Reset() {
            for(int i = 0; i < 2; i++) {
                r_irq[i] = 0;
                r_svc[i] = 0;
                r_abt[i] = 0;
                r_und[i] = 0;
            }

            for(int i = 0; i < 7; i++) {
                r_fiq[i] = 0;
                r_usr[i] = 0;
            }

            for(int i = 0; i < 16; i++) {
                r[i] = 0;
            }

            flagN = false;
            flagZ = false;
            flagC = false;
            flagV = false;
            flagI = false;
            flagF = false;
            flagT = false;
            mode = Mode.SYS;
        }

        private void CheckMode()
        {
            switch(mode) {
                case Mode.USR_OLD:
                case Mode.FIQ_OLD:
                case Mode.IRQ_OLD:
                case Mode.SVC_OLD:
                case Mode.USR:
                case Mode.FIQ:
                case Mode.IRQ:
                case Mode.SVC:
                case Mode.ABT:
                case Mode.UND:
                case Mode.SYS:
                    return;

                default:
                    throw new Exception("Undefined mode");
            }
        }

        private void Execute() {

        }

        private void Decode() {
            if(pipelineState == PipelineState.Decode) {
                if(flagT) {
                    
                }
            }
        }

        private void Fetch(uint fetchAddress) {
            if(flagT) {
                fetchedOpcodeThumb = gba.Bus.Read16(fetchAddress);
                r[15] += 2;
            } else {
                fetchedOpcodeARM = gba.Bus.Read32(fetchAddress);
                r[15] += 4;
            }
        }

        public void Cycle() {
            uint fetchAddress = r[15];

            Execute();
            Decode();
            Fetch(fetchAddress);

            switch(pipelineState) {
                case PipelineState.Flush:
                    pipelineState = PipelineState.Fetch;
                    break;

                case PipelineState.Fetch:
                    pipelineState = PipelineState.Decode;
                    break;

                case PipelineState.Decode:
                    pipelineState = PipelineState.Execute;
                    break;
            }
        }
    }
}
