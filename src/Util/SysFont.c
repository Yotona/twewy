#include "Util/SysFont.h"
#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include <nitro/types.h>
#include <stdarg.h>

#include <nitro/os/os_owner.h>

// DS-code -> system-font-code hash table (SysFontDsCodeMap).
#define SYSFONT_CODEMAP_SLOTS      0x209
#define SYSFONT_CODEMAP_MAX_PROBES 0x1C
#define SYSFONT_CODEMAP_NOT_FOUND  (-1)

// sysCode 4 is substituted on lookup; see SysFont_DsCodesToSysCodes.
#ifdef REGION_USA
    #define SYSFONT_SYSCODE_SUBST_FROM 4
    #define SYSFONT_SYSCODE_SUBST_TO   358
#endif

// Screen extents, used as "larger than any coordinate" seeds.
#define SYSFONT_SCREEN_WIDTH  0x100
#define SYSFONT_SCREEN_HEIGHT 0xC0

// BG map dimensions in tiles, selected by the screen-size arg of SysFont_BlitGlyphToScreen.
#define SYSFONT_BG_TILES_256PX 0x20
#define SYSFONT_BG_TILES_512PX 0x40

// One pen advance in the units the specified release measures in.
#ifdef REGION_JP
    #define SYSFONT_ADVANCE(px, scale) ((u16)((px) * (scale)))
#else
    #define SYSFONT_ADVANCE(px, scale) (px)
#endif

// Pitch of the decoded-glyph working buffer, in bytes (one byte per pixel).
#ifdef REGION_JP
    #define SYSFONT_GLYPH_PITCH 0x30
#else
    #define SYSFONT_GLYPH_PITCH 0x10
#endif

// OBJ VRAM bases for the main and sub 2D engines.
#define SYSFONT_OBJ_VRAM_MAIN 0x06400000
#define SYSFONT_OBJ_VRAM_SUB  0x06600000

// One slot of the open-addressed DS-code -> system-font-code hash table
// (SYSFONT_CODEMAP_SLOTS entries, probed by SysFont_FindDsCodeSlot). dsCode == 0 marks an empty slot.
typedef struct {
    u16     dsCode;
    SysCode sysCode;
} Ov031DsCodeMap;

typedef struct {
    void* dst;
    u8    nibbleSelect;
    u8    yParity;
    u8    pad6[0x1A];
} SysFontPixelRef;

typedef struct {
    s32 offset;
    s32 size;
} MessageTableEntry;

static u16 SysFont_GetGlyphWidth(SysFont* font, s32 glyph);

static SysFontInfo sFontInfo_8x8;
static SysFontInfo sFontInfo_10x10;
#ifdef REGION_USA
static SysFontInfo sFontInfo_10x12; // fontId 3
#endif
static SysFontInfo sFontInfo_12x12;

static const SysFontInfo* SysFontInfoTable[] = {
    &sFontInfo_8x8,
    &sFontInfo_10x10,
    &sFontInfo_12x12,
#ifdef REGION_USA
    &sFontInfo_10x12,
#endif
};

/* clang-format off */
#define SYSFONT_INFO_8X8                                        \
    { 0x0005, {0x0001, 0x0002, 0x0003, 0xFFFF, 0xFFFF},         \
      0x0001, {0x0001, 0x0002, 0x0003, 0xFFFF, 0xFFFF},         \
      0x000E, 0x0000, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,   \
      0x0008, 0x0008, 0x0010, 0x0010, 0x0010, 0x0010 }
#define SYSFONT_INFO_10X10                                      \
    { 0x0006, {0x0001, 0x0002, 0x0003, 0x0004, 0xFFFF},         \
      0x0002, {0x0001, 0x0002, 0x0003, 0x0004, 0xFFFF},         \
      0x000E, 0x0000, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,   \
      0x000A, 0x000A, 0x000A, 0x000A, 0x0019, 0x0019 }
#define SYSFONT_INFO_10X12                                      \
    { 0x0009, {0x0001, 0x0002, 0xFFFF, 0xFFFF, 0xFFFF},         \
      0x0008, {0x0001, 0x0002, 0xFFFF, 0xFFFF, 0xFFFF},         \
      0x000E, 0x0000, 0x0000, 0x0001, 0x0000, 0x0000, 0x0000,   \
      0x000A, 0x000C, 0x000A, 0x000C, 0x0019, 0x0015 }
#define SYSFONT_INFO_12X12                                      \
    { 0x0007, {0x0001, 0x0002, 0xFFFF, 0xFFFF, 0xFFFF},         \
      0x0003, {0x0001, 0x0002, 0xFFFF, 0xFFFF, 0xFFFF},         \
      0x000E, 0x0000, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,   \
      0x000C, 0x000C, 0x0010, 0x0010, 0x0010, 0x0010 }
/* clang-format on */

#ifdef REGION_USA
static SysFontInfo sFontInfo_12x12 = SYSFONT_INFO_12X12;
static SysFontInfo sFontInfo_8x8   = SYSFONT_INFO_8X8;
static SysFontInfo sFontInfo_10x10 = SYSFONT_INFO_10X10;
static SysFontInfo sFontInfo_10x12 = SYSFONT_INFO_10X12;
#else
static SysFontInfo sFontInfo_8x8   = SYSFONT_INFO_8X8;
static SysFontInfo sFontInfo_10x10 = SYSFONT_INFO_10X10;
static SysFontInfo sFontInfo_12x12 = SYSFONT_INFO_12X12;
#endif

