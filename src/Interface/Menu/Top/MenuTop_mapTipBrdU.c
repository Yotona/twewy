#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ BOOL           visible;
    /* 0x44 */ MenuTopObject* topMenu;
    /* 0x48 */ u16            area;
    /* 0x4A */ u16            topBrand;
} MenuTop_mapTipBrdU; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
    /* 0x8 */ u16            area;
} MenuTop_mapTipBrdU_Args;

static SpriteFrameInfo* MenuTop_mapTipBrdU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_mapTipBrdU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_mapTipBrdU = {"Tsk_MenuTop_mapTipBrdU", MenuTop_mapTipBrdU_RunTask,
                                                  sizeof(MenuTop_mapTipBrdU)};

static const SpriteAnimation MenuTop_mapTipBrdU_Anim = {
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
    .frameInfoCallback = MenuTop_mapTipBrdU_GetFrameInfo,
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

static SpriteFrameInfo* MenuTop_mapTipBrdU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void MenuTop_mapTipBrdU_Load(MenuTop_mapTipBrdU* mapTipBrdU, Sprite* sprite, MenuTop_mapTipBrdU_Args* args) {
    MenuTopObject*  topMenu = mapTipBrdU->topMenu;
    SpriteAnimation anim    = MenuTop_mapTipBrdU_Anim;

    mapTipBrdU->area     = args->area;
    mapTipBrdU->topBrand = topMenu->areaTopBrand[mapTipBrdU->area];
    mapTipBrdU->visible  = TRUE;

    anim.dataType  = args->dataType;
    anim.bits_7_9  = 5;
    anim.animIndex = mapTipBrdU->topBrand + 24;
    anim.posX      = MenuTop_AreaMapPos[args->area].x;
    anim.posY      = MenuTop_AreaMapPos[args->area].y;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_mapTipBrdU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_mapTipBrdU*      mapTipBrdU     = task->data;
    MenuTop_mapTipBrdU_Args* mapTipBrdUArgs = args;

    mapTipBrdU->topMenu = mapTipBrdUArgs->topMenu;
    MenuTop_mapTipBrdU_Load(mapTipBrdU, &mapTipBrdU->sprite, mapTipBrdUArgs);
    return 1;
}

static s32 MenuTop_mapTipBrdU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_mapTipBrdU* mapTipBrdU = task->data;

    Sprite_Update(&mapTipBrdU->sprite);
    return 1;
}

static s32 MenuTop_mapTipBrdU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_mapTipBrdU* mapTipBrdU = task->data;

    Sprite_RenderFrame(&mapTipBrdU->sprite);
    return 1;
}

static s32 MenuTop_mapTipBrdU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_mapTipBrdU* mapTipBrdU = task->data;

    Sprite_Release(&mapTipBrdU->sprite);
    return 1;
}

static s32 MenuTop_mapTipBrdU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_mapTipBrdU_Init,
        .update     = MenuTop_mapTipBrdU_Update,
        .render     = MenuTop_mapTipBrdU_Render,
        .cleanup    = MenuTop_mapTipBrdU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_mapTipBrdU_CreateTask(TaskPool* pool, s32 dataType, u16 area, MenuTopObject* topMenu) {
    MenuTop_mapTipBrdU_Args args;
    args.dataType = dataType;
    args.area     = area;
    args.topMenu  = topMenu;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_mapTipBrdU, NULL, 0, NULL, &args);
}
