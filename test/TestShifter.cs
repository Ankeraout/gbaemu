using System;
using Xunit;
using System.Collections.Generic;

namespace gbaemu.test {
    public class Shifter {
        [Fact]
        public void TestShifterImmediate() {
            List<uint> stack = RomUtil.ExecuteTestRom("test_shifter_immediate.gba");

            Assert.Equal(3, stack.Count);
            Assert.Equal(1U, stack[0]);
            Assert.Equal(0x40000000U, stack[1]);
            Assert.Equal(4U, stack[2]);
        }
    }
}
