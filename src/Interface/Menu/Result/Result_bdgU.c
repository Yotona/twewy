#include "Engine/File/DatMgr.h"
#include "Engine/IO/Input.h"
#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Result.h"
#include <nitro/mi/cpumem.h>

// JP slides the badges in slower and more leftward
#ifdef REGION_USA
    #define BDGU_SPEED  0x17000
    #define BDGU_DECEL  -0xE66
    #define BDGU_BASE_X 0x62000
#else
    #define BDGU_SPEED  0x12000
    #define BDGU_DECEL  -0xCCD
    #define BDGU_BASE_X 0xE000
#endif

typedef struct {
    /* 0x000 */ Sprite        sprites[4];
    /* 0x100 */ BOOL          visible[4];
    /* 0x110 */ ResultObject* owner;
    /* 0x114 */ s32           posX;
    /* 0x118 */ s32           posY;
    /* 0x11C */ s32           speed;
    /* 0x120 */ s32           unk_120;
    /* 0x124 */ s32           decel;
    /* 0x128 */ s32           unk_128;
    /* 0x12C */ u16           state;
    /* 0x12E */ u16           unk_12E;
    /* 0x130 */ s16           slot;
    /* 0x132 */ s16           countOffsetX;
    /* 0x134 */ s32           targetX;
    /* 0x138 */ u16           index;
    /* 0x13A */ u16           pinCount;
} Result_bdgU; // Size: 0x13C

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           index;
    /* 0xA */ u16           pinId;
    /* 0xC */ u16           count;
    /* 0xE */ u16           pinCount;
} Result_bdgU_Args;

static SpriteFrameInfo* Result_bdgU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_bdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Result_bdgU = {"Tsk_Result_bdgU", Result_bdgU_RunTask, sizeof(Result_bdgU)};

static const SpriteAnimation Result_bdgU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = Result_bdgU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Result_BinIdentifiers[28],
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* Result_bdgU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void Result_bdgU_LoadPackedData(Result_bdgU* bdgU, Sprite* sprite, Result_bdgU_Args* args, u16 packIndex) {
    s32   dataType = args->dataType;
    Data* data;
    u8*   charData;
    u8*   palette;
    u8*   dst;
    s32   col;
    s32   row;
    s32   blockX;
    s32   blockY;

    if (BinMgr_FindById((s32)&Result_BinIdentifiers[27]) == NULL) {
        data = DatMgr_LoadPackEntry(dataType, NULL, 0, (BinIdentifier*)&Result_BinIdentifiers[27], packIndex, FALSE);
    } else {
        data = DatMgr_LoadPackEntryDirect(dataType, (BinIdentifier*)&Result_BinIdentifiers[27], packIndex, 0);
    }

    charData = (u8*)Data_GetPackEntryData(data, 1) + 4;
    palette  = Data_GetPackEntryData(data, 4);

    dst = (u8*)sprite->unk34 + 4;
    MI_CpuCopyU8(palette, sprite->unk3C, 0x20);

    for (blockY = 0; blockY < 4; blockY++) {
        for (blockX = 0; blockX < 4; blockX++) {
            for (row = 0; row < 8; row++) {
                for (col = 0; col < 4; col++) {
                    *dst++ = charData[blockY * 0x80 + blockX * 4 + row * 0x10 + col];
                }
            }
        }
    }

    DatMgr_ReleaseData(data);
}

static void Result_bdgU_LoadIcon(Result_bdgU* bdgU, Sprite* sprites, Result_bdgU_Args* args, SpriteAnimation* anim) {
    anim->packIndex = args->index % 7 + 1;
    anim->binIden   = &Result_BinIdentifiers[28];
    anim->unk_18    = 2;
    anim->unk_22    = 1;
    anim->unk_2A    = 1;
    anim->unk_04    = bdgU->posX >> 12;
    anim->unk_06    = bdgU->posY >> 12;
    _Sprite_Load(&sprites[3], anim);

    bdgU->visible[3] = TRUE;
    Result_bdgU_LoadPackedData(bdgU, &sprites[3], args, args->pinId + 1);
}

static void Result_bdgU_LoadCount(Result_bdgU* bdgU, Sprite* sprites, Result_bdgU_Args* args, SpriteAnimation* anim) {
    u16 tens;
    u16 ones = 0;

    anim->packIndex = 0;
    anim->binIden   = &Result_BinIdentifiers[25];
    anim->unk_18    = 0;
    anim->unk_22    = 4;

    tens = args->count;
    if (tens < 10) {
        bdgU->countOffsetX = 0;
        bdgU->visible[0]   = TRUE;
        bdgU->visible[1]   = TRUE;
        bdgU->visible[2]   = FALSE;
    } else {
        tens               = args->count / 10;
        ones               = args->count % 10;
        bdgU->countOffsetX = -2;
        bdgU->visible[0]   = TRUE;
        bdgU->visible[1]   = TRUE;
        bdgU->visible[2]   = TRUE;
    }

    anim->unk_2A = 0x11;
    anim->unk_04 = bdgU->countOffsetX + ((bdgU->posX >> 12) + 9);
    anim->unk_06 = (bdgU->posY >> 12) + 0xB;
    _Sprite_Load(&sprites[0], anim);

    anim->unk_2A = tens + 7;
    anim->unk_04 = bdgU->countOffsetX + ((bdgU->posX >> 12) + 9);
    anim->unk_06 = (bdgU->posY >> 12) + 0xB;
    _Sprite_Load(&sprites[1], anim);

    anim->unk_2A = ones + 7;
    anim->unk_04 = bdgU->countOffsetX + ((bdgU->posX >> 12) + 0xD);
    anim->unk_06 = (bdgU->posY >> 12) + 0xB;
    _Sprite_Load(&sprites[2], anim);
}

