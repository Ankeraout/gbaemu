using System;

namespace gbaemu
{
    public struct Configuration
    {
        public string BiosPath {get; set;}
        public string RomPath {get; set;}

        public Configuration(string biosPath, string romPath) {
            BiosPath = biosPath;
            RomPath = romPath;
        }
    }
}
