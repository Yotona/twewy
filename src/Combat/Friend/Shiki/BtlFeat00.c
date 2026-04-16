#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Friend/Private/Shiki.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "SndMgr.h"

extern BinIdentifier data_ov003_020dd7dc;
extern BinIdentifier data_ov003_020dd7c4;

typedef struct {
    /* 0x0 */ u16 x;
    /* 0x2 */ u16 y;
} Coord;

typedef struct BtlFeat00 {
    /* 0x00 */ Data*            unk_00;
    /* 0x04 */ Data*            unk_04;
    /* 0x08 */ BgResource*      unk_08;
    /* 0x0C */ BgResource*      unk_0C;
    /* 0x10 */ BgResource*      unk_10;
    /* 0x14 */ BgResource*      unk_14;
    /* 0x18 */ PaletteResource* unk_18;
    /* 0x1C */ PaletteResource* unk_1C;
    /* 0x20 */ s32              unk_20;
    /* 0x24 */ s16              unk_24;
    /* 0x28 */ s32              unk_28;
    /* 0x2C */ s32              unk_2C;
    /* 0x30 */ u16              unk_30;
    /* 0x32 */ u16              unk_32;
    /* 0x34 */ void (*unk_34)(struct BtlFeat00*);
    /* 0x38 */ s16 unk_38;
    /* 0x3A */ u16 unk_3A;
    /* 0x3C */ s32 unk_3C;
} BtlFeat00; // Size: 0x44

s32 BtlFeat00_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static u16         data_ov004_020f0730[2] = {0, 2};
static const Coord data_ov004_020f03c4[8] = {};

static const s16 data_ov004_020f03a8[6] = {0, 2, 3, 1, 2, 3};

const TaskHandle Tsk_BtlFeat00 = {"Tsk_BtlFeat00", BtlFeat00_RunTask, 0x44};

void func_ov004_020eb4bc(BtlFeat00* feat, void (*callback)(BtlFeat00*)) {
    feat->unk_34 = callback;
    feat->unk_38 = 0;
    feat->unk_3A = 0;
}

void func_ov004_020eb4d0(BtlFeat00* feat) {
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 2;
    g_DisplaySettings.controls[0].layers &= ~4;
    g_DisplaySettings.controls[0].layers &= ~8;
    Display_SetBGOffset(DISPLAY_MAIN, DISPLAY_BG1, 0, 0);

    g_DisplaySettings.engineState[1].bgSettings[1].priority = 2;
    g_DisplaySettings.controls[1].layers &= ~4;
    g_DisplaySettings.controls[1].layers &= ~8;
    Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG1, 0, 0);

    feat->unk_00 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7dc, 1, 0);
    feat->unk_04 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7c4, 3, 0);

    void* var_r1 = Data_GetPackEntryData(feat->unk_00, 3);
    void* var_r4 = Data_GetPackEntryData(feat->unk_00, 4);
    void* var_r5 = Data_GetPackEntryData(feat->unk_04, 2);

    feat->unk_08 = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], var_r1,
                                        g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].charBase, 0, 0x7B60);
    feat->unk_10 = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], var_r4,
                                        g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].screenBase,
                                        g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].screenSizeText);
    feat->unk_18 = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], var_r5, 0, 0, 1);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], feat->unk_18);

    void* var_r1_2 = Data_GetPackEntryData(feat->unk_00, 1);
    void* var_r5_2 = Data_GetPackEntryData(feat->unk_00, 2);
    void* var_r4_2 = Data_GetPackEntryData(feat->unk_04, 1);

    feat->unk_0C = BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r1_2,
                                        g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, 0x7B60);
    feat->unk_14 =
        BgResMgr_AllocScreen(g_BgResourceManagers[1], var_r5_2, g_DisplaySettings.engineState[1].bgSettings[1].screenBase,
                             g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText);
    feat->unk_1C = PaletteMgr_AllocPalette(g_PaletteManagers[1], var_r4_2, 0, 0, 1);
    PaletteMgr_Flush(g_PaletteManagers[1], feat->unk_1C);

    feat->unk_20 = 0;
}

