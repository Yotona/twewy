// ============================================================================
// main_36.c - Game Data Management and Save System
// ============================================================================
// This overlay manages the core game data systems:
// - Inventory management (256 item slots, 304 pin slots, 472 thread slots)
// - Save/load system with dual-slot redundancy and checksums
// - Item/badge/thread ownership tracking and collection counting
// - Character stats and equipment calculations
// - Global friend data (50 slots)
// - Game progress flags and unlock conditions
// - Text rendering utilities
// ============================================================================

#include "BgResMgr.h"
#include "DatMgr.h"
#include "Memory.h"
#include "common_data.h"
#include <NitroSDK/os.h>
#include <types.h>

// ============================================================================
// External Data Declarations
// ============================================================================

extern s32            SystemStatusFlags;
extern struct MemPool gDebugHeap;
extern struct MemPool gMainHeap;

// Data table references
extern s32 data_0205c120;
extern s32 data_0205c1e4;
extern s32 data_0205c210;
extern s32 data_0205c23c;
extern s32 data_0205c270;
extern s32 data_0205c2a4;
extern s32 data_0205c2dc;
extern s32 data_0205c5be;

// BinIdentifier references
extern BinIdentifier data_0205c128;
extern BinIdentifier data_0205c130;
extern BinIdentifier data_0205c138;
extern BinIdentifier data_0205c140;
extern BinIdentifier data_0205c148;
extern BinIdentifier data_0205c150;
extern BinIdentifier data_0205c158;
extern BinIdentifier data_0205c160;
extern BinIdentifier data_0205c168;
extern BinIdentifier data_0205c170;
extern BinIdentifier data_0205c178;
extern BinIdentifier data_0205c180;
extern BinIdentifier data_0205c188;

extern s32 data_0205c96c;
extern s32 data_0205c990;
extern s32 data_0205ca94;
extern s32 data_0205caa4;
extern s32 data_0205cac4;

extern s32             data_0206b3cc[];
extern s32             data_0206b3d8[];
extern BgResMgr*       g_BgResourceManagers[2];
extern DisplaySettings g_DisplaySettings;

// Static data with initializers
static const char data_02065bd8[0xC]  = "badge_data";
static const char data_02065be4[0x10] = "BackUpData_Tmp";
static const char data_02065bf4[0x14] = "GlobalFriendData";
static s32        data_02065c24[14]   = {0};
static const char data_02065c84[0x10] = "Tsk_CellText";

// ============================================================================
// Data Structures for Memory Regions
// ============================================================================
// These structures map to specific memory addresses and store game state:
// - SystemState: Save operation state and special item tracking
// - MainData: Core game inventory (items, pins, threads) and player data
// - ItemSlot: Individual item storage (256 slots)
// - PinData: Badge/pin storage with quantity (304 slots, max 99 each)
// - ThreadData: Clothing/thread storage with level (472 slots, max level 9)
// ============================================================================

// data_02071cf0 - system state structure
struct SystemState {
    u16 state;
    u16 subState;
    s32 field_04;
    s32 lockID;
    s32 globalFriendData;
    s32 backupData1;
    s32 backupData2;
    s32 field_18;
    s32 field_1C;
    u16 unk20;
    u16 unk22;
    u16 unk30;
    u16 unk32;
    u16 unk94;
    u8  unk9C;
    u16 unk9E;
    u8  unkA6;
    u16 unkA8;
    u8  unkB0;
    u16 unkB2;
    u8  unkBA;
    u16 unkBC;
    u8  unkC4;
    u16 unkC6;
    u8  unkCE;
};

extern struct SystemState data_02071cf0;

// data_02071d10 - main data structure
struct MainData {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
    u16 unk08;
    u16 unk0A;
    u16 unk0C;
    u16 unk0E;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 unk1C[4];
    u16 unk34[0xC];
    u16 unk74[6];
    u16 unkB0[0x100];
    u16 unkAB0[0x130];
    u16 unkF70[0x1D8];
    s16 unk1FCC[0x60];
    u8  unk1D98[0x29];
    u8  unk22CC[0x100];
    u8  unk2CC[0x100];
};

extern struct MainData data_02071d10;

// data_02071dc0 - item slots
struct ItemSlot {
    u16 itemID;
    u16 field_02;
    u16 field_04;
    u16 field_06;
    u8  flags;
    u8  field_09;
};

extern struct ItemSlot data_02071dc8[0x100];

// data_020727c0 - pin data
struct PinData {
    u16 pinID;
    u8  quantity;
    u8  flags;
};

extern struct PinData data_020727c0[0x130];

// data_02072c80 - thread data
struct ThreadData {
    u16 threadID;
    u8  flags;
    u8  level;
};

extern struct ThreadData data_02072c80[0x1D8];

// data_02072cf0
struct Data02072CF0 {
    char pad[0xAC8];
    u32  unkAC8;
    char pad2[0x304];
    u32  unkDB4;
};

extern struct Data02072CF0 data_02072cf0;

// data_020736f0
struct Data020736F0 {
    char pad[0xC4];
    u16  unkC4;
    u16  unkC6;
    char pad2[0x0E];
    u16  unkD6;
    char pad3[0x08];
    u16  unkE0;
};

extern struct Data020736F0 data_020736f0;

// data_02073cf0
struct Data02073CF0 {
    char pad[0x48C];
    u32  unk48C;
    char pad2[0x14];
    s32  unk4A4;
};

extern struct Data02073CF0 data_02073cf0;

// data_02074cf0
struct Data02074CF0 {
    char pad[0x150];
    s32  unk150;
    char pad2[0x04];
    s32  unk158;
    s32  unk15C;
    char pad3[0xF4];
    s32  unk254;
    s32  unk258;
};

extern struct Data02074CF0 data_02074cf0;

extern u16 data_020737ca[4];
extern u8  data_02073aa8[0x29];
extern u8  data_02073adb[0xE7];
extern u8  data_02073bc2[0x118];
extern u16 data_02073ff0[0x10];
extern u16 data_02074036[0x11];
extern u32 data_02074184;
extern u32 data_0207418c;

// ============================================================================
// Function Prototypes
// ============================================================================

extern u32  func_0202296c(u16);
extern u32  func_020229d4(u16);
extern void func_020226fc(struct MainData*);
extern void func_02025b1c(void);

static u32 func_02023010(u32 arg0);

extern Data* DatMgr_LoadRawDataWithOffset(s32, void*, s32, BinIdentifier*, s32);
extern BOOL  DatMgr_ReleaseData(Data*);
extern Data* DatMgr_LoadRawData(s32, void*, s32, BinIdentifier*);

extern s32 MI_CpuFill(s32, void*, s32, s32, s32, s32);
extern s32 MI_CpuSet(void*, s32, s32);
extern s64 func_0203a444(void);

extern s32  func_02042330(void);
extern void func_0204237c(u16);
extern void func_0204238c(u16);
extern u8   func_02042944(void);
extern void func_0204296c(s32);
extern s32  func_02042ab0(void);
extern void func_02042aa4(void);
extern void func_0204285c(s32, void*, s32, s32, s32, s32, s32, s32, s32);

extern void func_020389c0(u16);

extern s32 func_020258bc(u16*, void*, s32, s32);
extern u16 func_0202593c(u16*, s32);
extern s32 func_02036710(void);
extern s32 func_02036808(void);

extern void* Mem_AllocHeapTail(struct MemPool*, s32);
extern void  Mem_Free(struct MemPool*, void*);
extern void  Mem_SetSequence(struct MemPool*, void*, const char*);
extern s32   Mem_GetBlockSize(struct MemPool*, void*);

extern s32  OS_GetLockID(void);
extern void OS_WaitForever(void);

extern void func_020104c0(void*, void*);
extern void func_020105b8(void*);
extern void func_02010b50(void*);
extern void func_0201187c(void*, void*);
extern void func_02011994(void*);
extern void func_02011e94(void*, s32, s32);
extern void func_02011ecc(void*);

extern void* func_0200a968(s32, const char*, s32, s32);
extern void  func_0200afec(s32, void*);
extern void* func_0200cb4c(s32, s32, s32, s32);
extern void  func_0200cd44(s32, void*);

extern void* BgResMgr_AllocCharAligned(BgResMgr*, s32, s32, s32);
extern BOOL  BgResMgr_ReleaseChar(BgResMgr*, BgResource*);

extern s32 G2_GetBG0ScrPtr(void);
extern s32 G2_GetBG1ScrPtr(void);
extern s32 G2_GetBG2ScrPtr(void);
extern s32 G2_GetBG3ScrPtr(void);
extern s32 G2S_GetBG0ScrPtr(void);
extern s32 G2S_GetBG1ScrPtr(void);

extern s32 func_02002a10(s32, s32, s32, s32, s32, s32);
extern s32 func_02053098(f64, f64);
extern s32 func_02054a9c(f64);

// ============================================================================
// Function Implementations
// ============================================================================
// Functions organized by category:
// - Inventory Management: Adding/checking items, pins, threads
// - Stat Calculation: Character stats with equipment bonuses
// - Flag Management: Game progress and unlock flags
// - Save System: Reading/writing save data with checksums
// - Collection Tracking: Badge/thread collection counters
// - Friend Data: Global friend list management
// - Text Rendering: UI text display utilities
// ============================================================================

// ============ func_02022a18 ============
// Calculates available inventory slots (max 256)
// Counts equipped items, item slots, and badges, returns free space
u16 func_02022a18(void) {
    u8  sp8[0x34];
    s32 sp4;
    s32 temp_r10;
    s32 temp_r1;
    u16 temp_r0;
    u16 temp_r1_2;
    u16 var_r4;
    u16 var_r6;
    u16 var_r7;

    var_r7 = 0;
    sp4    = 0;
    do {
        temp_r1 = sp4 * 0x3C;
        var_r6  = 0;
        do {
            temp_r10  = var_r6 * 0xA;
            temp_r1_2 = *(u16*)((s32)&data_02071d10 + temp_r1 + temp_r10 + 0x1DC2);
            if (temp_r1_2 != 0xFFFF) {
                DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp8, 0x34, &data_0205c188, temp_r1_2 * 0x34));
                if (sp8[0x25] != ((data_02073adb[temp_r1 + temp_r10] << 0x19) >> 0x19)) {
                    var_r7 += 1;
                }
            }
            var_r6 += 1;
        } while (var_r6 < 6);
        temp_r0 = sp4 + 1;
        sp4     = temp_r0;
    } while (temp_r0 < 4);
    var_r4 = 0;
    do {
        if (*(u16*)((s32)&data_02071d10 + var_r4 * 0xA + 0xB0) != 0xFFFF) {
            var_r7 += 1;
        }
        var_r4 += 1;
    } while (var_r4 < 0x100);
    if (var_r7 > 0x100) {
        return 0;
    }
    return (u16)(0x100 - var_r7);
}

// ============ func_02022b4c ============
// Checks if an item/pin can be added to inventory
// arg0: item ID, arg1: type (1=item, 2=pin)
// Returns 1 if can add, 0 if cannot
s32 func_02022b4c(u32 arg0, s32 arg1) {
    u8  sp4[0x34];
    s32 var_r6;
    u16 var_r3;
    u16 var_r3_2;
    u32 temp_r5;

    var_r6  = arg1;
    temp_r5 = func_020229d4((u16)arg0);
    if (arg0 >= 0x308U) {
        return 0;
    }
    if (var_r6 == 1) {
        DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp4, 0x34, &data_0205c158, (s32)arg0 * 0x34));
        if (sp4[0x25] == 1) {
            var_r6 = 2;
        }
    }
    if (temp_r5 == 0) {
        if (var_r6 == 1) {
            if (func_02022a18() != 0) {
                return 1;
            }
            return 0;
        }
        if (var_r6 == 2) {
            var_r3 = 0;
            while (1) {
                if (arg0 == *(u16*)((s32)&data_02071d10 + var_r3 * 4 + 0xA00 + 0xB0)) {
                    if ((u32)data_020727c0[var_r3].quantity < 0x63U) {
                        return 1;
                    }
                    return 0;
                }
                var_r3 += 1;
                if ((u32)var_r3 >= 0x130U) {
                    var_r3_2 = 0;
                    while (1) {
                        if (*(u16*)((s32)&data_02071d10 + var_r3_2 * 4 + 0xA00 + 0xB0) == 0xFFFF) {
                            return 1;
                        }
                        var_r3_2 += 1;
                        if ((u32)var_r3_2 >= 0x130U) {
                            return 0;
                        }
                    }
                }
            }
        }
        return 0;
    }
    if (func_02023010(arg0) < 9U) {
        return 1;
    }
    return 0;
}

