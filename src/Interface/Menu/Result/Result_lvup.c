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
} Result_lvup; // Size: 0x58

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           index;
} Result_lvup_Args;

static SpriteFrameInfo* Result_lvup_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_lvup_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Result_lvup_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = Result_lvup_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Result_BinIdentifiers[13],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* Result_lvup_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x3B6000);
}

static void Result_lvup_Load(Result_lvup* lvup, Sprite* sprite, Result_lvup_Args* args) {
    SpriteAnimation anim    = Result_lvup_Anim;
    s16             posX[6] = {0x2C, 0x4E, 0x70, 0x92, 0xB4, 0xD6};

    anim.dataType = args->dataType;
    anim.bits_7_9 = 6;
    anim.unk_04   = posX[args->index];
    anim.unk_06   = 0xA5;
    anim.unk_2A   = 1;
    lvup->visible = TRUE;

    _Sprite_Load(sprite, &anim);
}

static s32 Result_lvup_Init(TaskPool* pool, Task* task, void* args) {
    Result_lvup*      lvup     = task->data;
    Result_lvup_Args* lvupArgs = args;

    lvup->owner     = lvupArgs->owner;
    lvup->posY      = 0xA5000;
    lvup->velocityY = 0x6000;
    lvup->gravity   = -0x666;
    lvup->holdTimer = 0;
    lvup->state     = 0;

    Result_lvup_Load(lvup, &lvup->sprite, lvupArgs);
    SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_BADGEUP);
    return 1;
}

static s32 Result_lvup_Update(TaskPool* pool, Task* task, void* args) {
    Result_lvup* lvup = task->data;

    switch (lvup->state) {
        case 0: {
            if (lvup->posY < 0x87000) {
                lvup->posY += lvup->velocityY;
            } else if (lvup->posY > 0x87000) {
                lvup->posY -= lvup->velocityY;
            }

            if (lvup->velocityY > 0) {
                lvup->velocityY += lvup->gravity;
                if (lvup->velocityY <= 0) {
                    lvup->velocityY = 0;
                    lvup->holdTimer = 0x3C;
                    lvup->state     = 1;
                }
            }
        } break;

        case 1: {
            if (lvup->holdTimer > 0) {
                lvup->holdTimer--;
            } else {
                return 0;
            }
        } break;
    }

    lvup->sprite.posY = lvup->posY >> 12;
    Sprite_Update(&lvup->sprite);
    return 1;
}

static s32 Result_lvup_Render(TaskPool* pool, Task* task, void* args) {
    Result_lvup* lvup = task->data;

    if (lvup->visible != 0) {
        Sprite_RenderFrame(&lvup->sprite);
    }
    return 1;
}

static s32 Result_lvup_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_lvup* lvup = task->data;

    Sprite_Release(&lvup->sprite);
    return 1;
}

static s32 Result_lvup_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_lvup_Init,
        .update     = Result_lvup_Update,
        .render     = Result_lvup_Render,
        .cleanup    = Result_lvup_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Result_lvup = {"Tsk_Result_lvup", Result_lvup_RunTask, sizeof(Result_lvup)};

s32 Result_lvup_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner) {
    Result_lvup_Args args;

    args.dataType = dataType;
    args.owner    = owner;
    args.index    = index;

    return EasyTask_CreateTask(pool, &Tsk_Result_lvup, NULL, 0, NULL, &args);
}
