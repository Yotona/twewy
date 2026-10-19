#include "Field/FieldRegion.h"

#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Battle/FieldBattleTransition.h"
#include "Field/FieldTypes.h"
#include "Field/Tasks/Fld_BustShot.h"
#include "Field/Tasks/Fld_Player.h"
#include "Field/Tasks/XFade.h"
#include "Save.h"
#include "SpriteMgr.h"
#include <nitro/fx.h>

extern void func_020265d4(s32* value, s32 target, u16 scale);

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
} BustShotEntry; // Size: 0x10

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ s32            unk40;
    /* 0x44 */ s32            unk44;
    /* 0x48 */ s32            unk48;
    /* 0x4C */ s32            unk4C;
    /* 0x50 */ s32            unk50;
    /* 0x54 */ s32            unk54;
    /* 0x58 */ s16            unk58;
    /* 0x5A */ u8             pad_5A[2];
    /* 0x5C */ Sprite         sprite2;
    /* 0x9C */ s32            unk9C;
    /* 0xA0 */ s32            unkA0;
    /* 0xA4 */ s32            unkA4;
    /* 0xA8 */ s32            unkA8;
    /* 0xAC */ s32            unkAC;
    /* 0xB0 */ s32            unkB0;
    /* 0xB4 */ s32            unkB4;
    /* 0xB8 */ s32            unkB8;
    /* 0xBC */ s16            unkBC;
    /* 0xBE */ s16            unkBE;
    /* 0xC0 */ s16            unkC0;
    /* 0xC2 */ u8             pad_C2[2];
    /* 0xC4 */ BustShotEntry* unkC4;
    /* 0xC8 */ s32            unkC8;
} BustShotSprite; // Size: 0xCC

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
} BustShotArgs; // Size: 0x18

typedef struct {
    /* 0x00 */ void* owner;
    /* 0x04 */ void* sprite2;
    /* 0x08 */ s32   unk8;
    /* 0x0C */ s32   unkC;
    /* 0x10 */ s32   unk10;
    /* 0x14 */ float unk14;
    /* 0x18 */ s32   unk18;
    /* 0x1C */ s32   unk1C;
    /* 0x20 */ s32   unk20;
} BustShotSubArgs; // Size: 0x24
static SpriteFrameInfo* Fld_BustShot_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_BustShot_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const s16 Fld_BustShot_PresetY[3] = {-48, 0, 48};

static const TaskHandle Tsk_Fld_BustShot = {"Tsk_Fld_BustShot", Fld_BustShot_RunTask, sizeof(BustShotSprite)};

static const BinIdentifier Fld_BustShot_Packs[3] = {
    {30,  "Apl_Kit/Grp_FldEvePC.bin"},
    {30, "Apl_Kit/Grp_FldEveNpc.bin"},
    {30, "Apl_Kit/Grp_FldEveEvc.bin"}
};

static const SpriteAnimation Fld_BustShot_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 6,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0400,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_BustShot_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = NULL,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 1,
};