// ============ func_02022c9c ============
// Adds an item to item slot inventory
// arg0: slot index, arg1: item ID, arg2: new item flag
void func_02022c9c(u16 arg0, s16 arg1, s32 arg2) {
    s32 temp_r3;

    temp_r3                                            = arg0 * 0xA;
    *(u16*)((s32)&data_02071dc8[0].itemID + temp_r3)   = arg1;
    *(u16*)((s32)&data_02071dc8[0].field_02 + temp_r3) = 0;
    *(u16*)((s32)&data_02071dc8[0].field_04 + temp_r3) = 0;
    *(u16*)((s32)&data_02071dc8[0].field_06 + temp_r3) = 0;
    data_02071dc8[arg0].flags                          = ((data_02071dc8[arg0].flags & ~0x7F) | 1) & ~0x80;
    data_02071dc8[arg0].flags |= (u8)(arg2 << 7);
}

// ============ func_02022d08 ============
// Adds a pin/badge to pin inventory (max 99 per pin)
// arg0: pin slot index, arg1: pin ID, arg2: new pin flag
void func_02022d08(u16 arg0, s16 arg1, s32 arg2) {
    s32 temp_r3;
    u8  temp_r2;

    temp_r3                      = arg0 * 4;
    data_020727c0[arg0].pinID    = arg1;
    data_020727c0[arg0].flags    = (data_020727c0[arg0].flags & ~0x80) | (u8)(arg2 << 7);
    temp_r2                      = data_020727c0[arg0].quantity + 1;
    data_020727c0[arg0].quantity = temp_r2;
    if ((u32)temp_r2 > 0x63U) {
        data_020727c0[arg0].quantity = 0x63;
    }
}

// ============ func_02022d60 ============
// Adds a thread (clothing) to thread inventory (max level 9)
// arg0: thread slot index, arg1: thread ID, arg2: new thread flag
void func_02022d60(u16 arg0, s16 arg1, s32 arg2) {
    u8 temp_r1;
    u8 temp_r4;

    data_02072c80[arg0].threadID = arg1;
    temp_r1                      = ((data_02072c80[arg0].flags & ~0x10) | ((u32)(arg2 << 0x1F) >> 0x1B)) & ~0x20;
    temp_r4                      = (temp_r1 & ~0xF) | (((temp_r1 & 0xF) + 1) & 0xF);
    data_02072c80[arg0].flags    = temp_r4;
    if ((u32)((u32)(temp_r4 << 0x1C) >> 0x1C) > 9U) {
        data_02072c80[arg0].flags = (temp_r4 & ~0xF) | 9;
    }
}

// ============ func_02022dd8 ============
// General function to add items/pins/threads to inventory
// arg0: item/pin/thread ID, arg1: type flags (0x1000 bit set for adding)
// Returns 1 if successfully added, 0 if failed
s32 func_02022dd8(u32 arg0, s32 arg1) {
    u8  sp4[0x34];
    s32 temp_r0;
    s32 temp_r0_2;
    s32 var_r6;
    u16 var_r0;
    u16 var_r0_2;
    u16 var_r0_3;
    u16 var_r0_4;
    u16 var_r0_5;
    u32 temp_r4;

    var_r6  = arg1;
    temp_r4 = func_020229d4((u16)arg0);
    if (arg0 >= 0x308) {
        return 0;
    }
    if (var_r6 == 1) {
        DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp4, 0x34, &data_0205c160, (s32)arg0 * 0x34));
        if (sp4[0x25] == 1) {
            var_r6 = 2;
        }
    }
    temp_r0 = var_r6 & 0xF000;
    if (temp_r0 != 0x1000) {
        return temp_r0 != 0;
    }
    temp_r0_2 = var_r6 & 0xF;
    if (temp_r4 == 0) {
        if (temp_r0_2 == 1) {
            var_r0 = 0;
            if (func_02022a18() == 0) {
                return 0;
            }
            while (1) {
                if (*(u16*)((s32)&data_02071d10 + var_r0 * 0xA + 0xB0) == 0xFFFF) {
                    func_02022c9c(var_r0, (s16)arg0, 0);
                    return 1;
                }
                var_r0 += 1;
                if (var_r0 >= 0x100) {
                    return 0;
                }
            }
        }
        if (temp_r0_2 == 2) {
            var_r0_2 = 0;
            while (1) {
                if (arg0 == *(u16*)((s32)&data_02071d10 + var_r0_2 * 4 + 0xA00 + 0xB0)) {
                    func_02022d08(var_r0_2, (s16)arg0, 0);
                    return 1;
                }
                var_r0_2 += 1;
                if (var_r0_2 >= 0x130) {
                    var_r0_3 = 0;
                    while (1) {
                        if (*(u16*)((s32)&data_02071d10 + var_r0_3 * 4 + 0xA00 + 0xB0) == 0xFFFF) {
                            func_02022d08(var_r0_3, (s16)arg0, 0);
                            return 1;
                        }
                        var_r0_3 += 1;
                        if (var_r0_3 >= 0x130) {
                            return 0;
                        }
                    }
                }
            }
        }
        return 0;
    }
    var_r0_4 = 0;
    while (1) {
        if (arg0 == *(u16*)((s32)&data_02071d10 + var_r0_4 * 4 + 0xF00 + 0x70)) {
            func_02022d60(var_r0_4, (s16)arg0, 0);
            return 1;
        }
        var_r0_4 += 1;
        if (var_r0_4 >= 0x1D8) {
            var_r0_5 = 0;
            while (1) {
                if (*(u16*)((s32)&data_02071d10 + var_r0_5 * 4 + 0xF00 + 0x70) == 0xFFFF) {
                    func_02022d60(var_r0_5, (s16)arg0, 0);
                    return 1;
                }
                var_r0_5 += 1;
                if (var_r0_5 >= 0x1D8) {
                    return 0;
                }
            }
        }
    }
}

// ============ func_02023010 ============
// Counts how many of a specific item/pin/thread the player owns
// arg0: item/pin/thread ID
// Returns total count across all inventory types
static u32 func_02023010(u32 arg0) {
    u16 var_ip;
    u16 var_r2;
    u16 var_r3;
    u16 var_r5;
    u16 var_r5_2;
    u16 var_r5_3;
    u16 var_r7;
    u16 var_r7_2;
    u32 temp_r0;

    temp_r0 = func_020229d4((u16)arg0);
    var_r3  = 0;
    if (arg0 >= 0x308) {
        return 0;
    }
    var_r2 = 0;
    if (temp_r0 == 0) {
        do {
            var_r7 = 0;
            do {
                if (arg0 == *(u16*)((s32)&data_02071d10 + var_r7 * 0xA + var_r2 * 0x3C + 0x1DC2)) {
                    var_r3 += 1;
                }
                var_r7 += 1;
            } while (var_r7 < 6);
            var_r2 += 1;
        } while (var_r2 < 4);
        var_r5 = 0;
        do {
            if (arg0 == *(u16*)((s32)&data_02071d10 + var_r5 * 0xA + 0xB0)) {
                var_r3 += 1;
            }
            var_r5 += 1;
        } while (var_r5 < 0x100);
        var_r5_2 = 0;
        while (var_r5_2 < 0x130) {
            if (arg0 == *(u16*)((s32)&data_02071d10 + var_r5_2 * 4 + 0xA00 + 0xB0)) {
                var_r3 += data_020727c0[var_r5_2].quantity;
                break;
            }
            var_r5_2 += 1;
        }
    } else {
        do {
            if (arg0 == *(u16*)((s32)&data_02071d10 + var_r2 * 2 + 0x1C)) {
                var_r3 += 1;
            }
            var_r2 += 1;
        } while (var_r2 < 4);
        var_ip = 0;
        do {
            var_r7_2 = 0;
            do {
                if (arg0 == *(u16*)((s32)&data_02071d10 + var_r7_2 * 2 + var_ip * 0x18 + 0x34)) {
                    var_r3 += 1;
                }
                var_r7_2 += 1;
            } while (var_r7_2 < 4);
            var_ip += 1;
        } while (var_ip < 3);
        var_r5_3 = 0;
        while (var_r5_3 < 0x1D8) {
            if (arg0 == *(u16*)((s32)&data_02071d10 + var_r5_3 * 4 + 0xF00 + 0x70)) {
                var_r3 += (data_02072c80[var_r5_3].flags & 0xF);
                break;
            }
            var_r5_3 += 1;
        }
    }
    return var_r3;
}

// ============ func_02023208 ============
// Checks if player owns enough of an item with specified quality/level
// arg0: item/pin/thread ID, arg1: required quantity, arg2: required quality/level
// Returns 1 if owns enough, 0 if not
s32 func_02023208(u32 arg0, u32 arg1, s32 arg2) {
    u32 sp0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    u16 var_ip;
    u16 var_r1;
    u16 var_r2;
    u16 var_r2_2;
    u16 var_r2_3;
    u16 var_r3;
    u16 var_r3_2;
    u16 var_r6;
    u16 var_r7;
    u32 temp_r0;

    sp0     = arg1;
    temp_r0 = func_020229d4((u16)arg0);
    var_ip  = 0;
    if (arg0 >= 0x308) {
        return 0;
    }
    if (temp_r0 == 0) {
        var_r2 = 0;
        do {
            temp_r0_2 = var_r2 * 0x3C;
            var_r3    = 0;
            do {
                temp_r0_3 = var_r3 * 0xA;
                if ((arg0 == *(u16*)((s32)&data_02071d10 + temp_r0_2 + temp_r0_3 + 0x1DC2)) &&
                    (arg2 <= (s32)((data_02073adb[temp_r0_2 + temp_r0_3] << 0x19) >> 0x19)))
                {
                    var_ip += 1;
                }
                var_r3 += 1;
            } while (var_r3 < 6);
            var_r2 += 1;
        } while (var_r2 < 4);
        var_r1 = 0;
        do {
            if ((arg0 == *(u16*)((s32)&data_02071d10 + var_r1 * 0xA + 0xB0)) &&
                (arg2 <= (s32)((data_02071dc8[var_r1].flags << 0x19) >> 0x19)))
            {
                var_ip += 1;
            }
            var_r1 += 1;
        } while (var_r1 < 0x100);
        var_r6 = 0;
        while (1) {
            if ((arg0 == *(u16*)((s32)&data_02071d10 + var_r6 * 4 + 0xA00 + 0xB0)) &&
                (arg2 <= (s32)((data_020727c0[var_r6].flags << 0x19) >> 0x19)))
            {
                var_ip += data_020727c0[var_r6].quantity;
                break;
            }
            var_r6 += 1;
            if (var_r6 >= 0x130) {
                break;
            }
        }
        if (var_ip >= sp0) {
            return 1;
        }
        return 0;
    }
    var_r2_2 = 0;
    do {
        if (arg0 == *(u16*)((s32)&data_02071d10 + var_r2_2 * 2 + 0x1C)) {
            var_ip += 1;
        }
        var_r2_2 += 1;
    } while (var_r2_2 < 4);
    var_r2_3 = 0;
    do {
        var_r7 = 0;
        do {
            if (arg0 == *(u16*)((s32)&data_02071d10 + var_r7 * 2 + var_r2_3 * 0x18 + 0x34)) {
                var_ip += 1;
            }
            var_r7 += 1;
        } while (var_r7 < 4);
        var_r2_3 += 1;
    } while (var_r2_3 < 3);
    var_r3_2 = 0;
    while (1) {
        if (arg0 == *(u16*)((s32)&data_02071d10 + var_r3_2 * 4 + 0xF00 + 0x70)) {
            var_ip += (data_02072c80[var_r3_2].flags & 0xF);
            break;
        }
        var_r3_2 += 1;
        if (var_r3_2 >= 0x1D8) {
            break;
        }
    }
    if (var_ip >= sp0) {
        return 1;
    }
    return 0;
}

