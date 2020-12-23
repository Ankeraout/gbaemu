using System;
using System.IO;

namespace gbaemu.GBA {
    public class GBA {
        public const int BiosFileSize = 16384;
        public const int MaxRomFileSize = 33554432;
        public const int WramSlowSize = 262144;
        public const int WramFastSize = 32768;

        public BIOS Bios {get; private set;}
        public Bus Bus {get; private set;}
        public Cartridge Cartridge {get; private set;}
        public CPU CPU {get; private set;}
        public DMA Dma {get; private set;}
        public WRAM Iwram {get; private set;}
        public WRAM Ewram {get; private set;}
        public PPU Ppu {get; private set;}
        public Keypad Keypad {get; private set;}
        public IO Io {get; private set;}
        public Timer Timer {get; private set;}

        public bool SkipBoot {get; set;}

        private static byte[] readFile(string filePath, long maxFileSize) {
            FileInfo fi = new FileInfo(filePath);
            long fileSize = fi.Length;

            if(fileSize > maxFileSize) {
                throw new Exception("File too big");
            } else {
                return File.ReadAllBytes(filePath);
            }
        }

        public GBA(string biosFilePath, string romFilePath) : this(biosFilePath, romFilePath, false) {

        }

        public GBA(string biosFilePath, string romFilePath, bool skipBoot) {
            Bios = new BIOS(readFile(biosFilePath, BiosFileSize));

            if(romFilePath != null) {
                Cartridge = new Cartridge(readFile(romFilePath, MaxRomFileSize));
            } else {
                Cartridge = new Cartridge();
            }
            
            SkipBoot = skipBoot;

            CPU = new CPU(this, SkipBoot);
            Iwram = new WRAM(WramFastSize);
            Ewram = new WRAM(WramSlowSize);
            Keypad = new Keypad(this);
            Ppu = new PPU(this);
            Dma = new DMA(this);
            Timer = new Timer(this);
            Io = new IO(this);
            Bus = new Bus(this);
        }

        public void FrameAdvance() {

        }

        public void Cycle() {
            if(!Dma.Cycle()) {
                CPU.Cycle();
            }
            
            Ppu.Cycle();
            Timer.Cycle();
        }

        public void SetInterruptFlag(ushort flag) {
            Io.GetRegister(0x04000202).Value |= flag;
        }

        public void WriteToIF(uint addr, ushort flag) {
            Io.GetRegister(0x04000202).Value &= (ushort)~flag;
        }
    }
}
