#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
    /* 0x48 */ u16              slot;
} MenuBadge_bdgSC; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
    /* 0x8 */ u16              slot;
    /* 0xA */ u16              pinId;
    /* 0xC */ u16              unk_C;
    /* 0xE */ u8               slotCount;
} MenuBadge_bdgSC_Args;

static SpriteFrameInfo* MenuBadge_bdgSC_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuBadge_bdgSC_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_bdgSC = {"Tsk_MenuBadge_bdgSC", MenuBadge_bdgSC_RunTask, sizeof(MenuBadge_bdgSC)};

static const SpriteAnimation MenuBadge_bdgSC_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuBadge_bdgSC_GetFrameInfo,
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
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuBadge_bdgSC_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x38E000);
}

// Nonmatching
static void MenuBadge_bdgSC_Load(MenuBadge_bdgSC* bdgSC, Sprite* sprite, MenuBadge_bdgSC_Args* args) {
    SpriteAnimation anim = MenuBadge_bdgSC_Anim;

    anim.dataType = args->dataType;
    anim.unk_04   = MenuBadge_SlotPositions[args->slot].x + 9;
    anim.unk_06   = MenuBadge_SlotPositions[args->slot].y - 9;
    anim.bits_7_9 = 6;

    if (args->slot >= args->slotCount || args->pinId == 0xFFFF) {
        anim.unk_2A    = 7;
        bdgSC->visible = FALSE;
    } else {
        switch (args->unk_C) {
            case 0:
                anim.unk_2A    = 7;
                bdgSC->visible = FALSE;
                break;
            case 1:
                anim.unk_2A    = 8;
                bdgSC->visible = TRUE;
                break;
            case 2:
                anim.unk_2A    = 9;
                bdgSC->visible = TRUE;
                break;
        }
    }

    _Sprite_Load(sprite, &anim);
    bdgSC->slot = args->slot;
}

static s32 MenuBadge_bdgSC_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgSC*      bdgSC    = task->data;
    MenuBadge_bdgSC_Args* initArgs = args;

    bdgSC->menuBadge = initArgs->menuBadge;
    MenuBadge_bdgSC_Load(bdgSC, &bdgSC->sprite, initArgs);
    return 1;
}

static s32 MenuBadge_bdgSC_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgSC* bdgSC = task->data;

    Sprite_Update(&bdgSC->sprite);
    return 1;
}

static s32 MenuBadge_bdgSC_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgSC* bdgSC     = task->data;
    MenuBadgeObject* menuBadge = bdgSC->menuBadge;

    if (bdgSC->visible && menuBadge->slotVisible[bdgSC->slot] == 1) {
        Sprite_RenderFrame(&bdgSC->sprite);
    }
    return 1;
}

static s32 MenuBadge_bdgSC_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgSC* bdgSC = task->data;

    Sprite_Release(&bdgSC->sprite);
    return 1;
}

static s32 MenuBadge_bdgSC_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_bdgSC_Init,
        .update     = MenuBadge_bdgSC_Update,
        .render     = MenuBadge_bdgSC_Render,
        .cleanup    = MenuBadge_bdgSC_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_bdgSC_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuBadgeObject* menuBadge) {
    MenuBadge_bdgSC_Args args;

    args.dataType  = dataType;
    args.menuBadge = menuBadge;
    args.slot      = index;
    args.pinId     = menuBadge->slots[index]->pinId;
    args.unk_C     = menuBadge->slots[index]->unk_16;
    args.slotCount = menuBadge->deckSlotCount;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdgSC, NULL, 0, NULL, &args);
}
