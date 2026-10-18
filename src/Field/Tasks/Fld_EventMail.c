#include "Field/Tasks/Fld_EventMail.h"
#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "SpriteMgr.h"

static SpriteFrameInfo* Fld_EventMail_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_EventMail_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_EventMail = {"Tsk_Fld_EventMail", Fld_EventMail_RunTask, sizeof(Sprite)};

static const SpriteAnimation Fld_EventMail_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0C00,
    .posX              = 14,
    .posY              = 10,
    .frameInfoCallback = Fld_EventMail_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldDownScreen,
    .unk_18            = 2,
    .packIndex         = 4,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 29,
};

static SpriteFrameInfo* Fld_EventMail_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Fld_EventMail_Load(Sprite* sprite, s32* dataType) {
    SpriteAnimation anim = Fld_EventMail_Anim;

    anim.dataType = *dataType;
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_EventMail_Init(TaskPool* pool, Task* task, void* args) {
    s32* dataType = args;
    Fld_EventMail_Load(task->data, dataType);
    return 1;
}

static s32 Fld_EventMail_Update(TaskPool* pool, Task* task, void* args) {
    Sprite_Update(task->data);
    return 1;
}

static s32 Fld_EventMail_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

static s32 Fld_EventMail_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

static s32 Fld_EventMail_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_EventMail_Init,
        .update     = Fld_EventMail_Update,
        .render     = Fld_EventMail_Render,
        .cleanup    = Fld_EventMail_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_EventMail_CreateTask(TaskPool* pool, s32 dataType) {
    s32 param = dataType;

    return EasyTask_CreateTask(pool, &Tsk_Fld_EventMail, NULL, 0, NULL, &param);
}

void Fld_EventMail_Press(Task* task) {
    Sprite* sprite = task->data;

    Sprite_SetAnimation(sprite, sprite->animData, 0x1E, sprite->cellTable);
}
