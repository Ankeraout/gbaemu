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
        public WRAM Iwram {get; private set;}
        public WRAM Ewram {get; private set;}

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

        public GBA(string biosFilePath, string romFilePath) {
            Bios = new BIOS(readFile(biosFilePath, BiosFileSize));

            if(romFilePath != null) {
                Cartridge = new Cartridge(readFile(romFilePath, MaxRomFileSize));
            } else {
                Cartridge = new Cartridge();
            }
            
            SkipBoot = false;

            CPU = new CPU(this, SkipBoot);
            Iwram = new WRAM(WramFastSize);
            Ewram = new WRAM(WramSlowSize);
            Bus = new Bus(this);
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
            Bus = new Bus(this);
        }

        public void FrameAdvance() {

        }

        public void Cycle() {
            CPU.Cycle();
        }
    }
}
