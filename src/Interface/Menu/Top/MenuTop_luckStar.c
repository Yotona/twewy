#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk_40;
    /* 0x44 */ void*  topMenu;
} MenuTop_luckStar; // Size: 0x48

typedef struct {
    /* 0x0 */ s32   dataType;
    /* 0x4 */ void* topMenu;
} MenuTop_luckStar_Args;

static SpriteFrameInfo* MenuTop_luckStar_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_luckStar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_luckStar = {"Tsk_MenuTop_luckStar", MenuTop_luckStar_RunTask, sizeof(MenuTop_luckStar)};

static const SpriteAnimation MenuTop_luckStar_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .unk_04            = 0xCA,
    .unk_06            = 0xB6,
    .frameInfoCallback = MenuTop_luckStar_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[11],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 11,
};

static SpriteFrameInfo* MenuTop_luckStar_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_luckStar_Load(Sprite* sprite, MenuTop_luckStar_Args* args) {
    SpriteAnimation anim = MenuTop_luckStar_Anim;

    anim.dataType = args->dataType;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_luckStar_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckStar*      luckStar = task->data;
    MenuTop_luckStar_Args* initArgs = args;

    MenuTop_luckStar_Load(&luckStar->sprite, initArgs);
    luckStar->topMenu = initArgs->topMenu;
    return 1;
}

static s32 MenuTop_luckStar_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckStar* luckStar = task->data;

    Sprite_Update(&luckStar->sprite);
    return 1;
}

static s32 MenuTop_luckStar_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckStar* luckStar = task->data;

    Sprite_RenderFrame(&luckStar->sprite);
    return 1;
}

static s32 MenuTop_luckStar_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckStar* luckStar = task->data;

    Sprite_Release(&luckStar->sprite);
    return 1;
}

static s32 MenuTop_luckStar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_luckStar_Init,
        .update     = MenuTop_luckStar_Update,
        .render     = MenuTop_luckStar_Render,
        .cleanup    = MenuTop_luckStar_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_luckStar_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_luckStar_Args args;

    args.dataType = dataType;
    args.topMenu  = topMenu;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_luckStar, NULL, 0, NULL, &args);
}
