#include "Engine/File/BinMgr.h"
#include "Interface/Menu/TusinSet.h"
#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x000 */ s32             dataType;
    /* 0x004 */ Sprite          sprites[8];
    /* 0x204 */ BOOL            visible[8];
    /* 0x224 */ TusinSetObject* tusinSet;
} TusinSet_bdgU; // Size: 0x228

typedef struct {
    /* 0x00 */ s32             dataType;
    /* 0x04 */ TusinSetObject* tusinSet;
    /* 0x08 */ u16             pinId;
    /* 0x0A */ u16             iconIndex;
    /* 0x0C */ u16             slot;
    /* 0x0E */ u8              unk_E;
    /* 0x0F */ u8              unk_F;
    /* 0x10 */ u8              level;
    /* 0x11 */ u8              maxLevel;
    /* 0x12 */ u16             levelPP;
    /* 0x14 */ u16             totalPP;
    /* 0x16 */ u16             nextLevelPP;
} TusinSet_bdgU_Args;

#ifdef REGION_USA
    #define BDGU_DIGIT_OFFSET_Y 15
#else
    #define BDGU_DIGIT_OFFSET_Y 14
#endif

static SpriteFrameInfo* TusinSet_bdgU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_bdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_bdgU = {"Tsk_TusinSet_bdgU", TusinSet_bdgU_RunTask, sizeof(TusinSet_bdgU)};

static const SpriteAnimation TusinSet_bdgU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xc00,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = TusinSet_bdgU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &TusinSet_BinIdentifiers[11],
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static void TusinSet_bdgU_LoadPackedData(TusinSet_bdgU* bdgU, Sprite* sprite, TusinSet_bdgU_Args* args, u16 packIndex) {
    s32 dataType = args->dataType;

    Data* data;
    if (BinMgr_FindById((s32)&TusinSet_BinIdentifiers[9]) == NULL) {
        data = DatMgr_LoadPackEntry(dataType, NULL, 0, &TusinSet_BinIdentifiers[9], packIndex, FALSE);
    } else {
        data = DatMgr_LoadPackEntryDirect(dataType, &TusinSet_BinIdentifiers[9], packIndex, 0);
    }

    u8*   src        = (u8*)Data_GetPackEntryData(data, 1) + 4;
    void* paletteSrc = Data_GetPackEntryData(data, 4);
    u8*   dest       = (u8*)sprite->unk34 + 4;
    MI_CpuCopyU8(paletteSrc, sprite->unk3C, 0x20);

    s32 outer = 0;
    while (outer < 4) {
        s32 copyCount;
        s32 x;
        s32 y = 0;
        while (y < 4) {
            x = 0;
            while (x < 8) {
                copyCount = 0;
                while (copyCount < 4) {
                    *dest = *(src + outer * 0x80 + y * 4 + x * 0x10 + copyCount);
                    dest++;
                    copyCount++;
                }
                x++;
            }
            y++;
        }
        outer++;
    }

    DatMgr_ReleaseData(data);
}