// ============ func_02023480 ============
// Calculates price/value based on quantity (uses floating-point math)
// arg0: quantity, arg1: price table index
// Returns calculated price
s32 func_02023480(u32 arg0, s32 arg1) {
    u32  buffer[48]; // 0xc0 bytes = 192 bytes = 48 words
    u32* src;
    u32* dst;
    s32  i;
    s32  offset;
    f64  temp;
    f64  val0;
    f64  val1;
    f64  val2;
    u32  temp0, temp1, temp2, temp3;

    // Copy data using a loop to match original assembly
    src = (u32*)&data_0205c2dc;
    dst = buffer;

    for (i = 12; i != 0; i--) {
        temp0  = src[0];
        temp1  = src[1];
        temp2  = src[2];
        temp3  = src[3];
        dst[0] = temp0;
        dst[1] = temp1;
        dst[2] = temp2;
        dst[3] = temp3;
        src += 4;
        dst += 4;
    }

    if (arg0 <= 1) {
        return 0;
    }

    temp   = (f64)(arg0 - 1);
    offset = arg1 * 0x18;

    val0 = temp * *(f64*)((u8*)buffer + 0x10 + offset);
    val1 = temp * *(f64*)((u8*)buffer + 0x8 + offset);
    val2 = temp * *(f64*)((u8*)buffer + 0x0 + offset);

    return func_02054a9c(val2 + func_02053098(val0, val1));
}

// ============ func_0202353c ============
// Checks if a game progress flag is set (up to 47 flags)
// arg0: flag ID
// Returns 1 if flag is set, 0 if not
s32 func_0202353c(u32 arg0) {
    s32 var_r0;
    u16 temp_r5;
    u32 temp_r2;

    temp_r2 = arg0 >> 0x1F;
    temp_r5 = (u16)(temp_r2 + (((arg0 << 0x1C) - temp_r2) >> 0x1C));
    if (arg0 >= 0x2F) {
        OS_WaitForever();
    }
    var_r0 = 1;
    if (!(data_020737ca[(arg0 << 0xC) >> 0x10] & (1 << temp_r5))) {
        var_r0 = 0;
    }
    return var_r0;
}

// ============ func_02023588 ============
// Gets a value from game state
// Returns current value
u16 func_02023588(void) {
    return data_020736f0.unkE0;
}

// ============ func_02023598 ============
// Sets a value in game state
// arg0: value to set
void func_02023598(s16 arg0) {
    data_020736f0.unkE0 = arg0;
}

// ============ func_020235a8 ============
// Increments play time counter (max ~215M, about 60 hours)
void func_020235a8(void) {
    if (data_02072cf0.unkDB4 < 0x0CDFE600U) {
        data_02072cf0.unkDB4 += 1;
    }
}

// ============ func_020235cc ============
// Sets (ORs) flag bits for game progress (up to 41 flags)
// arg0: flag index, arg1: bit mask to set
void func_020235cc(u32 arg0, s32 arg1) {
    if (arg0 >= 0x29U) {
        OS_WaitForever();
    }
    data_02073aa8[arg0] = data_02073aa8[arg0] | (u8)arg1;
}

// ============ func_020235fc ============
// Clears (ANDs NOT) flag bits for game progress
// arg0: flag index, arg1: bit mask to clear
void func_020235fc(u32 arg0, s32 arg1) {
    if (arg0 >= 0x29U) {
        OS_WaitForever();
    }
    data_02073aa8[arg0] = data_02073aa8[arg0] & ~(u8)arg1;
}

// ============ func_02023630 ============
// Clears specific flag bits across all 41 flag bytes
// arg0: bit mask to clear from all flag bytes
void func_02023630(s32 arg0) {
    u16 var_r1;

    var_r1 = 0;
    do {
        data_02071d10.unk1D98[var_r1] &= ~arg0;
        var_r1 += 1;
    } while (var_r1 < 0x29);
}

// ============ func_0202366c ============
// Checks if specific flag bits are set
// arg0: flag index, arg1: bit mask to check
// Returns 1 if any specified bits are set, 0 if not
s32 func_0202366c(u32 arg0, s32 arg1) {
    if (arg0 >= 0x29U) {
        OS_WaitForever();
    }
    if ((u8)arg1 & data_02073aa8[arg0]) {
        return 1;
    }
    return 0;
}

// ============ func_020236a0 ============
// Checks if any flags with bits 1 or 2 are set across all flag bytes
// Returns 1 if any found, 0 if none
s32 func_020236a0(void) {
    u16 var_r2;

    var_r2 = 0;
    while (1) {
        if (data_02071d10.unk1D98[var_r2] & 6) {
            return 1;
        }
        var_r2 += 1;
        if ((u32)var_r2 >= 0x29U) {
            return 0;
        }
    }
}

// ============ func_020236e0 ============
// Calculates character stats (HP, ATK, DEF) with equipped threads
// arg0: character slot (0xFF for global threads only), arg1: stat type (0=HP, 1=ATK, 2=DEF)
// Returns calculated stat value
s16 func_020236e0(s32 arg0, s32 arg1) {
    u8  sp8[0x20];
    s32 sp4;
    s16 var_r8;
    u16 temp_r10;
    u16 temp_r7;
    u16 var_r9;
    u16 var_r9_2;
    u32 temp_r0;
    u32 temp_r0_2;

    sp4 = arg0;
    if (arg1 == 0) {
        var_r8 = (s16)data_02071cf0.unk30;
    } else if (arg1 == 1) {
        var_r8 = (s16)data_02071cf0.unk32;
    } else {
        var_r8 = (s16)data_02071cf0.unk22;
    }
    var_r9 = 0;
    do {
        temp_r7 = *(u16*)((s32)&data_02071d10 + var_r9 * 2 + 0x1C);
        temp_r0 = func_020229d4(temp_r7);
        if ((temp_r7 != 0xFFFF) && (temp_r0 == 1)) {
            DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp8, 0x18, &data_0205c170, func_0202296c(temp_r7) * 0x18));
            if (arg1 == 0) {
                var_r8 += *(s16*)(sp8 + 0xC);
            } else if (arg1 == 1) {
                var_r8 += *(s16*)(sp8 + 0xA);
            } else {
                var_r8 += *(s16*)(sp8 + 0xE);
            }
        }
        var_r9 += 1;
    } while (var_r9 < 4);
    if ((arg1 == 2) && (sp4 != 0xFF)) {
        var_r9_2 = 0;
        do {
            temp_r10  = *(u16*)((s32)&data_02071d10 + sp4 * 0x18 + var_r9_2 * 2 + 0x34);
            temp_r0_2 = func_020229d4(temp_r10);
            if ((temp_r10 != 0xFFFF) && (temp_r0_2 == 1)) {
                DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp8, 0x18, &data_0205c170, func_0202296c(temp_r10) * 0x18));
                var_r8 += *(s16*)(sp8 + 0xE);
            }
            var_r9_2 += 1;
        } while (var_r9_2 < 4);
    }
    if (var_r8 < 0) {
        var_r8 = 0;
    }
    return var_r8;
}

// ============ func_020238c0 ============
// Calculates maximum HP for a character at specified level
// arg0: character slot, arg1: character level
// Returns calculated max HP
s16 func_020238c0(s32 arg0, s32 arg1) {
    u8  sp8[0x20];
    s32 sp4;
    s16 var_r9;
    u16 temp_r10;
    u16 temp_r8;
    u16 var_r10;
    u16 var_r8;
    u32 temp_r0;
    u32 temp_r0_2;

    sp4     = arg0;
    var_r9  = (s16)(((arg1 - 1) * 0x32) + 0xC8 + (s16)data_02071cf0.unk22);
    var_r10 = 0;
    do {
        temp_r8 = *(u16*)((s32)&data_02071d10 + var_r10 * 2 + 0x1C);
        temp_r0 = func_020229d4(temp_r8);
        if ((temp_r8 != 0xFFFF) && (temp_r0 == 1)) {
            DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp8, 0x18, &data_0205c180, func_0202296c(temp_r8) * 0x18));
            var_r9 += *(s16*)(sp8 + 0xE);
        }
        var_r10 += 1;
    } while (var_r10 < 4);
    if (sp4 != 0xFF) {
        var_r8 = 0;
        do {
            temp_r10  = *(u16*)((s32)&data_02071d10 + sp4 * 0x18 + var_r8 * 2 + 0x34);
            temp_r0_2 = func_020229d4(temp_r10);
            if ((temp_r10 != 0xFFFF) && (temp_r0_2 == 1)) {
                DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp8, 0x18, &data_0205c180, func_0202296c(temp_r10) * 0x18));
                var_r9 += *(s16*)(sp8 + 0xE);
            }
            var_r8 += 1;
        } while (var_r8 < 4);
    }
    if (var_r9 < 1) {
        var_r9 = 1;
    }
    return var_r9;
}

// ============ func_02023a40 ============
// Calculates stat bonuses from equipped threads for a character
// arg0: character slot, arg1: stat type (0=HP, 1=ATK)
// Returns stat bonus value
s32 func_02023a40(s32 arg0, s32 arg1) {
    u8  sp4[0x18];
    s16 var_r7;
    u16 temp_r6;
    u16 var_r8;
    u32 temp_r0;

    if ((arg1 != 2) && (arg0 != 0xFF)) {
        return 0;
    }
    var_r7 = 0;
    if (arg1 == 0) {
        var_r7 = *(s16*)((s32)&data_02071d10 + arg0 * 0x18);
    } else if (arg1 == 1) {
        var_r7 = *(s16*)((s32)&data_02071d10 + arg0 * 0x18 + 2);
    }
    var_r8 = 0;
    do {
        temp_r6 = *(u16*)((s32)&data_02071d10 + arg0 * 0x18 + var_r8 * 2 + 0x34);
        temp_r0 = func_020229d4(temp_r6);
        if ((temp_r6 != 0xFFFF) && (temp_r0 == 1)) {
            DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp4, 0x18, &data_0205c138, func_0202296c(temp_r6) * 0x18));
            if (arg1 == 0) {
                var_r7 += *(s16*)(sp4 + 0xC);
            } else if (arg1 == 1) {
                var_r7 += *(s16*)(sp4 + 0xA);
            }
        }
        var_r8 += 1;
    } while (var_r8 < 4);
    if (var_r7 < 0) {
        var_r7 = 0;
    }
    return var_r7;
}

// ============ func_02023b7c ============
// Checks if a thread can be equipped (based on brand flags)
// arg0: thread ID
// Returns 1 if equippable, 0 if not
s32 func_02023b7c(u16 arg0) {
    u32 temp_r0;
    u32 temp_r4;

    temp_r4 = func_0202296c(arg0);
    temp_r0 = func_020229d4(arg0);
    if ((arg0 == 0xFFFF) || (temp_r0 != 1)) {
        return 0;
    }
    if (temp_r4 > 0x117) {
        return 0;
    }
    if ((data_02073bc2[temp_r4] & 1) != 0) {
        return 1;
    }
    return 0;
}

