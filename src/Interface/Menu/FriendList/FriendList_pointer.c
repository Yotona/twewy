#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/FriendList.h"

typedef struct {
    /* 0x00 */ Sprite            sprite;
    /* 0x40 */ BOOL              visible;
    /* 0x44 */ FriendListObject* friendList;
} FriendList_pointer; // Size: 0x48

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
} FriendList_pointer_Args;

static SpriteFrameInfo* FriendList_pointer_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_pointer = {"Tsk_FriendList_pointer", FriendList_pointer_RunTask,
                                                  sizeof(FriendList_pointer)};

static const SpriteAnimation FriendList_pointer_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = FriendList_pointer_GetFrameInfo,
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

static SpriteFrameInfo* FriendList_pointer_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void FriendList_pointer_Load(Sprite* sprite, FriendList_pointer_Args* args) {
    SpriteAnimation anim = FriendList_pointer_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 37;
    _Sprite_Load(sprite, &anim);
}

static s32 FriendList_pointer_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_pointer*      pointer     = task->data;
    FriendList_pointer_Args* pointerArgs = args;

    pointer->friendList = pointerArgs->friendList;
    pointer->visible    = FALSE;
    FriendList_pointer_Load(&pointer->sprite, pointerArgs);
    return 1;
}

static s32 FriendList_pointer_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_pointer* pointer = task->data;
    TouchCoord          coord;

    if (TouchInput_IsTouchActive() == FALSE) {
        pointer->visible = FALSE;
    } else {
        TouchInput_GetCoord(&coord);
        pointer->sprite.posX = coord.x;
        pointer->sprite.posY = coord.y;
        pointer->visible     = TRUE;
    }

    Sprite_Update(&pointer->sprite);
    return 1;
}

static s32 FriendList_pointer_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_pointer* pointer = task->data;

    if (pointer->visible == TRUE) {
        Sprite_RenderFrame(&pointer->sprite);
    }
    return 1;
}

static s32 FriendList_pointer_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_pointer* pointer = task->data;

    Sprite_Release(&pointer->sprite);
    return 1;
}

static s32 FriendList_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_pointer_Init,
        .update     = FriendList_pointer_Update,
        .render     = FriendList_pointer_Render,
        .cleanup    = FriendList_pointer_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_pointer_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_pointer_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_pointer, NULL, 0, NULL, &args);
}
