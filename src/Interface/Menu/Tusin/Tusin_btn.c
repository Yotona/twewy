#include "Interface/Menu/Tusin.h"

typedef struct {
    /* 0x00 */ Sprite       sprite;
    /* 0x40 */ BOOL         visible;
    /* 0x44 */ TusinObject* tusin;
    /* 0x48 */ s16          unk_48;
    /* 0x4A */ char         unk_4A[0x4C - 0x4A];
} Tusin_btn; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32          dataType;
    /* 0x4 */ TusinObject* tusin;
} Tusin_btn_Args;

static SpriteFrameInfo* Tusin_btn_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Tusin_btn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Tusin_btn = {"Tsk_Tusin_btn", Tusin_btn_RunTask, sizeof(Tusin_btn)};

static const SpriteAnimation Tusin_btn_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = Tusin_btn_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Tusin_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* Tusin_btn_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Tusin_btn_Load(Tusin_btn* btn, Tusin_btn_Args* args) {
    SpriteAnimation anim = Tusin_btn_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 1;
    anim.posX      = 57;
    anim.posY      = 128;
    _Sprite_Load(&btn->sprite, &anim);
}

static s32 Tusin_btn_Init(TaskPool* pool, Task* task, void* args) {
    Tusin_btn*      btn      = task->data;
    Tusin_btn_Args* initArgs = args;

    btn->tusin   = initArgs->tusin;
    btn->visible = TRUE;
    btn->unk_48  = 0;
    Tusin_btn_Load(btn, initArgs);
    return 1;
}

static s32 Tusin_btn_Update(TaskPool* pool, Task* task, void* args) {
    Tusin_btn* btn = task->data;

    if (btn->tusin->btnPressed == 1) {
        Tusin_SetSpriteFrame(&btn->sprite, 2);
    } else {
        Tusin_SetSpriteFrame(&btn->sprite, 1);
    }
    Sprite_Update(&btn->sprite);
    return 1;
}

static s32 Tusin_btn_Render(TaskPool* pool, Task* task, void* args) {
    Tusin_btn* btn = task->data;

    Sprite_RenderFrame(&btn->sprite);
    return 1;
}

static s32 Tusin_btn_Destroy(TaskPool* pool, Task* task, void* args) {
    Tusin_btn* btn = task->data;

    Sprite_Release(&btn->sprite);
    return 1;
}

static s32 Tusin_btn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Tusin_btn_Init,
        .update     = Tusin_btn_Update,
        .render     = Tusin_btn_Render,
        .cleanup    = Tusin_btn_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Tusin_btn_CreateTask(TaskPool* pool, s32 dataType, TusinObject* tusin) {
    Tusin_btn_Args args;

    args.dataType = dataType;
    args.tusin    = tusin;

    return EasyTask_CreateTask(pool, &Tsk_Tusin_btn, NULL, 0, NULL, &args);
}