static Ov031DsCodeMap SysFontDsCodeMap[0x209] = {
    {0x006C, 0x004C},
    {0xE00A, 0x01B5},
    {0x0035, 0x0015},
    {0x0000, 0x0000},
    {0x30D6, 0x00E7},
    {0xFF0C, 0x010A},
    {0x00E7, 0x0194},
    {0x00B0, 0x013B},
    {0x3068, 0x0086},
    {0x0079, 0x0059},
    {0xE017, 0x01C2},
    {0x0042, 0x0022},
    {0x0000, 0x0000},
    {0x30E3, 0x00F4},
    {0x00F4, 0x01A1},
    {0x30AC, 0x00BD},
    {0x02C6, 0x0159},
    {0x3075, 0x0093},
    {0x0000, 0x0000},
    {0x0000, 0x0000},
    {0x004F, 0x002F},
    {0xFF5D, 0x012E},
    {0x30F0, 0x0101},
    {0x2191, 0x0150},
    {0x30B9, 0x00CA},
    {0x00CA, 0x0178},
    {0x3082, 0x00A0},
    {0x203A, 0x0161},
    {0x304B, 0x0069},
    {0x005C, 0x003C},
    {0x3014, 0x0129},
    {0x0025, 0x0005},
    {0x0000, 0x0000},
    {0x30C6, 0x00D7},
    {0x00D7, 0x0136},
    {0x308F, 0x00AD},
    {0x0000, 0x0000},
    {0x3058, 0x0076},
    {0x0069, 0x0049},
    {0x2010, 0x011D},
    {0x0032, 0x0012},
    {0xFF40, 0x0114},
    {0x30D3, 0x00E4},
    {0xFF09, 0x0128},
    {0x309C, 0x0112},
    {0x25BD, 0x014A},
    {0x3065, 0x0083},
    {0x0076, 0x0056},
    {0x201D, 0x0126},
    {0x003F, 0x001F},
    {0x266F, 0x0152},
    {0x30E0, 0x00F1},
    {0x0152, 0x015A},
    {0x30A9, 0x00BA},
    {0x00E4, 0x0191},
    {0x3072, 0x0090},
    {0x00F1, 0x019E},
    {0xE007, 0x01B2},
    {0x004C, 0x002C},
    {0xE014, 0x01BF},
    {0x30ED, 0x00FE},
    {0x25A0, 0x0147},
    {0x30B6, 0x00C7},
    {0x00C7, 0x0175},
    {0x307F, 0x009D},
    {0x0000, 0x0000},
    {0x3048, 0x0066},
    {0x0059, 0x0039},
    {0x0000, 0x0000},
    {0x0022, 0x0002},
    {0x0000, 0x0000},
    {0x30C3, 0x00D4},
    {0x00D4, 0x0182},
    {0x308C, 0x00AA},
    {0x0000, 0x0000},
    {0x3055, 0x0073},
    {0x0066, 0x0046},
    {0xE004, 0x01AF},
    {0x002F, 0x000F},
    {0xFF3D, 0x012C},
    {0x30D0, 0x00E1},
    {0xFF06, 0x013E},
    {0x00E1, 0x018E},
    {0x0000, 0x0000},
    {0x3062, 0x0080},
    {0x0073, 0x0053},
    {0x201A, 0x0156},
    {0x003C, 0x001C},
    {0x25C7, 0x0144},
    {0x30DD, 0x00EE},
    {0x00EE, 0x019B},
    {0x30A6, 0x00B7},
    {0x00B7, 0x016C},
    {0x306F, 0x008D},
    {0xE011, 0x01BC},
    {0x0000, 0x0000},
    {0x0049, 0x0029},
    {0x3001, 0x0108},
    {0x30EA, 0x00FB},
    {0x00FB, 0x01A8},
    {0x30B3, 0x00C4},
    {0x00C4, 0x0172},
    {0x307C, 0x009A},
    {0x0000, 0x0000},
    {0x3045, 0x0063},
    {0x0056, 0x0036},
    {0x0000, 0x0000},
    {0x0000, 0x0000},
    {0x0000, 0x0000},
    {0x30C0, 0x00D1},
    {0x00D1, 0x017F},
    {0x3089, 0x00A7},
    {0x0000, 0x0000},
    {0x3052, 0x0070},
    {0x0063, 0x0043},
    {0xE001, 0x01AC},
    {0x002C, 0x000C},
    {0x0000, 0x0000},
    {0x30CD, 0x00DE},
    {0x0000, 0x0000},
    {0x0000, 0x0000},
    {0x0000, 0x0000},
    {0x305F, 0x007D},
    {0x0070, 0x0050},
    {0xE00E, 0x01B9},
    {0x0039, 0x0019},
    {0x0000, 0x0000},
    {0x30DA, 0x00EB},
    {0x00EB, 0x0198},
    {0x30A3, 0x00B4},
    {0x00B4, 0x0113},
    {0x306C, 0x008A},
    {0x007D, 0x005D},
    {0xE01B, 0x01C6},
    {0x0046, 0x0026},
    {0x0000, 0x0000},
    {0x30E7, 0x00F8},
    {0xFF1D, 0x0138},
    {0x30B0, 0x00C1},
    {0x00C1, 0x016F},
    {0x3079, 0x0097},
    {0x00F8, 0x01A5},
    {0x3042, 0x0060},
    {0x0053, 0x0033},
    {0xE028, 0x01C8},
    {0x30F4, 0x0105},
    {0x20AC, 0x0155},
    {0x30BD, 0x00CE},
    {0x00CE, 0x017C},
    {0x3086, 0x00A4},
    {0x0000, 0x0000},
    {0x304F, 0x006D},
    {0x0060, 0x0040},
    {0x0000, 0x0000},
    {0x0029, 0x0009},
    {0x0000, 0x0000},
    {0x30CA, 0x00DB},
    {0x00DB, 0x0189},
    {0x3093, 0x00B1},
    {0x0000, 0x0000},
    {0x305C, 0x007A},
    {0x006D, 0x004D},
    {0xE00B, 0x01B6},
    {0x0036, 0x0016},
    {0x0000, 0x0000},
    {0x30D7, 0x00E8},
    {0xFF0D, 0x0134},
    {0x00E8, 0x0195},
    {0x00B1, 0x0135},
    {0x3069, 0x0087},
    {0x007A, 0x005A},
    {0x2605, 0x0140},
    {0x0043, 0x0023},
    {0x25CE, 0x0143},
    {0x30E4, 0x00F5},
    {0xFF1A, 0x010D},
    {0x30AD, 0x00BE},
    {0x00F5, 0x01A2},
    {0x3076, 0x0094},
    {0xE018, 0x01C3},
    {0x0000, 0x0000},
    {0x0050, 0x0030},
    {0xFF5E, 0x0120},
    {0x30F1, 0x0102},
    {0x3008, 0x012F},
    {0x30BA, 0x00CB},
    {0x2192, 0x014E},
    {0x3083, 0x00A1},
    {0x203B, 0x014C},
    {0x304C, 0x006A},
    {0x005D, 0x003D},
    {0x3015, 0x012A},
    {0x0026, 0x0006},
    {0x00CB, 0x0179},
    {0x30C7, 0x00D8},
    {0x00D8, 0x0186},
    {0x3090, 0x00AE},
    {0x00A1, 0x0163},
    {0x3059, 0x0077},
    {0x006A, 0x004A},
    {0x0153, 0x0162},
    {0x0033, 0x0013},
    {0xE008, 0x01B3},
    {0x30D4, 0x00E5},
    {0x00E5, 0x0192},
    {0x309D, 0x0118},
    {0x00AE, 0x0168},
    {0x3066, 0x0084},
    {0x0077, 0x0057},
    {0x201E, 0x0157},
    {0x0040, 0x0020},
    {0x25CB, 0x0141},
    {0x30E1, 0x00F2},
    {0x00F2, 0x019F},
    {0x30AA, 0x00BB},
    {0xE015, 0x01C0},
    {0x3073, 0x0091},
    {0x2234, 0x013A},
    {0x0000, 0x0000},
    {0x004D, 0x002D},
    {0x3005, 0x011A},
    {0x30EE, 0x00FF},
    {0xFF5B, 0x012D},
    {0x30B7, 0x00C8},
    {0x25A1, 0x0146},
    {0x3080, 0x009E},
    {0x00C8, 0x0176},
    {0x3049, 0x0067},
    {0x005A, 0x003A},
    {0x3012, 0x014D},
    {0x0023, 0x0003},
    {0x30FB, 0x010C},
    {0x30C4, 0x00D5},
    {0xFFE3, 0x0117},
    {0x308D, 0x00AB},
    {0x00D5, 0x0183},
    {0x3056, 0x0074},
    {0x0067, 0x0047},
    {0xE005, 0x01B0},
    {0x0030, 0x0010},
    {0xFF3E, 0x0116},
    {0x30D1, 0x00E2},
    {0x00E2, 0x018F},
    {0x266D, 0x0153},
    {0x0000, 0x0000},
    {0x3063, 0x0081},
    {0x0074, 0x0054},
    {0xE012, 0x01BD},
    {0x003D, 0x001D},
    {0x0000, 0x0000},
    {0x30DE, 0x00EF},
    {0x00EF, 0x019C},
    {0x30A7, 0x00B8},
    {0x0000, 0x0000},
    {0x3070, 0x008E},
    {0x0000, 0x0000},
    {0x0000, 0x0000},
    {0x004A, 0x002A},
    {0x3002, 0x0109},
    {0x30EB, 0x00FC},
    {0x00FC, 0x01A9},
    {0x30B4, 0x00C5},
    {0x00C5, 0x0173},
    {0x307D, 0x009B},
    {0x0000, 0x0000},
    {0x3046, 0x0064},
    {0x0057, 0x0037},
    {0x0000, 0x0000},
    {0x0020, 0x0000},
    {0x0000, 0x0000},
    {0x30C1, 0x00D2},
    {0x00D2, 0x0180},
    {0x308A, 0x00A8},
    {0x0000, 0x0000},
    {0x3053, 0x0071},
    {0x0064, 0x0044},
    {0xE002, 0x01AD},
    {0x002D, 0x000D},
    {0xFF3B, 0x012B},
    {0x30CE, 0x00DF},
    {0x00DF, 0x018C},
    {0x266A, 0x0154},
    {0x00A8, 0x0115},
    {0x3060, 0x007E},
    {0x0071, 0x0051},
    {0x2018, 0x0123},
    {0x003A, 0x001A},
    {0xE00F, 0x01BA},
    {0x30DB, 0x00EC},
    {0x00EC, 0x0199},
    {0x30A4, 0x00B5},
    {0x0000, 0x0000},
    {0x306D, 0x008B},
    {0x007E, 0x005E},
    {0xE01C, 0x01C7},
    {0x0047, 0x0027},
    {0x0000, 0x0000},
    {0x30E8, 0x00F9},
    {0x00F9, 0x01A6},
    {0x30B1, 0x00C2},
    {0x00C2, 0x0170},
    {0x307A, 0x0098},
    {0x2032, 0x013C},
    {0x3043, 0x0061},
    {0x0054, 0x0034},
    {0x300C, 0x0131},
    {0x30F5, 0x0106},
    {0x0000, 0x0000},
    {0x30BE, 0x00CF},
    {0x00CF, 0x017D},
    {0x3087, 0x00A5},
    {0x0000, 0x0000},
    {0x3050, 0x006E},
    {0x0061, 0x0041},
    {0x0000, 0x0000},
    {0x002A, 0x000A},
    {0x0000, 0x0000},
    {0x30CB, 0x00DC},
    {0xFF01, 0x0110},
    {0x00DC, 0x018A},
    {0x0000, 0x0000},
    {0x305D, 0x007B},
    {0x006E, 0x004E},
    {0x2015, 0x011C},
    {0x0037, 0x0017},
    {0x221E, 0x0139},
    {0x30D8, 0x00E9},
    {0xFF0E, 0x010B},
    {0x30A1, 0x00B2},
    {0x00E9, 0x0196},
    {0x306A, 0x0088},
    {0x007B, 0x005B},
    {0x2606, 0x013F},
    {0x0044, 0x0024},
    {0x25CF, 0x0142},
    {0x30E5, 0x00F6},
    {0xFF1B, 0x010E},
    {0x30AE, 0x00BF},
    {0x2022, 0x015E},
    {0x3077, 0x0095},
    {0x00BF, 0x016D},
    {0x00F6, 0x01A3},
    {0x0051, 0x0031},
    {0x3009, 0x0130},
    {0x30F2, 0x0103},
    {0x2193, 0x0151},
    {0x30BB, 0x00CC},
    {0x00CC, 0x017A},
    {0x3084, 0x00A2},
    {0xE00C, 0x01B7},
    {0x304D, 0x006B},
    {0x005E, 0x003E},
    {0xE019, 0x01C4},
    {0x0027, 0x0007},
    {0x25B2, 0x0149},
    {0x30C8, 0x00D9},
    {0x00D9, 0x0187},
    {0x3091, 0x00AF},
    {0x00A2, 0x0164},
    {0x305A, 0x0078},
    {0x006B, 0x004B},
    {0xE009, 0x01B4},
    {0x0034, 0x0014},
    {0x0000, 0x0000},
    {0x30D5, 0x00E6},
    {0xFF0B, 0x0133},
    {0x309E, 0x0119},
    {0x00E6, 0x0193},
    {0x3067, 0x0085},
    {0x0078, 0x0058},
    {0xE016, 0x01C1},
    {0x0041, 0x0021},
    {0x0000, 0x0000},
    {0x30E2, 0x00F3},
    {0x00F3, 0x01A0},
    {0x30AB, 0x00BC},
    {0x0000, 0x0000},
    {0x3074, 0x0092},
    {0x0000, 0x0000},
    {0x0000, 0x0000},
    {0x004E, 0x002E},
    {0xFF5C, 0x0121},
    {0x30EF, 0x0100},
    {0x2190, 0x014F},
    {0x30B8, 0x00C9},
    {0x00C9, 0x0177},
    {0x3081, 0x009F},
    {0x2122, 0x0160},
    {0x304A, 0x0068},
    {0x005B, 0x003B},
    {0x0000, 0x0000},
    {0x0024, 0x0004},
    {0x30FC, 0x011B},
    {0x30C5, 0x00D6},
    {0x00D6, 0x0184},
    {0x308E, 0x00AC},
    {0x0000, 0x0000},
    {0x3057, 0x0075},
    {0x0068, 0x0048},
    {0xE006, 0x01B1},
    {0x0031, 0x0011},
    {0x25BC, 0x014B},
    {0x30D2, 0x00E3},
    {0xFF08, 0x0127},
    {0x309B, 0x0111},
    {0x00E3, 0x0190},
    {0x3064, 0x0082},
    {0x0075, 0x0055},
    {0x201C, 0x0125},
    {0x003E, 0x001E},
    {0xE013, 0x01BE},
    {0x30DF, 0x00F0},
    {0x00F0, 0x019D},
    {0x30A8, 0x00B9},
    {0x0000, 0x0000},
    {0x3071, 0x008F},
    {0x0000, 0x0000},
    {0x0000, 0x0000},
    {0x004B, 0x002B},
    {0x0000, 0x0000},
    {0x30EC, 0x00FD},
    {0x00FD, 0x01AA},
    {0x30B5, 0x00C6},
    {0x00C6, 0x0174},
    {0x307E, 0x009C},
    {0x0000, 0x0000},
    {0x3047, 0x0065},
    {0x0058, 0x0038},
    {0x0000, 0x0000},
    {0x0021, 0x0001},
    {0x0000, 0x0000},
    {0x30C2, 0x00D3},
    {0x02DC, 0x015F},
    {0x308B, 0x00A9},
    {0x00D3, 0x0181},
    {0x3054, 0x0072},
    {0x0065, 0x0045},
    {0xE003, 0x01AE},
    {0x002E, 0x000E},
    {0xFF3C, 0x011F},
    {0x30CF, 0x00E0},
    {0x00E0, 0x018D},
    {0x0000, 0x0000},
    {0x00A9, 0x0167},
    {0x3061, 0x007F},
    {0x0072, 0x0052},
    {0x2019, 0x0124},
    {0x003B, 0x001B},
    {0x25C6, 0x0145},
    {0x30DC, 0x00ED},
    {0x00ED, 0x019A},
    {0x30A5, 0x00B6},
    {0xE010, 0x01BB},
    {0x306E, 0x008C},
    {0x2026, 0x0122},
    {0x0000, 0x0000},
    {0x0048, 0x0028},
    {0x3000, 0x01C9},
    {0x30E9, 0x00FA},
    {0xFF1F, 0x010F},
    {0x30B2, 0x00C3},
    {0x00C3, 0x0171},
    {0x307B, 0x0099},
    {0x2033, 0x013D},
    {0x3044, 0x0062},
    {0x0055, 0x0035},
    {0x300D, 0x0132},
    {0x30F6, 0x0107},
    {0x00FA, 0x01A7},
    {0x30BF, 0x00D0},
    {0x00D0, 0x017E},
    {0x3088, 0x00A6},
    {0x0000, 0x0000},
    {0x3051, 0x006F},
    {0x0062, 0x0042},
    {0xE000, 0x01AB},
    {0x002B, 0x000B},
    {0x0000, 0x0000},
    {0x30CC, 0x00DD},
    {0x00DD, 0x018B},
    {0x0000, 0x0000},
    {0x0000, 0x0000},
    {0x305E, 0x007C},
    {0x006F, 0x004F},
    {0xE00D, 0x01B8},
    {0x0038, 0x0018},
    {0x0000, 0x0000},
    {0x30D9, 0x00EA},
    {0xFF0F, 0x011E},
    {0x30A2, 0x00B3},
    {0x00EA, 0x0197},
    {0x306B, 0x0089},
    {0x007C, 0x005C},
    {0xE01A, 0x01C5},
    {0x0045, 0x0025},
    {0x0000, 0x0000},
    {0x30E6, 0x00F7},
    {0x00F7, 0x0137},
    {0x30AF, 0x00C0},
    {0x00C0, 0x016E},
    {0x3078, 0x0096},
    {0x0000, 0x0000},
    {0x3041, 0x005F},
    {0x0052, 0x0032},
    {0x0000, 0x0000},
    {0x30F3, 0x0104},
    {0x0000, 0x0000},
    {0x30BC, 0x00CD},
    {0x00CD, 0x017B},
    {0x3085, 0x00A3},
    {0x0000, 0x0000},
    {0x304E, 0x006C},
    {0x005F, 0x003F},
    {0x0000, 0x0000},
    {0x0028, 0x0008},
    {0x25B3, 0x0148},
    {0x30C9, 0x00DA},
    {0x00DA, 0x0188},
    {0x3092, 0x00B0},
    {0x00A3, 0x0165},
    {0x305B, 0x0079},
};

