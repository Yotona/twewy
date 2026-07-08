#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* unk_44;
} MenuBadge_bdgCur; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* unk_4;
} MenuBadge_bdgCur_Args;

static SpriteFrameInfo* MenuBadge_bdgCur_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_bdgCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_bdgCur = {"Tsk_MenuBadge_bdgCur", MenuBadge_bdgCur_RunTask, sizeof(MenuBadge_bdgCur)};

static const SpriteAnimation MenuBadge_bdgCur_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x800,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_bdgCur_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82e4,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 0xD,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_bdgCur_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1: {
            info->unk_00 = 1;
            return info;
        } break;

        case 2: {
            info = &data_0206b408;

            info->unk_04 = 0;
            info->unk_08 = 0;
            info->unk_0C = 0;
            info->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                info->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                info->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info->unk_10 = 0x3C0000;
            return info;
        } break;
    }

    return NULL;
}

static void MenuBadge_bdgCur_Load(MenuBadge_bdgCur* bdgCur, Sprite* sprite, MenuBadge_bdgCur_Args* args) {
    SpriteAnimation anim = MenuBadge_bdgCur_Anim;

    bdgCur->visible = TRUE;

    anim.dataType = args->dataType;
    anim.unk_2A   = 46;
    anim.unk_04   = data_ov043_020c834c[0].x;
    anim.unk_06   = data_ov043_020c834c[0].y;

    _Sprite_Load(sprite, &anim);
}

static s32 MenuBadge_bdgCur_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgCur*      bdgCur   = task->data;
    MenuBadge_bdgCur_Args* initArgs = args;

    bdgCur->unk_44 = initArgs->unk_4;
    MenuBadge_bdgCur_Load(bdgCur, &bdgCur->sprite, initArgs);
    return 1;
}

// Nonmatching
static s32 MenuBadge_bdgCur_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgCur* bdgCur    = task->data;
    MenuBadgeObject*  menuBadge = bdgCur->unk_44;

    if (menuBadge->unk_AEE8 & 1) {
        bdgCur->visible = 0;
    } else {
        u16 cursorPos = menuBadge->unk_AEF8;

        if (cursorPos == 0xFFFF) {
            bdgCur->sprite.posX = data_ov043_020c834c[menuBadge->unk_AEF6].x;
            bdgCur->sprite.posY = data_ov043_020c834c[menuBadge->unk_AEF6].y;
            bdgCur->visible     = TRUE;
        } else if (cursorPos >= menuBadge->unk_AEFA && cursorPos < menuBadge->unk_AEFA + 24) {
            s32 slot = (u16)(cursorPos - menuBadge->unk_AEFA) + 6;

            bdgCur->sprite.posX = data_ov043_020c834c[slot].x;
            bdgCur->sprite.posY = data_ov043_020c834c[slot].y;
            bdgCur->visible     = TRUE;
        } else {
            bdgCur->visible = FALSE;
        }
    }

    if (menuBadge->unk_AF20 != 0) {
        bdgCur->visible = FALSE;
    }

    Sprite_Update(&bdgCur->sprite);
    return 1;
}

static s32 MenuBadge_bdgCur_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgCur* bdgCur = task->data;

    if (bdgCur->visible) {
        Sprite_RenderFrame(&bdgCur->sprite);
    }
    return 1;
}

static s32 MenuBadge_bdgCur_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgCur* bdgCur = task->data;

    Sprite_Release(&bdgCur->sprite);
    return 1;
}

static s32 MenuBadge_bdgCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_bdgCur_Init,
        .update     = MenuBadge_bdgCur_Update,
        .render     = MenuBadge_bdgCur_Render,
        .cleanup    = MenuBadge_bdgCur_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_bdgCur_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* owner) {
    MenuBadge_bdgCur_Args args;

    args.dataType = dataType;
    args.unk_4    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdgCur, NULL, 0, NULL, &args);
}
