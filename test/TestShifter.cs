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

        [Fact]
        public void TestShifterRegisterImmediate() {
            List<uint> stack = RomUtil.ExecuteTestRom("test_shifter_reg_imm.gba");

            Assert.Equal(16, stack.Count);
            Assert.Equal(1U, stack[0]);
            Assert.Equal(0U, stack[1]);
            Assert.Equal(2U, stack[2]);
            Assert.Equal(0U, stack[3]);
            Assert.Equal(0U, stack[4]);
            Assert.Equal(0x40000000U, stack[5]);
            Assert.Equal(0U, stack[6]);
            Assert.Equal(0x60000000U, stack[7]);
            Assert.Equal(0U, stack[8]);
            Assert.Equal(0x60000000U, stack[9]);
            Assert.Equal(0U, stack[10]);
            Assert.Equal(0x40000000U, stack[11]);
            Assert.Equal(0x80000000U, stack[12]);
            Assert.Equal(0xa0000000U, stack[13]);
            Assert.Equal(0x80000000U, stack[14]);
            Assert.Equal(0xa0000000U, stack[15]);
        }

        [Fact]
        public void TestShifterRegisterRegister() {
            List<uint> stack = RomUtil.ExecuteTestRom("test_shifter_reg_reg.gba");

            Assert.Equal(32, stack.Count);
            Assert.Equal(1U, stack[0]);
            Assert.Equal(0U, stack[1]);
            Assert.Equal(2U, stack[2]);
            Assert.Equal(0U, stack[3]);
            Assert.Equal(0U, stack[4]);
            Assert.Equal(0x60000000U, stack[5]);
            Assert.Equal(0U, stack[6]);
            Assert.Equal(0x40000000U, stack[7]);
            Assert.Equal(1U, stack[8]);
            Assert.Equal(0U, stack[9]);
            Assert.Equal(0U, stack[10]);
            Assert.Equal(0x60000000U, stack[11]);
            Assert.Equal(0U, stack[12]);
            Assert.Equal(0x40000000U, stack[13]);
            Assert.Equal(0U, stack[14]);
            Assert.Equal(0x40000000U, stack[15]);
            Assert.Equal(1U, stack[16]);
            Assert.Equal(0U, stack[17]);
            Assert.Equal(0U, stack[18]);
            Assert.Equal(0x60000000U, stack[19]);
            Assert.Equal(0U, stack[20]);
            Assert.Equal(0x40000000U, stack[21]);
            Assert.Equal(0U, stack[22]);
            Assert.Equal(0x40000000U, stack[23]);
            Assert.Equal(1U, stack[24]);
            Assert.Equal(0U, stack[25]);
            Assert.Equal(0x80000000U, stack[26]);
            Assert.Equal(0xa0000000U, stack[27]);
            Assert.Equal(1U, stack[28]);
            Assert.Equal(0U, stack[29]);
            Assert.Equal(0x80000000U, stack[30]);
            Assert.Equal(0xa0000000U, stack[31]);
        }
    }
}
