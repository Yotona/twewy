#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
    /* 0x48 */ u16              unk_48;
} MenuBadge_bdg; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
    /* 0x8 */ u16              unk_8;
    /* 0xA */ u16              unk_A;
    /* 0xC */ u16              unk_C;
    /* 0xE */ s16              unk_E;
} MenuBadge_bdg_Args;

static SpriteFrameInfo* MenuBadge_bdg_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_bdg_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_bdg = {"Tsk_MenuBadge_bdg", MenuBadge_bdg_RunTask, sizeof(MenuBadge_bdg)};

static const SpriteAnimation data_ov043_020c8720 = {
    .bits_0_1   = 2,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_bdg_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c8314,
    .unk_18     = 2,
    .packIndex  = 1,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_bdg_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1: {
            info->unk_00 = 1;
            return info;
        } break;

        case 2: {
            info         = &data_0206b408;
            info->unk_04 = 0;
            info->unk_08 = 0;
            info->unk_0C = 0;
            info->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                info->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                info->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info->unk_10 = 0x384000;
            return info;
        } break;
    }

    return NULL;
}

// Nonmatching: regswaps
static void MenuBadge_bdg_Load(MenuBadge_bdg* bdg, Sprite* sprite, MenuBadge_bdg_Args* args) {
    SpriteAnimation anim = data_ov043_020c8720;

    anim.dataType = args->dataType;

    MenuBadgePoint* point = &data_ov043_020c834c[args->unk_8];

    anim.unk_04 = point->x;
    anim.unk_06 = point->y;

    if (args->unk_8 < 16) {
        anim.bits_7_9 = 5;
    } else {
        anim.bits_7_9 = 6;
    }
    anim.bits_7_9 = func_ov043_0208fb8c();

    if (args->unk_A == 0xFFFF) {
        anim.packIndex = 1;
        bdg->visible   = FALSE;
    } else {
        anim.packIndex = args->unk_C + 1;
        bdg->visible   = TRUE;
    }

    _Sprite_Load(sprite, &anim);
    bdg->unk_48 = args->unk_8;
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

    if (bdg->visible != 0 && menuBadge->unk_AE70[bdg->unk_48] == 1) {
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

s32 MenuBadge_bdg_CreateTask(TaskPool* pool, s32 dataType, s16 index, MenuBadgeObject* badge) {
    MenuBadge_bdg_Args args;

    args.unk_8     = index;
    args.menuBadge = badge;
    args.dataType  = dataType;
    args.unk_A     = badge->unk_AD60[index]->unk_00;
    args.unk_C     = badge->unk_AD60[index]->unk_1A;
    args.unk_E     = badge->unk_AD60[index]->unk_17;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdg, NULL, 0, NULL, &args);
}

void* MenuBadge_bdg_GetTaskData(TaskPool* pool, u32 taskId) {
    return EasyTask_GetTaskData(pool, taskId);
}