static const BustShotEntry Fld_BustShot_Poses[280] = {
    {0,  1, 0, 23296, 0,  0, 96, 1},
    {0,  2, 0, 23296, 0,  0, 96, 1},
    {0,  3, 0, 23296, 0,  0, 96, 1},
    {0,  4, 0, 23296, 0,  0, 96, 1},
    {0,  5, 0, 23296, 0,  0, 96, 1},
    {0,  6, 0, 23296, 0,  0, 96, 1},
    {0,  7, 0, 23296, 0,  0, 96, 1},
    {0,  8, 0, 23296, 0,  0, 96, 1},
    {0,  9, 0, 23296, 0,  0, 96, 1},
    {0, 10, 0, 23296, 0,  0, 96, 1},
    {0, 11, 0, 23296, 0,  0, 96, 1},
    {0, 12, 0, 23296, 0,  0, 96, 1},
    {0, 13, 0, 23296, 0,  0, 96, 1},
    {0, 14, 0, 23296, 0,  0, 96, 1},
    {0, 15, 0, 23296, 0,  0, 96, 1},
    {0, 16, 0, 23296, 0,  0, 96, 1},
    {0, 17, 0, 23296, 0,  0, 96, 1},
    {0, 18, 0, 18432, 0,  0, 96, 1},
    {0, 19, 0, 18432, 0,  0, 96, 1},
    {0, 20, 0, 18432, 0,  0, 96, 1},
    {0, 21, 0, 18432, 0,  0, 96, 1},
    {0, 22, 0, 18432, 0,  0, 96, 1},
    {0, 23, 0, 18432, 0,  0, 96, 1},
    {0, 24, 0, 18432, 0,  0, 96, 1},
    {0, 25, 0, 18432, 0,  0, 96, 1},
    {0, 26, 0, 18432, 0,  0, 96, 1},
    {0, 27, 0, 18432, 0,  0, 96, 1},
    {0, 28, 0, 18432, 0,  0, 96, 1},
    {0, 29, 0, 18432, 0,  0, 96, 1},
    {0, 30, 0, 18432, 0,  0, 96, 1},
    {0, 31, 0, 18432, 0,  0, 96, 1},
    {0, 32, 0, 18432, 0,  0, 96, 1},
    {0, 33, 0, 18432, 0,  0, 96, 1},
    {0, 34, 0, 18432, 0,  0, 96, 1},
    {0, 35, 0, 18432, 0,  0, 96, 1},
    {0, 36, 0, 18048, 0,  0, 96, 1},
    {0, 37, 0, 18048, 0,  0, 96, 1},
    {0, 38, 0, 18048, 0,  0, 96, 1},
    {0, 39, 0, 18048, 0,  0, 96, 1},
    {0, 40, 0, 18048, 0,  0, 96, 1},
    {0, 41, 0, 18048, 0,  0, 96, 1},
    {0, 42, 0, 18048, 0,  0, 96, 1},
    {0, 43, 0, 18048, 0,  0, 96, 1},
    {0, 44, 0, 18048, 0,  0, 96, 1},
    {0, 45, 0, 18048, 0,  0, 96, 1},
    {0, 46, 0, 18048, 0,  0, 96, 1},
    {0, 47, 0, 18048, 0,  0, 96, 1},
    {0, 48, 0, 18048, 0,  0, 96, 1},
    {0, 49, 0, 18048, 0,  0, 96, 1},
    {0, 50, 0, 28800, 0,  0, 96, 1},
    {0, 51, 0, 28800, 0,  0, 96, 1},
    {0, 52, 0, 28800, 0,  0, 96, 1},
    {0, 53, 0, 28800, 0,  0, 96, 1},
    {0, 54, 0, 28800, 0,  0, 96, 1},
    {0, 55, 0, 28800, 0,  0, 96, 1},
    {0, 56, 0, 28800, 0,  0, 96, 1},
    {0, 57, 0, 28800, 0,  0, 96, 1},
    {0, 58, 0, 28800, 0,  0, 96, 1},
    {0, 59, 0, 28800, 0,  0, 96, 1},
    {0, 60, 0, 28800, 0,  0, 96, 1},
    {0, 61, 0, 28800, 0,  0, 96, 1},
    {0, 62, 0, 28800, 0,  0, 96, 1},
    {0, 63, 0, 28800, 0,  0, 96, 1},
    {0, 64, 0, 28800, 0,  0, 96, 1},
    {0, 65, 0, 28800, 0,  0, 96, 1},
    {2, 39, 0, 28416, 0,  0, 96, 1},
    {2, 40, 0, 28416, 0,  0, 96, 1},
    {2, 41, 0, 28416, 0,  0, 96, 1},
    {2, 42, 0, 28416, 0,  0, 96, 1},
    {2, 43, 0, 28416, 0,  0, 96, 1},
    {2, 44, 0, 28416, 0,  0, 96, 1},
    {2, 45, 0, 28416, 0,  0, 96, 1},
    {2, 46, 0, 28416, 0,  0, 96, 1},
    {2, 47, 0, 28416, 0,  0, 96, 1},
    {2, 48, 0, 28416, 0,  0, 96, 1},
    {2, 49, 0, 28416, 0,  0, 96, 1},
    {2, 50, 0, 28416, 0,  0, 96, 1},
    {2, 46, 0, 28416, 0, 96, 96, 1},
    {2, 77, 0, 15040, 0,  0, 96, 1},
    {2, 78, 0, 15040, 0,  0, 96, 1},
    {2, 79, 0, 15040, 0,  0, 96, 1},
    {2, 80, 0, 15040, 0,  0, 96, 1},
    {2, 81, 0, 15040, 0,  0, 96, 1},
    {2,  1, 0, 27520, 0,  0, 96, 1},
    {2,  2, 0, 27520, 0,  0, 96, 1},
    {2,  3, 0, 27520, 0,  0, 96, 1},
    {2,  4, 0, 27520, 0,  0, 96, 1},
    {2,  5, 0, 27520, 0,  0, 96, 1},
    {2,  6, 0, 27520, 0,  0, 96, 1},
    {2,  7, 0, 27520, 0,  0, 96, 1},
    {2,  9, 0, 27520, 0,  0, 96, 1},
    {2, 10, 0, 23808, 0,  0, 96, 1},
    {2, 11, 0, 23808, 0,  0, 96, 1},
    {2, 12, 0, 23808, 0,  0, 96, 1},
    {2, 13, 0, 23808, 0,  0, 96, 1},
    {2, 14, 0, 23808, 0,  0, 96, 1},
    {2, 15, 0, 23808, 0,  0, 96, 1},
    {2, 16, 0, 23808, 0,  0, 96, 1},
    {2, 17, 0, 23808, 0,  0, 96, 1},
    {2, 18, 0, 28928, 0,  0, 96, 1},
    {2, 19, 0, 28928, 0,  0, 96, 1},
    {2, 20, 0, 28928, 0,  0, 96, 1},
    {2, 21, 0, 28928, 0,  0, 96, 1},
    {2, 22, 0, 28928, 0,  0, 96, 1},
    {2, 23, 0, 28928, 0,  0, 96, 1},
    {2, 24, 0, 28928, 0,  0, 96, 1},
    {2, 25, 0, 28928, 0,  0, 96, 1},
    {2, 26, 0, 28928, 0,  0, 96, 1},
    {2, 27, 0, 32384, 0,  0, 96, 1},
    {2, 28, 0, 32384, 0,  0, 96, 1},
    {2, 29, 0, 32384, 0,  0, 96, 1},
    {2, 30, 0, 32384, 0,  0, 96, 1},
    {2, 31, 0, 32384, 0,  0, 96, 1},
    {2, 32, 0, 22784, 0,  0, 96, 1},
    {2, 33, 0, 22784, 0,  0, 96, 1},
    {2, 34, 0, 22784, 0,  0, 96, 1},
    {2, 35, 0, 22784, 0,  0, 96, 1},
    {2, 36, 0, 22784, 0,  0, 96, 1},
    {2, 37, 0, 22784, 0,  0, 96, 1},
    {2, 38, 0, 22784, 0,  0, 96, 1},
    {2, 51, 0, 26752, 0,  0, 96, 1},
    {2, 52, 0, 26752, 0,  0, 96, 1},
    {2, 53, 0, 26752, 0,  0, 96, 1},
    {2, 54, 0, 26752, 0,  0, 96, 1},
    {2, 55, 0, 26752, 0,  0, 96, 1},
    {2, 56, 0, 26752, 0,  0, 96, 1},
    {2, 57, 0, 26752, 0,  0, 96, 1},
    {2, 58, 0, 26752, 0,  0, 96, 1},
    {2, 59, 0, 26752, 0,  0, 96, 1},
    {2, 60, 0, 26752, 0,  0, 96, 1},
    {2, 61, 0, 28288, 0,  0, 96, 1},
    {2, 62, 0, 28288, 0,  0, 96, 1},
    {2, 63, 0, 28288, 0,  0, 96, 1},
    {2, 64, 0, 28288, 0,  0, 96, 1},
    {2, 65, 0, 28288, 0,  0, 96, 1},
    {2, 66, 0, 28288, 0,  0, 96, 1},
    {2, 67, 0, 31232, 0,  0, 96, 1},
    {2, 74, 0, 31232, 0,  0, 96, 1},
    {2, 68, 0, 31232, 0,  0, 96, 1},
    {2, 75, 0, 31232, 0,  0, 96, 1},
    {2, 69, 0, 31232, 0,  0, 96, 1},
    {2, 70, 0, 31232, 0,  0, 96, 1},
    {2, 76, 0, 31232, 0,  0, 96, 1},
    {2, 71, 0, 31232, 0,  0, 96, 1},
    {2, 72, 0, 31232, 0,  0, 96, 1},
    {2, 73, 0, 31232, 0,  0, 96, 1},
    {1,  1, 0, 16256, 0,  0, 96, 1},
    {1,  1, 1, 16256, 0,  0, 96, 1},
    {1,  1, 2, 16256, 0,  0, 96, 1},
    {1,  1, 3, 16256, 0,  0, 96, 1},
    {1,  1, 4, 16256, 0,  0, 96, 1},
    {1,  1, 5, 16256, 0,  0, 96, 1},
    {1,  1, 6, 16256, 0,  0, 96, 1},
    {1,  1, 7, 16256, 0,  0, 96, 1},
    {1,  2, 0, 13696, 0,  0, 96, 1},
    {1,  2, 1, 13696, 0,  0, 96, 1},
    {1,  2, 2, 13696, 0,  0, 96, 1},
    {1,  2, 3, 13696, 0,  0, 96, 1},
    {1,  2, 4, 13696, 0,  0, 96, 1},
    {1,  2, 5, 13696, 0,  0, 96, 1},
    {1,  2, 6, 13696, 0,  0, 96, 1},
    {1,  2, 7, 13696, 0,  0, 96, 1},
    {1,  3, 0, 18304, 0,  0, 96, 1},
    {1,  3, 1, 18304, 0,  0, 96, 1},
    {1,  3, 2, 18304, 0,  0, 96, 1},
    {1,  3, 3, 18304, 0,  0, 96, 1},
    {1,  3, 4, 18304, 0,  0, 96, 1},
    {1,  3, 5, 18304, 0,  0, 96, 1},
    {1,  3, 6, 18304, 0,  0, 96, 1},
    {1,  4, 0, 17920, 0,  0, 96, 1},
    {1,  4, 1, 17920, 0,  0, 96, 1},
    {1,  4, 2, 17920, 0,  0, 96, 1},
    {1,  4, 3, 17920, 0,  0, 96, 1},
    {1,  4, 4, 17920, 0,  0, 96, 1},
    {1,  4, 5, 17920, 0,  0, 96, 1},
    {1,  4, 6, 17920, 0,  0, 96, 1},
    {1,  5, 0, 17536, 0,  0, 96, 1},
    {1,  5, 1, 17536, 0,  0, 96, 1},
    {1,  5, 2, 17536, 0,  0, 96, 1},
    {1,  5, 3, 17536, 0,  0, 96, 1},
    {1,  5, 4, 17536, 0,  0, 96, 1},
    {1,  5, 5, 17536, 0,  0, 96, 1},
    {1,  5, 6, 17536, 0,  0, 96, 1},
    {1,  6, 0, 18688, 0,  0, 96, 1},
    {1,  6, 1, 18688, 0,  0, 96, 1},
    {1,  6, 2, 18688, 0,  0, 96, 1},
    {1,  6, 3, 18688, 0,  0, 96, 1},
    {1,  6, 4, 18688, 0,  0, 96, 1},
    {1,  6, 5, 18688, 0,  0, 96, 1},
    {1,  6, 6, 18688, 0,  0, 96, 1},
    {1,  7, 0, 16640, 0,  0, 96, 1},
    {1,  7, 1, 16640, 0,  0, 96, 1},
    {1,  7, 2, 16640, 0,  0, 96, 1},
    {1,  7, 3, 16640, 0,  0, 96, 1},
    {1,  7, 4, 16640, 0,  0, 96, 1},
    {1,  7, 5, 16640, 0,  0, 96, 1},
    {1,  8, 0, 17024, 0,  0, 96, 1},
    {1,  8, 1, 17024, 0,  0, 96, 1},
    {1,  8, 2, 17024, 0,  0, 96, 1},
    {1,  8, 3, 17024, 0,  0, 96, 1},
    {1,  8, 4, 17024, 0,  0, 96, 1},
    {1,  8, 5, 17024, 0,  0, 96, 1},
    {1,  9, 0, 16512, 0,  0, 96, 1},
    {1,  9, 1, 16512, 0,  0, 96, 1},
    {1,  9, 2, 16512, 0,  0, 96, 1},
    {1,  9, 3, 16512, 0,  0, 96, 1},
    {1,  9, 4, 16512, 0,  0, 96, 1},
    {1,  9, 5, 16512, 0,  0, 96, 1},
    {1, 10, 0, 21760, 0,  0, 96, 1},
    {1, 10, 1, 21760, 0,  0, 96, 1},
    {1, 10, 2, 21760, 0,  0, 96, 1},
    {1, 10, 3, 21760, 0,  0, 96, 1},
    {1, 10, 4, 21760, 0,  0, 96, 1},
    {1, 10, 5, 21760, 0,  0, 96, 1},
    {1, 10, 6, 21760, 0,  0, 96, 1},
    {1, 11, 0, 24064, 0,  0, 96, 1},
    {1, 11, 1, 24064, 0,  0, 96, 1},
    {1, 11, 2, 24064, 0,  0, 96, 1},
    {1, 11, 3, 24064, 0,  0, 96, 1},
    {1, 11, 4, 24064, 0,  0, 96, 1},
    {1, 11, 5, 24064, 0,  0, 96, 1},
    {1, 11, 6, 24064, 0,  0, 96, 1},
    {1, 12, 0, 19584, 0,  0, 96, 1},
    {1, 13, 0, 23936, 0,  0, 96, 1},
    {1, 14, 0, 19968, 0,  0, 96, 1},
    {1, 14, 1, 19968, 0,  0, 96, 1},
    {1, 14, 2, 19968, 0,  0, 96, 1},
    {1, 14, 3, 19968, 0,  0, 96, 1},
    {1, 14, 4, 19968, 0,  0, 96, 1},
    {1, 14, 5, 19968, 0,  0, 96, 1},
    {1, 15, 0, 28288, 0,  0, 96, 1},
    {1, 15, 0, 28288, 0,  0, 96, 1},
    {1, 15, 0, 28288, 0,  0, 96, 1},
    {1, 15, 0, 28288, 0,  0, 96, 1},
    {1, 16, 0, 25856, 0,  0, 96, 1},
    {1, 16, 1, 25856, 0,  0, 96, 1},
    {1, 16, 2, 25856, 0,  0, 96, 1},
    {1, 16, 3, 25856, 0,  0, 96, 1},
    {1, 16, 4, 25856, 0,  0, 96, 1},
    {1, 16, 5, 25856, 0,  0, 96, 1},
    {1, 16, 6, 25856, 0,  0, 96, 1},
    {1, 17, 0, 17152, 0,  0, 96, 1},
    {1, 17, 1, 17152, 0,  0, 96, 1},
    {1, 17, 2, 17152, 0,  0, 96, 1},
    {1, 17, 3, 17152, 0,  0, 96, 1},
    {1, 17, 4, 17152, 0,  0, 96, 1},
    {1, 17, 5, 17152, 0,  0, 96, 1},
    {1, 17, 6, 17152, 0,  0, 96, 1},
    {1, 18, 0, 21760, 0,  0, 96, 1},
    {1, 18, 1, 21760, 0,  0, 96, 1},
    {1, 18, 2, 21760, 0,  0, 96, 1},
    {1, 19, 0, 16384, 0,  0, 96, 1},
    {1, 19, 1, 16384, 0,  0, 96, 1},
    {1, 19, 2, 16384, 0,  0, 96, 1},
    {1, 20, 0, 16384, 0,  0, 96, 1},
    {1, 20, 1, 16384, 0,  0, 96, 1},
    {1, 20, 2, 16384, 0,  0, 96, 1},
    {1, 21, 0, 21504, 0,  0, 96, 1},
    {1, 21, 1, 21504, 0,  0, 96, 1},
    {1, 21, 2, 21504, 0,  0, 96, 1},
    {1, 30, 0, 14720, 0,  0, 96, 1},
    {1, 30, 1, 14720, 0,  0, 96, 1},
    {1, 31, 0, 16640, 0,  0, 96, 1},
    {1, 31, 1, 16640, 0,  0, 96, 1},
    {1, 34, 0, 16640, 0,  0, 96, 1},
    {1, 34, 1, 16640, 0,  0, 96, 1},
    {1, 36, 0, 16000, 0,  0, 96, 1},
    {1, 36, 1, 16000, 0,  0, 96, 1},
    {1, 37, 0, 16640, 0,  0, 96, 1},
    {1, 37, 1, 16640, 0,  0, 96, 1},
    {1, 38, 0, 16640, 0,  0, 96, 1},
    {1, 38, 1, 16640, 0,  0, 96, 1},
    {1, 39, 0, 16640, 0,  0, 96, 1},
    {1, 39, 1, 16640, 0,  0, 96, 1},
    {1, 52, 0, 16640, 0,  0, 96, 1},
    {1, 52, 1, 16640, 0,  0, 96, 1},
    {1, 59, 0, 16640, 0,  0, 96, 1},
    {1, 59, 1, 16640, 0,  0, 96, 1},
    {1, 60, 0, 17600, 0,  0, 96, 1},
    {1, 60, 1, 17600, 0,  0, 96, 1},
};

