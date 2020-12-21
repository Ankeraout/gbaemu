namespace gbaemu.GBA {
    public class Keypad {
        public GBA GBA {get; private set;}

        public enum Keys {
            A,
            B,
            Select,
            Start,
            Right,
            Left,
            Up,
            Down,
            R,
            L
        }

        public Keypad(GBA gba) {
            GBA = gba;
        }

        public void UpdatePressedKeys(bool[] pressedKeys) {
            ushort value = 0xffff;

            for(int i = 0; i < pressedKeys.Length; i++) {
                if(pressedKeys[i]) {
                    value &= (ushort)~(1 << i);
                }
            }

            IO.Register keyinput = GBA.Io.GetRegister(0x04000130);
            keyinput.Value = value;
        }

        public void Cycle() {
            IO.Register keyinput = GBA.Io.GetRegister(0x04000130);
            IO.Register keycnt = GBA.Io.GetRegister(0x04000132);
            
            if(BitUtils.BitTest16(keycnt.Value, 14)) {
                if(BitUtils.BitTest16(keycnt.Value, 15)) {
                    if((keycnt.Value & (~keyinput.Value) & 0x03ff) == (keycnt.Value & 0x03ff)) {
                        GBA.SetInterruptFlag(1 << 12);
                    }
                } else {
                    if((keycnt.Value & (~keyinput.Value) & 0x03ff) != 0) {
                        GBA.SetInterruptFlag(1 << 12);
                    }
                }
            }
        }
    }
}