static const BinIdentifier FontBinId            = {0x80000002, "Apl_Fuk/Grp_Font.bin"};
static const BinIdentifier MesTxtBinId          = {30, "Apl_Fuk/mestxt.bin"};
static const BinIdentifier MesTableBinId        = {30, "Apl_Fuk/mestable.bin"};
static u16                 PaletteEntryOrder[4] = {1, 2, 3, 0};

static SysFontPixelRef sPixelRef;

void SysFont_GetCellBoundsMin(const Sprite* sprite, s32* minX, s32* minY) {
    s32                 table_index = sprite->cellIndex * 4;
    const u16*          table       = (const u16*)sprite->cellTable;
    u16                 i;
    s32                 min_x = SYSFONT_SCREEN_WIDTH;
    s32                 min_y = SYSFONT_SCREEN_HEIGHT;
    u16                 count = table[(table_index + 1)];
    const OamCellPiece* entry = (const OamCellPiece*)(table + table[table_index]);

    i = 0;

    while (count > i) {
        OamMgr_GetCellShape((OamCellPiece*)entry);
        s32 raw_x = (u16)(entry->attr1 & 0x1FF);
        s32 raw_y = entry->attr0 & 0xFF;
        entry++;
#ifdef REGION_JP
        if (raw_x & 0x80) {
            raw_x |= -0x200;
        }
        if (raw_y & 0x40) {
            raw_y |= -0x100;
        }
#else
        if (raw_x & 0x100) {
            raw_x |= -0x200;
        }
        if (raw_y & 0x80) {
            raw_y |= -0x100;
        }
#endif
        if (min_x > raw_x) {
            min_x = raw_x;
        }
        if (min_y > raw_y) {
            min_y = raw_y;
        }
        i++;
    }

    *minX = min_x;
    *minY = min_y;
}

u16 SysFont_WriteU32(SysCode* dst, u32 value) {
    u16 digits = 0;

    if (value == 0) {
        dst[0] = SYSFONT_GLYPH_DIGIT_0;
        return 1;
    }

    u32 work = value;
    while (work != 0) {
        work /= 10;
        digits++;
    }

    work = value;
    for (u16 i = 0; i < digits; i++) {
        dst[(digits - i) - 1] = (work % 10) + SYSFONT_GLYPH_DIGIT_0;
        work /= 10;
    }

    return digits;
}

u16 SysFont_WriteS32(SysCode* dst, s32 value) {
    u16  digits   = 0;
    BOOL negative = value < 0;

    if (value == 0) {
        dst[0] = SYSFONT_GLYPH_DIGIT_0;
        return 1;
    }

    s32 work = value;
    while (work != 0) {
        work /= 10;
        digits++;
    }

    if (negative) {
        dst[0] = SYSFONT_GLYPH_MINUS;
        value  = -value;
        for (u16 i = 0; i < digits; i++) {
            dst[digits - i] = (value % 10) + SYSFONT_GLYPH_DIGIT_0;
            value /= 10;
        }
        return digits + 1;
    }

    for (u16 i = 0; i < digits; i++) {
        dst[(digits - i) - 1] = (value % 10) + SYSFONT_GLYPH_DIGIT_0;
        value /= 10;
    }

    return digits;
}

u16 SysFont_WriteString(SysCode* dst, const SysCode* src) {
    SysCode value = *src;
    u16     count = 0;

    if (value != SYSFONT_CODE_STR_END) {
        do {
            *dst++ = value;
            value  = *++src;
            count++;
        } while (value != SYSFONT_CODE_STR_END);
    }
    return count;
}

s32 SysFont_WriteChar(SysCode* dst, SysCode code) {
    *dst = code;
    return 1;
}

u16 SysFont_WriteFx32(SysCode* dst, s32 value, u16 fracDigits) {
    u16 prefix = 0;

    if (value & 0x80000000) {
        *dst = SYSFONT_GLYPH_MINUS;
        prefix++;
    }

    s32 scaled = F2I(value);
    if (fracDigits == 0) {
        u16 intFmt[2] = {SYSFONT_CODE_FMT_U32, SYSFONT_CODE_STR_END};

        return prefix + SysFont_Format(dst, intFmt, scaled);
    }

    u32 frac = 0;
    frac /= 10;
    for (u16 i = 0; i < 4 - fracDigits; i++) {
        frac /= 10;
    }

    SysCode fracFmt[4] = {SYSFONT_CODE_FMT_U32, SYSFONT_GLYPH_PERIOD, SYSFONT_CODE_FMT_U32, SYSFONT_CODE_STR_END};
    return prefix + SysFont_Format(dst, fracFmt, scaled, frac);
}

