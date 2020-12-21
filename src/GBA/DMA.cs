namespace gbaemu.GBA {
    public class DMA {
        public GBA GBA {get; private set;}

        public DMA(GBA gba) {
            GBA = gba;
        }

        public void WriteCallback(uint addr, ushort val) {
            System.Console.WriteLine("Unimplemented DMA write");
        }
    }
}