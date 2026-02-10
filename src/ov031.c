#include "DatMgr.h"
#include "Display.h"
#include "EasyTask.h"
#include "Memory.h"
#include "System.h"
#include "common_data.h"
#include <types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 unk18;
    u16 unk1A;
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    u16 unk22;
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2A;
    u16 unk2C;
    u16 unk2E;
} Ov031FontInfo;

typedef struct {
    u16 unk0;
    u16 unk2;
    u8  pad4[0x18];
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    u16 unk22;
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2A;
} Ov031D11C;

typedef struct {
    void* unk0;
    u8    unk4;
    u8    unk5;
} Ov031Db20;

typedef struct {
    u8   pad0[0x16];
    s16  unk16;
    u8   pad18[0x4];
    u16* unk1C;
} Ov031A220Arg;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
} Ov031A220Entry;

typedef struct {
    u8  pad0[0x8];
    s32 unk8;
    s32 unkC;
} Ov031EntryBounds;

typedef struct {
    u8  pad0[0x14];
    s32 unk14;
} Ov031GlyphSub28;

typedef struct {
    u8  pad0[0x0A];
    s16 unkA;
    u8  pad0C[0x4];
    s16 unk10;
    s16 unk12;
} Ov031GlyphSub2C;

typedef struct {
    u32              unk0;
    u8               pad4[0x12];
    s16              unk16;
    u8               pad18[0x04];
    u16*             unk1C;
    u8               pad20[0x08];
    Ov031GlyphSub28* unk28;
    Ov031GlyphSub2C* unk2C;
    u8               pad30[0x4];
    u32              unk34;
} Ov031Glyph;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    void (*unkC)(void*, s32);
    void (*unk10)(void*, s32);
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} Ov031MenuEntry;

typedef struct {
    s32   unk0;
    void* unk4;
    u8    pad8[0x1A0];
    u16   unk1A8;
    u16   unk1AA;
    u16   unk1AC;
    u16   unk1AE;
    void* unk1B0;
    s32   unk1B4;
    s32   unk1B8;
    s32   unk1BC;
    u16   unk1C8;
    u16   unk1CA;
    u16   unk1CC;
    u16   unk1CE;
    s32   unk1D0;
    s32   unk1D4;
    s32   unk1D8;
    s32   unk1DC;
    u16   unk1E0;
} Ov031MenuState;

typedef struct {
    u8  pad[0x2C];
    u16 unk2C;
} Ov031FirmwareOverlay;

typedef struct {
    s32 unk0;
    s32 unk4;
} Ov031PackEntry;

static inline s32 Ov031_RotateRight(s32 value, u32 shift) {
    return (s32)(((u32)value >> shift) | ((u32)value << (32U - shift)));
}

static inline u32 Ov031_MultHi(u32 a, u32 b) {
    return (u32)(((u64)a * (u64)b) >> 32);
}

void  DC_PurgeAll(void);                                                                                 /* extern */
void  GetDsFirmwareUserSettings(void*);                                                                  /* extern */
void  OS_WaitForever(void);                                                                              /* extern */
void* func_02002c18(void*);                                                                              /* extern */
void  func_0203b270(const void* src, void* dst, s32 size);                                               /* extern */
void  func_0203b28c(s32 arg0, void* dst, s32 size);                                                      /* extern */
void  func_0203b2d0(s32 arg0, void* dst, s32 size);                                                      /* extern */
void  func_0203b31c(const void* src, void* dst, s32 size);                                               /* extern */
void  func_0203b454(const void* src, void* dst, s32 size);                                               /* extern */
u16   func_ov031_0210a5fc(s16* arg0, const u16* arg1, s32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6); /* static */
s32   func_ov031_0210a7e0(u16 arg0);                                                                     /* static */
void  func_ov031_0210a808(UnkOv31Struct* arg0, s32 arg1, s32 arg2, s32 arg3);                            /* static */
u16*  func_ov031_0210c0d4(u16* arg0, s32 arg1);                                                          /* static */
u16   func_ov031_0210c194(UnkOv31Struct* arg0, s32 arg1);                                                /* static */
u32   func_ov031_0210c248(UnkOv31Struct* arg0, s32 arg1, s32 arg2, u32 arg3);                            /* static */
u32   func_ov031_0210c320(UnkOv31Struct* arg0, s32 arg1);                                                /* static */
u32   func_ov031_0210c46c(UnkOv31Struct* arg0, u16* arg1, u16* arg2);                                    /* static */
u16   func_ov031_0210c4b8(UnkOv31Struct* arg0, u16* arg1);                                               /* static */
u16   func_ov031_0210c4cc(UnkOv31Struct* arg0, u16* arg1, u32 arg2);                                     /* static */
extern SystemControl        SysControl;
static u16                  data_ov031_0210d160[4] = {1, 2, 3, 0};
static const Ov031FontInfo  data_ov031_0210d178;
static const Ov031FontInfo  data_ov031_0210d1a8;
static const Ov031FontInfo  data_ov031_0210d1d8;
static const Ov031FontInfo  data_ov031_0210d208;
static const Ov031FontInfo* data_ov031_0210d168[4] = {
    &data_ov031_0210d178,
    &data_ov031_0210d1a8,
    &data_ov031_0210d208,
    &data_ov031_0210d1d8,
};
static const Ov031FontInfo data_ov031_0210d178        = {0x0005, 0x0001, 0x0002, 0x0003, 0xFFFF, 0xFFFF, 0x0001, 0x0001,
                                                         0x0002, 0x0003, 0xFFFF, 0xFFFF, 0x000E, 0x0000, 0x0001, 0x0000,
                                                         0x0001, 0x0000, 0x0008, 0x0008, 0x0010, 0x0010, 0x0010, 0x0010};
static const Ov031FontInfo data_ov031_0210d1a8        = {0x0006, 0x0001, 0x0002, 0x0003, 0x0004, 0xFFFF, 0x0002, 0x0001,
                                                         0x0002, 0x0003, 0x0004, 0xFFFF, 0x000E, 0x0000, 0x0001, 0x0000,
                                                         0x0001, 0x0000, 0x000A, 0x000A, 0x000A, 0x000A, 0x0019, 0x0019};
static const Ov031FontInfo data_ov031_0210d1d8        = {0x0009, 0x0001, 0x0002, 0xFFFF, 0xFFFF, 0xFFFF, 0x0008, 0x0001,
                                                         0x0002, 0xFFFF, 0xFFFF, 0xFFFF, 0x000E, 0x0000, 0x0001, 0x0000,
                                                         0x0000, 0x0000, 0x000A, 0x000C, 0x000A, 0x000C, 0x0019, 0x0015};
static const Ov031FontInfo data_ov031_0210d208        = {0x0007, 0x0001, 0x0002, 0xFFFF, 0xFFFF, 0xFFFF, 0x0003, 0x0001,
                                                         0x0002, 0xFFFF, 0xFFFF, 0xFFFF, 0x000E, 0x0000, 0x0001, 0x0000,
                                                         0x0001, 0x0000, 0x000C, 0x000C, 0x0010, 0x0010, 0x0010, 0x0010};
