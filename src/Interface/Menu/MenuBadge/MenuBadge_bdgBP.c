#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[2];
    /* 0x80 */ s32              gaugeVisible;
    /* 0x84 */ s32              baseVisible;
    /* 0x88 */ MenuBadgeObject* unk_88;
    /* 0x8C */ u16              unk_8C;
} MenuBadge_bdgBP; // Size: 0x90

typedef struct {
    /* 0x00 */ s32              dataType;
    /* 0x04 */ MenuBadgeObject* unk_4;
    /* 0x08 */ u16              unk_8;
    /* 0x0A */ u16              unk_A;
    /* 0x0C */ u16              unk_C;
    /* 0x0E */ u16              unk_E;
    /* 0x10 */ u16              unk_10;
    /* 0x12 */ s16              unk_12;
} MenuBadge_bdgBP_Args;

static SpriteFrameInfo* MenuBadge_bdgBP_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_bdgBP_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_bdgBP = {"Tsk_MenuBadge_bdgBP", MenuBadge_bdgBP_RunTask, sizeof(MenuBadge_bdgBP)};

static const SpriteAnimation MenuBadge_bdgBP_Anim = {
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
    .unk_08     = MenuBadge_bdgBP_GetFrameInfo,
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

static SpriteFrameInfo* MenuBadge_bdgBP_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

            info->unk_10 = 0x3AC000;
            return info;
        } break;
    }

    return NULL;
}

// Nonmatching
static void MenuBadge_bdgBP_Load(MenuBadge_bdgBP* bdgBP, Sprite* baseSprite, Sprite* gaugeSprite, MenuBadge_bdgBP_Args* args) {
    SpriteAnimation anim = MenuBadge_bdgBP_Anim;

    anim.dataType = args->dataType;
    anim.unk_04   = data_ov043_020c834c[args->unk_8].x - 12;
    anim.unk_06   = data_ov043_020c834c[args->unk_8].y + 19;

    u16 gaugeFrame = 0;

    if (args->unk_A == 0xFFFF) {
        bdgBP->gaugeVisible = FALSE;
        bdgBP->baseVisible  = FALSE;
    } else {
        if (func_ov043_0208fac8(args->unk_4, args->unk_8) == 1) {
            bdgBP->gaugeVisible = TRUE;
            gaugeFrame          = 24;
        } else {
            if (args->unk_E == args->unk_C) {
                bdgBP->gaugeVisible = FALSE;
            } else {
                gaugeFrame          = ((args->unk_E - args->unk_C) * 23) / (args->unk_10 - args->unk_C);
                bdgBP->gaugeVisible = TRUE;
            }
        }
        bdgBP->baseVisible = TRUE;
    }

    anim.unk_2A = 0x1D;
    _Sprite_Load(baseSprite, &anim);

    anim.unk_2A = gaugeFrame + 4;
    _Sprite_Load(gaugeSprite, &anim);

    bdgBP->unk_8C = args->unk_8;
}

static s32 MenuBadge_bdgBP_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgBP*      bdgBP    = task->data;
    MenuBadge_bdgBP_Args* initArgs = args;

    bdgBP->unk_88 = initArgs->unk_4;
    MenuBadge_bdgBP_Load(bdgBP, &bdgBP->sprites[0], &bdgBP->sprites[1], initArgs);
    return 1;
}

static s32 MenuBadge_bdgBP_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgBP* bdgBP = task->data;

    Sprite_Update(&bdgBP->sprites[0]);
    Sprite_Update(&bdgBP->sprites[1]);
    return 1;
}

static s32 MenuBadge_bdgBP_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgBP* bdgBP = task->data;
    MenuBadgeObject* owner = bdgBP->unk_88;

    if (bdgBP->baseVisible != 0 && owner->unk_AE70[bdgBP->unk_8C] == 1) {
        Sprite_RenderFrame(&bdgBP->sprites[0]);
    }
    if (bdgBP->gaugeVisible != 0 && owner->unk_AE70[bdgBP->unk_8C] == 1) {
        Sprite_RenderFrame(&bdgBP->sprites[1]);
    }
    return 1;
}

static s32 MenuBadge_bdgBP_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgBP* bdgBP = task->data;

    Sprite_Release(&bdgBP->sprites[0]);
    Sprite_Release(&bdgBP->sprites[1]);
    return 1;
}

static s32 MenuBadge_bdgBP_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_bdgBP_Init,
        .update     = MenuBadge_bdgBP_Update,
        .render     = MenuBadge_bdgBP_Render,
        .cleanup    = MenuBadge_bdgBP_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_bdgBP_CreateTask(TaskPool* pool, s32 dataType, s16 index, MenuBadgeObject* owner) {
    MenuBadge_bdgBP_Args args;

    args.dataType = dataType;
    args.unk_8    = index;
    args.unk_4    = owner;
    args.unk_A    = owner->unk_AD60[index]->unk_00;
    args.unk_C    = owner->unk_AD60[index]->unk_08;
    args.unk_E    = owner->unk_AD60[index]->unk_0A;
    args.unk_10   = owner->unk_AD60[index]->unk_0C;
    args.unk_12   = owner->unk_AD60[index]->unk_17;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdgBP, NULL, 0, NULL, &args);
}
