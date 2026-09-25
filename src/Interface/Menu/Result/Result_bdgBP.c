#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x00 */ Sprite        sprites[2];
    /* 0x80 */ BOOL          visible[2];
    /* 0x88 */ ResultObject* owner;
    /* 0x8C */ u16           index;
} Result_bdgBP; // Size: 0x90

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           index;
} Result_bdgBP_Args;

static SpriteFrameInfo* Result_bdgBP_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_bdgBP_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Result_bdgBP_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = Result_bdgBP_GetFrameInfo,
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

static SpriteFrameInfo* Result_bdgBP_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x2EE000);
}

static u16 Result_bdgBP_GetGaugeFrame(Result_bdgBP* bdgBP, u16 index) {
    ResultObject* owner    = bdgBP->owner;
    u32           minBp    = owner->badges[index].levelPP;
    u32           maxBp    = owner->badges[index].nextLevelPP;
    u32           curBp    = owner->badges[index].totalPP;
    u8            level    = owner->badges[index].level;
    u8            maxLevel = owner->badges[index].maxLevel;
    u16           frame    = 0;

    if (owner->badges[index].pinID == 0xFFFF) {
        bdgBP->visible[0] = FALSE;
        bdgBP->visible[1] = FALSE;
        return 0;
    }

    if (level == maxLevel) {
        bdgBP->visible[0] = TRUE;
        frame             = 0x18;
    } else if (curBp == minBp) {
        bdgBP->visible[0] = FALSE;
    } else {
        frame             = (curBp - minBp) * 0x17 / (maxBp - minBp);
        bdgBP->visible[0] = TRUE;
    }

    bdgBP->visible[1] = TRUE;
    return frame;
}

static void Result_bdgBP_Load(Result_bdgBP* bdgBP, Sprite* sprites, Result_bdgBP_Args* args) {
    SpriteAnimation anim    = Result_bdgBP_Anim;
    s16             posX[6] = {0x2C, 0x4E, 0x70, 0x92, 0xB4, 0xD6};

    anim.dataType = args->dataType;
    anim.unk_04   = posX[args->index] - 0xC;
    anim.unk_06   = 0xB8;
    anim.unk_2A   = Result_bdgBP_GetGaugeFrame(bdgBP, args->index) + 5;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 0x1E;
    _Sprite_Load(&sprites[1], &anim);
}

static s32 Result_bdgBP_Init(TaskPool* pool, Task* task, void* args) {
    Result_bdgBP*      bdgBP     = task->data;
    Result_bdgBP_Args* bdgBPArgs = args;

    bdgBP->owner = bdgBPArgs->owner;
    bdgBP->index = bdgBPArgs->index;
    Result_bdgBP_Load(bdgBP, bdgBP->sprites, bdgBPArgs);
    return 1;
}

static s32 Result_bdgBP_Update(TaskPool* pool, Task* task, void* args) {
    Result_bdgBP* bdgBP = task->data;

    Result_SetSpriteFrame(&bdgBP->sprites[0], Result_bdgBP_GetGaugeFrame(bdgBP, bdgBP->index) + 5);

    for (s16 i = 0; i < 2; i++) {
        Sprite_Update(&bdgBP->sprites[i]);
    }
    return 1;
}

static s32 Result_bdgBP_Render(TaskPool* pool, Task* task, void* args) {
    Result_bdgBP* bdgBP = task->data;

    for (s16 i = 0; i < 2; i++) {
        if (bdgBP->visible[i] != 0) {
            Sprite_RenderFrame(&bdgBP->sprites[i]);
        }
    }
    return 1;
}

static s32 Result_bdgBP_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_bdgBP* bdgBP = task->data;

    for (s16 i = 0; i < 2; i++) {
        Sprite_Release(&bdgBP->sprites[i]);
    }
    return 1;
}

static s32 Result_bdgBP_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_bdgBP_Init,
        .update     = Result_bdgBP_Update,
        .render     = Result_bdgBP_Render,
        .cleanup    = Result_bdgBP_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Result_bdgBP = {"Tsk_Result_bdgBP", Result_bdgBP_RunTask, sizeof(Result_bdgBP)};

s32 Result_bdgBP_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner) {
    Result_bdgBP_Args args;

    args.dataType = dataType;
    args.owner    = owner;
    args.index    = index;

    return EasyTask_CreateTask(pool, &Tsk_Result_bdgBP, NULL, 0, NULL, &args);
}
