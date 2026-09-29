#include "Interface/Menu/TusinSet.h"

typedef struct {
    /* 0x00 */ Sprite          sprites[2];
    /* 0x80 */ char            unk_80[0x84 - 0x80];
    /* 0x84 */ TusinSetObject* tusinSet;
} TusinSet_icon; // Size: 0x88

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_icon_Args;

static SpriteFrameInfo* TusinSet_icon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_icon = {"Tsk_TusinSet_icon", TusinSet_icon_RunTask, sizeof(TusinSet_icon)};

static const SpriteAnimation TusinSet_icon_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = TusinSet_icon_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &TusinSet_BinIdentifiers[8],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* TusinSet_icon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void TusinSet_icon_Load(TusinSet_icon* icon, TusinSet_icon_Args* args) {
    SpriteAnimation anim = TusinSet_icon_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 3;
    anim.posX      = 218;
    anim.posY      = 12;
    _Sprite_Load(&icon->sprites[0], &anim);

    anim.animIndex = 5;
    anim.posX      = 243;
    anim.posY      = 12;
    _Sprite_Load(&icon->sprites[1], &anim);
}

static s32 TusinSet_icon_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_icon*      icon     = task->data;
    TusinSet_icon_Args* initArgs = args;

    icon->tusinSet = initArgs->tusinSet;
    TusinSet_icon_Load(icon, initArgs);
    return 1;
}

static s32 TusinSet_icon_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_icon*  icon     = task->data;
    TusinSetObject* tusinSet = icon->tusinSet;
    s32             i;

    for (i = 0; i < 2; i++) {
        if (tusinSet->iconPressed[i] == 1) {
            TusinSet_SetSpriteFrame(&icon->sprites[i], i * 2 + 4);
            if (tusinSet->iconTimer != 0) {
                tusinSet->iconTimer--;
            } else {
                tusinSet->iconPressed[i] = 0;
            }
        } else {
            TusinSet_SetSpriteFrame(&icon->sprites[i], i * 2 + 3);
        }
    }

    for (i = 0; i < 2; i++) {
        Sprite_Update(&icon->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_icon_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_RenderFrame(&icon->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_icon_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&icon->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_icon_Init,
        .update     = TusinSet_icon_Update,
        .render     = TusinSet_icon_Render,
        .cleanup    = TusinSet_icon_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_icon_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_icon_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_icon, NULL, 0, NULL, &args);
}
