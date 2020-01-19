#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/dma.hpp>
#include <gbaemu/gba/io.hpp>
#include <gbaemu/gba/lcd.hpp>
#include <gbaemu/gba/timer.hpp>

namespace gbaemu::gba::io {
    ioregTableEntry_t io[0x402];

    static inline uint32_t convertAddress(uint32_t address) {
        if((address & 0xff00fffc) == 0x04000800) {
            return (0x800 | (address & 0x00000003)) >> 1;
        } else if(address < 0x04000800) {
            return (address & 0x000007ff) >> 1;
        } else {
            return UINT32_MAX;
        }
    }

    void initRegister(uint32_t address, int registerNumber, uint16_t initialValue, writeCallback_t *writeCallback, uint16_t readMask, uint16_t writeMask) {
        address = convertAddress(address);
        io[address] = {registerNumber, initialValue, writeCallback, readMask, writeMask};
    }

    void init() {
        for(int address = 0x04000000; address < 0x04000804; address += 2) {
            initRegister(address, IOREGNONE, 0, NULL, 0, 0);
        }

        initRegister(0x04000000, DISPCNT, 0x0000, gbaemu::gba::lcd::writeCallback_dispcnt, 0xffff, 0xffff);
        initRegister(0x04000002, GREENSWP, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000004, DISPSTAT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000006, VCOUNT, 0x0000, NULL, 0xffff, 0x0000);
        initRegister(0x04000008, BG0CNT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x0400000a, BG1CNT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x0400000c, BG2CNT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x0400000e, BG3CNT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000010, BG0HOFS, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000012, BG0VOFS, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000014, BG1HOFS, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000016, BG1VOFS, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000018, BG2HOFS, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x0400001a, BG2VOFS, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x0400001c, BG3HOFS, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x0400001e, BG3VOFS, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000020, BG2PA, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000022, BG2PB, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000024, BG2PC, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000026, BG3PD, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000028, BG2X_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x0400002a, BG2X_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x0400002c, BG2Y_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x0400002e, BG2Y_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000030, BG3PA, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000032, BG3PB, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000034, BG3PC, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000036, BG3PD, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000038, BG3X_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x0400003a, BG3X_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x0400003c, BG3Y_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x0400003e, BG3Y_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000040, WIN0H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000042, WIN1H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000044, WIN0V, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000046, WIN1V, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000048, WININ, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x0400004a, WINOUT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x0400004c, MOSAIC, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000050, BLDCNT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000052, BLDALPHA, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000054, BLDY, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x04000060, SOUND1CNT_L, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000062, SOUND1CNT_H, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000064, SOUND1CNT_X, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000068, SOUND2CNT_L, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x0400006c, SOUND2CNT_H, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000070, SOUND3CNT_L, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000072, SOUND3CNT_H, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000074, SOUND3CNT_X, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000078, SOUND4CNT_L, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x0400007c, SOUND4CNT_H, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000080, SOUNDCNT_L, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000082, SOUNDCNT_H, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000084, SOUNDCNT_X, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000088, SOUNDBIAS, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000090, WAVE_RAM, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000092, WAVE_RAM, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000094, WAVE_RAM, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000096, WAVE_RAM, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000098, WAVE_RAM, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x0400009a, WAVE_RAM, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x0400009c, WAVE_RAM, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x0400009e, WAVE_RAM, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x040000a0, FIFO_A_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000a2, FIFO_A_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000a4, FIFO_B_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000a6, FIFO_B_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000b0, DMA0SAD_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000b2, DMA0SAD_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000b4, DMA0DAD_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000b6, DMA0DAD_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000b8, DMA0CNT_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000ba, DMA0CNT_H, 0x0000, gbaemu::gba::dma::writeCallback_dma0cnt, 0xffff, 0xffff);
        initRegister(0x040000bc, DMA1SAD_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000be, DMA1SAD_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000c0, DMA1DAD_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000c2, DMA1DAD_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000c4, DMA1CNT_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000c6, DMA1CNT_H, 0x0000, gbaemu::gba::dma::writeCallback_dma1cnt, 0xffff, 0xffff);
        initRegister(0x040000c8, DMA2SAD_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000ca, DMA2SAD_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000cc, DMA2DAD_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000ce, DMA2DAD_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000d0, DMA2CNT_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000d2, DMA2CNT_H, 0x0000, gbaemu::gba::dma::writeCallback_dma2cnt, 0xffff, 0xffff);
        initRegister(0x040000d4, DMA3SAD_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000d6, DMA3SAD_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000d8, DMA3DAD_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000da, DMA3DAD_H, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000dc, DMA3CNT_L, 0x0000, NULL, 0x0000, 0xffff);
        initRegister(0x040000de, DMA3CNT_H, 0x0000, gbaemu::gba::dma::writeCallback_dma3cnt, 0xffff, 0xffff);
        initRegister(0x04000100, TM0CNT_L, 0x0000, gbaemu::gba::timer::writeCallback_tm0cnt_l, 0xffff, 0xffff);
        initRegister(0x04000102, TM0CNT_H, 0x0000, gbaemu::gba::timer::writeCallback_tm0cnt_h, 0xffff, 0xffff);
        initRegister(0x04000104, TM1CNT_L, 0x0000, gbaemu::gba::timer::writeCallback_tm1cnt_l, 0xffff, 0xffff);
        initRegister(0x04000106, TM1CNT_H, 0x0000, gbaemu::gba::timer::writeCallback_tm1cnt_h, 0xffff, 0xffff);
        initRegister(0x04000108, TM2CNT_L, 0x0000, gbaemu::gba::timer::writeCallback_tm2cnt_l, 0xffff, 0xffff);
        initRegister(0x0400010a, TM2CNT_H, 0x0000, gbaemu::gba::timer::writeCallback_tm2cnt_h, 0xffff, 0xffff);
        initRegister(0x0400010c, TM3CNT_L, 0x0000, gbaemu::gba::timer::writeCallback_tm3cnt_l, 0xffff, 0xffff);
        initRegister(0x0400010e, TM3CNT_H, 0x0000, gbaemu::gba::timer::writeCallback_tm3cnt_h, 0xffff, 0xffff);
        initRegister(0x04000120, SIOMULTI0, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000122, SIOMULTI1, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000124, SIOMULTI2, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000126, SIOMULTI3, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000128, SIOCNT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x0400012a, SIOMLT_SEND, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000130, KEYINPUT, 0x0000, NULL, 0x03ff, 0x0000);
        initRegister(0x04000132, KEYCNT, 0x0000, NULL, 0xc3ff, 0xc3ff);
        initRegister(0x04000134, RCNT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000136, IR, 0x0000, NULL, 0x0000, 0x0000);
        initRegister(0x04000140, JOYCNT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000150, JOY_RECV_L, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000152, JOY_RECV_H, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000154, JOY_TRANS_L, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000156, JOY_TRANS_H, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000158, JOYSTAT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000200, IE, 0x0000, cpu::if_writeCallback, 0xffff, 0xffff);
        initRegister(0x04000202, IF, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000204, WAITCNT, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000208, IME, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000300, POSTFLG, 0x0000, NULL, 0xff00, 0xffff);
        initRegister(0x04000410, UNKBUG, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000800, MEMCTL_L, 0x0000, NULL, 0xffff, 0xffff);
        initRegister(0x04000802, MEMCTL_H, 0x0000, NULL, 0xffff, 0xffff);
    }

