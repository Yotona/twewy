#include "Field/Tasks/Fld_ScanMoya.h"
#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "Field/FieldTypes.h"
#include "SpriteMgr.h"

// Init args: the SpriteAnimation dataType plus the screen position.
typedef struct {
    /* 0x0 */ u32          dataType;
    /* 0x4 */ FldScreenPos pos;
} FldScanMoyaArgs;

static SpriteFrameInfo* Fld_ScanMoya_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_ScanMoya_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_ScanMoya = {"Tsk_Fld_ScanMoya", Fld_ScanMoya_RunTask, sizeof(Sprite)};

static const SpriteAnimation Fld_ScanMoya_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0400,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_ScanMoya_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldScan,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 6,
    .unk_1E            = 0,
    .unk_20            = 5,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 7,
    .unk_28            = 8,
    .animIndex         = 1,
};

static SpriteFrameInfo* Fld_ScanMoya_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Places one scan reticle.
static void Fld_ScanMoya_Load(Sprite* sprite, FldScanMoyaArgs* args) {
    SpriteAnimation anim = Fld_ScanMoya_Anim;
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

static s32 Fld_ScanMoya_Init(TaskPool* pool, Task* task, void* args) {
    FldScanMoyaArgs* params = args;
    Fld_ScanMoya_Load(task->data, params);
    return 1;
}

static s32 Fld_ScanMoya_Update(TaskPool* pool, Task* task, void* args) {
    Sprite_Update(task->data);
    return 1;
}

static s32 Fld_ScanMoya_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

static s32 Fld_ScanMoya_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

static s32 Fld_ScanMoya_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_ScanMoya_Init,
        .update     = Fld_ScanMoya_Update,
        .render     = Fld_ScanMoya_Render,
        .cleanup    = Fld_ScanMoya_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_ScanMoya_CreateTask(TaskPool* pool, s32 dataType, FldScreenPos pos) {
    FldScanMoyaArgs params;

    params.dataType = dataType;
    params.pos      = pos;
    return EasyTask_CreateTask(pool, &Tsk_Fld_ScanMoya, NULL, 0, NULL, &params);
}
