# 1 "src/gbaemu/gba/cpu/impl/arm/hsdt.cpp"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/gbaemu/gba/cpu/impl/arm/hsdt.cpp"
# 1 "D:/msys64/mingw64/include/c++/9.2.0/cstdint" 1 3
# 32 "D:/msys64/mingw64/include/c++/9.2.0/cstdint" 3

# 33 "D:/msys64/mingw64/include/c++/9.2.0/cstdint" 3

# 1 "D:/msys64/mingw64/include/c++/9.2.0/x86_64-w64-mingw32/bits/c++config.h" 1 3
# 252 "D:/msys64/mingw64/include/c++/9.2.0/x86_64-w64-mingw32/bits/c++config.h" 3

# 252 "D:/msys64/mingw64/include/c++/9.2.0/x86_64-w64-mingw32/bits/c++config.h" 3
namespace std
{
typedef long long unsigned int size_t;
typedef long long int ptrdiff_t;

typedef decltype(nullptr) nullptr_t;

} // namespace std
# 274 "D:/msys64/mingw64/include/c++/9.2.0/x86_64-w64-mingw32/bits/c++config.h" 3
namespace std
{
inline namespace __cxx11 __attribute__((__abi_tag__("cxx11"))) {}
} // namespace std
namespace __gnu_cxx
{
inline namespace __cxx11 __attribute__((__abi_tag__("cxx11"))) {}
} // namespace __gnu_cxx
# 524 "D:/msys64/mingw64/include/c++/9.2.0/x86_64-w64-mingw32/bits/c++config.h" 3
# 1 "D:/msys64/mingw64/include/c++/9.2.0/x86_64-w64-mingw32/bits/os_defines.h" 1 3
# 525 "D:/msys64/mingw64/include/c++/9.2.0/x86_64-w64-mingw32/bits/c++config.h" 2 3

# 1 "D:/msys64/mingw64/include/c++/9.2.0/x86_64-w64-mingw32/bits/cpu_defines.h" 1 3
# 528 "D:/msys64/mingw64/include/c++/9.2.0/x86_64-w64-mingw32/bits/c++config.h" 2 3
# 39 "D:/msys64/mingw64/include/c++/9.2.0/cstdint" 2 3

# 1 "D:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/9.2.0/include/stdint.h" 1 3 4
# 9 "D:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/9.2.0/include/stdint.h" 3 4
# 1 "D:/msys64/mingw64/x86_64-w64-mingw32/include/stdint.h" 1 3 4
# 28 "D:/msys64/mingw64/x86_64-w64-mingw32/include/stdint.h" 3 4
# 1 "D:/msys64/mingw64/x86_64-w64-mingw32/include/crtdefs.h" 1 3 4
# 10 "D:/msys64/mingw64/x86_64-w64-mingw32/include/crtdefs.h" 3 4
# 1 "D:/msys64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 1 3 4
# 10 "D:/msys64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
# 1 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 1 3 4
# 10 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
# 1 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw_mac.h" 1 3 4
# 98 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw_mac.h" 3 4

# 107 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw_mac.h" 3 4

# 11 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 1 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw_secapi.h" 1 3 4
# 44 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw_secapi.h" 3 4
extern "C++"
{
    template <bool __test, typename __dsttype>
    struct __if_array;
    template <typename __dsttype>
    struct __if_array<true, __dsttype>
    {
        typedef __dsttype __type;
    };
}
# 12 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 284 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
# 1 "D:/msys64/mingw64/x86_64-w64-mingw32/include/vadefs.h" 1 3 4
# 9 "D:/msys64/mingw64/x86_64-w64-mingw32/include/vadefs.h" 3 4
# 1 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 1 3 4
# 609 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
# 1 "D:/msys64/mingw64/x86_64-w64-mingw32/include/sdks/_mingw_directx.h" 1 3 4
# 610 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 1 "D:/msys64/mingw64/x86_64-w64-mingw32/include/sdks/_mingw_ddk.h" 1 3 4
# 611 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 10 "D:/msys64/mingw64/x86_64-w64-mingw32/include/vadefs.h" 2 3 4

#pragma pack(push, _CRT_PACKING)

extern "C"
{

    typedef __builtin_va_list __gnuc_va_list;

    typedef __gnuc_va_list va_list;
# 99 "D:/msys64/mingw64/x86_64-w64-mingw32/include/vadefs.h" 3 4
}