static SpriteFrameInfo* TusinSet_bdgU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void TusinSet_bdgU_Load(TusinSet_bdgU* bdgU, Sprite* sprites, TusinSet_bdgU_Args* args) {
    u16             digits[2]    = {1, 1};
    TusinSetObject* tusinSet     = bdgU->tusinSet;
    SpriteAnimation anim         = TusinSet_bdgU_Anim;
    u16             gauge        = 0;
    Point           positions[6] = {
        { 61, 165},
        { 95, 165},
        {129, 165},
        {163, 165},
        {197, 165},
        {231, 165},
    };
    s16 offsetX;
    u16 i;

    if (args->pinId == 0xFFFF) {
        for (i = 0; i < 8; i++) {
            bdgU->visible[i] = FALSE;
        }
    } else {
        for (i = 0; i < 8; i++) {
            bdgU->visible[i] = TRUE;
        }
    }

    for (i = 0; i < 8; i++) {
        if (args->slot >= tusinSet->badgeSlots) {
            bdgU->visible[0] = FALSE;
        } else {
            bdgU->visible[0] = TRUE;
        }
    }

    anim.dataType = args->dataType;

    anim.binIden   = &TusinSet_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x - 7;
    anim.posY      = positions[args->slot].y - 7;
    anim.packIndex = 0;
    anim.animIndex = args->slot + 36;
    _Sprite_Load(&sprites[0], &anim);

    anim.binIden   = &TusinSet_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x + 11;
    anim.posY      = positions[args->slot].y - 7;
    anim.packIndex = 0;
    if (args->unk_E == 0) {
        anim.animIndex   = 1;
        bdgU->visible[1] = FALSE;
    } else if (args->unk_E == 1) {
        anim.animIndex   = 43;
        bdgU->visible[1] = TRUE;
    } else {
        anim.animIndex   = 44;
        bdgU->visible[1] = TRUE;
    }
    _Sprite_Load(&sprites[1], &anim);

    if (args->level >= args->maxLevel) {
        digits[0]        = 10;
        digits[1]        = 0;
        bdgU->visible[4] = FALSE;
        offsetX          = -3;
    } else if (args->level < 10) {
        digits[0]        = args->level;
        digits[1]        = 0;
        bdgU->visible[4] = FALSE;
        offsetX          = -3;
    } else {
        digits[0] = args->level / 10;
        digits[1] = args->level % 10;
        offsetX   = -4;
    }

    anim.binIden   = &TusinSet_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.packIndex = 0;
    anim.unk_22    = 1;
    anim.animIndex = 46;
    anim.posX      = positions[args->slot].x - 5;
    anim.posY      = positions[args->slot].y + BDGU_DIGIT_OFFSET_Y;
    _Sprite_Load(&sprites[2], &anim);

    anim.animIndex = digits[0] + 47;
    anim.posX      = offsetX + positions[args->slot].x;
    anim.posY      = positions[args->slot].y + BDGU_DIGIT_OFFSET_Y;
    _Sprite_Load(&sprites[3], &anim);

    anim.animIndex = digits[1] + 47;
    anim.posX      = positions[args->slot].x;
    anim.posY      = positions[args->slot].y + BDGU_DIGIT_OFFSET_Y;
    _Sprite_Load(&sprites[4], &anim);

    if (args->pinId != 0xFFFF) {
        if (args->level == args->maxLevel) {
            gauge = 24;
        } else if (args->totalPP == args->levelPP) {
            bdgU->visible[5] = FALSE;
        } else {
            gauge = (args->totalPP - args->levelPP) * 23 / (args->nextLevelPP - args->levelPP);
        }
    }

    anim.binIden   = &TusinSet_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x - 10;
    anim.posY      = positions[args->slot].y + 21;
    anim.packIndex = 0;
    anim.animIndex = gauge + 7;
    _Sprite_Load(&sprites[5], &anim);

    anim.binIden   = &TusinSet_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x - 10;
    anim.posY      = positions[args->slot].y + 21;
    anim.packIndex = 0;
    anim.animIndex = 32;
    _Sprite_Load(&sprites[6], &anim);

    anim.binIden   = &TusinSet_BinIdentifiers[11];
    anim.unk_18    = 2;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x;
    anim.posY      = positions[args->slot].y;
    anim.packIndex = args->slot + 1;
    anim.animIndex = 1;
    _Sprite_Load(&sprites[7], &anim);

    if (args->pinId != 0xFFFF) {
        TusinSet_bdgU_LoadPackedData(bdgU, &sprites[7], args, args->iconIndex + 1);
    }
}

static s32 TusinSet_bdgU_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_bdgU*      bdgU     = task->data;
    TusinSet_bdgU_Args* bdgUArgs = args;

    bdgU->dataType = bdgUArgs->dataType;
    bdgU->tusinSet = bdgUArgs->tusinSet;
    TusinSet_bdgU_Load(bdgU, bdgU->sprites, bdgUArgs);
    return 1;
}

static s32 TusinSet_bdgU_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_bdgU* bdgU = task->data;

    for (u16 i = 0; i < 8; i++) {
        Sprite_Update(&bdgU->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_bdgU_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_bdgU* bdgU = task->data;

    for (u16 i = 0; i < 8; i++) {
        if (bdgU->visible[i]) {
            Sprite_RenderFrame(&bdgU->sprites[i]);
        }
    }
    return 1;
}

static s32 TusinSet_bdgU_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_bdgU* bdgU = task->data;

    for (u16 i = 0; i < 8; i++) {
        Sprite_Release(&bdgU->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_bdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_bdgU_Init,
        .update     = TusinSet_bdgU_Update,
        .render     = TusinSet_bdgU_Render,
        .cleanup    = TusinSet_bdgU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_bdgU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, TusinSetObject* tusinSet) {
    TusinSet_bdgU_Args args;

    args.dataType    = dataType;
    args.tusinSet    = tusinSet;
    args.pinId       = tusinSet->pins[slot].pinId;
    args.iconIndex   = tusinSet->pins[slot].iconIndex;
    args.slot        = slot;
    args.unk_E       = tusinSet->pins[slot].unk_4;
    args.unk_F       = tusinSet->pins[slot].unk_5;
    args.level       = tusinSet->pins[slot].level;
    args.maxLevel    = tusinSet->pins[slot].maxLevel;
    args.levelPP     = tusinSet->pins[slot].levelPP;
    args.totalPP     = tusinSet->pins[slot].totalPP;
    args.nextLevelPP = tusinSet->pins[slot].nextLevelPP;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_bdgU, NULL, 0, NULL, &args);
}