// Nonmatching: r7/r8 are swapped between the format pointer and the va_list; the instruction
// sequence is otherwise identical.
u16 SysFont_Format(SysCode* dst, const SysCode* fmt, ...) {
    const u16* src   = fmt;
    u16        count = 0;
    va_list    args;
    va_start(args, fmt);
    while (SysFont_IsTerminator(*src) == FALSE) {
        SysCode code = *src++;
        switch (code) {
            case SYSFONT_CODE_FMT_U32:
                count += SysFont_WriteU32(dst + count, va_arg(args, u32));
                break;

            case SYSFONT_CODE_FMT_S32:
                count += SysFont_WriteS32(dst + count, va_arg(args, s32));
                break;

            case SYSFONT_CODE_FMT_STR:
                count += SysFont_WriteString(dst + count, va_arg(args, const u16*));
                break;

            case SYSFONT_CODE_FMT_CHR:
                count += SysFont_WriteChar(dst + count, va_arg(args, u16));
                break;

            case SYSFONT_CODE_FMT_FX0:
            case SYSFONT_CODE_FMT_FX1:
            case SYSFONT_CODE_FMT_FX2:
            case SYSFONT_CODE_FMT_FX3:
            case SYSFONT_CODE_FMT_FX4:
                count += SysFont_WriteFx32(dst + count, va_arg(args, s32), (u16)(code % 10));
                break;

            default:
                dst[count] = code;
                count++;
                break;
        }
    }

    dst[count] = SYSFONT_CODE_STR_END;
    return count;
}

BOOL SysFont_IsTerminator(SysCode code) {
    switch (code) {
        case SYSFONT_CODE_MSG_END:
        case SYSFONT_CODE_STR_END:
            return TRUE;
    }
    return FALSE;
}

// Nonmatching: r0/r1 are swapped between SysFontInfoTable[fontId] and the constant 1.
void SysFont_InitEx(SysFont* font, s32 fontId, BOOL extendedGlyphs, BOOL loadGlyphBitmaps) {
    font->fontId = fontId;

    u16 lineSpacing     = 1;
    font->info          = SysFontInfoTable[fontId];
    font->letterSpacing = 1;
#ifdef REGION_USA
    if (fontId != 3)
#endif
    {
        lineSpacing = 2;
    }
    font->lineSpacing    = lineSpacing;
    font->color          = 14;
    font->extendedGlyphs = extendedGlyphs;
    font->widthScale     = 1;
    font->msgData        = NULL;
    font->proportional   = TRUE;
    font->hasMsg         = FALSE;
    font->x              = 0;
    font->clipRight      = 0x200;
    font->y              = 0;
    font->clipBottom     = 0x200;
    font->hAlign         = 1;
    font->vAlign         = 3;
    font->mesTableData   = DatMgr_LoadRawData(1, NULL, 0, &MesTableBinId);

    const SysFontInfo* fontInfo = SysFontInfoTable[fontId];
    if (loadGlyphBitmaps) {
        u16 charPackId = fontInfo->charPackId;

        if (BinMgr_FindById((s32)&FontBinId.id) == NULL) {
            font->charData = DatMgr_LoadPackEntry(1, NULL, 0, &FontBinId, charPackId, 1);
        } else {
            font->charData = DatMgr_LoadPackEntryDirect(1, &FontBinId, charPackId, 1);
        }

        for (u16 glyphBankIndex = 0; glyphBankIndex < 5; glyphBankIndex++) {
            u16 charPackIdForBank = font->info->charEntry[glyphBankIndex];

            font->glyphBanks[glyphBankIndex] = NULL;
            if (charPackIdForBank != SYSFONT_PACK_NONE) {
                if ((extendedGlyphs != 0) || ((glyphBankIndex != 1) && (glyphBankIndex != 2) && (glyphBankIndex != 3))) {
                    font->glyphBanks[glyphBankIndex] = Data_GetPackEntryData(font->charData, charPackIdForBank);
                }
            }
        }
    } else {
        font->charData = NULL;
        for (u16 bankIndex = 0; bankIndex < 5; bankIndex++) {
            font->glyphBanks[bankIndex] = NULL;
        }
    }

    u16 widthPackId = fontInfo->widthPackId;

    if (BinMgr_FindById((s32)&FontBinId) == NULL) {
        font->widthData = DatMgr_LoadPackEntry(1, NULL, 0, &FontBinId, widthPackId, 1);
    } else {
        font->widthData = DatMgr_LoadPackEntryDirect(1, &FontBinId, widthPackId, 1);
    }

    for (u16 widthBankIndex = 0; widthBankIndex < 5; widthBankIndex++) {
        u16 widthPackIdForBank = font->info->widthEntry[widthBankIndex];

        font->widthBanks[widthBankIndex] = NULL;
        if (widthPackIdForBank != SYSFONT_PACK_NONE) {
            if ((extendedGlyphs != 0) || ((widthBankIndex != 1) && (widthBankIndex != 2) && (widthBankIndex != 3))) {
                font->widthBanks[widthBankIndex] = Data_GetPackEntryData(font->widthData, widthPackIdForBank);
            }
        }
    }
}

void SysFont_Init(SysFont* font) {
    SysFont_InitEx(font, 1, TRUE, TRUE);
}

void SysFont_InitWithFont(SysFont* font, s32 fontId, BOOL extendedGlyphs) {
    SysFont_InitEx(font, fontId, extendedGlyphs, TRUE);
}

void SysFont_Destroy(SysFont* font) {
    if (font->mesTableData != NULL) {
        DatMgr_ReleaseData(font->mesTableData);
    }
    if (font->charData != NULL) {
        DatMgr_ReleaseData(font->charData);
        font->charData = NULL;
    }
    if (font->widthData != NULL) {
        DatMgr_ReleaseData(font->widthData);
        font->widthData = NULL;
    }
    if (font->msgData != NULL) {
        DatMgr_ReleaseData(font->msgData);
        font->msgData = NULL;
    }
    font->msg = NULL;
}

void SysFont_SetPos(SysFont* font, s32 x, s32 y) {
    font->x = x;
    font->y = y;
}

void SysFont_SetColor(SysFont* font, s32 color) {
    font->color = (u8)color;
}

void SysFont_SetHAlign(SysFont* font, s32 align, s32 boxWidth) {
    font->hAlign   = align;
    font->boxWidth = boxWidth;
}

void SysFont_SetVAlign(SysFont* font, s32 align, u16 boxHeight) {
    font->vAlign    = align;
    font->boxHeight = boxHeight;
}

void SysFont_SetSpacing(SysFont* font, BOOL proportional, u16 letterSpacing) {
    font->proportional  = proportional;
    font->letterSpacing = letterSpacing;
}

void SysFont_SetLineSpacing(SysFont* font, u16 lineSpacing) {
    font->lineSpacing = lineSpacing;
}

void* SysFont_GetAllocPal(s32 paletteIndex) {
    Data* fontPackData = DatMgr_LoadPackEntry(1, NULL, 0, &FontBinId, 4, 1);
    u16   paletteEntry = PaletteEntryOrder[paletteIndex];
    void* src          = Data_GetPackEntryData(fontPackData, paletteEntry);
    void* dst          = Mem_AllocHeapTail(&gDebugHeap, 0x20);

    Mem_SetSequence(&gDebugHeap, dst, "SysFont_GetAllocPal");
    MI_CpuCopy(src, dst, 0x20);
    DatMgr_ReleaseData(fontPackData);
    return dst;
}

// Nonmatching (both regions): as in SysFont_BlitGlyphToChar, the target folds
// (srcRow << 0x1D) >> 0x1B into the ldrb addressing mode at both uses while mwcc hoists it into a
// register; the rest is register allocation. The bank divide/modulo are emitted in the order
// written below (quotient first), which is what the target does -- swapping them costs eight
// instructions in the prologue even though objdiff's match_percent goes *up*.
void SysFont_DecodeGlyph(SysFont* font, u8* dst, s32 glyph) {
    s32                srcCellWidth;
    u8*                srcByte;
    s32                srcCol;
    s32                srcRow;
    void*              bankData;
    s32                col;
    s32                clearCol;
    s32                clearOffset;
    s32                clearRow;
    s32                row;
    s8*                clearPtr;
    s8*                dstRow;
    u16                bankIndex;
    u16                cellWidth;
    u16                glyphsPerBank;
    u16                glyphInBank;
    u16                cellHeight;
    u16                glyphsPerRow;
    u16                srcCellHeight;
    s32                col_base;
    s32                row_base;
    u32                colMod8;
    u8                 byteValue;
    const SysFontInfo* info;

    info          = font->info;
    glyphsPerRow  = info->glyphsPerRow;
    glyphsPerBank = glyphsPerRow * info->glyphRows;
    bankIndex     = (u16)(glyph / (s32)glyphsPerBank);
    glyphInBank   = (u16)(glyph % (s32)glyphsPerBank);
    cellHeight    = info->cellHeight;
    cellWidth     = info->cellWidth;
    srcCellWidth  = (s32)info->srcCellWidth;
    srcCellHeight = info->srcCellHeight;
    bankData      = font->glyphBanks[bankIndex];
#ifdef REGION_USA
    // A glyph outside the loaded banks blanks the cell instead of decoding through a null bank.
    if (((u32)bankIndex >= 5U) || (bankData == 0)) {
        clearRow = 0;
        if ((s32)cellHeight <= 0) {
            return;
        }
        clearOffset = 0;
        do {
            clearCol = 0;
            if ((s32)cellWidth > 0) {
                clearPtr = dst + clearOffset;
                do {
                    clearCol++;
                    *clearPtr = 0;
                    clearPtr++;
                } while (clearCol < (s32)cellWidth);
            }
            clearRow++;
            clearOffset += SYSFONT_GLYPH_PITCH;
        } while (clearRow < (s32)cellHeight);
        return;
    }
#endif
    col_base = srcCellWidth * ((s32)glyphInBank % (s32)glyphsPerRow);
    row_base = srcCellHeight * ((s32)glyphInBank / (s32)glyphsPerRow);
    row      = 0;
    if ((s32)cellHeight <= 0) {
        return;
    }
    do {
        col = 0;
        if ((s32)cellWidth > 0) {
            srcRow = row_base + row;
            dstRow = (s8*)dst + (row * SYSFONT_GLYPH_PITCH);
            do {
                srcCol  = col_base + col;
                colMod8 = srcCol & 7;
                srcByte = (u8*)bankData +
                          ((((s32)(srcCol + ((u32)(srcCol >> 2) >> 0x1D)) >> 3) +
                            (((s32)(srcRow + ((u32)(srcRow >> 2) >> 0x1D)) >> 3) << 5))
                           << 5) +
                          ((s32)(colMod8 + (colMod8 >> 0x1F)) >> 1);
                byteValue = *(srcByte + ((u32)(srcRow << 0x1D) >> 0x1B));
                col += 2;
                dstRow[0] = (s8)(byteValue & 0xF);
                dstRow[1] = (s8)(((s32) * (srcByte + ((u32)(srcRow << 0x1D) >> 0x1B)) >> 4) & 0xF);
                dstRow += 2;
            } while (col < (s32)cellWidth);
        }
        row++;
    } while (row < (s32)cellHeight);
}