s32 Fld_BustShot_GetMoveInputDir(s32* out) {
    TouchCoord pos;
    TouchCoord coord;

    if (Field_IsMoveInputActive() == 0) {
        return 0;
    }
    if (SysControl.buttonState.currButtons & 0xCF3) {
        *out = Fld_Player_AngleToDir(Fld_Player_GetDPadAngle());
    } else {
        FieldPos* origin;

        TouchInput_GetCoord(&coord);
        pos    = coord;
        origin = Field_GetCameraOrigin();
        *out   = Fld_Player_AngleToDir(FX_Atan2Idx((origin->y + (pos.y << 0xC)) - gSaveData.fieldPos.y,
                                                   (origin->x + (pos.x << 0xC)) - gSaveData.fieldPos.x));
    }
    return 1;
}

static SpriteFrameInfo* Fld_BustShot_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    BustShotSprite* owner = sprite->owner;

    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = owner->unkB4)
}

// Nonmatching
static void Fld_BustShot_LoadSprite(Sprite* sprite, s32 arg1, BustShotEntry cfg, s32 unk9c, FldScreenPos pos, void* owner,
                                    s32 index) {
    SpriteAnimation anim = Fld_BustShot_Anim;

    anim.dataType  = arg1;
    anim.packIndex = cfg.unk2;
    anim.unk_1E    = cfg.unk6;
    anim.binIden   = &Fld_BustShot_Packs[cfg.unk0];
    anim.unk_20    = cfg.unkE;
    anim.posX      = pos.x;
    anim.posY      = pos.y;
    anim.animIndex += cfg.unk4;
    anim.owner = owner;

    if (unk9c != 0) {
        Unk_Bitfield bits = anim.unk_02;

        bits.unk_05 = (u16)(bits.unk_05 | 8);
        anim.unk_02 = bits;
    }
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_BustShot_ResolvePresetY(s32 arg0) {
    if (arg0 < -0x3F2) {
        return arg0;
    }
    if (arg0 >= -0x3EF) {
        return arg0;
    }
    return Fld_BustShot_PresetY[arg0 + 0x3F2];
}

// Nonmatching
static s32 Fld_BustShot_Init(TaskPool* pool, Task* task, void* args) {
    BustShotArgs*   params = args;
    BustShotSprite* sprite = task->data;
    BustShotEntry   cfg;
    FldScreenPos    pos;
    u32             idx;
    BustShotEntry*  entry;
    s16             dx;
    s16             dy;
    s32             yOffset;
    s32             mirrored;

    idx = params->unk4;
    if (idx >= 0x118U) {
        idx = 0;
    }
    entry         = &Fld_BustShot_Poses[idx];
    sprite->unkC4 = entry;
    sprite->unkC8 = 0;
    sprite->unk58 = (s16)params->unkC;
    pos.sx        = Field_ResolvePresetCoord(params->unk12);
    if (pos.sx < 0x80) {
        mirrored = 1;
    } else {
        mirrored = 0;
    }
    sprite->unk9C = mirrored;
    if (sprite->unk9C == 0) {
        dx = entry->unk8;
    } else {
        dx = 0 - entry->unk8;
    }
    pos.sx        = (s16)(pos.sx + dx);
    yOffset       = Fld_BustShot_ResolvePresetY(params->unk16);
    sprite->unk48 = pos.sx << 0xC;
    pos.sy        = (s16)(entry->unkC + yOffset);
    if (sprite->unk9C == 0) {
        dy = entry->unkA;
    } else {
        dy = 0 - entry->unkA;
    }
    sprite->unk4C = (dy + Field_ResolvePresetCoord(params->unk10)) << 0xC;
    sprite->unk40 = sprite->unk48;
    sprite->unk44 = sprite->unk4C;
    sprite->unk50 = pos.sy << 0xC;
    sprite->unk54 = (entry->unkC + Fld_BustShot_ResolvePresetY(params->unk16)) << 0xC;
    cfg           = *entry;
    Fld_BustShot_LoadSprite(&sprite->sprite, params->unk0, cfg, sprite->unk9C, pos, sprite, idx);
    sprite->unkA0 = 1;
    sprite->unkA4 = 0;
    sprite->unkA8 = params->unk0;
    sprite->unkAC = -1;
    sprite->unkB0 = 0;
    sprite->unkB4 = 0x16;
    sprite->unkB8 = 0;
    sprite->unkBC = 0;
    sprite->unkBE = 0;
    sprite->unkC0 = 0;
}

static s32 Fld_BustShot_Update(TaskPool* pool, Task* task, void* args) {
    BustShotSprite* sprite = task->data;

    if (sprite->unkAC != -1) {
        if (EasyTask_ValidateTaskId(pool, (u32*)&sprite->unkAC) != 0) {
            return 1;
        }
        sprite->unkAC = -1;
        Sprite_Destroy(&sprite->sprite);
        sprite->sprite = sprite->sprite2;
        sprite->unkA0  = 1;
    }
    if (sprite->unk58 != 0) {
        if (FieldEvent_IsDebugSkipHeld() != 0) {
            sprite->unk58 = 1;
        }
        func_020265d4(&sprite->unk40, sprite->unk44, (u16)sprite->unk58);
        func_020265d4(&sprite->unk50, sprite->unk54, (u16)sprite->unk58);
        sprite->unk58 = (s16)(sprite->unk58 - 1);
    }
    sprite->sprite.posX = (s16)(sprite->unk40 >> 0xC);
    sprite->sprite.posY = (s16)(sprite->unk50 >> 0xC);
    if (sprite->unk58 != 0 && sprite->unkB8 != 0) {
        sprite->unkBC = (s16)(sprite->unkBC + 1);
        if (sprite->unkBC % 2 == 0) {
            sprite->unkBE = (s16)(RNG_Next(8) - 4);
            sprite->unkC0 = (s16)(RNG_Next(8) - 4);
        }
        sprite->sprite.posX += sprite->unkBE;
        sprite->sprite.posY += sprite->unkC0;
    } else {
        sprite->unkB8 = 0;
    }
    if (sprite->unkA0 != 0) {
        Sprite_UpdateAndCheck(&sprite->sprite);
    }
    if (sprite->unkB0 != 0 && Fld_BustShot_IsSlideOutDone(task) != 0) {
        return 0;
    }
    return 1;
}

static s32 Fld_BustShot_Render(TaskPool* pool, Task* task, void* args) {
    BustShotSprite* sprite = task->data;

    if (sprite->unkA0 != 0) {
        Sprite_Render(&sprite->sprite);
    }
    return 1;
}

static s32 Fld_BustShot_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Destroy(&((BustShotSprite*)task->data)->sprite);
    return 1;
}