#pragma pack(pop)
# 285 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 569 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
extern "C"
{
# 580 "D:/msys64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
    void __attribute__((__cdecl__)) __debugbreak(void);
    extern __inline__ __attribute__((__always_inline__, __gnu_inline__)) void __attribute__((__cdecl__)) __debugbreak(void)
    {
        __asm__ __volatile__("int {$}3"
                             :);
    }

    const char *__mingw_get_crt_info(void);
}
# 11 "D:/msys64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 2 3 4

#pragma pack(push, _CRT_PACKING)
# 40 "D:/msys64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
__extension__ typedef unsigned long long size_t;
# 50 "D:/msys64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
__extension__ typedef long long ssize_t;

typedef size_t rsize_t;
# 67 "D:/msys64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
__extension__ typedef long long intptr_t;
# 80 "D:/msys64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
__extension__ typedef unsigned long long uintptr_t;
# 93 "D:/msys64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
__extension__ typedef long long ptrdiff_t;
# 111 "D:/msys64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
typedef unsigned short wint_t;
typedef unsigned short wctype_t;

typedef int errno_t;

typedef long __time32_t;

__extension__ typedef long long __time64_t;
# 143 "D:/msys64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
typedef __time64_t time_t;
# 435 "D:/msys64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
struct threadlocaleinfostruct;
struct threadmbcinfostruct;
typedef struct threadlocaleinfostruct *pthreadlocinfo;
typedef struct threadmbcinfostruct *pthreadmbcinfo;
struct __lc_time_data;

typedef struct localeinfo_struct
{
    pthreadlocinfo locinfo;
    pthreadmbcinfo mbcinfo;
} _locale_tstruct, *_locale_t;

typedef struct tagLC_ID
{
    unsigned short wLanguage;
    unsigned short wCountry;
    unsigned short wCodePage;
} LC_ID, *LPLC_ID;

typedef struct threadlocaleinfostruct
{

    int refcount;
    unsigned int lc_codepage;
    unsigned int lc_collate_cp;
    unsigned long lc_handle[6];
    LC_ID lc_id[6];
    struct
    {
        char *locale;
        wchar_t *wlocale;
        int *refcount;
        int *wrefcount;
    } lc_category[6];
    int lc_clike;
    int mb_cur_max;
    int *lconv_intl_refcount;
    int *lconv_num_refcount;
    int *lconv_mon_refcount;
    struct lconv *lconv;
    int *ctype1_refcount;
    unsigned short *ctype1;
    const unsigned short *pctype;
    const unsigned char *pclmap;
    const unsigned char *pcumap;
    struct __lc_time_data *lc_time_curr;

} threadlocinfo;

#pragma pack(pop)
# 11 "D:/msys64/mingw64/x86_64-w64-mingw32/include/crtdefs.h" 2 3 4
# 29 "D:/msys64/mingw64/x86_64-w64-mingw32/include/stdint.h" 2 3 4

# 1 "D:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/9.2.0/include/stddef.h" 1 3 4
# 1 "D:/msys64/mingw64/x86_64-w64-mingw32/include/stddef.h" 1 3 4
# 13 "D:/msys64/mingw64/x86_64-w64-mingw32/include/stddef.h" 3 4
extern "C"
{

    __attribute__((__dllimport__)) extern int *__attribute__((__cdecl__)) _errno(void);

    errno_t __attribute__((__cdecl__)) _set_errno(int _Value);
    errno_t __attribute__((__cdecl__)) _get_errno(int *_Value);

    __attribute__((__dllimport__)) extern unsigned long __attribute__((__cdecl__)) __threadid(void);

    __attribute__((__dllimport__)) extern uintptr_t __attribute__((__cdecl__)) __threadhandle(void);
}
# 2 "D:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/9.2.0/include/stddef.h" 2 3 4
# 416 "D:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/9.2.0/include/stddef.h" 3 4
typedef struct
{
    long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
    long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
# 427 "D:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/9.2.0/include/stddef.h" 3 4
} max_align_t;

typedef decltype(nullptr) nullptr_t;
# 33 "D:/msys64/mingw64/x86_64-w64-mingw32/include/stdint.h" 2 3 4

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned uint32_t;
__extension__ typedef long long int64_t;
__extension__ typedef unsigned long long uint64_t;

typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;
typedef short int_least16_t;
typedef unsigned short uint_least16_t;
typedef int int_least32_t;
typedef unsigned uint_least32_t;
__extension__ typedef long long int_least64_t;
__extension__ typedef unsigned long long uint_least64_t;

typedef signed char int_fast8_t;
typedef unsigned char uint_fast8_t;
typedef short int_fast16_t;
typedef unsigned short uint_fast16_t;
typedef int int_fast32_t;
typedef unsigned int uint_fast32_t;
__extension__ typedef long long int_fast64_t;
__extension__ typedef unsigned long long uint_fast64_t;

