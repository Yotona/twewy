#include "Interface/Menu/Save.h"

typedef struct {
    /* 0x00 */ Sprite          sprite;
    /* 0x40 */ BOOL            visible;
    /* 0x44 */ SaveMenuObject* save;
    /* 0x48 */ u16             pressed;
} Save_saveBtn; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ SaveMenuObject* save;
} Save_saveBtn_Args;

static SpriteFrameInfo* Save_saveBtn_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Save_saveBtn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_saveBtn = {"Tsk_Save_saveBtn", Save_saveBtn_RunTask, sizeof(Save_saveBtn)};

static const SpriteAnimation Save_saveBtn_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = Save_saveBtn_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Save_BinIdentifiers[3],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 13,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* Save_saveBtn_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Save_saveBtn_Load(Save_saveBtn* saveBtn, Save_saveBtn_Args* args) {
    SpriteAnimation anim = Save_saveBtn_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 15;
    anim.posX      = 232;
    anim.posY      = 175;
    _Sprite_Load(&saveBtn->sprite, &anim);
}

static s32 Save_saveBtn_Init(TaskPool* pool, Task* task, void* args) {
    Save_saveBtn*      saveBtn  = task->data;
    Save_saveBtn_Args* initArgs = args;

    saveBtn->visible = TRUE;
    saveBtn->save    = initArgs->save;
    saveBtn->pressed = FALSE;
    Save_saveBtn_Load(saveBtn, initArgs);
    return 1;
}

static s32 Save_saveBtn_Update(TaskPool* pool, Task* task, void* args) {
    Save_saveBtn* saveBtn = task->data;

    if (saveBtn->save->saving == TRUE) {
        if (saveBtn->pressed == FALSE) {
            Save_SetSpriteFrame(&saveBtn->sprite, 16);
            saveBtn->pressed = TRUE;
        }
    } else if (saveBtn->pressed == TRUE) {
        Save_SetSpriteFrame(&saveBtn->sprite, 15);
        saveBtn->pressed = FALSE;
    }
    Sprite_Update(&saveBtn->sprite);
    return 1;
}

static s32 Save_saveBtn_Render(TaskPool* pool, Task* task, void* args) {
    Save_saveBtn* saveBtn = task->data;

    Sprite_RenderFrame(&saveBtn->sprite);
    return 1;
}

static s32 Save_saveBtn_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_saveBtn* saveBtn = task->data;

    Sprite_Release(&saveBtn->sprite);
    return 1;
}

static s32 Save_saveBtn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_saveBtn_Init,
        .update     = Save_saveBtn_Update,
        .render     = Save_saveBtn_Render,
        .cleanup    = Save_saveBtn_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_saveBtn_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save) {
    Save_saveBtn_Args args;

    args.dataType = dataType;
    args.save     = save;

    return EasyTask_CreateTask(pool, &Tsk_Save_saveBtn, NULL, 0, NULL, &args);
}
