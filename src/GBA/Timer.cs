namespace gbaemu.GBA {
    public class Timer {
        public class Channel {
            private GBA GBA;
            private Channel nextChannel;
            private ushort irqFlag;
            private int index;

            private ushort reloadValue;

            private uint prescaler;
            private bool countUp;
            private bool irq;
            private bool operate;

            private ushort counter;

            public Channel(GBA gba, Channel next, int index) {
                GBA = gba;
                nextChannel = next;
                irqFlag = (ushort)(1 << (index + 3));
                this.index = index;
            }

            public void Cycle(uint cycleCounter) {
                if(operate && !countUp) {
                    if((cycleCounter & prescaler) == 0) {
                        Increment();
                    }
                }
            }

            private void Increment() {
                counter++;

                if(counter == 0) {
                    if(irq) {
                        GBA.SetInterruptFlag(irqFlag);
                    }

                    if(nextChannel != null) {
                        nextChannel.LastOverflowed();
                    }

                    counter = reloadValue;
                }
            }

            private void LastOverflowed() {
                if(countUp) {
                    Increment();
                }
            }

            public void WriteCallback_Reload(uint address, ushort value) {
                reloadValue = value;
            }

            public void WriteCallback_Control(uint address, ushort value) {
                switch(value & 0x0003) {
                    case 0b00: prescaler = 0; break;
                    case 0b01: prescaler = 63; break;
                    case 0b10: prescaler = 255; break;
                    case 0b11: prescaler = 1023; break;
                }

                if(index == 0) {
                    countUp = false;
                    GBA.Io.GetRegister(0x04000102).Value &= 0xfffb;
                } else {
                    countUp = BitUtils.BitTest16(value, 2);
                }
                
                irq = BitUtils.BitTest16(value, 6);
                operate = BitUtils.BitTest16(value, 7);
            }
        }

        private Channel[] channels;

        public Channel Channel0 {
            get {
                return channels[0];
            }
        }

        public Channel Channel1 {
            get {
                return channels[1];
            }
        }

        public Channel Channel2 {
            get {
                return channels[2];
            }
        }

        public Channel Channel3 {
            get {
                return channels[3];
            }
        }

        public GBA GBA {get; private set;}

        private uint cycleCounter;

        public Timer(GBA gba) {
            GBA = gba;

            channels = new Channel[4];

            channels[3] = new Channel(gba, null, 3);
            channels[2] = new Channel(gba, channels[3], 2);
            channels[1] = new Channel(gba, channels[2], 1);
            channels[0] = new Channel(gba, channels[1], 0);
        }

        public void Cycle() {
            foreach(Channel channel in channels) {
                channel.Cycle(cycleCounter);
            }
        }
    }
}
