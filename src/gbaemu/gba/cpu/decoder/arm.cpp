#include <cstddef>
#include <cstdint>

#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/cpu/decoder.hpp>
#include <gbaemu/gba/cpu/decoder/arm.hpp>
#include <gbaemu/gba/cpu/impl/arm/bdt.hpp>
#include <gbaemu/gba/cpu/impl/arm/branch.hpp>
#include <gbaemu/gba/cpu/impl/arm/dataproc.hpp>
#include <gbaemu/gba/cpu/impl/arm/hsdt.hpp>
#include <gbaemu/gba/cpu/impl/arm/mlmal.hpp>
#include <gbaemu/gba/cpu/impl/arm/mma.hpp>
#include <gbaemu/gba/cpu/impl/arm/nop.hpp>
#include <gbaemu/gba/cpu/impl/arm/psrt.hpp>
#include <gbaemu/gba/cpu/impl/arm/sds.hpp>
#include <gbaemu/gba/cpu/impl/arm/sdt.hpp>
#include <gbaemu/gba/cpu/impl/arm/swi.hpp>

#define REP2(x) x, x
#define REP4(x) REP2(x), REP2(x)
#define REP8(x) REP4(x), REP4(x)
#define REP16(x) REP8(x), REP8(x)
#define REP32(x) REP16(x), REP16(x)
#define REP64(x) REP32(x), REP32(x)
#define REP128(x) REP64(x), REP64(x)
#define REP256(x) REP128(x), REP128(x)

#define OPCODE_ALU_VARIANTS(op) \
    op##_LSL_Rm_0, \
    op##_LSL_Rm_1, \
    op##_LSR_Rm_0, \
    op##_LSR_Rm_1, \
    op##_ASR_Rm_0, \
    op##_ASR_Rm_1, \
    op##_ROR_Rm_0, \
    op##_ROR_Rm_1

#define OPCODE_ALU_VARIANTS2(op) \
    OPCODE_ALU_VARIANTS(op), \
    op##_LSL_Rm_0, \
    NULL, \
    op##_LSR_Rm_0, \
    NULL, \
    op##_ASR_Rm_0, \
    NULL, \
    op##_ROR_Rm_0, \
    NULL

