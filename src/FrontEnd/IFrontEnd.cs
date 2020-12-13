namespace gbaemu.FrontEnd {
    public interface IFrontEnd {
        void Frame(uint[] colors);
        void Close();
    }
}