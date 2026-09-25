#include "Engine/Math/Random.h"
#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ BOOL          visible;
    /* 0x44 */ ResultObject* owner;
    /* 0x48 */ s32           posX;
    /* 0x4C */ s32           posY;
    /* 0x50 */ s32           velocityX;
    /* 0x54 */ s32           velocityY;
    /* 0x58 */ s32           accelX;
    /* 0x5C */ s32           accelY;
    /* 0x60 */ u16           lifeTimer;
} Result_star; // Size: 0x64

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           index;
    /* 0xA */ u16           kind;
} Result_star_Args;

static SpriteFrameInfo* Result_star_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_star_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Result_star_Anim = {
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
    .frameInfoCallback = Result_star_GetFrameInfo,
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

static SpriteFrameInfo* Result_star_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x384000);
}

static void Result_star_Load(Result_star* star, Sprite* sprite, Result_star_Args* args) {
    SpriteAnimation anim    = Result_star_Anim;
    s16             posX[6] = {0x2C, 0x4E, 0x70, 0x92, 0xB4, 0xD6};

    anim.dataType = args->dataType;
    anim.bits_7_9 = 6;
    anim.unk_04   = posX[args->index];
    anim.unk_06   = 0xA9;

    if (args->kind == 0) {
        anim.unk_2A = 2;
    } else {
        anim.unk_2A = 4;
    }

    star->posX      = anim.unk_04 << 12;
    star->velocityX = (s16)(RNG_Next(0xFFFF) % 30 - 15) * 0x4CD;
    star->accelX    = 0;
    star->posY      = anim.unk_06 << 12;
    star->velocityY = (s16)(RNG_Next(0xFFFF) % 30 - 15) * 0x4CD;
    star->accelY    = 0;
    star->lifeTimer = RNG_Next(0xFFFF) % 20 + 10;
    star->visible   = TRUE;

    _Sprite_Load(sprite, &anim);
}

static s32 Result_star_Init(TaskPool* pool, Task* task, void* args) {
    Result_star*      star     = task->data;
    Result_star_Args* starArgs = args;

    star->owner = starArgs->owner;
    Result_star_Load(star, &star->sprite, starArgs);
    return 1;
}

static s32 Result_star_Update(TaskPool* pool, Task* task, void* args) {
    Result_star* star = task->data;

    star->posX += star->velocityX;
    star->posY += star->velocityY;
    star->velocityX += star->accelX;
    star->velocityY += star->accelY;

    star->sprite.posX = star->posX >> 12;
    star->sprite.posY = star->posY >> 12;

    if (star->lifeTimer != 0) {
        star->lifeTimer--;
        if (star->lifeTimer == 0) {
            return 0;
        }
    }

    if (star->sprite.posX < -10 || star->sprite.posX > 0x10A) {
        return 0;
    }

    if (star->sprite.posY < -10 || star->sprite.posY > 0xCA) {
        return 0;
    }

    Sprite_Update(&star->sprite);
    return 1;
}

static s32 Result_star_Render(TaskPool* pool, Task* task, void* args) {
    Result_star* star = task->data;

    if (star->visible != 0) {
        Sprite_RenderFrame(&star->sprite);
    }
    return 1;
}

static s32 Result_star_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_star* star = task->data;

    Sprite_Release(&star->sprite);
    return 1;
}

static s32 Result_star_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_star_Init,
        .update     = Result_star_Update,
        .render     = Result_star_Render,
        .cleanup    = Result_star_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Result_star = {"Tsk_Result_star", Result_star_RunTask, sizeof(Result_star)};

s32 Result_star_CreateTask(TaskPool* pool, s32 dataType, u16 index, u16 kind, ResultObject* owner) {
    Result_star_Args args;

    args.dataType = dataType;
    args.owner    = owner;
    args.index    = index;
    args.kind     = kind;

    return EasyTask_CreateTask(pool, &Tsk_Result_star, NULL, 0, NULL, &args);
}
