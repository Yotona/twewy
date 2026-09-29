#include "Interface/Menu/Tusin.h"

typedef struct {
    /* 0x00 */ Sprite       sprite;
    /* 0x40 */ BOOL         visible;
    /* 0x44 */ TusinObject* tusin;
    /* 0x48 */ u16          lastState;
    /* 0x4A */ char         unk_4A[0x4C - 0x4A];
} Tusin_btnF; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32          dataType;
    /* 0x4 */ TusinObject* tusin;
} Tusin_btnF_Args;

static SpriteFrameInfo* Tusin_btnF_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Tusin_btnF_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Tusin_btnF = {"Tsk_Tusin_btnF", Tusin_btnF_RunTask, sizeof(Tusin_btnF)};

static const SpriteAnimation Tusin_btnF_Anim = {
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
    .frameInfoCallback = Tusin_btnF_GetFrameInfo,
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

static SpriteFrameInfo* Tusin_btnF_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Tusin_btnF_Load(Tusin_btnF* btnF, Tusin_btnF_Args* args) {
    SpriteAnimation anim = Tusin_btnF_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 17;
    anim.posX      = 58;
    anim.posY      = 179;
    _Sprite_Load(&btnF->sprite, &anim);
}

static s32 Tusin_btnF_Init(TaskPool* pool, Task* task, void* args) {
    Tusin_btnF*      btnF     = task->data;
    Tusin_btnF_Args* initArgs = args;

    btnF->visible   = TRUE;
    btnF->tusin     = initArgs->tusin;
    btnF->lastState = 0;
    Tusin_btnF_Load(btnF, initArgs);
    return 1;
}

static s32 Tusin_btnF_Update(TaskPool* pool, Task* task, void* args) {
    Tusin_btnF* btnF  = task->data;
    u8          state = btnF->tusin->btnFState;

    if (btnF->lastState == 0) {
        if (state == 1) {
            Tusin_SetSpriteFrame(&btnF->sprite, 18);
            btnF->lastState = 1;
        }
    } else if (state == 0) {
        Tusin_SetSpriteFrame(&btnF->sprite, 17);
        btnF->lastState = 0;
    }
    Sprite_Update(&btnF->sprite);
    return 1;
}

static s32 Tusin_btnF_Render(TaskPool* pool, Task* task, void* args) {
    Tusin_btnF* btnF = task->data;

    Sprite_RenderFrame(&btnF->sprite);
    return 1;
}

static s32 Tusin_btnF_Destroy(TaskPool* pool, Task* task, void* args) {
    Tusin_btnF* btnF = task->data;

    Sprite_Release(&btnF->sprite);
    return 1;
}

static s32 Tusin_btnF_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Tusin_btnF_Init,
        .update     = Tusin_btnF_Update,
        .render     = Tusin_btnF_Render,
        .cleanup    = Tusin_btnF_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Tusin_btnF_CreateTask(TaskPool* pool, s32 dataType, TusinObject* tusin) {
    Tusin_btnF_Args args;

    args.dataType = dataType;
    args.tusin    = tusin;

    return EasyTask_CreateTask(pool, &Tsk_Tusin_btnF, NULL, 0, NULL, &args);
}
