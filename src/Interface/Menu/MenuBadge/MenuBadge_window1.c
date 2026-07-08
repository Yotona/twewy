#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[5];
    /* 0x140 */ s32              visibleFlags[5];
    /* 0x154 */ MenuBadgeObject* unk_154;
} MenuBadge_window1; // Size: 0x158

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* unk_4;
} MenuBadge_window1_Args;

static SpriteFrameInfo* MenuBadge_window1_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_window1_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_window1 = {"Tsk_MenuBadge_window1", MenuBadge_window1_RunTask,
                                                 sizeof(MenuBadge_window1)};

static const SpriteAnimation MenuBadge_window1_Anim = {
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
    .unk_08     = MenuBadge_window1_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82e4,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 7,
    .unk_1E     = 0,
    .unk_20     = 0xD,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 8,
    .unk_28     = 9,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_window1_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuBadge_window1_Load(MenuBadge_window1* window1, Sprite* sprites, MenuBadge_window1_Args* args) {
    SpriteAnimation anim = MenuBadge_window1_Anim;

    anim.dataType = args->dataType;

    for (u16 i = 0; i < 5; i++) {
        window1->visibleFlags[i] = 1;
    }

    anim.unk_2A = 1;
    anim.unk_04 = 0x3A;
    anim.unk_06 = 0x46;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 3;
    anim.unk_04 = 0x3A;
    anim.unk_06 = 0x5D;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = 5;
    anim.unk_04 = 0x3A;
    anim.unk_06 = 0x7A;
    _Sprite_Load(&sprites[2], &anim);

    anim.unk_2A = 0xD;
    anim.unk_04 = 0xD0;
    anim.unk_06 = 0x8D;
    _Sprite_Load(&sprites[3], &anim);

    anim.unk_2A = 0xF;
    anim.unk_04 = 0x80;
    anim.unk_06 = 0x60;
    _Sprite_Load(&sprites[4], &anim);
}

static s16 func_ov043_020963ec(MenuBadgeObject* owner, u16 index, s32 sel) {
    if (index == 3) {
        return sel + 0xD;
    }
    return (owner->unk_AF2B[index] * 6) + (sel + 1) + (index * 2);
}

static s32 MenuBadge_window1_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window1*      window1  = task->data;
    MenuBadge_window1_Args* initArgs = args;

    window1->unk_154 = initArgs->unk_4;
    MenuBadge_window1_Load(window1, window1->sprites, initArgs);
    return 1;
}

// Nonmatching: regswap
static s32 MenuBadge_window1_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window1* window1 = task->data;
    MenuBadgeObject*   owner   = window1->unk_154;

    for (u16 i = 0; i < 4; i++) {
        if (owner->unk_AF27[i] == 1) {
            func_ov043_0208fa60(&window1->sprites[i], func_ov043_020963ec(owner, i, 1), 9, 8);
            if (owner->unk_AF02 == 0) {
                owner->unk_AF27[i] = 0;
            } else {
                owner->unk_AF02 -= 1;
            }
        } else {
            func_ov043_0208fa60(&window1->sprites[i], func_ov043_020963ec(owner, i, 0), 9, 8);
        }
    }

    for (u16 i = 0; i < 5; i++) {
        Sprite_Update(&window1->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_window1_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window1* window1 = task->data;

    for (u16 i = 0; i < 5; i++) {
        if (window1->visibleFlags[i]) {
            Sprite_RenderFrame(&window1->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuBadge_window1_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window1* window1 = task->data;

    for (u16 i = 0; i < 5; i++) {
        Sprite_Release(&window1->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_window1_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_window1_Init,
        .update     = MenuBadge_window1_Update,
        .render     = MenuBadge_window1_Render,
        .cleanup    = MenuBadge_window1_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_window1_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* owner) {
    MenuBadge_window1_Args args;

    args.dataType = dataType;
    args.unk_4    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_window1, NULL, 0, NULL, &args);
}
