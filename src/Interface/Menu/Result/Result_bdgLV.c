#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x00 */ Sprite        sprites[3];
    /* 0xC0 */ BOOL          visible[3];
    /* 0xCC */ ResultObject* owner;
    /* 0xD0 */ u16           index;
    /* 0xD2 */ s16           onesOffsetX;
    /* 0xD4 */ u16           tensDigit;
    /* 0xD6 */ u16           onesDigit;
} Result_bdgLV; // Size: 0xD8

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           index;
} Result_bdgLV_Args;

static SpriteFrameInfo* Result_bdgLV_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              Result_bdgLV_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Result_bdgLV_Anim = {
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
    .unk_08     = Result_bdgLV_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &Result_BinIdentifiers[13],
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* Result_bdgLV_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return info;
        } break;

        case 2: {
            data_0206b408.unk_04 = 0;
            data_0206b408.unk_08 = 0;
            data_0206b408.unk_0C = 0;
            data_0206b408.unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                data_0206b408.unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                data_0206b408.unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info->unk_10 = 0x320000;
            return info;
        } break;
    }

    return NULL;
}

static void Result_bdgLV_UpdateDigits(Result_bdgLV* bdgLV, u16 index) {
    ResultObject* owner        = bdgLV->owner;
    u16           graphicIndex = owner->badges[index].pinID;
    u8            level        = owner->badges[index].level;
    u8            maxLevel     = owner->badges[index].maxLevel;

    bdgLV->onesOffsetX = -5;

    if (graphicIndex == 0xFFFF) {
        bdgLV->visible[0] = FALSE;
        bdgLV->visible[1] = FALSE;
        bdgLV->visible[2] = FALSE;
        return;
    }

    if (level >= maxLevel) {
        bdgLV->tensDigit   = 10;
        bdgLV->onesDigit   = 0;
        bdgLV->visible[0]  = TRUE;
        bdgLV->visible[1]  = TRUE;
        bdgLV->visible[2]  = FALSE;
        bdgLV->onesOffsetX = -5;
        return;
    }

    if (level < 10) {
        bdgLV->tensDigit   = level;
        bdgLV->onesDigit   = 0;
        bdgLV->visible[0]  = TRUE;
        bdgLV->visible[1]  = TRUE;
        bdgLV->visible[2]  = FALSE;
        bdgLV->onesOffsetX = -5;
        return;
    }

    bdgLV->tensDigit   = level / 10;
    bdgLV->onesDigit   = level % 10;
    bdgLV->visible[0]  = TRUE;
    bdgLV->visible[1]  = TRUE;
    bdgLV->visible[2]  = TRUE;
    bdgLV->onesOffsetX = -6;
}

static void Result_bdgLV_Load(Result_bdgLV* bdgLV, Sprite* sprites, Result_bdgLV_Args* args) {
    SpriteAnimation anim    = Result_bdgLV_Anim;
    s16             posX[6] = {0x2C, 0x4E, 0x70, 0x92, 0xB4, 0xD6};

    anim.dataType = args->dataType;
    anim.bits_7_9 = 6;

    Result_bdgLV_UpdateDigits(bdgLV, args->index);

    anim.unk_2A = 0x29;
    anim.unk_04 = posX[args->index] - 7;
    anim.unk_06 = 0xB2;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = bdgLV->tensDigit + 0x2A;
    anim.unk_04 = bdgLV->onesOffsetX + posX[args->index];
    anim.unk_06 = 0xB2;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = bdgLV->onesDigit + 0x2A;
    anim.unk_04 = posX[args->index] - 2;
    anim.unk_06 = 0xB2;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 Result_bdgLV_Init(TaskPool* pool, Task* task, void* args) {
    Result_bdgLV*      bdgLV     = task->data;
    Result_bdgLV_Args* bdgLVArgs = args;

    bdgLV->owner     = bdgLVArgs->owner;
    bdgLV->index     = bdgLVArgs->index;
    bdgLV->tensDigit = 1;
    bdgLV->onesDigit = 1;
    Result_bdgLV_Load(bdgLV, bdgLV->sprites, bdgLVArgs);
    return 1;
}

static s32 Result_bdgLV_Update(TaskPool* pool, Task* task, void* args) {
    Result_bdgLV* bdgLV = task->data;

    Result_bdgLV_UpdateDigits(bdgLV, bdgLV->index);
    Result_SetSpriteFrame(&bdgLV->sprites[1], bdgLV->tensDigit + 0x2A);
    Result_SetSpriteFrame(&bdgLV->sprites[2], bdgLV->onesDigit + 0x2A);

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&bdgLV->sprites[i]);
    }
    return 1;
}

static s32 Result_bdgLV_Render(TaskPool* pool, Task* task, void* args) {
    Result_bdgLV* bdgLV = task->data;

    for (s32 i = 0; i < 3; i++) {
        if (bdgLV->visible[i] != 0) {
            Sprite_RenderFrame(&bdgLV->sprites[i]);
        }
    }
    return 1;
}

static s32 Result_bdgLV_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_bdgLV* bdgLV = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&bdgLV->sprites[i]);
    }
    return 1;
}

static s32 Result_bdgLV_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_bdgLV_Init,
        .update     = Result_bdgLV_Update,
        .render     = Result_bdgLV_Render,
        .cleanup    = Result_bdgLV_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Result_bdgLV = {"Tsk_Result_bdgLV", Result_bdgLV_RunTask, sizeof(Result_bdgLV)};

s32 Result_bdgLV_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner) {
    Result_bdgLV_Args args;

    args.dataType = dataType;
    args.owner    = owner;
    args.index    = index;

    return EasyTask_CreateTask(pool, &Tsk_Result_bdgLV, NULL, 0, NULL, &args);
}