void func_ov004_020eb7a4(BtlFeat00* feat) {
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], feat->unk_10);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_MAIN], feat->unk_18);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_MAIN], feat->unk_08);
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], feat->unk_14);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], feat->unk_1C);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], feat->unk_0C);
    DatMgr_ReleaseData(feat->unk_00);
    DatMgr_ReleaseData(feat->unk_04);
}

void func_ov004_020eb828(s32 arg0, s32 arg1) {
    Coord**      ranges;
    Coord*       var_r3;
    const Coord* var_r4;
    s16          x;
    s16          temp;
    s16          y;
    s32          row;
    s32          var_r2;
    u16*         phase_slot;
    u32          facing;
    u32          side_offset;

    var_r3 = &ranges[0][0];

    for (s32 i = 8; i != 0; i--) {
        ranges[i]->x = data_ov004_020f03c4[i].x;
        ranges[i]->y = data_ov004_020f03c4[i].y;
    }

    side_offset = (u32)(arg0 << 0x10) >> 0xF;
    phase_slot  = &data_ov004_020f0730[side_offset];
    *phase_slot += 1;
    *phase_slot &= 3;

    row = ((s32)(*phase_slot) << 3) >> 1;

    temp = ranges[0][row].x;
    x    = temp + RNG_Next(ranges[0][row + 1].x - temp);

    temp   = ranges[0][row].y;
    y      = temp + RNG_Next(ranges[0][row + 1].y - temp);
    facing = RNG_Next(2);

    if (arg1 >= 4) {
        OS_WaitForever();
    }

    BtlFeat00_Multi_CreateTask(arg0, arg1, x, y, facing, 1);
    SndMgr_PlaySEWithPan(0x1F, x);
}

void func_ov004_020eb934(BtlFeat00* feat) {
    s16 temp[6];
    temp = data_ov004_020f03a8;

    if (feat->unk_24 & 1) {
        feat->unk_30++;
        feat->unk_30 %= 6;
        func_ov004_020eb828(0, temp[feat->unk_30]);
    } else {
        feat->unk_32++;
        feat->unk_32 %= 6;
        func_ov004_020eb828(1, temp[feat->unk_32]);
    }
    feat->unk_24++;
}

void func_ov004_020eba1c(BtlFeat00* feat) {
    feat->unk_38--;
    if (feat->unk_38 <= 0) {
        func_ov004_020eb934(feat);
        if (feat->unk_24 <= 36) {
            feat->unk_38 = 5;
        } else {
            EasyFade_FadeBothDisplays(FADER_SMOOTH, 0x10, 0x1000);
            feat->unk_3C = 0;
        }
    }
}

s32 BtlFeat00_Init(TaskPool* pool, Task* task, void* args) {
    BtlFeat00* feat = task->data;

    MI_CpuSet(feat, 0, sizeof(BtlFeat00));
    feat->unk_3C = 1;
    feat->unk_24 = 0;
    feat->unk_28 = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    feat->unk_2C = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
    feat->unk_30 = 0;
    feat->unk_32 = 2;
    func_ov004_020eb4d0(feat);
    func_ov004_020eb4bc(feat, func_ov004_020eba1c);
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x800);
    return 1;
}

s32 BtlFeat00_Update(TaskPool* pool, Task* task, void* args) {
    BtlFeat00* feat = task->data;

    if (feat->unk_34 != NULL) {
        feat->unk_34(feat);
    }

    feat->unk_20 -= 0x6000;

    Display_SetBGOffset(DISPLAY_MAIN, DISPLAY_BG1, 0, feat->unk_20);
    Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG1, 0, feat->unk_20);

    return feat->unk_3C;
}

s32 BtlFeat00_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 BtlFeat00_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlFeat00* feat = task->data;

    func_ov004_020eb7a4(feat);
    return 1;
}

s32 BtlFeat00_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlFeat00_Init,
        .update     = BtlFeat00_Update,
        .render     = BtlFeat00_Render,
        .cleanup    = BtlFeat00_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}