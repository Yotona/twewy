#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ s32              unk_C0;
    /* 0xC4 */ MenuBadgeObject* badge;
} MenuBadge_icon; // Size: 0xC8

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* badge;
} MenuBadge_icon_Args;

static SpriteFrameInfo* MenuBadge_icon_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_icon = {"Tsk_MenuBadge_icon", MenuBadge_icon_RunTask, sizeof(MenuBadge_icon)};

static const SpriteAnimation MenuBadge_icon_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0xB6,
    .unk_06     = 0x10,
    .unk_08     = MenuBadge_icon_GetFrameInfo,
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

static SpriteFrameInfo* MenuBadge_icon_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuBadge_icon_Load(Sprite* sprites, MenuBadge_icon_Args* args) {
    SpriteAnimation anim = MenuBadge_icon_Anim;

    anim.dataType = args->dataType;

    anim.unk_2A = 0x28;
    anim.unk_04 = 0xC1;
    anim.unk_06 = 0xC;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 0x2A;
    anim.unk_04 = 0xDA;
    anim.unk_06 = 0xC;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = 0x2C;
    anim.unk_04 = 0xF3;
    anim.unk_06 = 0xC;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 MenuBadge_icon_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_icon*      icon     = task->data;
    MenuBadge_icon_Args* initArgs = args;

    MenuBadge_icon_Load(icon->sprites, initArgs);
    icon->badge = initArgs->badge;
    return 1;
}

// Nonmatching
static s32 MenuBadge_icon_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_icon*  icon  = task->data;
    MenuBadgeObject* badge = icon->badge;

    s32 idleFrame   = 40;
    s32 activeFrame = 41;

    for (s32 i = 0; i < 3; i++) {
        if (badge->unk_AEFF[i] == 1) {
            func_ov043_0208fa60(&icon->sprites[i], activeFrame, 3, 2);
            if (badge->unk_AF02 != 0) {
                badge->unk_AF02--;
            } else {
                badge->unk_AEFF[i] = 0;
            }
        } else {
            func_ov043_0208fa60(&icon->sprites[i], idleFrame, 3, 2);
        }
        activeFrame += 2;
        idleFrame += 2;
    }

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_icon_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_icon* icon = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_RenderFrame(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_icon_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_icon* icon = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_icon_Init,
        .update     = MenuBadge_icon_Update,
        .render     = MenuBadge_icon_Render,
        .cleanup    = MenuBadge_icon_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_icon_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* badge) {
    MenuBadge_icon_Args args;

    args.dataType = dataType;
    args.badge    = badge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_icon, NULL, 0, NULL, &args);
}
