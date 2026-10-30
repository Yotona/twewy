#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatBgRes.h"
#include "Combat/Core/CombatCutin.h"
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
#include <nitro/math.h>
#include <nitro/mi/cpumem.h>

// The main-screen field background of a battle: one BG tilemap driven by the camera, and the floor
// rows that scroll over it. The sub-screen twin of this record lives in a sibling unit and builds
// its resources from the same `DatMgr_LoadPackEntry` recipe.

/// The 0x1C-byte entry of `data_ov003_020da2bc` that describes one background of the main screen.
typedef struct BtlFieldDef {
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
    /* 0x18 */ u8                  rowIndex2;
    /* 0x19 */ u8                  rowIndex3;
    /* 0x1A */ u8                  scrollRows;
    /* 0x1B */ u8                  unk_1B;
} BtlFieldDef; // Size: 0x1C

/// The task data of the field background: the camera it follows, its resources, the two floor strips and the
/// resource script state (`baseMap`) that drives the tilemap.
typedef struct BtlFieldUG {
    /* 0x00 */ Ov003Camera*     camera;
    /* 0x04 */ Data*            data;
    /* 0x08 */ BgResource*      charResA;
    /* 0x0C */ BgResource*      charResB;
    /* 0x10 */ PaletteResource* palette;
    /* 0x14 */ ScreenMapEntry   rowMaps[2];
    /* 0x64 */ const void*      rowData[4]; // graphic data of each of the four floor strips
    /* 0x74 */ s16              timer;      // fade-in timer, 0 = idle
    /* 0x76 */ s16              unk_76;
    /* 0x78 */ void (*update)(struct BtlFieldUG* self);
    /* 0x7C */ Ov003BgResState baseMap;
} BtlFieldUG; // Size: 0x108

// External data.

extern const BtlFieldDef data_ov003_020da2bc[];

// External functions.

extern void func_0200d858(ScreenMapEntry* entry, s32 wrapH, s32 wrapV, void* defaultTile);
extern void func_0200d954(s32 engine, s32 bg);
extern void func_02026590(s32* dst, s32 target, u16 weight);
extern void func_ov003_02083300(Ov003BgResState* state, s32 engine, BinIdentifier* iden);
extern void func_ov003_0208335c(Ov003BgResState* state, const Ov003ResCmd* script, void* arg);
extern void func_ov003_02083390(Ov003BgResState* state);
extern void func_ov003_020833cc(Ov003BgResState* state);
extern void func_ov003_0209409c(s32 bgIndex);

// Defined in this unit.

void func_ov003_020933dc(BtlFieldUG* state, void (*update)(BtlFieldUG*));
void func_ov003_020933f0(void);
void func_ov003_02093494(BtlFieldUG* state, const Ov003ResCmd** scripts, u16 count);
void func_ov003_020934c8(Ov003Camera* camera);
void func_ov003_02093608(BtlFieldUG* state);
void func_ov003_020936fc(BtlFieldUG* state);

s32 func_ov003_0209378c(TaskPool* pool, Task* task, void* args);
s32 func_ov003_02093b40(TaskPool* pool, Task* task, void* args);
s32 func_ov003_02093b94(TaskPool* pool, Task* task, void* args);
s32 func_ov003_02093ba8(TaskPool* pool, Task* task, void* args);

s32 func_ov003_02093c28(TaskPool* pool, Task* task, void* args, s32 stage);

/// `TaskHandle.taskName` of `data_ov003_020d7b68`.
char data_ov003_020e3b28[0x10] = "Tsk_BtlField_UG";

/// The four stage functions of the field background task.
const TaskStages data_ov003_020d7b74 = {
    func_ov003_0209378c,
    func_ov003_02093b40,
    func_ov003_02093b94,
    func_ov003_02093ba8,
};

/// `Tsk_BtlField_UG`.
const TaskHandle data_ov003_020d7b68 = {data_ov003_020e3b28, func_ov003_02093c28, 0x108};

/// Resets the fade timer and swaps in the background's scroll-update callback.
void func_ov003_020933dc(BtlFieldUG* state, void (*update)(BtlFieldUG*)) {
    state->update = update;
    state->unk_76 = 0;
    state->timer  = 0;
}

