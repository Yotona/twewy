#include "Engine/File/BinMgr.h"
#include "Interface/Menu/FriendList.h"
#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x000 */ s32               dataType;
    /* 0x004 */ Sprite            sprites[4];
    /* 0x104 */ BOOL              visible[4];
    /* 0x114 */ FriendListObject* friendList;
} FriendList_bdgU; // Size: 0x118

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
    /* 0x8 */ u16               pinId;
    /* 0xA */ u16               iconIndex;
    /* 0xC */ u16               slot;
    /* 0xE */ u8                bufferIndex;
} FriendList_bdgU_Args;

static SpriteFrameInfo* FriendList_bdgU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_bdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_bdgU = {"Tsk_FriendList_bdgU", FriendList_bdgU_RunTask, sizeof(FriendList_bdgU)};

static const SpriteAnimation FriendList_bdgU_Anim = {
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
    .frameInfoCallback = FriendList_bdgU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &FriendList_BinIdentifiers[12],
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

static void FriendList_bdgU_LoadPackedData(FriendList_bdgU* bdgU, Sprite* sprite, FriendList_bdgU_Args* args, u16 packIndex) {
    s32 dataType = args->dataType;

    Data* data;
    if (BinMgr_FindById((s32)&FriendList_BinIdentifiers[10]) == NULL) {
        data = DatMgr_LoadPackEntry(dataType, NULL, 0, &FriendList_BinIdentifiers[10], packIndex, FALSE);
    } else {
        data = DatMgr_LoadPackEntryDirect(dataType, &FriendList_BinIdentifiers[10], packIndex, 0);
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

static SpriteFrameInfo* FriendList_bdgU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void FriendList_bdgU_Load(FriendList_bdgU* bdgU, Sprite* sprites, FriendList_bdgU_Args* args) {
    SpriteAnimation anim         = FriendList_bdgU_Anim;
    Point           positions[6] = {
        { 61, 165},
        { 95, 165},
        {129, 165},
        {163, 165},
        {197, 165},
        {231, 165},
    };

    anim.dataType = args->dataType;

    anim.binIden   = &FriendList_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x - 9;
    anim.posY      = positions[args->slot].y - 7;
    anim.packIndex = 0;
    anim.animIndex = args->slot + 36;
    _Sprite_Load(&sprites[0], &anim);

    anim.binIden   = &FriendList_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x + 9;
    anim.posY      = positions[args->slot].y - 7;
    anim.packIndex = 0;
    anim.animIndex = 43;
    _Sprite_Load(&sprites[1], &anim);

    anim.binIden   = &FriendList_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x - 10;
    anim.posY      = positions[args->slot].y + 21;
    anim.packIndex = 0;
    anim.animIndex = 32;
    _Sprite_Load(&sprites[2], &anim);

    anim.binIden   = &FriendList_BinIdentifiers[12];
    anim.unk_18    = 2;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x;
    anim.posY      = positions[args->slot].y;
    anim.packIndex = args->bufferIndex * 6 + args->slot + 1;
    anim.animIndex = 1;
    _Sprite_Load(&sprites[3], &anim);

    if (args->pinId == 0xFFFF) {
        bdgU->visible[3] = FALSE;
    } else {
        FriendList_bdgU_LoadPackedData(bdgU, &sprites[3], args, args->iconIndex + 1);
        bdgU->visible[3] = TRUE;
    }

    if (args->pinId == 0xFFFF) {
        bdgU->visible[0] = FALSE;
        bdgU->visible[1] = FALSE;
        bdgU->visible[2] = FALSE;
    } else {
        bdgU->visible[0] = TRUE;
        bdgU->visible[1] = FALSE;
        bdgU->visible[2] = TRUE;
    }
    bdgU->visible[0] = TRUE;
}

static s32 FriendList_bdgU_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_bdgU*      bdgU     = task->data;
    FriendList_bdgU_Args* bdgUArgs = args;

    bdgU->dataType   = bdgUArgs->dataType;
    bdgU->friendList = bdgUArgs->friendList;
    FriendList_bdgU_Load(bdgU, bdgU->sprites, bdgUArgs);
    return 1;
}

static s32 FriendList_bdgU_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_bdgU* bdgU = task->data;

    for (u16 i = 0; i < 4; i++) {
        Sprite_Update(&bdgU->sprites[i]);
    }
    return 1;
}

static s32 FriendList_bdgU_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_bdgU* bdgU = task->data;

    for (u16 i = 0; i < 4; i++) {
        if (bdgU->visible[i]) {
            Sprite_RenderFrame(&bdgU->sprites[i]);
        }
    }
    return 1;
}

static s32 FriendList_bdgU_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_bdgU* bdgU = task->data;

    for (u16 i = 0; i < 4; i++) {
        Sprite_Release(&bdgU->sprites[i]);
    }
    return 1;
}

static s32 FriendList_bdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_bdgU_Init,
        .update     = FriendList_bdgU_Update,
        .render     = FriendList_bdgU_Render,
        .cleanup    = FriendList_bdgU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_bdgU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, FriendListObject* friendList) {
    FriendList_bdgU_Args args;

    args.dataType    = dataType;
    args.friendList  = friendList;
    args.pinId       = friendList->pins[slot].itemId;
    args.iconIndex   = friendList->pins[slot].graphicIndex;
    args.slot        = slot;
    args.bufferIndex = friendList->unk_28A1;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_bdgU, NULL, 0, NULL, &args);
}

void FriendList_bdgU_ReleaseSprite(TaskPool* pool, s32 taskId) {
    Sprite_Release(&((FriendList_bdgU*)EasyTask_GetTaskData(pool, taskId))->sprites[3]);
}
