#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ BOOL          visible;
    /* 0x44 */ ResultObject* owner;
} Result_bdg; // Size: 0x48

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           index;
    /* 0xA */ u16           graphicIndex;
} Result_bdg_Args;

static SpriteFrameInfo* Result_bdg_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_bdg_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Result_bdg_Anim = {
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
    .frameInfoCallback = Result_bdg_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Result_BinIdentifiers[27],
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static const TaskHandle Tsk_Result_bdg = {"Tsk_Result_bdg", Result_bdg_RunTask, sizeof(Result_bdg)};

static SpriteFrameInfo* Result_bdg_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x258000);
}

static void Result_bdg_Load(Result_bdg* bdg, Sprite* sprite, Result_bdg_Args* args) {
    SpriteAnimation anim    = Result_bdg_Anim;
    s16             posX[6] = {0x2C, 0x4E, 0x70, 0x92, 0xB4, 0xD6};

    anim.dataType = args->dataType;
    anim.unk_04   = posX[args->index];
    anim.unk_06   = 0xA5;
    anim.bits_7_9 = 5;

    if (args->graphicIndex == 0xFFFF) {
        anim.packIndex = 1;
        bdg->visible   = FALSE;
    } else {
        anim.packIndex = args->graphicIndex + 1;
        bdg->visible   = TRUE;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 Result_bdg_Init(TaskPool* pool, Task* task, void* args) {
    Result_bdg*      bdg     = task->data;
    Result_bdg_Args* bdgArgs = args;

    Result_bdg_Load(bdg, &bdg->sprite, bdgArgs);
    bdg->owner = bdgArgs->owner;
    return 1;
}

static s32 Result_bdg_Update(TaskPool* pool, Task* task, void* args) {
    Result_bdg* bdg = task->data;

    Sprite_Update(&bdg->sprite);
    return 1;
}

static s32 Result_bdg_Render(TaskPool* pool, Task* task, void* args) {
    Result_bdg* bdg = task->data;

    if (bdg->visible != 0) {
        Sprite_RenderFrame(&bdg->sprite);
    }
    return 1;
}

static s32 Result_bdg_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_bdg* bdg = task->data;

    Sprite_Release(&bdg->sprite);
    return 1;
}

static s32 Result_bdg_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_bdg_Init,
        .update     = Result_bdg_Update,
        .render     = Result_bdg_Render,
        .cleanup    = Result_bdg_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Result_bdg_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner) {
    Result_bdg_Args args;

    args.dataType     = dataType;
    args.owner        = owner;
    args.index        = index;
    args.graphicIndex = owner->badges[index].pinID;

    return EasyTask_CreateTask(pool, &Tsk_Result_bdg, NULL, 0, NULL, &args);
}