static void Result_bdgU_Load(Result_bdgU* bdgU, Sprite* sprites, Result_bdgU_Args* args) {
    SpriteAnimation anim = Result_bdgU_Anim;

    anim.dataType = args->dataType;

    bdgU->state        = 0;
    bdgU->unk_12E      = 0;
    bdgU->countOffsetX = 0;
    bdgU->index        = args->index;
    bdgU->pinCount     = args->pinCount;
    bdgU->speed        = BDGU_SPEED;
    bdgU->decel        = BDGU_DECEL;
    bdgU->posY         = 0xE000;
    bdgU->unk_120      = -0x4000;
    bdgU->unk_128      = 0;

    if (bdgU->index < 6) {
        bdgU->posX    = -0xE000 - ((bdgU->index * 0x1C) << 12);
        bdgU->targetX = (((5 - bdgU->index) * 0x1C) << 12) + BDGU_BASE_X;
        bdgU->slot    = 5 - bdgU->index;
    } else {
        bdgU->posX    = -0x9A000;
        bdgU->targetX = BDGU_BASE_X;
        bdgU->slot    = 0;
    }

    Result_bdgU_LoadCount(bdgU, sprites, args, &anim);
    Result_bdgU_LoadIcon(bdgU, sprites, args, &anim);
}

static s32 Result_bdgU_Init(TaskPool* pool, Task* task, void* args) {
    Result_bdgU*      bdgU     = task->data;
    Result_bdgU_Args* bdgUArgs = args;

    bdgU->owner = bdgUArgs->owner;
    Result_bdgU_Load(bdgU, bdgU->sprites, bdgUArgs);
    return 1;
}

static s32 Result_bdgU_Update(TaskPool* pool, Task* task, void* args) {
    Result_bdgU*  bdgU  = task->data;
    ResultObject* owner = bdgU->owner;
    s32           targetX;

    switch (bdgU->state) {
        case 0: {
            if (bdgU->posX < bdgU->targetX) {
                bdgU->posX += bdgU->speed;
            } else if (bdgU->posX > bdgU->targetX) {
                bdgU->posX -= bdgU->speed;
            }

            if (bdgU->speed > 0) {
                bdgU->speed += bdgU->decel;
                if (bdgU->speed <= 0) {
                    bdgU->speed = 0;
                    bdgU->state = 1;
                }
            }

            if (TouchInput_IsTouchActive() || (InputStatus.buttonState.currButtons & 1)) {
                bdgU->posX  = bdgU->targetX;
                bdgU->state = 1;
            }
        } break;

        case 1: {
            if (owner->stageFlags & 4) {
                if (bdgU->slot >= 5) {
                    bdgU->state = 3;
                } else {
                    bdgU->state = 2;
                    bdgU->slot++;
                }
            }
        } break;

        case 2: {
            targetX = ((bdgU->slot * 0x1C) << 12) + BDGU_BASE_X;
            if (bdgU->posX >= targetX) {
                bdgU->posX  = targetX;
                bdgU->state = 1;
            } else {
                bdgU->posX += 0x2000;
            }

#ifdef REGION_USA
            if (TouchInput_IsTouchActive() || (InputStatus.buttonState.currButtons & 1)) {
                bdgU->posX  = targetX;
                bdgU->state = 1;
            }
#endif
        } break;

        case 3: {
            if (bdgU->posY < -0xE000) {
                return 0;
            }
            bdgU->posY -= 0x4000;

#ifdef REGION_USA
            if (TouchInput_IsTouchActive() || (InputStatus.buttonState.currButtons & 1)) {
                return 0;
            }
#endif
        } break;
    }

    bdgU->sprites[3].posX = bdgU->posX >> 12;
    bdgU->sprites[0].posX = bdgU->sprites[3].posX + 9 + bdgU->countOffsetX;
    bdgU->sprites[1].posX = bdgU->sprites[3].posX + 9 + bdgU->countOffsetX;
    bdgU->sprites[2].posX = bdgU->sprites[3].posX + 0xD + bdgU->countOffsetX;
    bdgU->sprites[3].posY = bdgU->posY >> 12;
    bdgU->sprites[0].posY = bdgU->sprites[3].posY + 0xB;
    bdgU->sprites[1].posY = bdgU->sprites[3].posY + 0xB;
    bdgU->sprites[2].posY = bdgU->sprites[3].posY + 0xB;

    for (s32 i = 0; i < 4; i++) {
        Sprite_Update(&bdgU->sprites[i]);
    }
    return 1;
}

static s32 Result_bdgU_Render(TaskPool* pool, Task* task, void* args) {
    Result_bdgU* bdgU = task->data;

    for (s32 i = 0; i < 4; i++) {
        if (bdgU->visible[i] != 0) {
            Sprite_RenderFrame(&bdgU->sprites[i]);
        }
    }
    return 1;
}

static s32 Result_bdgU_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_bdgU* bdgU = task->data;

    for (s32 i = 0; i < 4; i++) {
        Sprite_Release(&bdgU->sprites[i]);
    }
    return 1;
}

static s32 Result_bdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_bdgU_Init,
        .update     = Result_bdgU_Update,
        .render     = Result_bdgU_Render,
        .cleanup    = Result_bdgU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Result_bdgU_CreateTask(TaskPool* pool, s32 dataType, s32 mode, u16 index, ResultObject* owner) {
    Result_bdgU_Args args;

    args.dataType = dataType;
    args.owner    = owner;
    args.index    = index;
    args.pinId    = owner->dropPinIDs[index];
    args.count    = owner->dropQuantities[index];
    args.pinCount = owner->dropCount;

    return EasyTask_CreateTask(pool, &Tsk_Result_bdgU, NULL, 0, NULL, &args);
}
