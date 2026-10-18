#include "Field/Tasks/Fld_ShinigamiON.h"
#include "Engine/EasyTask.h"
#include "Field/Tasks/Fld_ShinigamiCoin.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s16    unk40; // current animation index
    /* 0x42 */ s16    pad42;
} Ov030ONSprite;             // Size: 0x44

static SpriteFrameInfo* Fld_ShinigamiON_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_ShinigamiON_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_ShinigamiON = {"Tsk_Fld_ShinigamiON", Fld_ShinigamiON_RunTask, sizeof(Ov030ONSprite)};

static const SpriteAnimation Fld_ShinigamiON_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 6,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0400,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_ShinigamiON_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldShinigami,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 0,
};

static SpriteFrameInfo* Fld_ShinigamiON_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Nonmatching: SpriteAnimation build; retail materialises the dataType value
// operand before the posY/posX stores (loading *arg1 first, then bic/orr
// against the anim word), ours sinks the load to just before its own
// read-modify-write (USA 71.97%; JP unvalidated).
static void Fld_ShinigamiON_Load(Sprite* sprite, s32* arg1) {
    SpriteAnimation anim = Fld_ShinigamiON_Anim;

    anim.posY     = 0x60;
    anim.posX     = 0x80;
    anim.dataType = *arg1;
    SndMgr_PlaySEWithPan(0xC7, 0x80);
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_ShinigamiON_Init(TaskPool* pool, Task* task, void* args) {
    s32*           arg2   = args;
    Ov030ONSprite* sprite = task->data;

    sprite->unk40 = 4;
    Fld_ShinigamiON_Load(&sprite->sprite, arg2);
}

static s32 Fld_ShinigamiON_Update(TaskPool* pool, Task* task, void* args) {
    Sprite_Update(task->data);
    return 1;
}

static s32 Fld_ShinigamiON_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

static s32 Fld_ShinigamiON_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

static s32 Fld_ShinigamiON_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_ShinigamiON_Init,
        .update     = Fld_ShinigamiON_Update,
        .render     = Fld_ShinigamiON_Render,
        .cleanup    = Fld_ShinigamiON_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_ShinigamiON_CreateTask(TaskPool* pool, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(pool, &Tsk_Fld_ShinigamiON, NULL, 0, NULL, &param);
}

s32 Fld_ShinigamiON_SetAnimation(Task* task, s16 arg1) {
    Ov030ONSprite* sprite = task->data;

    Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, arg1, sprite->sprite.cellTable);
    sprite->unk40 = arg1;
    return 1;
}
