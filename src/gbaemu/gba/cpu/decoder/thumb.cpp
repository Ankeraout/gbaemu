#include <cstddef>
#include <cstdint>

#include <gbaemu/gba/cpu/decoder.hpp>
#include <gbaemu/gba/cpu/impl/thumb/addsub.hpp>
#include <gbaemu/gba/cpu/impl/thumb/alu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/cb.hpp>
#include <gbaemu/gba/cpu/impl/thumb/hrobe.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lbl.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lsio.hpp>
#include <gbaemu/gba/cpu/impl/thumb/mcas.hpp>
#include <gbaemu/gba/cpu/impl/thumb/mls.hpp>
#include <gbaemu/gba/cpu/impl/thumb/msr.hpp>
#include <gbaemu/gba/cpu/impl/thumb/pcrldr.hpp>

#define REP2(x) x, x
#define REP4(x) REP2(x), REP2(x)
#define REP8(x) REP4(x), REP4(x)
#define REP16(x) REP8(x), REP8(x)
#define REP32(x) REP16(x), REP16(x)
#define REP64(x) REP32(x), REP32(x)

namespace gbaemu::gba::cpu::decoder::thumb {
    const thumbOpcodeCallback_t opcodeDecoderTable[] = {
        REP32(gbaemu::gba::cpu::impl::thumb::msr::opcode_lsl), // 0x000
        REP32(gbaemu::gba::cpu::impl::thumb::msr::opcode_lsr), // 0x080
        REP32(gbaemu::gba::cpu::impl::thumb::msr::opcode_asr), // 0x100
        REP8(gbaemu::gba::cpu::impl::thumb::addsub::opcode_add_Rn), // 0x180
        REP8(gbaemu::gba::cpu::impl::thumb::addsub::opcode_sub_Rn), // 0x1a0
        REP8(gbaemu::gba::cpu::impl::thumb::addsub::opcode_add_Offset), // 0x1c0
        REP8(gbaemu::gba::cpu::impl::thumb::addsub::opcode_sub_Offset), // 0x1e0
        REP32(gbaemu::gba::cpu::impl::thumb::mcas::opcode_mov), // 0x200
        REP32(gbaemu::gba::cpu::impl::thumb::mcas::opcode_cmp), // 0x280
        REP32(gbaemu::gba::cpu::impl::thumb::mcas::opcode_add), // 0x300
        REP32(gbaemu::gba::cpu::impl::thumb::mcas::opcode_sub), // 0x380
        gbaemu::gba::cpu::impl::thumb::alu::opcode_and, // 0x400
        gbaemu::gba::cpu::impl::thumb::alu::opcode_eor, // 0x404
        gbaemu::gba::cpu::impl::thumb::alu::opcode_lsl, // 0x408
        gbaemu::gba::cpu::impl::thumb::alu::opcode_lsr, // 0x40c
        gbaemu::gba::cpu::impl::thumb::alu::opcode_asr, // 0x410
        gbaemu::gba::cpu::impl::thumb::alu::opcode_adc, // 0x414
        gbaemu::gba::cpu::impl::thumb::alu::opcode_sbc, // 0x418
        gbaemu::gba::cpu::impl::thumb::alu::opcode_ror, // 0x41c
        gbaemu::gba::cpu::impl::thumb::alu::opcode_tst, // 0x420
        gbaemu::gba::cpu::impl::thumb::alu::opcode_neg, // 0x424
        gbaemu::gba::cpu::impl::thumb::alu::opcode_cmp, // 0x428
        gbaemu::gba::cpu::impl::thumb::alu::opcode_cmn, // 0x42c
        gbaemu::gba::cpu::impl::thumb::alu::opcode_orr, // 0x430
        gbaemu::gba::cpu::impl::thumb::alu::opcode_mul, // 0x434
        gbaemu::gba::cpu::impl::thumb::alu::opcode_bic, // 0x438
        gbaemu::gba::cpu::impl::thumb::alu::opcode_mvn, // 0x43c
        NULL, // 0x440
        gbaemu::gba::cpu::impl::thumb::hrobe::opcode_add_Rd_Hs, // 0x444
        gbaemu::gba::cpu::impl::thumb::hrobe::opcode_add_Hd_Rs, // 0x448
        gbaemu::gba::cpu::impl::thumb::hrobe::opcode_add_Hd_Hs, // 0x44c
        NULL, // 0x450
        gbaemu::gba::cpu::impl::thumb::hrobe::opcode_cmp_Rd_Hs, // 0x454
        gbaemu::gba::cpu::impl::thumb::hrobe::opcode_cmp_Hd_Rs, // 0x458
        gbaemu::gba::cpu::impl::thumb::hrobe::opcode_cmp_Hd_Hs, // 0x45c
        NULL, // 0x460
        gbaemu::gba::cpu::impl::thumb::hrobe::opcode_mov_Rd_Hs, // 0x464
        gbaemu::gba::cpu::impl::thumb::hrobe::opcode_mov_Hd_Rs, // 0x468
        gbaemu::gba::cpu::impl::thumb::hrobe::opcode_mov_Hd_Hs, // 0x46c
        gbaemu::gba::cpu::impl::thumb::hrobe::opcode_bx_Rs, // 0x470
        gbaemu::gba::cpu::impl::thumb::hrobe::opcode_bx_Hs, // 0x474
        NULL, // 0x478
        NULL, // 0x47c
        REP32(gbaemu::gba::cpu::impl::thumb::pcrldr::pcrldr), // 0x480
        NULL, // 0x500
        NULL, // 0x504
        NULL, // 0x508
        NULL, // 0x50c
        NULL, // 0x510
        NULL, // 0x514
        NULL, // 0x518
        NULL, // 0x51c
        NULL, // 0x520
        NULL, // 0x524
        NULL, // 0x528
        NULL, // 0x52c
        NULL, // 0x530
        NULL, // 0x534
        NULL, // 0x538
        NULL, // 0x53c
        NULL, // 0x540
        NULL, // 0x544
        NULL, // 0x548
        NULL, // 0x54c
        NULL, // 0x550
        NULL, // 0x554
        NULL, // 0x558
        NULL, // 0x55c
        NULL, // 0x560
        NULL, // 0x564
        NULL, // 0x568
        NULL, // 0x56c
        NULL, // 0x570
        NULL, // 0x574
        NULL, // 0x578
        NULL, // 0x57c
        NULL, // 0x580
        NULL, // 0x584
        NULL, // 0x588
        NULL, // 0x58c
        NULL, // 0x590
        NULL, // 0x594
        NULL, // 0x598
        NULL, // 0x59c
        NULL, // 0x5a0
        NULL, // 0x5a4
        NULL, // 0x5a8
        NULL, // 0x5ac
        NULL, // 0x5b0
        NULL, // 0x5b4
        NULL, // 0x5b8
        NULL, // 0x5bc
        NULL, // 0x5c0
        NULL, // 0x5c4
        NULL, // 0x5c8
        NULL, // 0x5cc
        NULL, // 0x5d0
        NULL, // 0x5d4
        NULL, // 0x5d8
        NULL, // 0x5dc
        NULL, // 0x5e0
        NULL, // 0x5e4
        NULL, // 0x5e8
        NULL, // 0x5ec
        NULL, // 0x5f0
        NULL, // 0x5f4
        NULL, // 0x5f8
        NULL, // 0x5fc
        REP32(gbaemu::gba::cpu::impl::thumb::lsio::opcode_str), // 0x600
        REP32(gbaemu::gba::cpu::impl::thumb::lsio::opcode_ldr), // 0x680
        REP32(gbaemu::gba::cpu::impl::thumb::lsio::opcode_strb), // 0x700
        REP32(gbaemu::gba::cpu::impl::thumb::lsio::opcode_ldrb), // 0x780
        NULL, // 0x800
        NULL, // 0x804
        NULL, // 0x808
        NULL, // 0x80c
        NULL, // 0x810
        NULL, // 0x814
        NULL, // 0x818
        NULL, // 0x81c
        NULL, // 0x820
        NULL, // 0x824
        NULL, // 0x828
        NULL, // 0x82c
        NULL, // 0x830
        NULL, // 0x834
        NULL, // 0x838
        NULL, // 0x83c
        NULL, // 0x840
        NULL, // 0x844
        NULL, // 0x848
        NULL, // 0x84c
        NULL, // 0x850
        NULL, // 0x854
        NULL, // 0x858
        NULL, // 0x85c
        NULL, // 0x860
        NULL, // 0x864
        NULL, // 0x868
        NULL, // 0x86c
        NULL, // 0x870
        NULL, // 0x874
        NULL, // 0x878
        NULL, // 0x87c
        NULL, // 0x880
        NULL, // 0x884
        NULL, // 0x888
        NULL, // 0x88c
        NULL, // 0x890
        NULL, // 0x894
        NULL, // 0x898
        NULL, // 0x89c
        NULL, // 0x8a0
        NULL, // 0x8a4
        NULL, // 0x8a8
        NULL, // 0x8ac
        NULL, // 0x8b0
        NULL, // 0x8b4
        NULL, // 0x8b8
        NULL, // 0x8bc
        NULL, // 0x8c0
        NULL, // 0x8c4
        NULL, // 0x8c8
        NULL, // 0x8cc
        NULL, // 0x8d0
        NULL, // 0x8d4
        NULL, // 0x8d8
        NULL, // 0x8dc
        NULL, // 0x8e0
        NULL, // 0x8e4
        NULL, // 0x8e8
        NULL, // 0x8ec
        NULL, // 0x8f0
        NULL, // 0x8f4
        NULL, // 0x8f8
        NULL, // 0x8fc
        NULL, // 0x900
        NULL, // 0x904
        NULL, // 0x908
        NULL, // 0x90c
        NULL, // 0x910
        NULL, // 0x914
        NULL, // 0x918
        NULL, // 0x91c
        NULL, // 0x920
        NULL, // 0x924
        NULL, // 0x928
        NULL, // 0x92c
        NULL, // 0x930
        NULL, // 0x934
        NULL, // 0x938
        NULL, // 0x93c
        NULL, // 0x940
        NULL, // 0x944
        NULL, // 0x948
        NULL, // 0x94c
        NULL, // 0x950
        NULL, // 0x954
        NULL, // 0x958
        NULL, // 0x95c
        NULL, // 0x960
        NULL, // 0x964
        NULL, // 0x968
        NULL, // 0x96c
        NULL, // 0x970
        NULL, // 0x974
        NULL, // 0x978
        NULL, // 0x97c
        NULL, // 0x980
        NULL, // 0x984
        NULL, // 0x988
        NULL, // 0x98c
        NULL, // 0x990
        NULL, // 0x994
        NULL, // 0x998
        NULL, // 0x99c
        NULL, // 0x9a0
        NULL, // 0x9a4
        NULL, // 0x9a8
        NULL, // 0x9ac
        NULL, // 0x9b0
        NULL, // 0x9b4
        NULL, // 0x9b8
        NULL, // 0x9bc
        NULL, // 0x9c0
        NULL, // 0x9c4
        NULL, // 0x9c8
        NULL, // 0x9cc
        NULL, // 0x9d0
        NULL, // 0x9d4
        NULL, // 0x9d8
        NULL, // 0x9dc
        NULL, // 0x9e0
        NULL, // 0x9e4
        NULL, // 0x9e8
        NULL, // 0x9ec
        NULL, // 0x9f0
        NULL, // 0x9f4
        NULL, // 0x9f8
        NULL, // 0x9fc
        NULL, // 0xa00
        NULL, // 0xa04
        NULL, // 0xa08
        NULL, // 0xa0c
        NULL, // 0xa10
        NULL, // 0xa14
        NULL, // 0xa18
        NULL, // 0xa1c
        NULL, // 0xa20
        NULL, // 0xa24
        NULL, // 0xa28
        NULL, // 0xa2c
        NULL, // 0xa30
        NULL, // 0xa34
        NULL, // 0xa38
        NULL, // 0xa3c
        NULL, // 0xa40
        NULL, // 0xa44
        NULL, // 0xa48
        NULL, // 0xa4c
        NULL, // 0xa50
        NULL, // 0xa54
        NULL, // 0xa58
        NULL, // 0xa5c
        NULL, // 0xa60
        NULL, // 0xa64
        NULL, // 0xa68
        NULL, // 0xa6c
        NULL, // 0xa70
        NULL, // 0xa74
        NULL, // 0xa78
        NULL, // 0xa7c
        NULL, // 0xa80
        NULL, // 0xa84
        NULL, // 0xa88
        NULL, // 0xa8c
        NULL, // 0xa90
        NULL, // 0xa94
        NULL, // 0xa98
        NULL, // 0xa9c
        NULL, // 0xaa0
        NULL, // 0xaa4
        NULL, // 0xaa8
        NULL, // 0xaac
        NULL, // 0xab0
        NULL, // 0xab4
        NULL, // 0xab8
        NULL, // 0xabc
        NULL, // 0xac0
        NULL, // 0xac4
        NULL, // 0xac8
        NULL, // 0xacc
        NULL, // 0xad0
        NULL, // 0xad4
        NULL, // 0xad8
        NULL, // 0xadc
        NULL, // 0xae0
        NULL, // 0xae4
        NULL, // 0xae8
        NULL, // 0xaec
        NULL, // 0xaf0
        NULL, // 0xaf4
        NULL, // 0xaf8
        NULL, // 0xafc
        NULL, // 0xb00
        NULL, // 0xb04
        NULL, // 0xb08
        NULL, // 0xb0c
        NULL, // 0xb10
        NULL, // 0xb14
        NULL, // 0xb18
        NULL, // 0xb1c
        NULL, // 0xb20
        NULL, // 0xb24
        NULL, // 0xb28
        NULL, // 0xb2c
        NULL, // 0xb30
        NULL, // 0xb34
        NULL, // 0xb38
        NULL, // 0xb3c
        NULL, // 0xb40
        NULL, // 0xb44
        NULL, // 0xb48
        NULL, // 0xb4c
        NULL, // 0xb50
        NULL, // 0xb54
        NULL, // 0xb58
        NULL, // 0xb5c
        NULL, // 0xb60
        NULL, // 0xb64
        NULL, // 0xb68
        NULL, // 0xb6c
        NULL, // 0xb70
        NULL, // 0xb74
        NULL, // 0xb78
        NULL, // 0xb7c
        NULL, // 0xb80
        NULL, // 0xb84
        NULL, // 0xb88
        NULL, // 0xb8c
        NULL, // 0xb90
        NULL, // 0xb94
        NULL, // 0xb98
        NULL, // 0xb9c
        NULL, // 0xba0
        NULL, // 0xba4
        NULL, // 0xba8
        NULL, // 0xbac
        NULL, // 0xbb0
        NULL, // 0xbb4
        NULL, // 0xbb8
        NULL, // 0xbbc
        NULL, // 0xbc0
        NULL, // 0xbc4
        NULL, // 0xbc8
        NULL, // 0xbcc
        NULL, // 0xbd0
        NULL, // 0xbd4
        NULL, // 0xbd8
        NULL, // 0xbdc
        NULL, // 0xbe0
        NULL, // 0xbe4
        NULL, // 0xbe8
        NULL, // 0xbec
        NULL, // 0xbf0
        NULL, // 0xbf4
        NULL, // 0xbf8
        NULL, // 0xbfc
        REP32(gbaemu::gba::cpu::impl::thumb::mls::opcode_stmia), // 0xc00
        REP32(gbaemu::gba::cpu::impl::thumb::mls::opcode_ldmia), // 0xc80
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_beq), // 0xd00
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_bne), // 0xd10
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_bcs), // 0xd20
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_bcc), // 0xd30
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_bmi), // 0xd40
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_bpl), // 0xd50
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_bvs), // 0xd60
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_bvc), // 0xd70
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_bhi), // 0xd80
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_bls), // 0xd90
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_bge), // 0xda0
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_blt), // 0xdb0
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_bgt), // 0xdc0
        REP4(gbaemu::gba::cpu::impl::thumb::cb::opcode_ble), // 0xdd0
        NULL, // 0xde0
        NULL, // 0xde4
        NULL, // 0xde8
        NULL, // 0xdec
        NULL, // 0xdf0
        NULL, // 0xdf4
        NULL, // 0xdf8
        NULL, // 0xdfc
        NULL, // 0xe00
        NULL, // 0xe04
        NULL, // 0xe08
        NULL, // 0xe0c
        NULL, // 0xe10
        NULL, // 0xe14
        NULL, // 0xe18
        NULL, // 0xe1c
        NULL, // 0xe20
        NULL, // 0xe24
        NULL, // 0xe28
        NULL, // 0xe2c
        NULL, // 0xe30
        NULL, // 0xe34
        NULL, // 0xe38
        NULL, // 0xe3c
        NULL, // 0xe40
        NULL, // 0xe44
        NULL, // 0xe48
        NULL, // 0xe4c
        NULL, // 0xe50
        NULL, // 0xe54
        NULL, // 0xe58
        NULL, // 0xe5c
        NULL, // 0xe60
        NULL, // 0xe64
        NULL, // 0xe68
        NULL, // 0xe6c
        NULL, // 0xe70
        NULL, // 0xe74
        NULL, // 0xe78
        NULL, // 0xe7c
        NULL, // 0xe80
        NULL, // 0xe84
        NULL, // 0xe88
        NULL, // 0xe8c
        NULL, // 0xe90
        NULL, // 0xe94
        NULL, // 0xe98
        NULL, // 0xe9c
        NULL, // 0xea0
        NULL, // 0xea4
        NULL, // 0xea8
        NULL, // 0xeac
        NULL, // 0xeb0
        NULL, // 0xeb4
        NULL, // 0xeb8
        NULL, // 0xebc
        NULL, // 0xec0
        NULL, // 0xec4
        NULL, // 0xec8
        NULL, // 0xecc
        NULL, // 0xed0
        NULL, // 0xed4
        NULL, // 0xed8
        NULL, // 0xedc
        NULL, // 0xee0
        NULL, // 0xee4
        NULL, // 0xee8
        NULL, // 0xeec
        NULL, // 0xef0
        NULL, // 0xef4
        NULL, // 0xef8
        NULL, // 0xefc
        REP32(gbaemu::gba::cpu::impl::thumb::lbl::opcode_bl), // 0xf00
        REP32(gbaemu::gba::cpu::impl::thumb::lbl::opcode_blh), // 0xf80
    };

    thumbOpcodeCallback_t decode(uint16_t opcode) {
        return opcodeDecoderTable[opcode >> 6];
    }
}
