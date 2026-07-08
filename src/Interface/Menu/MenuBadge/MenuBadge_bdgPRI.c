#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* unk_44;
} MenuBadge_bdgPRI; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* unk_4;
    /* 0x8 */ u16              unk_8;
    /* 0xA */ u16              unk_A;
    /* 0xC */ s16              unk_C;
    /* 0xE */ u8               unk_E;
} MenuBadge_bdgPRI_Args;

static SpriteFrameInfo* MenuBadge_bdgPRI_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_bdgPRI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_bdgPRI = {"Tsk_MenuBadge_bdgPRI", MenuBadge_bdgPRI_RunTask, sizeof(MenuBadge_bdgPRI)};

static const SpriteAnimation MenuBadge_bdgPRI_Anim = {
    .bits_0_1   = 2,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_bdgPRI_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82e4,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 4,
    .unk_1E     = 0,
    .unk_20     = 0xD,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 5,
    .unk_28     = 6,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_bdgPRI_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1: {
            info->unk_00 = 1;
            return info;
        } break;

        case 2: {
            info = &data_0206b408;

            info->unk_04 = 0;
            info->unk_08 = 0;
            info->unk_0C = 0;
            info->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                info->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                info->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info->unk_10 = 0x398000;
            return info;
        } break;
    }

    return NULL;
}

static void MenuBadge_bdgPRI_Load(MenuBadge_bdgPRI* bdgPRI, Sprite* sprite, MenuBadge_bdgPRI_Args* args) {
    SpriteAnimation anim = MenuBadge_bdgPRI_Anim;

    anim.dataType = args->dataType;
    anim.unk_04   = data_ov043_020c834c[args->unk_8].x - 9;
    anim.unk_06   = data_ov043_020c834c[args->unk_8].y - 9;
    anim.bits_7_9 = 6;

    if (args->unk_8 < args->unk_E) {
        anim.unk_2A     = args->unk_8 + 1;
        bdgPRI->visible = TRUE;
    } else {
        anim.unk_2A     = 1;
        bdgPRI->visible = FALSE;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 MenuBadge_bdgPRI_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgPRI*      bdgPRI     = task->data;
    MenuBadge_bdgPRI_Args* bdgPRIArgs = args;

    MenuBadge_bdgPRI_Load(bdgPRI, &bdgPRI->sprite, bdgPRIArgs);
    bdgPRI->unk_44 = bdgPRIArgs->unk_4;
    return 1;
}

static s32 MenuBadge_bdgPRI_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgPRI* bdgPRI = task->data;

    Sprite_Update(&bdgPRI->sprite);
    return 1;
}

static s32 MenuBadge_bdgPRI_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgPRI* bdgPRI = task->data;

    if (bdgPRI->visible) {
        Sprite_RenderFrame(&bdgPRI->sprite);
    }
    return 1;
}

static s32 MenuBadge_bdgPRI_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgPRI* bdgPRI = task->data;

    Sprite_Release(&bdgPRI->sprite);
    return 1;
}

static s32 MenuBadge_bdgPRI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_bdgPRI_Init,
        .update     = MenuBadge_bdgPRI_Update,
        .render     = MenuBadge_bdgPRI_Render,
        .cleanup    = MenuBadge_bdgPRI_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_bdgPRI_CreateTask(TaskPool* pool, s32 dataType, s16 index, MenuBadgeObject* owner) {
    MenuBadge_bdgPRI_Args args;

    args.dataType = dataType;
    args.unk_4    = owner;
    args.unk_8    = index;
    args.unk_A    = owner->unk_AD60[index]->unk_00;
    args.unk_C    = owner->unk_AD60[index]->unk_15;
    args.unk_E    = owner->unk_AEEA;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdgPRI, NULL, 0, NULL, &args);
}
