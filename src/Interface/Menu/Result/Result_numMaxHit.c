#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x000 */ Sprite        sprites[6];
    /* 0x180 */ BOOL          visible[6];
    /* 0x198 */ ResultObject* owner;
} Result_numMaxHit; // Size: 0x19C

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ u32           value;
    /* 0x8 */ ResultObject* owner;
} Result_numMaxHit_Args;

static SpriteFrameInfo* Result_numMaxHit_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_numMaxHit_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Result_numMaxHit = {"Tsk_Result_numMaxHit", Result_numMaxHit_RunTask, sizeof(Result_numMaxHit)};

static const SpriteAnimation Result_numMaxHit_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = Result_numMaxHit_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Result_BinIdentifiers[12],
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

static SpriteFrameInfo* Result_numMaxHit_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void Result_numMaxHit_Load(Result_numMaxHit* numMaxHit, Sprite* sprites, Result_numMaxHit_Args* args) {
    s32             digits[5];
    SpriteAnimation anim = Result_numMaxHit_Anim;
    u32             value;
    s32             posX;
    s16             i;

    anim.dataType = args->dataType;

    value = args->value;
    if (value > 99999) {
        value = 99999;
    }

    digits[0] = value / 10000;
    value %= 10000;
    digits[1] = value / 1000;
    value %= 1000;
    digits[2] = value / 100;
    value %= 100;
    digits[3] = value / 10;
    digits[4] = value % 10;

    for (i = 0; i < 6; i++) {
        numMaxHit->visible[i] = TRUE;
    }

    for (i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        numMaxHit->visible[i] = FALSE;
    }

    for (i = 0, posX = 0; i < 5; i++) {
        anim.unk_2A = digits[i] + 0xF;
        anim.unk_04 = posX;
        anim.unk_06 = 8;
        _Sprite_Load(&sprites[i], &anim);
        posX += 7;
    }

#ifdef REGION_USA
    // "hit" vs "hits"; JP has a single label.
    if (args->value >= 2) {
        anim.unk_2A = 0x3F;
    } else {
        anim.unk_2A = 2;
    }
#else
    anim.unk_2A = 2;
#endif
    anim.unk_04 = 0x1C;
    anim.unk_06 = 8;
    _Sprite_Load(&sprites[5], &anim);
}

static s32 Result_numMaxHit_Init(TaskPool* pool, Task* task, void* args) {
    Result_numMaxHit*      numMaxHit     = task->data;
    Result_numMaxHit_Args* numMaxHitArgs = args;

    Result_numMaxHit_Load(numMaxHit, numMaxHit->sprites, numMaxHitArgs);
    numMaxHit->owner = numMaxHitArgs->owner;
    return 1;
}

static s32 Result_numMaxHit_Update(TaskPool* pool, Task* task, void* args) {
    Result_numMaxHit* numMaxHit = task->data;

    for (s32 i = 0; i < 6; i++) {
        Sprite_Update(&numMaxHit->sprites[i]);
    }
    return 1;
}

static s32 Result_numMaxHit_Render(TaskPool* pool, Task* task, void* args) {
    Result_numMaxHit* numMaxHit = task->data;

    for (s32 i = 0; i < 6; i++) {
        if (numMaxHit->visible[i] != 0) {
            Sprite_RenderFrame(&numMaxHit->sprites[i]);
        }
    }
    return 1;
}

static s32 Result_numMaxHit_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_numMaxHit* numMaxHit = task->data;

    for (s32 i = 0; i < 6; i++) {
        Sprite_Release(&numMaxHit->sprites[i]);
    }
    return 1;
}

static s32 Result_numMaxHit_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_numMaxHit_Init,
        .update     = Result_numMaxHit_Update,
        .render     = Result_numMaxHit_Render,
        .cleanup    = Result_numMaxHit_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Result_numMaxHit_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner) {
    Result_numMaxHit_Args args;

    args.dataType = dataType;
    args.value    = owner->maxHits;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_Result_numMaxHit, NULL, 0, NULL, &args);
}
