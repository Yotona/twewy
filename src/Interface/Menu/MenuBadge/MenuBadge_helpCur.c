#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ s32              visibleFlags[3];
    /* 0xCC */ MenuBadgeObject* unk_CC;
} MenuBadge_helpCur; // Size: 0xD0

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* unk_4;
} MenuBadge_helpCur_Args;

extern s32 func_02023d1c(s32 arg0);

static SpriteFrameInfo* MenuBadge_helpCur_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_helpCur = {"Tsk_MenuBadge_helpCur", MenuBadge_helpCur_RunTask,
                                                 sizeof(MenuBadge_helpCur)};

static const SpriteAnimation MenuBadge_helpCur_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_helpCur_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82e4,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 0xD,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_helpCur_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuBadge_helpCur_Load(MenuBadge_helpCur* helpCur, Sprite* sprites, MenuBadge_helpCur_Args* args) {
    SpriteAnimation anim = MenuBadge_helpCur_Anim;

    anim.dataType = args->dataType;

    for (u16 i = 0; i < 3; i++) {
        helpCur->visibleFlags[i] = 1;
    }

    anim.unk_2A = 0x55;
    anim.unk_04 = 0x30;
    anim.unk_06 = 0x58;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 0x57;
    anim.unk_04 = 0xD0;
    anim.unk_06 = 0x58;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = 0x59;
    anim.unk_04 = 0x80;
    anim.unk_06 = 0x7D;
    _Sprite_Load(&sprites[2], &anim);

    if (func_02023d1c(4) == 0) {
        helpCur->visibleFlags[2] = 0;
    }
}

static s32 MenuBadge_helpCur_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_helpCur*      helpCur  = task->data;
    MenuBadge_helpCur_Args* initArgs = args;

    helpCur->unk_CC = initArgs->unk_4;
    MenuBadge_helpCur_Load(helpCur, helpCur->sprites, initArgs);
    return 1;
}

// Nonmatching
static s32 MenuBadge_helpCur_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_helpCur* helpCur = task->data;
    MenuBadgeObject*   owner   = helpCur->unk_CC;

    s32 idleFrame   = 0x55;
    s32 activeFrame = 0x56;

    for (s32 i = 0; i < 3; i++) {
        if (owner->unk_AF1C[i] == 1) {
            func_ov043_0208fa60(&helpCur->sprites[i], activeFrame, 3, 2);
            if (owner->unk_AF02 != 0) {
                owner->unk_AF02--;
            } else {
                owner->unk_AF1C[i] = 0;
            }
        } else {
            func_ov043_0208fa60(&helpCur->sprites[i], idleFrame, 3, 2);
        }
        activeFrame += 2;
        idleFrame += 2;
    }

    if (owner->unk_AF1F == 0) {
        helpCur->visibleFlags[0] = 0;
        helpCur->visibleFlags[1] = 1;
    } else if (owner->unk_AF1F != 10) {
        helpCur->visibleFlags[0] = 1;
        helpCur->visibleFlags[1] = 1;
    } else {
        helpCur->visibleFlags[0] = 1;
        helpCur->visibleFlags[1] = 0;
    }

    if (func_02023d1c(4) == 1) {
        helpCur->visibleFlags[2] = 1;
    }

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_helpCur_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        if (helpCur->visibleFlags[i]) {
            Sprite_RenderFrame(&helpCur->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuBadge_helpCur_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_helpCur_Init,
        .update     = MenuBadge_helpCur_Update,
        .render     = MenuBadge_helpCur_Render,
        .cleanup    = MenuBadge_helpCur_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_helpCur_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* owner) {
    MenuBadge_helpCur_Args args;

    args.dataType = dataType;
    args.unk_4    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_helpCur, NULL, 0, NULL, &args);
}
