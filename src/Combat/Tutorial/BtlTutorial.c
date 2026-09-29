#include "Combat/Tutorial/BtlTutorial.h"
#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Display.h"
#include "Engine/Core/DMA.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "common_data.h"

#include <nitro/mi/cpumem.h>

extern void func_ov003_02083ab0(s32, s32, s32, s32);
extern s32  func_ov003_0208b690(u16);

extern void*       BgResMgr_AllocCharExtended(BgResMgr* mgr, void* source, s32 charBase, u32 arg3, u32 size);
extern BgResource* BgResMgr_AllocScreen(BgResMgr* mgr, void* source, s32 screenBase, GXBGScreenSizeText screenSize);

// Task stage handlers (forward declared for the TaskStages tables below).
s32 func_ov008_020e7374(TaskPool*, Task*, void*);
s32 func_ov008_020e742c(TaskPool*, Task*, void*);
s32 func_ov008_020e7464(TaskPool*, Task*, void*);
s32 func_ov008_020e746c(TaskPool*, Task*, void*);
s32 func_ov008_020e7474(TaskPool*, Task*, void*, s32);
s32 func_ov008_020e769c(TaskPool*, Task*, void*);
s32 func_ov008_020e78e0(TaskPool*, Task*, void*);
s32 func_ov008_020e78f8(TaskPool*, Task*, void*);
s32 func_ov008_020e7910(TaskPool*, Task*, void*);
s32 func_ov008_020e799c(TaskPool*, Task*, void*, s32);

void func_ov008_020e7360(BtlPlayerNone*, void (*)(BtlPlayerNone*));
void func_ov008_020e74dc(void);
s32  func_ov008_020e7580(void);
void func_ov008_020e75f8(CombatSprite*, s32);

// MARK: Data

char data_ov008_020e7a80[]  = "Tsk_BtlPlayerNone";
u16  data_ov008_020e7a94[6] = {1, 2, 2, 2, 4, 3};
char data_ov008_020e7aa0[]  = "Tsk_BtlTutorial";
char data_ov008_020e7ab0[]  = "Apl_Fur/Grp_Tutorial.bin";

const TaskHandle Tsk_BtlPlayerNone = {data_ov008_020e7a80, func_ov008_020e7474, 0x84};

static const TaskStages data_ov008_020e7a10 = {
    .initialize = func_ov008_020e7374,
    .update     = func_ov008_020e742c,
    .render     = func_ov008_020e7464,
    .cleanup    = func_ov008_020e746c,
};

/// `Tsk_BtlTutorial` asset pack identifier.
static const BinIdentifier data_ov008_020e7a20 = {8, data_ov008_020e7ab0};

static const TaskStages data_ov008_020e7a28 = {
    .initialize = func_ov008_020e769c,
    .update     = func_ov008_020e78e0,
    .render     = func_ov008_020e78f8,
    .cleanup    = func_ov008_020e7910,
};

/// Pack indices/palette index and sizes for each tutorial variant.
static const BtlTutorialVariant data_ov008_020e7a38[6] = {
    { 3, 3, 1, 2, 0x6400, 0x6400},
    { 4, 3, 1, 2, 0x6400, 0x6400},
    { 5, 3, 1, 2, 0x6400, 0x6400},
    { 8, 3, 1, 2, 0x6400, 0x6400},
    { 9, 3, 1, 2, 0x6400, 0x6400},
    {10, 3, 1, 2, 0x6400, 0x6400},
};

const TaskHandle Tsk_BtlTutorial = {data_ov008_020e7aa0, func_ov008_020e799c, 0x7C};

// MARK: Functions

void func_ov008_020e7360(BtlPlayerNone* data, void (*callback)(BtlPlayerNone*)) {
    data->unk_7C = callback;
    data->unk_80 = 0;
    data->unk_82 = 0;
}

s32 func_ov008_020e7374(TaskPool* pool, Task* task, void* args) {
    BtlPlayerNone* data = task->data;

    MI_CpuSet(data, 0, sizeof(BtlPlayerNone));
    CombatActor_Init(&data->actor, 0);
    data_ov003_020e71b8->unk3D89C = data;
    data->actor.isFlipped         = FALSE;
    data->actor.position.x        = data_ov003_020e71b8->unk3D838;
    data->actor.position.y        = data_ov003_020e71b8->unk3D83C;
    data->actor.position.z        = 0;
    data->actor.zGravity          = 0x800;
    data->actor.unk_70            = 0xC;
    data->actor.unk_72            = 0x30;
    data->actor.unk_76            = 0x18;
    data->actor.flags |= 0x10;
    func_ov003_02083ab0(1, data->actor.position.x, data->actor.position.y, data->actor.position.z);
    func_ov008_020e7360(data, NULL);
    return 1;
}

