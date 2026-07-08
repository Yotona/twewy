#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
} MenuBadge_brdLogoU; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
    /* 0x8 */ u16              badgeId;
    /* 0xA */ u8               brandId;
} MenuBadge_brdLogoU_Args;

static SpriteFrameInfo* MenuBadge_brdLogoU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_brdLogoU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_brdLogoU = {"Tsk_MenuBadge_brdLogoU", MenuBadge_brdLogoU_RunTask,
                                                  sizeof(MenuBadge_brdLogoU)};

static const SpriteAnimation data_ov043_020c8f80 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0xA8,
    .unk_06     = 0x1C,
    .unk_08     = MenuBadge_brdLogoU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c8334,
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

static SpriteFrameInfo* MenuBadge_brdLogoU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuBadge_brdLogoU_Load(MenuBadge_brdLogoU* brdLogo, Sprite* sprite, MenuBadge_brdLogoU_Args* args) {
    SpriteAnimation anim = data_ov043_020c8f80;

    anim.dataType = (u16)args->dataType;

    if (args->badgeId == 0xFFFF) {
        anim.packIndex   = 1;
        brdLogo->visible = 0;
    } else {
        u8 brandId = args->brandId;
        if (brandId <= 0xC) {
            anim.packIndex   = brandId + 1;
            brdLogo->visible = 1;
        } else {
            anim.packIndex   = 1;
            brdLogo->visible = 0;
        }
    }

    _Sprite_Load(sprite, &anim);
}

static s32 MenuBadge_brdLogoU_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_brdLogoU*      brdLogo     = task->data;
    MenuBadge_brdLogoU_Args* brdLogoArgs = args;

    brdLogo->menuBadge = brdLogoArgs->menuBadge;
    MenuBadge_brdLogoU_Load(brdLogo, &brdLogo->sprite, brdLogoArgs);
    return 1;
}

static s32 MenuBadge_brdLogoU_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_brdLogoU* brdLogo = task->data;

    Sprite_Update(&brdLogo->sprite);
    return 1;
}

static s32 MenuBadge_brdLogoU_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_brdLogoU* brdLogo = task->data;

    if (brdLogo->visible != 0) {
        Sprite_RenderFrame(&brdLogo->sprite);
    }
    return 1;
}

static s32 MenuBadge_brdLogoU_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_brdLogoU* brdLogo = task->data;

    Sprite_Release(&brdLogo->sprite);
    return 1;
}

static s32 MenuBadge_brdLogoU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_brdLogoU_Init,
        .update     = MenuBadge_brdLogoU_Update,
        .render     = MenuBadge_brdLogoU_Render,
        .cleanup    = MenuBadge_brdLogoU_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_brdLogoU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* badge) {
    MenuBadge_brdLogoU_Args args;

    args.dataType  = dataType;
    args.menuBadge = badge;
    args.badgeId   = badge->unk_ADD8;
    args.brandId   = badge->unk_ADF7;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_brdLogoU, NULL, 0, NULL, &args);
}
