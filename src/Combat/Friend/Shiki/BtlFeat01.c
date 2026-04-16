#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "SndMgr.h"

extern BinIdentifier data_ov003_020dd7c4;

typedef struct BtlFeat01 {
    /* 0x000 */ s32 (**unk_000)(struct BtlFeat01*);
    /* 0x004 */ char             unk_004[4];
    /* 0x008 */ char*            unk_008;
    /* 0x00C */ char             unk_00C[0x0DC - 0x00C];
    /* 0x0DC */ Data*            unk_0DC;
    /* 0x0E0 */ BgResource*      unk_0E0;
    /* 0x0E4 */ BgResource*      unk_0E4;
    /* 0x0E8 */ BgResource*      unk_0E8;
    /* 0x0EC */ BgResource*      unk_0EC;
    /* 0x0F0 */ PaletteResource* unk_0F0;
    /* 0x0F4 */ PaletteResource* unk_0F4;
    /* 0x0F8 */ s32              unk_0F8;
    /* 0x0FC */ char             unk_0FC[0x10A - 0x0FC];
    /* 0x10A */ s16              unk_10A;
    /* 0x10C */ s16              unk_10C;
    /* 0x10E */ u16              unk_10E;
    /* 0x110 */ void*            unk_110;
    /* 0x114 */ void*            unk_114;
    /* 0x118 */ u16              unk_118;
    /* 0x11A */ u16              unk_11A;
    /* 0x11C */ char             unk_11C[0x12C - 0x11C];
    /* 0x12C */ s16              unk_12C;
    /* 0x12E */ s16              unk_12E;
    /* 0x130 */ s16              unk_130;
} BtlFeat01; // Size: 0x134

s32 func_ov004_020ec638(BtlFeat01* feat);
s32 func_ov004_020eccac(BtlFeat01* feat);
s32 func_ov004_020ecb0c(BtlFeat01* feat);
s32 func_ov004_020ecb44(BtlFeat01* feat);
s32 func_ov004_020ecc68(BtlFeat01* feat);
s32 func_ov004_020ecc84(BtlFeat01* feat);
s32 func_ov004_020ed020(BtlFeat01* feat);
s32 func_ov004_020ec790(BtlFeat01* feat);
s32 func_ov004_020ec630(BtlFeat01* feat);
s32 BtlFeat01_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static s32 (*data_ov004_020f0790[])(BtlFeat01*) = {
    func_ov004_020ec638, func_ov004_020eccac, func_ov004_020ecb0c, func_ov004_020ecb44, func_ov004_020ecc68,
    func_ov004_020ecc84, func_ov004_020ed020, func_ov004_020ec790, func_ov004_020ec630,
};

const TaskHandle Tsk_BtlFeat01 = {"Tsk_BtlFeat01", BtlFeat01_RunTask, 0x134};

void func_ov004_020ec510(BtlFeat01* feat) {
    feat->unk_10C++;
    feat->unk_10A = 0;
    feat->unk_12C = 0;
    feat->unk_12E = 0;
    feat->unk_130 = 0;
}

void func_ov004_020ec538(BtlFeat01* feat, s32 (**cbTable)(BtlFeat01*), const char* name) {
    feat->unk_000 = cbTable;
    feat->unk_008 = name;
}

void func_ov004_020ec544(BtlFeat01* feat) {
    if (feat->unk_0E8 != NULL) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], feat->unk_0E8);
        feat->unk_0E8 = NULL;
    }
    if (feat->unk_0F0 != NULL) {
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_MAIN], feat->unk_0F0);
        feat->unk_0F0 = NULL;
    }
    if (feat->unk_0E0 != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_MAIN], feat->unk_0E0);
        feat->unk_0E0 = NULL;
    }
    if (feat->unk_0EC != NULL) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[1], feat->unk_0EC);
        feat->unk_0EC = NULL;
    }
    if (feat->unk_0F4 != NULL) {
        PaletteMgr_ReleaseResource(g_PaletteManagers[1], feat->unk_0F4);
        feat->unk_0F4 = NULL;
    }
    if (feat->unk_0E4 != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[1], feat->unk_0E4);
        feat->unk_0E4 = NULL;
    }
    if (feat->unk_0DC != NULL) {
        DatMgr_ReleaseData(feat->unk_0DC);
        feat->unk_0DC = NULL;
    }
}

s32 func_ov004_020ec630(BtlFeat01* feat) {
    return 1;
}

s32 func_ov004_020ec638(BtlFeat01* feat) {
    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 0x10;
    g_DisplaySettings.engineState[0].blendLayer1 = 0x2F;
    g_DisplaySettings.engineState[1].blendMode   = 1;
    g_DisplaySettings.engineState[1].blendLayer0 = 0x10;
    g_DisplaySettings.engineState[1].blendLayer1 = 0x2F;

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 3, 1, 0x20C);

    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 4);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 3, 1, 0x20C);

    Display_Commit();
    func_0203aafc(3, G2_GetBG1CharPtr(), 0, 0x10000);
    func_0203aafc(3, G2S_GetBG1CharPtr(), 0, 0x10000);
    func_ov004_020ec510(feat);
    return 0;
}

