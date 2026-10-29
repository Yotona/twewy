#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatBgRes.h"
#include "Combat/Core/CombatBgScroll.h"
#include "Combat/Core/CombatSprite.h"
#include "Display.h"
#include "Engine/Core/DMA.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Resources/ScreenMapper.h"
#include "Save.h"
#include "SpriteMgr.h"

#include <nitro/g2.h>
#include <nitro/mi/cpumem.h>

// The sub-screen field background of a battle (`Tsk_BtlField_RG`): the twin of `BtlField_UG`. One BG tilemap driven by
// the camera, two floor strips that scroll over it and, for some backgrounds, a third strip with its own characters
// on BG0.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The 0x1C-byte entry of `data_ov003_020d9c90` that describes one background of the sub screen.
typedef struct BtlFieldDefRG {
    /* 0x00 */ BinIdentifier*      iden;     // pack the background is cut from
    /* 0x04 */ s32                 groundY;  // world Y of the floor
    /* 0x08 */ const Ov003ResCmd** scripts;  // resource script of each battle index
    /* 0x0C */ u8                  charPack; // pack indices inside the entry's data
    /* 0x0D */ u8                  rowPack;
    /* 0x0E */ u8                  paletteIndex;
    /* 0x0F */ u8                  palCount;
    /* 0x10 */ u8                  charIndexA;
    /* 0x11 */ u8                  charIndexB;
    /* 0x12 */ u16                 charCountA;
    /* 0x14 */ u16                 charCountB;
    /* 0x16 */ u8                  rowIndex0;
    /* 0x17 */ u8                  rowIndex1;
    /* 0x18 */ u8                  scrollRows;
    /* 0x19 */ u8                  extraIndex; // 0xFF when the background has no third strip
    /* 0x1A */ u8                  unk_1A[2];
} BtlFieldDefRG;                               // Size: 0x1C

/// The characters and tilemaps of the third strip.
typedef struct BtlFieldExtra {
    /* 0x0 */ u16 charPack;
    /* 0x2 */ u16 charCount;
    /* 0x4 */ u16 rowIndex0;
    /* 0x6 */ u16 rowIndex1;
} BtlFieldExtra; // Size: 0x8

struct BtlFieldRG;

typedef struct BtlFieldRG {
    /* 0x00 */ Ov003Camera*     camera;
    /* 0x04 */ Data*            data;
    /* 0x08 */ PaletteResource* palette;
    /* 0x0C */ BgResource*      charResA;
    /* 0x10 */ BgResource*      charResB;
    /* 0x14 */ ScreenMapEntry   rowMaps[2];
    /* 0x64 */ const void*      rowData[2];
    /* 0x6C */ s32              hasExtra; // the third strip is in use
    /* 0x70 */ BgResource*      extraChars;
    /* 0x74 */ ScreenMapEntry   extraMap;
    /* 0x9C */ const void*      extraData[2];
    /* 0xA4 */ s16              timer; // fade-in timer, 0 = idle
    /* 0xA6 */ s16              unk_A6;
    /* 0xA8 */ void (*update)(struct BtlFieldRG* self);
    /* 0xAC */ Ov003BgResState baseMap;
} BtlFieldRG; // Size: 0x138

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

extern const BtlFieldDefRG data_ov003_020d9c90[];
extern const BtlFieldExtra data_ov003_020d9afc[];

extern BgResource* BgResMgr_AllocCharExtended(BgResMgr* mgr, void* source, u32 charBase, u32 offset, u32 size);
extern void        func_0200d858(ScreenMapEntry* entry, s32 wrapH, s32 wrapV, void* defaultTile);
extern void        func_0200d954(s32 engine, s32 bg);
extern void        func_02026590(void* dst, s32 step, u16 count);
extern void        func_ov003_02083300(Ov003BgResState* state, s32 engine, BinIdentifier* iden);
extern void        func_ov003_0208335c(Ov003BgResState* state, const Ov003ResCmd* script, void* arg);
extern void        func_ov003_02083390(Ov003BgResState* state);
extern void        func_ov003_020833cc(Ov003BgResState* state);