/// Sets BG1 and BG2 of the main engine up as the two text layers the floor strips are drawn on.
void func_ov003_020933f0(void) {
    DisplayBGSettings* bg;

    bg                 = &g_DisplaySettings.engineState[0].bgSettings[1];
    bg->bgMode         = DISPLAY_BGMODE_TEXT;
    bg->screenSizeText = GX_BG_SIZE_TEXT_512x256;
    bg->colorMode      = GX_BG_COLORS_16;
    bg->screenBase     = 0;
    bg->charBase       = 1;
    bg->extPlttSlot    = 0;
    G2_SetBG1Control(GX_BG_SIZE_TEXT_512x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, 1, 0);

    bg                 = &g_DisplaySettings.engineState[0].bgSettings[2];
    bg->bgMode         = DISPLAY_BGMODE_TEXT;
    bg->screenSizeText = GX_BG_SIZE_TEXT_512x256;
    bg->colorMode      = GX_BG_COLORS_16;
    bg->screenBase     = 2;
    bg->charBase       = 3;
    bg->extPlttSlot    = 1;
    G2_SetBG2Control(GX_BG_SIZE_TEXT_512x256, GX_BG_COLORMODE_16, 2, 3, 0);

    g_DisplaySettings.engineState[0].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.controls[0].layers |= 6;
}

/// Starts the scroll script of `rowData` that the save's battle index picks (the first one if it is out of range).
void func_ov003_02093494(BtlFieldUG* state, const Ov003ResCmd** scripts, u16 count) {
    u16 index = gSaveData.unk_3232;

    if (scripts == NULL)
        return;
    if (index >= count)
        index = 0;
    func_ov003_0208335c(&state->baseMap, scripts[index], state->data);
}

/// Eases the camera's view point toward its target (ignoring drifts that are too small to see), keeping it inside the
/// floor's bounds.
void func_ov003_020934c8(Ov003Camera* camera) {
    s32 dx;
    s32 dy;
    s32 dz;
    s32 mag;
    s32 xMin, xMax, yMin, yMax;

    dx = (camera->unk_34 - camera->unk_28) >> 5;
    dy = (camera->unk_38 - camera->unk_2C) >> 5;
    dz = (camera->unk_3C - camera->unk_30) >> 2;

    mag = ABS(dx);
    if (mag < 0x666)
        dx = 0;
    mag = ABS(dy);
    if (mag < 0x666)
        dy = 0;
    mag = ABS(dz);
    if (mag < 0x666)
        dz = 0;

    camera->unk_28 += dx;
    camera->unk_2C += dy;
    camera->unk_30 += dz;
    camera->unk_40 = camera->shakeX + camera->unk_2C + camera->unk_30;

    xMin = camera->unk_04 + 0x80000;
    xMax = camera->unk_04 - 0x80000 + camera->unk_0C;
    yMin = camera->unk_08 + 0x60000 + camera->posY;
    yMax = camera->unk_10 + (camera->unk_08 - 0x50000);
    if (camera->unk_28 < xMin) {
        camera->unk_28 = xMin;
    } else if (camera->unk_28 > xMax) {
        camera->unk_28 = xMax;
    }
    if (camera->unk_40 < yMin) {
        camera->unk_40 = yMin;
    } else if (camera->unk_40 > yMax) {
        camera->unk_40 = yMax;
    }
    camera->unk_28 = camera->unk_28 + camera->unk_44;
    camera->unk_40 += camera->unk_48 + camera->unk_4C;
}

/// Parks the two floor strips behind the camera: the near one first, then the far one.
void func_ov003_02093608(BtlFieldUG* state) {
    BtlField_ScrollRow(&state->rowMaps[0], state->camera, 0x80000, 0x60000);
    BtlField_ScrollRow(&state->rowMaps[1], state->camera, 0x180000, 0x60000);
}

/// Fades the background in with the battle's transition, then hands the task to the live update.
void func_ov003_020936fc(BtlFieldUG* state) {
    if (state->timer == 0) {
        if (state->camera->pathId == 2 || state->camera->pathId == 30) {
            state->camera->shakeX /= 4;
        }
        state->timer = 100;
    }
    if (state->timer > 0) {
        func_02026590(&state->camera->shakeX, 0, (u16)state->timer);
        state->timer--;
    }
    if (state->timer > 0) {
        return;
    }
    func_ov003_020933dc(state, NULL);
}