s32 func_ov004_020ec790(BtlFeat01* feat) {
    g_DisplaySettings.controls[0].brightness = 0x10;
    g_DisplaySettings.controls[1].brightness = 0x10;

    Display_CommitSynced();

    g_DisplaySettings.unk_000 = 0;
    REG_POWER_CNT &= ~0x8000;

    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 0, 1, 0, 0x4004);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 2, 3, 1, 0x420C);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 4, 1, 1, 0x404);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[0].layers = 0x1F;

    g_DisplaySettings.controls[1].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_256, 6, 5, 0, 0x4694);
    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_256, 0, 1, 0, 0x4084);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_256, 2, 3, 1, 0x428C);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_256, 4, 5, 1, 0x494);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[1].layers = 0x1E;

    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 1;
    g_DisplaySettings.engineState[0].blendLayer1 = 0x3E;
    g_DisplaySettings.engineState[0].blendCoeff0 = 6;
    g_DisplaySettings.engineState[0].blendCoeff1 = 0xA;

    g_DisplaySettings.engineState[1].blendMode   = 1;
    g_DisplaySettings.engineState[1].blendLayer0 = 1;
    g_DisplaySettings.engineState[1].blendLayer1 = 0x3E;
    g_DisplaySettings.engineState[1].blendCoeff0 = 6;
    g_DisplaySettings.engineState[1].blendCoeff1 = 0xA;

    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.engineState[0].windowObj            = 8;
    g_DisplaySettings.engineState[0].windowObjEffects     = 0;
    g_DisplaySettings.engineState[0].windowOutside        = 0x17;
    g_DisplaySettings.engineState[0].windowOutsideEffects = 0;

    g_DisplaySettings.controls[1].windows |= 4;
    g_DisplaySettings.engineState[1].windowObj            = 8;
    g_DisplaySettings.engineState[1].windowObjEffects     = 0;
    g_DisplaySettings.engineState[1].windowOutside        = 0x17;
    g_DisplaySettings.engineState[1].windowOutsideEffects = 0;

    Display_Commit();
    func_ov004_020ec510(feat);
    return 0;
}

void func_ov004_020eca70(s32 arg0, s32 arg1) {
    s16 phase = (s16)(arg1 >> 1);
    phase %= 3;

    s16 index    = phase + 1;
    s16 strength = index * 0x30;
    if (strength < 90) {
        strength = 90;
    }

    s32 velocity = I2F(strength + RNG_Next(59));
    if (velocity > 0xB4000) {
        velocity = 0xB4000;
    }

    BtlFeat01_Multi_CreateTask(arg0, velocity, (u16)(index % 3));
}

s32 func_ov004_020ecb0c(BtlFeat01* feat) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x800);
    feat->unk_10E = 0;
    feat->unk_11A = 0;
    func_ov004_020ec510(feat);
    return 2;
}

s32 func_ov004_020ecb44(BtlFeat01* feat) {
    if (feat->unk_10E == 0x28) {
        func_ov004_020ec510(feat);
    }

    if (feat->unk_11A != 0) {
        feat->unk_11A--;
        return 0;
    }

    feat->unk_11A = RNG_Next(10);
    if (feat->unk_10E == 5) {
        func_ov003_020bc690(4, 47, 345);
        func_ov003_020bce18(0, 0, 1, 218);
        func_ov003_020bce18(1, 0, 1, 218);
    }
    if (feat->unk_10E == 20) {
        BtlFeat01_BigMulti_CreateTask(0);
        BtlFeat01_BigMulti_CreateTask(1);
        SndMgr_StartPlayingSE(SEIDX_SE_GIRL_SP_LV2_02);
    }
    if (feat->unk_10E == 36) {
        EasyFade_FadeBothDisplays(FADER_LINEAR, 0x10, 0x666);
    }
    if (feat->unk_10E & 1) {
        func_ov004_020eca70(0, (s16)feat->unk_10E);
    } else {
        func_ov004_020eca70(1, (s16)feat->unk_10E);
    }
    feat->unk_10E++;
    return 0;
}

s32 func_ov004_020ecc68(BtlFeat01* feat) {
    feat->unk_118 = 60;
    func_ov004_020ec510(feat);
    return 2;
}

s32 func_ov004_020ecc84(BtlFeat01* feat) {
    if (feat->unk_118 != 0) {
        feat->unk_118--;
    } else {
        func_ov004_020ec510(feat);
    }
    return 0;
}