s32 func_ov008_020e742c(TaskPool* pool, Task* task, void* args) {
    BtlPlayerNone* data = task->data;

    if (data->unk_7C != NULL) {
        data->unk_7C(data);
    }
    CombatActor_UpdateEffects(1, &data->actor);
    CombatActor_UpdatePhysics(&data->actor);
    return 1;
}

s32 func_ov008_020e7464(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov008_020e746c(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov008_020e7474(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov008_020e7a10;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

void func_ov008_020e74dc(void) {
    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, 1, 1, 0, 1, 0, 0x4084);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, 1, 1, 2, 3, 1, 0x428C);
    Display_GetBG1Settings(DISPLAY_SUB)->priority = 2;
    Display_GetBG2Settings(DISPLAY_SUB)->priority = 2;
    g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2;
}

s32 func_ov008_020e7580(void) {
    s32 result = 0;

    switch (*(u16*)((u8*)data_ov003_020e71b8 + 0x3D800 + 0x8E) - 0x32) {
        case 0:
            result = 0;
            break;
        case 1:
            result = 1;
            break;
        case 2:
            result = 2;
            break;
        case 6:
            result = 3;
            break;
        case 7:
            result = 4;
            break;
        case 8:
            result = 5;
            break;
    }
    return result;
}

void func_ov008_020e75f8(CombatSprite* cSprite, s32 variant) {
    SpriteAnimationEx anim;

    CombatSprite_InitAnim(&anim.anim, 1, &data_ov008_020e7a20);
    anim.anim.unk_18     = 2;
    anim.anim.packIndex  = 1;
    anim.anim.bits_10_11 = 0;
    anim.anim.unk_1C     = 3;
    anim.anim.unk_26     = 4;
    anim.anim.unk_28     = 5;
    anim.anim.unk_20     = 6;
    anim.anim.animIndex  = data_ov008_020e7a94[variant] + 1;
    anim.unk_2C          = 0;
    CombatSprite_Load(cSprite, &anim);
    CombatSprite_SetPosition(cSprite, 0x80, 0x60);
}

s32 func_ov008_020e769c(TaskPool* pool, Task* task, void* args) {
    BtlTutorial* data = task->data;

    MI_CpuSet(data, 0, sizeof(BtlTutorial));
    func_ov008_020e74dc();
    data_ov003_020e71b8->unk3D81A |= 1;

    s32                       variant = func_ov008_020e7580();
    Data*                     pack1;
    Data*                     pack2;
    const BtlTutorialVariant* entry = &data_ov008_020e7a38[variant];

    pack1 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov008_020e7a20, entry->unk_00, FALSE);
    pack2 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov008_020e7a20, 1, FALSE);

    void* char1   = Data_GetPackEntryData(pack1, entry->unk_02);
    void* screen1 = Data_GetPackEntryData(pack2, 1);

    data->unk_00 = BgResMgr_AllocCharExtended(g_BgResourceManagers[DISPLAY_SUB], char1,
                                              g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, entry->unk_04);
    data->unk_08 = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], screen1,
                                        g_DisplaySettings.engineState[1].bgSettings[1].screenBase,
                                        g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText);

    void* char2   = Data_GetPackEntryData(pack1, entry->unk_03);
    void* screen2 = Data_GetPackEntryData(pack2, 2);

    data->unk_04 = BgResMgr_AllocCharExtended(g_BgResourceManagers[DISPLAY_SUB], char2,
                                              g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, entry->unk_06);
    data->unk_0C = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], screen2,
                                        g_DisplaySettings.engineState[1].bgSettings[2].screenBase,
                                        g_DisplaySettings.engineState[1].bgSettings[2].screenSizeText);

    data->unk_10 =
        PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], Data_GetPackEntryData(pack1, entry->unk_01), 0, 0, 15);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], data->unk_10);
    DMA_Flush();
    DatMgr_ReleaseData(pack1);
    DatMgr_ReleaseData(pack2);
    func_ov008_020e75f8(&data->sprite, func_ov008_020e7580());
    return 1;
}

s32 func_ov008_020e78e0(TaskPool* pool, Task* task, void* args) {
    BtlTutorial* data = task->data;

    CombatSprite_Update(&data->sprite);
    return 1;
}

s32 func_ov008_020e78f8(TaskPool* pool, Task* task, void* args) {
    BtlTutorial* data = task->data;

    CombatSprite_Render(&data->sprite);
    return 1;
}

s32 func_ov008_020e7910(TaskPool* pool, Task* task, void* args) {
    BtlTutorial* data = task->data;

    CombatSprite_Release(&data->sprite);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], data->unk_00);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], data->unk_04);
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], data->unk_08);
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], data->unk_0C);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], data->unk_10);
    data_ov003_020e71b8->unk3D81A &= ~1;
    return 1;
}

s32 func_ov008_020e799c(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov008_020e7a28;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}
