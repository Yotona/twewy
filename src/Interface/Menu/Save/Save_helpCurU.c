#include "Interface/Menu/Save.h"

typedef struct {
    /* 0x00 */ Sprite          sprites[2];
    /* 0x80 */ BOOL            visible[2];
    /* 0x88 */ SaveMenuObject* save;
} Save_helpCurU; // Size: 0x8C

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ SaveMenuObject* save;
} Save_helpCurU_Args;

static SpriteFrameInfo* Save_helpCurU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Save_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Save_helpCurU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02            = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = Save_helpCurU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Save_BinIdentifiers[7],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static const TaskHandle Tsk_Save_helpCurU = {"Tsk_Save_helpCurU", Save_helpCurU_RunTask, sizeof(Save_helpCurU)};

static SpriteFrameInfo* Save_helpCurU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Save_helpCurU_Load(Save_helpCurU* helpCurU, Save_helpCurU_Args* args) {
    SpriteAnimation anim = Save_helpCurU_Anim;
    anim.dataType        = args->dataType;

    anim.animIndex = 33;
    anim.posX      = 7;
    anim.posY      = 96;
    _Sprite_Load(&helpCurU->sprites[0], &anim);

    anim.animIndex = 34;
    anim.posX      = 249;
    anim.posY      = 96;
    _Sprite_Load(&helpCurU->sprites[1], &anim);
}

static s32 Save_helpCurU_Init(TaskPool* pool, Task* task, void* args) {
    Save_helpCurU*      helpCurU     = task->data;
    Save_helpCurU_Args* helpCurUArgs = args;

    helpCurU->save = helpCurUArgs->save;
    Save_helpCurU_Load(helpCurU, helpCurUArgs);
    return 1;
}

static s32 Save_helpCurU_Update(TaskPool* pool, Task* task, void* args) {
    Save_helpCurU* helpCurU = task->data;

    if (helpCurU->save->helpPage == 0) {
        helpCurU->visible[0] = FALSE;
        helpCurU->visible[1] = TRUE;
    } else if (helpCurU->save->helpPage == 3) {
        helpCurU->visible[0] = TRUE;
        helpCurU->visible[1] = FALSE;
    } else {
        helpCurU->visible[0] = TRUE;
        helpCurU->visible[1] = TRUE;
    }

    for (u16 i = 0; i < 2; i++) {
        Sprite_Update(&helpCurU->sprites[i]);
    }
    return 1;
}

static s32 Save_helpCurU_Render(TaskPool* pool, Task* task, void* args) {
    Save_helpCurU* helpCurU = task->data;

    for (u16 i = 0; i < 2; i++) {
        if (helpCurU->visible[i]) {
            Sprite_RenderFrame(&helpCurU->sprites[i]);
        }
    }
    return 1;
}

static s32 Save_helpCurU_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_helpCurU* helpCurU = task->data;

    for (u16 i = 0; i < 2; i++) {
        Sprite_Release(&helpCurU->sprites[i]);
    }
    return 1;
}

static s32 Save_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_helpCurU_Init,
        .update     = Save_helpCurU_Update,
        .render     = Save_helpCurU_Render,
        .cleanup    = Save_helpCurU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_helpCurU_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save) {
    Save_helpCurU_Args args;

    args.dataType = dataType;
    args.save     = save;

    return EasyTask_CreateTask(pool, &Tsk_Save_helpCurU, NULL, 0, NULL, &args);
}
