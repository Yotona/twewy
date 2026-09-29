#include "Interface/Menu/Tusin.h"

typedef struct {
    /* 0x00 */ Sprite       sprites[2];
    /* 0x80 */ BOOL         visible;
    /* 0x84 */ TusinObject* tusin;
    /* 0x88 */ u16          unk_88;
    /* 0x8A */ u16          unk_8A;
    /* 0x8C */ char         unk_8C[0x90 - 0x8C];
} Tusin_btnLR; // Size: 0x90

typedef struct {
    /* 0x0 */ s32          dataType;
    /* 0x4 */ TusinObject* tusin;
} Tusin_btnLR_Args;

static SpriteFrameInfo* Tusin_btnLR_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Tusin_btnLR_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Tusin_btnLR = {"Tsk_Tusin_btnLR", Tusin_btnLR_RunTask, sizeof(Tusin_btnLR)};

static const SpriteAnimation Tusin_btnLR_Anim = {
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
    .frameInfoCallback = Tusin_btnLR_GetFrameInfo,
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

static SpriteFrameInfo* Tusin_btnLR_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Tusin_btnLR_Load(Tusin_btnLR* btnLR, Tusin_btnLR_Args* args) {
    SpriteAnimation anim = Tusin_btnLR_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 13;
    anim.posX      = 36;
    anim.posY      = 157;
    _Sprite_Load(&btnLR->sprites[0], &anim);

    anim.animIndex = 15;
    anim.posX      = 78;
    anim.posY      = 157;
    _Sprite_Load(&btnLR->sprites[1], &anim);
}

static s32 Tusin_btnLR_Init(TaskPool* pool, Task* task, void* args) {
    Tusin_btnLR*      btnLR    = task->data;
    Tusin_btnLR_Args* initArgs = args;

    btnLR->tusin   = initArgs->tusin;
    btnLR->visible = TRUE;
    btnLR->unk_88  = 0;
    btnLR->unk_8A  = 0;
    Tusin_btnLR_Load(btnLR, initArgs);
    return 1;
}

static s32 Tusin_btnLR_Update(TaskPool* pool, Task* task, void* args) {
    Tusin_btnLR* btnLR = task->data;
    TusinObject* tusin = btnLR->tusin;
    u16          i;

    for (i = 0; i < 2; i++) {
        if (tusin->btnLRPressed[i] == 1) {
            Tusin_SetSpriteFrame(&btnLR->sprites[i], i * 2 + 14);
            if (tusin->btnLRTimer != 0) {
                tusin->btnLRTimer--;
            } else {
                tusin->btnLRPressed[i] = 0;
            }
        } else {
            Tusin_SetSpriteFrame(&btnLR->sprites[i], i * 2 + 13);
        }
    }

    for (i = 0; i < 2; i++) {
        Sprite_Update(&btnLR->sprites[i]);
    }
    return 1;
}

static s32 Tusin_btnLR_Render(TaskPool* pool, Task* task, void* args) {
    Tusin_btnLR* btnLR = task->data;
    u16          i;

    for (i = 0; i < 2; i++) {
        Sprite_RenderFrame(&btnLR->sprites[i]);
    }
    return 1;
}

static s32 Tusin_btnLR_Destroy(TaskPool* pool, Task* task, void* args) {
    Tusin_btnLR* btnLR = task->data;
    u16          i;

    for (i = 0; i < 2; i++) {
        Sprite_Release(&btnLR->sprites[i]);
    }
    return 1;
}

static s32 Tusin_btnLR_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Tusin_btnLR_Init,
        .update     = Tusin_btnLR_Update,
        .render     = Tusin_btnLR_Render,
        .cleanup    = Tusin_btnLR_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Tusin_btnLR_CreateTask(TaskPool* pool, s32 dataType, TusinObject* tusin) {
    Tusin_btnLR_Args args;

    args.dataType = dataType;
    args.tusin    = tusin;

    return EasyTask_CreateTask(pool, &Tsk_Tusin_btnLR, NULL, 0, NULL, &args);
}