static s32 Fld_BustShot_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_BustShot_Init,
        .update     = Fld_BustShot_Update,
        .render     = Fld_BustShot_Render,
        .cleanup    = Fld_BustShot_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_BustShot_CreateTask(TaskPool* pool, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7) {
    BustShotArgs args;

    args.unk0  = arg1;
    args.unk4  = arg2;
    args.unkC  = arg3;
    args.unk12 = arg4;
    args.unk16 = arg5;
    args.unk10 = arg6;
    args.unk14 = arg7;
    return EasyTask_CreateTask(pool, &Tsk_Fld_BustShot, NULL, 0, NULL, &args);
}

void Fld_BustShot_SlideOut(Task* task, s32 arg1, s32 arg2) {
    BustShotSprite* sprite = task->data;
    s16             dx;
    s32             x;
    s32             slideX;

    sprite->unkB0 = arg2;
    if (sprite->unkC8 != 0) {
        return;
    }
    sprite->unkC8 = 1;
    if (sprite->unk9C == 0) {
        dx = sprite->unkC4->unk8;
    } else {
        dx = 0 - sprite->unkC4->unk8;
    }
    x = sprite->unk40;
    if (arg1 != 0) {
        if (x < 0x80000) {
            slideX = (dx + Field_ResolvePresetCoord(-0x3FD)) << 0xC;
        } else {
            slideX = (dx + Field_ResolvePresetCoord(0xFFFFFC0D)) << 0xC;
        }
    } else if (x >= 0x80000) {
        slideX = (dx + Field_ResolvePresetCoord(-0x3FD)) << 0xC;
    } else {
        slideX = (dx + Field_ResolvePresetCoord(0xFFFFFC0D)) << 0xC;
    }
    sprite->unk44 = slideX;
    sprite->unk48 = slideX;
    sprite->unk58 = (s16)(0x14 - sprite->unk58);
    if ((s32)sprite->unk58 < 1) {
        sprite->unk58 = 1;
    }
}

