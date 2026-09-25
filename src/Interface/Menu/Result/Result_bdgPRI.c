#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ BOOL          visible;
    /* 0x44 */ ResultObject* owner;
} Result_bdgPRI; // Size: 0x48

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           index;
    /* 0xA */ u16           graphicIndex;
    /* 0xC */ u16           priority;
    /* 0xE */ u8            badgeCount;
} Result_bdgPRI_Args;

static SpriteFrameInfo* Result_bdgPRI_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              Result_bdgPRI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Result_bdgPRI_Anim = {
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
    .unk_08     = Result_bdgPRI_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &Result_BinIdentifiers[13],
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* Result_bdgPRI_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return info;
        } break;

        case 2: {
            data_0206b408.unk_04 = 0;
            data_0206b408.unk_08 = 0;
            data_0206b408.unk_0C = 0;
            data_0206b408.unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                data_0206b408.unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                data_0206b408.unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info->unk_10 = 0x28A000;
            return info;
        } break;
    }

    return NULL;
}

static void Result_bdgPRI_Load(Result_bdgPRI* bdgPRI, Sprite* sprite, Result_bdgPRI_Args* args) {
    SpriteAnimation anim    = Result_bdgPRI_Anim;
    s16             posX[6] = {0x2C, 0x4E, 0x70, 0x92, 0xB4, 0xD6};

    anim.dataType = args->dataType;
    anim.unk_04   = posX[args->index] - 9;
    anim.unk_06   = 0x9C;
    anim.bits_7_9 = 6;

    if (args->index < args->badgeCount) {
        anim.unk_2A     = args->index + 0x1F;
        bdgPRI->visible = TRUE;
    } else {
        anim.unk_2A     = 0x1F;
        bdgPRI->visible = FALSE;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 Result_bdgPRI_Init(TaskPool* pool, Task* task, void* args) {
    Result_bdgPRI*      bdgPRI     = task->data;
    Result_bdgPRI_Args* bdgPRIArgs = args;

    Result_bdgPRI_Load(bdgPRI, &bdgPRI->sprite, bdgPRIArgs);
    bdgPRI->owner = bdgPRIArgs->owner;
    return 1;
}

static s32 Result_bdgPRI_Update(TaskPool* pool, Task* task, void* args) {
    Result_bdgPRI* bdgPRI = task->data;

    Sprite_Update(&bdgPRI->sprite);
    return 1;
}

static s32 Result_bdgPRI_Render(TaskPool* pool, Task* task, void* args) {
    Result_bdgPRI* bdgPRI = task->data;

    if (bdgPRI->visible != 0) {
        Sprite_RenderFrame(&bdgPRI->sprite);
    }
    return 1;
}

static s32 Result_bdgPRI_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_bdgPRI* bdgPRI = task->data;

    Sprite_Release(&bdgPRI->sprite);
    return 1;
}

static s32 Result_bdgPRI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_bdgPRI_Init,
        .update     = Result_bdgPRI_Update,
        .render     = Result_bdgPRI_Render,
        .cleanup    = Result_bdgPRI_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Result_bdgPRI = {"Tsk_Result_bdgPRI", Result_bdgPRI_RunTask, sizeof(Result_bdgPRI)};

s32 Result_bdgPRI_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner) {
    Result_bdgPRI_Args args;

    args.dataType     = dataType;
    args.owner        = owner;
    args.index        = index;
    args.graphicIndex = owner->badges[index].pinID;
    args.priority     = owner->badges[index].slot;
    args.badgeCount   = owner->badgeCount;

    return EasyTask_CreateTask(pool, &Tsk_Result_bdgPRI, NULL, 0, NULL, &args);
}
