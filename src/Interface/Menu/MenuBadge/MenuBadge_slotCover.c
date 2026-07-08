#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* unk_44;
} MenuBadge_slotCover; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* unk_4;
    /* 0x8 */ u16              unk_8;
} MenuBadge_slotCover_Args;

static SpriteFrameInfo* MenuBadge_slotCover_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_slotCover_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_slotCover = {"Tsk_MenuBadge_slotCover", MenuBadge_slotCover_RunTask,
                                                   sizeof(MenuBadge_slotCover)};

static const SpriteAnimation MenuBadge_slotCover_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_slotCover_GetFrameInfo,
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

static SpriteFrameInfo* MenuBadge_slotCover_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuBadge_slotCover_Load(MenuBadge_slotCover* slotCover, Sprite* sprite, MenuBadge_slotCover_Args* args) {
    SpriteAnimation anim = MenuBadge_slotCover_Anim;

    slotCover->visible = 1;
    anim.dataType      = args->dataType;
    anim.unk_2A        = 0x39;
    anim.unk_04        = data_ov043_020c834c[args->unk_8].x;
    anim.unk_06        = data_ov043_020c834c[args->unk_8].y;

    _Sprite_Load(sprite, &anim);
}

static s32 MenuBadge_slotCover_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_slotCover*      slotCover = task->data;
    MenuBadge_slotCover_Args* initArgs  = args;

    slotCover->unk_44 = initArgs->unk_4;
    MenuBadge_slotCover_Load(slotCover, &slotCover->sprite, initArgs);
    return 1;
}

static s32 MenuBadge_slotCover_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_slotCover* slotCover = task->data;

    Sprite_Update(&slotCover->sprite);
    return 1;
}

static s32 MenuBadge_slotCover_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_slotCover* slotCover = task->data;

    Sprite_RenderFrame(&slotCover->sprite);
    return 1;
}

static s32 MenuBadge_slotCover_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_slotCover* slotCover = task->data;

    Sprite_Release(&slotCover->sprite);
    return 1;
}

static s32 MenuBadge_slotCover_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_slotCover_Init,
        .update     = MenuBadge_slotCover_Update,
        .render     = MenuBadge_slotCover_Render,
        .cleanup    = MenuBadge_slotCover_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_slotCover_CreateTask(TaskPool* pool, s32 dataType, s16 index, MenuBadgeObject* owner) {
    MenuBadge_slotCover_Args args;

    args.dataType = dataType;
    args.unk_8    = index;
    args.unk_4    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_slotCover, NULL, 0, NULL, &args);
}
