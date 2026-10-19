#include "Field/Tasks/Fld_TouchShine.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Event/EventTables.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ s32 dataType;
} Ov030TouchShineConfig;

static s16 Fld_TouchShine_ActiveCount;

static SpriteFrameInfo* Fld_TouchShine_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_TouchShine_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_TouchShine = {"Tsk_Fld_TouchShine", Fld_TouchShine_RunTask, sizeof(Sprite)};

static const SpriteAnimation Fld_TouchShine_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 0,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0400,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_TouchShine_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldTouch,
    .unk_18            = 2,
    .packIndex         = 2,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 4,
};

static SpriteFrameInfo* Fld_TouchShine_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Fld_TouchShine_Load(Sprite* sprite, Ov030TouchShineConfig* arg1) {
    SpriteAnimation anim = Fld_TouchShine_Anim;
    TouchCoord      coord;

    anim.dataType = arg1->dataType;
    TouchInput_GetCoord(&coord);
    anim.posX = coord.x;
    anim.posY = coord.y;
    SndMgr_PlaySEWithPan(0xC7, anim.posX);
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_TouchShine_Init(TaskPool* pool, Task* task, void* args) {
    Ov030TouchShineConfig* arg2   = args;
    Sprite*                sprite = task->data;

    Fld_TouchShine_ActiveCount += 1;
    Fld_TouchShine_Load(sprite, arg2);
}

static s32 Fld_TouchShine_Update(TaskPool* pool, Task* task, void* args) {
    Sprite* sprite = task->data;

    Sprite_Update(sprite);
    return Sprite_HasAnimation(sprite) != 0;
}

static s32 Fld_TouchShine_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

static s32 Fld_TouchShine_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    Fld_TouchShine_ActiveCount -= 1;
    return 1;
}

static s32 Fld_TouchShine_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TouchShine_Init,
        .update     = Fld_TouchShine_Update,
        .render     = Fld_TouchShine_Render,
        .cleanup    = Fld_TouchShine_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_TouchShine_CreateTask(TaskPool* pool, s32 arg1) {
    s32 param;

    param = arg1;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TouchShine, NULL, 0, NULL, &param);
}

void Fld_TouchShine_SetActiveCount(s16 arg0) {
    Fld_TouchShine_ActiveCount = arg0;
}

s16 Fld_TouchShine_GetActiveCount(void) {
    return Fld_TouchShine_ActiveCount;
}