#ifdef REGION_JP
/**
 * @brief Scales the decoded glyph in @p glyph up by @p scale, in place.
 *
 * Nearest-neighbour, walked from the far corner backwards so a source row is never read after
 * it has been overwritten.
 *
 * Nonmatching: instruction-for-instruction identical, but the target keeps `size` in r11 and
 * the row counter in r4 where mwcc picks r9/r4; no declaration order flips it.
 */
void SysFont_ScaleGlyph(SysFont* font, u8* glyph, s32 scale) {
    s32 size;
    s32 row;
    s32 col;

    (void)font;

    if (scale == 1) {
        return;
    }
    size = scale * 0x10;
    if (size < 0) {
        return;
    }
    row = size;
    do {
        col = size;
        if (col >= 0) {
            u8* dst = (glyph + (row * SYSFONT_GLYPH_PITCH)) + size;
            u8* src = glyph + ((row / scale) * SYSFONT_GLYPH_PITCH);

            do {
                *dst-- = src[col / scale];
                col--;
            } while (col >= 0);
        }
        row--;
    } while (row >= 0);
}
#endif

// Nonmatching
void SysFont_BlitGlyphToChar(SysFont* font, u8* glyph, void* charData, u16 color, u16 skipTransparent, u16 widthTiles,
                             u16 heightTiles, u16 dstX, u16 dstY, u16 scale) {
    s32 rowTile;
    s32 heightPx;
    s32 col;
    s32 row;
    s32 baseX;
    s32 clipRight;
    s32 baseY;
    s32 clipBottom;
    s32 srcOffset;
    s32 tileWidthPx;
    s32 tilesPerRow;
    s32 cellWidth;
    s32 cellHeight;

#ifdef REGION_USA
    cellHeight = font->info->cellHeight;
    cellWidth  = font->info->cellWidth;
#else
    cellWidth  = font->info->cellWidth * scale;
    cellHeight = cellWidth;
#endif
    clipRight  = font->clipRight;
    clipBottom = font->clipBottom;
    row        = 0;
    baseX      = font->x + dstX;
    baseY      = font->y + dstY;
    if (cellHeight <= 0) {
        return;
    }
    heightPx    = heightTiles;
    srcOffset   = row;
    tileWidthPx = widthTiles * 8;
    tilesPerRow = tileWidthPx / 8;
    do {
        col = 0;
        if (cellWidth > 0) {
            u8* src    = glyph + srcOffset;
            s32 y      = baseY + row;
            u32 yShift = (u32)(y << 0x1D);

            rowTile = (y / 8) * tilesPerRow;
            do {
                if ((*src != 0) || (skipTransparent != 1)) {
                    s32 x = baseX + col;

                    if ((x < clipRight) && (x < tileWidthPx) && (y < clipBottom) && (y < heightPx * 8)) {
                        u8  pixel          = (u8)((*src + color) - font->info->baseColorIndex);
                        u8* dst            = (u8*)charData + ((rowTile + (x / 8)) << 5) + ((x & 7) / 2);
                        u8  existing       = dst[yShift >> 0x1B];
                        int preserve_shift = (x & 1) ? 0 : 4;
                        int insert_shift   = (x & 1) ? 4 : 0;

                        dst[yShift >> 0x1B] = (u8)((existing & (0xF << preserve_shift)) | (pixel << insert_shift));
                    }
                }
                src++;
                col++;
            } while (col < cellWidth);
        }
        srcOffset += SYSFONT_GLYPH_PITCH;
        row++;
    } while (row < cellHeight);
}

// Nonmatching
void SysFont_BlitGlyphToScreen(SysFont* font, u8* glyph, u16* map, void* charData, u8 color, s32 skipTransparent, u16 dstX,
                               u16 dstY, u16 scale, u32 screenSize) {
    s32 mapWidthTiles;
    s32 row;
    s32 mapHeightTiles;
    s32 baseX;
    s32 clipRight;
    s32 baseY;
    s32 clipBottom;
    u16 cellWidth;
#ifdef REGION_USA
    u16 cellHeight;
#endif
    s32 srcOffset;

    baseX      = font->x + dstX;
    baseY      = font->y + dstY;
    cellWidth  = font->info->cellWidth;
    clipRight  = font->clipRight;
    clipBottom = font->clipBottom;
#ifdef REGION_USA
    cellHeight = font->info->cellHeight;
#endif

    switch (screenSize) {
        case 0:
            mapWidthTiles  = SYSFONT_BG_TILES_256PX;
            mapHeightTiles = SYSFONT_BG_TILES_256PX;
            break;
        case 1:
            mapWidthTiles  = SYSFONT_BG_TILES_512PX;
            mapHeightTiles = SYSFONT_BG_TILES_256PX;
            break;
        case 2:
            mapWidthTiles  = SYSFONT_BG_TILES_256PX;
            mapHeightTiles = SYSFONT_BG_TILES_512PX;
            break;
        case 3:
            mapWidthTiles  = SYSFONT_BG_TILES_512PX;
            mapHeightTiles = SYSFONT_BG_TILES_512PX;
            break;
        default:
            OS_WaitForever();
            break;
    }

    row = 0;
#ifdef REGION_USA
    s32 heightPx = cellHeight * scale;
    if (heightPx <= 0) {
        return;
    }
    s32 widthPx = cellWidth * scale;
#else
    s32 widthPx  = cellWidth * scale;
    s32 heightPx = widthPx;
    if (heightPx <= 0) {
        return;
    }
#endif
    srcOffset = row;
    do {
        s32 col = 0;
        if (widthPx > 0) {
            u8* src       = glyph + srcOffset;
            s32 y         = baseY + row;
            s32 yMod      = y % 8;
            s32 rowOffset = (y / 8) * mapWidthTiles;
            u32 yShift    = (u32)(yMod << 0x1D);

            do {
                if ((*src != 0) || (skipTransparent != 1)) {
                    s32 x = baseX + col;

                    if ((x < clipRight) && (x < mapWidthTiles * 8) && (y < clipBottom) && (y < mapHeightTiles * 8)) {
                        u8  pixel    = (u8)((*src + color) - font->info->baseColorIndex);
                        s32 xMod     = x % 8;
                        u16 mapIndex = (u16)(rowOffset + (x / 8));
                        u16 tile     = (u16)(map[mapIndex] & 0x3FF);
                        u8* dst      = (u8*)charData + (tile << 5) + (((xMod / 8) + (yMod / 8)) << 5) + ((xMod & 7) / 2);
                        dst[yShift >> 0x1B] =
                            (u8)((dst[yShift >> 0x1B] & (0xF << ((x & 1) ? 0 : 4))) | (pixel << ((x & 1) ? 4 : 0)));
                    }
                }
                src++;
                col++;
            } while (col < widthPx);
        }
        srcOffset += SYSFONT_GLYPH_PITCH;
        row++;
    } while (row < heightPx);
}

