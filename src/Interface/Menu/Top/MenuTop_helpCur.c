#include "Interface/Menu/Top.h"

extern s32 func_02023d1c(s32 arg0);

typedef struct {
    /* 0x00 */ Sprite         sprites[3];
    /* 0xC0 */ BOOL           visible[3];
    /* 0xCC */ MenuTopObject* topMenu;
} MenuTop_helpCur; // Size: 0xD0

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
} MenuTop_helpCur_Args;

static SpriteFrameInfo* MenuTop_helpCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_helpCur = {"Tsk_MenuTop_helpCur", MenuTop_helpCur_RunTask, sizeof(MenuTop_helpCur)};

static const SpriteAnimation MenuTop_helpCur_Anim = {
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
    .frameInfoCallback = MenuTop_helpCur_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[4],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 4,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuTop_helpCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_helpCur_Load(MenuTop_helpCur* helpCur, Sprite* sprites, MenuTop_helpCur_Args* args) {
    SpriteAnimation anim = MenuTop_helpCur_Anim;

    anim.dataType = args->dataType;

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

    for (u16 i = 0; i < 3; i++) {
        helpCur->visible[i] = TRUE;
    }

    if (func_02023d1c(0) == 0) {
        helpCur->visible[2] = FALSE;
    }
}

static s32 MenuTop_helpCur_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCur*      helpCur     = task->data;
    MenuTop_helpCur_Args* helpCurArgs = args;

    helpCur->topMenu = helpCurArgs->topMenu;
    MenuTop_helpCur_Load(helpCur, helpCur->sprites, helpCurArgs);
    return 1;
}

static s32 MenuTop_helpCur_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCur* helpCur = task->data;
    MenuTopObject*   topMenu = helpCur->topMenu;
    s32              i;

    for (i = 0; i < 3; i++) {
        if (topMenu->helpPressed[i] == 1) {
            MenuTop_SetSpriteFrame(&helpCur->sprites[i], i * 2 + 2);
            if (topMenu->pressTimer != 0) {
                topMenu->pressTimer--;
            } else {
                topMenu->helpPressed[i] = 0;
            }
        } else {
            MenuTop_SetSpriteFrame(&helpCur->sprites[i], i * 2 + 1);
        }
    }

    if (topMenu->helpPage == 0) {
        helpCur->visible[0] = FALSE;
        helpCur->visible[1] = TRUE;
    } else if (topMenu->helpPage == 6) {
        helpCur->visible[0] = TRUE;
        helpCur->visible[1] = FALSE;
    } else {
        helpCur->visible[0] = TRUE;
        helpCur->visible[1] = TRUE;
    }

    if (func_02023d1c(0) == 1) {
        helpCur->visible[2] = TRUE;
    }

    for (i = 0; i < 3; i++) {
        Sprite_Update(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_helpCur_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        if (helpCur->visible[i]) {
            Sprite_RenderFrame(&helpCur->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuTop_helpCur_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_helpCur_Init,
        .update     = MenuTop_helpCur_Update,
        .render     = MenuTop_helpCur_Render,
        .cleanup    = MenuTop_helpCur_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_helpCur_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_helpCur_Args args;
    args.dataType = dataType;
    args.topMenu  = topMenu;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_helpCur, NULL, 0, NULL, &args);
}