__extension__ typedef long long intmax_t;
__extension__ typedef unsigned long long uintmax_t;
# 10 "D:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/9.2.0/include/stdint.h" 2 3 4
# 42 "D:/msys64/mingw64/include/c++/9.2.0/cstdint" 2 3

namespace std
{

using ::int16_t;
using ::int32_t;
using ::int64_t;
using ::int8_t;

using ::int_fast16_t;
using ::int_fast32_t;
using ::int_fast64_t;
using ::int_fast8_t;

using ::int_least16_t;
using ::int_least32_t;
using ::int_least64_t;
using ::int_least8_t;

using ::intmax_t;
using ::intptr_t;

using ::uint16_t;
using ::uint32_t;
using ::uint64_t;
using ::uint8_t;

using ::uint_fast16_t;
using ::uint_fast32_t;
using ::uint_fast64_t;
using ::uint_fast8_t;

using ::uint_least16_t;
using ::uint_least32_t;
using ::uint_least64_t;
using ::uint_least8_t;

using ::uintmax_t;
using ::uintptr_t;

} // namespace std
# 2 "src/gbaemu/gba/cpu/impl/arm/hsdt.cpp" 2

# 1 "src/gbaemu/gba/cpu.hpp" 1

# 1 "src/gbaemu/gbaemu.hpp" 1

# 13 "src/gbaemu/gbaemu.hpp"

# 13 "src/gbaemu/gbaemu.hpp"
namespace gbaemu
{
typedef struct
{
    const char *biosPath;
    const char *romPath;
    const char *forcedSaveType;
} conf_t;

extern conf_t conf;
} // namespace gbaemu
# 4 "src/gbaemu/gba/cpu.hpp" 2
# 1 "src/gbaemu/gba/cpu/decoder.hpp" 1

namespace gbaemu::gba::cpu::decoder
{
typedef void (*armOpcodeCallback_t)(uint32_t opcode);
typedef void (*thumbOpcodeCallback_t)(uint16_t opcode);
} // namespace gbaemu::gba::cpu::decoder
# 5 "src/gbaemu/gba/cpu.hpp" 2

namespace gbaemu::gba::cpu
{
typedef union {
    uint32_t value;

    struct
    {
        uint32_t mode : 5;
        uint32_t flagT : 1;
        uint32_t flagF : 1;
        uint32_t flagI : 1;
        uint32_t reserved : 19;
        uint32_t flagQ : 1;
        uint32_t flagV : 1;
        uint32_t flagC : 1;
        uint32_t flagZ : 1;
        uint32_t flagN : 1;
    } __attribute__((packed))
# 21 "src/gbaemu/gba/cpu.hpp"
    fields;
} psr_t;

typedef enum
{
    PSR_MODE_USR = 0b10000,
    PSR_MODE_FIQ = 0b10001,
    PSR_MODE_IRQ = 0b10010,
    PSR_MODE_SVC = 0b10011,
    PSR_MODE_ABT = 0b10111,
    PSR_MODE_UND = 0b11011,
    PSR_MODE_SYS = 0b11111,
} mode_t;

typedef struct
{
    uint32_t opcode;
    gbaemu::gba::cpu::decoder::armOpcodeCallback_t function;
} ARMInstruction_t;

typedef struct
{
    uint16_t opcode;
    gbaemu::gba::cpu::decoder::thumbOpcodeCallback_t function;
} thumbInstruction_t;

typedef enum
{
    PIPELINE_FLUSH,
    PIPELINE_FETCH,
    PIPELINE_FETCH_DECODE,
    PIPELINE_FETCH_DECODE_EXECUTE
} pipelineStage_t;

typedef struct
{
    union {
        uint32_t fetchedOpcodeARM;
        uint16_t fetchedOpcodeThumb;
    };

    union {
        gbaemu::gba::cpu::decoder::armOpcodeCallback_t decodedOpcodeARM;
        gbaemu::gba::cpu::decoder::thumbOpcodeCallback_t decodedOpcodeThumb;
    };

    union {
        uint32_t decodedOpcodeARM_value;
        uint16_t decodedOpcodeThumb_value;
    };

    pipelineStage_t pipelineStage;
} pipeline_t;

enum
{
    MODE_USR,
    MODE_FIQ,
    MODE_SVC,
    MODE_ABT,
    MODE_IRQ,
    MODE_UND,
    MODE_SYS,
    MODE_INV
};

enum
{
    CPU_MODE_ARM,
    CPU_MODE_THUMB
};

enum
{
    CPU_CONDITION_EQ,
    CPU_CONDITION_NE,
    CPU_CONDITION_CS,
    CPU_CONDITION_CC,
    CPU_CONDITION_MI,
    CPU_CONDITION_PL,
    CPU_CONDITION_VS,
    CPU_CONDITION_VC,
    CPU_CONDITION_HI,
    CPU_CONDITION_LS,
    CPU_CONDITION_GE,
    CPU_CONDITION_LT,
    CPU_CONDITION_GT,
    CPU_CONDITION_LE,
    CPU_CONDITION_AL,
    CPU_CONDITION_NV,
};

enum
{
    CPU_REG_R0,
    CPU_REG_R1,
    CPU_REG_R2,
    CPU_REG_R3,
    CPU_REG_R4,
    CPU_REG_R5,
    CPU_REG_R6,
    CPU_REG_R7,
    CPU_REG_R8,
    CPU_REG_R9,
    CPU_REG_R10,
    CPU_REG_R11,
    CPU_REG_R12,
    CPU_REG_SP,
    CPU_REG_LR,
    CPU_REG_PC,
};

typedef struct
{
    bool flagC;
    uint32_t result;
} shifter_t;

extern psr_t cpsr;
extern shifter_t shifter;

extern void init();
extern void cycle();
extern bool checkCondition(uint32_t opcode);

extern uint32_t registerRead(int reg);
extern void registerWrite(int reg, uint32_t value);
extern void performJump(uint32_t address);
extern psr_t readSPSR();
extern void writeSPSR(psr_t value);
extern void writeSPSR(uint32_t value);
extern void writeCPSR(psr_t value);
extern void writeCPSR(uint32_t value);

extern void displayState();
} // namespace gbaemu::gba::cpu
# 4 "src/gbaemu/gba/cpu/impl/arm/hsdt.cpp" 2
# 1 "src/gbaemu/gba/cpu/impl/arm/hsdt.hpp" 1

