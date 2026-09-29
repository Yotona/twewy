#include "Interface/Menu/TusinSet.h"

// USA only: JP has no second scroll bar for lists that fit on one page.
#ifdef REGION_USA

typedef struct {
    /* 0x00 */ Sprite          sprites[3];
    /* 0xC0 */ char            unk_C0[0xC4 - 0xC0];
    /* 0xC4 */ TusinSetObject* tusinSet;
    /* 0xC8 */ u16             lastScroll;
} TusinSet_sbar2; // Size: 0xCC

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_sbar2_Args;

static SpriteFrameInfo* TusinSet_sbar2_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_sbar2_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_sbar2 = {"Tsk_TusinSet_sbar2", TusinSet_sbar2_RunTask, sizeof(TusinSet_sbar2)};

static const SpriteAnimation TusinSet_sbar2_Anim = {
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
    .frameInfoCallback = TusinSet_sbar2_GetFrameInfo,
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

static SpriteFrameInfo* TusinSet_sbar2_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void TusinSet_sbar2_Load(Sprite* sprites, TusinSet_sbar2_Args* args) {
    SpriteAnimation anim = TusinSet_sbar2_Anim;

    anim.dataType = args->dataType;

    anim.animIndex = 32;
    anim.posX      = 248;
    anim.posY      = 143;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 17;
    anim.posX      = 248;
    anim.posY      = 88;
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 18;
    anim.posX      = 248;
    anim.posY      = 150;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 TusinSet_sbar2_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_sbar2_Args* sbar2Args = args;
    TusinSetObject*      tusinSet  = sbar2Args->tusinSet;

    TusinSet_sbar2* sbar2 = task->data;

    sbar2->tusinSet   = tusinSet;
    sbar2->lastScroll = tusinSet->scroll;
    TusinSet_sbar2_Load(sbar2->sprites, sbar2Args);
    return 1;
}

static s32 TusinSet_sbar2_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_sbar2* sbar2 = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&sbar2->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_sbar2_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_sbar2* sbar2    = task->data;
    TusinSetObject* tusinSet = sbar2->tusinSet;

    if (tusinSet->fitsOnePage[tusinSet->tab] == 1) {
        for (s32 i = 0; i < 3; i++) {
            Sprite_RenderFrame(&sbar2->sprites[i]);
        }
    }
    return 1;
}

static s32 TusinSet_sbar2_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_sbar2* sbar2 = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&sbar2->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_sbar2_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_sbar2_Init,
        .update     = TusinSet_sbar2_Update,
        .render     = TusinSet_sbar2_Render,
        .cleanup    = TusinSet_sbar2_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_sbar2_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_sbar2_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_sbar2, NULL, 0, NULL, &args);
}
#endif