namespace gbaemu::gba::cpu::decoder::arm {
    const armOpcodeCallback_t opcodeDecoderTable[4096] = {
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_and), // 0x000
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_and_LSL_Rm_0, // 0x008
        gbaemu::gba::cpu::impl::arm::mma::opcode_mul, // 0x009
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_and_LSR_Rm_0, // 0x00a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strh, // 0x00b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_and_ASR_Rm_0, // 0x00c
        NULL, // 0x00d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_and_ROR_Rm_0, // 0x00e
        NULL, // 0x00f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_ands), // 0x010
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_ands_LSL_Rm_0, // 0x018
        gbaemu::gba::cpu::impl::arm::mma::opcode_muls, // 0x019
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_ands_LSR_Rm_0, // 0x01a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrh, // 0x01b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_ands_ASR_Rm_0, // 0x01c
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrs, // 0x01d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_ands_ROR_Rm_0, // 0x01e
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrsh, // 0x01f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_eor), // 0x020
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_eor_LSL_Rm_0, // 0x028
        gbaemu::gba::cpu::impl::arm::mma::opcode_mla, // 0x029
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_eor_LSR_Rm_0, // 0x02a
        NULL, // 0x02b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_eor_ASR_Rm_0, // 0x02c
        NULL, // 0x02d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_eor_ROR_Rm_0, // 0x02e
        NULL, // 0x02f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_eors), // 0x030
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_eors_LSL_Rm_0, // 0x038
        gbaemu::gba::cpu::impl::arm::mma::opcode_mlas, // 0x039
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_eors_LSR_Rm_0, // 0x03a
        NULL, // 0x03b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_eors_ASR_Rm_0, // 0x03c
        NULL, // 0x03d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_eors_ROR_Rm_0, // 0x03e
        NULL, // 0x03f
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sub_LSL_Rm_0, // 0x040
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sub_LSL_Rm_0, // 0x048
        NULL, // 0x049
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_sub), // 0x04a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strih, // 0x04b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sub_ASR_Rm_0, // 0x04c
        NULL, // 0x04d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sub_ROR_Rm_0, // 0x04e
        NULL, // 0x04f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_subs), // 0x050
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_subs_LSL_Rm_0, // 0x058
        NULL, // 0x059
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_subs_LSR_Rm_0, // 0x05a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrih, // 0x05b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_subs_ASR_Rm_0, // 0x05c
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldris, // 0x05d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_subs_ROR_Rm_0, // 0x05e
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrish, // 0x05f
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::dataproc::opcode_rsb), // 0x060
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::dataproc::opcode_rsbs), // 0x070
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_add), // 0x080
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_add_LSL_Rm_0, // 0x088
        gbaemu::gba::cpu::impl::arm::mlmal::opcode_umull, // 0x089
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_add_LSR_Rm_0, // 0x08a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strhu, // 0x08b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_add_ASR_Rm_0, // 0x08c
        NULL, // 0x08d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_add_ROR_Rm_0, // 0x08e
        NULL, // 0x08f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_adds), // 0x090
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adds_LSL_Rm_0, // 0x098
        gbaemu::gba::cpu::impl::arm::mlmal::opcode_umulls, // 0x099
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adds_LSR_Rm_0, // 0x09a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrhu, // 0x09b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adds_ASR_Rm_0, // 0x09c
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrsu, // 0x09d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adds_ROR_Rm_0, // 0x09e
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrshu, // 0x09f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_adc), // 0x0a0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adc_LSL_Rm_0, // 0x0a8
        gbaemu::gba::cpu::impl::arm::mlmal::opcode_umlal, // 0x0a9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adc_LSR_Rm_0, // 0x0aa
        NULL, // 0x0ab
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adc_ASR_Rm_0, // 0x0ac
        NULL, // 0x0ad
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adc_ROR_Rm_0, // 0x0ae
        NULL, // 0x0af
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_adcs), // 0x0b0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adcs_LSL_Rm_0, // 0x0b8
        gbaemu::gba::cpu::impl::arm::mlmal::opcode_umlals, // 0x0b9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adcs_LSR_Rm_0, // 0x0ba
        NULL, // 0x0bb
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adcs_ASR_Rm_0, // 0x0bc
        NULL, // 0x0bd
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_adcs_ROR_Rm_0, // 0x0be
        NULL, // 0x0bf
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbc), // 0x0c0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbc_LSL_Rm_0, // 0x0c8
        gbaemu::gba::cpu::impl::arm::mlmal::opcode_smull, // 0x0c9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbc_LSR_Rm_0, // 0x0ca
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strihu, // 0x0cb
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbc_ASR_Rm_0, // 0x0cc
        NULL, // 0x0cd
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbc_ROR_Rm_0, // 0x0ce
        NULL, // 0x0cf
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbcs), // 0x0d0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbcs_LSL_Rm_0, // 0x0d8
        gbaemu::gba::cpu::impl::arm::mlmal::opcode_smulls, // 0x0d9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbcs_LSR_Rm_0, // 0x0da
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrihu, // 0x0db
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbcs_ASR_Rm_0, // 0x0dc
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrisu, // 0x0dd
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbcs_ROR_Rm_0, // 0x0de
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrishu, // 0x0df
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_rsc), // 0x0e0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_rsc_LSL_Rm_0, // 0x0e8
        gbaemu::gba::cpu::impl::arm::mlmal::opcode_smlal, // 0x0e9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_rsc_LSR_Rm_0, // 0x0ea
        NULL, // 0x0eb
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_rsc_ASR_Rm_0, // 0x0ec
        NULL, // 0x0ed
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_rsc_ROR_Rm_0, // 0x0ee
        NULL, // 0x0ef
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_rscs), // 0x0f0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_rscs_LSL_Rm_0, // 0x0f8
        gbaemu::gba::cpu::impl::arm::mlmal::opcode_smlals, // 0x0f9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_rscs_LSR_Rm_0, // 0x0fa
        NULL, // 0x0fb
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_rscs_ASR_Rm_0, // 0x0fc
        NULL, // 0x0fd
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_rscs_ROR_Rm_0, // 0x0fe
        NULL, // 0x0ff
        gbaemu::gba::cpu::impl::arm::psrt::opcode_mrs_cpsr, // 0x100
        NULL, // 0x101
        NULL, // 0x102
        NULL, // 0x103
        NULL, // 0x104
        NULL, // 0x105
        NULL, // 0x106
        NULL, // 0x107
        NULL, // 0x108
        gbaemu::gba::cpu::impl::arm::sds::opcode_swp, // 0x109
        NULL, // 0x10a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strhp, // 0x10b
        NULL, // 0x10c
        NULL, // 0x10d
        NULL, // 0x10e
        NULL, // 0x10f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_tst), // 0x110
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_tst_LSL_Rm_0, // 0x118
        NULL, // 0x119
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_tst_LSR_Rm_0, // 0x11a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrhp, // 0x11b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_tst_ASR_Rm_0, // 0x11c
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrsp, // 0x11d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_tst_ROR_Rm_0, // 0x11e
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrshp, // 0x11f
        gbaemu::gba::cpu::impl::arm::psrt::opcode_msr_cpsr, // 0x120
        gbaemu::gba::cpu::impl::arm::branch::bx, // 0x121
        NULL, // 0x122
        NULL, // 0x123
        NULL, // 0x124
        NULL, // 0x125
        NULL, // 0x126
        NULL, // 0x127
        NULL, // 0x128
        NULL, // 0x129
        NULL, // 0x12a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strhpw, // 0x12b
        NULL, // 0x12c
        NULL, // 0x12d
        NULL, // 0x12e
        NULL, // 0x12f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_teq), // 0x130
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_teq_LSL_Rm_0, // 0x138
        NULL, // 0x139
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_teq_LSR_Rm_0, // 0x13a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrhpw, // 0x13b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_teq_ASR_Rm_0, // 0x13c
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrspw, // 0x13d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_teq_ROR_Rm_0, // 0x13e
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrshpw, // 0x13f
        gbaemu::gba::cpu::impl::arm::psrt::opcode_mrs_spsr, // 0x140
        NULL, // 0x141
        NULL, // 0x142
        NULL, // 0x143
        NULL, // 0x144
        NULL, // 0x145
        NULL, // 0x146
        NULL, // 0x147
        NULL, // 0x148
        gbaemu::gba::cpu::impl::arm::sds::opcode_swpb, // 0x149
        NULL, // 0x14a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strihp, // 0x14b
        NULL, // 0x14c
        NULL, // 0x14d
        NULL, // 0x14e
        NULL, // 0x14f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmp), // 0x150
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmp_LSL_Rm_0, // 0x158
        NULL, // 0x159
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmp_LSR_Rm_0, // 0x15a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrihp, // 0x15b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmp_ASR_Rm_0, // 0x15c
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrisp, // 0x15d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmp_ROR_Rm_0, // 0x15e
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrishp, // 0x15f
        gbaemu::gba::cpu::impl::arm::psrt::opcode_msr_spsr, // 0x160
        NULL, // 0x161
        NULL, // 0x162
        NULL, // 0x163
        NULL, // 0x164
        NULL, // 0x165
        NULL, // 0x166
        NULL, // 0x167
        NULL, // 0x168
        NULL, // 0x169
        NULL, // 0x16a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strihpw, // 0x16b
        NULL, // 0x16c
        NULL, // 0x16d
        NULL, // 0x16e
        NULL, // 0x16f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmn), // 0x170
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmn_LSL_Rm_0, // 0x178
        NULL, // 0x179
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmn_LSR_Rm_0, // 0x17a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrihpw, // 0x17b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmn_ASR_Rm_0, // 0x17c
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrispw, // 0x17d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmn_ROR_Rm_0, // 0x17e
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrishpw, // 0x17f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_orr), // 0x180
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_orr_LSL_Rm_0, // 0x188
        NULL, // 0x189
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_orr_LSR_Rm_0, // 0x18a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strhpu, // 0x18b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_orr_ASR_Rm_0, // 0x18c
        NULL, // 0x18d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_orr_ROR_Rm_0, // 0x18e
        NULL, // 0x18f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_orrs), // 0x190
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_orrs_LSL_Rm_0, // 0x198
        NULL, // 0x199
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_orrs_LSR_Rm_0, // 0x19a
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrhpu, // 0x19b
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_orrs_ASR_Rm_0, // 0x19c
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrspu, // 0x19d
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_orrs_ROR_Rm_0, // 0x19e
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrshpu, // 0x19f
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_mov), // 0x1a0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mov_LSL_Rm_0, // 0x1a8
        NULL, // 0x1a9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mov_LSR_Rm_0, // 0x1aa
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strhpuw, // 0x1ab
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mov_ASR_Rm_0, // 0x1ac
        NULL, // 0x1ad
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mov_ROR_Rm_0, // 0x1ae
        NULL, // 0x1af
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_movs), // 0x1b0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_movs_LSL_Rm_0, // 0x1b8
        NULL, // 0x1b9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_movs_LSR_Rm_0, // 0x1ba
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrhpuw, // 0x1bb
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_movs_ASR_Rm_0, // 0x1bc
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrspuw, // 0x1bd
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_movs_ROR_Rm_0, // 0x1be
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrshpuw, // 0x1bf
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_bic), // 0x1c0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_bic_LSL_Rm_0, // 0x1c8
        NULL, // 0x1c9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_bic_LSR_Rm_0, // 0x1ca
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strihpu, // 0x1cb
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_bic_ASR_Rm_0, // 0x1cc
        NULL, // 0x1cd
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_bic_ROR_Rm_0, // 0x1ce
        NULL, // 0x1cf
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_bics), // 0x1d0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_bics_LSL_Rm_0, // 0x1d8
        NULL, // 0x1d9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_bics_LSR_Rm_0, // 0x1da
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrihpu, // 0x1db
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_bics_ASR_Rm_0, // 0x1dc
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrispu, // 0x1dd
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_bics_ROR_Rm_0, // 0x1de
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrishpu, // 0x1df
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvn), // 0x1e0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvn_LSL_Rm_0, // 0x1e8
        NULL, // 0x1e9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvn_LSR_Rm_0, // 0x1ea
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_strihpuw, // 0x1eb
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvn_ASR_Rm_0, // 0x1ec
        NULL, // 0x1ed
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvn_ROR_Rm_0, // 0x1ee
        NULL, // 0x1ef
        OPCODE_ALU_VARIANTS(gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvns), // 0x1f0
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvns_LSL_Rm_0, // 0x1f8
        NULL, // 0x1f9
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvns_LSR_Rm_0, // 0x1fa
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrihpuw, // 0x1fb
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvns_ASR_Rm_0, // 0x1fc
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrispuw, // 0x1fd
        gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvns_ROR_Rm_0, // 0x1fe
        gbaemu::gba::cpu::impl::arm::hsdt::opcode_ldrishpuw, // 0x1ff
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_and_Immediate), // 0x200
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_ands_Immediate), // 0x210
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_eor_Immediate), // 0x220
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_eors_Immediate), // 0x230
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_sub_Immediate), // 0x240
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_subs_Immediate), // 0x250
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_rsb_Immediate), // 0x260
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_rsbs_Immediate), // 0x270
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_add_Immediate), // 0x280
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_adds_Immediate), // 0x290
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_adc_Immediate), // 0x2a0
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_adcs_Immediate), // 0x2b0
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbc_Immediate), // 0x2c0
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_sbcs_Immediate), // 0x2d0
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_rsc_Immediate), // 0x2e0
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_rscs_Immediate), // 0x2f0
        REP16(NULL), // 0x300
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_tst_Immediate), // 0x310
        REP16(gbaemu::gba::cpu::impl::arm::psrt::opcode_msr_cpsr_Immediate), // 0x320
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_teq_Immediate), // 0x330
        REP16(NULL), // 0x340
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmp_Immediate), // 0x350
        REP16(gbaemu::gba::cpu::impl::arm::psrt::opcode_msr_spsr_Immediate), // 0x360
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_cmn_Immediate), // 0x370
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_orr_Immediate), // 0x380
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_orrs_Immediate), // 0x390
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_mov_Immediate), // 0x3a0
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_movs_Immediate), // 0x3b0
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_bic_Immediate), // 0x3c0
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_bics_Immediate), // 0x3d0
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvn_Immediate), // 0x3e0
        REP16(gbaemu::gba::cpu::impl::arm::dataproc::opcode_mvns_Immediate), // 0x3f0
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_str_Immediate), // 0x400
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldr_Immediate), // 0x410
        REP32(NULL), // 0x420
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_strb_Immediate), // 0x440
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrb_Immediate), // 0x450
        REP32(NULL), // 0x460
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_stru_Immediate), // 0x480
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldru_Immediate), // 0x490
        REP32(NULL), // 0x4a0
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_strub_Immediate), // 0x4c0
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrub_Immediate), // 0x4d0
        REP32(NULL), // 0x4e0
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_strp_Immediate), // 0x500
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrp_Immediate), // 0x510
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpw_Immediate), // 0x520
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpw_Immediate), // 0x530
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpb_Immediate), // 0x540
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpb_Immediate), // 0x550
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpbw_Immediate), // 0x560
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpbw_Immediate), // 0x570
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpu_Immediate), // 0x580
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpu_Immediate), // 0x590
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpuw_Immediate), // 0x5a0
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpuw_Immediate), // 0x5b0
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpub_Immediate), // 0x5c0
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpub_Immediate), // 0x5d0
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpubw_Immediate), // 0x5e0
        REP16(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpubw_Immediate), // 0x5f0
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_str), // 0x600
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldr), // 0x610
        REP32(NULL), // 0x620
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_strb), // 0x640
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrb), // 0x650
        REP32(NULL), // 0x660
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_stru), // 0x680
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldru), // 0x690
        REP32(NULL), // 0x6a0
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_strub), // 0x6c0
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrub), // 0x6d0
        REP32(NULL), // 0x6e0
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_strp), // 0x700
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrp), // 0x710
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpw), // 0x720
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpw), // 0x730
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpb), // 0x740
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpb), // 0x750
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpbw), // 0x760
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpbw), // 0x770
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpu), // 0x780
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpu), // 0x790
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpuw), // 0x7a0
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpuw), // 0x7b0
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpub), // 0x7c0
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpub), // 0x7d0
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_strpubw), // 0x7e0
        OPCODE_ALU_VARIANTS2(gbaemu::gba::cpu::impl::arm::sdt::opcode_ldrpubw), // 0x7f0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmda), // 0x800
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmda), // 0x810
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmdaw), // 0x820
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmdaw), // 0x830
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmdas), // 0x840
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmdas), // 0x850
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmdasw), // 0x860
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmdasw), // 0x870
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmia), // 0x880
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmia), // 0x890
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmiaw), // 0x8a0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmiaw), // 0x8b0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmias), // 0x8c0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmias), // 0x8d0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmiasw), // 0x8e0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmiasw), // 0x8f0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmdb), // 0x900
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmdb), // 0x910
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmdbw), // 0x920
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmdbw), // 0x930
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmdbs), // 0x940
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmdbs), // 0x950
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmdbsw), // 0x960
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmdbsw), // 0x970
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmib), // 0x980
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmib), // 0x990
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmibw), // 0x9a0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmibw), // 0x9b0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmibs), // 0x9c0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmibs), // 0x9d0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_stmibsw), // 0x9e0
        REP16(gbaemu::gba::cpu::impl::arm::bdt::opcode_ldmibsw), // 0x9f0
        REP256(gbaemu::gba::cpu::impl::arm::branch::b), // 0xa00
        REP256(gbaemu::gba::cpu::impl::arm::branch::bl), // 0xb00
        REP256(gbaemu::gba::cpu::impl::arm::nop::opcode_nop), // 0xc00
        REP256(gbaemu::gba::cpu::impl::arm::nop::opcode_nop), // 0xd00
        REP256(gbaemu::gba::cpu::impl::arm::nop::opcode_nop), // 0xe00
        REP256(gbaemu::gba::cpu::impl::arm::swi::swi) // 0xf00
    };

    armOpcodeCallback_t decode(uint32_t opcode) {
        return opcodeDecoderTable[((opcode >> 16) & 0xff0) | ((opcode >> 4) & 0xf)];
    }
}
