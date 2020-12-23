namespace gbaemu.GBA {
    public class DMA {
        public class Channel {
            int index;
            uint iobase;
            uint sourceAddress;
            uint destinationAddress;
            uint wordCount;

            public enum DestinationAddressControl {
                Increment,
                Decrement,
                Fixed,
                IncrementAndReload
            }

            public enum SourceAddressControl {
                Increment,
                Decrement,
                Fixed,
                Prohibited
            }

            public enum StartTiming {
                Immediately,
                VBlank,
                HBlank,
                Special
            }

            private DestinationAddressControl destinationAddressControl;
            private SourceAddressControl sourceAddressControl;
            private bool repeat;
            private bool bitWidth;
            private bool gamePakDRQ;
            private StartTiming startTiming;
            private bool irq;
            private bool enabled;
            private bool running;

            private GBA GBA;

            public Channel(GBA gba, int index) {
                GBA = gba;
                this.index = index;
                iobase = 0x040000b0 + ((uint)index * 12);

                gba.Ppu.OnVblankEvent += OnVblank;
                gba.Ppu.OnHblankEvent += OnHblank;
            }

            public bool Cycle() {
                if(running) {
                    if(bitWidth) {
                        GBA.Bus.Write32(destinationAddress, GBA.Bus.Read32(sourceAddress));

                        switch(destinationAddressControl) {
                            case DestinationAddressControl.Increment: destinationAddress += 4; break;
                            case DestinationAddressControl.Decrement: destinationAddress -= 4; break;
                            case DestinationAddressControl.Fixed: break;
                            case DestinationAddressControl.IncrementAndReload: destinationAddress += 4; break;
                        }

                        switch(sourceAddressControl) {
                            case SourceAddressControl.Increment: sourceAddress += 4; break;
                            case SourceAddressControl.Decrement: sourceAddress -= 4; break;
                            case SourceAddressControl.Fixed: break;
                            case SourceAddressControl.Prohibited: sourceAddress += 4; break;
                        }
                    } else {
                        GBA.Bus.Write16(destinationAddress, GBA.Bus.Read16(sourceAddress));

                        switch(destinationAddressControl) {
                            case DestinationAddressControl.Increment: destinationAddress += 2; break;
                            case DestinationAddressControl.Decrement: destinationAddress -= 2; break;
                            case DestinationAddressControl.Fixed: break;
                            case DestinationAddressControl.IncrementAndReload: destinationAddress += 2; break;
                        }

                        switch(sourceAddressControl) {
                            case SourceAddressControl.Increment: sourceAddress += 2; break;
                            case SourceAddressControl.Decrement: sourceAddress -= 2; break;
                            case SourceAddressControl.Fixed: break;
                            case SourceAddressControl.Prohibited: sourceAddress += 2; break;
                        }
                    }

                    wordCount--;

                    if(wordCount == 0) {
                        Finish();
                    }

                    return true;
                }
                
                return false;
            }

            private void Repeat() {
                ReloadRegisters(true);
                running = true;
            }

            private void Finish() {
                System.Console.WriteLine("Completed DMA transfer (channel " + index + ").");

                running = false;

                if(!repeat) {
                    enabled = false;
                    GBA.Io.GetRegister(iobase + 10).Value &= 0x7fff;
                } else {
                    System.Console.WriteLine("Repeating DMA transfer on channel " + index + ".");

                    Repeat();
                    
                    if(startTiming == StartTiming.Immediately) {
                        running = true;
                    }
                }

                if(irq) {
                    GBA.SetInterruptFlag((ushort)(1 << (index + 8)));
                }
            }

            private void OnVblank() {
                if(enabled && !running && startTiming == StartTiming.VBlank) {
                    running = true;
                }
            }

            private void OnHblank() {
                if(enabled && !running && startTiming == StartTiming.HBlank) {
                    running = true;
                }
            }

            private void ReloadSourceAddress() {
                sourceAddress = (uint)GBA.Io.GetRegister(iobase).Value | (uint)(GBA.Io.GetRegister(iobase + 2).Value << 16);
            }

            private void ReloadDestinationAddress() {
                destinationAddress = (uint)GBA.Io.GetRegister(iobase + 4).Value | (uint)(GBA.Io.GetRegister(iobase + 6).Value << 16);
            }

            private void ReloadWordCount() {
                wordCount = GBA.Io.GetRegister(iobase + 8).Value;

                if(wordCount == 0) {
                    if(iobase == 0x040000d4) {
                        wordCount = 0x10000;
                    } else {
                        wordCount = 0x4000;
                    }
                }
            }

            private void ReloadRegisters(bool repeat) {
                if(repeat) {
                    if(destinationAddressControl == DestinationAddressControl.IncrementAndReload) {
                        ReloadDestinationAddress();
                    }

                    ReloadWordCount();
                } else {
                    ReloadSourceAddress();
                    ReloadDestinationAddress();
                    ReloadWordCount();
                }
            }

            public void CntHWriteCallback(uint address, ushort value) {
                bool oldEnabled = enabled;

                destinationAddressControl = (DestinationAddressControl)((value & 0x0060) >> 5);
                sourceAddressControl = (SourceAddressControl)((value & 0x0180) >> 7);
                repeat = BitUtils.BitTest16(value, 9);
                bitWidth = BitUtils.BitTest16(value, 10);
                gamePakDRQ = BitUtils.BitTest16(value, 11);
                startTiming = (StartTiming)((value & 0x3000) >> 12);
                irq = BitUtils.BitTest16(value, 14);
                enabled = BitUtils.BitTest16(value, 15);

                if(oldEnabled && !enabled) {
                    running = false;
                    System.Console.WriteLine("Disabled DMA channel " + index + ".");
                } else if(!oldEnabled && enabled) {
                    ReloadRegisters(false);

                    if(startTiming == StartTiming.Immediately) {
                        running = true;
                    }

                    System.Console.WriteLine(string.Format("Enabled DMA channel {0:d}. SAD={1:x8} DAD={2:x8} CNT={3:d} W={4:d}", index, sourceAddress, destinationAddress, wordCount, bitWidth ? 32 : 16));
                }
            }
        }

        public GBA GBA {get; private set;}

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

        public DMA(GBA gba) {
            GBA = gba;
            channels = new Channel[4];

            for(int i = 0; i < 4; i++) {
                channels[i] = new Channel(GBA, i);
            }
        }

        public bool Cycle() {
            foreach(Channel channel in channels) {
                if(channel.Cycle()) {
                    return true;
                }
            }

            return false;
        }
    }
}