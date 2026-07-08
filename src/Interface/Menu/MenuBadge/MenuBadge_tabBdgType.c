#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ s32              visible;
    /* 0xC4 */ MenuBadgeObject* menuBadge;
} MenuBadge_tabBdgType; // Size: 0xC8

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_tabBdgType_Args;

static SpriteFrameInfo* MenuBadge_tabBdgType_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_tabBdgType_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_tabBdgType = {"Tsk_MenuBadge_tabBdgType", MenuBadge_tabBdgType_RunTask,
                                                    sizeof(MenuBadge_tabBdgType)};

static const SpriteAnimation MenuBadge_tabBdgType_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_tabBdgType_GetFrameInfo,
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

static SpriteFrameInfo* MenuBadge_tabBdgType_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

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

static void MenuBadge_tabBdgType_Load(Sprite* sprites, MenuBadge_tabBdgType_Args* args) {
    SpriteAnimation anim = MenuBadge_tabBdgType_Anim;

    const MenuBadgePoint data_ov043_020c8a6c[2] = {
        {0x89, 0xB8},
        {0xD4, 0xB8},
    };

    anim.dataType = args->dataType;

    anim.unk_2A = 0x3A;
    anim.unk_04 = data_ov043_020c8a6c[0].x;
    anim.unk_06 = data_ov043_020c8a6c[0].y;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 0x3C;
    anim.unk_04 = data_ov043_020c8a6c[1].x;
    anim.unk_06 = data_ov043_020c8a6c[1].y;
    _Sprite_Load(&sprites[1], &anim);
}

static s32 MenuBadge_tabBdgType_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tabBdgType*      tabBdgType = task->data;
    MenuBadge_tabBdgType_Args* initArgs   = args;

    tabBdgType->visible   = TRUE;
    tabBdgType->menuBadge = initArgs->menuBadge;
    MenuBadge_tabBdgType_Load(tabBdgType->sprites, initArgs);
    return 1;
}

static s32 MenuBadge_tabBdgType_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tabBdgType* tabBdgType = task->data;

    for (u16 i = 0; i < 2; i++) {
        Sprite_Update(&tabBdgType->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_tabBdgType_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tabBdgType* tabBdgType = task->data;

    for (u16 i = 0; i < 2; i++) {
        Sprite_RenderFrame(&tabBdgType->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_tabBdgType_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tabBdgType* tabBdgType = task->data;

    for (s16 i = 0; i < 2; i++) {
        Sprite_Release(&tabBdgType->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_tabBdgType_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_tabBdgType_Init,
        .update     = MenuBadge_tabBdgType_Update,
        .render     = MenuBadge_tabBdgType_Render,
        .cleanup    = MenuBadge_tabBdgType_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_tabBdgType_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge) {
    MenuBadge_tabBdgType_Args args;

    args.dataType  = dataType;
    args.menuBadge = menuBadge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_tabBdgType, NULL, 0, NULL, &args);
}