static s32                 data_ov031_0210d238[0x209] = {
    0x4C006C,   0x01B5E00A, 0x150035,   0,          0xE730D6,   0x010AFF0C, 0x019400E7, 0x013B00B0, 0x863068,   0x590079,
    0x01C2E017, 0x220042,   0,          0xF430E3,   0x01A100F4, 0xBD30AC,   0x015902C6, 0x933075,   0,          0,
    0x2F004F,   0x012EFF5D, 0x010130F0, 0x01502191, 0xCA30B9,   0x017800CA, 0xA03082,   0x0161203A, 0x69304B,   0x3C005C,
    0x01293014, 0x50025,    0,          0xD730C6,   0x013600D7, 0xAD308F,   0,          0x763058,   0x490069,   0x011D2010,
    0x120032,   0x0114FF40, 0xE430D3,   0x0128FF09, 0x0112309C, 0x014A25BD, 0x833065,   0x560076,   0x0126201D, 0x1F003F,
    0x0152266F, 0xF130E0,   0x015A0152, 0xBA30A9,   0x019100E4, 0x903072,   0x019E00F1, 0x01B2E007, 0x2C004C,   0x01BFE014,
    0xFE30ED,   0x014725A0, 0xC730B6,   0x017500C7, 0x9D307F,   0,          0x663048,   0x390059,   0,          0x20022,
    0,          0xD430C3,   0x018200D4, 0xAA308C,   0,          0x733055,   0x460066,   0x01AFE004, 0xF002F,    0x012CFF3D,
    0xE130D0,   0x013EFF06, 0x018E00E1, 0,          0x803062,   0x530073,   0x0156201A, 0x1C003C,   0x014425C7, 0xEE30DD,
    0x019B00EE, 0xB730A6,   0x016C00B7, 0x8D306F,   0x01BCE011, 0,          0x290049,   0x01083001, 0xFB30EA,   0x01A800FB,
    0xC430B3,   0x017200C4, 0x9A307C,   0,          0x633045,   0x360056,   0,          0,          0,          0xD130C0,
    0x017F00D1, 0xA73089,   0,          0x703052,   0x430063,   0x01ACE001, 0xC002C,    0,          0xDE30CD,   0,
    0,          0,          0x7D305F,   0x500070,   0x01B9E00E, 0x190039,   0,          0xEB30DA,   0x019800EB, 0xB430A3,
    0x011300B4, 0x8A306C,   0x5D007D,   0x01C6E01B, 0x260046,   0,          0xF830E7,   0x0138FF1D, 0xC130B0,   0x016F00C1,
    0x973079,   0x01A500F8, 0x603042,   0x330053,   0x01C8E028, 0x010530F4, 0x015520AC, 0xCE30BD,   0x017C00CE, 0xA43086,
    0,          0x6D304F,   0x400060,   0,          0x90029,    0,          0xDB30CA,   0x018900DB, 0xB13093,   0,
    0x7A305C,   0x4D006D,   0x01B6E00B, 0x160036,   0,          0xE830D7,   0x0134FF0D, 0x019500E8, 0x013500B1, 0x873069,
    0x5A007A,   0x01402605, 0x230043,   0x014325CE, 0xF530E4,   0x010DFF1A, 0xBE30AD,   0x01A200F5, 0x943076,   0x01C3E018,
    0,          0x300050,   0x0120FF5E, 0x010230F1, 0x012F3008, 0xCB30BA,   0x014E2192, 0xA13083,   0x014C203B, 0x6A304C,
    0x3D005D,   0x012A3015, 0x60026,    0x017900CB, 0xD830C7,   0x018600D8, 0xAE3090,   0x016300A1, 0x773059,   0x4A006A,
    0x01620153, 0x130033,   0x01B3E008, 0xE530D4,   0x019200E5, 0x0118309D, 0x016800AE, 0x843066,   0x570077,   0x0157201E,
    0x200040,   0x014125CB, 0xF230E1,   0x019F00F2, 0xBB30AA,   0x01C0E015, 0x913073,   0x013A2234, 0,          0x2D004D,
    0x011A3005, 0xFF30EE,   0x012DFF5B, 0xC830B7,   0x014625A1, 0x9E3080,   0x017600C8, 0x673049,   0x3A005A,   0x014D3012,
    0x30023,    0x010C30FB, 0xD530C4,   0x0117FFE3, 0xAB308D,   0x018300D5, 0x743056,   0x470067,   0x01B0E005, 0x100030,
    0x0116FF3E, 0xE230D1,   0x018F00E2, 0x0153266D, 0,          0x813063,   0x540074,   0x01BDE012, 0x1D003D,   0,
    0xEF30DE,   0x019C00EF, 0xB830A7,   0,          0x8E3070,   0,          0,          0x2A004A,   0x01093002, 0xFC30EB,
    0x01A900FC, 0xC530B4,   0x017300C5, 0x9B307D,   0,          0x643046,   0x370057,   0,          0x20,       0,
    0xD230C1,   0x018000D2, 0xA8308A,   0,          0x713053,   0x440064,   0x01ADE002, 0xD002D,    0x012BFF3B, 0xDF30CE,
    0x018C00DF, 0x0154266A, 0x011500A8, 0x7E3060,   0x510071,   0x01232018, 0x1A003A,   0x01BAE00F, 0xEC30DB,   0x019900EC,
    0xB530A4,   0,          0x8B306D,   0x5E007E,   0x01C7E01C, 0x270047,   0,          0xF930E8,   0x01A600F9, 0xC230B1,
    0x017000C2, 0x98307A,   0x013C2032, 0x613043,   0x340054,   0x0131300C, 0x010630F5, 0,          0xCF30BE,   0x017D00CF,
    0xA53087,   0,          0x6E3050,   0x410061,   0,          0xA002A,    0,          0xDC30CB,   0x0110FF01, 0x018A00DC,
    0,          0x7B305D,   0x4E006E,   0x011C2015, 0x170037,   0x0139221E, 0xE930D8,   0x010BFF0E, 0xB230A1,   0x019600E9,
    0x88306A,   0x5B007B,   0x013F2606, 0x240044,   0x014225CF, 0xF630E5,   0x010EFF1B, 0xBF30AE,   0x015E2022, 0x953077,
    0x016D00BF, 0x01A300F6, 0x310051,   0x01303009, 0x010330F2, 0x01512193, 0xCC30BB,   0x017A00CC, 0xA23084,   0x01B7E00C,
    0x6B304D,   0x3E005E,   0x01C4E019, 0x70027,    0x014925B2, 0xD930C8,   0x018700D9, 0xAF3091,   0x016400A2, 0x78305A,
    0x4B006B,   0x01B4E009, 0x140034,   0,          0xE630D5,   0x0133FF0B, 0x0119309E, 0x019300E6, 0x853067,   0x580078,
    0x01C1E016, 0x210041,   0,          0xF330E2,   0x01A000F3, 0xBC30AB,   0,          0x923074,   0,          0,
    0x2E004E,   0x0121FF5C, 0x010030EF, 0x014F2190, 0xC930B8,   0x017700C9, 0x9F3081,   0x01602122, 0x68304A,   0x3B005B,
    0,          0x40024,    0x011B30FC, 0xD630C5,   0x018400D6, 0xAC308E,   0,          0x753057,   0x480068,   0x01B1E006,
    0x110031,   0x014B25BC, 0xE330D2,   0x0127FF08, 0x0111309B, 0x019000E3, 0x823064,   0x550075,   0x0125201C, 0x1E003E,
    0x01BEE013, 0xF030DF,   0x019D00F0, 0xB930A8,   0,          0x8F3071,   0,          0,          0x2B004B,   0,
    0xFD30EC,   0x01AA00FD, 0xC630B5,   0x017400C6, 0x9C307E,   0,          0x653047,   0x380058,   0,          0x10021,
    0,          0xD330C2,   0x015F02DC, 0xA9308B,   0x018100D3, 0x723054,   0x450065,   0x01AEE003, 0xE002E,    0x011FFF3C,
    0xE030CF,   0x018D00E0, 0,          0x016700A9, 0x7F3061,   0x520072,   0x01242019, 0x1B003B,   0x014525C6, 0xED30DC,
    0x019A00ED, 0xB630A5,   0x01BBE010, 0x8C306E,   0x01222026, 0,          0x280048,   0x01C93000, 0xFA30E9,   0x010FFF1F,
    0xC330B2,   0x017100C3, 0x99307B,   0x013D2033, 0x623044,   0x350055,   0x0132300D, 0x010730F6, 0x01A700FA, 0xD030BF,
    0x017E00D0, 0xA63088,   0,          0x6F3051,   0x420062,   0x01ABE000, 0xB002B,    0,          0xDD30CC,   0x018B00DD,
    0,          0,          0x7C305E,   0x4F006F,   0x01B8E00D, 0x180038,   0,          0xEA30D9,   0x011EFF0F, 0xB330A2,
    0x019700EA, 0x89306B,   0x5C007C,   0x01C5E01A, 0x250045,   0,          0xF730E6,   0x013700F7, 0xC030AF,   0x016E00C0,
    0x963078,   0,          0x5F3041,   0x320052,   0,          0x010430F3, 0,          0xCD30BC,   0x017B00CD, 0xA33085,
    0,          0x6C304E,   0x3F005F,   0,          0x80028,    0x014825B3, 0xDA30C9,   0x018800DA, 0xB03092,   0x016500A3,
    0x79305B,
};
static s8              data_ov031_0210da5c[0x14] = "Apl_Fuk/mestxt.bin";
static s8              data_ov031_0210da70[0x18] = "Apl_Fuk/mestable.bin";
static s8              data_ov031_0210da88[0x18] = "Apl_Fuk/Grp_Font.bin";
static s8              data_ov031_0210daa0[0x14] = "SysFont_GetAllocPal";
static s8              data_ov031_0210dab4[0x14] = "SysFont_GetMsgBuf";
static s8              data_ov031_0210dac8[0x24] = "SysFont_GetSysCodeBuf_from_DsCode";
static s8              data_ov031_0210daec[0x24] = "SysFont_GetUserDSCommentBufReturn";
static s8              data_ov031_0210db10[0x10] = "PrcMaster";
static Ov031Db20       data_ov031_0210db20;
static const Ov031D11C data_ov031_0210d11c = {
    0xFFD0, 0xFFFF, {0}, 0xFFD0, 0x005C, 0xFFD0, 0xFFFF, 0xFFD2, 0xFFFE, 0xFFD2, 0xFFFF,
};
static const BinIdentifier data_ov031_0210d120    = {0x1E, data_ov031_0210da70};       /* const */
static const BinIdentifier data_ov031_0210d128    = {0x80000002, data_ov031_0210da88}; /* const */
static const BinIdentifier data_ov031_0210d130[3] = {
    {      0x1E, data_ov031_0210da5c},
    {  0x5CFFD0,     (s8*)0xFFFFFFD0},
    {0xFFFEFFD2,     (s8*)0xFFFFFFD2},
}; /* const */

void func_ov031_0210a220(void* arg0, s32* arg1, s32* arg2) {
    const Ov031A220Arg*   glyph       = (const Ov031A220Arg*)arg0;
    const u16*            table       = glyph->unk1C;
    s32                   table_index = glyph->unk16 * 4;
    u16                   count       = table[(table_index + 1)];
    const Ov031A220Entry* entry       = (const Ov031A220Entry*)(table + table[table_index]);
    s32                   min_x       = 0x100;
    s32                   min_y       = 0xC0;
    u16                   i           = 0;

    if (count > 0) {
        const s32 sign_extend_9 = -0x200;
        const s32 sign_extend_8 = -0x100;
        do {
            func_02002c18((void*)entry);
            s32 raw_x = entry->unk4 & 0x1FF;
            s32 raw_y = entry->unk2 & 0xFF;
            entry++;
            if (raw_x & 0x100) {
                raw_x |= sign_extend_9;
            }
            if (raw_y & 0x80) {
                raw_y |= sign_extend_8;
            }
            if (min_x > raw_x) {
                min_x = raw_x;
            }
            if (min_y > raw_y) {
                min_y = raw_y;
            }
            i++;
        } while (count > i);
    }

    *arg1 = min_x;
    *arg2 = min_y;
}

u32 func_ov031_0210a2d8(s16* arg0, u32 arg1) {
    u32 value  = arg1;
    u32 digits = 0;

    if (value == 0) {
        arg0[0] = 0x10;
        return 1U;
    }

    while (value != 0) {
        value = (u32)(Ov031_MultHi(value, 0xCCCCCCCD) >> 3);
        digits++;
    }

    value = arg1;
    for (u32 i = 0; i < digits; i++) {
        u32 quotient           = (u32)(Ov031_MultHi(value, 0xCCCCCCCD) >> 3);
        u32 digit              = value - (quotient * 10U);
        arg0[(digits - i) - 1] = (s16)(digit + 0x10);
        value                  = quotient;
    }

    return digits;
}

u16 func_ov031_0210a384(s16* arg0, s32 arg1) {
    s32  value    = arg1;
    u16  digits   = 0;
    s32  work     = value;
    BOOL negative = FALSE;

    if (work < 0) {
        negative = TRUE;
    }

    if (work == 0) {
        arg0[0] = 0x10;
        return 1U;
    }

    while (work != 0) {
        work = work / 10;
        digits++;
    }

    if (negative) {
        s32 abs_value = -value;
        arg0[0]       = 0x208;
        for (u16 i = 0; i < digits; i++) {
            s32 quotient       = abs_value / 10;
            s32 digit          = abs_value - (quotient * 10);
            arg0[(digits - i)] = (s16)(digit + 0x10);
            abs_value          = quotient;
        }
        return (u16)(digits + 1);
    }

    for (u16 i = 0; i < digits; i++) {
        s32 quotient           = value / 10;
        s32 digit              = value - (quotient * 10);
        arg0[(digits - i) - 1] = (s16)(digit + 0x10);
        value                  = quotient;
    }

    return digits;
}

u16 func_ov031_0210a4c4(u16* arg0, const u16* arg1) {
    u16 value = *arg1;
    u16 count = 0;

    if (value != 0xFFFF) {
        do {
            *arg0++ = value;
            value   = *++arg1;
            count++;
        } while (value != 0xFFFF);
    }
    return count;
}

s32 func_ov031_0210a500(u16* arg0, u16 arg1) {
    *arg0 = arg1;
    return 1;
}

u16 func_ov031_0210a50c(s16* arg0, s32 arg1, u16 arg2) {
    u16                       temp2[2];
    u16                       temp4[4];
    u16                       prefix = 0;
    s32                       scaled = arg1 >> 0xC;
    const volatile Ov031D11C* cfg    = &data_ov031_0210d11c;

    if (arg1 & 0x80000000) {
        *arg0  = 0x208;
        prefix = 1;
    }

    if (arg2 == 0) {
        temp2[0] = cfg->unk0;
        temp2[1] = cfg->unk2;
        return (u16)(prefix + func_ov031_0210a5fc(arg0, temp2, scaled, 0, 0, 0, 0));
    }
    temp4[0] = cfg->unk1C;
    temp4[1] = cfg->unk1E;
    temp4[2] = cfg->unk20;
    temp4[3] = cfg->unk22;

    return (u16)(prefix + func_ov031_0210a5fc(arg0, temp4, scaled, temp4[0], temp4[1], temp4[2], temp4[3]));
}

u16 func_ov031_0210a5fc(s16* arg0, const u16* arg1, s32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    const u16* src          = arg1;
    u16        count        = 0;
    const u32* args         = &arg3;
    const s16  control_base = (s16)0xFFD1;
    const s16  control_low  = (s16)(control_base - 0x11);
    const s16  control_num  = (s16)(control_base - 1);
    const s16  control_str  = (s16)(control_base + 1);
    const s16  control_char = (s16)(control_base + 2);

    (void)arg2;

    if (func_ov031_0210a7e0(*src) == 0) {
        do {
            s16 code = (s16)*src++;
            if (code <= control_base) {
                if (code < control_base) {
                    if (code <= (s16)(control_base - 0xD)) {
                        s32 temp = (s32)code - (s32)control_low;
                        if (temp >= 0 && temp <= 4) {
                            s32 code_s   = (s32)code;
                            s32 quotient = code_s / 10;
                            u16 mod      = (u16)(code_s - (quotient * 10));
                            u32 var_arg  = *args++;
                            count += func_ov031_0210a50c(arg0 + count, (s32)var_arg, mod);
                            continue;
                        }
                    } else if (code == control_num) {
                        u32 var_arg = *args++;
                        count += func_ov031_0210a2d8(arg0 + count, var_arg);
                        continue;
                    }
                } else {
                    u32 var_arg = *args++;
                    count += func_ov031_0210a384(arg0 + count, (s32)var_arg);
                    continue;
                }
            } else if (code <= control_str) {
                if (code == control_str) {
                    u32 var_arg = *args++;
                    count += func_ov031_0210a4c4((u16*)(arg0 + count), (const u16*)var_arg);
                    continue;
                }
            } else if (code == control_char) {
                u32 var_arg = *args++;
                count += func_ov031_0210a500((u16*)(arg0 + count), (u16)var_arg);
                continue;
            }

            arg0[count++] = (s16)code;
        } while (func_ov031_0210a7e0(*src) == 0);
    }

    arg0[count] = (s16)0xFFFF;
    return count;
}

