#include "Interface/Menu/FriendList.h"

typedef struct {
    /* 0x00 */ Sprite            sprite;
    /* 0x40 */ BOOL              visible;
    /* 0x44 */ FriendListObject* friendList;
} FriendList_item; // Size: 0x48

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
    /* 0x8 */ u16               slot;
    /* 0xA */ u16               graphicIndex;
} FriendList_item_Args;

static SpriteFrameInfo* FriendList_item_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_item_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_item = {"Tsk_FriendList_item", FriendList_item_RunTask, sizeof(FriendList_item)};

static const SpriteAnimation FriendList_item_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = FriendList_item_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &FriendList_BinIdentifiers[11],
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

static SpriteFrameInfo* FriendList_item_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x384000);
}

static void FriendList_item_Load(FriendList_item* item, Sprite* sprite, FriendList_item_Args* args) {
    SpriteAnimation anim = FriendList_item_Anim;

    anim.dataType = args->dataType;
    anim.posX     = 21;
    anim.posY     = args->slot * 48 + 57;
    anim.bits_7_9 = 5;
    if (args->graphicIndex == 0xFFFF) {
        anim.packIndex = 1;
        item->visible  = FALSE;
    } else {
        anim.packIndex = args->graphicIndex + 1;
        item->visible  = TRUE;
    }
    _Sprite_Load(sprite, &anim);
}

static s32 FriendList_item_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_item*      item     = task->data;
    FriendList_item_Args* initArgs = args;

    item->friendList = initArgs->friendList;
    FriendList_item_Load(item, &item->sprite, initArgs);
    return 1;
}

static s32 FriendList_item_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_item* item = task->data;

    Sprite_Update(&item->sprite);
    return 1;
}

static s32 FriendList_item_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_item* item = task->data;

    if (item->visible) {
        Sprite_RenderFrame(&item->sprite);
    }
    return 1;
}

static s32 FriendList_item_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_item* item = task->data;

    Sprite_Release(&item->sprite);
    return 1;
}

static s32 FriendList_item_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_item_Init,
        .update     = FriendList_item_Update,
        .render     = FriendList_item_Render,
        .cleanup    = FriendList_item_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_item_CreateTask(TaskPool* pool, s32 dataType, u16 slot, FriendListObject* friendList) {
    FriendList_item_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;
    args.slot       = slot;
#ifdef REGION_USA
    args.graphicIndex = friendList->friends[friendList->scroll + slot].giftIcon;
#else
    args.graphicIndex = friendList->rowFriends[slot]->giftIcon;
#endif

    return EasyTask_CreateTask(pool, &Tsk_FriendList_item, NULL, 0, NULL, &args);
}
