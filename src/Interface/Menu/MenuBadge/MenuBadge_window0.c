#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[5];
    /* 0x140 */ s32              visibleFlags[5];
    /* 0x154 */ MenuBadgeObject* unk_154;
} MenuBadge_window0; // Size: 0x158

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* unk_4;
} MenuBadge_window0_Args;

static SpriteFrameInfo* MenuBadge_window0_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_window0_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_window0 = {"Tsk_MenuBadge_window0", MenuBadge_window0_RunTask,
                                                 sizeof(MenuBadge_window0)};

static const SpriteAnimation MenuBadge_window0_Anim = {
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
    .unk_08     = MenuBadge_window0_GetFrameInfo,
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

static SpriteFrameInfo* MenuBadge_window0_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuBadge_window0_Load(MenuBadge_window0* window0, Sprite* sprites, MenuBadge_window0_Args* args) {
    MenuBadgeObject* owner = window0->unk_154;
    SpriteAnimation  anim  = MenuBadge_window0_Anim;
    u16              i;

    anim.dataType = args->dataType;

    for (i = 0; i < 5; i++) {
        window0->visibleFlags[i] = 1;
    }

    anim.unk_2A = 5;
    anim.unk_04 = 0x4B;
    anim.unk_06 = 0x75;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 7;
    anim.unk_04 = 0xB4;
    anim.unk_06 = 0x75;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = 1;
    anim.unk_04 = 0xE0;
    anim.unk_06 = 0x49;
    _Sprite_Load(&sprites[2], &anim);

    anim.unk_2A = 3;
    anim.unk_04 = 0xE0;
    anim.unk_06 = 0x5C;
    _Sprite_Load(&sprites[3], &anim);

    anim.unk_2A = 9;
    anim.unk_04 = 0x80;
    anim.unk_06 = 0x60;
    _Sprite_Load(&sprites[4], &anim);

    if (func_ov043_0208fac8(owner, owner->unk_AEEC) == 0) {
        window0->visibleFlags[2] = 0;
        window0->visibleFlags[3] = 0;
    }
}

static s32 MenuBadge_window0_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window0*      window0  = task->data;
    MenuBadge_window0_Args* initArgs = args;

    window0->unk_154 = initArgs->unk_4;
    MenuBadge_window0_Load(window0, window0->sprites, initArgs);
    return 1;
}

// Nonmatching: regswap
static s32 MenuBadge_window0_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window0* window0 = task->data;
    MenuBadgeObject*   owner   = window0->unk_154;

    for (u16 i = 0; i < 2; i++) {
        if (owner->unk_AF12[i] == 1) {
            func_ov043_0208fa60(&window0->sprites[i], i * 2 + 6, 0xC, 0xB);
            if (owner->unk_AF02 == 0) {
                owner->unk_AF12[i] = 0;
            } else {
                owner->unk_AF02--;
            }
        } else {
            func_ov043_0208fa60(&window0->sprites[i], i * 2 + 5, 0xC, 0xB);
        }
    }

    for (u16 i = 0; i < 2; i++) {
        if (owner->unk_AF18[i] == 1) {
            func_ov043_0208fa60(&window0->sprites[i + 2], i * 2 + 2, 0xC, 0xB);
            if (owner->unk_AF02 == 0) {
                owner->unk_AF18[i] = 0;
            } else {
                owner->unk_AF02--;
            }
        } else {
            func_ov043_0208fa60(&window0->sprites[i + 2], i * 2 + 1, 0xC, 0xB);
        }
    }

    for (u16 i = 0; i < 5; i++) {
        Sprite_Update(&window0->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_window0_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window0* window0 = task->data;

    for (u16 i = 0; i < 5; i++) {
        if (window0->visibleFlags[i]) {
            Sprite_RenderFrame(&window0->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuBadge_window0_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_window0* window0 = task->data;

    window0->unk_154->unk_AF02 = 0;
    for (u16 i = 0; i < 5; i++) {
        Sprite_Release(&window0->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_window0_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_window0_Init,
        .update     = MenuBadge_window0_Update,
        .render     = MenuBadge_window0_Render,
        .cleanup    = MenuBadge_window0_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_window0_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* owner) {
    MenuBadge_window0_Args args;

    args.dataType = dataType;
    args.unk_4    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_window0, NULL, 0, NULL, &args);
}
