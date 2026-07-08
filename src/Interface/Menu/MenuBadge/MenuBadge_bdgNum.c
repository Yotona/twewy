#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[4];
    /* 0x100 */ BOOL             visible[3];
    /* 0x10C */ MenuBadgeObject* unk_10C;
    /* 0x110 */ u16              unk_110;
} MenuBadge_bdgNum; // Size: 0x114

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* unk_4;
    /* 0x8 */ u16              unk_8;
    /* 0xA */ u16              unk_A;
    /* 0xC */ u16              unk_C;
} MenuBadge_bdgNum_Args;

static SpriteFrameInfo* MenuBadge_bdgNum_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_bdgNum_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const MenuBadgePoint data_ov043_020c8900 = {1, 1};

static const TaskHandle Tsk_MenuBadge_bdgNum = {"Tsk_MenuBadge_bdgNum", MenuBadge_bdgNum_RunTask, sizeof(MenuBadge_bdgNum)};

static const SpriteAnimation MenuBadge_bdgNum_Anim = {
    .bits_0_1   = 2,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_bdgNum_GetFrameInfo,
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

static SpriteFrameInfo* MenuBadge_bdgNum_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

            info->unk_10 = 0x3A2000;
            return info;
        } break;
    }

    return NULL;
}

// Nonmatching
static void MenuBadge_bdgNum_Load(MenuBadge_bdgNum* bdgNum, Sprite* sprites, MenuBadge_bdgNum_Args* args) {
    SpriteAnimation anim = MenuBadge_bdgNum_Anim;

    s32 xOffset = 0;
    u16 ones    = data_ov043_020c8900.x;
    u16 tens    = data_ov043_020c8900.y;

    anim.dataType = args->dataType;

    if (args->unk_A == 0xFFFF) {
        bdgNum->visible[0] = 0;
        bdgNum->visible[1] = 0;
        bdgNum->visible[2] = 0;
    } else {
        if (args->unk_C < 10) {
            ones = 0;
            tens = args->unk_C;

            bdgNum->visible[0] = 1;
            bdgNum->visible[1] = 1;
            bdgNum->visible[2] = 0;
        } else {
            ones = args->unk_C - (tens * 10);
            tens = args->unk_C / 10;

            bdgNum->visible[0] = 1;
            bdgNum->visible[1] = 1;
            bdgNum->visible[2] = 1;
            xOffset            = -2;
        }
    }

    anim.bits_0_1 = 2;
    anim.unk_26   = 5;
    anim.bits_7_9 = 6;
    anim.binIden  = &data_ov043_020c82e4;
    anim.unk_22   = 2;
    anim.unk_1C   = 4;
    anim.unk_20   = 0xD;
    anim.unk_28   = 6;

    anim.unk_2A = 24;
    anim.unk_04 = data_ov043_020c834c[args->unk_8].x + xOffset;
    anim.unk_06 = data_ov043_020c834c[args->unk_8].y + 19;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = tens + 25;
    anim.unk_04 = data_ov043_020c834c[args->unk_8].x + xOffset;
    anim.unk_06 = data_ov043_020c834c[args->unk_8].y + 19;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = ones + 25;
    anim.unk_04 = data_ov043_020c834c[args->unk_8].x + 4 + xOffset;
    anim.unk_06 = data_ov043_020c834c[args->unk_8].y + 19;
    _Sprite_Load(&sprites[2], &anim);

    anim.binIden  = &data_ov043_020c82e4;
    anim.bits_0_1 = 0;
    anim.bits_7_9 = 5;
    anim.unk_1C   = 1;
    anim.unk_26   = 2;
    anim.unk_20   = 13;
    anim.unk_22   = 2;
    anim.unk_28   = 3;

    anim.unk_2A = 3;
    anim.unk_04 = data_ov043_020c834c[args->unk_8].x;
    anim.unk_06 = data_ov043_020c834c[args->unk_8].y + 0x13;

    _Sprite_Load(&bdgNum->sprites[3], &anim);
}

static s32 MenuBadge_bdgNum_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgNum*      bdgNum   = task->data;
    MenuBadge_bdgNum_Args* initArgs = args;

    bdgNum->unk_10C = initArgs->unk_4;
    MenuBadge_bdgNum_Load(bdgNum, bdgNum->sprites, initArgs);
    bdgNum->unk_110 = initArgs->unk_8;
    return 1;
}

static s32 MenuBadge_bdgNum_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgNum* bdgNum = task->data;
    s32               i;

    Sprite_Update(&bdgNum->sprites[3]);
    for (i = 0; i < 3; i++) {
        Sprite_Update(&bdgNum->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_bdgNum_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgNum* bdgNum = task->data;
    s32               i;

    if (bdgNum->unk_10C->unk_AE70[bdgNum->unk_110] == 1) {
        if (bdgNum->visible[0] != 0) {
            Sprite_RenderFrame(&bdgNum->sprites[3]);
        }
        for (i = 0; i < 3; i++) {
            if (bdgNum->visible[i] != 0) {
                Sprite_RenderFrame(&bdgNum->sprites[i]);
            }
        }
    }
    return 1;
}

static s32 MenuBadge_bdgNum_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgNum* bdgNum = task->data;
    s32               i;

    Sprite_Release(&bdgNum->sprites[3]);
    for (i = 0; i < 3; i++) {
        Sprite_Release(&bdgNum->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_bdgNum_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_bdgNum_Init,
        .update     = MenuBadge_bdgNum_Update,
        .render     = MenuBadge_bdgNum_Render,
        .cleanup    = MenuBadge_bdgNum_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_bdgNum_CreateTask(TaskPool* pool, s32 dataType, s16 index, MenuBadgeObject* owner) {
    MenuBadge_bdgNum_Args args;

    args.unk_8    = index;
    args.unk_4    = owner;
    args.dataType = dataType;
    args.unk_A    = owner->unk_AD60[index]->unk_00;
    args.unk_C    = owner->unk_AD60[index]->unk_17;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdgNum, NULL, 0, NULL, &args);
}