// Nonmatching: regswaps
SysFontPixelRef* SysFont_ResolveCellPixel(Sprite* sprite, s32 px, s32 py, const void* cellPieces, s32 bitmapIndexDiv4,
                                          u32 pieceCount, s32 minX, s32 minY, BOOL toVram) {
    const OamCellPiece* entry = cellPieces;
    const OamCellPiece* hit;

    u16 count = 0;

    hit = NULL;
    u32 hit_x;
    u32 hit_y;

    while (pieceCount > count) {
        const OamCellShape* bounds = OamMgr_GetCellShape(entry);
        {
            u32 x = entry->attr1 & 0x1FF;
            u32 y = entry->attr0 & 0xFF;

            if (x >> 8) {
                x |= -0x200;
            }
            if (y >> 7) {
                y |= -0x100;
            }

            s32 dx = (s32)x - minX;
            s32 dy = (s32)y - minY;
            if ((px >= dx) && (py >= dy)) {
                if ((px < dx + bounds->width) && (py < dy + bounds->height)) {
                    if (hit == NULL) {
                        hit   = entry;
                        hit_x = dx;
                        hit_y = dy;
                    }
                }
            }

            entry++;
            count++;
        }
    }

    if (hit != NULL) {
        const OamCellShape* bounds = OamMgr_GetCellShape((OamCellPiece*)hit);
        u32                 rel_x  = (u16)(px - hit_x);
        u32                 rel_y  = (u16)(py - hit_y);

        s32 base;
        if (toVram != 0) {
            if (sprite->bits_0_1 == 0) {
                base = (sprite->charData->bitmapIndex << 5) + SYSFONT_OBJ_VRAM_MAIN;
            } else {
                base = (sprite->charData->bitmapIndex << 5) + SYSFONT_OBJ_VRAM_SUB;
            }
        } else {
            base = (s32)sprite->unk34 + 4;
        }

        s32 tile;
        if (toVram != 0) {
            if (sprite->bits_3_4 == 1) {
                tile = hit->attr2 & 0x3FF;
            } else {
                tile = hit->charName & 0x1FFF;
            }
        } else {
            tile = hit->charName & 0x1FFF;
        }

        s32 width              = bounds->width / 8;
        s32 offset             = ((rel_y >> 3) * width + (rel_x >> 3)) << 5;
        s32 fine               = (s32)(rel_x & 7) / 2;
        s32 byte_index         = (u32)(rel_y << 0x1D) >> 0x1B;
        sPixelRef.dst          = (void*)(base + (tile << 5) + offset + fine + byte_index);
        sPixelRef.nibbleSelect = (u8)((px + hit_x) & 1);
        sPixelRef.yParity      = (u8)((py + hit_y) & 1);
    } else {
        sPixelRef.dst = NULL;
    }

    return &sPixelRef;
}

// Nonmatching (both regions): instruction scheduling in the prologue, plus register allocation.
// The frame size, every stack slot and the whole loop body now match.
void SysFont_BlitGlyphToSprite(SysFont* font, u8* glyph, Sprite* sprite, s32 color, s32 skipTransparent, u16 dstX, u16 dstY,
                               u16 scale, BOOL toVram) {
    s32         cellEntry;
    const u16*  table;
    s32         row;
    const void* cellPieces;
    u32         pieceCount;
    s32         bitmapIndexDiv4;
    s32         baseX;
    s32         clipRight;
    s32         baseY;
    s32         clipBottom;
    s32         srcOffset;
    s32         cellWidth;
#ifdef REGION_USA
    s32 cellHeight;
#endif

    cellEntry  = sprite->cellIndex * 4;
    table      = (const u16*)sprite->cellTable;
    pieceCount = table[cellEntry + 1];
    cellWidth  = font->info->cellWidth;
#ifdef REGION_USA
    cellHeight = font->info->cellHeight;
#endif
    cellPieces      = table + table[cellEntry];
    bitmapIndexDiv4 = sprite->charData->bitmapIndex / 4;
    baseX           = font->x + dstX;
    clipRight       = font->clipRight;
    baseY           = font->y + dstY;
    clipBottom      = font->clipBottom;

    s32 minX;
    s32 minY;
    SysFont_GetCellBoundsMin(sprite, &minX, &minY);

    row = 0;
#ifdef REGION_JP
    // As in the other two blitters.
    s32 cellHeight = cellWidth * scale;
    cellWidth      = cellHeight;
#endif
    if (cellHeight <= 0) {
        return;
    }
    srcOffset = 0;
    do {
        s32 col = 0;
        if (cellWidth > 0) {
            u8* src = glyph + srcOffset;
            s32 y   = baseY + row;

            do {
                if ((*src != 0) || (skipTransparent != TRUE)) {
                    s32 x = baseX + col;

                    if ((x < clipRight) && (y < clipBottom)) {
                        u8               value = ((*src + color) - font->info->baseColorIndex);
                        SysFontPixelRef* hit = SysFont_ResolveCellPixel(sprite, x, y, cellPieces, bitmapIndexDiv4, pieceCount,
                                                                        minX, minY, toVram);
                        u8*              dst = hit->dst;

                        if (dst != NULL) {
                            u8 pixelByte = (u8)((*dst & (0xF << ((hit->nibbleSelect != 0) ? 0 : 4))) |
                                                (value << ((hit->nibbleSelect != 0) ? 4 : 0)));
                            if (toVram == 0) {
                                *dst = pixelByte;
                            } else {
                                MI_CpuCopyU8(&pixelByte, dst, 1);
                            }
                        }
                    }
                }
                src++;
                col++;
            } while (col < cellWidth);
        }
        srcOffset += SYSFONT_GLYPH_PITCH;
        row++;
    } while (row < cellHeight);
}

void SysFont_SetMsgPtr(SysFont* font, SysCode* msg) {
    if (font->msgData != NULL) {
        DatMgr_ReleaseData(font->msgData);
        font->msgData = NULL;
    }
    font->msg    = msg;
    font->hasMsg = TRUE;
}

void SysFont_SetMsg(SysFont* font, u16 msgIndex) {
    if (font->msgData != NULL) {
        DatMgr_ReleaseData(font->msgData);
        font->msgData = NULL;
    }

    const MessageTableEntry* entries = font->mesTableData->buffer;
    Data* newMsgData = DatMgr_LoadRawDataWithOffset(1, NULL, entries[msgIndex].size, &MesTxtBinId, entries[msgIndex].offset);
    font->msgData    = newMsgData;
    font->msg        = newMsgData->buffer;
    font->hasMsg     = TRUE;
}

SysCode* SysFont_GetMsgBuf(SysFont* font, u16 msgIndex) {
    Data* msgData;

    u16                      length  = 1;
    const MessageTableEntry* entries = font->mesTableData->buffer;
    msgData = DatMgr_LoadRawDataWithOffset(1, NULL, entries[msgIndex].size, &MesTxtBinId, entries[msgIndex].offset);

    SysCode* cursor = msgData->buffer;
    while (*cursor != SYSFONT_CODE_STR_END) {
        cursor++;
        length++;
    }

    SysCode* dst = Mem_AllocHeapTail(&gDebugHeap, length * 2);
    Mem_SetSequence(&gDebugHeap, dst, "SysFont_GetMsgBuf");
    MI_CpuCopyU16(msgData->buffer, dst, length * 2);
    DatMgr_ReleaseData(msgData);
    return dst;
}

// Nonmatching
s32 SysFont_DrawInternal(SysFont* font, SysCode* msg, void* charData, u16 widthTiles, u16 heightTiles, u16* map,
                         void* screenCharData, u32 screenSize, Sprite* dstSprite, BOOL toVram, u32 firstGlyph, u16 glyphCount,
                         s32 dstKind) {
    u8  glyphBits[SYSFONT_GLYPH_PITCH * SYSFONT_GLYPH_PITCH];
    u16 lineWidths[0x100];
    u16 penX;
    u16 penY;

    u32 clearCount = sizeof(glyphBits) / 4;
    u8* clear      = glyphBits;
    do {
        clear[0] = 0;
        clear[1] = 0;
        clear[2] = 0;
        clear[3] = 0;
        clear += 4;
        clearCount--;
    } while (clearCount != 0);

    u16                count      = 0;
    u8                 color      = font->color;
    u16                line       = 0;
    u16                limit      = firstGlyph + glyphCount;
    s32                lastCode   = SYSFONT_CODE_STR_END;
    const SysFontInfo* info       = font->info;
    u16                widthScale = font->widthScale;
    SysFont_MeasureLineWidths(font, msg, lineWidths);
    switch (font->hAlign) {
        case 0:
            penX = (font->boxWidth - lineWidths[0]) / 2;
            break;
        case 2:
            penX = font->boxWidth - lineWidths[0];
            break;
        default:
            penX = 0;
            break;
    }
    switch (font->vAlign) {
        case 0:
            penY = (font->boxHeight - SysFont_MeasureHeight(font, msg)) / 2;
            break;
        case 4:
            penY = font->boxHeight - SysFont_MeasureHeight(font, msg);
            break;
        default:
            penY = 0;
            break;
    }
    while ((SysFont_IsTerminator(*msg) == FALSE) && (count < limit)) {
        switch (*msg) {
            case SYSFONT_GLYPH_SPACE:
                penX = (u16)(penX + SYSFONT_ADVANCE(SYSFONT_SPACE_ADVANCE, widthScale)) +
                       SYSFONT_ADVANCE(font->letterSpacing, widthScale);
                count++;
                break;
            case SYSFONT_GLYPH_WIDE_SPACE:
                penX = (u16)(penX + SYSFONT_ADVANCE(info->cellWidth, widthScale)) +
                       SYSFONT_ADVANCE(font->letterSpacing, widthScale);
                count++;
                break;
            case SYSFONT_CODE_WIDTH_1X:
                widthScale = 1;
                break;
            case SYSFONT_CODE_WIDTH_2X:
                widthScale = 2;
                break;
            // 0xFFB1..0xFFBF: set the text colour.
            case 0xFFB1:
            case 0xFFB2:
            case 0xFFB3:
            case 0xFFB4:
            case 0xFFB5:
            case 0xFFB6:
            case 0xFFB7:
            case 0xFFB8:
            case 0xFFB9:
            case 0xFFBA:
            case 0xFFBB:
            case 0xFFBC:
            case 0xFFBD:
            case 0xFFBE:
            case 0xFFBF:
                color = *msg & 0xF;
                break;
                // Format specifiers are consumed by SysFont_Format; drawing skips them.
#ifdef REGION_USA
            case SYSFONT_CODE_FMT_FX0:
            case SYSFONT_CODE_FMT_FX1:
            case SYSFONT_CODE_FMT_FX2:
            case SYSFONT_CODE_FMT_FX3:
            case SYSFONT_CODE_FMT_FX4:
            case SYSFONT_CODE_FMT_U32:
            case SYSFONT_CODE_FMT_S32:
            case SYSFONT_CODE_FMT_STR:
            case SYSFONT_CODE_FMT_CHR:
#endif
            case SYSFONT_CODE_MSG_END:
                break;
            case SYSFONT_CODE_LINEBREAK:
                line++;
                switch (font->hAlign) {
                    case 0:
                        penX = (font->boxWidth - lineWidths[line]) / 2;
                        break;
                    case 2:
                        penX = font->boxWidth - lineWidths[line];
                        break;
                    default:
                        penX = 0;
                        break;
                }
#ifdef REGION_USA
                penY += font->info->cellHeight + font->lineSpacing;
#else
                penY += font->lineSpacing + (u16)(widthScale * info->cellHeight);
#endif
                break;
            default:
                if (count < firstGlyph) {
                    penX = (u16)(penX + SYSFONT_ADVANCE(SysFont_GetGlyphWidth(font, *msg), widthScale)) +
                           SYSFONT_ADVANCE(font->letterSpacing, widthScale);
                    count++;
                } else {
                    count++;
                    SysFont_DecodeGlyph(font, glyphBits, *msg);
#ifdef REGION_JP
                    SysFont_ScaleGlyph(font, glyphBits, widthScale);
#endif
                    switch (dstKind) {
                        case 1:
                            SysFont_BlitGlyphToChar(font, glyphBits, charData, color, 1, widthTiles, heightTiles, penX, penY,
                                                    widthScale);
                            break;
                        case 2:
                            SysFont_BlitGlyphToScreen(font, glyphBits, map, screenCharData, color, 1, penX, penY, widthScale,
                                                      screenSize);
                            break;
                        case 3:
                            SysFont_BlitGlyphToSprite(font, glyphBits, dstSprite, color, 1, penX, penY, widthScale, toVram);
                            if (toVram == 0) {
                                dstSprite->charData->unk_12      = -1;
                                dstSprite->charData->unk_0A      = -1;
                                dstSprite->resourceData->binIden = NULL;
                            }
                            break;
                        default:
                            OS_WaitForever();
                            break;
                    }
                    penX = (u16)(penX + SYSFONT_ADVANCE(SysFont_GetGlyphWidth(font, *msg), widthScale)) +
                           SYSFONT_ADVANCE(font->letterSpacing, widthScale);
                }
                break;
        }
        lastCode = *msg++;
    }
    if (SysFont_IsTerminator(*msg) != FALSE) {
        lastCode = *msg;
    }
    DC_PurgeAll();
    return lastCode;
}

