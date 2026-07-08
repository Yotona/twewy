#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ s32              visibleFlags[3];
    /* 0xCC */ MenuBadgeObject* menuBadge;
    /* 0xD0 */ u16              unk_D0;
    /* 0xD2 */ s16              unk_D2;
} MenuBadge_bdgLV; // Size: 0xD4

typedef struct {
    /* 0x00 */ s32              dataType;
    /* 0x04 */ MenuBadgeObject* menuBadge;
    /* 0x08 */ u16              unk_8;
    /* 0x0A */ u16              unk_A;
    /* 0x0C */ u16              unk_C;
    /* 0x0E */ u16              unk_E;
    /* 0x10 */ s16              unk_10;
} MenuBadge_bdgLV_Args;

static SpriteFrameInfo* MenuBadge_bdgLV_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_bdgLV_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_bdgLV = {"Tsk_MenuBadge_bdgLV", MenuBadge_bdgLV_RunTask, sizeof(MenuBadge_bdgLV)};

static const SpriteAnimation MenuBadge_bdgLV_Anim = {
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
    .unk_08     = MenuBadge_bdgLV_GetFrameInfo,
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

static const struct BadgeLevelDigits {
    s16 ones;
    s16 tens;
} data_ov043_020c87dc = {1, 1};

static SpriteFrameInfo* MenuBadge_bdgLV_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

            info->unk_10 = 0x3B6000;
            return info;
        } break;
    }

    return NULL;
}

// Nonmatching
static void MenuBadge_bdgLV_Load(MenuBadge_bdgLV* bdgLV, Sprite* sprites, MenuBadge_bdgLV_Args* args) {
    SpriteAnimation anim = MenuBadge_bdgLV_Anim;

    s16 ones = data_ov043_020c87dc.ones;
    s16 tens = data_ov043_020c87dc.tens;

    anim.dataType = args->dataType;
    anim.bits_7_9 = 6;

    bdgLV->unk_D2 = -5;
    if (args->unk_A == 0xFFFF) {
        bdgLV->visibleFlags[0] = 0;
        bdgLV->visibleFlags[1] = 0;
        bdgLV->visibleFlags[2] = 0;
    } else {
        if (args->unk_C >= args->unk_E) {
            bdgLV->visibleFlags[0] = 1;
            bdgLV->visibleFlags[1] = 1;
            bdgLV->visibleFlags[2] = 0;
            tens                   = 0xA;
            ones                   = 0;
            bdgLV->unk_D2          = -5;
        } else if (args->unk_C < 10) {
            bdgLV->visibleFlags[0] = 1;
            bdgLV->visibleFlags[1] = 1;
            bdgLV->visibleFlags[2] = 0;
            tens                   = args->unk_C;
            ones                   = 0;
            bdgLV->unk_D2          = -5;
        } else {
            bdgLV->visibleFlags[0] = 1;
            bdgLV->visibleFlags[1] = 1;
            bdgLV->visibleFlags[2] = 1;
            tens                   = args->unk_C / 10;
            ones                   = args->unk_C - (tens * 10);
            bdgLV->unk_D2          = -6;
        }

        if (args->unk_8 >= 6 && args->unk_10 == 0) {
            bdgLV->visibleFlags[0] = 0;
            bdgLV->visibleFlags[1] = 0;
            bdgLV->visibleFlags[2] = 0;
        }
    }

    anim.unk_2A = 11;
    anim.unk_04 = data_ov043_020c834c[args->unk_8].x - 7;
    anim.unk_06 = data_ov043_020c834c[args->unk_8].y + 13;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = tens + 12;
    anim.unk_04 = data_ov043_020c834c[args->unk_8].x + bdgLV->unk_D2;
    anim.unk_06 = data_ov043_020c834c[args->unk_8].y + 13;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = ones + 12;
    anim.unk_04 = data_ov043_020c834c[args->unk_8].x - 2;
    anim.unk_06 = data_ov043_020c834c[args->unk_8].y + 13;
    _Sprite_Load(&sprites[2], &anim);

    bdgLV->unk_D0 = args->unk_8;
}

static s32 MenuBadge_bdgLV_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgLV*      bdgLV     = task->data;
    MenuBadge_bdgLV_Args* bdgLVArgs = args;

    bdgLV->menuBadge = bdgLVArgs->menuBadge;
    MenuBadge_bdgLV_Load(bdgLV, bdgLV->sprites, bdgLVArgs);
    return 1;
}

static s32 MenuBadge_bdgLV_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgLV* bdgLV = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&bdgLV->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_bdgLV_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgLV* bdgLV     = task->data;
    MenuBadgeObject* menuBadge = bdgLV->menuBadge;

    for (s32 i = 0; i < 3; i++) {
        if (bdgLV->visibleFlags[i] != 0 && menuBadge->unk_AE70[bdgLV->unk_D0] == 1) {
            Sprite_RenderFrame(&bdgLV->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuBadge_bdgLV_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgLV* bdgLV = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&bdgLV->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_bdgLV_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_bdgLV_Init,
        .update     = MenuBadge_bdgLV_Update,
        .render     = MenuBadge_bdgLV_Render,
        .cleanup    = MenuBadge_bdgLV_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_bdgLV_CreateTask(TaskPool* pool, s32 dataType, s16 index, MenuBadgeObject* menuBadge) {
    MenuBadge_bdgLV_Args args;

    args.unk_8     = index;
    args.menuBadge = menuBadge;
    args.dataType  = dataType;
    args.unk_A     = menuBadge->unk_AD60[index]->unk_00;
    args.unk_C     = menuBadge->unk_AD60[index]->unk_14;
    args.unk_E     = menuBadge->unk_AD60[index]->unk_41;
    args.unk_10    = menuBadge->unk_AD60[index]->unk_17;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdgLV, NULL, 0, NULL, &args);
}