s32 Fld_BustShot_IsSlideOutDone(Task* task) {
    BustShotSprite* sprite = task->data;

    if (sprite->unk58 == 0 && sprite->unk44 == sprite->unk48) {
        sprite->unkC8 = 0;
        return 1;
    }
    return 0;
}

s32 Fld_BustShot_IsMoveDone(Task* task) {
    return ((BustShotSprite*)task->data)->unk58 == 0;
}

s16 Fld_BustShot_GetPosX(Task* task) {
    return ((BustShotSprite*)task->data)->sprite.posX;
}

void Fld_BustShot_Nudge(Task* task, s32 arg1) {
    BustShotSprite* sprite = task->data;
    s32             nudge;

    nudge = 0x10000;
    if ((s32)sprite->sprite.posX < 0x60) {
        nudge = -0x10000;
    }
    if (arg1 != 0) {
        nudge = 0 - nudge;
    }
    sprite->unk44 = sprite->unk44 + nudge;
    if (sprite->unk58 == 0) {
        sprite->unk58 = 8;
    }
}

void Fld_BustShot_MoveToCenter(Task* task, s16 arg1) {
    BustShotSprite* sprite = task->data;

    sprite->unk44 = 0x80000;
    sprite->unk58 = arg1;
}

void Fld_BustShot_ToggleFlip(Task* task) {
    BustShotSprite* sprite = task->data;

    if (sprite->sprite.unk_0A.unk_05 & 8) {
        Unk_Bitfield bits = sprite->sprite.unk_0A;

        bits.unk_05           = 0;
        sprite->sprite.unk_0A = bits;
        sprite->unk9C         = 0;
        return;
    }
    {
        Unk_Bitfield bits = sprite->sprite.unk_0A;

        bits.unk_05 |= 8;
        sprite->sprite.unk_0A = bits;
    }
    sprite->unk9C = 1;
}