void func_ov003_020929a4(BtlFieldRG* state, void (*update)(BtlFieldRG*));
void func_ov003_02092c10(BtlFieldRG* state);
void func_ov003_02092c88(Ov003Camera* camera);
void func_ov003_02092dbc(BtlFieldRG* state);

s32 func_ov003_02092f30(TaskPool*, Task*, void*);
s32 func_ov003_0209327c(TaskPool*, Task*, void*);
s32 func_ov003_020932d0(TaskPool*, Task*, void*);
s32 func_ov003_020932e4(TaskPool*, Task*, void*);

s32 func_ov003_02093394(TaskPool*, Task*, void*, s32);

/// `TaskHandle.taskName` of `data_ov003_020d7b4c`.
char data_ov003_020e3b18[0x10] = "Tsk_BtlField_RG";

/// `Tsk_BtlField_RG`.
const TaskHandle data_ov003_020d7b4c = {data_ov003_020e3b18, func_ov003_02093394, 0x138};

/// The four stage functions of the field background task.
const TaskStages data_ov003_020d7b58 = {
    func_ov003_02092f30,
    func_ov003_0209327c,
    func_ov003_020932d0,
    func_ov003_020932e4,
};

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

/// Resets the fade timer and swaps in the background's scroll-update callback.
void func_ov003_020929a4(BtlFieldRG* state, void (*update)(BtlFieldRG*)) {
    state->update = update;
    state->unk_A6 = 0;
    state->timer  = 0;
}

/// Sets BG1 and BG2 of the sub engine up as the two text layers the floor strips are drawn on.
void func_ov003_020929b8(void) {
    DisplayBGSettings* bg;

    bg                 = &g_DisplaySettings.engineState[1].bgSettings[1];
    bg->bgMode         = DISPLAY_BGMODE_TEXT;
    bg->screenSizeText = GX_BG_SIZE_TEXT_512x256;
    bg->colorMode      = GX_BG_COLORS_256;
    bg->screenBase     = 0;
    bg->charBase       = 1;
    bg->extPlttSlot    = 0;
    G2S_SetBG1Control(GX_BG_SIZE_TEXT_512x256, 1, GX_BG_SCRBASE_0x0000, 1, 0);

    bg                 = &g_DisplaySettings.engineState[1].bgSettings[2];
    bg->bgMode         = DISPLAY_BGMODE_TEXT;
    bg->screenSizeText = GX_BG_SIZE_TEXT_512x256;
    bg->colorMode      = GX_BG_COLORS_256;
    bg->screenBase     = 2;
    bg->charBase       = 3;
    bg->extPlttSlot    = 1;
    G2S_SetBG2Control(GX_BG_SIZE_TEXT_512x256, 1, 2, 3, 0);

    g_DisplaySettings.engineState[1].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.controls[1].layers |= 6;
}

/// Starts the scroll script that the save's battle index picks (the first one if it is out of range).
void func_ov003_02092a5c(BtlFieldRG* state, const Ov003ResCmd** scripts, u16 count) {
    u16 index = gSaveData.unk_3232;

    if (scripts == NULL)
        return;
    if (index >= count)
        index = 0;
    func_ov003_0208335c(&state->baseMap, scripts[index], state->data);
}

/// Loads the characters of the third strip. Returns NULL when the background has none.
BgResource* func_ov003_02092a90(BtlFieldRG* state, u8 index) {
    BgResource* result = NULL;

    if (index != 0xFF) {
        const BtlFieldExtra* extra = &data_ov003_020d9afc[index];
        void*                chars = Data_GetPackEntryData(state->data, extra->charPack);

        result = BgResMgr_AllocCharExtended(g_BgResourceManagers[1], chars,
                                            g_DisplaySettings.engineState[1].bgSettings[0].charBase, 0, extra->charCount);
    }
    return result;
}

