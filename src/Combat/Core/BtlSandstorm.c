#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatBgScroll.h"
#include "Display.h"
#include "Engine/Core/DMA.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Math/Random.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"

#include <nitro/g2.h>

// The sandstorm backdrop: a scrolling noise overlay drawn on BG3 of both screens. The task
// configures the two BG3 layers, allocates their char/screen resources and palettes out of one
// raw data file, then scrolls both layers with a random walk.

typedef struct BtlSandstorm {
    /* 0x00 */ Data*            data;
    /* 0x04 */ BgResource*      screen;
    /* 0x08 */ BgResource*      subScreen;
    /* 0x0C */ BgResource*      characters;
    /* 0x10 */ BgResource*      subCharacters;
    /* 0x14 */ PaletteResource* palette;
    /* 0x18 */ PaletteResource* subPalette;
    /* 0x1C */ s32              offsetX;
    /* 0x20 */ s32              offsetY;
} BtlSandstorm; // Size: 0x24

extern BgResource* BgResMgr_AllocScreen(BgResMgr* mgr, void* source, s32 screenBase, s32 screenSize);
extern BgResource* BgResMgr_AllocChar32(BgResMgr* mgr, void* source, u32 charBase, u32 offset, u32 size);
extern BgResource* BgResMgr_AllocCharExtended(BgResMgr* mgr, void* source, s32 charBase, u32 arg3, u32 size);

s32 func_ov003_0209e2cc(TaskPool*, Task*, void*);
s32 func_ov003_0209e584(TaskPool*, Task*, void*);
s32 func_ov003_0209e5a8(TaskPool*, Task*, void*);
s32 func_ov003_0209e678(TaskPool*, Task*, void*);
s32 func_ov003_0209e70c(TaskPool* pool, Task* task, void* args, s32 stage);

char                data_ov003_020e3d50[20] = "Tsk_BtlSandstorm";
char                data_ov003_020e3d64[32] = "Apl_Fur/Grp_BtlFld_Noise.bin";
const BinIdentifier data_ov003_020d81c8     = {3, data_ov003_020e3d64};
const TaskStages    data_ov003_020d81d0     = {
    func_ov003_0209e2cc,
    func_ov003_0209e584,
    func_ov003_0209e5a8,
    func_ov003_0209e678,
};
const TaskHandle data_ov003_020d81e0 = {data_ov003_020e3d50, func_ov003_0209e70c, sizeof(BtlSandstorm)};

s32 func_ov003_0209e2cc(TaskPool* pool, Task* task, void* args) {
    BtlSandstorm*      self = task->data;
    DisplayBGSettings* mbg;
    DisplayBGSettings* sbg;
    void*              mainChar;
    void*              mainScreen;
    void*              mainPal;
    void*              subChar;
    void*              subScreen;
    void*              subPal;

    data_ov003_020e71b8->unk3D878 |= 1;

    mbg                 = &g_DisplaySettings.engineState[0].bgSettings[3];
    mbg->bgMode         = DISPLAY_BGMODE_TEXT;
    mbg->screenSizeText = GX_BG_SIZE_TEXT_256x256;
    mbg->colorMode      = GX_BG_COLORS_16;
    mbg->screenBase     = 4;
    mbg->charBase       = 1;
    mbg->extPlttSlot    = 1;
    G2_SetBG3Control(GX_BG_SIZE_TEXT_256x256, GX_BG_COLORMODE_16, 4, 1, 0);
    mbg->priority = 3;
    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.controls[0].layers |= 8;

    sbg                 = &g_DisplaySettings.engineState[1].bgSettings[3];
    sbg->bgMode         = DISPLAY_BGMODE_TEXT;
    sbg->screenSizeText = GX_BG_SIZE_TEXT_256x256;
    sbg->colorMode      = GX_BG_COLORS_256;
    sbg->screenBase     = 4;
    sbg->charBase       = 5;
    sbg->extPlttSlot    = 1;
    G2S_SetBG3Control(GX_BG_SIZE_TEXT_256x256, 1, 4, 5, 0);
    sbg->priority = 3;
    g_DisplaySettings.controls[1].windows |= 4;
    g_DisplaySettings.controls[1].layers |= 8;

    self->data       = DatMgr_LoadRawData(1, NULL, 0, &data_ov003_020d81c8);
    mainChar         = Data_GetPackEntryData(self->data, 1);
    mainScreen       = Data_GetPackEntryData(self->data, 2);
    mainPal          = Data_GetPackEntryData(self->data, 3);
    self->characters = BgResMgr_AllocChar32(g_BgResourceManagers[0], mainChar,
                                            g_DisplaySettings.engineState[0].bgSettings[3].charBase, 0x3E0, 0x400);
    self->screen =
        BgResMgr_AllocScreen(g_BgResourceManagers[0], mainScreen, g_DisplaySettings.engineState[0].bgSettings[3].screenBase,
                             g_DisplaySettings.engineState[0].bgSettings[3].screenSizeText);
    self->palette = PaletteMgr_AllocPalette(g_PaletteManagers[0], mainPal, 0, 0xF, 1);
    PaletteMgr_Flush(g_PaletteManagers[0], self->palette);
    subChar             = Data_GetPackEntryData(self->data, 4);
    subScreen           = Data_GetPackEntryData(self->data, 5);
    subPal              = Data_GetPackEntryData(self->data, 6);
    self->subCharacters = BgResMgr_AllocCharExtended(g_BgResourceManagers[1], subChar,
                                                     g_DisplaySettings.engineState[1].bgSettings[3].charBase, 0x1E0, 0x800);
    self->subScreen =
        BgResMgr_AllocScreen(g_BgResourceManagers[1], subScreen, g_DisplaySettings.engineState[1].bgSettings[3].screenBase,
                             g_DisplaySettings.engineState[1].bgSettings[3].screenSizeText);
    self->subPalette = PaletteMgr_AllocPalette(g_PaletteManagers[1], subPal, 0, 0xF, 1);
    PaletteMgr_Flush(g_PaletteManagers[1], self->subPalette);

    DMA_Flush();
    DatMgr_ReleaseData(self->data);
    return 1;
}

s32 func_ov003_0209e584(TaskPool* pool, Task* task, void* args) {
    if (!(data_ov003_020e71b8->unk3D878 & 0x2000)) {
        return 1;
    }
    return 0;
}

s32 func_ov003_0209e5a8(TaskPool* pool, Task* task, void* args) {
    BtlSandstorm* self = task->data;

    self->offsetX += (RNG_Next(0x801) - 0x400) << 12;
    self->offsetY += (RNG_Next(0x801) - 0x400) << 12;
    Ov003_ScrollBgById(0, 3, self->offsetX, self->offsetY);
    Ov003_ScrollBgById(1, 3, self->offsetX, self->offsetY);
    return 1;
}

s32 func_ov003_0209e678(TaskPool* pool, Task* task, void* args) {
    BtlSandstorm* self = task->data;

    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], self->screen);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_MAIN], self->characters);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_MAIN], self->palette);
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], self->subScreen);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], self->subCharacters);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], self->subPalette);
    data_ov003_020e71b8->unk3D878 &= ~1;
    return 1;
}

s32 func_ov003_0209e70c(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d81d0;

    return stages.iter[stage](pool, task, args);
}