# 32 "src/gbaemu/gba/cpu/impl/arm/hsdt.hpp"
namespace gbaemu::gba::cpu::impl::arm::HSDT
{
void opcode_ldrh(uint32_t opcode);
void opcode_ldrhu(uint32_t opcode);
void opcode_ldrhp(uint32_t opcode);
void opcode_ldrhpw(uint32_t opcode);
void opcode_ldrhpu(uint32_t opcode);
void opcode_ldrhpuw(uint32_t opcode);
void opcode_ldrs(uint32_t opcode);
void opcode_ldrsu(uint32_t opcode);
void opcode_ldrsp(uint32_t opcode);
void opcode_ldrspw(uint32_t opcode);
void opcode_ldrspu(uint32_t opcode);
void opcode_ldrspuw(uint32_t opcode);
void opcode_ldrsh(uint32_t opcode);
void opcode_ldrshu(uint32_t opcode);
void opcode_ldrshp(uint32_t opcode);
void opcode_ldrshpw(uint32_t opcode);
void opcode_ldrshpu(uint32_t opcode);
void opcode_ldrshpuw(uint32_t opcode);
void opcode_ldrih(uint32_t opcode);
void opcode_ldrihu(uint32_t opcode);
void opcode_ldrihp(uint32_t opcode);
void opcode_ldrihpw(uint32_t opcode);
void opcode_ldrihpu(uint32_t opcode);
void opcode_ldrihpuw(uint32_t opcode);
void opcode_ldris(uint32_t opcode);
void opcode_ldrisu(uint32_t opcode);
void opcode_ldrisp(uint32_t opcode);
void opcode_ldrispw(uint32_t opcode);
void opcode_ldrispu(uint32_t opcode);
void opcode_ldrispuw(uint32_t opcode);
void opcode_ldrish(uint32_t opcode);
void opcode_ldrishu(uint32_t opcode);
void opcode_ldrishp(uint32_t opcode);
void opcode_ldrishpw(uint32_t opcode);
void opcode_ldrishpu(uint32_t opcode);
void opcode_ldrishpuw(uint32_t opcode);
void opcode_strh(uint32_t opcode);
void opcode_strhu(uint32_t opcode);
void opcode_strhp(uint32_t opcode);
void opcode_strhpw(uint32_t opcode);
void opcode_strhpu(uint32_t opcode);
void opcode_strhpuw(uint32_t opcode);
void opcode_strih(uint32_t opcode);
void opcode_strihu(uint32_t opcode);
void opcode_strihp(uint32_t opcode);
void opcode_strihpw(uint32_t opcode);
void opcode_strihpu(uint32_t opcode);
void opcode_strihpuw(uint32_t opcode);
} // namespace gbaemu::gba::cpu::impl::arm::HSDT
# 5 "src/gbaemu/gba/cpu/impl/arm/hsdt.cpp" 2
# 1 "src/gbaemu/gba/mmu.hpp" 1

