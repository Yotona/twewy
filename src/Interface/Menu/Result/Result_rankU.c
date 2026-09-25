#include "Interface/Menu/Result.h"
#include "SndMgr.h"

// JP slides the rank badge in from the right and stops it further across.
#ifdef REGION_USA
    #define RANKU_START_X -0x1A000
    #define RANKU_ACCEL_X -0x4CD
    #define RANKU_STOP_X  0x1C000
#else
    #define RANKU_START_X 0x118000
    #define RANKU_ACCEL_X -0x666
    #define RANKU_STOP_X  0xE5000
#endif

typedef struct {
    /* 0x00 */ s32           unk_00;
    /* 0x04 */ Sprite        sprite;
    /* 0x44 */ BOOL          visible;
    /* 0x48 */ ResultObject* owner;
    /* 0x4C */ u16           state;
    /* 0x50 */ s32           posX;
    /* 0x54 */ s32           velocityX;
    /* 0x58 */ s32           accelX;
} Result_rankU; // Size: 0x5C

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           rank;
} Result_rankU_Args;

static SpriteFrameInfo* Result_rankU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              Result_rankU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Result_rankU = {"Tsk_Result_rankU", Result_rankU_RunTask, sizeof(Result_rankU)};

static const SpriteAnimation Result_rankU_Anim = {
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
    .unk_08     = Result_rankU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &Result_BinIdentifiers[25],
    .unk_18     = 0,
    .packIndex  = 1,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 4,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* Result_rankU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

            return info;
        } break;
    }

    return NULL;
}

static void Result_rankU_Load(Result_rankU* rankU, Sprite* sprite, Result_rankU_Args* args) {
    SpriteAnimation anim = Result_rankU_Anim;

    rankU->posX      = RANKU_START_X;
    rankU->velocityX = 0x6000;
    rankU->accelX    = RANKU_ACCEL_X;

    anim.dataType = args->dataType;
    anim.unk_2A   = args->rank + 1;
    anim.unk_04   = rankU->posX >> 12;
    anim.unk_06   = 0x20;

    _Sprite_Load(sprite, &anim);
    rankU->visible = TRUE;
}

static s32 Result_rankU_Init(TaskPool* pool, Task* task, void* args) {
    Result_rankU*      rankU     = task->data;
    Result_rankU_Args* rankUArgs = args;

    rankU->owner = rankUArgs->owner;
    rankU->state = 0;
    Result_rankU_Load(rankU, &rankU->sprite, rankUArgs);
    return 1;
}

static s32 Result_rankU_Update(TaskPool* pool, Task* task, void* args) {
    Result_rankU* rankU = task->data;
    ResultObject* owner = rankU->owner;

    switch (rankU->state) {
        case 0: {
            if (owner->stageFlags & 8) {
                SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_RANK);
                rankU->state = 1;
            }
        } break;

        case 1: {
            if (rankU->posX < RANKU_STOP_X) {
                rankU->posX += rankU->velocityX;
            } else if (rankU->posX > RANKU_STOP_X) {
                rankU->posX -= rankU->velocityX;
            }

            if (rankU->velocityX > 0) {
                rankU->velocityX += rankU->accelX;
                if (rankU->velocityX <= 0) {
                    rankU->velocityX = 0;
                    rankU->state     = 2;
                }
            }
        } break;

        case 2: {
        } break;
    }

    rankU->sprite.posX = rankU->posX >> 12;
    Sprite_Update(&rankU->sprite);
    return 1;
}

static s32 Result_rankU_Render(TaskPool* pool, Task* task, void* args) {
    Result_rankU* rankU = task->data;

    if (rankU->visible != 0) {
        Sprite_RenderFrame(&rankU->sprite);
    }
    return 1;
}

static s32 Result_rankU_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_rankU* rankU = task->data;

    Sprite_Release(&rankU->sprite);
    return 1;
}

static s32 Result_rankU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_rankU_Init,
        .update     = Result_rankU_Update,
        .render     = Result_rankU_Render,
        .cleanup    = Result_rankU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Result_rankU_CreateTask(TaskPool* pool, s32 dataType, u16 rank, ResultObject* owner) {
    Result_rankU_Args args;

    args.dataType = dataType;
    args.owner    = owner;
    args.rank     = rank;

    return EasyTask_CreateTask(pool, &Tsk_Result_rankU, NULL, 0, NULL, &args);
}
