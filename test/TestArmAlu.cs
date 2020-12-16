using System;
using Xunit;
using System.Collections.Generic;

namespace gbaemu.test {
    public class TestArmAlu {
        [Fact]
        public void Test_Cmp_0_0() {
            List<uint> stack = RomUtil.ExecuteTestRom("test_arm_alu_cmp_0_0.gba");

            Assert.Single(stack);
            Assert.Equal(0x6000001fU, stack[0]);
        }
    }
}