s32 SysFont_DrawToCharRange(SysFont* font, SysCode* msg, void* charData, u16 widthTiles, u16 heightTiles, u16 firstGlyph,
                            u16 glyphCount) {
    return SysFont_DrawInternal(font, msg, charData, widthTiles, heightTiles, NULL, NULL, 0xFFFF, NULL, 0, firstGlyph,
                                glyphCount, 1);
}

s32 SysFont_DrawToChar(SysFont* font, SysCode* msg, void* charData, u16 widthTiles, u16 heightTiles) {
    return SysFont_DrawToCharRange(font, msg, charData, widthTiles, heightTiles, 0U, SYSFONT_NO_LIMIT);
}

s32 SysFont_DrawCurrentToChar(SysFont* font, void* charData, u16 widthTiles, u16 heightTiles) {
    if (font->msg == 0) {
        OS_WaitForever();
    }
    return SysFont_DrawToChar(font, font->msg, charData, widthTiles, heightTiles);
}

s32 SysFont_DrawToScreenRange(SysFont* font, SysCode* msg, u16* map, void* charData, u32 screenSize, u16 firstGlyph,
                              u16 glyphCount) {
    return SysFont_DrawInternal(font, msg, NULL, 0, 0, map, charData, screenSize, NULL, 0, firstGlyph, glyphCount, 2);
}

s32 SysFont_DrawToScreen(SysFont* font, SysCode* msg, u16* map, void* charData, u32 screenSize) {
    return SysFont_DrawToScreenRange(font, msg, map, charData, screenSize, 0, SYSFONT_NO_LIMIT);
}

s32 SysFont_DrawCurrentToScreen(SysFont* font, u16* map, void* charData, s32 screenSize) {
    if (font->msg == NULL) {
        OS_WaitForever();
    }
    return SysFont_DrawToScreen(font, font->msg, map, charData, screenSize);
}

s32 SysFont_DrawToSpriteRange(SysFont* font, SysCode* msg, Sprite* sprite, BOOL toVram, u16 firstGlyph, u16 glyphCount) {
    return SysFont_DrawInternal(font, msg, NULL, 0, 0, NULL, NULL, 0xFFFF, sprite, toVram, firstGlyph, glyphCount, 3);
}

s32 SysFont_DrawToSprite(SysFont* font, SysCode* msg, Sprite* sprite, BOOL toVram) {
    return SysFont_DrawToSpriteRange(font, msg, sprite, toVram, 0, SYSFONT_NO_LIMIT);
}

s32 SysFont_DrawCurrentToSprite(SysFont* font, Sprite* sprite, BOOL toVram) {
    if (font->msg == NULL) {
        OS_WaitForever();
    }
    return SysFont_DrawToSpriteRange(font, font->msg, sprite, toVram, 0, SYSFONT_NO_LIMIT);
}

s32 SysFont_DrawMsgToSprite(SysFont* font, s32 msgIndex, Sprite* sprite, BOOL toVram) {
    if (font->msg == NULL) {
        OS_WaitForever();
    }

    SysCode* msg = SysFont_GetMsgAt(font->msg, msgIndex);
    if (msg == NULL) {
        return SYSFONT_MSG_NOT_FOUND;
    }
    return SysFont_DrawToSpriteRange(font, msg, sprite, toVram, 0, SYSFONT_NO_LIMIT);
}

s32 SysFont_DrawMsgToSpriteRange(SysFont* font, s32 msgIndex, Sprite* sprite, BOOL toVram, u16 firstGlyph, u16 glyphCount) {
    if (font->msg == NULL) {
        OS_WaitForever();
    }

    SysCode* msg = SysFont_GetMsgAt(font->msg, msgIndex);
    if (msg == NULL) {
        return SYSFONT_MSG_NOT_FOUND;
    }
    return SysFont_DrawToSpriteRange(font, msg, sprite, toVram, firstGlyph, glyphCount);
}

#ifdef REGION_USA
u16 SysFont_CountMsgs(SysCode* msg) {
    u16 index;
    u16 count = 1;

    for (index = 0;; index++) {
        SysCode code = msg[index];
        if (code == SYSFONT_CODE_STR_END) {
            break;
        }
        if (code == SYSFONT_CODE_MSG_END) {
            count++;
        }
    }
    return count;
}

u16 SysFont_LoadMsgCount(s32 entryIndex) {
    Data* mesTableData = DatMgr_LoadRawData(1, NULL, 0, &MesTableBinId);

    const MessageTableEntry* entries = mesTableData->buffer;

    Data* msgData = DatMgr_LoadRawDataWithOffset(1, NULL, entries[entryIndex].size, &MesTxtBinId, entries[entryIndex].offset);
    DatMgr_ReleaseData(mesTableData);
    u16 msgCount = SysFont_CountMsgs(msgData->buffer);
    DatMgr_ReleaseData(msgData);
    return msgCount;
}
#endif

SysCode* SysFont_GetMsgAt(SysCode* msg, s32 msgIndex) {
    u16 index;

    u16 msgCount = 0;
    if (msgIndex == 0) {
        return msg;
    }

    for (index = 0;; index++) {
        SysCode code = msg[index];
        if (code == SYSFONT_CODE_STR_END) {
            break;
        }
        if (code == SYSFONT_CODE_MSG_END) {
            if (msgIndex == (++msgCount)) {
                return msg + (index + 1);
            }
        }
    }
    return NULL;
}

BOOL SysFont_IsProportionalGlyph(u32 glyph) {
    BOOL result = FALSE;
    if ((glyph <= 94) || ((glyph >= 341) && (glyph <= 426)) || ((glyph >= 468) && (glyph <= 519))) {
        result = TRUE;
    }
    if (glyph & 0xF000) {
        result = FALSE;
    }
    return result;
}

u16 SysFont_GetGlyphWidth(SysFont* font, s32 glyph) {
    const SysFontInfo* info = font->info;

    u16 glyphsPerBank = font->info->glyphsPerRow * font->info->glyphRows;
    u16 bankIndex     = glyph / glyphsPerBank;
    u16 glyphInBank   = glyph % glyphsPerBank;

#ifdef REGION_USA
    if (bankIndex >= 5) {
        return 0;
    }
#endif
    switch (glyph) {
        case SYSFONT_GLYPH_SPACE:
            return SYSFONT_SPACE_ADVANCE;
        case SYSFONT_GLYPH_WIDE_SPACE:
            return info->cellWidth;
    }
    if (SysFont_IsProportionalGlyph(glyph) == FALSE) {
        return info->cellWidth;
    }
    if (font->proportional != 0) {
        return ((u8*)font->widthBanks[bankIndex])[glyphInBank];
    }
    return info->cellWidth;
}

