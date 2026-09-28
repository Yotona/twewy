#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
} MenuBadge_bdgPRI; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
    /* 0x8 */ u16              slot;
    /* 0xA */ u16              pinId;
    /* 0xC */ s16              deckSlot;
    /* 0xE */ u8               slotCount;
} MenuBadge_bdgPRI_Args;

static SpriteFrameInfo* MenuBadge_bdgPRI_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuBadge_bdgPRI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_bdgPRI = {"Tsk_MenuBadge_bdgPRI", MenuBadge_bdgPRI_RunTask, sizeof(MenuBadge_bdgPRI)};

static const SpriteAnimation MenuBadge_bdgPRI_Anim = {
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
    .frameInfoCallback = MenuBadge_bdgPRI_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuBadge_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 4,
    .unk_1E            = 0,
    .unk_20            = 0xD,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 5,
    .unk_28            = 6,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuBadge_bdgPRI_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x398000);
}

static void MenuBadge_bdgPRI_Load(MenuBadge_bdgPRI* bdgPRI, Sprite* sprite, MenuBadge_bdgPRI_Args* args) {
    SpriteAnimation anim = MenuBadge_bdgPRI_Anim;

    anim.dataType = args->dataType;
    anim.posX     = MenuBadge_SlotPositions[args->slot].x - 9;
    anim.posY     = MenuBadge_SlotPositions[args->slot].y - 9;
    anim.bits_7_9 = 6;

    if (args->slot < args->slotCount) {
        anim.animIndex  = args->slot + 1;
        bdgPRI->visible = TRUE;
    } else {
        anim.animIndex  = 1;
        bdgPRI->visible = FALSE;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 MenuBadge_bdgPRI_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgPRI*      bdgPRI     = task->data;
    MenuBadge_bdgPRI_Args* bdgPRIArgs = args;

    MenuBadge_bdgPRI_Load(bdgPRI, &bdgPRI->sprite, bdgPRIArgs);
    bdgPRI->menuBadge = bdgPRIArgs->menuBadge;
    return 1;
}

static s32 MenuBadge_bdgPRI_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgPRI* bdgPRI = task->data;

    Sprite_Update(&bdgPRI->sprite);
    return 1;
}

static s32 MenuBadge_bdgPRI_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgPRI* bdgPRI = task->data;

    if (bdgPRI->visible) {
        Sprite_RenderFrame(&bdgPRI->sprite);
    }
    return 1;
}

static s32 MenuBadge_bdgPRI_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgPRI* bdgPRI = task->data;

    Sprite_Release(&bdgPRI->sprite);
    return 1;
}

static s32 MenuBadge_bdgPRI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_bdgPRI_Init,
        .update     = MenuBadge_bdgPRI_Update,
        .render     = MenuBadge_bdgPRI_Render,
        .cleanup    = MenuBadge_bdgPRI_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_bdgPRI_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuBadgeObject* owner) {
    MenuBadge_bdgPRI_Args args;

    args.dataType  = dataType;
    args.menuBadge = owner;
    args.slot      = index;
    args.pinId     = owner->slots[index]->pinId;
    args.deckSlot  = owner->slots[index]->deckSlot;
    args.slotCount = owner->deckSlotCount;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdgPRI, NULL, 0, NULL, &args);
}