// ============ func_02023be8 ============
// Gets item/pin/thread/food price
// arg0: item ID, arg1: price type (1=buy, other=sell with quantity adjustment)
// Returns price value
s32 func_02023be8(u16 arg0, s32 arg1) {
    u8    sp38[0x34];
    u8    sp20[0x18];
    u8    spC[0x14];
    u8    sp4[8];
    Data* temp_r0_2;
    s32   var_r5;
    u32   temp_r0;
    u32   temp_r4;

    var_r5  = 0;
    temp_r4 = func_0202296c(arg0);
    temp_r0 = func_020229d4(arg0);
    switch (temp_r0) {
        default:
            OS_WaitForever();
            break;
        case 0:
            temp_r0_2 = DatMgr_LoadRawDataWithOffset(1, sp38, 0x34, &data_0205c168, temp_r4 * 0x34);
            if (arg1 == 1) {
                var_r5 = *(s32*)(sp38 + 8);
            } else {
                var_r5 = ((sp38[0x25] - 1) * *(s16*)(sp38 + 0xC)) + *(s32*)(sp38 + 8);
            }
            DatMgr_ReleaseData(temp_r0_2);
            break;
        case 1:
            var_r5 = *(s32*)(sp20 + 4);
            DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp20, 0x18, &data_0205c128, temp_r4 * 0x18));
            break;
        case 2:
            var_r5 = *(s32*)(spC + 4);
            DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, spC, 0x14, &data_0205c178, temp_r4 * 0x14));
            break;
        case 3:
            var_r5 = *(s32*)(sp4 + 4);
            DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp4, 8, &data_0205c130, temp_r4 * 8));
            break;
    }
    return var_r5;
}

// ============ func_02023d00 ============
// Sets a flag bit in flag array (bitmask 1 << arg0)
// arg0: flag bit index
void func_02023d00(s32 arg0) {
    data_02073ff0[0x22] |= (1 << arg0);
}

// ============ func_02023d1c ============
// Checks if a flag bit is set
// arg0: flag bit index
// Returns 1 if set, 0 if not
s32 func_02023d1c(s32 arg0) {
    s32 var_r2;

    var_r2 = 1;
    if (!(data_02073ff0[0x22] & (1 << arg0))) {
        var_r2 = 0;
    }
    return var_r2;
}

// ============ func_02023d3c ============
// Sets a flag bit in extended flag array (up to 256 flags)
// arg0: flag bit index
void func_02023d3c(u32 arg0) {
    u16 temp_r5;
    u32 temp_r2;
    u32 temp_r4;

    temp_r2 = arg0 >> 0x1F;
    temp_r4 = (arg0 << 0xC) >> 0x10;
    temp_r5 = (u16)(temp_r2 + (((arg0 << 0x1C) - temp_r2) >> 0x1C));
    if (arg0 >= 0x100) {
        OS_WaitForever();
    }
    data_02074036[temp_r4] |= (1 << temp_r5);
}

// ============ func_02023d88 ============
// Checks if a flag bit is set in extended flag array (up to 256 flags)
// arg0: flag bit index
// Returns 1 if set, 0 if not
s32 func_02023d88(u32 arg0) {
    s32 var_r0;
    u16 temp_r5;
    u32 temp_r2;

    temp_r2 = arg0 >> 0x1F;
    temp_r5 = (u16)(temp_r2 + (((arg0 << 0x1C) - temp_r2) >> 0x1C));
    if (arg0 >= 0x100) {
        OS_WaitForever();
    }
    var_r0 = 1;
    if (!(data_02074036[(arg0 << 0xC) >> 0x10] & (1 << temp_r5))) {
        var_r0 = 0;
    }
    return var_r0;
}

// ============ func_02023dd4 ============
// Checks if equipment slots are empty
// arg0: character slot index (0-2)
// Returns 1 if all slots empty, 0 if any occupied
s32 func_02023dd4(u32 arg0) {
    u16 var_ip;
    u16 var_r3;

    var_ip = 0;
    while (1) {
        if (*(u16*)((s32)&data_02071d10 + var_ip * 2 + 0x1C) != 0xFFFF) {
            return 0;
        }
        var_ip += 1;
        if (var_ip >= 4) {
            if (arg0 < 3) {
                var_r3 = 0;
                while (1) {
                    if (*(u16*)((s32)&data_02071d10 + arg0 * 0x18 + var_r3 * 2 + 0x34) != 0xFFFF) {
                        return 0;
                    }
                    var_r3 += 1;
                    if (var_r3 >= 4) {
                        return 1;
                    }
                }
            }
            return 1;
        }
    }
}

// ============ func_02023e58 ============
// Checks if all equipped threads match a specific brand
// arg0: brand ID to check (up to 13 brands)
// Returns 1 if all match, 0 if not
s32 func_02023e58(u32 arg0) {
    u8  sp8[0x18];
    u8  sp7;
    u8  sp6;
    u8  sp5;
    u8  sp4;
    u16 temp_r9;
    u16 var_r2;
    u16 var_r8;
    u32 temp_r0;
    u32 temp_r5;

    sp4 = *(u8*)((s32)&data_0205c120 + 4);
    sp5 = *(u8*)((s32)&data_0205c120 + 5);
    sp6 = *(u8*)((s32)&data_0205c120 + 6);
    sp7 = *(u8*)((s32)&data_0205c120 + 7);
    if (arg0 >= 0xD) {
        return 0;
    }
    var_r8 = 0;
    do {
        temp_r9 = *(u16*)((s32)&data_02071d10 + var_r8 * 2 + 0x1C);
        temp_r5 = func_0202296c(temp_r9);
        temp_r0 = func_020229d4(temp_r9);
        if ((temp_r9 != 0xFFFF) && (temp_r0 == 1)) {
            DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp8, 0x18, &data_0205c140, temp_r5 * 0x18));
            *(&sp4 + var_r8) = sp8[2];
        }
        var_r8 += 1;
    } while (var_r8 < 4);
    var_r2 = 0;
    while (1) {
        if (*(&sp4 + var_r2) != arg0) {
            return 0;
        }
        var_r2 += 1;
        if (var_r2 >= 4) {
            return 1;
        }
    }
}

// ============ func_02023f60 ============
// Checks if all equipped threads on a character match a specific brand
// arg0: character slot (0-2), arg1: brand ID to check
// Returns 1 if all match, 0 if not
s32 func_02023f60(u32 arg0, u32 arg1) {
    u8  sp8[0x18];
    u8  sp7;
    u8  sp6;
    u8  sp5;
    u8  sp4;
    u16 temp_r8;
    u16 var_r2;
    u16 var_r7;
    u32 temp_r0;
    u32 temp_r5;

    sp4 = *(u8*)((s32)&data_0205c120 + 0);
    sp5 = *(u8*)((s32)&data_0205c120 + 1);
    sp6 = *(u8*)((s32)&data_0205c120 + 2);
    sp7 = *(u8*)((s32)&data_0205c120 + 3);
    if (arg1 >= 0xD) {
        return 0;
    }
    if (arg0 >= 3) {
        return 0;
    }
    var_r7 = 0;
    do {
        temp_r8 = *(u16*)((s32)&data_02071d10 + arg0 * 0x18 + var_r7 * 2 + 0x34);
        temp_r5 = func_0202296c(temp_r8);
        temp_r0 = func_020229d4(temp_r8);
        if ((temp_r8 != 0xFFFF) && (temp_r0 == 1)) {
            DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, sp8, 0x18, &data_0205c148, temp_r5 * 0x18));
            *(&sp4 + var_r7) = sp8[2];
        }
        var_r7 += 1;
    } while (var_r7 < 4);
    var_r2 = 0;
    while (1) {
        if (*(&sp4 + var_r2) != arg1) {
            return 0;
        }
        var_r2 += 1;
        if (var_r2 >= 4) {
            return 1;
        }
    }
}

// ============ func_02024080 ============
// Counts active battle records/achievements (up to 96 entries)
// Returns count of active records
u32 func_02024080(void) {
    s32 temp_gt;
    u16 var_r0;
    u16 var_r3;
    u32 temp_r1;

    var_r0 = 0;
    var_r3 = 0;
    do {
        temp_gt = *(s16*)((s32)&data_02071d10 + var_r3 * 4 + 0x1F00 + 0xCC) > 0;
        var_r3 += 1;
        if (temp_gt) {
            var_r0 += 1;
        }
    } while (var_r3 < 0x60);
    temp_r1 = data_020736f0.unkC4 << 0x19;
    if (var_r0 <= (temp_r1 >> 0x19)) {
        var_r0 = (u16)(temp_r1 >> 0x19);
    }
    return var_r0;
}

// ============ func_020240e0 ============
// Checks if a specific badge is owned (in equipped slots or badge inventory)
// arg0: badge ID, arg1: badge data pointer
// Returns 1 if owned, 0 if not
s32 func_020240e0(u16 arg0, s32 arg1) {
    s32 temp_r2;
    s32 temp_r2_2;
    u16 var_lr;
    u16 var_r3;
    u16 var_r4;

    var_lr = 0;
    while (1) {
        temp_r2 = var_lr * 0x3C;
        var_r4  = 0;
        while (1) {
            temp_r2_2 = var_r4 * 0xA;
            if ((arg0 == *(u16*)((s32)&data_02071d10 + temp_r2 + temp_r2_2 + 0x1DC2)) &&
                ((*(u8*)((s32)&data_02073adb + temp_r2 + temp_r2_2) << 0x19) >> 0x19) ==
                    *(u8*)((s32)arg1 + arg0 * 0x34 + 0x25))
            {
                return 1;
            }
            var_r4 += 1;
            if (var_r4 >= 6) {
                var_lr += 1;
                if (var_lr >= 4) {
                    var_r3 = 0;
                    while (1) {
                        if (arg0 == *(u16*)((s32)&data_02071d10 + var_r3 * 4 + 0xA00 + 0xB0)) {
                            return 1;
                        }
                        var_r3 += 1;
                        if (var_r3 >= 0x130) {
                            return 0;
                        }
                    }
                }
                break;
            }
        }
    }
}

// ============ func_020241b0 ============
// Counts total unique badges collected (max 304)
// Loads badge data and checks against player inventory
// Returns total badge count
u16 func_020241b0(void) {
    u16   var_r4;
    u16   var_r5;
    void* temp_r0;

    var_r5  = 0;
    temp_r0 = Mem_AllocHeapTail(&gDebugHeap, 0x3DC0);
    Mem_SetSequence(&gDebugHeap, temp_r0, data_02065bd8);
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, temp_r0, 0x3DC0, &data_0205c150));
    var_r4 = 0;
    do {
        if (func_020240e0(var_r4, (s32)temp_r0) == 1) {
            var_r5 += 1;
        }
        var_r4 += 1;
    } while (var_r4 < 0x130);
    Mem_Free(&gDebugHeap, temp_r0);
    return var_r5;
}

// ============ func_02024244 ============
// Counts total threads collected (max 472)
// Returns thread count
u32 func_02024244(void) {
    s32 temp_z;
    u16 var_ip;
    u16 var_r0;
    u32 temp_r1;

    var_r0 = 0;
    var_ip = 0;
    do {
        temp_z = *(u16*)((s32)&data_02071d10 + var_ip * 4 + 0xF00 + 0x70) == 0xFFFF;
        var_ip += 1;
        if (!temp_z) {
            var_r0 += 1;
        }
    } while (var_ip < 0x1D8);
    temp_r1 = data_020736f0.unkC6 << 0x17;
    if (var_r0 <= (temp_r1 >> 0x17)) {
        var_r0 = (u16)(temp_r1 >> 0x17);
    }
    return var_r0;
}

// ============ func_020242ac ============
// Gets a value from data table by index
// arg0: table index
// Returns value from data_0205c1e4 table
u16 func_020242ac(s32 arg0) {
    u16 sp0[0x16];
    s32 var_r3;

    for (var_r3 = 0; var_r3 < 0x16; var_r3++) {
        sp0[var_r3] = *(u16*)((s32)&data_0205c1e4 + var_r3 * 2);
    }
    return sp0[arg0];
}