    uint8_t read8(uint32_t address) {
        if(address & 0x00000001) {
            return read16(address) >> 8;
        } else {
            return read16(address) & 0x00ff;
        }
    }

    uint16_t read16(uint32_t address) {
        address = convertAddress(address & 0xfffffffe);

        if(address == UINT32_MAX) {
            return 0x0000;
        }

        return io[address].value & io[address].readMask;
    }

    uint32_t read32(uint32_t address) {
        address &= 0xfffffffc;

        return read16(address) | (read16(address | 0x00000002) << 16);
    }

    void write8(uint32_t address, uint8_t value) {
        if(address & 0x00000001) {
            write16(address, (read16(address) & 0x00ff) | (value << 8));
        } else {
            write16(address, (read16(address) & 0xff00) | value);
        }
    }

    void write16(uint32_t address, uint16_t value) {
        address = convertAddress(address & 0xfffffffe);

        if(address == UINT32_MAX) {
            return;
        }

        io[address].value &= ~io[address].writeMask;
        io[address].value |= value & io[address].writeMask;

        if(io[address].writeCallback) {
            io[address].writeCallback(value);
        }
    }

    void write32(uint32_t address, uint32_t value) {
        address &= 0xfffffffc;
        write16(address, value & 0x0000ffff);
        write16(address | 2, value >> 16);
    }
    
    void set(unsigned int index, uint16_t value) {
        io[index >> 1].value = value;
    }

    void set32(unsigned int index, uint32_t value) {
        set(index, value & 0x0000ffff);
        set(index + 1, value >> 16);
    }

    uint16_t get(unsigned int index) {
        return io[index >> 1].value;
    }

    uint32_t get32(unsigned int index) {
        return get(index) | (get(index + 1) << 16);
    }
}
