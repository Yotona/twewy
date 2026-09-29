#include "Interface/Menu/TusinSet.h"

typedef struct {
    /* 0x00 */ Sprite          sprites[2];
    /* 0x80 */ BOOL            visible;
    /* 0x84 */ TusinSetObject* tusinSet;
} TusinSet_nameD; // Size: 0x88

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_nameD_Args;

static SpriteFrameInfo* TusinSet_nameD_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_nameD = {"Tsk_TusinSet_nameD", TusinSet_nameD_RunTask, sizeof(TusinSet_nameD)};

static const SpriteAnimation TusinSet_nameD_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xc00,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = TusinSet_nameD_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &TusinSet_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 7,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* TusinSet_nameD_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void TusinSet_nameD_Load(TusinSet_nameD* nameD, TusinSet_nameD_Args* args) {
    SpriteAnimation anim         = TusinSet_nameD_Anim;
    Point           positions[2] = {
        {11, 51},
        {11, 77},
    };

    anim.dataType  = args->dataType;
    anim.animIndex = 26;
    anim.posX      = positions[0].x;
    anim.posY      = positions[0].y;
    _Sprite_Load(&nameD->sprites[0], &anim);

    anim.animIndex = 27;
    anim.posX      = positions[1].x;
    anim.posY      = positions[1].y;
    _Sprite_Load(&nameD->sprites[1], &anim);
}

static s32 TusinSet_nameD_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_nameD*      nameD    = task->data;
    TusinSet_nameD_Args* initArgs = args;

    nameD->visible  = TRUE;
    nameD->tusinSet = initArgs->tusinSet;
    TusinSet_nameD_Load(nameD, initArgs);
    return 1;
}

static s32 TusinSet_nameD_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_nameD* nameD = task->data;

    for (s16 i = 0; i < 2; i++) {
        Sprite_Update(&nameD->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_nameD_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_nameD* nameD = task->data;

    for (s16 i = 0; i < 2; i++) {
        Sprite_RenderFrame(&nameD->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_nameD_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_nameD* nameD = task->data;

    for (s16 i = 0; i < 2; i++) {
        Sprite_Release(&nameD->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_nameD_Init,
        .update     = TusinSet_nameD_Update,
        .render     = TusinSet_nameD_Render,
        .cleanup    = TusinSet_nameD_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_nameD_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_nameD_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_nameD, NULL, 0, NULL, &args);
}