void Fld_BustShot_CrossFadeTo(TaskPool* pool, Task* task, u32 idx, s32 arg3, s32 arg4) {
    BustShotSprite* sprite = task->data;
    BustShotEntry   cfg;
    BustShotSubArgs sub;

    if (sprite->unkAC != -1) {
        return;
    }
    sprite->unkA0 = 0;
    sprite->unkA4 = 1;
    if (idx >= 0x118U) {
        idx = 0;
    }
    cfg = Fld_BustShot_Poses[idx];
    Fld_BustShot_LoadSprite(&sprite->sprite2, sprite->unkA8, cfg, sprite->unk9C, *(FldScreenPos*)&sprite->sprite.posX, sprite,
                            idx);

    sub.owner   = sprite;
    sub.sprite2 = &sprite->sprite2;
    sub.unk8    = 0;
    sub.unkC    = 1;
    if (arg4 == 0xA) {
        sub.unk14 = 1.0f;
    } else if (arg4 == 0x3C) {
        sub.unk14 = 0.25f;
    } else if (arg4 == 0) {
        sub.unk14 = 4.0f;
    } else {
        sub.unk14 = 0.5f;
    }
    sub.unk10     = arg3;
    sprite->unkAC = EasyTask_CreateTask(pool, &Tsk_XFade, NULL, 0, NULL, &sub);
}

