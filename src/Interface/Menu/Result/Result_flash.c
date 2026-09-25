#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ BOOL          visible;
    /* 0x44 */ ResultObject* owner;
} Result_flash; // Size: 0x48

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           index;
} Result_flash_Args;

static SpriteFrameInfo* Result_flash_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_flash_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Result_flash_Anim = {
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
    .frameInfoCallback = Result_flash_GetFrameInfo,
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

static SpriteFrameInfo* Result_flash_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x26C000);
}

static void Result_flash_Load(Result_flash* flash, Sprite* sprite, Result_flash_Args* args) {
    ResultObject*   owner   = flash->owner;
    SpriteAnimation anim    = Result_flash_Anim;
    s16             posX[6] = {0x2C, 0x4E, 0x70, 0x92, 0xB4, 0xD6};

    anim.dataType      = args->dataType;
    anim.bits_7_9      = 6;
    anim.unk_04        = posX[args->index];
    anim.unk_06        = 0xA5;
    anim.unk_2A        = 0x36;
    flash->visible     = TRUE;
    owner->flashLevel  = 1;
    anim.unk_02.unk_02 = 1;

    data_0206a890.unk_0C = owner->flashLevel & 0x1F;

    _Sprite_Load(sprite, &anim);
}

static s32 Result_flash_Init(TaskPool* pool, Task* task, void* args) {
    Result_flash*      flash     = task->data;
    Result_flash_Args* flashArgs = args;

    flash->owner = flashArgs->owner;
    Result_flash_Load(flash, &flash->sprite, flashArgs);
    return 1;
}

static s32 Result_flash_Update(TaskPool* pool, Task* task, void* args) {
    Result_flash* flash = task->data;

    data_0206a890.unk_0C = flash->owner->flashLevel & 0x1F;
    Sprite_Update(&flash->sprite);
    return 1;
}

static s32 Result_flash_Render(TaskPool* pool, Task* task, void* args) {
    Result_flash* flash = task->data;

    if (flash->visible) {
        Sprite_RenderFrame(&flash->sprite);
    }
    return 1;
}

static s32 Result_flash_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_flash* flash = task->data;

    Sprite_Release(&flash->sprite);
    return 1;
}

static s32 Result_flash_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_flash_Init,
        .update     = Result_flash_Update,
        .render     = Result_flash_Render,
        .cleanup    = Result_flash_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Result_flash = {"Tsk_Result_flash", Result_flash_RunTask, sizeof(Result_flash)};

s32 Result_flash_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner) {
    Result_flash_Args args;

    args.dataType = dataType;
    args.owner    = owner;
    args.index    = index;

    return EasyTask_CreateTask(pool, &Tsk_Result_flash, NULL, 0, NULL, &args);
}
