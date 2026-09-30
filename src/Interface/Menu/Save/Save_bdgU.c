#include "Engine/File/BinMgr.h"
#include "Interface/Menu/Save.h"
#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x000 */ s32             dataType;
    /* 0x004 */ Sprite          sprites[8];
    /* 0x204 */ BOOL            visible[8];
    /* 0x224 */ SaveMenuObject* save;
} Save_bdgU; // Size: 0x228

typedef struct {
    /* 0x00 */ s32             dataType;
    /* 0x04 */ SaveMenuObject* save;
    /* 0x08 */ u16             pinId;
    /* 0x0A */ u16             iconIndex;
    /* 0x0C */ u16             slot;
    /* 0x0E */ u8              unk_E;
    /* 0x0F */ u8              bufferIndex;
    /* 0x10 */ u8              unk_10;
    /* 0x11 */ u8              level;
    /* 0x12 */ u8              maxLevel;
    /* 0x14 */ u16             levelPP;
    /* 0x16 */ u16             totalPP;
    /* 0x18 */ u16             nextLevelPP;
} Save_bdgU_Args;

// Vertical offset of the "Lv" label and level digits below the pin.
#ifdef REGION_USA
    #define LEVEL_OFFSET_Y 15
#else
    #define LEVEL_OFFSET_Y 14
#endif

static SpriteFrameInfo* Save_bdgU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Save_bdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_bdgU = {"Tsk_Save_bdgU", Save_bdgU_RunTask, sizeof(Save_bdgU)};

static const SpriteAnimation Save_bdgU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = Save_bdgU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Save_BinIdentifiers[10],
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

static void Save_bdgU_LoadPackedData(Save_bdgU* bdgU, Sprite* sprite, Save_bdgU_Args* args, u16 packIndex) {
    s32 dataType = args->dataType;

    Data* data;
    if (BinMgr_FindById((s32)&Save_BinIdentifiers[9]) == NULL) {
        data = DatMgr_LoadPackEntry(dataType, NULL, 0, &Save_BinIdentifiers[9], packIndex, FALSE);
    } else {
        data = DatMgr_LoadPackEntryDirect(dataType, &Save_BinIdentifiers[9], packIndex, 0);
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

static SpriteFrameInfo* Save_bdgU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void Save_bdgU_Load(Save_bdgU* bdgU, Sprite* sprites, Save_bdgU_Args* args) {
    SaveMenuObject* save         = bdgU->save;
    u16             digits[2]    = {1, 1};
    SpriteAnimation anim         = Save_bdgU_Anim;
    u16             gauge        = 0;
    Point           positions[6] = {
        { 61, 165},
        { 95, 165},
        {129, 165},
        {163, 165},
        {197, 165},
        {231, 165},
    };
    s16 digitX;
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
        if (args->slot >= save->badgeSlots) {
            bdgU->visible[0] = FALSE;
        } else {
            bdgU->visible[0] = TRUE;
        }
    }

    anim.dataType = args->dataType;

    anim.binIden   = &Save_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x - 7;
    anim.posY      = positions[args->slot].y - 7;
    anim.packIndex = 0;
    anim.animIndex = args->slot + 36;
    _Sprite_Load(&sprites[0], &anim);

    anim.binIden   = &Save_BinIdentifiers[7];
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
        digitX           = -3;
    } else if (args->level < 10) {
        digits[0]        = args->level;
        digits[1]        = 0;
        bdgU->visible[4] = FALSE;
        digitX           = -3;
    } else {
        digits[0] = args->level / 10;
        digits[1] = args->level % 10;
        digitX    = -4;
    }

    anim.binIden   = &Save_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.packIndex = 0;
    anim.unk_22    = 1;
    anim.animIndex = 46;
    anim.posX      = positions[args->slot].x - 5;
    anim.posY      = positions[args->slot].y + LEVEL_OFFSET_Y;
    _Sprite_Load(&sprites[2], &anim);

    anim.animIndex = digits[0] + 47;
    anim.posX      = digitX + positions[args->slot].x;
    anim.posY      = positions[args->slot].y + LEVEL_OFFSET_Y;
    _Sprite_Load(&sprites[3], &anim);

    anim.animIndex = digits[1] + 47;
    anim.posX      = positions[args->slot].x;
    anim.posY      = positions[args->slot].y + LEVEL_OFFSET_Y;
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

    anim.binIden   = &Save_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x - 10;
    anim.posY      = positions[args->slot].y + 21;
    anim.packIndex = 0;
    anim.animIndex = gauge + 7;
    _Sprite_Load(&sprites[5], &anim);

    anim.binIden   = &Save_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x - 10;
    anim.posY      = positions[args->slot].y + 21;
    anim.packIndex = 0;
    anim.animIndex = 32;
    _Sprite_Load(&sprites[6], &anim);

    anim.binIden   = &Save_BinIdentifiers[10];
    anim.unk_18    = 2;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x;
    anim.posY      = positions[args->slot].y;
    anim.packIndex = args->slot + args->bufferIndex * 6 + 1;
    anim.animIndex = 1;
    _Sprite_Load(&sprites[7], &anim);

    if (args->pinId == 0xFFFF) {
        return;
    }
    Save_bdgU_LoadPackedData(bdgU, &sprites[7], args, args->iconIndex + 1);
}

