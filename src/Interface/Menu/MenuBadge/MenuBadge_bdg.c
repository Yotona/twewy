#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
    /* 0x48 */ u16              slot;
} MenuBadge_bdg; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
    /* 0x8 */ u16              slot;
    /* 0xA */ u16              pinId;
    /* 0xC */ u16              iconIndex;
    /* 0xE */ s16              count;
} MenuBadge_bdg_Args;

static SpriteFrameInfo* MenuBadge_bdg_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuBadge_bdg_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_bdg = {"Tsk_MenuBadge_bdg", MenuBadge_bdg_RunTask, sizeof(MenuBadge_bdg)};

static const SpriteAnimation data_ov043_020c8720 = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = MenuBadge_bdg_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuBadge_BinIdentifiers[8],
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuBadge_bdg_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x384000);
}

// Nonmatching: regswaps
static void MenuBadge_bdg_Load(MenuBadge_bdg* bdg, Sprite* sprite, MenuBadge_bdg_Args* args) {
    SpriteAnimation anim = data_ov043_020c8720;

    anim.dataType = args->dataType;

    Point* point = &MenuBadge_SlotPositions[args->slot];

    anim.posX = point->x;
    anim.posY = point->y;

    if (args->slot < 16) {
        anim.bits_7_9 = 5;
    } else {
        anim.bits_7_9 = 6;
    }
    anim.bits_7_9 = MenuBadge_GetFreePaletteSlot();

    if (args->pinId == 0xFFFF) {
        anim.packIndex = 1;
        bdg->visible   = FALSE;
    } else {
        anim.packIndex = args->iconIndex + 1;
        bdg->visible   = TRUE;
    }

    _Sprite_Load(sprite, &anim);
    bdg->slot = args->slot;
}

static s32 MenuBadge_bdg_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdg*      bdg     = task->data;
    MenuBadge_bdg_Args* bdgArgs = args;

    bdg->menuBadge = bdgArgs->menuBadge;
    MenuBadge_bdg_Load(bdg, &bdg->sprite, bdgArgs);
    return 1;
}

static s32 MenuBadge_bdg_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdg* bdg = task->data;

    Sprite_Update(&bdg->sprite);
    return 1;
}

static s32 MenuBadge_bdg_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdg*   bdg       = task->data;
    MenuBadgeObject* menuBadge = bdg->menuBadge;

    if (bdg->visible != 0 && menuBadge->slotVisible[bdg->slot] == 1) {
        Sprite_RenderFrame(&bdg->sprite);
    }
    return 1;
}

static s32 MenuBadge_bdg_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdg* bdg = task->data;

    Sprite_Release(&bdg->sprite);
    return 1;
}

static s32 MenuBadge_bdg_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_bdg_Init,
        .update     = MenuBadge_bdg_Update,
        .render     = MenuBadge_bdg_Render,
        .cleanup    = MenuBadge_bdg_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_bdg_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuBadgeObject* badge) {
    MenuBadge_bdg_Args args;

    args.slot      = index;
    args.menuBadge = badge;
    args.dataType  = dataType;
    args.pinId     = badge->slots[index]->pinId;
    args.iconIndex = badge->slots[index]->iconIndex;
    args.count     = badge->slots[index]->count;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdg, NULL, 0, NULL, &args);
}

void* MenuBadge_bdg_GetTaskData(TaskPool* pool, u32 taskId) {
    return EasyTask_GetTaskData(pool, taskId);
}