u32 SysFont_MeasureWidthEx(SysFont* font, SysCode* msg, s32 from, u32 limit) {
    SysCode* cursor     = msg + from;
    u16      max_width  = 0;
    u16      line_width = 0;
    u16      count      = 0;
#ifdef REGION_JP
    u16 scale = font->widthScale;
#endif

    while ((SysFont_IsTerminator(*cursor) == FALSE) && (count < limit)) {
        SysCode code = *cursor;

#ifdef REGION_USA
        if ((code != SYSFONT_CODE_WIDTH_1X) && (code != SYSFONT_CODE_WIDTH_2X)) {
            if (code == SYSFONT_CODE_LINEBREAK) {
                if (line_width > max_width) {
                    max_width = line_width;
                }
                line_width = 0;
            } else if ((code & SYSFONT_CODE_MASK) != SYSFONT_CODE_MASK) {
                line_width = (u16)(line_width + SysFont_GetGlyphWidth(font, code));
                if (count != 0) {
                    line_width += font->letterSpacing;
                }
                count++;
            }
        }
#else
        switch (code) {
            case SYSFONT_CODE_WIDTH_1X:
                scale = 1;
                break;
            case SYSFONT_CODE_WIDTH_2X:
                scale = 2;
                break;
            case SYSFONT_CODE_LINEBREAK:
                if (line_width > max_width) {
                    max_width = line_width;
                }
                line_width = 0;
                break;
            default:
                if ((code & SYSFONT_CODE_MASK) != SYSFONT_CODE_MASK) {
                    line_width += (SysFont_GetGlyphWidth(font, code) * scale);
                    if (count != 0) {
                        line_width += (scale * font->letterSpacing);
                    }
                    count++;
                }
                break;
        }
#endif
        cursor++;
        count++;
    }

    if (line_width > max_width) {
        max_width = line_width;
    }
    return max_width;
}

u32 SysFont_MeasureWidth(SysFont* font, SysCode* msg) {
    return SysFont_MeasureWidthEx(font, msg, 0, SYSFONT_NO_LIMIT);
}

u32 SysFont_MeasureCurrentWidth(SysFont* font) {
    return SysFont_MeasureWidth(font, font->msg);
}

u32 SysFont_MeasureMsgWidth(SysFont* font, s32 msgIndex) {
    SysCode* msg = SysFont_GetMsgAt(font->msg, msgIndex);
    if (msg != NULL) {
        return SysFont_MeasureWidth(font, msg);
    }
    return 0;
}

u32 SysFont_MeasureLineWidthsEx(SysFont* font, SysCode* msg, u16* lineWidths, s32 from, u32 limit) {
    SysCode* cursor     = msg + from;
    u16      line_width = 0;
    u16      count      = 0;
#ifdef REGION_JP
    u16 scale = font->widthScale;
#endif
    u16 line_index = 0;

    while ((SysFont_IsTerminator(*cursor) == FALSE) && (count < limit)) {
#ifdef REGION_USA
        if ((*cursor != SYSFONT_CODE_WIDTH_1X) && (*cursor != SYSFONT_CODE_WIDTH_2X)) {
            if (*cursor == SYSFONT_CODE_LINEBREAK) {
                lineWidths[line_index] = line_width;
                line_width             = 0;
                line_index++;
            } else if ((*cursor & SYSFONT_CODE_MASK) != SYSFONT_CODE_MASK) {
                line_width = (u16)(line_width + SysFont_GetGlyphWidth(font, *cursor));
                if (count != 0) {
                    line_width = (u16)(line_width + font->letterSpacing);
                }
                count++;
            }
        }
#else
        switch (*cursor) {
            case SYSFONT_CODE_WIDTH_1X:
                scale = 1;
                break;
            case SYSFONT_CODE_WIDTH_2X:
                scale = 2;
                break;
            case SYSFONT_CODE_LINEBREAK:
                lineWidths[line_index] = line_width;
                line_index++;
                line_width = 0;
                break;
            default:
                if ((*cursor & SYSFONT_CODE_MASK) != SYSFONT_CODE_MASK) {
                    line_width += (SysFont_GetGlyphWidth(font, *cursor) * scale);
                    if (count != 0) {
                        line_width += (scale * font->letterSpacing);
                    }
                    count++;
                }
                break;
        }
#endif
        cursor++;
        count++;
    }

    lineWidths[line_index] = line_width;
    line_index++;
    return line_index;
}

u32 SysFont_MeasureLineWidths(SysFont* font, SysCode* msg, u16* lineWidths) {
    return SysFont_MeasureLineWidthsEx(font, msg, lineWidths, 0, SYSFONT_NO_LIMIT);
}

u32 SysFont_MeasureMsgLineWidths(SysFont* font, s32 msgIndex, u16* lineWidths) {
    SysCode* msg = SysFont_GetMsgAt(font->msg, msgIndex);
    if (msg == NULL) {
        return 0;
    }
    return SysFont_MeasureLineWidths(font, msg, lineWidths);
}

u16 SysFont_MeasureHeight(SysFont* font, SysCode* msg) {
    return SysFont_MeasureHeightEx(font, msg, SYSFONT_NO_LIMIT);
}

u16 SysFont_MeasureHeightEx(SysFont* font, SysCode* msg, u16 limit) {
    u16 sum       = 0;
    u16 count     = 0;
    u16 width     = font->widthScale;
    u16 width_max = width;

    while ((SysFont_IsTerminator(*msg) == FALSE) && (count < limit)) {
        switch (*msg) {
            case SYSFONT_CODE_WIDTH_1X:
                width = 1;
                if (width_max < 1) {
                    width_max = 1;
                }
                break;
            case SYSFONT_CODE_WIDTH_2X:
                width = 2;
                if (width_max < 2) {
                    width_max = 2;
                }
                break;
            case SYSFONT_CODE_LINEBREAK: {
                sum += (width_max * font->info->cellHeight + font->lineSpacing);
                width_max = width;
                break;
            }
            default:
                if ((*msg & SYSFONT_CODE_MASK) == SYSFONT_CODE_MASK) {
                    count++;
                }
                break;
        }

        msg++;
    }

    return (width_max * font->info->cellHeight) + sum;
}

u16 SysFont_MeasureCurrentHeight(SysFont* font) {
    return SysFont_MeasureHeight(font, font->msg);
}

u16 SysFont_MeasureMsgHeight(SysFont* font, s32 msgIndex) {
    SysCode* msg = SysFont_GetMsgAt(font->msg, msgIndex);
    if (msg != NULL) {
        return SysFont_MeasureHeight(font, msg);
    }
    return 0;
}

#ifdef REGION_USA
u16 SysFont_GetStrBufLen(s32 unused, SysCode* msg) {
    u16 count = 0;
    while (SysFont_IsTerminator(*msg) == FALSE) {
        msg++;
        count++;
    }
    return count + 1;
}
#endif

static u16 SysFont_HashDsCode(u32 dsCode) {
    for (s32 i = 0; i < 4; i++) {
        dsCode += i + dsCode * 0x10;
    }
    return dsCode % SYSFONT_CODEMAP_SLOTS;
}

static s32 SysFont_FindDsCodeSlot(u16 dsCode) {
    s32 firstSlot = SysFont_HashDsCode(dsCode);
    s32 slot      = firstSlot;
    s32 probes    = 0;
    do {
        if (dsCode == SysFontDsCodeMap[slot].dsCode) {
            return slot;
        }
        slot = (slot + 1) % SYSFONT_CODEMAP_SLOTS;
        probes++;
    } while ((slot != firstSlot) && (probes <= SYSFONT_CODEMAP_MAX_PROBES));
    return SYSFONT_CODEMAP_NOT_FOUND;
}

static void SysFont_DsCodesToSysCodes(const u16* dsCodes, SysCode* sysCodes, s32 count) {
    for (s32 i = 0; i < count; i++) {
        s32 index = SysFont_FindDsCodeSlot(dsCodes[i]);
        if (index >= 0) {
#ifdef REGION_USA
            sysCodes[i] = SysFontDsCodeMap[index].sysCode;
            if (sysCodes[i] == SYSFONT_SYSCODE_SUBST_FROM) {
                sysCodes[i] = SYSFONT_SYSCODE_SUBST_TO;
            }
#else
            sysCodes[i] = SysFontDsCodeMap[index].sysCode;
#endif
        } else {
            sysCodes[i] = SYSFONT_CODE_STR_END;
        }
    }

    sysCodes[count] = SYSFONT_CODE_STR_END;
}

SysCode* SysFont_GetSysCodeBuf_from_DsCode(const u16* dsCodes, s32 length) {
    SysCode* buf = Mem_AllocHeapTail(&gDebugHeap, (length + 1) * 2);
    Mem_SetSequence(&gDebugHeap, buf, "SysFont_GetSysCodeBuf_from_DsCode");
    SysFont_DsCodesToSysCodes(dsCodes, buf, length);
    return buf;
}

SysCode* SysFont_GetOwnerName(void) {
    OSOwnerInfo info;

    OS_GetOwnerInfo(&info);
    return SysFont_GetSysCodeBuf_from_DsCode(info.nickName, 10);
}

SysCode* SysFont_GetOwnerMessage(void) {
    OSOwnerInfo info;

    OS_GetOwnerInfo(&info);
    return SysFont_GetSysCodeBuf_from_DsCode(info.message, 26);
}

SysCode* SysFont_GetUserDSCommentBufReturn(void) {
    SysCode fmt[4] = {SYSFONT_CODE_FMT_STR, SYSFONT_CODE_LINEBREAK, SYSFONT_CODE_FMT_STR, SYSFONT_CODE_STR_END};

    // The owner's message is up to 26 characters, shown as two 13-code lines: one copy is truncated to
    // the first line, and the second is passed in at a 13-code (26-byte) offset for the second line.
    SysCode* line1      = SysFont_GetOwnerMessage();
    SysCode* messageBuf = SysFont_GetOwnerMessage();
    line1[13]           = SYSFONT_CODE_STR_END;

    SysCode* commentBuf = Mem_AllocHeapTail(&gDebugHeap, 0x38);
    Mem_SetSequence(&gDebugHeap, commentBuf, "SysFont_GetUserDSCommentBufReturn");
    SysFont_Format(commentBuf, fmt, line1, messageBuf + 13);
    Mem_Free(&gDebugHeap, line1);
    Mem_Free(&gDebugHeap, messageBuf);
    return commentBuf;
}
