#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[4];
    /* 0x100 */ s32              visible;
    /* 0x104 */ MenuBadgeObject* unk_104;
} MenuBadge_tabDeck; // Size: 0x108

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* unk_4;
} MenuBadge_tabDeck_Args;

static SpriteFrameInfo* MenuBadge_tabDeck_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_tabDeck_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_tabDeck = {"Tsk_MenuBadge_tabDeck", MenuBadge_tabDeck_RunTask,
                                                 sizeof(MenuBadge_tabDeck)};

static const SpriteAnimation MenuBadge_tabDeck_Anim = {
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
    .unk_08     = MenuBadge_tabDeck_GetFrameInfo,
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

static SpriteFrameInfo* MenuBadge_tabDeck_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuBadge_tabDeck_Load(MenuBadge_tabDeck* tabDeck, Sprite* sprites, MenuBadge_tabDeck_Args* args) {
    MenuBadgeObject* menuBadge = tabDeck->unk_104;
    SpriteAnimation  anim      = MenuBadge_tabDeck_Anim;
    u16              sel[4]    = {0};

    MenuBadgePoint positions[4] = {
        {0x85, 0x14},
        {0x90, 0x14},
        {0x9B, 0x14},
        {0xA6, 0x14},
    };

    anim.dataType = args->dataType;

    sel[menuBadge->unk_AF26] = 1;

    for (u16 i = 0; i < 4; i++) {
        anim.unk_2A = i * 2 + 0x31 + sel[i];
        anim.unk_04 = positions[i].x;
        anim.unk_06 = positions[i].y;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuBadge_tabDeck_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tabDeck*      tabDeck  = task->data;
    MenuBadge_tabDeck_Args* initArgs = args;

    tabDeck->visible = TRUE;
    tabDeck->unk_104 = initArgs->unk_4;
    MenuBadge_tabDeck_Load(tabDeck, tabDeck->sprites, initArgs);
    return 1;
}

static s32 MenuBadge_tabDeck_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tabDeck* tabDeck   = task->data;
    MenuBadgeObject*   menuBadge = tabDeck->unk_104;

    u16 sel[4] = {0};

    sel[menuBadge->unk_AF26] = 1;

    for (u16 i = 0; i < 4; i++) {
        func_ov043_0208fa60(&tabDeck->sprites[i], i * 2 + 0x31 + sel[i], 3, 2);
    }

    for (u16 i = 0; i < 4; i++) {
        Sprite_Update(&tabDeck->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_tabDeck_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tabDeck* tabDeck = task->data;

    for (u16 i = 0; i < 4; i++) {
        Sprite_RenderFrame(&tabDeck->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_tabDeck_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tabDeck* tabDeck = task->data;

    for (s16 i = 0; i < 4; i++) {
        Sprite_Release(&tabDeck->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_tabDeck_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_tabDeck_Init,
        .update     = MenuBadge_tabDeck_Update,
        .render     = MenuBadge_tabDeck_Render,
        .cleanup    = MenuBadge_tabDeck_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_tabDeck_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* owner) {
    MenuBadge_tabDeck_Args args;

    args.dataType = dataType;
    args.unk_4    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_tabDeck, NULL, 0, NULL, &args);
}
