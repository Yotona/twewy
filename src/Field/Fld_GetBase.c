#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Field/FieldTasks.h"
#include "SpriteMgr.h"

extern BinIdentifier data_ov030_020d9944;

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk_40;
    /* 0x44 */ u16    unk_44;
} Fld_GetBase; // Size: 0x48

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
} Fld_GetBase_Args;

SpriteFrameInfo* Fld_GetBase_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
s32              Fld_GetBase_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_GetBase = {"Tsk_Fld_GetBase", Fld_GetBase_RunTask, 0x48};

static const SpriteAnimation data_ov030_020ec980 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 1,
    .unk_04     = 0x80,
    .unk_06     = 0x51,
    .unk_08     = Fld_GetBase_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov030_020d9944,
    .unk_18     = 2,
    .packIndex  = 11,
    .unk_1C     = 2,
    .unk_1E     = 0,
    .unk_20     = 1,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 3,
    .unk_28     = 4,
    .unk_2A     = 1,
};

SpriteFrameInfo* Fld_GetBase_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    // Not yet implemented
}

void Fld_GetBase_Load(Sprite* sprite, Fld_GetBase_Args* args, void* arg2) {
    SpriteAnimation anim = data_ov030_020ec980;

    anim.dataType = args->unk_0;
    anim.unk_10   = arg2;
    if (args->unk_4 != 0) {
        anim.unk_2A = 4;
    }
    _Sprite_Load(sprite, &anim);
}

s32 Fld_GetBase_Init(TaskPool* pool, Task* task, void* args) {
    Fld_GetBase* base = task->data;

    Fld_GetBase_Load(&base->sprite, args, base);
    base->unk_44 = 20;
    base->unk_40 = 0;
    return 1;
}

s32 Fld_GetBase_Update(TaskPool* pool, Task* task, void* args) {
    Fld_GetBase* base = task->data;

    if (base->unk_44 != 0) {
        func_02026590(&base->unk_40, 0x1000, base->unk_44);
        base->unk_44--;
    }
    Sprite_Update(&base->sprite);
    return 1;
}

s32 Fld_GetBase_Render(TaskPool* pool, Task* task, void* args) {
    Fld_GetBase* base = task->data;

    Sprite_RenderFrame(&base->sprite);
    return 1;
}

s32 Fld_GetBase_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_GetBase* base = task->data;

    Sprite_Destroy(&base->sprite);
    return 1;
}

s32 Fld_GetBase_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_GetBase_Init,
        .update     = Fld_GetBase_Update,
        .render     = Fld_GetBase_Render,
        .cleanup    = Fld_GetBase_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_GetBase_CreateTask(TaskPool* arg0, s32 arg1, s32 arg2) {
    Fld_GetBase_Args args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    return EasyTask_CreateTask(arg0, &Tsk_Fld_GetBase, NULL, 0, NULL, &args);
}