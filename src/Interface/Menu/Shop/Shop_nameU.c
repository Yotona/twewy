#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x000 */ Sprite      sprites[6];
    /* 0x180 */ BOOL        unk_180;
    /* 0x184 */ ShopObject* unk_184;
} Shop_nameU; // Size: 0x188

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
} Shop_nameU_Args;

static SpriteFrameInfo* Shop_nameU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              Shop_nameU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const s16 data_ov043_020caad8[6] = {
    6, 7, 8, 9, 1, 3,
};

static const TaskHandle Tsk_Shop_nameU = {"Tsk_Shop_nameU", Shop_nameU_RunTask, sizeof(Shop_nameU)};

static const Point data_ov043_020cab00[6] = {
    { 8, 159},
    { 8, 171},
    { 8, 183},
    {51, 183},
    {80,  19},
    {75, 171},
};

static const SpriteAnimation data_ov043_020cab18 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x800,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = Shop_nameU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca1d8,
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

static SpriteFrameInfo* Shop_nameU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;
            temp->unk_04          = 0;
            temp->unk_08          = 0;
            temp->unk_0C          = 0;
            temp->unk_10          = -1;

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

// Nonmatching
static void Shop_nameU_Load(Sprite* sprites, Shop_nameU_Args* args) {
    SpriteAnimation anim = data_ov043_020cab18;

    anim.dataType = args->dataType;
    for (s16 i = 0; i < 6; i++) {
        anim.unk_2A = data_ov043_020caad8[i];
        anim.unk_04 = data_ov043_020cab00[i].x;
        anim.unk_06 = data_ov043_020cab00[i].y;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 Shop_nameU_Init(TaskPool* pool, Task* task, void* args) {
    Shop_nameU*      name     = task->data;
    Shop_nameU_Args* nameArgs = args;

    name->unk_180 = TRUE;
    name->unk_184 = nameArgs->shop;
    Shop_nameU_Load(name->sprites, nameArgs);
    return 1;
}

static s32 Shop_nameU_Update(TaskPool* pool, Task* task, void* args) {
    Shop_nameU* name = task->data;

    for (s16 i = 0; i < 6; i++) {
        Sprite_Update(&name->sprites[i]);
    }
    return 1;
}

static s32 Shop_nameU_Render(TaskPool* pool, Task* task, void* args) {
    Shop_nameU* name = task->data;

    for (s16 i = 0; i < 6; i++) {
        Sprite_RenderFrame(&name->sprites[i]);
    }
    return 1;
}

static s32 Shop_nameU_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_nameU* name = task->data;

    for (s16 i = 0; i < 6; i++) {
        Sprite_Release(&name->sprites[i]);
    }
    return 1;
}

static s32 Shop_nameU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_nameU_Init,
        .update     = Shop_nameU_Update,
        .render     = Shop_nameU_Render,
        .cleanup    = Shop_nameU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_nameU_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_nameU_Args args;

    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_nameU, NULL, 0, NULL, &args);
}
