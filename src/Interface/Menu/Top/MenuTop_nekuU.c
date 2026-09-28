#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ BOOL           visible;
    /* 0x44 */ MenuTopObject* topMenu;
    /* 0x48 */ Point          unk_48;
    /* 0x4C */ BOOL           onMap;
} MenuTop_nekuU; // Size: 0x50

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
} MenuTop_nekuU_Args;

static SpriteFrameInfo* MenuTop_nekuU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_nekuU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_nekuU = {"Tsk_MenuTop_nekuU", MenuTop_nekuU_RunTask, sizeof(MenuTop_nekuU)};

static const SpriteAnimation MenuTop_nekuU_Anim = {
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
    .frameInfoCallback = MenuTop_nekuU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[7],
    .unk_18            = 0,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuTop_nekuU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_nekuU_Load(MenuTop_nekuU* nekuU, Sprite* sprite, MenuTop_nekuU_Args* args) {
    MenuTopObject*  topMenu = nekuU->topMenu;
    SpriteAnimation anim    = MenuTop_nekuU_Anim;

    anim.dataType = args->dataType;

    if (topMenu->currentArea < 21) {
        anim.animIndex = 13;
        anim.posX      = MenuTop_AreaMapPos[topMenu->currentArea].x;
        anim.posY      = MenuTop_AreaMapPos[topMenu->currentArea].y - 2;
        nekuU->visible = TRUE;
        nekuU->onMap   = TRUE;
    } else {
        anim.animIndex = 13;
        anim.posX      = 0;
        anim.posY      = 0;
        nekuU->visible = FALSE;
        nekuU->onMap   = FALSE;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_nekuU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_nekuU*      nekuU     = task->data;
    MenuTop_nekuU_Args* nekuUArgs = args;

    nekuU->topMenu  = nekuUArgs->topMenu;
    nekuU->unk_48.x = 40;
    nekuU->unk_48.y = 0;
    MenuTop_nekuU_Load(nekuU, &nekuU->sprite, nekuUArgs);
    return 1;
}

static s32 MenuTop_nekuU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_nekuU* nekuU = task->data;

    nekuU->visible = nekuU->topMenu->blinkOn;

    Sprite_Update(&nekuU->sprite);
    return 1;
}

static s32 MenuTop_nekuU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_nekuU* nekuU = task->data;

    if (nekuU->visible) {
        Sprite_RenderFrame(&nekuU->sprite);
    }
    return 1;
}

static s32 MenuTop_nekuU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_nekuU* nekuU = task->data;

    Sprite_Release(&nekuU->sprite);
    return 1;
}

static s32 MenuTop_nekuU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_nekuU_Init,
        .update     = MenuTop_nekuU_Update,
        .render     = MenuTop_nekuU_Render,
        .cleanup    = MenuTop_nekuU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_nekuU_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_nekuU_Args args;
    args.dataType = dataType;
    args.topMenu  = topMenu;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_nekuU, NULL, 0, NULL, &args);
}