/// Sets the third strip up on BG0. Returns 0 when the background has none.
s32 func_ov003_02092b14(BtlFieldRG* state, u8 index) {
    const BtlFieldExtra* extra;

    if (index == 0xFF) {
        return 0;
    }
    extra               = &data_ov003_020d9afc[index];
    state->extraData[0] = (u8*)Data_GetPackEntryData(state->data, extra->rowIndex0) + 4;
    state->extraData[1] = (u8*)Data_GetPackEntryData(state->data, extra->rowIndex1) + 4;
    func_0200d1d8(&state->extraMap, 1, 0, NULL, state->extraData, 2, 1);
    func_0200d858(&state->extraMap, 0, 1, NULL);
    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.controls[1].layers |= 1;
    return 1;
}

/// Fades the background in with the battle's transition, then hands the task to the live update.
void func_ov003_02092c10(BtlFieldRG* state) {
    if (state->timer == 0) {
        state->camera->shakeX = -0x48000;
        state->timer          = 100;
    }
    if (state->timer > 0) {
        func_02026590(&state->camera->shakeX, 0, (u16)state->timer);
        state->timer--;
    }
    if (state->timer > 0) {
        return;
    }
    func_ov003_020929a4(state, NULL);
}

/// Eases the camera's view point toward its target, keeping it inside the floor's bounds.
void func_ov003_02092c88(Ov003Camera* camera) {
    s32 dx;
    s32 dy;
    s32 dz;
    s32 xMin, xMax;

    dx = (camera->unk_34 - camera->unk_28) >> 2;
    dy = (camera->unk_38 - camera->unk_2C) >> 2;
    dz = (camera->unk_3C - camera->unk_30) >> 2;

    if (dx > 0x2000) {
        dx = 0x2000;
    } else if (dx < -0x2000) {
        dx = -0x2000;
    }
    if (dy > 0x2000) {
        dy = 0x2000;
    } else if (dy < -0x2000) {
        dy = -0x2000;
    }
    if (dz > 0x2000) {
        dz = 0x2000;
    } else if (dz < -0x2000) {
        dz = -0x2000;
    }

    camera->unk_28 += dx;
    camera->unk_2C += dy;
    camera->unk_30 += dz;
    camera->unk_40 = camera->shakeX + camera->unk_2C + camera->unk_30;

    xMin = camera->unk_04 + 0x80000;
    xMax = camera->unk_04 - 0x80000 + camera->unk_0C;
    if (camera->unk_28 < xMin) {
        camera->unk_28 = xMin;
    } else if (camera->unk_28 > xMax) {
        camera->unk_28 = xMax;
    }
    if (camera->unk_40 < 0x178000) {
        camera->unk_40 = 0x178000;
    } else if (camera->unk_40 > 0x198000) {
        camera->unk_40 = 0x198000;
    }
    camera->unk_28 = camera->unk_28 + camera->unk_44;
    camera->unk_40 += camera->unk_48 + camera->unk_4C;
}

/// Scrolls the BG of `map` to the camera's position, shifted by `offsetX`/`offsetY`.
static inline void BtlField_ScrollRow(ScreenMapEntry* map, Ov003Camera* camera, s32 offsetX, s32 offsetY) {
    s32                 layer  = map->bgLayer;
    DisplayEngineState* engine = &g_DisplaySettings.engineState[map->engineId];
    s32                 posY   = camera->unk_40 - offsetY;
    s32                 posX   = camera->unk_28 - offsetX;

    Ov003_ScrollBg(engine, layer, posX, posY);
}

/// Parks the strips behind the camera.
void func_ov003_02092dbc(BtlFieldRG* state) {
    BtlField_ScrollRow(&state->rowMaps[0], state->camera, 0x80000, 0x60000);
    BtlField_ScrollRow(&state->rowMaps[1], state->camera, 0x180000, 0x60000);
    if (state->hasExtra == 0) {
        return;
    }
    BtlField_ScrollRow(&state->extraMap, state->camera, 0x80000, 0x60000);
}