s32 func_ov031_0210a7e0(u16 arg0) {
    if ((arg0 == 0xFFFD) || (arg0 == 0xFFFF)) {
        return 1;
    }
    return 0;
}

void func_ov031_0210a808(UnkOv31Struct* arg0, s32 arg1, s32 arg2, s32 arg3) {
    const Ov031FontInfo* temp_r5;
    const u16*           font_words;
    Data*                temp_r2;
    Data*                temp_r3;
    Data*                var_r0_2;
    Data*                var_r0_3;
    s16                  var_r0;
    u16                  temp_ip;
    u16                  temp_lr;
    u16                  temp_r5_2;
    u16                  temp_r7;
    u16                  var_r0_4;
    u16                  var_r1;
    u16                  var_r3;
    void*                temp_r1;
    void*                temp_r3_2;
    void*                temp_r7_2;
    void*                var_r3_2;
    void*                var_r7;

    arg0->unk0  = arg1;
    var_r0      = 1;
    arg0->unk4  = data_ov031_0210d168[arg1];
    font_words  = (const u16*)arg0->unk4;
    arg0->unk56 = 1;
    if (arg1 != 3) {
        var_r0 = 2;
    }
    arg0->unk58  = var_r0;
    arg0->unk5A  = 0xE;
    arg0->unk64  = arg2;
    arg0->unk_5C = 1;
    arg0->unk10  = NULL;
    arg0->unk68  = 1;
    arg0->unk1C  = 0;
    arg0->unk6C  = 0;
    arg0->unk74  = 0x200;
    arg0->unk_70 = 0;
    arg0->unk78  = 0x200;
    arg0->unk48  = 1;
    arg0->unk50  = 3;
    arg0->unk18  = DatMgr_LoadRawData(1, NULL, 0, &data_ov031_0210d120);
    temp_r5      = data_ov031_0210d168[arg1];
    if (arg3 != 0) {
        temp_r7 = temp_r5->unk0;
        if (BinMgr_FindById((s32)&data_ov031_0210d128) == NULL) {
            var_r0_2 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov031_0210d128, (s32)temp_r7, 1);
        } else {
            var_r0_2 = DatMgr_LoadPackEntryDirect(1, &data_ov031_0210d128, (s32)temp_r7, 1);
        }
        var_r1     = 0;
        arg0->unk8 = var_r0_2;
    loop_7:
        temp_lr             = font_words[var_r1 + 1];
        arg0->unk20[var_r1] = 0;
        if (temp_lr != 0xFFFF) {
            if ((arg2 == 0) && ((var_r1 == 1) || (var_r1 == 2) || (var_r1 == 3))) {
                goto block_19;
            }
            temp_r3 = arg0->unk8;
            if ((temp_r3 == NULL) || ((s32)temp_lr <= 0)) {
                var_r7 = NULL;
            } else {
                const u8* pack_base = (const u8*)temp_r3->buffer + 0x20;
                var_r7              = (void*)(pack_base + *(const u32*)(pack_base + (temp_lr * 8)));
            }
            arg0->unk20[var_r1] = (s32)var_r7;
            goto block_19;
        }
    block_19:
        var_r1 += 1;
        if ((u32)var_r1 >= 5U) {
            goto block_23;
        }
        goto loop_7;
    }
    var_r3     = 0;
    arg0->unk8 = NULL;
    do {
        arg0->unk20[var_r3] = 0;
        var_r3 += 1;
    } while ((u32)var_r3 < 5U);
block_23:
    temp_r5_2 = temp_r5->unkC;
    if (BinMgr_FindById((s32)&data_ov031_0210d128) == NULL) {
        var_r0_3 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov031_0210d128, (s32)temp_r5_2, 1);
    } else {
        var_r0_3 = DatMgr_LoadPackEntryDirect(1, &data_ov031_0210d128, (s32)temp_r5_2, 1);
    }
    arg0->unkC = var_r0_3;
    var_r0_4   = 0;
loop_27:
    temp_ip               = font_words[var_r0_4 + 7];
    arg0->unk34[var_r0_4] = 0;
    if (temp_ip != 0xFFFF) {
        if ((arg2 == 0) && ((var_r0_4 == 1) || (var_r0_4 == 2) || (var_r0_4 == 3))) {
            goto block_39;
        }
        temp_r2 = arg0->unkC;
        if ((temp_r2 == NULL) || ((s32)temp_ip <= 0)) {
            var_r3_2 = NULL;
        } else {
            const u8* pack_base = (const u8*)temp_r2->buffer + 0x20;
            var_r3_2            = (void*)(pack_base + *(const u32*)(pack_base + (temp_ip * 8)));
        }
        arg0->unk34[var_r0_4] = (s32)var_r3_2;
        goto block_39;
    }
block_39:
    var_r0_4 += 1;
    if ((u32)var_r0_4 >= 5U) {
        return;
    }
    goto loop_27;
}

void func_ov031_0210aa94(UnkOv31Struct* arg0) {
    func_ov031_0210a808(arg0, 1, 1, 1);
}

void func_ov031_0210aaac(UnkOv31Struct* arg0, s32 arg1, s32 arg2) {
    func_ov031_0210a808(arg0, arg1, arg2, 1);
}

void func_ov031_0210aabc(UnkOv31Struct* arg0) {
    Data* temp_r0;
    Data* temp_r0_2;
    Data* temp_r0_3;
    Data* temp_r0_4;

    temp_r0 = arg0->unk18;
    if (temp_r0 != NULL) {
        DatMgr_ReleaseData(temp_r0);
    }
    temp_r0_2 = arg0->unk8;
    if (temp_r0_2 != NULL) {
        DatMgr_ReleaseData(temp_r0_2);
        arg0->unk8 = NULL;
    }
    temp_r0_3 = arg0->unkC;
    if (temp_r0_3 != NULL) {
        DatMgr_ReleaseData(temp_r0_3);
        arg0->unkC = NULL;
    }
    temp_r0_4 = arg0->unk10;
    if (temp_r0_4 != NULL) {
        DatMgr_ReleaseData(temp_r0_4);
        arg0->unk10 = NULL;
    }
    arg0->unk_14 = NULL;
}

void func_ov031_0210ab28(UnkOv31Struct* arg0, s32 arg1, s32 arg2) {
    arg0->unk6C  = arg1;
    arg0->unk_70 = arg2;
}

void func_ov031_0210ab34(UnkOv31Struct* arg0, s32 arg1) {
    arg0->unk5A = (s8)arg1;
}

void func_ov031_0210ab3c(UnkOv31Struct* arg0, s32 arg1, s16 arg2) {
    arg0->unk48 = arg1;
    arg0->unk4C = arg2;
}

void func_ov031_0210ab48(UnkOv31Struct* arg0, s32 arg1, s16 arg2) {
    arg0->unk50 = arg1;
    arg0->unk54 = arg2;
}

void func_ov031_0210ab54(UnkOv31Struct* arg0, s32 arg1, s16 arg2) {
    arg0->unk68 = arg1;
    arg0->unk56 = arg2;
}

void func_ov031_0210ab60(UnkOv31Struct* arg0, s16 arg1) {
    arg0->unk58 = arg1;
}

void* func_ov031_0210ab68(s32 arg0) {
    Data* temp_r0_2;
    u16   temp_r2;
    void* temp_r0;
    void* var_r6;

    temp_r0_2 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov031_0210d128, 4, 1);
    temp_r2   = data_ov031_0210d160[arg0];
    if ((temp_r0_2 == NULL) || ((s32)temp_r2 <= 0)) {
        var_r6 = NULL;
    } else {
        const u8* pack_base = (const u8*)temp_r0_2->buffer + 0x20;
        var_r6              = (void*)(pack_base + *(const u32*)(pack_base + (temp_r2 * 8)));
    }
    temp_r0 = Mem_AllocHeapTail(&gDebugHeap, 0x20);
    Mem_SetSequence(&gDebugHeap, temp_r0, data_ov031_0210daa0);
    func_0203b31c(var_r6, temp_r0, 0x20);
    DatMgr_ReleaseData(temp_r0_2);
    return temp_r0;
}

void func_ov031_0210ac1c(UnkOv31Struct* arg0, u8* arg1, s32 arg2) {
    s32                  sp0;
    u8*                  temp_ip;
    s32                  temp_lr;
    s32                  temp_r1;
    s32                  temp_r5_2;
    s32                  var_r11;
    s32                  var_r2;
    s32                  var_r3;
    s32                  var_r3_2;
    s32                  var_r4;
    s32                  var_r9;
    s8*                  var_r5;
    s8*                  var_r7;
    u16                  temp_r5;
    u16                  temp_r6;
    u16                  temp_r7;
    u16                  temp_r7_2;
    u16                  temp_r8;
    u16                  temp_r9;
    u32                  temp_lr_2;
    u32                  temp_r1_2;
    u8                   temp_lr_3;
    const Ov031FontInfo* temp_r4;
    u8*                  glyph_base;

    temp_r4   = (const Ov031FontInfo*)arg0->unk4;
    temp_r9   = temp_r4->unk2C;
    temp_r7   = temp_r9 * temp_r4->unk2E;
    temp_r5   = (u16)(arg2 / (s32)temp_r7);
    temp_r7_2 = (u16)(arg2 % (s32)temp_r7);
    temp_r6   = temp_r4->unk24;
    temp_r8   = temp_r4->unk26;
    temp_r5_2 = arg0->unk20[temp_r5];
    sp0       = (s32)temp_r4->unk28;
    if (((u32)temp_r5 >= 5U) || (temp_r5_2 == 0)) {
        var_r3 = 0;
        if ((s32)temp_r8 <= 0) {
            return;
        }
        var_r4 = 0;
        do {
            var_r2 = 0;
            if ((s32)temp_r6 > 0) {
                var_r5 = arg1 + var_r4;
                do {
                    var_r2 += 1;
                    *var_r5 = 0;
                    var_r5 += 1;
                } while (var_r2 < (s32)temp_r6);
            }
            var_r3 += 1;
            var_r4 += 0x10;
        } while (var_r3 < (s32)temp_r8);
        return;
    }
    var_r9 = 0;
    if ((s32)temp_r8 <= 0) {
        return;
    }
    var_r3_2 = 0;
    do {
        var_r11 = 0;
        if ((s32)temp_r6 > 0) {
            temp_r1    = (temp_r4->unk2A * ((s32)temp_r7_2 / (s32)temp_r9)) + var_r9;
            var_r7     = (s8*)arg1 + var_r3_2;
            temp_r1_2  = temp_r1 << 0x1D;
            glyph_base = (u8*)temp_r5_2;
            do {
                temp_lr   = (sp0 * ((s32)temp_r7_2 % (s32)temp_r9)) + var_r11;
                temp_lr_2 = temp_lr & 7;
                temp_ip   = glyph_base +
                          ((((s32)(temp_lr + ((u32)(temp_lr >> 2) >> 0x1D)) >> 3) +
                            (((s32)(temp_r1 + ((u32)(temp_r1 >> 2) >> 0x1D)) >> 3) << 5))
                           << 5) +
                          ((s32)(temp_lr_2 + (temp_lr_2 >> 0x1F)) >> 1);
                temp_lr_3 = *(temp_ip + (temp_r1_2 >> 0x1B));
                var_r11 += 2;
                var_r7[0] = (s8)(temp_lr_3 & 0xF);
                var_r7[1] = (s8)(((s32) * (temp_ip + (temp_r1_2 >> 0x1B)) >> 4) & 0xF);
                var_r7 += 2;
            } while (var_r11 < (s32)temp_r6);
        }
        var_r9 += 1;
        var_r3_2 += 0x10;
    } while (var_r9 < (s32)temp_r8);
}

