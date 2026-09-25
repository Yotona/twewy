#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
    /* 0x48 */ u16              unk_48;
} MenuBadge_bdgSC; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
    /* 0x8 */ u16              unk_8;
    /* 0xA */ u16              unk_A;
    /* 0xC */ u16              unk_C;
    /* 0xE */ u8               unk_E;
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
    .binIden           = &data_ov043_020c82e4,
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
    anim.unk_04   = data_ov043_020c834c[args->unk_8].x + 9;
    anim.unk_06   = data_ov043_020c834c[args->unk_8].y - 9;
    anim.bits_7_9 = 6;

    if (args->unk_8 >= args->unk_E || args->unk_A == 0xFFFF) {
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
    bdgSC->unk_48 = args->unk_8;
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

    if (bdgSC->visible && menuBadge->unk_AE70[bdgSC->unk_48] == 1) {
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

s32 MenuBadge_bdgSC_CreateTask(TaskPool* pool, s32 dataType, s16 index, MenuBadgeObject* menuBadge) {
    MenuBadge_bdgSC_Args args;

    args.dataType  = dataType;
    args.menuBadge = menuBadge;
    args.unk_8     = index;
    args.unk_A     = menuBadge->unk_AD60[index]->unk_00;
    args.unk_C     = menuBadge->unk_AD60[index]->unk_16;
    args.unk_E     = menuBadge->unk_AEEA;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdgSC, NULL, 0, NULL, &args);
}
