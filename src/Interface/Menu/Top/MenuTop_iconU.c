#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ u32            visible;
    /* 0x44 */ MenuTopObject* topMenu;
} MenuTop_iconU; // Size: 0x48

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
    /* 0x8 */ u16            area;
    /* 0xA */ u16            iconFrame;
} MenuTop_iconU_Args;

// Plain FALSE/TRUE schedule the conditional moves in the opposite order; the enum constants match.
enum {
    ICONU_HIDDEN = 0,
    ICONU_SHOWN  = 1,
};

static SpriteFrameInfo* MenuTop_iconU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_iconU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation MenuTop_iconU_Anim = {
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
    .frameInfoCallback = MenuTop_iconU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[7],
    .unk_18            = 0,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static const TaskHandle Tsk_MenuTop_iconU = {"Tsk_MenuTop_iconU", MenuTop_iconU_RunTask, sizeof(MenuTop_iconU)};

static SpriteFrameInfo* MenuTop_iconU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_iconU_Load(MenuTop_iconU* icon, Sprite* sprite, MenuTop_iconU_Args* args) {
    SpriteAnimation anim = MenuTop_iconU_Anim;

    anim.dataType = args->dataType;

    if (args->area < 21) {
        anim.animIndex = args->iconFrame;
        anim.posX      = MenuTop_AreaMapPos[args->area].x;
        anim.posY      = MenuTop_AreaMapPos[args->area].y - 2;
        icon->visible  = TRUE;
    } else if (args->area <= 34) {
        anim.animIndex = args->iconFrame;
        anim.posX      = MenuTop_AreaMapPos[21].x;
        anim.posY      = MenuTop_AreaMapPos[21].y - 2;
        icon->visible  = TRUE;
    } else {
        anim.animIndex = args->iconFrame;
        anim.posX      = MenuTop_AreaMapPos[22].x;
        anim.posY      = MenuTop_AreaMapPos[22].y - 2;
        icon->visible  = TRUE;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_iconU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_iconU*      icon     = task->data;
    MenuTop_iconU_Args* iconArgs = args;

    icon->topMenu = iconArgs->topMenu;
    MenuTop_iconU_Load(icon, &icon->sprite, iconArgs);
    return 1;
}

static s32 MenuTop_iconU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_iconU* icon = task->data;

    icon->visible = (icon->topMenu->blinkOn == 1) ? ICONU_HIDDEN : ICONU_SHOWN;
    Sprite_Update(&icon->sprite);
    return 1;
}

static s32 MenuTop_iconU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_iconU* icon = task->data;

    if (icon->visible) {
        Sprite_RenderFrame(&icon->sprite);
    }
    return 1;
}

static s32 MenuTop_iconU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_iconU* icon = task->data;

    Sprite_Release(&icon->sprite);
    return 1;
}

static s32 MenuTop_iconU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = MenuTop_iconU_Init,
        .update     = MenuTop_iconU_Update,
        .render     = MenuTop_iconU_Render,
        .cleanup    = MenuTop_iconU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_iconU_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu, u16 area, u16 iconFrame) {
    MenuTop_iconU_Args args;
    args.dataType  = dataType;
    args.topMenu   = topMenu;
    args.area      = area;
    args.iconFrame = iconFrame;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_iconU, NULL, 0, NULL, &args);
}
