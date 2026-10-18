#include "Field/Tasks/Fld_Piranha.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Engine/Math/Random.h"
#include "Field/FieldTypes.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite    sprite;
    /* 0x40 */ s32       x; // fx32 map position
    /* 0x44 */ s32       y;
    /* 0x48 */ u8        unk_48[0x8];
    /* 0x50 */ FieldPos* origin;
    /* 0x54 */ s32       index;
    /* 0x58 */ u8        unk_58[0x4];
    /* 0x5C */ s32       angle;
    /* 0x60 */ u8        unk_60[0x4];
} Fld_Piranha; // Size: 0x64

typedef struct {
    /* 0x0 */ s32       dataType;
    /* 0x4 */ FieldPos* origin;
    /* 0x8 */ s32       index;
} Fld_Piranha_Args;

s32 Fld_Piranha_Init(TaskPool* pool, Task* task, void* args);
s32 Fld_Piranha_GetAngleToPlayer(Fld_Piranha* obj);
s32 Fld_Piranha_Update(TaskPool* pool, Task* task, void* args);
s32 Fld_Piranha_Render(TaskPool* pool, Task* task, void* args);

static s32              Fld_Piranha_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* Fld_Piranha_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const BinIdentifier   Iden_FldPiranha  = {30, "Apl_Kit/GRP_FldPiranha.bin"};
static const TaskHandle      Tsk_Fld_Piranha  = {"Tsk_Fld_Piranha", Fld_Piranha_RunTask, sizeof(Fld_Piranha)};
static const SpriteAnimation Fld_Piranha_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0,
    .posX              = 0x0,
    .posY              = 0x0,
    .frameInfoCallback = Fld_Piranha_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldPiranha,
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

static s32 Fld_Piranha_AngleToAnim(s32 angle) {
    if (angle < 0x1000) {
        return 1;
    }
    if (angle < 0x4000) {
        return 7;
    }
    if (angle < 0x7000) {
        return 8;
    }
    if (angle < 0x9000) {
        return 2;
    }
    if (angle < 0xC000) {
        return 5;
    }
    if (angle < 0xF000) {
        return 4;
    }
    return 1;
}

void Fld_Piranha_Load(Fld_Piranha* obj, Sprite* sprite, Fld_Piranha_Args* args) {
    SpriteAnimation anim = Fld_Piranha_Anim;

    anim.dataType  = (u16)args->dataType;
    anim.posX      = (obj->x - obj->origin->x) >> 12;
    anim.posY      = (obj->y - obj->origin->y) >> 12;
    obj->angle     = Fld_Piranha_GetAngleToPlayer(obj);
    anim.animIndex = Fld_Piranha_AngleToAnim(obj->angle);
    if (obj->index < 21) {
        anim.unk_20 = 5;
    } else if (obj->index < 42) {
        anim.unk_20 = 4;
    } else {
        anim.unk_20 = 6;
    }
    _Sprite_Load(sprite, &anim);
    Sprite_Update(&obj->sprite);
    obj->sprite.frameTimer = RNG_Next(310) / 10 + 1;
}

static SpriteFrameInfo* Fld_Piranha_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = -0x8C000);
}

s32 Fld_Piranha_GetAnimGroupEnd(Sprite* sprite) {
    s32 animIndex = sprite->animIndex;

    if (animIndex <= 3) {
        return 3;
    }
    if (animIndex <= 6) {
        return 6;
    }
    return 9;
}

static s32 Fld_Piranha_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return TRUE;
}

static s32 Fld_Piranha_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Piranha_Init,
        .update     = Fld_Piranha_Update,
        .render     = Fld_Piranha_Render,
        .cleanup    = Fld_Piranha_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_Piranha_CreateTask(TaskPool* pool, s32 dataType, FieldPos* origin, s32 index) {
    Fld_Piranha_Args args;

    args.dataType = dataType;
    args.origin   = origin;
    args.index    = index;
    return EasyTask_CreateTask(pool, &Tsk_Fld_Piranha, NULL, 0, NULL, &args);
}

