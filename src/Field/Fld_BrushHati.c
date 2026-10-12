#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "SpriteMgr.h"
#include <nitro/types.h>

extern BinIdentifier data_ov030_020d95b4;

typedef struct {
    s32 dataType;
    s16 animIndex;
} Fld_BrushHati_Args;

SpriteFrameInfo* Fld_BrushHati_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
s32              Fld_BrushHati_Load(Sprite* sprite, Fld_BrushHati_Args* args);
s32              Fld_BrushHati_Init(TaskPool* pool, Task* task, void* args);
s32              Fld_BrushHati_Update(TaskPool* pool, Task* task, void* args);
s32              Fld_BrushHati_Render(TaskPool* pool, Task* task, void* args);
s32              Fld_BrushHati_Destroy(TaskPool* pool, Task* task, void* args);
s32              Fld_BrushHati_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
s32              Fld_BrushHati_CreateTask(TaskPool* pool, s32 dataType, s16 animIndex);

static const TaskHandle Tsk_Fld_BrushHati = {"Tsk_Fld_BrushHati", Fld_BrushHati_RunTask, sizeof(Sprite)};

static const SpriteAnimation Fld_BrushHati_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0800,
    .posX              = 0x80,
    .posY              = 0x60,
    .frameInfoCallback = (SpriteFrameInfoCallback)Fld_BrushHati_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov030_020d95b4,
    .unk_18            = 2,
    .packIndex         = 11,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 1,
};

SpriteFrameInfo* Fld_BrushHati_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case SPRITE_FRAME_UPDATE:
            g_SpriteFrameInfo.updateSteps = 1;
            return &g_SpriteFrameInfo;
        case SPRITE_FRAME_RENDER: {
            SpriteFrameInfo* temp = &g_SpriteFrameInfo;
            Sprite_FillFrameInfo(temp, sprite);
            info = temp;
        } break;
    }
    return info;
}

s32 Fld_BrushHati_Load(Sprite* sprite, Fld_BrushHati_Args* args) {
    SpriteAnimation anim = Fld_BrushHati_Anim;

    anim.dataType = args->dataType;
    anim.animIndex += args->animIndex;
    return _Sprite_Load(sprite, &anim);
}

s32 Fld_BrushHati_Init(TaskPool* pool, Task* task, void* args) {
    Fld_BrushHati_Load(task->data, args);
    return 1;
}

s32 Fld_BrushHati_Update(TaskPool* pool, Task* task, void* args) {
    Sprite_Update(task->data);
    return 1;
}

s32 Fld_BrushHati_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

s32 Fld_BrushHati_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

s32 Fld_BrushHati_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_BrushHati_Init,
        .update     = Fld_BrushHati_Update,
        .render     = Fld_BrushHati_Render,
        .cleanup    = Fld_BrushHati_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_BrushHati_CreateTask(TaskPool* pool, s32 dataType, s16 animIndex) {
    Fld_BrushHati_Args args;
    args.dataType  = dataType;
    args.animIndex = animIndex;
    return EasyTask_CreateTask(pool, &Tsk_Fld_BrushHati, NULL, 0, NULL, &args);
}