s32 Fld_BustShot_IsCrossFadeDone(Task* task) {
    BustShotSprite* sprite = task->data;

    if (sprite->unkA4 != 0 && sprite->unkA0 != 0) {
        sprite->unkA4 = 0;
        return 1;
    }
    return 0;
}

void Fld_BustShot_SwapSide(Task* task, s16 arg1) {
    BustShotSprite* sprite = task->data;
    s32             v;

    sprite->unk48 = 0x100000 - sprite->unk48;
    v             = 0x100000 - sprite->unk4C;
    sprite->unk4C = v;
    sprite->unk44 = v;
    sprite->unk58 = arg1;
}

void Fld_BustShot_SetSortKey(Task* task, s32 arg1) {
    ((BustShotSprite*)task->data)->unkB4 = arg1;
}

void Fld_BustShot_MoveTo(Task* task, s16 arg1, s32 arg2, s16 arg3) {
    BustShotSprite* sprite = task->data;
    s16             dy;

    if (sprite->unk9C == 0) {
        dy = sprite->unkC4->unkA;
    } else {
        dy = 0 - sprite->unkC4->unkA;
    }
    sprite->unk44 = (dy + Field_ResolvePresetCoord(arg1)) << 0xC;
    sprite->unk54 = (sprite->unkC4->unkC + Fld_BustShot_ResolvePresetY(arg2)) << 0xC;
    sprite->unk58 = arg3;
}

