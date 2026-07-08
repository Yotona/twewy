#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[13];
    /* 0x340 */ s32              visible;
    /* 0x344 */ MenuBadgeObject* menuBadge;
} MenuBadge_nameU; // Size: 0x348

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_nameU_Args;

extern void func_ov043_0208fa14(Sprite* sprite, s16 frame);

static SpriteFrameInfo* MenuBadge_nameU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_nameU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_nameU = {"Tsk_MenuBadge_nameU", MenuBadge_nameU_RunTask, sizeof(MenuBadge_nameU)};

static const SpriteAnimation MenuBadge_nameU_Anim = {
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
    .unk_08     = MenuBadge_nameU_GetFrameInfo,
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

static const MenuBadgePoint data_ov043_020c8e4c[13] = {
    {  6,  11},
    {  6,  35},
    {  6, 145},
    { 10,  62},
    {104,  62},
    { 10,  74},
    {124,  74},
    { 10,  92},
    {152,  92},
    { 10, 104},
    {152, 104},
    { 10, 116},
    { 10, 128},
};

static SpriteFrameInfo* MenuBadge_nameU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuBadge_nameU_Load(Sprite* sprites, MenuBadge_nameU_Args* args) {
    SpriteAnimation anim = MenuBadge_nameU_Anim;
    MenuBadgePoint  points[13];

    u32                   i;
    MenuBadgePoint*       dst = points;
    const MenuBadgePoint* src = data_ov043_020c8e4c;

    for (i = 13; i != 0; i--) {
        *dst = *src;
        src++;
        dst++;
    }

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 3; i++) {
        anim.unk_2A = i + 1;
        anim.unk_04 = points[i].x;
        anim.unk_06 = points[i].y;
        _Sprite_Load(&sprites[i], &anim);
    }

    Sprite* sprite = &sprites[3];
    for (s16 i = 3; i < 13; i++) {
        anim.unk_2A = i + 3;
        anim.unk_04 = points[i].x;
        anim.unk_06 = points[i].y;
        _Sprite_Load(sprite, &anim);
        sprite++;
    }
}

static s32 MenuBadge_nameU_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_nameU*      name     = task->data;
    MenuBadge_nameU_Args* nameArgs = args;

    name->visible   = TRUE;
    name->menuBadge = nameArgs->menuBadge;
    MenuBadge_nameU_Load(name->sprites, nameArgs);
    return 1;
}

static s32 MenuBadge_nameU_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_nameU* name      = task->data;
    MenuBadgeObject* menuBadge = name->menuBadge;

    if (menuBadge->unk_AEE8 & 0x40) {
        menuBadge->unk_AEE8 &= ~0x40;
        func_ov043_0208fa14(&name->sprites[2], menuBadge->unk_AEFE + 3);
    }

    for (s16 i = 0; i < 13; i++) {
        Sprite_Update(&name->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_nameU_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_nameU* name = task->data;

    for (s16 i = 0; i < 13; i++) {
        Sprite_RenderFrame(&name->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_nameU_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_nameU* name = task->data;

    for (s16 i = 0; i < 13; i++) {
        Sprite_Release(&name->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_nameU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_nameU_Init,
        .update     = MenuBadge_nameU_Update,
        .render     = MenuBadge_nameU_Render,
        .cleanup    = MenuBadge_nameU_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_nameU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* badge) {
    MenuBadge_nameU_Args args;

    args.dataType  = dataType;
    args.menuBadge = badge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_nameU, NULL, 0, NULL, &args);
}
