#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite         sprites[2];
    /* 0x80 */ s32            visible[2];
    /* 0x88 */ MenuTopObject* topMenu;
} MenuTop_helpCurU; // Size: 0x8C

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s32 topMenu;
} MenuTop_helpCurU_Args;

static SpriteFrameInfo* MenuTop_helpCurU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_helpCurU = {"Tsk_MenuTop_helpCurU", MenuTop_helpCurU_RunTask, sizeof(MenuTop_helpCurU)};

static const SpriteAnimation MenuTop_helpCurU_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02            = 0,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuTop_helpCurU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[7],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuTop_helpCurU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_helpCurU_Load(Sprite* sprites, MenuTop_helpCurU_Args* args) {
    SpriteAnimation anim = MenuTop_helpCurU_Anim;

    anim.dataType = args->dataType;

    anim.unk_2A = 27;
    anim.unk_04 = 7;
    anim.unk_06 = 96;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 28;
    anim.unk_04 = 249;
    anim.unk_06 = 96;
    _Sprite_Load(&sprites[1], &anim);
}

static s32 MenuTop_helpCurU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCurU*      helpCurU = task->data;
    MenuTop_helpCurU_Args* helpArgs = args;

    helpCurU->topMenu = (MenuTopObject*)helpArgs->topMenu;
    MenuTop_helpCurU_Load(helpCurU->sprites, helpArgs);
    return 1;
}

static s32 MenuTop_helpCurU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCurU* helpCurU = task->data;

    u8 helpPage = helpCurU->topMenu->helpPage;
    if (helpPage == 0) {
        helpCurU->visible[0] = 0;
        helpCurU->visible[1] = 1;
    } else if (helpPage == 6) {
        helpCurU->visible[0] = 1;
        helpCurU->visible[1] = 0;
    } else {
        helpCurU->visible[0] = 1;
        helpCurU->visible[1] = 1;
    }

    for (u16 i = 0; i < 2; i++) {
        Sprite_Update(&helpCurU->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_helpCurU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCurU* helpCurU = task->data;

    for (u16 i = 0; i < 2; i++) {
        if (helpCurU->visible[i] != 0) {
            Sprite_RenderFrame(&helpCurU->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuTop_helpCurU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCurU* helpCurU = task->data;

    for (u16 i = 0; i < 2; i++) {
        Sprite_Release(&helpCurU->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_helpCurU_Init,
        .update     = MenuTop_helpCurU_Update,
        .render     = MenuTop_helpCurU_Render,
        .cleanup    = MenuTop_helpCurU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_helpCurU_CreateTask(TaskPool* pool, s32 dataType, s32 topMenu) {
    MenuTop_helpCurU_Args args;
    args.dataType = dataType;
    args.topMenu  = topMenu;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_helpCurU, NULL, 0, NULL, &args);
}