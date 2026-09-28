#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite         sprites[2];
    /* 0x80 */ u32            unk_80;
    /* 0x84 */ MenuTopObject* topMenu;
} MenuTop_icon; // Size: 0x88

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
} MenuTop_icon_Args;

static SpriteFrameInfo* MenuTop_icon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const u16 MenuTop_icon_Frames[] = {3, 5};

static const TaskHandle Tsk_MenuTop_icon = {"Tsk_MenuTop_icon", MenuTop_icon_RunTask, sizeof(MenuTop_icon)};

static const SpriteAnimation MenuTop_icon_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .posX              = 0xB6,
    .posY              = 0x10,
    .frameInfoCallback = MenuTop_icon_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[12],
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

static SpriteFrameInfo* MenuTop_icon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Nonmatching
static void MenuTop_icon_Load(MenuTop_icon* icon, MenuTop_icon_Args* args) {
    SpriteAnimation anim = MenuTop_icon_Anim;

    anim.dataType = args->dataType;

    anim.animIndex = 3;
    anim.posX      = 0xDA;
    anim.posY      = 0xC;
    anim.unk_22    = 1;
    _Sprite_Load(&icon->sprites[0], &anim);

    anim.animIndex = 5;
    anim.posX      = 0xF3;
    anim.posY      = 0xC;
    anim.unk_22    = 1;
    _Sprite_Load(&icon->sprites[1], &anim);
}

static s32 MenuTop_icon_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_icon*      icon     = task->data;
    MenuTop_icon_Args* iconArgs = args;

    icon->topMenu = iconArgs->topMenu;
    MenuTop_icon_Load(icon, iconArgs);
    return 1;
}

// Nonmatching
static s32 MenuTop_icon_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        if (icon->topMenu->iconPressed[i] == 1) {
            MenuTop_SetSpriteFrame(&icon->sprites[i], (s16)(MenuTop_icon_Frames[i] + 1));

            if (icon->topMenu->pressTimer != 0) {
                icon->topMenu->pressTimer--;
            } else {
                icon->topMenu->iconPressed[i] = 0;
            }
        } else {
            MenuTop_SetSpriteFrame(&icon->sprites[i], (s16)MenuTop_icon_Frames[i]);
        }
    }

    for (s32 i = 0; i < 2; i++) {
        Sprite_Update(&icon->sprites[i]);
    }

    return 1;
}

static s32 MenuTop_icon_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_RenderFrame(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_icon_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_icon_Init,
        .update     = MenuTop_icon_Update,
        .render     = MenuTop_icon_Render,
        .cleanup    = MenuTop_icon_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_icon_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_icon_Args args;

    args.dataType = dataType;
    args.topMenu  = topMenu;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_icon, NULL, 0, NULL, &args);
}
