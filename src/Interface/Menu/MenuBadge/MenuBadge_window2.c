#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    visible;
    /* 0x44 */ void*  unk_44;
} MenuBadge_window2; // Size: 0x48

typedef struct {
    /* 0x0 */ s32   dataType;
    /* 0x4 */ void* unk_4;
} MenuBadge_window2_Args;

static SpriteFrameInfo* MenuBadge_window2_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_window2_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_window2 = {"Tsk_MenuBadge_window2", MenuBadge_window2_RunTask,
                                                 sizeof(MenuBadge_window2)};

static const SpriteAnimation MenuBadge_window2_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_window2_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82e4,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 0xA,
    .unk_1E     = 0,
    .unk_20     = 0xD,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 0xB,
    .unk_28     = 0xC,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_window2_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;

            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info = temp;
        } break;
    }

    return info;
}

static void MenuBadge_window2_Load(Sprite* sprite, MenuBadge_window2_Args* args) {
    SpriteAnimation anim = MenuBadge_window2_Anim;

    anim.dataType = args->dataType;
    anim.unk_2A   = 0xA;
    anim.unk_04   = 0x80;
    anim.unk_06   = 0x60;

    _Sprite_Load(sprite, &anim);
}

static s32 MenuBadge_window2_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window2*      window2  = task->data;
    MenuBadge_window2_Args* initArgs = args;

    window2->visible = 1;
    window2->unk_44  = initArgs->unk_4;
    MenuBadge_window2_Load(&window2->sprite, initArgs);
    return 1;
}

static s32 MenuBadge_window2_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window2* window2 = task->data;

    Sprite_Update(&window2->sprite);
    return 1;
}

static s32 MenuBadge_window2_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window2* window2 = task->data;

    if (window2->visible) {
        Sprite_RenderFrame(&window2->sprite);
    }
    return 1;
}

static s32 MenuBadge_window2_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window2* window2 = task->data;

    Sprite_Release(&window2->sprite);
    return 1;
}

static s32 MenuBadge_window2_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_window2_Init,
        .update     = MenuBadge_window2_Update,
        .render     = MenuBadge_window2_Render,
        .cleanup    = MenuBadge_window2_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_window2_CreateTask(TaskPool* pool, s32 dataType, void* arg2) {
    MenuBadge_window2_Args args;

    args.dataType = dataType;
    args.unk_4    = arg2;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_window2, NULL, 0, NULL, &args);
}