s32 func_ov004_020eccac(BtlFeat01* feat) {
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].priority = 2;
    g_DisplaySettings.controls[0].layers |= 1;

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 1;
    g_DisplaySettings.controls[0].layers |= 2;

    Display_SetBGOffset(DISPLAY_MAIN, 1, 0, 0x8000);

    g_DisplaySettings.controls[0].layers = (g_DisplaySettings.controls[0].layers & ~4) | 8;

    Display_SetBGOffset(DISPLAY_MAIN, 3, 0, 0);

    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].priority = 1;
    g_DisplaySettings.controls[1].layers |= 2;

    Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG1, 0, 0x8000);

    g_DisplaySettings.controls[1].layers &= ~4;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].priority = 2;
    g_DisplaySettings.controls[1].layers |= 8;

    Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG3, 0, 0);

    feat->unk_0DC = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7c4, 4, 0);

    void* srcChar    = Data_GetPackEntryData(feat->unk_0DC, 5);
    void* srcScreen  = Data_GetPackEntryData(feat->unk_0DC, 6);
    void* srcPalette = Data_GetPackEntryData(feat->unk_0DC, 2);

    feat->unk_0E0 = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], srcChar,
                                         g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].charBase, 0, 0x7B60);
    feat->unk_0E8 = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], srcScreen,
                                         g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].screenBase,
                                         g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].screenSizeText);
    feat->unk_0F0 = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], srcPalette, 0, 0, 2);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], feat->unk_0F0);

    srcChar    = Data_GetPackEntryData(feat->unk_0DC, 3);
    srcScreen  = Data_GetPackEntryData(feat->unk_0DC, 4);
    srcPalette = Data_GetPackEntryData(feat->unk_0DC, 1);

    feat->unk_0E4 = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_SUB], srcChar,
                                         g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].charBase, 0, 0x7B60);
    feat->unk_0EC = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], srcScreen,
                                         g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].screenBase,
                                         g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].screenSizeText);
    feat->unk_0F4 = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], srcPalette, 0, 0, 2);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], feat->unk_0F4);

    feat->unk_0F8 = 0;
    func_ov004_020ec510(feat);
    return 0;
}

s32 func_ov004_020ed020(BtlFeat01* feat) {
    func_ov004_020ec544(feat);
    func_ov004_020ec510(feat);
    return 2;
}

s32 BtlFeat01_Init(TaskPool* pool, Task* task, void* args) {
    BtlFeat01* feat = task->data;

    MI_CpuSet(feat, 0, 0x134);
    feat->unk_110 = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    feat->unk_114 = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
    func_ov004_020ec538(feat, data_ov004_020f0790, "prcset_BtlFeat01");
    return 1;
}

s32 BtlFeat01_Update(TaskPool* pool, Task* task, void* args) {
    BtlFeat01* feat = task->data;

    if (feat->unk_000[feat->unk_10C] == 0) {
        return 0;
    }

    s32 temp_r0;
    do {
        temp_r0 = feat->unk_000[feat->unk_10C](feat);
    } while (temp_r0 == 2);
    if (temp_r0 == 1) {
        return 0;
    }
    feat->unk_0F8 += 0x6000;

    DisplayEngineState* engMain = &g_DisplaySettings.engineState[0];
    s32                 offset  = feat->unk_0F8;

    switch (engMain->bgSettings[3].bgMode) {
        case DISPLAY_BGMODE_AFFINE:
        case DISPLAY_BGMODE_PLTT:
        case DISPLAY_BGMODE_BMP256:
        case DISPLAY_BGMODE_BMPDIRECT:
        case DISPLAY_BGMODE_BMPLARGE:
            engMain->bgAffines[3].unk_14 = 1;
            break;
    }
    engMain->bgOffsets[3].hOffset = offset;
    engMain->bgOffsets[3].vOffset = 0;

    DisplayEngineState* engSub = &g_DisplaySettings.engineState[1];
    offset                     = feat->unk_0F8;

    switch (engSub->bgSettings[3].bgMode) {
        case DISPLAY_BGMODE_AFFINE:
        case DISPLAY_BGMODE_PLTT:
        case DISPLAY_BGMODE_BMP256:
        case DISPLAY_BGMODE_BMPDIRECT:
        case DISPLAY_BGMODE_BMPLARGE:
            engSub->bgAffines[3].unk_14 = 1;
            break;
    }
    engSub->bgOffsets[3].hOffset = offset;
    engSub->bgOffsets[3].vOffset = 0;
    return 1;
}

s32 BtlFeat01_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 BtlFeat01_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlFeat01* feat = task->data;

    func_ov004_020ec544(feat);
    return 0;
}

s32 BtlFeat01_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlFeat01_Init,
        .update     = BtlFeat01_Update,
        .render     = BtlFeat01_Render,
        .cleanup    = BtlFeat01_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}
