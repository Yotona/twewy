#include "Interface/Menu/Save.h"

s32 Inventory_IsHelpSeen(s32);

typedef struct {
    /* 0x00 */ Sprite          sprites[3];
    /* 0xC0 */ BOOL            visible[3];
    /* 0xCC */ SaveMenuObject* save;
} Save_helpCur; // Size: 0xD0

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ SaveMenuObject* save;
} Save_helpCur_Args;

static SpriteFrameInfo* Save_helpCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Save_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_helpCur = {"Tsk_Save_helpCur", Save_helpCur_RunTask, sizeof(Save_helpCur)};

static const SpriteAnimation Save_helpCur_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = Save_helpCur_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Save_BinIdentifiers[4],
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

static SpriteFrameInfo* Save_helpCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Save_helpCur_Load(Save_helpCur* helpCur, Sprite* sprites, Save_helpCur_Args* args) {
    SpriteAnimation anim = Save_helpCur_Anim;

    anim.dataType = args->dataType;
    for (u16 i = 0; i < 3; i++) {
        helpCur->visible[i] = TRUE;
    }

    anim.animIndex = 1;
    anim.posX      = 48;
    anim.posY      = 88;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 3;
    anim.posX      = 208;
    anim.posY      = 88;
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 5;
    anim.posX      = 128;
    anim.posY      = 125;
    _Sprite_Load(&sprites[2], &anim);

    if (Inventory_IsHelpSeen(7) == 0) {
        helpCur->visible[2] = FALSE;
    }
}

static s32 Save_helpCur_Init(TaskPool* pool, Task* task, void* args) {
    Save_helpCur_Args* helpCurArgs = args;
    Save_helpCur*      helpCur     = task->data;

    helpCur->save = helpCurArgs->save;
    Save_helpCur_Load(helpCur, helpCur->sprites, helpCurArgs);
    return 1;
}

static s32 Save_helpCur_Update(TaskPool* pool, Task* task, void* args) {
    Save_helpCur*   helpCur = task->data;
    SaveMenuObject* save    = helpCur->save;
    s32             i;

    for (i = 0; i < 3; i++) {
        if (save->helpButtonPressed[i] == 1) {
            Save_SetSpriteFrame(&helpCur->sprites[i], i * 2 + 2);
            if (save->iconTimer != 0) {
                save->iconTimer--;
            } else {
                save->helpButtonPressed[i] = 0;
            }
        } else {
            Save_SetSpriteFrame(&helpCur->sprites[i], i * 2 + 1);
        }
    }

    if (save->helpPage == 0) {
        helpCur->visible[0] = FALSE;
        helpCur->visible[1] = TRUE;
    } else if (save->helpPage == 3) {
        helpCur->visible[0] = TRUE;
        helpCur->visible[1] = FALSE;
    } else {
        helpCur->visible[0] = TRUE;
        helpCur->visible[1] = TRUE;
    }

    if (Inventory_IsHelpSeen(7) == 1) {
        helpCur->visible[2] = TRUE;
    }

    for (i = 0; i < 3; i++) {
        Sprite_Update(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 Save_helpCur_Render(TaskPool* pool, Task* task, void* args) {
    Save_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        if (helpCur->visible[i]) {
            Sprite_RenderFrame(&helpCur->sprites[i]);
        }
    }
    return 1;
}

static s32 Save_helpCur_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 Save_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_helpCur_Init,
        .update     = Save_helpCur_Update,
        .render     = Save_helpCur_Render,
        .cleanup    = Save_helpCur_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_helpCur_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save) {
    Save_helpCur_Args args;

    args.dataType = dataType;
    args.save     = save;

    return EasyTask_CreateTask(pool, &Tsk_Save_helpCur, NULL, 0, NULL, &args);
}
