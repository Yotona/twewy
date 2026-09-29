#include "Interface/Menu/TusinSet.h"

s32 Inventory_IsHelpSeen(s32);

typedef struct {
    /* 0x00 */ Sprite          sprites[3];
    /* 0xC0 */ BOOL            visible[3];
    /* 0xCC */ TusinSetObject* tusinSet;
} TusinSet_helpCur; // Size: 0xD0

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_helpCur_Args;

static SpriteFrameInfo* TusinSet_helpCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_helpCur = {"Tsk_TusinSet_helpCur", TusinSet_helpCur_RunTask, sizeof(TusinSet_helpCur)};

static const SpriteAnimation TusinSet_helpCur_Anim = {
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
    .frameInfoCallback = TusinSet_helpCur_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &TusinSet_BinIdentifiers[4],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 6,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* TusinSet_helpCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void TusinSet_helpCur_Load(TusinSet_helpCur* helpCur, Sprite* sprites, TusinSet_helpCur_Args* args) {
    SpriteAnimation anim = TusinSet_helpCur_Anim;

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

    if (Inventory_IsHelpSeen(6) == 0) {
        helpCur->visible[2] = FALSE;
    }
}

static s32 TusinSet_helpCur_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_helpCur_Args* helpCurArgs = args;
    TusinSet_helpCur*      helpCur     = task->data;

    helpCur->tusinSet = helpCurArgs->tusinSet;
    TusinSet_helpCur_Load(helpCur, helpCur->sprites, helpCurArgs);
    return 1;
}

static s32 TusinSet_helpCur_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_helpCur* helpCur  = task->data;
    TusinSetObject*   tusinSet = helpCur->tusinSet;
    s32               i;

    for (i = 0; i < 3; i++) {
        if (tusinSet->helpButtonPressed[i] == 1) {
            TusinSet_SetSpriteFrame(&helpCur->sprites[i], i * 2 + 2);
            if (tusinSet->iconTimer != 0) {
                tusinSet->iconTimer--;
            } else {
                tusinSet->helpButtonPressed[i] = 0;
            }
        } else {
            TusinSet_SetSpriteFrame(&helpCur->sprites[i], i * 2 + 1);
        }
    }

    if (tusinSet->helpPage == 0) {
        helpCur->visible[0] = FALSE;
        helpCur->visible[1] = TRUE;
    } else if (tusinSet->helpPage == 6) {
        helpCur->visible[0] = TRUE;
        helpCur->visible[1] = FALSE;
    } else {
        helpCur->visible[0] = TRUE;
        helpCur->visible[1] = TRUE;
    }

    if (Inventory_IsHelpSeen(6) == 1) {
        helpCur->visible[2] = TRUE;
    }

    for (i = 0; i < 3; i++) {
        Sprite_Update(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_helpCur_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        if (helpCur->visible[i]) {
            Sprite_RenderFrame(&helpCur->sprites[i]);
        }
    }
    return 1;
}

static s32 TusinSet_helpCur_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_helpCur_Init,
        .update     = TusinSet_helpCur_Update,
        .render     = TusinSet_helpCur_Render,
        .cleanup    = TusinSet_helpCur_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_helpCur_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_helpCur_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_helpCur, NULL, 0, NULL, &args);
}