static const u16 Field_EncounterCategories[1276] = {
    1,  1,  0,  0,  0,  0,  0,  13, 13, 13, 10, 0,  0,  0,  0,  11, 0,  0,  0,  9,  12, 0,  0,  0,  0,  0,  21, 21, 21, 21, 21,
    21, 0,  21, 0,  0,  15, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  17, 17, 0,  0,  0,  0,  0,  0,  4,  4,  4,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  4,  15, 0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  4,  4,  0,  0,
    0,  0,  0,  0,  0,  1,  1,  1,  1,  2,  4,  5,  2,  4,  5,  2,  4,  5,  2,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  1,  4,  2,  1,  1,  6,  6,  2,  4,  4,
    1,  4,  2,  1,  4,  2,  5,  4,  6,  6,  0,  6,  0,  4,  4,  4,  1,  1,  1,  1,  2,  4,  4,  1,  2,  4,  4,  4,  5,  5,  1,
    1,  2,  2,  6,  0,  6,  0,  2,  4,  2,  0,  2,  4,  0,  4,  1,  2,  4,  0,  4,  0,  5,  5,  4,  2,  2,  4,  4,  6,  2,  6,
    6,  4,  4,  2,  2,  4,  2,  4,  2,  5,  2,  1,  1,  2,  1,  6,  6,  6,  0,  1,  1,  1,  4,  6,  0,  6,  6,  6,  4,  6,  6,
    4,  4,  5,  5,  1,  2,  1,  2,  6,  6,  1,  2,  4,  2,  4,  2,  4,  0,  4,  6,  4,  4,  5,  1,  1,  5,  1,  4,  1,  1,  24,
    27, 24, 2,  1,  25, 27, 5,  5,  5,  1,  5,  5,  1,  4,  4,  0,  5,  4,  5,  4,  27, 24, 25, 6,  6,  1,  6,  1,  1,  6,  6,
    6,  1,  4,  6,  27, 24, 23, 2,  2,  4,  2,  4,  4,  1,  4,  4,  1,  1,  5,  4,  4,  5,  1,  1,  4,  4,  5,  1,  1,  4,  23,
    25, 24, 6,  6,  4,  4,  5,  4,  5,  4,  5,  4,  6,  1,  25, 27, 25, 5,  5,  1,  5,  1,  1,  0,  4,  4,  1,  4,  1,  1,  5,
    4,  5,  1,  1,  1,  4,  27, 24, 23, 2,  4,  4,  1,  4,  1,  4,  1,  5,  5,  5,  0,  5,  5,  5,  4,  25, 24, 4,  6,  6,  1,
    1,  1,  5,  1,  5,  4,  5,  5,  1,  1,  4,  4,  5,  4,  5,  5,  5,  1,  1,  1,  1,  5,  5,  5,  1,  4,  4,  5,  1,  4,  1,
    5,  5,  4,  4,  5,  4,  4,  4,  4,  4,  4,  4,  1,  4,  4,  5,  4,  5,  6,  6,  6,  4,  6,  6,  6,  6,  5,  5,  5,  4,  4,
    4,  5,  4,  5,  1,  5,  5,  5,  5,  0,  1,  1,  2,  2,  24, 24, 24, 4,  5,  1,  4,  4,  0,  4,  0,  1,  4,  2,  23, 27, 24,
    4,  1,  1,  1,  4,  1,  1,  1,  1,  2,  2,  27, 24, 25, 5,  4,  6,  1,  5,  4,  5,  4,  5,  4,  4,  2,  5,  1,  7,  7,  1,
    5,  7,  7,  5,  1,  7,  7,  2,  1,  27, 27, 24, 7,  7,  1,  1,  26, 24, 7,  7,  0,  3,  26, 24, 7,  7,  3,  4,  4,  4,  2,
    1,  2,  4,  7,  7,  4,  4,  7,  7,  1,  4,  7,  7,  5,  2,  24, 27, 27, 7,  7,  5,  0,  25, 27, 7,  7,  0,  2,  27, 25, 7,
    7,  3,  4,  4,  4,  1,  4,  7,  7,  4,  4,  7,  7,  3,  1,  24, 24, 27, 7,  7,  6,  5,  26, 27, 7,  7,  6,  6,  26, 27, 7,
    7,  6,  3,  1,  2,  4,  0,  4,  2,  7,  7,  1,  4,  7,  7,  1,  1,  7,  7,  2,  4,  24, 24, 24, 7,  7,  1,  0,  25, 24, 7,
    7,  1,  0,  24, 24, 7,  7,  1,  4,  4,  4,  4,  4,  7,  7,  1,  5,  7,  7,  3,  4,  27, 27, 24, 7,  7,  5,  1,  26, 28, 7,
    7,  3,  0,  24, 27, 7,  7,  0,  3,  4,  5,  1,  5,  7,  7,  1,  5,  7,  7,  5,  5,  24, 27, 27, 7,  7,  6,  6,  24, 24, 7,
    7,  6,  6,  25, 24, 7,  7,  6,  6,  1,  1,  4,  4,  7,  7,  5,  1,  7,  7,  5,  3,  27, 27, 24, 7,  7,  1,  5,  24, 27, 7,
    7,  1,  1,  24, 27, 7,  7,  0,  3,  5,  5,  5,  1,  24, 24, 24, 7,  7,  5,  1,  26, 24, 7,  7,  3,  3,  26, 24, 7,  7,  3,
    3,  4,  4,  1,  4,  7,  7,  1,  1,  7,  7,  1,  5,  24, 27, 27, 7,  7,  0,  1,  26, 27, 7,  7,  3,  1,  24, 24, 7,  7,  1,
    1,  5,  5,  5,  5,  27, 24, 27, 7,  7,  5,  6,  24, 24, 7,  7,  0,  5,  24, 27, 7,  7,  4,  4,  1,  5,  1,  2,  7,  7,  5,
    6,  24, 27, 24, 7,  7,  6,  6,  25, 24, 7,  7,  6,  6,  25, 27, 7,  7,  4,  3,  5,  5,  2,  5,  7,  7,  5,  5,  27, 27, 24,
    7,  7,  5,  5,  25, 26, 7,  7,  1,  3,  27, 26, 7,  7,  3,  3,  5,  2,  5,  5,  7,  7,  5,  5,  27, 24, 27, 7,  7,  1,  5,
    25, 26, 7,  7,  5,  1,  25, 25, 7,  7,  5,  4,  0,  1,  5,  5,  24, 27, 25, 7,  7,  1,  0,  25, 24, 7,  7,  4,  1,  5,  2,
    27, 3,  5,  2,  3,  5,  26, 24, 25, 7,  7,  1,  3,  25, 24, 7,  7,  5,  5,  1,  1,  3,  5,  1,  6,  6,  6,  24, 27, 26, 7,
    7,  6,  5,  27, 27, 7,  7,  4,  1,  4,  0,  4,  2,  2,  4,  7,  7,  4,  4,  7,  7,  1,  0,  7,  7,  6,  2,  24, 24, 24, 7,
    7,  6,  6,  26, 24, 7,  7,  6,  6,  27, 24, 7,  7,  6,  6,  6,  2,  6,  5,  6,  5,  7,  7,  6,  5,  7,  7,  5,  6,  7,  7,
    5,  5,  27, 27, 27, 7,  7,  2,  3,  26, 25, 7,  7,  3,  5,  25, 27, 7,  7,  4,  4,  5,  5,  5,  5,  7,  7,  5,  5,  7,  7,
    5,  1,  7,  7,  2,  5,  24, 27, 27, 7,  7,  6,  1,  24, 26, 7,  7,  1,  1,  25, 24, 7,  7,  4,  6,  6,  6,  1,  6,  7,  7,
    6,  1,  7,  7,  0,  5,  7,  7,  3,  5,  24, 27, 24, 7,  7,  1,  1,  24, 27, 7,  7,  3,  1,  24, 27, 7,  7,  3,  3,  2,  1,
    1,  2,  7,  7,  1,  4,  7,  7,  2,  2,  7,  7,  1,  3,  24, 27, 24, 7,  7,  1,  2,  25, 26, 7,  7,  1,  1,  24, 25, 7,  7,
    3,  4,  4,  1,  3,  4,  26, 27, 27, 7,  7,  4,  4,  4,  6,  6,  6,  6,  3,  3,  3,  3,  4,  1,  0,  1,  3,  4,  3,  4,  5,
    4,  5,  4,  1,  1,  1,  1,  4,  3,  3,  4,  1,  1,  1,  4,  3,  4,  4,  4,  1,  4,  0,  4,  3,  3,  4,  4,  0,  3,  0,  4,
    3,  4,  3,  4,  0,
};

u16 Field_GetEncounterCategory(s32 encounter) {
    return Field_EncounterCategories[encounter];
}
