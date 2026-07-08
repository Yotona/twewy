#include "Interface/Menu/MenuBadge.h"
#include "common_data.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
} MenuBadge_shadow; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_shadow_Args;

static SpriteFrameInfo* MenuBadge_shadow_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_shadow_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_shadow = {"Tsk_MenuBadge_shadow", MenuBadge_shadow_RunTask, sizeof(MenuBadge_shadow)};

static const SpriteAnimation MenuBadge_shadow_Anim = {
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
    .unk_08     = MenuBadge_shadow_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82e4,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 4,
    .unk_1E     = 0,
    .unk_20     = 0xD,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 5,
    .unk_28     = 6,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_shadow_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

            info->unk_10 = 0x3CA000;
            return info;
        } break;
    }

    return NULL;
}

static void MenuBadge_shadow_Load(Sprite* sprite, MenuBadge_shadow_Args* args) {
    SpriteAnimation anim = MenuBadge_shadow_Anim;

    anim.dataType      = args->dataType;
    anim.unk_2A        = 23;
    anim.bits_7_9      = 6;
    anim.unk_02.unk_02 = 1;

    data_0206a890.unk_0C = 10;

    _Sprite_Load(sprite, &anim);
}

static s32 MenuBadge_shadow_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_shadow*      shadow     = task->data;
    MenuBadge_shadow_Args* shadowArgs = args;

    MenuBadge_shadow_Load(&shadow->sprite, shadowArgs);
    shadow->visible   = TRUE;
    shadow->menuBadge = shadowArgs->menuBadge;
    return 1;
}

static s32 MenuBadge_shadow_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_shadow* shadow    = task->data;
    MenuBadgeObject*  menuBadge = shadow->menuBadge;

    shadow->sprite.posX = menuBadge->unk_AEF2 + 4;
    shadow->sprite.posY = menuBadge->unk_AEF4 + 6;
    Sprite_Update(&shadow->sprite);
    return 1;
}

static s32 MenuBadge_shadow_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_shadow* shadow = task->data;

    if (shadow->visible) {
        Sprite_RenderFrame(&shadow->sprite);
    }
    return 1;
}

static s32 MenuBadge_shadow_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_shadow* shadow = task->data;

    Sprite_Release(&shadow->sprite);
    return 1;
}

static s32 MenuBadge_shadow_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_shadow_Init,
        .update     = MenuBadge_shadow_Update,
        .render     = MenuBadge_shadow_Render,
        .cleanup    = MenuBadge_shadow_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_shadow_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge) {
    MenuBadge_shadow_Args args;

    args.dataType  = dataType;
    args.menuBadge = menuBadge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_shadow, NULL, 0, NULL, &args);
}