void Fld_BustShot_ChangePose(TaskPool* pool, Task* task, u32 idx, s32 arg3) {
    BustShotSprite* sprite = task->data;
    BustShotEntry   cfg;
    FldScreenPos    pos;

    if (idx >= 0x118U) {
        idx = 0;
    }
    pos = *(FldScreenPos*)&sprite->sprite.posX;
    Sprite_Destroy(&sprite->sprite);
    cfg = Fld_BustShot_Poses[idx];
    Fld_BustShot_LoadSprite(&sprite->sprite, sprite->unkA8, cfg, sprite->unk9C, pos, sprite, idx);
    sprite->unkA4 = 1;
}

s32 Fld_BustShot_ConsumePoseChanged(Task* task) {
    BustShotSprite* sprite = task->data;
    s32             pending;

    if (sprite->unkA4 != 0) {
        sprite->unkA4 = 0;
        pending       = 1;
    } else {
        pending = 0;
    }
    return pending;
}

void Fld_BustShot_StartShake(Task* task) {
    BustShotSprite* sprite = task->data;

    sprite->unkB8 = 1;
    sprite->unkBC = 0;
    if (sprite->unk58 == 0) {
        sprite->unk58 = 0x14;
    }
}

s16 Fld_BustShot_GetScreenX(Task* task) {
    return ((BustShotSprite*)task->data)->sprite.posX;
}

void Fld_BustShot_SetPriority(Task* task, u16 arg1) {
    BustShotSprite* sprite = task->data;

    sprite->sprite.unk_0A.unk_10 = arg1;
}

void Fld_BustShot_ClearPoseChanged(Task* task) {
    ((BustShotSprite*)task->data)->unkA4 = 0;
}
