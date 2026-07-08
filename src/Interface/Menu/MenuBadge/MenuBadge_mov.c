#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
} MenuBadge_mov; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
    /* 0x8 */ u16              unk_8;
    /* 0xA */ u16              unk_A;
} MenuBadge_mov_Args;

static SpriteFrameInfo* MenuBadge_mov_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_mov_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_mov = {"Tsk_MenuBadge_mov", MenuBadge_mov_RunTask, sizeof(MenuBadge_mov)};

static const SpriteAnimation MenuBadge_mov_Anim = {
    .bits_0_1   = 2,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_mov_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c8314,
    .unk_18     = 2,
    .packIndex  = 1,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_mov_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1: {
            info->unk_00 = 1;
            return info;
        } break;

        case 2: {
            info         = &data_0206b408;
            info->unk_04 = 0;
            info->unk_08 = 0;
            info->unk_0C = 0;
            info->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                info->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                info->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info->unk_10 = 0x3E8000;
            return info;
        } break;
    }

    return NULL;
}

static void MenuBadge_mov_Load(MenuBadge_mov* mov, Sprite* sprite, MenuBadge_mov_Args* args) {
    SpriteAnimation anim = MenuBadge_mov_Anim;

    anim.dataType = args->dataType;
    anim.bits_7_9 = func_ov043_0208fb8c();

    if (args->unk_8 != 0xFFFF) {
        anim.packIndex = args->unk_A + 1;
        _Sprite_Load(sprite, &anim);
    }
}

static s32 MenuBadge_mov_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_mov*      mov     = task->data;
    MenuBadge_mov_Args* movArgs = args;

    mov->visible   = TRUE;
    mov->menuBadge = movArgs->menuBadge;
    MenuBadge_mov_Load(mov, &mov->sprite, movArgs);
    return 1;
}

static s32 MenuBadge_mov_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_mov*   mov       = task->data;
    MenuBadgeObject* menuBadge = mov->menuBadge;

    mov->sprite.posX = menuBadge->unk_AEF2;
    mov->sprite.posY = menuBadge->unk_AEF4;
    Sprite_Update(&mov->sprite);
    return 1;
}

static s32 MenuBadge_mov_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_mov* mov = task->data;

    if (mov->visible) {
        Sprite_RenderFrame(&mov->sprite);
    }
    return 1;
}

static s32 MenuBadge_mov_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_mov* mov = task->data;

    Sprite_Release(&mov->sprite);
    return 1;
}

static s32 MenuBadge_mov_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_mov_Init,
        .update     = MenuBadge_mov_Update,
        .render     = MenuBadge_mov_Render,
        .cleanup    = MenuBadge_mov_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_mov_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge) {
    MenuBadge_mov_Args args;

    args.dataType  = dataType;
    args.menuBadge = menuBadge;
    args.unk_8     = menuBadge->unk_ADD8;
    args.unk_A     = menuBadge->unk_ADF2;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_mov, NULL, 0, NULL, &args);
}