static s32 Save_bdgU_Init(TaskPool* pool, Task* task, void* args) {
    Save_bdgU*      bdgU     = task->data;
    Save_bdgU_Args* bdgUArgs = args;

    bdgU->dataType = bdgUArgs->dataType;
    bdgU->save     = bdgUArgs->save;
    Save_bdgU_Load(bdgU, bdgU->sprites, bdgUArgs);
    return 1;
}

static s32 Save_bdgU_Update(TaskPool* pool, Task* task, void* args) {
    Save_bdgU* bdgU = task->data;

    for (u16 i = 0; i < 8; i++) {
        Sprite_Update(&bdgU->sprites[i]);
    }
    return 1;
}

static s32 Save_bdgU_Render(TaskPool* pool, Task* task, void* args) {
    Save_bdgU* bdgU = task->data;

    for (u16 i = 0; i < 8; i++) {
        if (bdgU->visible[i]) {
            Sprite_RenderFrame(&bdgU->sprites[i]);
        }
    }
    return 1;
}

static s32 Save_bdgU_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_bdgU* bdgU = task->data;

    for (u16 i = 0; i < 8; i++) {
        Sprite_Release(&bdgU->sprites[i]);
    }
    return 1;
}

static s32 Save_bdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_bdgU_Init,
        .update     = Save_bdgU_Update,
        .render     = Save_bdgU_Render,
        .cleanup    = Save_bdgU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_bdgU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, SaveMenuObject* save) {
    Save_bdgU_Args args;

    args.dataType    = dataType;
    args.save        = save;
    args.pinId       = save->pins[slot].pinId;
    args.iconIndex   = save->pins[slot].iconIndex;
    args.slot        = slot;
    args.unk_E       = save->pins[slot].unk_4;
    args.bufferIndex = save->bufferIndex;
    args.unk_10      = save->pins[slot].unk_5;
    args.level       = save->pins[slot].level;
    args.maxLevel    = save->pins[slot].maxLevel;
    args.levelPP     = save->pins[slot].levelPP;
    args.totalPP     = save->pins[slot].totalPP;
    args.nextLevelPP = save->pins[slot].nextLevelPP;

    return EasyTask_CreateTask(pool, &Tsk_Save_bdgU, NULL, 0, NULL, &args);
}

void Save_bdgU_ReleaseSprite(TaskPool* pool, s32 taskId) {
    Sprite_Release(&((Save_bdgU*)EasyTask_GetTaskData(pool, taskId))->sprites[7]);
}
