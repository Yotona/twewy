#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ s32              unk_C0;
    /* 0xC4 */ MenuBadgeObject* badge;
} MenuBadge_icon; // Size: 0xC8

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* badge;
} MenuBadge_icon_Args;

static SpriteFrameInfo* MenuBadge_icon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuBadge_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_icon = {"Tsk_MenuBadge_icon", MenuBadge_icon_RunTask, sizeof(MenuBadge_icon)};

static const SpriteAnimation MenuBadge_icon_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .posX              = 0xB6,
    .posY              = 0x10,
    .frameInfoCallback = MenuBadge_icon_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuBadge_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 0xD,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuBadge_icon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuBadge_icon_Load(Sprite* sprites, MenuBadge_icon_Args* args) {
    SpriteAnimation anim = MenuBadge_icon_Anim;

    anim.dataType = args->dataType;

    anim.animIndex = 0x28;
    anim.posX      = 0xC1;
    anim.posY      = 0xC;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 0x2A;
    anim.posX      = 0xDA;
    anim.posY      = 0xC;
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 0x2C;
    anim.posX      = 0xF3;
    anim.posY      = 0xC;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 MenuBadge_icon_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_icon*      icon     = task->data;
    MenuBadge_icon_Args* initArgs = args;

    MenuBadge_icon_Load(icon->sprites, initArgs);
    icon->badge = initArgs->badge;
    return 1;
}

// Nonmatching
static s32 MenuBadge_icon_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_icon*  icon  = task->data;
    MenuBadgeObject* badge = icon->badge;

    s32 idleFrame   = 40;
    s32 activeFrame = 41;

    for (s32 i = 0; i < 3; i++) {
        if (badge->buttonPressed[i] == 1) {
            MenuBadge_SetSpriteFrameFromPack(&icon->sprites[i], activeFrame, 3, 2);
            if (badge->pressTimer != 0) {
                badge->pressTimer--;
            } else {
                badge->buttonPressed[i] = 0;
            }
        } else {
            MenuBadge_SetSpriteFrameFromPack(&icon->sprites[i], idleFrame, 3, 2);
        }
        activeFrame += 2;
        idleFrame += 2;
    }

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_icon_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_icon* icon = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_RenderFrame(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_icon_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_icon* icon = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_icon_Init,
        .update     = MenuBadge_icon_Update,
        .render     = MenuBadge_icon_Render,
        .cleanup    = MenuBadge_icon_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_icon_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* badge) {
    MenuBadge_icon_Args args;

    args.dataType = dataType;
    args.badge    = badge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_icon, NULL, 0, NULL, &args);
}
