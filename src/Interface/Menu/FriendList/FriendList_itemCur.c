#include "Interface/Menu/FriendList.h"

typedef struct {
    /* 0x00 */ Sprite            sprite;
    /* 0x40 */ BOOL              visible;
    /* 0x44 */ FriendListObject* friendList;
} FriendList_itemCur; // Size: 0x48

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
} FriendList_itemCur_Args;

static SpriteFrameInfo* FriendList_itemCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_itemCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_itemCur = {"Tsk_FriendList_itemCur", FriendList_itemCur_RunTask,
                                                  sizeof(FriendList_itemCur)};

static const SpriteAnimation FriendList_itemCur_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = FriendList_itemCur_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &FriendList_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 6,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* FriendList_itemCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void FriendList_itemCur_Load(FriendList_itemCur* itemCur, FriendList_itemCur_Args* args) {
    SpriteAnimation anim = FriendList_itemCur_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 18;
    anim.posX      = 123;
    anim.posY      = 49;
    _Sprite_Load(&itemCur->sprite, &anim);
}

static s32 FriendList_itemCur_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_itemCur*      itemCur  = task->data;
    FriendList_itemCur_Args* initArgs = args;

    itemCur->friendList = initArgs->friendList;
    itemCur->visible    = TRUE;
    FriendList_itemCur_Load(itemCur, initArgs);
    return 1;
}

static s32 FriendList_itemCur_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_itemCur* itemCur    = task->data;
    FriendListObject*   friendList = itemCur->friendList;
    u16                 scroll;
    u16                 cursor;

    if (friendList->mode != 0) {
        itemCur->visible = FALSE;
        return 1;
    }

    itemCur->visible = TRUE;
    scroll           = friendList->scroll;
    cursor           = friendList->cursor;
    if (cursor >= scroll && cursor < scroll + 3) {
        itemCur->sprite.posX = 123;
        itemCur->sprite.posY = (u16)(cursor - scroll) * 48 + 49;
        itemCur->visible     = TRUE;
    } else {
        itemCur->visible = FALSE;
    }
    if (friendList->helpOpen != 0) {
        itemCur->visible = FALSE;
    }
    Sprite_Update(&itemCur->sprite);
    return 1;
}

static s32 FriendList_itemCur_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_itemCur* itemCur = task->data;

    if (itemCur->visible) {
        Sprite_RenderFrame(&itemCur->sprite);
    }
    return 1;
}

static s32 FriendList_itemCur_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_itemCur* itemCur = task->data;

    Sprite_Release(&itemCur->sprite);
    return 1;
}

static s32 FriendList_itemCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_itemCur_Init,
        .update     = FriendList_itemCur_Update,
        .render     = FriendList_itemCur_Render,
        .cleanup    = FriendList_itemCur_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_itemCur_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_itemCur_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_itemCur, NULL, 0, NULL, &args);
}
