namespace gbaemu.GBA {
    public class BIOS : BusPeripheral {
        private byte[] biosData;

        public BIOS(byte[] _biosData) {
            biosData = new byte[GBA.BiosFileSize];

            if(_biosData.Length != GBA.BiosFileSize) {
                throw new System.Exception("Wrong BIOS ROM size.");
            } else {
                _biosData.CopyTo(biosData, 0);
            }
        }

        public override byte Read8(uint addr) {
            return biosData[addr];
        }

        public override void Write8(uint addr, byte val) {
            
        }
    }
}
