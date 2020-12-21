using System;

namespace gbaemu
{
    static class Program
    {
        public static Configuration Configuration {get; private set;}

        static void Main(string[] args) {
            try {
                WrappedMain(args);
            } catch(Exception e) {
                Console.WriteLine(e.Message);
            }
        }

        static void WrappedMain(string[] args) {
            if(ReadCommandLineArguments(args) != 0) {
                PrintHelp();
                return;
            }

            CheckConfiguration();

            GBA.GBA gba = new GBA.GBA(Configuration.BiosPath, Configuration.RomPath, true);

            FrontEnd.IFrontEnd frontEnd = new FrontEnd.SDL2(gba);
            gba.Ppu.OnFrameEvent += frontEnd.Frame;

            try {
                while(true) {
                    gba.Cycle();
                }
            } catch(FrontEnd.QuitException) {
                
            }

            frontEnd.Close();
        }

        static int ReadCommandLineArguments(string[] args) {
            string biosPath = null;
            string romPath = null;
            bool flag_bios = false;
            bool flag_rom = false;

            foreach(string arg in args) {
                if(flag_bios) {
                    if(biosPath != null) {
                        throw new Exception("Too many BIOS files.");
                    } else {
                        biosPath = arg;
                        flag_bios = false;
                    }
                } else if(flag_rom) {
                    if(romPath != null) {
                        throw new Exception("Too many ROM files.");
                    } else {
                        romPath = arg;
                        flag_rom = false;
                    }
                } else if(arg == "--bios") {
                    flag_bios = true;
                } else if(arg == "--rom") {
                    flag_rom = true;
                } else if(arg == "--help") {
                    return 1;
                } else {
                    throw new Exception("Invalid argument '" + arg + "'.");
                }
            }

            Configuration = new Configuration(biosPath, romPath);

            return 0;
        }

        static void CheckConfiguration() {
            if(Configuration.BiosPath == null) {
                throw new Exception("No BIOS file path specified.");
            }
        }

        static void PrintHelp() {
            Console.WriteLine("gbaemu");
            Console.WriteLine("======");
            Console.WriteLine();
            Console.WriteLine("Required command-line options:");
            Console.WriteLine("  --bios <bios file name>");
            Console.WriteLine("  --rom <rom file name>");
            Console.WriteLine();
            Console.WriteLine("Optional command-line options:");
            Console.WriteLine("  --help");
        }
    }
}
