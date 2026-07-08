#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
} MenuBadge_mstStarU; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_mstStarU_Args;

static SpriteFrameInfo* MenuBadge_mstStarU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_mstStarU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_mstStarU = {"Tsk_MenuBadge_mstStarU", MenuBadge_mstStarU_RunTask,
                                                  sizeof(MenuBadge_mstStarU)};

static const SpriteAnimation MenuBadge_mstStarU_Anim = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_mstStarU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82fc,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_mstStarU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;

            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info = temp;
        } break;
    }

    return info;
}

static void MenuBadge_mstStarU_Load(MenuBadge_mstStarU* mstStar, Sprite* sprite, MenuBadge_mstStarU_Args* args) {
    MenuBadgeObject* menuBadge = mstStar->menuBadge;
    SpriteAnimation  anim      = MenuBadge_mstStarU_Anim;

    anim.dataType = (u16)args->dataType;
    anim.unk_2A   = 0x25;
    anim.unk_04   = 0xDA;
    anim.unk_06   = 0xA;
    _Sprite_Load(sprite, &anim);

    u16 badgeId = menuBadge->unk_ADD8;
    if (badgeId != 0xFFFF) {
        if ((&menuBadge->unk_AF32)[badgeId] > 0) {
            mstStar->visible = 1;
        } else {
            mstStar->visible = 0;
        }
    } else {
        mstStar->visible = 0;
    }
}

static s32 MenuBadge_mstStarU_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_mstStarU*      mstStar     = task->data;
    MenuBadge_mstStarU_Args* mstStarArgs = args;

    mstStar->menuBadge = mstStarArgs->menuBadge;
    MenuBadge_mstStarU_Load(mstStar, &mstStar->sprite, mstStarArgs);
    return 1;
}

static s32 MenuBadge_mstStarU_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_mstStarU* mstStar   = task->data;
    MenuBadgeObject*    menuBadge = mstStar->menuBadge;

    u16 badgeId = menuBadge->unk_ADD8;
    if (badgeId != 0xFFFF) {
        if ((&menuBadge->unk_AF32)[badgeId] > 0) {
            mstStar->visible = 1;
        } else {
            mstStar->visible = 0;
        }
    } else {
        mstStar->visible = 0;
    }

    Sprite_Update(&mstStar->sprite);
    return 1;
}

static s32 MenuBadge_mstStarU_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_mstStarU* mstStar = task->data;

    if (mstStar->visible != 0) {
        Sprite_RenderFrame(&mstStar->sprite);
    }
    return 1;
}

static s32 MenuBadge_mstStarU_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_mstStarU* mstStar = task->data;

    Sprite_Release(&mstStar->sprite);
    return 1;
}

static s32 MenuBadge_mstStarU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_mstStarU_Init,
        .update     = MenuBadge_mstStarU_Update,
        .render     = MenuBadge_mstStarU_Render,
        .cleanup    = MenuBadge_mstStarU_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_mstStarU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* badge) {
    MenuBadge_mstStarU_Args args;

    args.dataType  = dataType;
    args.menuBadge = badge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_mstStarU, NULL, 0, NULL, &args);
}
