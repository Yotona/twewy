#include "Interface/Menu/Tusin.h"

typedef struct {
    /* 0x00 */ Sprite       sprites[2];
    /* 0x80 */ BOOL         visible[2];
    /* 0x88 */ TusinObject* tusin;
    /* 0x8C */ u16          unk_8C;
    /* 0x8E */ char         unk_8E[0x90 - 0x8E];
} Tusin_num; // Size: 0x90

typedef struct {
    /* 0x0 */ s32          dataType;
    /* 0x4 */ TusinObject* tusin;
} Tusin_num_Args;

static SpriteFrameInfo* Tusin_num_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Tusin_num_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Tusin_num = {"Tsk_Tusin_num", Tusin_num_RunTask, sizeof(Tusin_num)};

static const SpriteAnimation Tusin_num_Anim = {
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
    .frameInfoCallback = Tusin_num_GetFrameInfo,
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

static SpriteFrameInfo* Tusin_num_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Tusin_num_UpdateDigits(Tusin_num* num, u16 value) {
    s32 digits[2];
    s32 offsetX;
    u16 i;

    digits[0] = (u16)(value / 10);
    digits[1] = (u16)(value % 10);

    for (i = 0; i < 2; i++) {
        num->visible[i] = TRUE;
    }

    if (digits[0] == 0) {
        num->visible[0] = FALSE;
        num->visible[1] = TRUE;
        offsetX         = -6;
    } else {
        num->visible[0] = TRUE;
        num->visible[1] = TRUE;
        offsetX         = 0;
    }

    for (i = 0; i < 2; i++) {
        num->sprites[i].posX = offsetX + (i * 12 + 52);
        num->sprites[i].posY = 157;
        Tusin_SetSpriteFrame(&num->sprites[i], digits[i] + 3);
    }
}

static void Tusin_num_Load(Tusin_num* num, Sprite* sprites, Tusin_num_Args* args) {
    TusinObject*    tusin = num->tusin;
    SpriteAnimation anim  = Tusin_num_Anim;
    u16             tens;
    s32             offsetX;
    u16             i;

    anim.dataType = args->dataType;
    tens          = tusin->mingleRemaining / 10;

    for (i = 0; i < 2; i++) {
        num->visible[i] = TRUE;
    }

    if (tens == 0) {
        num->visible[0] = FALSE;
        num->visible[1] = TRUE;
        offsetX         = -6;
    } else {
        num->visible[0] = TRUE;
        num->visible[1] = TRUE;
        offsetX         = 0;
    }

    for (i = 0; i < 2; i++) {
        anim.posX = offsetX + (i * 12 + 52);
        anim.posY = 157;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 Tusin_num_Init(TaskPool* pool, Task* task, void* args) {
    Tusin_num*      num      = task->data;
    Tusin_num_Args* initArgs = args;

    num->tusin  = initArgs->tusin;
    num->unk_8C = 0;
    Tusin_num_Load(num, num->sprites, initArgs);
    return 1;
}

static s32 Tusin_num_Update(TaskPool* pool, Task* task, void* args) {
    Tusin_num* num = task->data;
    u16        i;

    Tusin_num_UpdateDigits(num, num->tusin->mingleRemaining);
    for (i = 0; i < 2; i++) {
        Sprite_Update(&num->sprites[i]);
    }
    return 1;
}

static s32 Tusin_num_Render(TaskPool* pool, Task* task, void* args) {
    Tusin_num* num = task->data;
    u16        i;

    for (i = 0; i < 2; i++) {
        if (num->visible[i]) {
            Sprite_RenderFrame(&num->sprites[i]);
        }
    }
    return 1;
}

static s32 Tusin_num_Destroy(TaskPool* pool, Task* task, void* args) {
    Tusin_num* num = task->data;
    u16        i;

    for (i = 0; i < 2; i++) {
        Sprite_Release(&num->sprites[i]);
    }
    return 1;
}

static s32 Tusin_num_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Tusin_num_Init,
        .update     = Tusin_num_Update,
        .render     = Tusin_num_Render,
        .cleanup    = Tusin_num_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Tusin_num_CreateTask(TaskPool* pool, s32 dataType, TusinObject* tusin) {
    Tusin_num_Args args;

    args.dataType = dataType;
    args.tusin    = tusin;

    return EasyTask_CreateTask(pool, &Tsk_Tusin_num, NULL, 0, NULL, &args);
}
