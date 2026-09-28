#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[2];
    /* 0x80 */ BOOL             gaugeVisible;
    /* 0x84 */ BOOL             baseVisible;
    /* 0x88 */ MenuBadgeObject* menuBadge;
    /* 0x8C */ u16              slot;
} MenuBadge_bdgBP; // Size: 0x90

typedef struct {
    /* 0x00 */ s32              dataType;
    /* 0x04 */ MenuBadgeObject* menuBadge;
    /* 0x08 */ u16              slot;
    /* 0x0A */ u16              pinId;
    /* 0x0C */ u16              levelPP;
    /* 0x0E */ u16              totalPP;
    /* 0x10 */ u16              nextLevelPP;
    /* 0x12 */ s16              count;
} MenuBadge_bdgBP_Args;

static SpriteFrameInfo* MenuBadge_bdgBP_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuBadge_bdgBP_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_bdgBP = {"Tsk_MenuBadge_bdgBP", MenuBadge_bdgBP_RunTask, sizeof(MenuBadge_bdgBP)};

static const SpriteAnimation MenuBadge_bdgBP_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = MenuBadge_bdgBP_GetFrameInfo,
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

static SpriteFrameInfo* MenuBadge_bdgBP_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x3AC000);
}

static void MenuBadge_bdgBP_Load(MenuBadge_bdgBP* bdgBP, Sprite* baseSprite, Sprite* gaugeSprite, MenuBadge_bdgBP_Args* args) {
    SpriteAnimation anim = MenuBadge_bdgBP_Anim;

    anim.dataType = args->dataType;
    anim.posX     = MenuBadge_SlotPositions[args->slot].x - 12;
    anim.posY     = MenuBadge_SlotPositions[args->slot].y + 19;

    u16 gaugeFrame = 0;

    if (args->pinId == 0xFFFF) {
        bdgBP->gaugeVisible = FALSE;
        bdgBP->baseVisible  = FALSE;
    } else {
        if (MenuBadge_IsSlotMastered(args->menuBadge, args->slot) == 1) {
            bdgBP->gaugeVisible = TRUE;
            gaugeFrame          = 24;
        } else {
            if (args->totalPP == args->levelPP) {
                bdgBP->gaugeVisible = FALSE;
            } else {
                gaugeFrame          = ((args->totalPP - args->levelPP) * 23) / (args->nextLevelPP - args->levelPP);
                bdgBP->gaugeVisible = TRUE;
            }
        }
        bdgBP->baseVisible = TRUE;
    }

    anim.animIndex = 0x1D;
    _Sprite_Load(baseSprite, &anim);

    anim.animIndex = gaugeFrame + 4;
    _Sprite_Load(gaugeSprite, &anim);

    bdgBP->slot = args->slot;
}

static s32 MenuBadge_bdgBP_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgBP*      bdgBP    = task->data;
    MenuBadge_bdgBP_Args* initArgs = args;

    bdgBP->menuBadge = initArgs->menuBadge;
    MenuBadge_bdgBP_Load(bdgBP, &bdgBP->sprites[0], &bdgBP->sprites[1], initArgs);
    return 1;
}

static s32 MenuBadge_bdgBP_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgBP* bdgBP = task->data;

    Sprite_Update(&bdgBP->sprites[0]);
    Sprite_Update(&bdgBP->sprites[1]);
    return 1;
}

static s32 MenuBadge_bdgBP_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgBP* bdgBP = task->data;
    MenuBadgeObject* owner = bdgBP->menuBadge;

    if (bdgBP->baseVisible != 0 && owner->slotVisible[bdgBP->slot] == 1) {
        Sprite_RenderFrame(&bdgBP->sprites[0]);
    }
    if (bdgBP->gaugeVisible != 0 && owner->slotVisible[bdgBP->slot] == 1) {
        Sprite_RenderFrame(&bdgBP->sprites[1]);
    }
    return 1;
}

static s32 MenuBadge_bdgBP_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgBP* bdgBP = task->data;

    Sprite_Release(&bdgBP->sprites[0]);
    Sprite_Release(&bdgBP->sprites[1]);
    return 1;
}

static s32 MenuBadge_bdgBP_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_bdgBP_Init,
        .update     = MenuBadge_bdgBP_Update,
        .render     = MenuBadge_bdgBP_Render,
        .cleanup    = MenuBadge_bdgBP_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_bdgBP_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuBadgeObject* owner) {
    MenuBadge_bdgBP_Args args;

    args.dataType    = dataType;
    args.slot        = index;
    args.menuBadge   = owner;
    args.pinId       = owner->slots[index]->pinId;
    args.levelPP     = owner->slots[index]->levelPP;
    args.totalPP     = owner->slots[index]->totalPP;
    args.nextLevelPP = owner->slots[index]->nextLevelPP;
    args.count       = owner->slots[index]->count;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdgBP, NULL, 0, NULL, &args);
}