void func_ov031_0210ada0(UnkOv31Struct* arg0, u8* arg1, s32 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, u16 arg7, u16 arg8,
                         s32 arg9) {
    const Ov031FontInfo* font          = (const Ov031FontInfo*)arg0->unk4;
    const u16            tile_height   = font->unk26;
    const u16            tile_width    = font->unk24;
    const s32            bound_x       = arg0->unk74;
    const s32            bound_y       = arg0->unk78;
    const s32            base_x        = arg0->unk6C + (s32)arg7;
    const s32            base_y        = arg0->unk_70 + (s32)arg8;
    const s32            tile_width_px = (s32)arg5 * 8;
    const s32            tiles_per_row = (s32)((tile_width_px + ((u32)(tile_width_px >> 2) >> 0x1D)) >> 3);
    s32                  row           = 0;
    s32                  src_offset    = 0;
    u8*                  dst_base      = (u8*)arg2;

    (void)arg9;

    if ((s32)tile_height <= 0) {
        return;
    }

    while (row < (s32)tile_height) {
        if ((s32)tile_width > 0) {
            u8* src      = arg1 + src_offset;
            s32 y        = base_y + row;
            s32 y_div8   = (s32)((y + ((u32)(y >> 2) >> 0x1D)) >> 3);
            s32 row_tile = y_div8 * tiles_per_row;
            u32 y_shift  = (u32)(y << 0x1D);
            s32 x        = 0;

            while (x < (s32)tile_width) {
                if ((*src != 0) || (arg4 != 1)) {
                    s32 draw_x = base_x + x;
                    if ((draw_x < bound_x) && (draw_x < tile_width_px) && (y < bound_y) && (y < (s32)((u32)arg6 * 8U))) {
                        s32 x_div8         = (draw_x + ((u32)((s32)draw_x >> 2) >> 0x1D)) >> 3;
                        s32 tile_offset    = row_tile + x_div8;
                        s32 x_mod          = draw_x & 7;
                        s32 nibble_index   = (x_mod + (x_mod >> 31)) >> 1;
                        u8* dst            = dst_base + (tile_offset << 5) + nibble_index;
                        u8  existing       = dst[y_shift >> 0x1B];
                        int preserve_shift = (draw_x & 1) ? 0 : 4;
                        int insert_shift   = (draw_x & 1) ? 4 : 0;
                        u8  color          = (u8)((*src + arg3) - font->unk18);
                        u8  out            = (u8)((existing & (0xF << preserve_shift)) | (color << insert_shift));

                        dst[y_shift >> 0x1B] = out;
                    }
                }
                src += 1;
                x += 1;
            }
        }
        src_offset += 0x10;
        row += 1;
    }
}

void func_ov031_0210af48(UnkOv31Struct* arg0, u8* arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5, u16 arg6, u16 arg7, u16 arg8,
                         u32 arg9) {
    s32                  sp30;
    s32                  sp2C;
    s32                  sp28;
    s32                  sp24;
    s32                  sp20;
    s32                  sp1C;
    s32                  sp18;
    s32                  sp14;
    s32                  sp10;
    s32                  spC;
    s32                  sp8;
    u8*                  sp4;
    UnkOv31Struct*       sp0;
    s32                  temp_r0;
    s32                  temp_r0_2;
    s32                  temp_r1_2;
    s32                  temp_r1_3;
    s32                  temp_r2;
    s32                  temp_r2_2;
    s32                  temp_r4_3;
    s32                  var_lr;
    u16                  temp_r4_2;
    u16                  temp_r5;
    u32                  temp_r1;
    u32                  temp_r6;
    u8*                  var_r5;
    const Ov031FontInfo* temp_r4;

    sp0       = arg0;
    sp4       = arg1;
    sp24      = sp0->unk6C + arg6;
    sp1C      = sp0->unk_70 + arg7;
    temp_r4   = (const Ov031FontInfo*)arg0->unk4;
    temp_r5   = temp_r4->unk24;
    sp20      = sp0->unk74;
    temp_r4_2 = temp_r4->unk26;
    sp8       = arg2;
    spC       = arg3;
    sp18      = sp0->unk78;
    switch (arg9) {
        default:
            OS_WaitForever();
            break;
        case 0:
            sp30 = 0x20;
            sp28 = 0x20;
            break;
        case 1:
            sp30 = 0x40;
            sp28 = 0x20;
            break;
        case 2:
            sp30 = 0x20;
            sp28 = 0x40;
            break;
        case 3:
            sp30 = 0x40;
            sp28 = 0x40;
            break;
    }
    sp2C    = 0;
    temp_r0 = temp_r4_2 * arg8;
    sp10    = temp_r0;
    if (temp_r0 <= 0) {
        return;
    }
    temp_r4_3 = temp_r5 * arg8;
    sp14      = sp2C;
loop_15:
    var_lr = 0;
    if (temp_r4_3 > 0) {
        var_r5    = sp4 + sp14;
        temp_r6   = sp1C + sp2C;
        temp_r1   = temp_r6 >> 0x1F;
        temp_r2   = temp_r1 + Ov031_RotateRight((temp_r6 << 0x1D) - temp_r1, 0x1D);
        temp_r1_2 = temp_r2 + ((u32)(temp_r2 >> 2) >> 0x1D);
        temp_r2_2 = temp_r2 << 0x1D;
    loop_17:
        if ((*var_r5 == 0) && (arg5 == 1)) {
            goto block_33;
        }
        temp_r0_2 = sp24 + var_lr;
        if ((temp_r0_2 < sp20) && (temp_r0_2 < (s32)(sp30 * 8)) && ((s32)temp_r6 < sp18) && ((s32)temp_r6 < (s32)(sp28 * 8))) {
            u16* tile_map       = (u16*)sp8;
            u8*  tile_data      = (u8*)spC;
            u8   color          = (u8)((*var_r5 + arg4) - temp_r4->unk18);
            s32  x_sign         = temp_r0_2 >> 31;
            s32  x_rot          = x_sign + Ov031_RotateRight((temp_r0_2 << 0x1D) - x_sign, 0x1D);
            s32  row_offset     = ((s32)(temp_r6 + ((u32)((s32)temp_r6 >> 2) >> 0x1D)) >> 3) * sp30;
            s32  x_div8         = (temp_r0_2 + ((u32)((s32)temp_r0_2 >> 2) >> 0x1D)) >> 3;
            s32  map_index      = row_offset + x_div8;
            u16  tile           = (u16)(tile_map[map_index] & 0x3FF);
            s32  x_div8_rot     = (x_rot + ((u32)(x_rot >> 2) >> 0x1D)) >> 3;
            s32  offset         = x_div8_rot + (temp_r1_2 >> 3);
            s32  x_mod          = x_rot & 7;
            s32  nibble_index   = (x_mod + (x_mod >> 31)) >> 1;
            u8*  dst            = tile_data + (tile << 5) + (offset << 5) + nibble_index;
            u8   existing       = dst[temp_r2_2 >> 0x1B];
            int  preserve_shift = (temp_r0_2 & 1) ? 0 : 4;
            int  insert_shift   = (temp_r0_2 & 1) ? 4 : 0;
            u8   out            = (u8)((existing & (0xF << preserve_shift)) | (color << insert_shift));

            dst[temp_r2_2 >> 0x1B] = out;
            return;
        }
    block_33:
        var_r5 += 1;
        var_lr += 1;
        if (var_lr >= temp_r4_3) {
            goto block_34;
        }
        goto loop_17;
    }
block_34:
    sp14 += 0x10;
    temp_r1_3 = sp2C + 1;
    sp2C      = temp_r1_3;
    if (temp_r1_3 >= sp10) {
        return;
    }
    goto loop_15;
}

Ov031Db20* func_ov031_0210b1bc(Ov031Glyph* arg0, s32 arg1, s32 arg2, void* arg3, u32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32                   hit_x = 0;
    s32                   hit_y = 0;
    const Ov031A220Entry* entry = (const Ov031A220Entry*)arg3;
    const Ov031A220Entry* hit   = NULL;
    u16                   count = 0;

    if (arg5 > 0U) {
        do {
            const Ov031EntryBounds* bounds = (const Ov031EntryBounds*)func_02002c18((void*)entry);
            s32                     x      = entry->unk4 & 0x1FF;
            s32                     y      = entry->unk2 & 0xFF;

            if (x & 0x100) {
                x |= -0x200;
            }
            if (y & 0x80) {
                y |= -0x100;
            }

            s32 dx = x - arg6;
            s32 dy = y - arg7;
            if ((arg1 >= dx) && (arg2 >= dy)) {
                if ((arg1 < dx + bounds->unk8) && (arg2 < dy + bounds->unkC)) {
                    if (hit == NULL) {
                        hit   = entry;
                        hit_x = dx;
                        hit_y = dy;
                    }
                }
            }

            count++;
            entry++;
        } while (arg5 > count);
    }

    if (hit == NULL) {
        data_ov031_0210db20.unk0 = 0;
        return &data_ov031_0210db20;
    }

    const Ov031EntryBounds* bounds = (const Ov031EntryBounds*)func_02002c18((void*)hit);
    s32                     rel_x  = arg1 - hit_x;
    s32                     rel_y  = arg2 - hit_y;

    s32 base;
    if (arg8 != 0) {
        if (((arg0->unk0 << 0x1E) >> 0x1E) == 0) {
            base = (arg0->unk2C->unk10 << 5) + 0x06400000;
        } else {
            base = (arg0->unk2C->unk10 << 5) + 0x06600000;
        }
    } else {
        base = (s32)arg0->unk34 + 4;
    }

    s32 tile;
    if (arg8 != 0) {
        if (((arg0->unk0 << 0x1B) >> 0x1E) == 1) {
            tile = hit->unk6 & 0x3FF;
        } else {
            tile = hit->unk0 & 0x1FFF;
        }
    } else {
        tile = hit->unk0 & 0x1FFF;
    }

    s32 width                = (s32)((bounds->unk8 + ((u32)(bounds->unk8 >> 2) >> 0x1D)) >> 3);
    s32 offset               = ((rel_y >> 3) * width + (rel_x >> 3)) << 5;
    s32 fine                 = ((rel_x & 7) + ((rel_x & 7) >> 31)) >> 1;
    s32 byte_index           = (u32)(rel_y << 0x1D) >> 0x1B;
    data_ov031_0210db20.unk0 = (void*)(base + (tile << 5) + offset + fine + byte_index);
    data_ov031_0210db20.unk4 = (u8)((arg1 + hit_x) & 1);
    data_ov031_0210db20.unk5 = (u8)((arg2 + hit_y) & 1);
    return &data_ov031_0210db20;
}

