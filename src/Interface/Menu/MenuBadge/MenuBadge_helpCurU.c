#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[2];
    /* 0x80 */ s32              shouldRender[2];
    /* 0x88 */ MenuBadgeObject* menuBadge;
} MenuBadge_helpCurU; // Size: 0x8C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_helpCurU_Args;

static SpriteFrameInfo* MenuBadge_helpCurU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_helpCurU = {"Tsk_MenuBadge_helpCurU", MenuBadge_helpCurU_RunTask,
                                                  sizeof(MenuBadge_helpCurU)};

static const SpriteAnimation MenuBadge_helpCurU_Anim = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_helpCurU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82fc,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_helpCurU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

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

static void MenuBadge_helpCurU_Load(Sprite* sprites, MenuBadge_helpCurU_Args* args) {
    SpriteAnimation anim = MenuBadge_helpCurU_Anim;

    anim.dataType = args->dataType;
    anim.unk_2A   = 0x1E;
    anim.unk_04   = 7;
    anim.unk_06   = 0x60;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 0x1F;
    anim.unk_04 = 0xF9;
    anim.unk_06 = 0x60;
    _Sprite_Load(&sprites[1], &anim);
}

static s32 MenuBadge_helpCurU_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_helpCurU*      helpCur     = task->data;
    MenuBadge_helpCurU_Args* helpCurArgs = args;

    helpCur->menuBadge = helpCurArgs->menuBadge;
    MenuBadge_helpCurU_Load(helpCur->sprites, helpCurArgs);
    return 1;
}

static s32 MenuBadge_helpCurU_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_helpCurU* helpCur = task->data;

    u8 page = helpCur->menuBadge->unk_AF1F;
    if (page == 0) {
        helpCur->shouldRender[0] = 0;
        helpCur->shouldRender[1] = 1;
    } else if (page == 0xA) {
        helpCur->shouldRender[0] = 1;
        helpCur->shouldRender[1] = 0;
    } else {
        helpCur->shouldRender[0] = 1;
        helpCur->shouldRender[1] = 1;
    }

    for (u16 i = 0; i < 2; i++) {
        Sprite_Update(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_helpCurU_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_helpCurU* helpCur = task->data;

    for (u16 i = 0; i < 2; i++) {
        if (helpCur->shouldRender[i] != 0) {
            Sprite_RenderFrame(&helpCur->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuBadge_helpCurU_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_helpCurU* helpCur = task->data;

    for (u16 i = 0; i < 2; i++) {
        Sprite_Release(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_helpCurU_Init,
        .update     = MenuBadge_helpCurU_Update,
        .render     = MenuBadge_helpCurU_Render,
        .cleanup    = MenuBadge_helpCurU_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_helpCurU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* badge) {
    MenuBadge_helpCurU_Args args;

    args.dataType  = dataType;
    args.menuBadge = badge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_helpCurU, NULL, 0, NULL, &args);
}