namespace gbaemu::gba::mmu
{
extern uint8_t read8(uint32_t address);
extern void write8(uint32_t address, uint8_t value);
extern uint16_t read16(uint32_t address);
extern void write16(uint32_t address, uint16_t value);
extern uint32_t read32(uint32_t address);
extern void write32(uint32_t address, uint32_t value);
} // namespace gbaemu::gba::mmu
# 6 "src/gbaemu/gba/cpu/impl/arm/hsdt.cpp" 2
# 94 "src/gbaemu/gba/cpu/impl/arm/hsdt.cpp"
namespace gbaemu::gba::cpu::impl::arm::hsdt
{
void opcode_strh(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
    address -= offset;
    registerWrite(Rn, address);
}
void opcode_strih(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
    address -= offset;
    registerWrite(Rn, address);
}
void opcode_strhu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
    address += offset;
    registerWrite(Rn, address);
}
void opcode_strihu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
    address += offset;
    registerWrite(Rn, address);
}
void opcode_strhp(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address -= offset;
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
}
void opcode_strihp(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address -= offset;
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
}
void opcode_strhpw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address -= offset;
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
    registerWrite(Rn, address);
}
void opcode_strihpw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address -= offset;
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
    registerWrite(Rn, address);
}
void opcode_strhpu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address += offset;
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
}
void opcode_strihpu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address += offset;
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
}
void opcode_strhpuw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address += offset;
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
    registerWrite(Rn, address);
}
void opcode_strihpuw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t value = registerRead(Rd);
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address += offset;
    mmu::write16(address & 0xfffffffe, (uint16_t)value);
    registerWrite(Rn, address);
}

void opcode_ldrh(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    address -= offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrih(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    address -= offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrhu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    address += offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrihu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    address += offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrhp(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address -= offset;
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    registerWrite(Rd, value);
}
void opcode_ldrihp(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address -= offset;
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    registerWrite(Rd, value);
}
void opcode_ldrhpw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address -= offset;
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrihpw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address -= offset;
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrhpu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address += offset;
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    registerWrite(Rd, value);
}
void opcode_ldrihpu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address += offset;
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    registerWrite(Rd, value);
}
void opcode_ldrhpuw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address += offset;
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrihpuw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address += offset;
    const uint32_t value = mmu::read16(address & 0xfffffffe);
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}

void opcode_ldrs(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    address -= offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldris(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    address -= offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrsu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    address += offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrisu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    address += offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrsp(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address -= offset;
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    registerWrite(Rd, value);
}
void opcode_ldrisp(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address -= offset;
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    registerWrite(Rd, value);
}
void opcode_ldrspw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address -= offset;
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrispw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address -= offset;
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrspu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address += offset;
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    registerWrite(Rd, value);
}
void opcode_ldrispu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address += offset;
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    registerWrite(Rd, value);
}
void opcode_ldrspuw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address += offset;
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrispuw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address += offset;
    const uint32_t value = (uint32_t)((int8_t)mmu::read8(address));
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}

void opcode_ldrsh(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    address -= offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrish(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    address -= offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrshu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    address += offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrishu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    address += offset;
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrshp(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address -= offset;
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    registerWrite(Rd, value);
}
void opcode_ldrishp(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address -= offset;
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    registerWrite(Rd, value);
}
void opcode_ldrshpw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address -= offset;
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrishpw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address -= offset;
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrshpu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address += offset;
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    registerWrite(Rd, value);
}
void opcode_ldrishpu(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address += offset;
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    registerWrite(Rd, value);
}
void opcode_ldrshpuw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t Rm = opcode & 0x0000000f;
    const uint32_t offset = registerRead(Rm);
    address += offset;
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
void opcode_ldrishpuw(uint32_t opcode)
{
    const uint32_t Rn = (opcode & 0x000f0000) >> 16;
    const uint32_t Rd = (opcode & 0x0000f000) >> 12;
    const uint32_t Rn_v = registerRead(Rn);
    uint32_t address = Rn_v;
    const uint32_t offset = ((opcode & 0x00000f00) >> 4) | (opcode & 0x0000000f);
    address += offset;
    const uint32_t value = (uint32_t)((int16_t)mmu::read16(address & 0xfffffffe));
    registerWrite(Rn, address);
    registerWrite(Rd, value);
}
} // namespace gbaemu::gba::cpu::impl::arm::hsdt
