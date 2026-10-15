#include "Engine/EasyTask.h"
#include "Field/Ov030Data.h"
#include "SpriteMgr.h"

// USA 0x020c248c-0x020c268c: the scan reticle ("moya") task. The task data is a
// single Sprite (0x40); the init args carry the dataType and the screen position.

// Init args: the SpriteAnimation dataType plus the screen position.
typedef struct {
    /* 0x0 */ u32          dataType;
    /* 0x4 */ FldScreenPos pos;
} FldScanMoyaArgs;

typedef TaskStages FldScanMoyaFns;

extern TaskHandle      data_ov030_020e704c;
extern FldScanMoyaFns  data_ov030_020e7058;
extern SpriteAnimation data_ov030_020e7068;

SpriteFrameInfo* func_ov030_020c248c(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Places one scan reticle: the shared animation template plus the dataType and
// the screen position.
void func_ov030_020c2528(Sprite* sprite, FldScanMoyaArgs* args) {
    SpriteAnimation anim = data_ov030_020e7068;
    u32             x;
    u16             pos;

    anim.dataType = (u16)args->dataType;
    pos           = args->pos.x;
    x             = pos;
    pos           = args->pos.y;
    anim.posX     = x;
    anim.posY     = pos;
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020c25a4(TaskPool* pool, Task* task, FldScanMoyaArgs* args) {
    func_ov030_020c2528(task->data, args);
    return 1;
}

s32 func_ov030_020c25bc(TaskPool* pool, Task* task) {
    Sprite_Update(task->data);
    return 1;
}

s32 func_ov030_020c25d0(TaskPool* pool, Task* task) {
    Sprite_RenderFrame(task->data);
    return 1;
}

s32 func_ov030_020c25e4(TaskPool* pool, Task* task) {
    Sprite_Release(task->data);
    return 1;
}

s32 func_ov030_020c25f8(TaskPool* pool, Task* task, void* args, s32 stage) {
    FldScanMoyaFns funcs = data_ov030_020e7058;

    return funcs.iter[stage](pool, task, args);
}

s32 func_ov030_020c2640(TaskPool* pool, s32 dataType, FldScreenPos pos) {
    FldScanMoyaArgs params;

    params.dataType = dataType;
    params.pos      = pos;
    return EasyTask_CreateTask(pool, &data_ov030_020e704c, NULL, 0, NULL, &params);
}
