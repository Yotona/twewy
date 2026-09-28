#include "Interface/Menu/Top.h"
#include "Player/Inventory.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ BOOL           visible;
    /* 0x44 */ MenuTopObject* topMenu;
    /* 0x48 */ u16            area;
    /* 0x4A */ u16            topBrand;
    /* 0x4C */ s16            unk_4C;
    /* 0x4E */ s16            unk_4E;
} MenuTop_mapTipU; // Size: 0x50

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
    /* 0x8 */ u16            area;
} MenuTop_mapTipU_Args;

static SpriteFrameInfo* MenuTop_mapTipU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_mapTipU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_mapTipU = {"Tsk_MenuTop_mapTipU", MenuTop_mapTipU_RunTask, sizeof(MenuTop_mapTipU)};

static const SpriteAnimation MenuTop_mapTipU_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = MenuTop_mapTipU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[8],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 13,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuTop_mapTipU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void MenuTop_mapTipU_Load(MenuTop_mapTipU* mapTipU, Sprite* sprite, MenuTop_mapTipU_Args* args) {
    MenuTopObject*  topMenu = mapTipU->topMenu;
    SpriteAnimation anim    = MenuTop_mapTipU_Anim;

    anim.dataType = args->dataType;
    anim.bits_7_9 = 5;

    mapTipU->area     = args->area;
    mapTipU->topBrand = topMenu->areaTopBrand[mapTipU->area];
    mapTipU->visible  = TRUE;
    mapTipU->unk_4C   = 0;
    mapTipU->unk_4E   = 0;

    anim.animIndex = mapTipU->area + 1;
    anim.posX      = MenuTop_AreaMapPos[mapTipU->area].x;
    anim.posY      = MenuTop_AreaMapPos[mapTipU->area].y;
    _Sprite_Load(sprite, &anim);

    if (mapTipU->area == 21) {
        if (func_0202366c(22, 1) == 0) {
            mapTipU->visible = FALSE;
        }
    } else if (mapTipU->area == 22) {
        if (func_0202366c(35, 1) == 0) {
            mapTipU->visible = FALSE;
        }
    }
}

static s32 MenuTop_mapTipU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_mapTipU*      mapTipU     = task->data;
    MenuTop_mapTipU_Args* mapTipUArgs = args;

    mapTipU->topMenu = mapTipUArgs->topMenu;
    MenuTop_mapTipU_Load(mapTipU, &mapTipU->sprite, mapTipUArgs);
    return 1;
}

static s32 MenuTop_mapTipU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_mapTipU* mapTipU = task->data;

    Sprite_Update(&mapTipU->sprite);
    return 1;
}

static s32 MenuTop_mapTipU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_mapTipU* mapTipU = task->data;

    if (mapTipU->visible) {
        if (mapTipU->area >= 21) {
            Sprite_RenderFrame(&mapTipU->sprite);
        } else {
            Sprite_RenderAltPalette(&mapTipU->sprite, NULL, NULL, (u8)mapTipU->topBrand);
        }
    }
    return 1;
}

static s32 MenuTop_mapTipU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_mapTipU* mapTipU = task->data;

    Sprite_Release(&mapTipU->sprite);
    return 1;
}

static s32 MenuTop_mapTipU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_mapTipU_Init,
        .update     = MenuTop_mapTipU_Update,
        .render     = MenuTop_mapTipU_Render,
        .cleanup    = MenuTop_mapTipU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_mapTipU_CreateTask(TaskPool* pool, s32 dataType, u16 area, MenuTopObject* topMenu) {
    MenuTop_mapTipU_Args args;
    args.dataType = dataType;
    args.area     = area;
    args.topMenu  = topMenu;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_mapTipU, NULL, 0, NULL, &args);
}
