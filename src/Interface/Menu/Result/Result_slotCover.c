#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ BOOL          visible;
    /* 0x44 */ ResultObject* owner;
} Result_slotCover; // Size: 0x48

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           index;
} Result_slotCover_Args;

static SpriteFrameInfo* Result_slotCover_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_slotCover_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Result_slotCover_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = Result_slotCover_GetFrameInfo,
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

static SpriteFrameInfo* Result_slotCover_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x370000);
}

static void Result_slotCover_Load(Result_slotCover* slotCover, Sprite* sprite, Result_slotCover_Args* args) {
    SpriteAnimation anim    = Result_slotCover_Anim;
    s16             posX[6] = {0x2C, 0x4E, 0x70, 0x92, 0xB4, 0xD6};

    slotCover->visible = TRUE;
    anim.dataType      = args->dataType;
    anim.unk_2A        = 0x37;
    anim.unk_04        = posX[args->index];
    anim.unk_06        = 0xA5;

    _Sprite_Load(sprite, &anim);
}

static s32 Result_slotCover_Init(TaskPool* pool, Task* task, void* args) {
    Result_slotCover*      slotCover     = task->data;
    Result_slotCover_Args* slotCoverArgs = args;

    slotCover->owner = slotCoverArgs->owner;
    Result_slotCover_Load(slotCover, &slotCover->sprite, slotCoverArgs);
    return 1;
}

static s32 Result_slotCover_Update(TaskPool* pool, Task* task, void* args) {
    Result_slotCover* slotCover = task->data;

    Sprite_Update(&slotCover->sprite);
    return 1;
}

static s32 Result_slotCover_Render(TaskPool* pool, Task* task, void* args) {
    Result_slotCover* slotCover = task->data;

    Sprite_RenderFrame(&slotCover->sprite);
    return 1;
}

static s32 Result_slotCover_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_slotCover* slotCover = task->data;

    Sprite_Release(&slotCover->sprite);
    return 1;
}

static s32 Result_slotCover_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_slotCover_Init,
        .update     = Result_slotCover_Update,
        .render     = Result_slotCover_Render,
        .cleanup    = Result_slotCover_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Result_slotCover = {"Tsk_Result_slotCover", Result_slotCover_RunTask, sizeof(Result_slotCover)};

s32 Result_slotCover_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner) {
    Result_slotCover_Args args;

    args.dataType = dataType;
    args.owner    = owner;
    args.index    = index;

    return EasyTask_CreateTask(pool, &Tsk_Result_slotCover, NULL, 0, NULL, &args);
}
