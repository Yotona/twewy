// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020c8868-0x020c8a58; 9 functions.
// Research draft: docs/decomp/ov030/Fld_EventMail.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

typedef TaskStages Ov030EventMailFns;

extern SpriteAnimation   data_ov030_020ecad0;
extern Ov030EventMailFns data_ov030_020ecac0;
extern TaskHandle        data_ov030_020ecab4;

SpriteFrameInfo* func_ov030_020c8868(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void func_ov030_020c8904(Sprite* sprite, s32* dataType) {
    SpriteAnimation anim = data_ov030_020ecad0;

    anim.dataType = *dataType;
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020c8970(TaskPool* pool, Task* task, s32* dataType) {
    func_ov030_020c8904(task->data, dataType);
    return 1;
}

s32 func_ov030_020c8988(TaskPool* pool, Task* task) {
    Sprite_Update(task->data);
    return 1;
}

s32 func_ov030_020c899c(TaskPool* pool, Task* task) {
    Sprite_RenderFrame(task->data);
    return 1;
}

s32 func_ov030_020c89b0(TaskPool* pool, Task* task) {
    Sprite_Release(task->data);
    return 1;
}

s32 func_ov030_020c89c4(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov030EventMailFns funcs = data_ov030_020ecac0;

    return funcs.iter[stage](pool, task, args);
}

s32 func_ov030_020c8a0c(TaskPool* pool, s32 dataType) {
    s32 param = dataType;

    return EasyTask_CreateTask(pool, &data_ov030_020ecab4, NULL, 0, NULL, &param);
}

void func_ov030_020c8a3c(Task* task) {
    Sprite* sprite = task->data;

    Sprite_SetAnimation(sprite, sprite->animData, 0x1E, sprite->cellTable);
}