void func_ov031_0210b3f4(UnkOv31Struct* arg0, u8* arg1, Ov031Glyph* arg2, s32 arg3, s32 arg4, u16 arg5, u16 arg6, s32 arg7,
                         s32 arg8) {
    s32                  sp4C;
    s32                  sp48;
    s32                  sp40;
    s32                  sp3C;
    s32                  sp38;
    s32                  sp34;
    s32                  sp30;
    s32                  sp2C;
    s32                  sp28;
    s32                  sp24;
    s32                  sp20;
    s32                  sp1C;
    s32                  sp18;
    u8*                  sp14;
    s16                  temp_r2;
    s32                  temp_r1;
    s32                  temp_r4;
    const u16*           temp_r7;
    s32                  var_r4;
    u16                  temp_r5;
    u8*                  var_r6;
    u8                   temp_byte;
    const Ov031FontInfo* temp_r0;
    const u16*           table;

    temp_r2 = arg2->unk2C->unk10;
    (void)arg7;
    temp_r4 = arg2->unk16 * 4;
    sp14    = arg1;
    temp_r0 = (const Ov031FontInfo*)arg0->unk4;
    temp_r7 = arg2->unk1C;
    sp18    = arg3;
    temp_r5 = temp_r0->unk24;
    sp1C    = (s32)temp_r0->unk26;
    table   = temp_r7;
    sp38    = (s32)table[temp_r4 + 1];
    sp3C    = (s32)(table + table[temp_r4]);
    sp34    = (s32)(temp_r2 + ((u32)(temp_r2 >> 1) >> 0x1E)) >> 2;
    sp30    = arg0->unk6C + arg5;
    sp2C    = arg0->unk74;
    sp24    = arg0->unk78;
    sp28    = arg0->unk_70 + arg6;
    func_ov031_0210a220(arg2, &sp4C, &sp48);
    sp40 = 0;
    if (sp1C <= 0) {
        return;
    }
    sp20 = 0;
loop_3:
    var_r4 = 0;
    if ((s32)temp_r5 > 0) {
        var_r6 = sp14 + sp20;
    loop_5:
        if ((*var_r6 == 0) && (arg4 == 1)) {
            goto block_22;
        }
        if (((s32)(sp30 + var_r4) < sp2C) && ((s32)(sp28 + sp40) < sp24)) {
            u8         value = (u8)((*var_r6 + arg3) - temp_r0->unk18);
            Ov031Db20* hit;
            u8*        dst;
            u8         existing;
            int        preserve_shift;
            int        insert_shift;
            u8         out;
            s32        x_pos = sp30 + var_r4;
            s32        y_pos = sp28 + sp40;

            hit = func_ov031_0210b1bc(arg2, x_pos, y_pos, sp3C, (u32)sp38, sp4C, sp48, arg8);
            dst = (u8*)hit->unk0;
            if (dst == NULL) {
                goto block_22;
            }
            existing       = *dst;
            preserve_shift = (hit->unk4 != 0) ? 0 : 4;
            insert_shift   = (hit->unk4 != 0) ? 4 : 0;
            out            = (u8)((existing & (0xF << preserve_shift)) | (value << insert_shift));
            temp_byte      = out;
            if (arg8 == 0) {
                *dst = out;
                goto block_22;
            }
            func_0203b454(&temp_byte, dst, 1);
            goto block_22;
        }
    block_22:
        var_r6 += 1;
        var_r4 += 1;
        if (var_r4 >= (s32)temp_r5) {
            goto block_23;
        }
        goto loop_5;
    }
block_23:
    sp20 += 0x10;
    temp_r1 = sp40 + 1;
    sp40    = temp_r1;
    if (temp_r1 >= sp1C) {
        return;
    }
    goto loop_3;
}

void func_ov031_0210b5fc(UnkOv31Struct* arg0, s32 arg1) {
    Data* temp_r0;

    temp_r0 = arg0->unk10;
    if (temp_r0 != NULL) {
        DatMgr_ReleaseData(temp_r0);
        arg0->unk10 = NULL;
    }
    arg0->unk_14 = arg1;
    arg0->unk1C  = 1;
}

void func_ov031_0210b630(UnkOv31Struct* arg0, s32 arg1) {
    Data*                 temp_r0;
    Data*                 temp_r0_2;
    const Ov031PackEntry* entries;

    temp_r0 = arg0->unk10;
    if (temp_r0 != NULL) {
        DatMgr_ReleaseData(temp_r0);
        arg0->unk10 = NULL;
    }
    entries      = (const Ov031PackEntry*)((Data*)arg0->unk18)->buffer;
    temp_r0_2    = DatMgr_LoadRawDataWithOffset(1, NULL, entries[arg1].unk4, data_ov031_0210d130, entries[arg1].unk0);
    arg0->unk10  = temp_r0_2;
    arg0->unk_14 = (void*)temp_r0_2->buffer;
    arg0->unk1C  = 1;
}

void* func_ov031_0210b698(UnkOv31Struct* arg0, s32 arg1) {
    Data*                 temp_r0_3;
    const Ov031PackEntry* entries;
    s32                   temp_r6;
    u16                   var_r5;
    void*                 temp_r0;
    u16*                  var_r3;

    var_r5    = 1;
    entries   = (const Ov031PackEntry*)((Data*)arg0->unk18)->buffer;
    temp_r0_3 = DatMgr_LoadRawDataWithOffset(1, NULL, entries[arg1].unk4, data_ov031_0210d130, entries[arg1].unk0);
    var_r3    = (u16*)temp_r0_3->buffer;
    if (*var_r3 != 0xFFFF) {
        do {
            var_r3 += 1;
            var_r5 += 1;
        } while (*var_r3 != 0xFFFF);
    }
    temp_r6 = var_r5 * 2;
    temp_r0 = Mem_AllocHeapTail(&gDebugHeap, temp_r6);
    Mem_SetSequence(&gDebugHeap, temp_r0, data_ov031_0210dab4);
    func_0203b270(temp_r0_3->buffer, temp_r0, temp_r6);
    DatMgr_ReleaseData(temp_r0_3);
    return temp_r0;
}

s32 func_ov031_0210b754(UnkOv31Struct* arg0, u16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u32 arg7, void* arg8,
                        s32 arg9, u32 argA, u32 argB, s32 argC) {
    u8                   sp244[0x100];
    u16                  sp44;
    s32                  sp40;
    s32                  sp3C;
    s32                  sp38;
    s32                  sp34;
    u32                  sp30;
    s32                  sp2C;
    s32                  sp28;
    s32                  sp24;
    const Ov031FontInfo* sp20;
    s32                  sp1C;
    s32                  sp18;
    u8*                  var_r4;
    Ov031FirmwareOverlay unksp358;
    GetDsFirmwareUserSettings(&unksp358);
    s32  temp_r0;
    s32  temp_r0_3;
    s32  temp_r0_5;
    s32  temp_r0_6;
    s32  var_r1;
    u16* var_r8;
    u16  temp_r0_8;
    u16  temp_r2;
    u16  var_r11;
    u16  var_r4_2;
    u16  var_r5;
    u16  var_r7;
    u32  temp_r0_2;
    u32  temp_r0_4;
    u32  temp_r0_7;
    u32  var_r6;

    var_r8 = arg1;
    sp18   = arg2;
    var_r1 = 0x40;
    sp1C   = arg3;
    var_r4 = sp244;
    do {
        var_r4[0] = 0;
        var_r4[1] = 0;
        var_r4[2] = 0;
        var_r4[3] = 0;
        var_r4 += 4;
        var_r1 -= 1;
    } while (var_r1 != 0);
    var_r6  = 0;
    sp28    = (s32)arg0->unk5A;
    var_r11 = 0;
    sp30    = (argA + unksp358.unk2C) << 0x10;
    sp24    = 0xFFFF;
    var_r7  = arg0->unk_5C;
    sp20    = (const Ov031FontInfo*)arg0->unk4;
    func_ov031_0210c46c(arg0, var_r8, &sp44);
    temp_r0 = arg0->unk48;
    switch (temp_r0) { /* switch 1; irregular */
        case 0:        /* switch 1 */
            temp_r0_2 = arg0->unk4C - sp44;
            var_r4_2  = (u16)((u32)((temp_r0_2 + (temp_r0_2 >> 0x1F)) << 0xF) >> 0x10);
            break;
        case 2: /* switch 1 */
            var_r4_2 = arg0->unk4C - sp44;
            break;
        default: /* switch 1 */
            var_r4_2 = 0;
            break;
    }
    temp_r0_3 = arg0->unk50;
    switch (temp_r0_3) { /* switch 2; irregular */
        case 0:          /* switch 2 */
            temp_r0_4 = arg0->unk54 - func_ov031_0210c4b8(arg0, var_r8);
            var_r5    = (u16)((u32)((temp_r0_4 + (temp_r0_4 >> 0x1F)) << 0xF) >> 0x10);
            break;
        case 4: /* switch 2 */
            var_r5 = arg0->unk54 - func_ov031_0210c4b8(arg0, var_r8);
            break;
        default: /* switch 2 */
            var_r5 = 0;
            break;
    }
    sp2C = -1;
    sp38 = 0x1C9;
    sp34 = 0xFFE1;
    sp3C = 0xFFFD;
    sp40 = 0xFFFE;
loop_103:
    if ((func_ov031_0210a7e0(*var_r8) == 0) && (var_r6 < (u32)(sp30 >> 0x10))) {
        temp_r2 = *var_r8;
        if ((s32)temp_r2 <= sp34) {
            temp_r0_5 = temp_r2 - 0xFFB1;
            if ((temp_r0_5 >= 0) && (temp_r0_5 <= 14)) {
                sp28 = temp_r2 & 0xF;
            } else if (temp_r0_5 == 47) {
                var_r7 = 1;
            } else if (temp_r0_5 == 48) {
                var_r7 = 2;
            } else if (((temp_r0_5 >= 20) && (temp_r0_5 <= 30)) || ((temp_r0_5 >= 35) && (temp_r0_5 <= 46))) {
                goto block_90;
            } else {
                if ((s32)temp_r2 <= 0) {
                    if (temp_r2 != 0) {
                        goto block_90;
                    }
                    var_r4_2 = arg0->unk56 + (u16)(var_r4_2 + 3);
                    var_r6   = (u32)(u16)(var_r6 + 1);
                } else {
                    if (temp_r2 != sp38) {
                        goto block_90;
                    }
                    var_r4_2 = arg0->unk56 + (u16)(var_r4_2 + sp20->unk24);
                    var_r6   = (u32)(u16)(var_r6 + 1);
                }
            }
        } else if ((s32)temp_r2 <= sp3C) {
            if (temp_r2 != sp3C) {
                goto block_90;
            }
        } else if (temp_r2 != sp40) {
            goto block_90;
        } else {
            var_r11 += 1;
            temp_r0_6 = arg0->unk48;
            switch (temp_r0_6) { /* switch 4; irregular */
                case 0:          /* switch 4 */
                    temp_r0_7 = arg0->unk4C - (&sp44)[var_r11];
                    var_r4_2  = (u16)((u32)((temp_r0_7 + (temp_r0_7 >> 0x1F)) << 0xF) >> 0x10);
                    break;
                case 2: /* switch 4 */
                    var_r4_2 = arg0->unk4C - (&sp44)[var_r11];
                    break;
                default: /* switch 4 */
                    var_r4_2 = 0;
                    break;
            }
            var_r5 += sp20->unk26 + arg0->unk58;
        }
        temp_r0_8 = *var_r8;
        var_r8 += 1;
        sp24 = (s32)temp_r0_8;
        goto loop_103;
    }
    goto post_loop;
block_90:
    if ((func_ov031_0210a7e0(*var_r8) == 0) && (var_r6 < (u32)(sp30 >> 0x10))) {
        if (var_r6 < argA) {
            var_r4_2 = arg0->unk56 + (u16)(var_r4_2 + func_ov031_0210c194(arg0, temp_r2));
            var_r6   = (u32)(u16)(var_r6 + 1);
        } else {
            var_r6 = (u32)(u16)(var_r6 + 1);
            func_ov031_0210ac1c(arg0, sp244, temp_r2);
            switch (argC) { /* switch 5; irregular */
                case 1:     /* switch 5 */
                    func_ov031_0210ada0(arg0, sp244, sp18, (u16)sp28, 1U, (u16)sp1C, (u16)arg4, (u32)var_r4_2, (u32)var_r5,
                                        (s32)var_r7);
                    break;
                case 2: /* switch 5 */
                    func_ov031_0210af48(arg0, sp244, arg5, arg6, (u8)sp28, 1, (u16)(s32)var_r4_2, (u16)(s32)var_r5,
                                        (u16)(s32)var_r7, arg7);
                    break;
                case 3: { /* switch 5 */
                    Ov031Glyph* glyph = (Ov031Glyph*)arg8;
                    func_ov031_0210b3f4(arg0, sp244, glyph, sp28, 1, (u16)(s32)var_r4_2, (u16)(s32)var_r5, (s32)var_r7, arg9);
                    if (arg9 == 0) {
                        glyph->unk2C->unk12 = (s16)sp2C;
                        glyph->unk2C->unkA  = (s16)sp2C;
                        glyph->unk28->unk14 = 0;
                    }
                    break;
                }
                default: /* switch 5 */
                    OS_WaitForever();
                    break;
            }
            var_r4_2 = arg0->unk56 + (u16)(var_r4_2 + func_ov031_0210c194(arg0, *var_r8));
        }
        temp_r0_8 = *var_r8;
        var_r8 += 1;
        sp24 = (s32)temp_r0_8;
        goto loop_103;
    }
post_loop:
    if (func_ov031_0210a7e0(*var_r8) != 0) {
        sp24 = (s32)*var_r8;
    }
    DC_PurgeAll();
    return sp24;
}