// ============ func_020242f8 ============
// Checks if collection requirements are met for all categories
// Verifies completion status across 22 collection types
// Returns 1 if all complete, 0 if not
s32 func_020242f8(void) {
    u16 sp0[0x16];
    s32 var_lr;
    u16 temp_r0;
    u16 temp_r0_2;
    u16 temp_r1;
    u16 temp_r5;
    u16 var_r4;

    for (var_lr = 0; var_lr < 0x16; var_lr++) {
        sp0[var_lr] = *(u16*)((s32)&data_0205c210 + var_lr * 2);
    }
    var_lr = 0;
    while (1) {
        temp_r5 = sp0[var_lr];
        var_r4  = 0;
        if (temp_r5 > 0) {
            while (1) {
                if (*(u8*)((s32)&data_02071d10 + var_lr * 4 + var_r4 + 0x2000 + 0x2CC) == 0) {
                    return 0;
                }
                temp_r0_2 = var_r4 + 1;
                if (temp_r5 <= temp_r0_2) {
                    break;
                }
                var_r4 = temp_r0_2;
            }
        }
        var_lr += 1;
        if (var_lr >= 0x16) {
            return 1;
        }
    }
}

// ============ func_020243ac ============
// Adds to a counter value (max 0xFFFF)
// arg0: value to add
void func_020243ac(s32 arg0) {
    u32 temp_r0;

    temp_r0              = data_02072cf0.unkAC8 + arg0;
    data_02072cf0.unkAC8 = temp_r0;
    if (temp_r0 > 0xFFFF) {
        data_02072cf0.unkAC8 = 0xFFFF;
    }
}

// ============ func_020243d4 ============
// Gets a value from data table by index (range 0x16-0x22)
// arg0: table index
// Returns value from data_0205c270 table
s32 func_020243d4(u32 arg0) {
    s32 sp0[13];
    s32 var_r3;

    for (var_r3 = 0; var_r3 < 13; var_r3++) {
        sp0[var_r3] = *(s32*)((s32)&data_0205c270 + var_r3 * 4);
    }
    if ((arg0 < 0x16) || (arg0 > 0x22)) {
        OS_WaitForever();
    }
    return sp0[arg0 - 0x16];
}

// ============ func_02024434 ============
// Gets a value based on item ID (specific items have special values)
// arg0: item ID
// Returns special value (10, 30, 50, 70, 100) or 0
s32 func_02024434(s32 arg0) {
    s32 var_r2;

    var_r2 = 0;
    switch (arg0) {
        case 0xE2:
            var_r2 = 0x64;
            break;
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x93:
        case 0x110:
        case 0xB0:
            var_r2 = 0x1E;
            break;
        case 0xAF:
        case 0x7F:
        case 0x64:
        case 0x10B:
            var_r2 = 0x32;
            break;
        case 0x9:
        case 0x35:
        case 0x65:
            var_r2 = 0x46;
            break;
        case 0x85:
            var_r2 = 0xA;
            break;
        default:
            var_r2 = 0;
            break;
    }
    return var_r2;
}

// ============ func_02024558 ============
// Clears specific high-value thread slots (IDs 0x2B2, 0x2B3, 0x2BA, 0x2BB)
// Sets IDs to 0xFFFF and resets flags
void func_02024558(void) {
    u16 temp_ip;
    u16 var_lr;

    var_lr = 0;
    while (1) {
        temp_ip = var_lr + 0x130;
        if ((temp_ip != 0x2B2) && (temp_ip != 0x2B3) && (temp_ip != 0x2BA) && (temp_ip != 0x2BB)) {
            return;
        }
        *(u16*)((s32)&data_02071d10 + var_lr * 4 + 0xF00 + 0x70) = 0xFFFF;
        data_02072c80[var_lr].flags                              = ((data_02072c80[var_lr].flags & ~0xF) | 0x10) & ~0x20;
        var_lr += 1;
        if (var_lr >= 0x1D8) {
            return;
        }
    }
}

// ============ func_0202462c ============
// Initializes special item IDs (0x109-0x10E) in system state
void func_0202462c(void) {
    data_02071cf0.unk94 = 0x109;
    data_02071cf0.unk9C = 0;
    data_02071cf0.unk9E = 0x10A;
    data_02071cf0.unkA6 = 0;
    data_02071cf0.unkA8 = 0x10B;
    data_02071cf0.unkB0 = 0;
    data_02071cf0.unkB2 = 0x10C;
    data_02071cf0.unkBA = 0;
    data_02071cf0.unkBC = 0x10D;
    data_02071cf0.unkC4 = 1;
    data_02071cf0.unkC6 = 0x10E;
    data_02071cf0.unkCE = 1;
}

// ============ func_0202468c ============
// Checks if equipment slots are empty (6 slots)
// Returns 1 if all empty, 0 if any occupied
s32 func_0202468c(void) {
    u16 var_ip;

    var_ip = 0;
    while (1) {
        if (*(u16*)((s32)&data_02071d10 + var_ip * 0xA + 0x74) != 0xFFFF) {
            return 0;
        }
        var_ip += 1;
        if (var_ip >= 6) {
            return 1;
        }
    }
}

// ============ func_020246d4 ============
// Allocates temporary backup data buffer on main heap
// arg0: buffer size
// Returns pointer to allocated buffer
u16* func_020246d4(s32 arg0) {
    u16* temp_r0;

    temp_r0 = Mem_AllocHeapTail(&gMainHeap, (arg0 + 7) & ~7);
    Mem_SetSequence(&gMainHeap, temp_r0, data_02065be4);
    return temp_r0;
}

// ============ func_0202470c ============
// Frees temporary backup data buffer
// arg0: pointer to buffer to free
void func_0202470c(u16* arg0) {
    Mem_Free(&gMainHeap, arg0);
}

// ============ func_02024724 ============
// Reads and verifies save data from a specific slot
// arg0: slot offset (0 or 0x5470)
// Returns: 0=success, 1=checksum fail, 8=data corrupt, other=error
s32 func_02024724(s32 arg0) {
    s32  var_r5;
    u16* temp_r0;

    temp_r0 = func_020246d4(0x3444);
    func_0204285c(arg0, temp_r0, 0x3444, 0, 1, 6, 1, 0, 0);
    func_02042aa4();
    func_02042330();
    if (func_020258bc(temp_r0 + 0x3420 / 2, &data_0205c96c, 0x1B, 0x3420) != 0) {
        if (func_0202593c(temp_r0, 0x3444) != 0) {
            var_r5 = 8;
        } else {
            var_r5              = 0;
            data_020736f0.unkD6 = temp_r0[0x1AB6 / 2];
        }
    } else {
        var_r5 = 1;
    }
    Mem_Free(&gMainHeap, temp_r0);
    return var_r5;
}

// ============ func_020247f4 ============
// Validates primary save data (both slots 0 and 0x5470)
// Returns: 0=valid, 1=no save, 2=error, 8=data corrupt
s32 func_020247f4(void) {
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 var_r4;

    temp_r0              = OS_GetLockID();
    data_02071cf0.lockID = temp_r0;
    if (temp_r0 == -3) {
        OS_WaitForever();
    }
    func_0204237c((u16)data_02071cf0.lockID);
    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        var_r4 = 2;
        func_0204238c((u16)data_02071cf0.lockID);
        func_020389c0((u16)data_02071cf0.lockID);
        return var_r4;
    }
    if (func_02042944() == 1) {
        temp_r0_2 = func_02024724(0);
        if (temp_r0_2 != 0) {
            temp_r0_3 = func_02024724(0x5470);
            if (temp_r0_3 != 0) {
                if ((temp_r0_2 == 1) && (temp_r0_3 == 1)) {
                    var_r4 = 1;
                } else if ((temp_r0_2 != 2) && (temp_r0_3 != 2)) {
                    var_r4 = 8;
                } else {
                    var_r4 = 2;
                }
            } else {
                var_r4 = 0;
            }
        } else {
            var_r4 = 0;
        }
    } else {
        var_r4 = 2;
    }
    func_0204238c((u16)data_02071cf0.lockID);
    func_020389c0((u16)data_02071cf0.lockID);
    return var_r4;
}

// ============ func_020248e4 ============
// Reads and verifies backup save data from a specific slot
// arg0: slot offset (0x3444 or 0x88B4)
// Returns: 0=success, 1=checksum fail, 8=data corrupt
s32 func_020248e4(s32 arg0) {
    s32  var_r5;
    u16* temp_r0;

    temp_r0 = func_020246d4(0x202C);
    func_0204285c(arg0, temp_r0, 0x202C, 0, 1, 6, 1, 0, 0);
    func_02042aa4();
    func_02042330();
    if (func_020258bc(temp_r0 + 0x2008 / 2, &data_0205c96c, 0x1B, 0x2008) != 0) {
        if (func_0202593c(temp_r0, 0x202C) == 0) {
            var_r5 = 0;
        } else {
            var_r5 = 8;
        }
    } else {
        var_r5 = 1;
    }
    Mem_Free(&gMainHeap, temp_r0);
    return var_r5;
}

// ============ func_0202499c ============
// Validates backup save data (both slots 0x3444 and 0x88B4)
// Returns: 0=valid, 1=no save, 2=error, 8=data corrupt
s32 func_0202499c(void) {
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 var_r4;

    temp_r0              = OS_GetLockID();
    data_02071cf0.lockID = temp_r0;
    if (temp_r0 == -3) {
        OS_WaitForever();
    }
    func_0204237c((u16)data_02071cf0.lockID);
    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        var_r4 = 2;
        func_0204238c((u16)data_02071cf0.lockID);
        func_020389c0((u16)data_02071cf0.lockID);
        return var_r4;
    }
    if (func_02042944() == 1) {
        temp_r0_2 = func_020248e4(0x3444);
        if (temp_r0_2 != 0) {
            temp_r0_3 = func_020248e4(0x88B4);
            if (temp_r0_3 != 0) {
                if ((temp_r0_2 == 1) && (temp_r0_3 == 1)) {
                    var_r4 = 1;
                } else if ((temp_r0_2 != 2) && (temp_r0_3 != 2)) {
                    var_r4 = 8;
                } else {
                    var_r4 = 2;
                }
            } else if (!(data_020736f0.unkD6 & 1)) {
                var_r4 = 8;
            } else {
                var_r4 = 0;
            }
        } else {
            var_r4 = 0;
        }
    } else {
        var_r4 = 2;
    }
    func_0204238c((u16)data_02071cf0.lockID);
    func_020389c0((u16)data_02071cf0.lockID);
    return var_r4;
}

// ============ func_02024aa4 ============
// Main save data validation function (checks both primary and backup)
// Temporarily disables system flags during validation
// Returns combined result of primary and backup validation
s32 func_02024aa4(void) {
    s32 temp_r0;
    s32 temp_r4;

    SystemStatusFlags &= ~0x20;
    SystemStatusFlags &= ~0x80;
    temp_r4 = func_020247f4();
    temp_r0 = temp_r4 | func_0202499c();
    SystemStatusFlags |= 0x20;
    SystemStatusFlags |= 0x80;
    return temp_r0;
}

