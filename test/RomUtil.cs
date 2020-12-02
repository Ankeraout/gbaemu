using System;
using System.Collections.Generic;

namespace gbaemu.test
{
    public static class RomUtil
    {
        private const int TestStartMaxCycles = 100;
        private const int TestEndMaxCycles = 100000;

        public static List<uint> ExecuteTestRom(string testRom) {
            List<uint> stack = new List<uint>();

            GBA.GBA gba = new GBA.GBA("../../../nobios.bin", "../../../../testroms/" + testRom, true);

            int cycleCount = 0;

            uint stackStart = gba.CPU.R13;

            // Wait for the test to start
            while(cycleCount++ < TestStartMaxCycles && gba.Bus.Read8(0x03000000) != 0x11) {
                gba.Cycle();
            }

            // If the test didn't start in time, throw an exception
            if(gba.Bus.Read8(0x03000000) != 0x11) {
                throw new Exception("The test didn't start in time.");
            }

            // Wait for the test to finish
            cycleCount = 0;

            while(cycleCount++ < TestEndMaxCycles && gba.Bus.Read8(0x03000000) != 0x22) {
                gba.Cycle();
            }

            // If the test didn't end in time, throw an exception
            if(gba.Bus.Read8(0x03000000) != 0x22) {
                throw new Exception("The test didn't end in time.");
            }

            // Read the stack
            for(uint sp = stackStart - 4; sp >= gba.CPU.R13; sp -= 4) {
                stack.Add(gba.Bus.Read32(sp));
            }

            return stack;
        }
    }
}