s32 func_ov031_0210bcdc(UnkOv31Struct* arg0, u16* arg1, void* arg2, s32 arg3, s32 arg4, u16 arg5, u16 arg6) {
    return func_ov031_0210b754(arg0, arg1, (s32)arg2, arg3, arg4, 0, 0, 0xFFFFU, NULL, 0, (u32)arg5, (u32)arg6, 1);
}

s32 func_ov031_0210bd2c(UnkOv31Struct* arg0, u16* arg1, void* arg2, s32 arg3, s32 arg4) {
    return func_ov031_0210bcdc(arg0, arg1, arg2, arg3, arg4, 0U, 0xFFFFU);
}

s32 func_ov031_0210bd58(UnkOv31Struct* arg0, void* arg1, s32 arg2, s32 arg3) {
    if (arg0->unk_14 == 0) {
        OS_WaitForever();
    }
    return func_ov031_0210bd2c(arg0, arg0->unk_14, arg1, arg2, arg3);
}

s32 func_ov031_0210bd98(UnkOv31Struct* arg0, u16* arg1, s32 arg2, s32 arg3, u32 arg4, u16 arg5, u16 arg6) {
    return func_ov031_0210b754(arg0, arg1, 0, 0, 0, arg2, arg3, arg4, NULL, 0, (u32)arg5, (u32)arg6, 2);
}

s32 func_ov031_0210bde8(UnkOv31Struct* arg0, u16* arg1, s32 arg2, s32 arg3, u32 arg4) {
    return func_ov031_0210bd98(arg0, arg1, arg2, arg3, arg4, 0U, 0xFFFFU);
}

s32 func_ov031_0210be18(UnkOv31Struct* arg0, u16 arg1, u16 arg2, s32 arg3) {
    if (arg0->unk_14 == 0) {
        OS_WaitForever();
    }
    return func_ov031_0210bde8(arg0, arg0->unk_14, arg1, arg2, arg3);
}

s32 func_ov031_0210be58(UnkOv31Struct* arg0, u16* arg1, void* arg2, s32 arg3, u16 arg4, u16 arg5) {
    return func_ov031_0210b754(arg0, arg1, 0, 0, 0, 0, 0, 0xFFFFU, arg2, arg3, (u32)arg4, (u32)arg5, 3);
}

s32 func_ov031_0210beb0(UnkOv31Struct* arg0, u16* arg1, void* arg2, s32 arg3) {
    return func_ov031_0210be58(arg0, arg1, arg2, arg3, 0U, 0xFFFFU);
}

s32 func_ov031_0210bed8(UnkOv31Struct* arg0, void* arg1, s32 arg2) {
    if (arg0->unk_14 == NULL) {
        OS_WaitForever();
    }
    return func_ov031_0210be58(arg0, arg0->unk_14, arg1, arg2, 0U, 0xFFFFU);
}

s32 func_ov031_0210bf2c(UnkOv31Struct* arg0, s32 arg1, void* arg2, s32 arg3) {
    u16* temp_r0;

    if (arg0->unk_14 == NULL) {
        OS_WaitForever();
    }
    temp_r0 = func_ov031_0210c0d4(arg0->unk_14, arg1);
    if (temp_r0 == NULL) {
        return 0xFFFC;
    }
    return (s32)func_ov031_0210be58(arg0, temp_r0, arg2, arg3, 0U, 0xFFFFU);
}

s32 func_ov031_0210bfa0(UnkOv31Struct* arg0, s32 arg1, void* arg2, s32 arg3, u16 arg4, u16 arg5) {
    u16* temp_r0;

    if (arg0->unk_14 == NULL) {
        OS_WaitForever();
    }
    temp_r0 = func_ov031_0210c0d4(arg0->unk_14, arg1);
    if (temp_r0 == NULL) {
        return 0xFFFC;
    }
    return (s32)func_ov031_0210be58(arg0, temp_r0, arg2, arg3, (u16)(s32)arg4, (u16)(s32)arg5);
}

u16 func_ov031_0210c010(u16* arg0) {
    u16 temp_r1;
    u16 var_ip;
    u16 var_lr;

    var_lr = 1;
    var_ip = 0;
loop_1:
    temp_r1 = arg0[var_ip];
    if (temp_r1 != 0xFFFF) {
        if (temp_r1 == 0xFFFD) {
            var_lr += 1;
        }
        var_ip += 1;
        goto loop_1;
    }
    return var_lr;
}

u16 func_ov031_0210c060(s32 arg0) {
    Data*                 temp_r0;
    Data*                 temp_r5;
    u16                   temp_r4;
    const Ov031PackEntry* entries;

    temp_r0 = DatMgr_LoadRawData(1, NULL, 0, &data_ov031_0210d120);
    entries = (const Ov031PackEntry*)temp_r0->buffer;
    temp_r5 = DatMgr_LoadRawDataWithOffset(1, NULL, entries[arg0].unk4, data_ov031_0210d130, entries[arg0].unk0);
    DatMgr_ReleaseData(temp_r0);
    temp_r4 = func_ov031_0210c010(temp_r5->buffer);
    DatMgr_ReleaseData(temp_r5);
    return temp_r4;
}

u16* func_ov031_0210c0d4(u16* arg0, s32 arg1) {
    s32 temp_z;
    u16 temp_r2;
    u16 temp_r2_2;
    u16 var_lr;
    u16 var_r4;

    var_r4 = 0;
    if (arg1 == 0) {
        return arg0;
    }
    var_lr = 0;
loop_3:
    temp_r2 = arg0[var_lr];
    if (temp_r2 != 0xFFFF) {
        if (temp_r2 == 0xFFFD) {
            temp_r2_2 = var_r4 + 1;
            temp_z    = arg1 == temp_r2_2;
            var_r4    = temp_r2_2;
            if (temp_z) {
                return arg0 + (var_lr + 1);
            }
        }
        var_lr += 1;
        goto loop_3;
    }
    return NULL;
}

s32 func_ov031_0210c140(u32 arg0) {
    s32 var_r2;

    var_r2 = 0;
    if ((arg0 <= 0x5EU) || ((arg0 >= 0x155U) && (arg0 <= 0x1AAU)) || ((arg0 >= 0x1D4U) && (arg0 <= 0x207U))) {
        var_r2 = 1;
    }
    if (arg0 & 0xF000) {
        var_r2 = 0;
    }
    return var_r2;
}

u16 func_ov031_0210c194(UnkOv31Struct* arg0, s32 arg1) {
    u16                  temp_r7;
    u16                  temp_r8;
    const Ov031FontInfo* temp_r4;

    temp_r4 = (const Ov031FontInfo*)arg0->unk4;
    temp_r7 = temp_r4->unk2C * temp_r4->unk2E;
    temp_r8 = (u16)(arg1 / (s32)temp_r7);
    if ((u32)temp_r8 >= 5U) {
        return 0U;
    }
    switch (arg1) { /* irregular */
        case 0x0:
            return 3U;
        case 0x1C9:
            return temp_r4->unk24;
        default:
            if (func_ov031_0210c140((u32)arg1) == 0) {
                return temp_r4->unk24;
            }
            if (arg0->unk68 == 0) {
                return temp_r4->unk24;
            }
            return (u16) * ((u16*)(arg0->unk34[temp_r8]) + (u16)(arg1 % (s32)temp_r7));
    }
}

u32 func_ov031_0210c248(UnkOv31Struct* arg0, s32 arg1, s32 arg2, u32 arg3) {
    u16*      var_r6       = (u16*)((u8*)arg1 + (arg2 * 2));
    u16       max_width    = 0;
    u16       line_width   = 0;
    u16       count        = 0;
    const u16 control_base = 0xFFFE;
    const u16 control_ffe1 = (u16)(control_base - 0x1D);
    const u16 control_ffe0 = (u16)(control_base - 0x1E);

    while ((func_ov031_0210a7e0(*var_r6) == 0) && (count < arg3)) {
        u16 code = *var_r6;

        if ((code != control_ffe0) && (code != control_ffe1)) {
            if (code == control_base) {
                if (line_width > max_width) {
                    max_width = line_width;
                }
                line_width = 0;
            } else if ((code & 0xFF00) != 0xFF00) {
                line_width = (u16)(line_width + func_ov031_0210c194(arg0, code));
                if (count != 0) {
                    line_width = (u16)(line_width + arg0->unk56);
                }
                count += 1;
            }
        }
        count += 1;
        var_r6 += 1;
    }

    if (line_width > max_width) {
        max_width = line_width;
    }
    return max_width;
}

u32 func_ov031_0210c320(UnkOv31Struct* arg0, s32 arg1) {
    return func_ov031_0210c248(arg0, arg1, 0, 0xFFFFU);
}

void func_ov031_0210c338(UnkOv31Struct* arg0) {
    func_ov031_0210c320(arg0, arg0->unk_14);
}

s32 func_ov031_0210c348(UnkOv31Struct* arg0) {
    if (func_ov031_0210c0d4(arg0->unk_14, 0) == NULL) {
        return 0;
    }
    return (s32)func_ov031_0210c320(arg0, arg0->unk_14);
}

u32 func_ov031_0210c370(UnkOv31Struct* arg0, u16* arg1, u16* arg2, s32 arg3, u32 arg4) {
    u16*      var_r4       = (u16*)((u8*)arg1 + (arg3 * 2));
    u16       line_width   = 0;
    u16       line_index   = 0;
    u16       count        = 0;
    const u16 control_base = 0xFFFE;
    const u16 control_ffe1 = (u16)(control_base - 0x1D);
    const u16 control_ffe0 = (u16)(control_base - 0x1E);

    while ((func_ov031_0210a7e0(*var_r4) == 0) && (count < arg4)) {
        u16 code = *var_r4;

        if ((code != control_ffe0) && (code != control_ffe1)) {
            if (code == control_base) {
                arg2[line_index] = line_width;
                line_width       = 0;
                line_index += 1;
            } else if ((code & 0xFF00) != 0xFF00) {
                line_width = (u16)(line_width + func_ov031_0210c194(arg0, code));
                if (count != 0) {
                    line_width = (u16)(line_width + arg0->unk56);
                }
                count += 1;
            }
        }
        count += 1;
        var_r4 += 1;
    }

    arg2[line_index] = line_width;
    return (u32)(line_index + 1);
}

