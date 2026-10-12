#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    affineScale;
    /* 0x44 */ s32    affineFlags;
} Fld_BattleIn; // Size: 0x48

typedef struct {
    /* 0x00 */ s32 dataType;
    /* 0x04 */ u16 posX;
    /* 0x06 */ u16 posY;
    /* 0x08 */ s32 flags;
    /* 0x0C */ s32 scale;
    /* 0x10 */ s32 packSet;
} Fld_BattleIn_Args;

s32              Fld_BattleIn_Init(TaskPool* pool, Task* task, void* args);
s32              Fld_BattleIn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
SpriteFrameInfo* Fld_BattleIn_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
void             Fld_BattleIn_Load(Sprite* sprite, Fld_BattleIn_Args* args, Fld_BattleIn* owner);

static const TaskHandle Tsk_Fld_BattleIn = {"Tsk_Fld_BattleIn", Fld_BattleIn_RunTask, sizeof(Fld_BattleIn)};

const s16                    data_ov030_020eca64[] = {0x0E, 0x0F, 0x10, 0x11};
extern BinIdentifier         data_ov030_020d9944;
static const SpriteAnimation Fld_BattleIn_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 2,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x403,
    .posX              = 0x0,
    .posY              = 0x0,
    .frameInfoCallback = Fld_BattleIn_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov030_020d9944,
    .unk_18            = 2,
    .packIndex         = 0,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 1,
};

// Nonmatching: the NULL info value lives in r4 in the target, r3 here
SpriteFrameInfo* Fld_BattleIn_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Fld_BattleIn*    owner = sprite->owner;
    SpriteFrameInfo* info;

    info = NULL;
    switch (mode) {
        case SPRITE_FRAME_UPDATE:
            g_SpriteFrameInfo.updateSteps = 1;
            return &g_SpriteFrameInfo;
        case SPRITE_FRAME_RENDER:
            info = &g_SpriteFrameInfo;
            Sprite_FillFrameInfo(info, sprite);
            sprite->unk_0A.unk_05 =
                OamMgr_AllocAffineGroup(&g_OamMgr[0], 0, owner->affineScale, owner->affineScale, owner->affineFlags);
            break;
    }
    return info;
}

void Fld_BattleIn_Load(Sprite* sprite, Fld_BattleIn_Args* args, Fld_BattleIn* owner) {
    Unk_Bitfield    unk02;
    SpriteAnimation animation = Fld_BattleIn_Anim;

    animation.dataType = args->dataType;
    u16 posY           = args->posY;
    u16 posX           = args->posX;
    animation.posY     = posY;
    animation.posX     = posX;
    if (args->flags != 0) {
        unk02 = animation.unk_02;
        unk02.unk_05 |= 8;
        animation.unk_02 = unk02;
    }
    animation.packIndex = data_ov030_020eca64[args->packSet];
    animation.owner     = owner;
    _Sprite_Load(sprite, &animation);
}

s32 Fld_BattleIn_Init(TaskPool* pool, Task* task, void* args) {
    Fld_BattleIn*      battleIn   = task->data;
    Fld_BattleIn_Args* battleArgs = args;

    battleIn->affineScale = battleArgs->scale;
    battleIn->affineFlags = battleArgs->flags;
    Fld_BattleIn_Load(&battleIn->sprite, battleArgs, battleIn);
    return 1;
}

s32 Fld_BattleIn_Update(TaskPool* pool, Task* task, void* args) {
    Sprite_UpdateAndCheck(task->data);
    return 1;
}

s32 Fld_BattleIn_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_Render(task->data);
    return 1;
}

s32 Fld_BattleIn_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Destroy(task->data);
    return 1;
}

s32 Fld_BattleIn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_BattleIn_Init,
        .update     = Fld_BattleIn_Update,
        .render     = Fld_BattleIn_Render,
        .cleanup    = Fld_BattleIn_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_BattleIn_CreateTask(TaskPool* pool, s32 dataType, u16 posX, u16 posY, s32 flags, s32 scale, s32 packSet) {
    Fld_BattleIn_Args args;
    args.dataType = dataType;
    args.posX     = posX;
    args.posY     = posY;
    args.flags    = flags;
    args.scale    = scale;
    args.packSet  = packSet;
    return EasyTask_CreateTask(pool, &Tsk_Fld_BattleIn, NULL, 0, NULL, &args);
}
