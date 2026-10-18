#include "Field/Tasks/Fld_NSymbolUGAppear.h"
#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ u16    timer;
    /* 0x42 */ u16    phase;
} Fld_NSymbolUGAppear;

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ u16 posX;
    /* 0x6 */ u16 posY;
    /* 0x8 */ s32 kind;
} Fld_NSymbolUGAppear_Args;

s32              Fld_NSymbolUGAppear_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
SpriteFrameInfo* Fld_NSymbolUGAppear_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle      Tsk_Fld_NSymbolUGAppear  = {"Tsk_Fld_NSymbolUGAppear", Fld_NSymbolUGAppear_RunTask,
                                                         sizeof(Fld_NSymbolUGAppear)};
static const SpriteAnimation Fld_NSymbolUGAppear_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0,
    .posX              = 0x0,
    .posY              = 0x0,
    .frameInfoCallback = Fld_NSymbolUGAppear_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldEncount,
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 51,
    .unk_1E            = 0,
    .unk_20            = 46,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 52,
    .unk_28            = 53,
    .animIndex         = 1,
};

SpriteFrameInfo* Fld_NSymbolUGAppear_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = 0x180 - (sprite->posY * 2));
}

void Fld_NSymbolUGAppear_Load(Sprite* sprite, s32 arg, Fld_NSymbolUGAppear_Args* args) {
    SpriteAnimation animation = Fld_NSymbolUGAppear_Anim;
    animation.dataType        = args->dataType;
    u16 posX                  = args->posX;
    u16 posY                  = args->posY;
    animation.posY            = posY;
    animation.posX            = posX;
    if (args->kind >= 0x1F) {
        animation.unk_20 = 0x32;
    } else if (args->kind >= 0x17) {
        animation.unk_20 = 0x2F;
    } else if (args->kind == 0x16) {
        animation.unk_20 = 0x3A;
    } else if (args->kind == 0x8) {
        animation.unk_20 = 0x30;
    } else if (args->kind >= 0x9) {
        animation.unk_20 = 0x31;
    }
    _Sprite_Load(sprite, &animation);
}

s32 Fld_NSymbolUGAppear_Init(TaskPool* pool, Task* task, void* args) {
    Fld_NSymbolUGAppear*      appear     = task->data;
    Fld_NSymbolUGAppear_Args* appearArgs = args;

    Fld_NSymbolUGAppear_Load(&appear->sprite, appearArgs->dataType, appearArgs);
    appear->timer = 0x18;
    appear->phase = 0;
    return 1;
}

s32 Fld_NSymbolUGAppear_Update(TaskPool* pool, Task* task, void* args) {
    Fld_NSymbolUGAppear* appear = task->data;

    Sprite_UpdateAndCheck(&appear->sprite);
    if (appear->timer != 0) {
        appear->timer--;
    } else if (appear->phase == 0) {
        appear->phase++;
        Sprite_SetAnimation(&appear->sprite, appear->sprite.animData, 2, appear->sprite.cellTable);
        appear->timer = 0x12;
    } else {
        return 0;
    }
    return 1;
}

s32 Fld_NSymbolUGAppear_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_Render(task->data);
    return 1;
}

s32 Fld_NSymbolUGAppear_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Destroy(task->data);
    return 1;
}

s32 Fld_NSymbolUGAppear_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_NSymbolUGAppear_Init,
        .update     = Fld_NSymbolUGAppear_Update,
        .render     = Fld_NSymbolUGAppear_Render,
        .cleanup    = Fld_NSymbolUGAppear_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_NSymbolUGAppear_CreateTask(TaskPool* pool, s32 dataType, s32 posX, s32 posY, s32 kind) {
    Fld_NSymbolUGAppear_Args args;
    args.dataType = dataType;
    args.posX     = posX;
    args.posY     = posY;
    args.kind     = kind;
    return EasyTask_CreateTask(pool, &Tsk_Fld_NSymbolUGAppear, NULL, 0, NULL, &args);
}