u32 func_ov031_0210c46c(UnkOv31Struct* arg0, u16* arg1, u16* arg2) {
    return func_ov031_0210c370(arg0, arg1, arg2, 0, 0xFFFFU);
}

u32 func_ov031_0210c488(UnkOv31Struct* arg0, s32 arg1, u16* arg2) {
    u16* temp_r0;

    temp_r0 = func_ov031_0210c0d4(arg0->unk_14, arg1);
    if (temp_r0 == NULL) {
        return 0;
    }
    return func_ov031_0210c46c(arg0, temp_r0, arg2);
}

u16 func_ov031_0210c4b8(UnkOv31Struct* arg0, u16* arg1) {
    return func_ov031_0210c4cc(arg0, arg1, 0xFFFFU);
}

u16 func_ov031_0210c4cc(UnkOv31Struct* arg0, u16* arg1, u32 arg2) {
    const u16 base      = 0xFFE0;
    const u16 base_next = (u16)(base + 1);
    const u16 base_end  = (u16)(base + 0x1E);
    u16*      cursor    = arg1;
    u16       width     = arg0->unk_5C;
    u16       width_max = width;
    u16       sum       = 0;
    u16       count     = 0;

    while ((func_ov031_0210a7e0(*cursor) == 0) && (count < arg2)) {
        u16 code = *cursor;

        if (code == base) {
            width = 1;
            if (width_max < 1U) {
                width_max = 1;
            }
        } else if (code == base_next) {
            width = 2;
            if (width_max < 2U) {
                width_max = 2;
            }
        } else if (code == base_end) {
            u16 font_width = ((const Ov031FontInfo*)arg0->unk4)->unk26;
            u16 offset     = (u16)arg0->unk58;

            sum       = (u16)(sum + (u16)(width_max * font_width + offset));
            width_max = width;
        } else if ((code & 0xFF00) == 0xFF00) {
            count = (u16)(count + 1);
        }

        cursor += 1;
    }

    return (u16)((((const Ov031FontInfo*)arg0->unk4)->unk26 * width_max) + sum);
}

void func_ov031_0210c5b4(UnkOv31Struct* arg0) {
    func_ov031_0210c4b8(arg0, arg0->unk_14);
}

s32 func_ov031_0210c5c4(UnkOv31Struct* arg0) {
    if (func_ov031_0210c0d4(arg0->unk_14, 0) == NULL) {
        return 0;
    }
    return func_ov031_0210c4b8(arg0, arg0->unk_14);
}

u16 func_ov031_0210c5ec(u16* arg1) {
    u16* var_r5;
    u16  var_r4;

    var_r5 = arg1;
    var_r4 = 0;
    if (func_ov031_0210a7e0(*var_r5) == 0) {
        do {
            var_r5 += 1;
            var_r4 += 1;
        } while (func_ov031_0210a7e0(*var_r5) == 0);
    }
    return (u16)(var_r4 + 1);
}

u16 func_ov031_0210c634(s32 arg0) {
    s32 temp_r1;
    s32 var_r0;
    s32 var_r2;

    var_r0 = arg0;
    var_r2 = 0;
    do {
        temp_r1 = var_r2 + (var_r0 * 0x10);
        var_r2 += 1;
        var_r0 += temp_r1;
    } while (var_r2 < 4);
    return (u16)(var_r0 - (0x209 * ((u32)MULTU_HI(var_r0, 0xFB93E673) >> 9)));
}

u16 func_ov031_0210c678(u16 arg0) {
    s32 var_r7;
    u16 temp_r0;
    u16 var_r6;
    u32 temp_lr;

    temp_r0 = func_ov031_0210c634(arg0);
    var_r6  = temp_r0;
    var_r7  = 0;
loop_1:
    if (arg0 == *(const u16*)((const u8*)data_ov031_0210d238 + (var_r6 * 4))) {
        return var_r6;
    }
    temp_lr = var_r6 + 1;
    var_r6  = temp_lr - (0x209 * ((temp_lr >> 0x1F) + ((var_r6 + 1) / 521)));
    var_r7 += 1;
    if ((var_r6 == temp_r0) || (var_r7 > 0x1C)) {
        return -1U;
    }
    goto loop_1;
}

void func_ov031_0210c6ec(const u16* arg0, u16* arg1, s32 arg2) {
    s32 i = 0;

    if (arg2 > 0) {
        do {
            s32 index = (s32)func_ov031_0210c678(arg0[i]);
            if (index >= 0) {
                u16 value = (u16)((u32)data_ov031_0210d238[index] >> 16);
                arg1[i]   = value;
                if (value == 4) {
                    arg1[i] = 0x166;
                }
            } else {
                arg1[i] = 0xFFFF;
            }
            i++;
        } while (i < arg2);
    }
    arg1[arg2] = 0xFFFF;
}

void* func_ov031_0210c774(const u16* arg0, s32 arg1) {
    void* temp_r0;

    temp_r0 = Mem_AllocHeapTail(&gDebugHeap, (arg1 + 1) * 2);
    Mem_SetSequence(&gDebugHeap, temp_r0, data_ov031_0210dac8);
    func_ov031_0210c6ec(arg0, (u16*)temp_r0, arg1);
    return temp_r0;
}

void func_ov031_0210c7c4(void) {
    u8 firmware[0x54];

    GetDsFirmwareUserSettings(firmware);
    func_ov031_0210c774((const u16*)(firmware + 0x4), 0xA);
}

u16* func_ov031_0210c7e8(void) {
    u8 firmware[0x54];

    GetDsFirmwareUserSettings(firmware);
    return func_ov031_0210c774((const u16*)(firmware + 0x1C), 0x1A);
}

s16* func_ov031_0210c80c(void) {
    s16* temp_r0;
    u16* temp_r5;
    u16* temp_r6;
    u16  sp0[4];

    sp0[0] = data_ov031_0210d11c.unk24;
    sp0[1] = data_ov031_0210d11c.unk26;
    sp0[2] = data_ov031_0210d11c.unk28;
    sp0[3] = data_ov031_0210d11c.unk2A;

    temp_r6       = func_ov031_0210c7e8();
    temp_r5       = func_ov031_0210c7e8();
    temp_r6[0x0D] = 0xFFFF;
    temp_r0       = Mem_AllocHeapTail(&gDebugHeap, 0x38);
    Mem_SetSequence(&gDebugHeap, temp_r0, data_ov031_0210daec);
    func_ov031_0210a5fc(temp_r0, sp0, (s32)temp_r6, (u32)temp_r5 + 0x1A, 0, 0, 0);
    Mem_Free(&gDebugHeap, temp_r6);
    Mem_Free(&gDebugHeap, temp_r5);
    return temp_r0;
}

void func_ov031_0210c8b8(Ov031MenuState* arg0, s8* arg1, s32 arg2) {
    s32 temp_ip;
    u16 var_lr;

    arg0->unk1B0 = arg1;
    arg0->unk1A8 = 0xFFFF;
    arg0->unk1AA = 0xFFFF;
    arg0->unk1AC = 0xFFFF;
    var_lr       = 0;
    arg0->unk1B4 = 0;
    arg0->unk0   = arg2;
    arg0->unk1BC = 0;
    arg0->unk1C8 = 0;
    arg0->unk1CA = 0;
    arg0->unk1CC = 0;
    arg0->unk1CE = 0;
    arg0->unk1D0 = 0;
    arg0->unk1D4 = 0;
    arg0->unk1D8 = 0;
    arg0->unk1DC = 0xFFFF;
    do {
        Ov031MenuEntry* entry = (Ov031MenuEntry*)((u8*)arg0 + (var_lr * 0x1C));
        entry->unk4           = 0;
        entry->unk8           = 0;
        entry->unkC           = 0;
        entry->unk10          = 0;
        entry->unk14          = 0;
        entry->unk18          = 0;
        var_lr += 1;
        entry->unk1C = 0;
    } while ((u32)var_lr < 0xFU);
    arg0->unk1B8 = 0;
}

void func_ov031_0210c94c(Ov031MenuState* arg0, s8* arg1, s32 arg2) {
    void* temp_r0;

    func_ov031_0210c8b8(arg0, arg1, 0);
    temp_r0 = Mem_AllocHeapTail(&gMainHeap, arg2);
    Mem_SetSequence(&gMainHeap, temp_r0, arg1);
    arg0->unk0   = temp_r0;
    arg0->unk1B8 = 1;
    func_0203b28c(0, arg0->unk0, arg2);
}

void func_ov031_0210c9a8(Ov031MenuState* arg0, void* arg1) {
    void* temp_r1;

    temp_r1 = arg0->unk0;
    if (temp_r1 != NULL) {
        Mem_Free(&gMainHeap, temp_r1);
    }
    arg0->unk0   = arg1;
    arg0->unk1B8 = 0;
}

void func_ov031_0210c9dc(Ov031MenuState* arg0, Ov031MenuEntry* entry, s32 arg2) {
    if (entry == NULL) {
        return;
    }
    if (entry->unk0 == 0) {
        return;
    }
    entry->unk18 = 1;
    ((void (*)(void*, s32))entry->unk0)(arg0->unk0, arg2);
}

void func_ov031_0210ca08(Ov031MenuState* arg0, Ov031MenuEntry* entry) {
    if (entry == NULL) {
        return;
    }
    if ((entry->unk18 == 0) || (entry->unk10 == NULL)) {
        return;
    }
    entry->unk10(arg0->unk0, 0);
    entry->unk18 = 0;
}

void func_ov031_0210ca38(Ov031MenuState* arg0) {
    Ov031MenuEntry* entry = (Ov031MenuEntry*)((u8*)arg0 + (arg0->unk1A8 * 0x1C));
    func_ov031_0210ca08(arg0, entry);
    if (arg0->unk1B8 != 0) {
        Mem_Free(&gMainHeap, arg0->unk0);
    }
    arg0->unk0 = NULL;
}

s32 func_ov031_0210ca80(Ov031MenuState* arg0) {
    s32             temp_r0;
    s32             var_r6;
    u16             temp_r1;
    u16             temp_r3;
    Ov031MenuEntry* entry;

    var_r6 = 1;
    if ((SysControl.buttonState.currButtons & 0x2000) && !(SysControl.buttonState.pressedButtons & 0x200)) {
        return 1;
    }
    if (arg0->unk1A8 == 0xFFFF) {
        return 0;
    }
loop_6:
    temp_r3 = arg0->unk1A8;
    entry   = (Ov031MenuEntry*)((u8*)arg0 + (temp_r3 * 0x1C));
    if ((arg0->unk1AA != temp_r3) || (arg0->unk1AC != entry->unk14)) {
        arg0->unk1AA = temp_r3;
        arg0->unk1AC = (u16)entry->unk14;
    }
    arg0->unk1B4 = 0;
    temp_r0      = ((s32(*)(Ov031MenuState*, s32))((s32*)entry->unk4 + entry->unk14))(arg0, (s32)arg0->unk0);
    if (temp_r0 == 2) {
        goto loop_6;
    }
    if (temp_r0 == 1) {
        func_ov031_0210ca08(arg0, (Ov031MenuEntry*)((u8*)arg0 + (arg0->unk1A8 * 0x1C)));
        temp_r1 = arg0->unk1A8;
        if (temp_r1 != 0) {
            arg0->unk1A8 = (u16)(temp_r1 - 1);
        } else {
            var_r6 = 0;
        }
        return var_r6;
    }
    return var_r6;
}