// ============ func_02024b08 ============
// Initializes global friend data structure (50 friend slots)
// arg0: pointer to friend data buffer
// Returns 0
s32 func_02024b08(s32 arg0) {
    s32 temp_r5;
    s32 temp_r8;
    u16 var_r2;
    u16 var_r4;
    u16 var_r7;
    u16 var_r8;
    u16 var_r8_2;
    u16 var_r9;

    var_r4 = 0;
    do {
        temp_r5 = (var_r4 * 0x98) + arg0;
        var_r7  = 0;
        do {
            *(u8*)(temp_r5 + var_r7) = 0xFF;
            var_r7 += 1;
        } while (var_r7 < 6);
        temp_r8                 = (var_r4 * 0x98) + arg0;
        var_r9                  = 0;
        *(u16*)(temp_r8 + 0x6A) = 0xFFFF;
        do {
            *(u16*)(temp_r8 + (var_r9 * 2) + 0x6C) = 0xFFFF;
            var_r9 += 1;
        } while (var_r9 < 6);
        var_r8 = 0;
        do {
            *(u16*)(temp_r5 + (var_r8 * 2) + 0x78) = 0xFFFF;
            var_r8 += 1;
        } while (var_r8 < 0x10);
        var_r4 += 1;
    } while (var_r4 < 0x32);
    var_r8_2 = 0;
    do {
        var_r2 = 0;
        do {
            *(u8*)((var_r8_2 * 0xC) + arg0 + var_r2 + 0x1DB0) = 0xFF;
            var_r2 += 1;
        } while (var_r2 < 6);
        var_r8_2 += 1;
    } while (var_r8_2 < 0x32);
    return 0;
}

// ============ func_02024c00 ============
// Marks save operation as complete, clears bit 0 of save flag
void func_02024c00(void) {
    func_02025b1c();
    data_020736f0.unkD6 = (u16)(data_020736f0.unkD6 & ~1);
}

// ============ func_02024c20 ============
// Allocates global friend data buffer (0x2008 bytes) on main heap
void func_02024c20(void) {
    void* temp_r0;

    temp_r0 = Mem_AllocHeapTail(&gMainHeap, 0x2008);
    Mem_SetSequence(&gMainHeap, temp_r0, data_02065bf4);
    MI_CpuFill(0, temp_r0, Mem_GetBlockSize(&gMainHeap, temp_r0), 0, 0, 0);
    data_02071cf0.globalFriendData = (s32)temp_r0;
    func_02024b08((s32)temp_r0);
}

// ============ func_02024c84 ============
// Frees global friend data buffer
void func_02024c84(void) {
    Mem_Free(&gMainHeap, (void*)data_02071cf0.globalFriendData);
}

// ============ func_02024ca4 ============
// Save state machine step: Initialize friend data and advance state
// Returns 0
s32 func_02024ca4(void) {
    func_02024c00();
    func_02024c20();
    data_02071cf0.state += 1;
    return 0;
}

// ============ func_02024ccc ============
// Save state machine step: Complete save and re-enable system flags
// Returns 1
s32 func_02024ccc(void) {
    func_02024c84();
    SystemStatusFlags |= 0x20;
    SystemStatusFlags |= 0x80;
    return 1;
}

// ============ func_02024d04 ============
// Resets save state machine to initial state, disables system flags
void func_02024d04(void) {
    data_02071cf0.state    = 0;
    data_02071cf0.subState = 0;
    data_02071cf0.field_04 = 0;
    SystemStatusFlags &= ~0x20;
    SystemStatusFlags &= ~0x80;
}

// ============ func_02024d48 ============
// Prepares primary save data buffer (0x3444 bytes) from game state
// Copies game data to backup slot 1 and adds checksums
// Returns 0
s32 func_02024d48(void) {
    s32*  var_r4;
    s32   temp_lr;
    s32   temp_r0_2;
    s32   temp_r1;
    s32   temp_r2_2;
    s32   temp_r3_2;
    s32   var_lr;
    u16   var_r1;
    u16   var_r5;
    u32   temp_r4;
    u8    temp_r3;
    void* temp_r0;
    void* temp_r2;
    void* var_ip;

    MI_CpuSet((void*)data_02071cf0.backupData1, 0, 0x3444);
    var_ip = (void*)data_02071cf0.backupData1;
    var_r1 = 0;
    do {
        temp_r3 = *(u8*)((s32)&data_0205c96c + var_r1);
        temp_r0 = var_ip + var_r1 + 0x3420;
        var_r1 += 1;
        *(u8*)temp_r0 = temp_r3;
    } while (var_r1 < 0x1B);
    var_r5 = 0;
    do {
        temp_lr = var_r5 * 4;
        temp_r4 = 0x3420 & (0xF << temp_lr);
        temp_r2 = var_ip + var_r5 + 0x343B;
        var_r5 += 1;
        *(u8*)temp_r2 = (s8)(temp_r4 >> temp_lr);
    } while (var_r5 < 4);
    var_r4 = (s32*)&data_02071d10;
    var_lr = 0x342;
    do {
        temp_r0_2 = var_r4[0];
        temp_r1   = var_r4[1];
        temp_r2_2 = var_r4[2];
        temp_r3_2 = var_r4[3];
        var_r4 += 4;
        *(s32*)((s32)var_ip + 0)  = temp_r0_2;
        *(s32*)((s32)var_ip + 4)  = temp_r1;
        *(s32*)((s32)var_ip + 8)  = temp_r2_2;
        *(s32*)((s32)var_ip + 12) = temp_r3_2;
        var_ip                    = (void*)((s32)var_ip + 0x10);
        var_lr -= 1;
    } while (var_lr != 0);
    return 0;
}

// ============ func_02024e04 ============
// Prepares backup save data buffer (0x202C bytes) from friend data
// arg0: pointer to friend data
// Copies data to backup slot 2 and adds checksums
// Returns 0
s32 func_02024e04(void* arg0) {
    s32   temp_r0_2;
    s32   temp_r1;
    s32   temp_r2_2;
    s32   temp_r3_2;
    s32   temp_r5;
    s32   var_lr;
    u16   var_r1;
    u16   var_r6;
    u32   temp_lr;
    u8    temp_r3;
    void* temp_r0;
    void* temp_r2;
    void* var_ip;
    void* var_r4;

    var_r4 = arg0;
    MI_CpuSet((void*)data_02071cf0.backupData2, 0, 0x202C);
    var_ip = (void*)data_02071cf0.backupData2;
    var_r1 = 0;
    do {
        temp_r3 = *(u8*)((s32)&data_0205c96c + var_r1);
        temp_r0 = var_ip + var_r1 + 0x2008;
        var_r1 += 1;
        *(u8*)temp_r0 = temp_r3;
    } while (var_r1 < 0x1B);
    var_r6 = 0;
    do {
        temp_r5 = var_r6 * 4;
        temp_lr = 0x2008 & (0xF << temp_r5);
        temp_r2 = var_ip + var_r6 + 0x2023;
        var_r6 += 1;
        *(u8*)temp_r2 = (s8)(temp_lr >> temp_r5);
    } while (var_r6 < 4);
    var_lr = 0x200;
    do {
        temp_r0_2                 = *(s32*)((s32)var_r4 + 0);
        temp_r1                   = *(s32*)((s32)var_r4 + 4);
        temp_r2_2                 = *(s32*)((s32)var_r4 + 8);
        temp_r3_2                 = *(s32*)((s32)var_r4 + 12);
        var_r4                    = (void*)((s32)var_r4 + 0x10);
        *(s32*)((s32)var_ip + 0)  = temp_r0_2;
        *(s32*)((s32)var_ip + 4)  = temp_r1;
        *(s32*)((s32)var_ip + 8)  = temp_r2_2;
        *(s32*)((s32)var_ip + 12) = temp_r3_2;
        var_ip                    = (void*)((s32)var_ip + 0x10);
        var_lr -= 1;
    } while (var_lr != 0);
    *(s32*)((s32)var_ip + 0) = *(s32*)((s32)var_r4 + 0);
    *(s32*)((s32)var_ip + 4) = *(s32*)((s32)var_r4 + 4);
    return 0;
}

// ============ func_02024ec0 ============
// Cleanup after save operation: unlock cartridge and free buffer
// arg0: pointer to buffer to free
void func_02024ec0(u16* arg0) {
    func_0204238c((u16)data_02071cf0.lockID);
    func_020389c0((u16)data_02071cf0.lockID);
    func_0202470c(arg0);
}

// ============ func_02024f00 ============
// Begin primary save operation: allocate buffer, prepare data, lock cartridge
// Returns 0 on success, 1 on error
s32 func_02024f00(void) {
    s32  temp_r0_2;
    u16* temp_r0;

    temp_r0                   = func_020246d4(0x3444);
    data_02071cf0.backupData1 = (s32)temp_r0;
    func_02024d48();
    *(u16*)((s32)temp_r0 + 0x3440) = 0;
    *(u16*)((s32)temp_r0 + 0x3440) = func_0202593c(temp_r0, 0x3444);
    temp_r0_2                      = OS_GetLockID();
    data_02071cf0.lockID           = temp_r0_2;
    if (temp_r0_2 == -3) {
        OS_WaitForever();
    }
    func_0204237c((u16)data_02071cf0.lockID);
    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        data_02071cf0.subState = 4;
        func_02024ec0(temp_r0);
        return 1;
    }
    data_02071cf0.state += 1;
    return 0;
}

// ============ func_02024fc0 ============
// Save state: Start writing to slot 0, records start time
// Returns 0
s32 func_02024fc0(void) {
    s32 temp_r1;
    s32 temp_r4;
    s64 temp_ret;

    temp_r4                = data_02071cf0.backupData1;
    data_02071cf0.field_04 = 0;
    if (func_02042944() == 1) {
        temp_ret               = func_0203a444();
        temp_r1                = (s32)(temp_ret >> 32);
        data_02071cf0.field_18 = (s32)temp_ret;
        data_02071cf0.field_1C = temp_r1;
        func_0204285c(temp_r4, NULL, 0x3444, 0, 0, 1, 8, 0xA, 2);
    } else {
        OS_WaitForever();
    }
    data_02071cf0.state += 1;
    return 0;
}

// ============ func_02025058 ============
// Save state: Start writing to slot 0x5470
// Returns 0
s32 func_02025058(void) {
    s32 temp_r4;

    temp_r4                = data_02071cf0.backupData1;
    data_02071cf0.field_04 = 0;
    if (func_02042944() == 1) {
        func_0204285c(temp_r4, (u16*)0x5470, 0x3444, 0, 1, 8, 0xA, 2, 0);
    } else {
        OS_WaitForever();
    }
    data_02071cf0.state += 1;
    return 0;
}

// ============ func_020250e8 ============
// Save state: Wait for save operation to complete
// Checks for errors and advances state when done
// Returns 0
s32 func_020250e8(void) {
    u16 var_r1;

    if (func_02042ab0() != 0) {
        if (func_02042330() == 0) {
            var_r1 = 0;
        } else {
            var_r1 = data_02071cf0.subState | 4;
        }
        data_02071cf0.subState = var_r1;
        data_02071cf0.state += 1;
    }
    return 0;
}

// ============ func_02025138 ============
// Finish primary save: cleanup, calculate elapsed time
// Returns 0
s32 func_02025138(void) {
    s32 temp_r1;
    s64 temp_ret;
    u32 temp_r0;

    func_02024ec0((u16*)data_02071cf0.backupData1);
    temp_ret               = func_0203a444();
    temp_r1                = (s32)(temp_ret >> 32);
    temp_r0                = (s32)temp_ret - data_02071cf0.field_18;
    data_02071cf0.field_18 = temp_r0;
    data_02071cf0.field_1C = (s32)((temp_r1 - data_02071cf0.field_1C) + ((s64)temp_r0 < 0));
    data_02071cf0.state += 1;
    return 0;
}

// ============ func_02025180 ============
// Finish backup save: cleanup, calculate elapsed time
// Returns 0
s32 func_02025180(void) {
    s32 temp_r1;
    s64 temp_ret;
    u32 temp_r0;

    func_02024ec0((u16*)data_02071cf0.backupData2);
    temp_ret               = func_0203a444();
    temp_r1                = (s32)(temp_ret >> 32);
    temp_r0                = (s32)temp_ret - data_02071cf0.field_18;
    data_02071cf0.field_18 = temp_r0;
    data_02071cf0.field_1C = (s32)((temp_r1 - data_02071cf0.field_1C) + ((s64)temp_r0 < 0));
    data_02071cf0.state += 1;
    return 0;
}

