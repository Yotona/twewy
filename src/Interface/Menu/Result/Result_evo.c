#include "Interface/Menu/Result.h"
#include "SndMgr.h"

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ BOOL          visible;
    /* 0x44 */ ResultObject* owner;
    /* 0x48 */ s32           posY;
    /* 0x4C */ s32           velocityY;
    /* 0x50 */ s32           gravity;
    /* 0x54 */ s16           holdTimer;
    /* 0x56 */ s16           state;
} Result_evo; // Size: 0x58

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           index;
} Result_evo_Args;

static SpriteFrameInfo* Result_evo_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              Result_evo_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Result_evo_Anim = {
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
    .unk_08     = Result_evo_GetFrameInfo,
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

static SpriteFrameInfo* Result_evo_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

            info->unk_10 = 0x3B6000;
            return info;
        } break;
    }

    return NULL;
}

static void Result_evo_Load(Result_evo* evo, Sprite* sprite, Result_evo_Args* args) {
    SpriteAnimation anim    = Result_evo_Anim;
    s16             posX[6] = {0x2C, 0x4E, 0x70, 0x92, 0xB4, 0xD6};

    anim.dataType = args->dataType;
    anim.bits_7_9 = 6;
    anim.unk_04   = posX[args->index];
    anim.unk_06   = 0xA5;
    anim.unk_2A   = 3;
    evo->visible  = TRUE;

    _Sprite_Load(sprite, &anim);
}

static s32 Result_evo_Init(TaskPool* pool, Task* task, void* args) {
    Result_evo*      evo     = task->data;
    Result_evo_Args* evoArgs = args;

    evo->owner     = evoArgs->owner;
    evo->posY      = 0xA5000;
    evo->velocityY = 0x6000;
    evo->gravity   = -0x666;
    evo->holdTimer = 0;
    evo->state     = 0;

    Result_evo_Load(evo, &evo->sprite, evoArgs);
    SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_BADGEUP);
    SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_EVO_LVUP);
    return 1;
}

static s32 Result_evo_Update(TaskPool* pool, Task* task, void* args) {
    Result_evo* evo = task->data;

    switch (evo->state) {
        case 0: {
            if (evo->posY < 0x87000) {
                evo->posY += evo->velocityY;
            } else if (evo->posY > 0x87000) {
                evo->posY -= evo->velocityY;
            }

            if (evo->velocityY > 0) {
                evo->velocityY += evo->gravity;
                if (evo->velocityY <= 0) {
                    evo->velocityY = 0;
                    evo->holdTimer = 0x3C;
                    evo->state     = 1;
                }
            }
        } break;

        case 1: {
            if (evo->holdTimer > 0) {
                evo->holdTimer--;
            } else {
                return 0;
            }
        } break;
    }

    evo->sprite.posY = evo->posY >> 12;
    Sprite_Update(&evo->sprite);
    return 1;
}

static s32 Result_evo_Render(TaskPool* pool, Task* task, void* args) {
    Result_evo* evo = task->data;

    if (evo->visible != 0) {
        Sprite_RenderFrame(&evo->sprite);
    }
    return 1;
}

static s32 Result_evo_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_evo* evo = task->data;

    Sprite_Release(&evo->sprite);
    return 1;
}

static s32 Result_evo_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_evo_Init,
        .update     = Result_evo_Update,
        .render     = Result_evo_Render,
        .cleanup    = Result_evo_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Result_evo = {"Tsk_Result_evo", Result_evo_RunTask, sizeof(Result_evo)};

s32 Result_evo_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner) {
    Result_evo_Args args;

    args.dataType = dataType;
    args.owner    = owner;
    args.index    = index;

    return EasyTask_CreateTask(pool, &Tsk_Result_evo, NULL, 0, NULL, &args);
}