void func_ov031_0210cb68(Ov031MenuState* arg0) {
    void (*temp_r2_2)(void*, s32);
    u16 temp_r2;

    if ((SysControl.buttonState.currButtons & 0x2000) && !(SysControl.buttonState.pressedButtons & 0x200)) {
        return;
    }
    temp_r2 = arg0->unk1A8;
    if (temp_r2 == 0xFFFF) {
        return;
    }
    temp_r2_2 = ((Ov031MenuEntry*)((u8*)arg0 + (temp_r2 * 0x1C)))->unkC;
    if (temp_r2_2 == NULL) {
        return;
    }
    temp_r2_2(arg0, arg0->unk0);
}

void func_ov031_0210cbc4(Ov031MenuState* arg0) {
    void (*temp_r2_2)(void*, s32);
    u16 temp_r2;

    if ((SysControl.buttonState.currButtons & 0x2000) && !(SysControl.buttonState.pressedButtons & 0x200)) {
        return;
    }
    temp_r2 = arg0->unk1A8;
    if (temp_r2 == 0xFFFF) {
        return;
    }
    temp_r2_2 = ((Ov031MenuEntry*)((u8*)arg0 + (temp_r2 * 0x1C)))->unk10;
    if (temp_r2_2 == NULL) {
        return;
    }
    temp_r2_2(arg0, arg0->unk0);
}

void func_ov031_0210cc20(Ov031MenuState* arg0, s32 arg1) {
    s32 temp_r7;

    if (arg0->unk1A8 == 0xFFFF) {
        arg0->unk1A8 = 0U;
    }
    arg0->unk1B4 = 1;
    temp_r7      = (s32)((u8*)arg0 + (arg0->unk1A8 * 0x1C));
    func_ov031_0210ca08(arg0, (Ov031MenuEntry*)temp_r7);
    ((Ov031MenuEntry*)temp_r7)->unk0  = 0;
    ((Ov031MenuEntry*)temp_r7)->unk4  = arg1;
    ((Ov031MenuEntry*)temp_r7)->unk8  = 0;
    ((Ov031MenuEntry*)temp_r7)->unkC  = 0;
    ((Ov031MenuEntry*)temp_r7)->unk10 = 0;
    ((Ov031MenuEntry*)temp_r7)->unk14 = 0;
}

void func_ov031_0210cc84(Ov031MenuState* arg0, void* arg1, s32 arg2) {
    s32   temp_r4;
    void* temp_r5;
    void* temp_r6;

    if (arg0->unk1A8 == 0xFFFF) {
        arg0->unk1A8 = 0U;
    }
    temp_r5 = (void*)arg0;
    temp_r4 = arg0->unk1A8 * 0x1C;
    temp_r6 = temp_r5 + temp_r4;
    func_ov031_0210ca08(arg0, (Ov031MenuEntry*)temp_r6);
    func_ov031_0210cc20(arg0, ((Ov031MenuEntry*)arg1)->unk4);
    *(s32*)((u8*)temp_r5 + temp_r4)   = ((Ov031MenuEntry*)arg1)->unk0;
    ((Ov031MenuEntry*)temp_r6)->unk8  = ((Ov031MenuEntry*)arg1)->unk8;
    ((Ov031MenuEntry*)temp_r6)->unkC  = ((Ov031MenuEntry*)arg1)->unkC;
    ((Ov031MenuEntry*)temp_r6)->unk10 = ((Ov031MenuEntry*)arg1)->unk10;
    func_ov031_0210c9dc(arg0, (Ov031MenuEntry*)temp_r6, arg2);
}

void func_ov031_0210cd14(Ov031MenuState* arg0, s32 arg1) {
    if (arg0->unk1A8 == 0xFFFF) {
        arg0->unk1A8 = 0U;
    }
    ((Ov031MenuEntry*)((u8*)arg0 + (arg0->unk1A8 * 0x1C)))->unkC = (void (*)(void*, s32))arg1;
}

void func_ov031_0210cd48(Ov031MenuState* arg0, s32 arg1) {
    s32   temp_r2;
    void* temp_r1;
    void* temp_r3;

    arg0->unk1A8 = (u16)(arg0->unk1A8 + 1);
    if ((u32)arg0->unk1A8 >= 0xFU) {
        OS_WaitForever();
    }
    arg0->unk1B4                      = 1;
    temp_r3                           = (void*)arg0;
    temp_r2                           = arg0->unk1A8 * 0x1C;
    *(s32*)((u8*)temp_r3 + temp_r2)   = 0;
    temp_r1                           = (void*)((u8*)temp_r3 + temp_r2);
    ((Ov031MenuEntry*)temp_r1)->unk4  = arg1;
    ((Ov031MenuEntry*)temp_r1)->unk10 = 0;
    ((Ov031MenuEntry*)temp_r1)->unk14 = 0;
}

void func_ov031_0210cdac(Ov031MenuState* arg0) {
    s32 temp_r4;

    temp_r4 = (s32)((u8*)arg0 + (arg0->unk1A8 * 0x1C));
    if (arg0->unk1B4 != 0) {
        OS_WaitForever();
    }
    ((Ov031MenuEntry*)temp_r4)->unk14 = (u16)(((Ov031MenuEntry*)temp_r4)->unk14 + 1);
}

s32 func_ov031_0210cde4(Ov031MenuState* arg0) {
    return ((Ov031MenuEntry*)((u8*)arg0 + (arg0->unk1A8 * 0x1C)))->unk8;
}

void func_ov031_0210cdfc(Ov031MenuState* arg0, u16 arg1) {
    void* temp_r0;

    arg0->unk0 = arg1;
    temp_r0    = Mem_AllocHeapTail(&gMainHeap, arg0->unk0 * 4);
    Mem_SetSequence(&gMainHeap, temp_r0, data_ov031_0210db10);
    arg0->unk4 = temp_r0;
    func_0203b2d0(0, temp_r0, arg0->unk0 * 4);
}

u16 func_ov031_0210ce50(Ov031MenuState* arg0, s32 arg1) {
    s32 temp_hi;
    s32 temp_ip;
    u16 temp_lr;
    u16 temp_r0;
    u16 var_r2;
    u16 var_r3;

    temp_lr = arg0->unk0;
    var_r3  = 0xFFFF;
    var_r2  = 0;
    if ((u32)temp_lr > 0U) {
        temp_ip = (s32)arg0->unk4;
    loop_2:
        if (*(s32*)(temp_ip + (var_r2 * 4)) == 0) {
            var_r3                          = var_r2;
            *(s32*)(temp_ip + (var_r2 * 4)) = arg1;
        } else {
            temp_r0 = var_r2 + 1;
            temp_hi = (u32)temp_lr > (u32)temp_r0;
            var_r2  = temp_r0;
            if (temp_hi) {
                goto loop_2;
            }
        }
    }
    return var_r3;
}

u16 func_ov031_0210cea0(Ov031MenuState* arg0) {
    s32 temp_hi;
    u16 temp_r0;
    u16 var_r6;
    u16 var_r7;

    var_r7 = 0;
    var_r6 = 0;
    if ((u32)arg0->unk0 > 0U) {
    loop_2:
        if (*((s32*)arg0->unk4 + var_r6) != 0) {
            if (func_ov031_0210ca80(arg0) == 0) {
                *((s32*)arg0->unk4 + var_r6) = 0;
            } else {
                var_r7 = 1;
            }
        }
        temp_r0 = var_r6 + 1;
        temp_hi = (u32)arg0->unk0 > (u32)temp_r0;
        var_r6  = temp_r0;
        if (!temp_hi) {
            /* Duplicate return node #7. Try simplifying control flow for better match */
            return var_r7;
        }
        goto loop_2;
    }
    return var_r7;
}

void func_ov031_0210cf08(Ov031MenuState* arg0) {
    s32 temp_hi;
    u16 temp_r0;
    u16 var_r4;

    var_r4 = 0;
    if ((u32)arg0->unk0 <= 0U) {
        return;
    }
    do {
        if (*((s32*)arg0->unk4 + var_r4) != 0) {
            func_ov031_0210cb68(arg0);
        }
        temp_r0 = var_r4 + 1;
        temp_hi = (u32)arg0->unk0 > (u32)temp_r0;
        var_r4  = temp_r0;
    } while (temp_hi);
}

void func_ov031_0210cf50(Ov031MenuState* arg0) {
    s32 temp_hi;
    u16 temp_r0;
    u16 var_r4;

    var_r4 = 0;
    if ((u32)arg0->unk0 <= 0U) {
        return;
    }
    do {
        if (*((s32*)arg0->unk4 + var_r4) != 0) {
            func_ov031_0210cbc4(arg0);
        }
        temp_r0 = var_r4 + 1;
        temp_hi = (u32)arg0->unk0 > (u32)temp_r0;
        var_r4  = temp_r0;
    } while (temp_hi);
}

void func_ov031_0210cf98(Ov031MenuState* arg0, s32 arg1) {
    s32 temp_hi;
    u16 temp_r0;
    u16 temp_r3;
    u16 var_r4;

    temp_r3 = arg0->unk0;
    var_r4  = 0;
    if ((u32)temp_r3 > 0U) {
    loop_2:
        if (*((s32*)arg0->unk4 + var_r4) == arg1) {
            func_ov031_0210ca38(arg0);
            *((s32*)arg0->unk4 + var_r4) = 0;
            return;
        }
        temp_r0 = var_r4 + 1;
        temp_hi = (u32)temp_r3 > (u32)temp_r0;
        var_r4  = temp_r0;
        if (!temp_hi) {
            /* Duplicate return node #5. Try simplifying control flow for better match */
            OS_WaitForever();
            return;
        }
        goto loop_2;
    }
    OS_WaitForever();
}

void func_ov031_0210cff0(Ov031MenuState* arg0, s32 arg1) {
    s32 temp_hi;
    u16 temp_r0;
    u16 temp_r3;
    u16 var_r4;

    temp_r3 = arg0->unk0;
    var_r4  = 0;
    if ((u32)temp_r3 <= 0U) {
        return;
    }
loop_3:
    if (*((s32*)arg0->unk4 + var_r4) == arg1) {
        func_ov031_0210ca38(arg0);
        *((s32*)arg0->unk4 + var_r4) = 0;
        return;
    }
    temp_r0 = var_r4 + 1;
    temp_hi = (u32)temp_r3 > (u32)temp_r0;
    var_r4  = temp_r0;
    if (!temp_hi) {
        return;
    }
    goto loop_3;
}

void func_ov031_0210d044(Ov031MenuState* arg0) {
    s32 temp_hi;
    u16 temp_r0;
    u16 var_r6;

    var_r6 = 0;
    if ((u32)arg0->unk0 > 0U) {
        do {
            if (*((s32*)arg0->unk4 + var_r6) != 0) {
                func_ov031_0210ca38(arg0);
                *((s32*)arg0->unk4 + var_r6) = 0;
            }
            temp_r0 = var_r6 + 1;
            temp_hi = (u32)arg0->unk0 > (u32)temp_r0;
            var_r6  = temp_r0;
        } while (temp_hi);
    }
    Mem_Free(&gMainHeap, arg0->unk4);
    arg0->unk4 = NULL;
}

s32 func_ov031_0210d0b0(Ov031MenuState* arg0) {
    arg0->unk1E0 = 0xA;
    func_ov031_0210cdac(arg0);
    return 2;
}

s32 func_ov031_0210d0cc(Ov031MenuState* arg0) {
    u16 temp_r2;

    temp_r2 = arg0->unk1E0;
    if (temp_r2 != 0) {
        arg0->unk1E0 = (u16)(temp_r2 - 1);
        return 0;
    }
    func_ov031_0210cdac(arg0);
    return 2;
}

s32 func_ov031_0210d0fc(Ov031MenuState* arg0) {
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov031_0210d10c(void) {
    return 1;
}

s32 func_ov031_0210d114(void) {
    return 0;
}