/// Task stage 0: fetches the background's resources and seeds the tilemaps.
s32 func_ov003_02092f30(TaskPool* pool, Task* task, void* args) {
    BtlFieldRG*          state = task->data;
    const BtlFieldDefRG* def;
    void*                palData;
    void*                charDataA;
    void*                charDataB;

    MI_CpuSet(state, NULL, sizeof(*state));
    func_ov003_020929b8();
    state->camera = &data_ov003_020e71b8->unk3D7C0[1];
    state->camera->unk_02 |= 1;

    def = &data_ov003_020d9c90[state->camera->pathId];
    func_ov003_02083300(&state->baseMap, 1, def->iden);

    state->data = DatMgr_LoadPackEntry(1, NULL, 0, def->iden, def->charPack, 0);
    palData     = Data_GetPackEntryData(state->data, def->paletteIndex);
    charDataA   = Data_GetPackEntryData(state->data, def->charIndexA);
    charDataB   = Data_GetPackEntryData(state->data, def->charIndexB);

    state->palette    = PaletteMgr_AllocPalette(g_PaletteManagers[1], palData, 0, def->palCount, 0);
    state->charResA   = BgResMgr_AllocCharExtended(g_BgResourceManagers[1], charDataA,
                                                   g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, def->charCountA);
    state->charResB   = BgResMgr_AllocCharExtended(g_BgResourceManagers[1], charDataB,
                                                   g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, def->charCountB);
    state->extraChars = func_ov003_02092a90(state, def->extraIndex);
    PaletteMgr_Flush(g_PaletteManagers[1], state->palette);
    DMA_Flush();

    func_ov003_02092a5c(state, def->scripts, def->scrollRows);
    DatMgr_ReleaseData(state->data);

    state->data = DatMgr_LoadPackEntry(1, NULL, 0, def->iden, def->rowPack, 0);
    // The strips start after the 4-byte header of their pack entries.
    state->rowData[0] = (u8*)Data_GetPackEntryData(state->data, def->rowIndex0) + 4;
    state->rowData[1] = (u8*)Data_GetPackEntryData(state->data, def->rowIndex1) + 4;

    func_0200d1d8(&state->rowMaps[0], 1, 1, NULL, &state->rowData[0], 1, 1);
    func_0200d1d8(&state->rowMaps[1], 1, 2, NULL, &state->rowData[1], 1, 1);
    func_0200d858(&state->rowMaps[0], 0, 1, NULL);
    func_0200d858(&state->rowMaps[1], 0, 1, NULL);
    state->hasExtra = func_ov003_02092b14(state, def->extraIndex);

    if (*(u16*)args == 0) {
        state->camera->unk_28 = state->camera->unk_34;
        state->camera->unk_2C = state->camera->unk_38;
        state->camera->unk_30 = state->camera->unk_3C;
        func_ov003_020929a4(state, func_ov003_02092c10);
    } else {
        state->camera->shakeX = 0;
        func_ov003_020929a4(state, NULL);
    }
    func_ov003_02092dbc(state);
    return 1;
}

/// Task stage 1: advances the background every frame.
s32 func_ov003_0209327c(TaskPool* pool, Task* task, void* args) {
    BtlFieldRG* state = task->data;

    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 0;
    }
    if (state->update != NULL) {
        state->update(state);
    }
    func_ov003_02083390(&state->baseMap);
    func_ov003_02092c88(state->camera);
    return 1;
}

/// Task stage 2: re-parks the background behind the camera.
s32 func_ov003_020932d0(TaskPool* pool, Task* task, void* args) {
    func_ov003_02092dbc(task->data);
    return 1;
}

/// Task stage 3: releases the background's resources.
s32 func_ov003_020932e4(TaskPool* pool, Task* task, void* args) {
    BtlFieldRG* state = task->data;

    BgResMgr_ReleaseChar(g_BgResourceManagers[1], state->charResA);
    BgResMgr_ReleaseChar(g_BgResourceManagers[1], state->charResB);
    PaletteMgr_ReleaseResource(g_PaletteManagers[1], state->palette);
    func_0200d954(1, 1);
    func_0200d954(1, 2);
    func_ov003_020833cc(&state->baseMap);
    if (state->extraChars != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[1], state->extraChars);
    }
    if (state->hasExtra != 0) {
        func_0200d954(1, 0);
    }
    DatMgr_ReleaseData(state->data);
    state->camera->unk_02 &= ~1;
    return 1;
}

/// Task dispatcher: runs `stage` of the field background's stage table.
s32 func_ov003_02093394(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7b58;

    return stages.iter[stage](pool, task, args);
}
