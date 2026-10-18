#include "Field/Tasks/Fld_TalkIcon.h"
#include "Engine/EasyTask.h"
#include "Field/Battle/FieldBattleTransition.h"
#include "Field/Event/EventTables.h"
#include "Field/FieldTypes.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite    sprite;
    /* 0x40 */ FieldPos* screen;
    /* 0x44 */ s32*      scale;
    /* 0x48 */ s32       height;
} TalkIconSprite; // Size: 0x4C

typedef struct {
    /* 0x00 */ s32       dataType;
    /* 0x04 */ FieldPos* screen;
    /* 0x08 */ s32*      scale;
    /* 0x0C */ s32       height;
    /* 0x10 */ s32       animIndex;
} TalkIconArgs; // Size: 0x14

static SpriteFrameInfo* Fld_TalkIcon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_TalkIcon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_TalkIcon = {"Tsk_Fld_TalkIcon", Fld_TalkIcon_RunTask, sizeof(TalkIconSprite)};

static const SpriteAnimation Fld_TalkIcon_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0C00,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_TalkIcon_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldDownScreen,
    .unk_18            = 2,
    .packIndex         = 2,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 7,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* Fld_TalkIcon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = 0xC0 - sprite->posY)
}

static void Fld_TalkIcon_Load(Sprite* sprite, TalkIconArgs* args) {
    SpriteAnimation anim = Fld_TalkIcon_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = args->animIndex;
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_TalkIcon_Init(TaskPool* pool, Task* task, void* args) {
    TalkIconArgs*   params = args;
    TalkIconSprite* sprite = task->data;

    sprite->screen = params->screen;
    sprite->scale  = params->scale;
    sprite->height = params->height;
    Fld_TalkIcon_Load(&sprite->sprite, params);
    if (SndMgr_IsSEPlaying((SndMgrSeIdx)0xC3) == 0 && gSaveData.unk_2498 == 0) {
        SndMgr_StartPlayingSE((SndMgrSeIdx)0xC3);
    }
    return 1;
}

static s32 Fld_TalkIcon_Update(TaskPool* pool, Task* task, void* args) {
    Sprite_UpdateAndCheck(task->data);
    return 1;
}

static s32 Fld_TalkIcon_Render(TaskPool* pool, Task* task, void* args) {
    TalkIconSprite* sprite = task->data;
    s32             offset = (s32)(((s64)sprite->height * *sprite->scale + 0x800) >> 0xC);

    sprite->sprite.posX = sprite->screen->x >> 0xC;
    sprite->sprite.posY = (sprite->screen->y - offset) >> 0xC;
    if (Field_IsTalkEventActive() == 0) {
        Sprite_Render(&sprite->sprite);
    }
    return 1;
}

static s32 Fld_TalkIcon_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Destroy(task->data);
    return 1;
}

static s32 Fld_TalkIcon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TalkIcon_Init,
        .update     = Fld_TalkIcon_Update,
        .render     = Fld_TalkIcon_Render,
        .cleanup    = Fld_TalkIcon_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_TalkIcon_CreateTask(TaskPool* pool, s32 dataType, FieldPos* screen, s32* scale, s32 height, s32 animIndex) {
    TalkIconArgs params;

    params.dataType  = dataType;
    params.screen    = screen;
    params.scale     = scale;
    params.height    = height;
    params.animIndex = animIndex;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TalkIcon, NULL, 0, NULL, &params);
}

void Fld_TalkIcon_Highlight(Task* task) {
    TalkIconSprite* sprite = task->data;

    if (sprite->sprite.animIndex == 1) {
        Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, 2, sprite->sprite.cellTable);
    } else if (sprite->sprite.animIndex == 3) {
        Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, 4, sprite->sprite.cellTable);
    }
}

void Fld_TalkIcon_Unhighlight(Task* task) {
    TalkIconSprite* sprite = task->data;

    if (sprite->sprite.animIndex == 2) {
        Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, 1, sprite->sprite.cellTable);
    } else if (sprite->sprite.animIndex == 4) {
        Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, 3, sprite->sprite.cellTable);
    }
}