// ============ func_020251c8 ============
// Sets save operation substate
// arg0: substate value
void func_020251c8(s32 arg0) {
    data_02071cf0.subState = arg0;
}

// ============ func_020251d8 ============
// Begin backup save operation: allocate buffer, prepare data, lock cartridge
// Returns 0 on success, 1 on error
s32 func_020251d8(void) {
    s32  temp_r0_2;
    u16* temp_r0;

    temp_r0                   = func_020246d4(0x202C);
    data_02071cf0.backupData2 = (s32)temp_r0;
    func_02024e04((void*)data_02071cf0.subState);
    *(u16*)((s32)temp_r0 + 0x2028) = 0;
    *(u16*)((s32)temp_r0 + 0x2028) = func_0202593c(temp_r0, 0x202C);
    temp_r0_2                      = OS_GetLockID();
    data_02071cf0.lockID           = temp_r0_2;
    if (temp_r0_2 == -3) {
        OS_WaitForever();
    }
    func_0204237c((u16)data_02071cf0.lockID);
    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        data_02071cf0.subState = 4;
        func_02024ec0(temp_r0);
        return 1;
    }
    data_02071cf0.state += 1;
    return 0;
}

// ============ func_0202529c ============
// Save state: Start writing backup data to slot 0x3444, records start time
// Returns 0
s32 func_0202529c(void) {
    s32 temp_r1;
    s32 temp_r4;
    s64 temp_ret;

    temp_r4                = data_02071cf0.backupData2;
    data_02071cf0.field_04 = 0;
    if (func_02042944() == 1) {
        temp_ret               = func_0203a444();
        temp_r1                = (s32)(temp_ret >> 32);
        data_02071cf0.field_18 = (s32)temp_ret;
        data_02071cf0.field_1C = temp_r1;
        func_0204285c(temp_r4, (u16*)0x3444, 0x202C, 0, 1, 8, 0xA, 2, 0);
    } else {
        OS_WaitForever();
    }
    data_02071cf0.state += 1;
    return 0;
}

// ============ func_0202533c ============
// Save state: Start writing backup data to slot 0x88B4
// Returns 0
s32 func_0202533c(void) {
    s32 temp_r4;

    temp_r4                = data_02071cf0.backupData2;
    data_02071cf0.field_04 = 0;
    if (func_02042944() == 1) {
        func_0204285c(temp_r4, (u16*)0x88B4, 0x202C, 0, 1, 8, 0xA, 2, 0);
    } else {
        OS_WaitForever();
    }
    data_02071cf0.state += 1;
    return 0;
}

// ============ func_020253cc ============
// Save state: Wait for backup save operation to complete
// Checks for errors and advances state when done
// Returns 0
s32 func_020253cc(void) {
    if (func_02042ab0() != 0) {
        if (func_02042330() != 0) {
            data_02071cf0.subState = 4;
        } else {
            data_02071cf0.subState = 0;
        }
        data_02071cf0.state += 1;
    }
    return 0;
}

// ============ func_02025414 ============
// Complete save operation and re-enable system flags
// Returns 1 (signals completion)
s32 func_02025414(void) {
    SystemStatusFlags |= 0x20;
    SystemStatusFlags |= 0x80;
    return 1;
}

// ============ func_02025444 ============
// Executes save state machine (14 state functions)
// Dispatches to current state handler function
void func_02025444(void) {
    s32 sp0[14];
    s32 var_r3;

    for (var_r3 = 0; var_r3 < 14; var_r3++) {
        sp0[var_r3] = *(s32*)((s32)&data_0205c2a4 + var_r3 * 4);
    }
    ((void (*)(void))sp0[data_02071cf0.state])();
}

// ============ func_02025498 ============
// Executes save state machine (13 state functions)
// Dispatches to current state handler function
void func_02025498(void) {
    s32 sp0[13];
    s32 var_r3;

    for (var_r3 = 0; var_r3 < 13; var_r3++) {
        sp0[var_r3] = *(s32*)((s32)&data_0205c23c + var_r3 * 4);
    }
    ((void (*)(void))sp0[data_02071cf0.state])();
}

// ============ func_020254ec ============
// Loads save data from a specific slot into game memory
// arg0: slot offset (0 or 0x5470)
// Returns: 0=success, 1=checksum fail, 8=data corrupt
s32 func_020254ec(s32 arg0) {
    s32* var_lr;
    s32  temp_r0;
    s32  temp_r1;
    s32  temp_r2;
    s32  temp_r3;
    s32  var_ip;
    u16* var_r4;

    func_0204285c(arg0, (u16*)data_02071cf0.backupData1, 0x3444, 0, 1, 6, 1, 0, 0);
    func_02042aa4();
    func_02042330();
    var_r4 = (u16*)data_02071cf0.backupData1;
    if (func_020258bc(var_r4 + 0x3420 / 2, &data_0205c96c, 0x1B, 0x3420) != 0) {
        if (func_0202593c(var_r4, 0x3444) == 0) {
            var_lr = (s32*)&data_02071d10;
            var_ip = 0x342;
            do {
                temp_r0 = *(s32*)((s32)var_r4 + 0);
                temp_r1 = *(s32*)((s32)var_r4 + 4);
                temp_r2 = *(s32*)((s32)var_r4 + 8);
                temp_r3 = *(s32*)((s32)var_r4 + 12);
                var_r4 += 8;
                var_lr[0] = temp_r0;
                var_lr[1] = temp_r1;
                var_lr[2] = temp_r2;
                var_lr[3] = temp_r3;
                var_lr += 4;
                var_ip -= 1;
            } while (var_ip != 0);
            return 0;
        }
        return 8;
    }
    return 1;
}

// ============ func_020255bc ============
// Loads primary save data (checks both slots 0 and 0x5470)
// Returns: 0=success, 2=error, 8=data corrupt
s32 func_020255bc(void) {
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 var_r4;

    data_02071cf0.backupData1 = (s32)func_020246d4(0x3444);
    temp_r0                   = OS_GetLockID();
    data_02071cf0.lockID      = temp_r0;
    if (temp_r0 == -3) {
        OS_WaitForever();
    }
    func_0204237c((u16)data_02071cf0.lockID);
    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        var_r4 = 2;
        func_0204238c((u16)data_02071cf0.lockID);
        func_020389c0((u16)data_02071cf0.lockID);
        func_0202470c((u16*)data_02071cf0.backupData1);
        return var_r4;
    }
    if (func_02042944() == 1) {
        temp_r0_2 = func_020254ec(0);
        if (temp_r0_2 != 0) {
            temp_r0_3 = func_020254ec(0x5470);
            if (temp_r0_3 != 0) {
                if ((temp_r0_2 != 2) && (temp_r0_3 != 2)) {
                    var_r4 = 8;
                } else {
                    var_r4 = 2;
                }
            } else {
                var_r4 = 0;
            }
        } else {
            var_r4 = 0;
        }
    } else {
        var_r4 = 2;
    }
    func_0204238c((u16)data_02071cf0.lockID);
    func_020389c0((u16)data_02071cf0.lockID);
    func_0202470c((u16*)data_02071cf0.backupData1);
    return var_r4;
}

// ============ func_020256bc ============
// Wrapper function to load save data
void func_020256bc(void) {
    func_020255bc();
}

// ============ func_020256c8 ============
// Loads backup save data from a specific slot into buffer
// arg0: destination buffer, arg1: slot offset (0x3444 or 0x88B4)
// Returns: 0=success, 1=checksum fail, 8=data corrupt
s32 func_020256c8(void* arg0, s32 arg1) {
    s32   temp_r0;
    s32   temp_r1;
    s32   temp_r2;
    s32   temp_r3;
    s32   var_ip;
    u16*  var_r4;
    void* var_r5;

    var_r5 = arg0;
    func_0204285c(arg1, (u16*)data_02071cf0.backupData2, 0x202C, 0, 1, 6, 1, 0, 0);
    func_02042aa4();
    func_02042330();
    var_r4 = (u16*)data_02071cf0.backupData2;
    if (func_020258bc(var_r4 + 0x2008 / 2, &data_0205c96c, 0x1B, 0x2008) != 0) {
        if (func_0202593c(var_r4, 0x202C) == 0) {
            var_ip = 0x200;
            do {
                temp_r0 = *(s32*)((s32)var_r4 + 0);
                temp_r1 = *(s32*)((s32)var_r4 + 4);
                temp_r2 = *(s32*)((s32)var_r4 + 8);
                temp_r3 = *(s32*)((s32)var_r4 + 12);
                var_r4 += 8;
                *(s32*)((s32)var_r5 + 0)  = temp_r0;
                *(s32*)((s32)var_r5 + 4)  = temp_r1;
                *(s32*)((s32)var_r5 + 8)  = temp_r2;
                *(s32*)((s32)var_r5 + 12) = temp_r3;
                var_r5                    = (void*)((s32)var_r5 + 0x10);
                var_ip -= 1;
            } while (var_ip != 0);
            *(s32*)((s32)var_r5 + 0) = *(s32*)((s32)var_r4 + 0);
            *(s32*)((s32)var_r5 + 4) = *(s32*)((s32)var_r4 + 4);
            return 0;
        }
        return 8;
    }
    return 1;
}

// ============ func_0202579c ============
// Loads backup save data (checks both slots 0x3444 and 0x88B4)
// arg0: destination buffer for friend data
// Returns: 0=success, 2=error, 8=data corrupt
s32 func_0202579c(void* arg0) {
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 var_r4;

    data_02071cf0.backupData2 = (s32)func_020246d4(0x202C);
    temp_r0                   = OS_GetLockID();
    data_02071cf0.lockID      = temp_r0;
    if (temp_r0 == -3) {
        OS_WaitForever();
    }
    func_0204237c((u16)data_02071cf0.lockID);
    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        var_r4 = 2;
        func_0204238c((u16)data_02071cf0.lockID);
        func_020389c0((u16)data_02071cf0.lockID);
        func_0202470c((u16*)data_02071cf0.backupData2);
        return var_r4;
    }
    if (func_02042944() == 1) {
        temp_r0_2 = func_020256c8(arg0, 0x3444);
        if (temp_r0_2 != 0) {
            temp_r0_3 = func_020256c8(arg0, 0x88B4);
            if (temp_r0_3 != 0) {
                if ((temp_r0_2 != 2) && (temp_r0_3 != 2)) {
                    var_r4 = 8;
                } else {
                    var_r4 = 2;
                }
            } else {
                var_r4 = 0;
            }
        } else {
            var_r4 = 0;
        }
    } else {
        var_r4 = 2;
    }
    func_0204238c((u16)data_02071cf0.lockID);
    func_020389c0((u16)data_02071cf0.lockID);
    func_0202470c((u16*)data_02071cf0.backupData2);
    return var_r4;
}

// ============ func_020258ac ============
// Gets current save operation substate
// Returns substate value
u16 func_020258ac(void) {
    return data_02071cf0.subState;
}

// ============ func_020258bc ============
// Verifies save data checksums and magic bytes
// arg0: data pointer, arg1: expected checksum array, arg2: checksum count, arg3: expected size
// Returns 1 if valid, 0 if invalid
s32 func_020258bc(u16* arg0, void* arg1, s32 arg2, s32 arg3) {
    s32 var_r4;
    s32 var_r4_2;

    var_r4 = 0;
    if (arg2 > 0) {
        while (1) {
            if (*(arg0 + var_r4) != *(u16*)((s32)arg1 + var_r4 * 2)) {
                return 0;
            }
            var_r4 += 1;
            if (arg2 <= var_r4) {
                break;
            }
        }
    }
    var_r4_2 = 0;
    while (1) {
        if ((u8)((arg3 >> (var_r4_2 * 4)) & 0xF) != *(u8*)((s32)arg0 + var_r4_2 + 0x1B)) {
            return 0;
        }
        var_r4_2 += 1;
        if (var_r4_2 >= 4) {
            return 1;
        }
    }
}