/// Task stage 0: fetches the background's resources and seeds the tilemaps.
s32 func_ov003_0209378c(TaskPool* pool, Task* task, void* args) {
    BtlFieldUG*        state = task->data;
    const BtlFieldDef* def;
    void*              palData;
    void*              charDataA;
    void*              charDataB;

    MI_CpuSet(state, NULL, sizeof(*state));
    func_ov003_020933f0();
    state->camera = &data_ov003_020e71b8->unk3D7C0[0];
    state->camera->unk_02 |= 1;

    def = &data_ov003_020da2bc[state->camera->pathId];
    func_ov003_02083300(&state->baseMap, 0, def->iden);

    state->data = DatMgr_LoadPackEntry(1, NULL, 0, def->iden, def->charPack, 0);
    palData     = Data_GetPackEntryData(state->data, def->paletteIndex);
    charDataA   = Data_GetPackEntryData(state->data, def->charIndexA);
    charDataB   = Data_GetPackEntryData(state->data, def->charIndexB);

    state->palette  = PaletteMgr_AllocPalette(g_PaletteManagers[0], palData, 0, def->palCount, 0);
    state->charResA = BgResMgr_AllocChar32(g_BgResourceManagers[0], charDataA,
                                           g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, def->charCountA);
    state->charResB = BgResMgr_AllocChar32(g_BgResourceManagers[0], charDataB,
                                           g_DisplaySettings.engineState[0].bgSettings[2].charBase, 0, def->charCountB);
    PaletteMgr_Flush(g_PaletteManagers[0], state->palette);
    DMA_Flush();

    func_ov003_02093494(state, def->scripts, def->scrollRows);
    DatMgr_ReleaseData(state->data);

    state->data = DatMgr_LoadPackEntry(1, NULL, 0, def->iden, def->rowPack, 0);
    // The strips start after the 4-byte header of their pack entries.
    state->rowData[0] = Ov003Cutin_Tiles(state->data, def->rowIndex0);
    state->rowData[1] = Ov003Cutin_Tiles(state->data, def->rowIndex1);
    state->rowData[2] = Ov003Cutin_Tiles(state->data, def->rowIndex2);
    state->rowData[3] = Ov003Cutin_Tiles(state->data, def->rowIndex3);

    func_0200d1d8(&state->rowMaps[0], 0, 1, NULL, state->rowData, 1, 2);
    func_0200d1d8(&state->rowMaps[1], 0, 2, NULL, &state->rowData[2], 1, 2);
    func_0200d858(&state->rowMaps[0], 0, 0, NULL);
    func_0200d858(&state->rowMaps[1], 0, 0, NULL);

    if (*(u16*)args == 0) {
        func_ov003_0209409c(state->camera->pathId);
        state->camera->unk_28 = state->camera->unk_44;
        state->camera->unk_2C = state->camera->unk_48;
        state->camera->unk_30 = state->camera->unk_4C;
        func_ov003_020933dc(state, func_ov003_020936fc);
    } else {
        state->camera->shakeX = 0;
        func_ov003_020933dc(state, NULL);
    }
    func_ov003_02093608(state);
    return 1;
}

/// Task stage 1: advances the background every frame.
s32 func_ov003_02093b40(TaskPool* pool, Task* task, void* args) {
    BtlFieldUG* state = task->data;

    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 0;
    }
    if (state->update != NULL) {
        state->update(state);
    }
    func_ov003_02083390(&state->baseMap);
    func_ov003_020934c8(state->camera);
    return 1;
}

/// Task stage 2: re-parks the background behind the camera.
s32 func_ov003_02093b94(TaskPool* pool, Task* task, void* args) {
    func_ov003_02093608(task->data);
    return 1;
}

/// Task stage 3: releases the background's resources.
s32 func_ov003_02093ba8(TaskPool* pool, Task* task, void* args) {
    BtlFieldUG* state = task->data;

    BgResMgr_ReleaseChar(g_BgResourceManagers[0], state->charResA);
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], state->charResB);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], state->palette);
    func_0200d954(0, 1);
    func_0200d954(0, 2);
    func_ov003_020833cc(&state->baseMap);
    DatMgr_ReleaseData(state->data);
    state->camera->unk_02 &= ~1;
    return 1;
}

/// Task dispatcher: runs `stage` of the field background's stage table.
s32 func_ov003_02093c28(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7b74;

    return stages.iter[stage](pool, task, args);
}
