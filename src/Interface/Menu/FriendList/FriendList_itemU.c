#include "Engine/File/BinMgr.h"
#include "Interface/Menu/FriendList.h"
#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x00 */ s32               dataType;
    /* 0x04 */ Sprite            sprite;
    /* 0x44 */ BOOL              visible;
    /* 0x48 */ FriendListObject* friendList;
} FriendList_itemU; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
    /* 0x8 */ u16               itemId;
    /* 0xA */ u16               graphicIndex;
    /* 0xC */ u16               slot;
    /* 0xE */ u8                bufferIndex;
} FriendList_itemU_Args;

static SpriteFrameInfo* FriendList_itemU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_itemU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_itemU = {"Tsk_FriendList_itemU", FriendList_itemU_RunTask, sizeof(FriendList_itemU)};

static const SpriteAnimation FriendList_itemU_Anim = {
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
    .frameInfoCallback = FriendList_itemU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &FriendList_BinIdentifiers[13],
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

static void FriendList_itemU_LoadPackedData(FriendList_itemU* itemU, Sprite* sprite, FriendList_itemU_Args* args,
                                            u16 packIndex) {
    s32 dataType = args->dataType;

    Data* data;
    if (BinMgr_FindById((s32)&FriendList_BinIdentifiers[11]) == NULL) {
        data = DatMgr_LoadPackEntry(dataType, NULL, 0, &FriendList_BinIdentifiers[11], packIndex, FALSE);
    } else {
        data = DatMgr_LoadPackEntryDirect(dataType, &FriendList_BinIdentifiers[11], packIndex, 0);
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

static SpriteFrameInfo* FriendList_itemU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void FriendList_itemU_Load(FriendList_itemU* itemU, Sprite* sprite, FriendList_itemU_Args* args) {
    SpriteAnimation anim         = FriendList_itemU_Anim;
    Point           positions[9] = {
        { 81, 105},
        {110, 105},
        { 81, 134},
        {110, 134},
        {209, 105},
        {238, 105},
        {209, 134},
        {238, 134},
        { 20, 172},
    };

    anim.dataType  = args->dataType;
    anim.posX      = positions[args->slot].x;
    anim.posY      = positions[args->slot].y;
    anim.packIndex = args->slot + args->bufferIndex * 9 + 1;
    _Sprite_Load(sprite, &anim);

    if (args->itemId == 0xFFFF) {
        itemU->visible = FALSE;
        return;
    }

    FriendList_itemU_LoadPackedData(itemU, sprite, args, args->graphicIndex + 1);
    itemU->visible = TRUE;
}

static s32 FriendList_itemU_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_itemU*      itemU     = task->data;
    FriendList_itemU_Args* itemUArgs = args;

    itemU->dataType   = itemUArgs->dataType;
    itemU->friendList = itemUArgs->friendList;
    FriendList_itemU_Load(itemU, &itemU->sprite, itemUArgs);
    return 1;
}

static s32 FriendList_itemU_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_itemU* itemU = task->data;

    Sprite_Update(&itemU->sprite);
    return 1;
}

static s32 FriendList_itemU_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_itemU* itemU = task->data;

    if (itemU->visible) {
        Sprite_RenderFrame(&itemU->sprite);
    }
    return 1;
}

static s32 FriendList_itemU_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_itemU* itemU = task->data;

    Sprite_Release(&itemU->sprite);
    return 1;
}

static s32 FriendList_itemU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_itemU_Init,
        .update     = FriendList_itemU_Update,
        .render     = FriendList_itemU_Render,
        .cleanup    = FriendList_itemU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_itemU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, FriendListObject* friendList) {
    FriendList_itemU_Args args;

    args.dataType     = dataType;
    args.friendList   = friendList;
    args.itemId       = friendList->slots[slot]->itemId;
    args.graphicIndex = friendList->slots[slot]->graphicIndex;
    args.slot         = slot;
    args.bufferIndex  = friendList->unk_28A1;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_itemU, NULL, 0, NULL, &args);
}

void FriendList_itemU_ReleaseSprite(TaskPool* pool, s32 taskId) {
    Sprite_Release(&((FriendList_itemU*)EasyTask_GetTaskData(pool, taskId))->sprite);
}
