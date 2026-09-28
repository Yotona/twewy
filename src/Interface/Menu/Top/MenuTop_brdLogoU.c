#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ s32            visible;
    /* 0x44 */ MenuTopObject* topMenu;
} MenuTop_brdLogoU; // Size: 0x48

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
} MenuTop_brdLogoU_Args;

static SpriteFrameInfo* MenuTop_brdLogoU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_brdLogoU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_brdLogoU = {"Tsk_MenuTop_brdLogoU", MenuTop_brdLogoU_RunTask, sizeof(MenuTop_brdLogoU)};

static const SpriteAnimation MenuTop_brdLogoU_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .unk_04            = 0x3E,
    .unk_06            = 0x3C,
    .frameInfoCallback = MenuTop_brdLogoU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[13],
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuTop_brdLogoU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_brdLogoU_Load(MenuTop_brdLogoU* brdLogoU, Sprite* sprite, MenuTop_brdLogoU_Args* args) {
    MenuTopObject*  topMenu = brdLogoU->topMenu;
    SpriteAnimation anim    = MenuTop_brdLogoU_Anim;

    anim.dataType = args->dataType;

    if (topMenu->currentArea < 21) {
        anim.packIndex    = topMenu->areaBrandRanking[0] + 1;
        brdLogoU->visible = 1;
    } else if (topMenu->currentArea >= 22) {
        anim.packIndex    = 1;
        brdLogoU->visible = 0;
    } else {
        anim.packIndex            = 1;
        *(u32*)&brdLogoU->visible = 0;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_brdLogoU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_brdLogoU*      brdLogoU    = task->data;
    MenuTop_brdLogoU_Args* brdLogoArgs = args;

    brdLogoU->topMenu = brdLogoArgs->topMenu;
    MenuTop_brdLogoU_Load(brdLogoU, &brdLogoU->sprite, brdLogoArgs);
    return 1;
}

static s32 MenuTop_brdLogoU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_brdLogoU* brdLogoU = task->data;

    Sprite_Update(&brdLogoU->sprite);
    return 1;
}

static s32 MenuTop_brdLogoU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_brdLogoU* brdLogoU = task->data;

    if (brdLogoU->visible != 0) {
        Sprite_RenderFrame(&brdLogoU->sprite);
    }
    return 1;
}

static s32 MenuTop_brdLogoU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_brdLogoU* brdLogoU = task->data;

    Sprite_Release(&brdLogoU->sprite);
    return 1;
}

static s32 MenuTop_brdLogoU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_brdLogoU_Init,
        .update     = MenuTop_brdLogoU_Update,
        .render     = MenuTop_brdLogoU_Render,
        .cleanup    = MenuTop_brdLogoU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_brdLogoU_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_brdLogoU_Args args;
    args.dataType = dataType;
    args.topMenu  = topMenu;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_brdLogoU, NULL, 0, NULL, &args);
}
