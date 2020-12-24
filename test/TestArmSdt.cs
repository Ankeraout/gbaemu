using System;
using Xunit;
using System.Collections.Generic;

namespace gbaemu.test {
    public class TestArmSdt {
        [Fact]
        public void Test_Str_R15() {
            List<uint> stack = RomUtil.ExecuteTestRom("test_arm_sdt.gba");

            Assert.Equal(2, stack.Count);
            Assert.Equal(stack[0], stack[1]);
        }
    }
}