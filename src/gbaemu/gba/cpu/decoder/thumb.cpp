#include <cstddef>
#include <cstdint>

#include <gbaemu/gba/cpu/decoder.hpp>
#include <gbaemu/gba/cpu/impl/thumb/addspimm.hpp>
#include <gbaemu/gba/cpu/impl/thumb/addsub.hpp>
#include <gbaemu/gba/cpu/impl/thumb/alu.hpp>
#include <gbaemu/gba/cpu/impl/thumb/cb.hpp>
#include <gbaemu/gba/cpu/impl/thumb/hrobe.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lbl.hpp>
#include <gbaemu/gba/cpu/impl/thumb/ldstrhw.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lsio.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lsro.hpp>
#include <gbaemu/gba/cpu/impl/thumb/lssebh.hpp>
#include <gbaemu/gba/cpu/impl/thumb/mcas.hpp>
#include <gbaemu/gba/cpu/impl/thumb/mls.hpp>
#include <gbaemu/gba/cpu/impl/thumb/msr.hpp>
#include <gbaemu/gba/cpu/impl/thumb/pcrldr.hpp>
#include <gbaemu/gba/cpu/impl/thumb/ppr.hpp>
#include <gbaemu/gba/cpu/impl/thumb/sprldstr.hpp>
#include <gbaemu/gba/cpu/impl/thumb/swi.hpp>

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
        REP8(gbaemu::gba::cpu::impl::thumb::lsro::opcode_str), // 0x500
        REP8(gbaemu::gba::cpu::impl::thumb::lssebh::opcode_strh), // 0x520
        REP8(gbaemu::gba::cpu::impl::thumb::lsro::opcode_strb), // 0x540
        REP8(gbaemu::gba::cpu::impl::thumb::lssebh::opcode_ldrh), // 0x560
        REP8(gbaemu::gba::cpu::impl::thumb::lsro::opcode_ldr), // 0x580
        REP8(gbaemu::gba::cpu::impl::thumb::lssebh::opcode_ldrsb), // 0x5a0
        REP8(gbaemu::gba::cpu::impl::thumb::lsro::opcode_ldrb), // 0x5c0
        REP8(gbaemu::gba::cpu::impl::thumb::lssebh::opcode_ldrsh), // 0x5e0
        REP32(gbaemu::gba::cpu::impl::thumb::lsio::opcode_str), // 0x600
        REP32(gbaemu::gba::cpu::impl::thumb::lsio::opcode_ldr), // 0x680
        REP32(gbaemu::gba::cpu::impl::thumb::lsio::opcode_strb), // 0x700
        REP32(gbaemu::gba::cpu::impl::thumb::lsio::opcode_ldrb), // 0x780
        REP32(gbaemu::gba::cpu::impl::thumb::ldrstrhw::opcode_strh), // 0x800
        REP32(gbaemu::gba::cpu::impl::thumb::ldrstrhw::opcode_ldrh), // 0x880
        REP32(gbaemu::gba::cpu::impl::thumb::sprldstr::opcode_str), // 0x900
        REP32(gbaemu::gba::cpu::impl::thumb::sprldstr::opcode_ldr), // 0x980
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
        REP4(gbaemu::gba::cpu::impl::thumb::addspimm::opcode_add_sp_imm),
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
        REP4(gbaemu::gba::cpu::impl::thumb::ppr::opcode_push), // 0xb40
        REP4(gbaemu::gba::cpu::impl::thumb::ppr::opcode_push_lr), // 0xb50
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
        REP4(gbaemu::gba::cpu::impl::thumb::ppr::opcode_pop), // 0xbc0
        REP4(gbaemu::gba::cpu::impl::thumb::ppr::opcode_pop_pc), // 0xbd0
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
        REP4(gbaemu::gba::cpu::impl::thumb::swi::opcode_swi), // 0xdf0
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