// ============ func_0202593c ============
// Calculates 16-bit checksum of data buffer
// arg0: data pointer, arg1: data size in bytes
// Returns inverted checksum value
u16 func_0202593c(u16* arg0, s32 arg1) {
    s32  var_r1;
    u16* var_r0;
    u16  temp_r1;
    u16  temp_r2;
    u16  var_r3;

    var_r0 = arg0;
    var_r1 = arg1;
    var_r3 = 0;
    if (var_r1 > 1) {
        do {
            temp_r2 = *var_r0;
            var_r0 += 2;
            var_r1 -= 2;
            var_r3 += temp_r2;
        } while (var_r1 > 1);
    }
    if (var_r1 > 0) {
        var_r3 += (u8)*var_r0;
    }
    temp_r1 = (var_r3 >> 0x10) + var_r3;
    return (u16) ~((temp_r1 >> 0x10) + temp_r1);
}

// ============ func_0202599c ============
// Checks if a flag bit is set in flag array
// arg0: flag bit index
// Returns 1 if set, 0 if not
s32 func_0202599c(s32 arg0) {
    if (data_02073cf0.unk48C & (1 << arg0)) {
        return 1;
    }
    return 0;
}

// ============ func_020259e4 ============
// Checks if a flag bit is set in extended flag array (up to 64 flags across 2 dwords)
// arg0: flag bit index
// Returns flag value
s32 func_020259e4(s32 arg0) {
    u32* var_r1;
    s32  var_r3;
    u32  temp_r2;

    var_r3  = (arg0 + ((u32)(arg0 >> 4) >> 0x1B)) >> 5;
    var_r1  = &data_02074184;
    temp_r2 = (u32)arg0 >> 0x1F;
    if (var_r3 > 1) {
        var_r1 = &data_0207418c;
        var_r3 -= 2;
    }
    if (var_r3 != 0) {
        var_r1 += 1;
    }
    return *var_r1 & (1 << (temp_r2 + (((arg0 << 0x1B) - temp_r2) >> 0x1B)));
}

// ============ func_02025a2c ============
// Checks if content is unlocked based on conditions
// arg0: condition value to check against unlock table (67 entries)
// Returns 1 if unlocked, 0 if locked
s32 func_02025a2c(s32 arg0) {
    u16 temp_r0;
    u8* var_r9;
    u32 var_r8;

    var_r9 = (u8*)&data_0205c5be;
    var_r8 = 0;
    while (1) {
        temp_r0 = *(u16*)(var_r9 + 0xC);
        if (!(temp_r0 & 2) && (!(temp_r0 & 1) || (func_02023208(0x2AE, 1, 0) != 0))) {
            if (((s32) * (s16*)(var_r9 + 0) <= arg0) && (arg0 <= (s32) * (s16*)(var_r9 + 2))) {
                if (func_020259e4(var_r8) != 0) {
                    return 1;
                }
            }
        }
        var_r8 += 1;
        var_r9 += 0xE;
        if (var_r8 >= 0x43) {
            return 0;
        }
    }
}

// ============ func_02025ac4 ============
// Resets/zeros save data fields in game state
// arg0: pointer to game data structure
void func_02025ac4(s32* arg0) {
    arg0[0x91B]                 = 0; // 0x246C / 4
    arg0[0x91D]                 = 0; // 0x2474 / 4
    arg0[0x91E]                 = 0; // 0x2478 / 4
    arg0[0x91F]                 = 0; // 0x247C / 4
    arg0[0x920]                 = 0; // 0x2480 / 4
    *(u16*)((s32)arg0 + 0x3106) = 0;
    *(s16*)((s32)arg0 + 0x3108) = -1;
    *(u16*)((s32)arg0 + 0x3112) = 0;
    *(u16*)((s32)arg0 + 0x3114) = 0;
    *(u16*)((s32)arg0 + 0x3116) = 0;
    *(u16*)((s32)arg0 + 0x3118) = 0;
    *(u16*)((s32)arg0 + 0x311A) = 0;
    *(u16*)((s32)arg0 + 0x311C) = 0;
    arg0[0x949]                 = -1; // 0x3124 / 4
    arg0[0x94B]                 = 1;  // 0x312C / 4
}

// ============ func_02025b1c ============
// Initializes/resets all game data to default values
// Resets stats, inventory, flags, and calls initialization helper
void func_02025b1c(void) {
    data_02073cf0.unk4A4 = 1;
    data_02074cf0.unk150 = 0;
    data_02074cf0.unk258 = 0x7F;
    data_02074cf0.unk254 = 0;
    data_02074cf0.unk15C = 0;
    func_020226fc(&data_02071d10);
    func_02025ac4((s32*)&data_02071d10);
}

// ============ func_02025e30 ============
// Releases graphics resources (character data and sprites)
// arg0: pointer to resource structure
void func_02025e30(void* arg0) {
    BgResMgr_ReleaseChar(g_BgResourceManagers[*(s32*)arg0], *(BgResource**)((s32)arg0 + 4));
    func_0200afec(data_0206b3cc[*(s32*)arg0], *(void**)((s32)arg0 + 8));
    func_020105b8((void*)((s32)arg0 + 0xC));
}

// ============ func_02025e74 ============
// Sets up text rendering resources (character graphics and palettes)
// arg0: destination structure, arg1: text rendering configuration
// Returns 1 on success
s32 func_02025e74(void* arg0, void* arg1) {
    s32   sp28;
    s32   sp24;
    void* sp20;
    s32   sp1C;
    s32   sp18;
    u16   sp12;
    u16   sp10;
    s32   spC;
    s32   sp8;
    s32   sp4;
    s32   sp0;
    void* var_ip;
    void* var_r4_2;
    s32   temp_r0;
    s32   temp_r1;
    s32   temp_r2;
    s32   temp_r3;
    s32   temp_r8;
    s32   var_r4;
    void* temp_r7;

    var_ip = &data_0205cac4;
    var_r4 = 4;
    do {
        temp_r0 = *(s32*)((s32)var_ip + 0);
        temp_r1 = *(s32*)((s32)var_ip + 4);
        temp_r2 = *(s32*)((s32)var_ip + 8);
        temp_r3 = *(s32*)((s32)var_ip + 12);
        var_ip  = (void*)((s32)var_ip + 0x10);
        sp0     = temp_r0;
        sp4     = temp_r1;
        sp8     = temp_r2;
        spC     = temp_r3;
        var_r4 -= 1;
    } while (var_r4 != 0);
    sp0      = *(s32*)((s32)var_ip + 0);
    sp4      = *(s32*)((s32)var_ip + 4);
    sp8      = *(s32*)((s32)var_ip + 8);
    sp0      = *(s32*)((s32)arg1 + 0);
    sp4      = *(s32*)((s32)arg1 + 4);
    sp24     = *(s32*)((s32)arg1 + 8);
    sp28     = *(s32*)((s32)arg1 + 12);
    spC      = *(s32*)((s32)arg1 + 20);
    sp10     = *(u16*)((s32)arg1 + 24);
    sp12     = *(u16*)((s32)arg1 + 26);
    var_r4_2 = *(void**)((s32)arg1 + 16);
    temp_r8  = (((sp10 * sp12) + 0x1F) & ~0x1F) << 5;
    if (var_r4_2 == NULL) {
        var_r4_2 = &data_0205caa4;
    }
    temp_r7 = Mem_AllocHeapTail(&gMainHeap, temp_r8 + 0xA0);
    Mem_SetSequence(&gMainHeap, temp_r7, data_02065c84);
    *(void**)((s32)arg0 + 4)   = temp_r7;
    sp20                       = temp_r7;
    *(s32*)arg0                = sp0;
    *(void**)((s32)arg0 + 8)   = func_0200cb4c(data_0206b3d8[sp0], 0, temp_r8, 0);
    *(void**)((s32)arg0 + 12)  = func_0200a968(data_0206b3cc[sp0], var_r4_2, 5, 1);
    *(s32*)((s32)arg0 + 0x428) = sp10;
    *(s32*)((s32)arg0 + 0x42C) = sp12;
    sp18                       = ((s32*)data_0206b3d8[sp0])[2] + (*(s16*)((s32)arg0 + 8 + 0x10) << 5);
    sp1C                       = *(s32*)((s32)arg0 + 12 + 0x10);
    func_0201187c((void*)((s32)arg0 + 0x10), &sp0);
    func_02011ecc((void*)((s32)arg0 + 0x10));
    if (*(s32*)((s32)arg1 + 0x24) != 0) {
        func_02011e94((void*)((s32)arg0 + 0x10), *(s32*)((s32)arg1 + 0x1C), *(s32*)((s32)arg1 + 0x20));
    }
    return 1;
}

// ============ func_0202600c ============
// Placeholder function that returns success
// arg0: unused
// Returns 1
s32 func_0202600c(s32 arg0) {
    return 1;
}

// ============ func_02026014 ============
// Renders text to screen buffer
// arg0: pointer to text rendering structure
void func_02026014(void* arg0) {
    func_02002a10(data_0206b3d8[*(s32*)arg0] + 0x108C, *(s32*)((s32)arg0 + 0x428), *(s32*)((s32)arg0 + 0x42C),
                  *(s32*)((s32)arg0 + 0x4C), 0, *(s32*)((s32)arg0 + 8 + 0x10));
}

// ============ func_02026064 ============
// Cleans up text rendering resources (VRAM, palettes, objects)
// arg0: pointer to text rendering structure
void func_02026064(void* arg0) {
    func_0200cd44(data_0206b3d8[*(s32*)arg0], *(void**)((s32)arg0 + 8));
    func_0200afec(data_0206b3cc[*(s32*)arg0], *(void**)((s32)arg0 + 12));
    func_02011994((void*)((s32)arg0 + 0x10));
    Mem_Free(&gMainHeap, *(void**)((s32)arg0 + 4));
}

// ============ func_020260b8 ============
// Wrapper: sets up text rendering
// arg1: pointer to structure containing render config, arg2: render config
void func_020260b8(void* arg1, void* arg2) {
    func_02025e74(*(void**)((s32)arg1 + 0x18), arg2);
}

// ============ func_020260cc ============
// Wrapper: checks text rendering status
// arg1: pointer to structure
void func_020260cc(void* arg1) {
    func_0202600c(*(s32*)((s32)arg1 + 0x18));
}

// ============ func_020260dc ============
// Wrapper: renders text and returns success
// arg1: pointer to structure
// Returns 1
s32 func_020260dc(void* arg1) {
    func_02026014(*(void**)((s32)arg1 + 0x18));
    return 1;
}

// ============ func_020260f0 ============
// Wrapper: cleans up text rendering and returns success
// arg1: pointer to structure
// Returns 1
s32 func_020260f0(void* arg1) {
    func_02026064(*(void**)((s32)arg1 + 0x18));
    return 1;
}

// ============ func_02026104 ============
// UI function dispatcher - calls function based on arg3 index (11 functions)
// arg0, arg1, arg2: parameters passed to dispatched function
// arg3: function index in dispatch table
void func_02026104(void* arg0, void* arg1, void* arg2, s32 arg3) {
    s32  sp0[4];
    s32* var_ip;
    s32* var_lr;
    s32  var_r3;

    var_lr = (s32*)&data_0205ca94;
    var_ip = sp0;
    var_r3 = 0xB;
    do {
        sp0[0] = var_lr[0];
        sp0[1] = var_lr[1];
        var_lr += 4;
        var_ip += 4;
        var_r3 -= 1;
    } while (var_r3 != 0);
    ((void (*)(void*, void*, void*))sp0[arg3])(arg0, arg1, arg2);
}
