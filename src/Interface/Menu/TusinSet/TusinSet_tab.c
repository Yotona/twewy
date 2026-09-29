#include "Interface/Menu/TusinSet.h"

typedef struct {
    /* 0x000 */ Sprite          sprites[8];
    /* 0x200 */ BOOL            visible;
    /* 0x204 */ TusinSetObject* tusinSet;
} TusinSet_tab; // Size: 0x208

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_tab_Args;

static SpriteFrameInfo* TusinSet_tab_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_tab_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_tab = {"Tsk_TusinSet_tab", TusinSet_tab_RunTask, sizeof(TusinSet_tab)};

static const SpriteAnimation TusinSet_tab_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .posX              = -13,
    .posY              = 0xc,
    .frameInfoCallback = TusinSet_tab_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &TusinSet_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 7,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* TusinSet_tab_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void TusinSet_tab_Load(TusinSet_tab* tab, TusinSet_tab_Args* args) {
    SpriteAnimation anim = TusinSet_tab_Anim;

    anim.dataType = args->dataType;
    for (s16 i = 0; i < 8; i++) {
        anim.animIndex = i * 2 + 1;
        anim.posX      = i * 14 + 138;
        anim.posY      = 89;
        _Sprite_Load(&tab->sprites[i], &anim);
    }
}

static s32 TusinSet_tab_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_tab*      tab      = task->data;
    TusinSet_tab_Args* initArgs = args;

    tab->tusinSet = initArgs->tusinSet;
    tab->visible  = TRUE;
    TusinSet_tab_Load(tab, initArgs);
    return 1;
}

static s32 TusinSet_tab_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_tab*   tab      = task->data;
    TusinSetObject* tusinSet = tab->tusinSet;
    s32             i;

    for (i = 0; i < 8; i++) {
        if (tusinSet->tabSelected[i] == 1) {
            TusinSet_SetSpriteFrame(&tab->sprites[i], i * 2 + 2);
        } else {
            TusinSet_SetSpriteFrame(&tab->sprites[i], i * 2 + 1);
        }
    }
    for (i = 0; i < 8; i++) {
        Sprite_Update(&tab->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_tab_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_tab* tab = task->data;

    for (s32 i = 0; i < 8; i++) {
        Sprite_RenderFrame(&tab->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_tab_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_tab* tab = task->data;

    for (s32 i = 0; i < 8; i++) {
        Sprite_Release(&tab->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_tab_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_tab_Init,
        .update     = TusinSet_tab_Update,
        .render     = TusinSet_tab_Render,
        .cleanup    = TusinSet_tab_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_tab_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_tab_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_tab, NULL, 0, NULL, &args);
}
