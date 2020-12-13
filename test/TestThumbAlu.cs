using System;
using Xunit;
using System.Collections.Generic;

namespace gbaemu.test {
    public class TestThumbAlu {
        [Fact]
        public void TestBic() {
            List<uint> stack = RomUtil.ExecuteTestRom("test_thumb_bic.gba");

            Assert.Single(stack);
            Assert.Equal(0x00040000U, stack[0]);
        }
    }
}