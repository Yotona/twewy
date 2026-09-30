#include "Interface/Menu/FriendList.h"

typedef struct {
    /* 0x00 */ Sprite            sprites[3];
    /* 0xC0 */ char              unk_C0[0xC4 - 0xC0];
    /* 0xC4 */ FriendListObject* friendList;
} FriendList_icon; // Size: 0xC8

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
} FriendList_icon_Args;

static SpriteFrameInfo* FriendList_icon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_icon = {"Tsk_FriendList_icon", FriendList_icon_RunTask, sizeof(FriendList_icon)};

static const SpriteAnimation FriendList_icon_Anim = {
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
    .frameInfoCallback = FriendList_icon_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &FriendList_BinIdentifiers[9],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* FriendList_icon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void FriendList_icon_Load(FriendList_icon* icon, FriendList_icon_Args* args) {
    SpriteAnimation anim = FriendList_icon_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 1;
    anim.posX      = 193;
    anim.posY      = 12;
    _Sprite_Load(&icon->sprites[0], &anim);

    anim.animIndex = 3;
    anim.posX      = 218;
    anim.posY      = 12;
    _Sprite_Load(&icon->sprites[1], &anim);

    anim.animIndex = 5;
    anim.posX      = 243;
    anim.posY      = 12;
    _Sprite_Load(&icon->sprites[2], &anim);
}

static s32 FriendList_icon_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_icon*      icon     = task->data;
    FriendList_icon_Args* initArgs = args;

    icon->friendList = initArgs->friendList;
    FriendList_icon_Load(icon, initArgs);
    return 1;
}

static s32 FriendList_icon_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_icon*  icon       = task->data;
    FriendListObject* friendList = icon->friendList;
    s32               i;

    for (i = 0; i < 3; i++) {
        if (friendList->iconPressed[i] == 1) {
            FriendList_SetSpriteFrame(&icon->sprites[i], i * 2 + 2);
            if (friendList->iconTimer != 0) {
                friendList->iconTimer--;
            } else {
                friendList->iconPressed[i] = 0;
            }
        } else {
            FriendList_SetSpriteFrame(&icon->sprites[i], i * 2 + 1);
        }
    }

    for (i = 0; i < 3; i++) {
        Sprite_Update(&icon->sprites[i]);
    }
    return 1;
}

static s32 FriendList_icon_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_icon* icon = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_RenderFrame(&icon->sprites[i]);
    }
    return 1;
}

static s32 FriendList_icon_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_icon* icon = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&icon->sprites[i]);
    }
    return 1;
}

static s32 FriendList_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_icon_Init,
        .update     = FriendList_icon_Update,
        .render     = FriendList_icon_Render,
        .cleanup    = FriendList_icon_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_icon_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_icon_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_icon, NULL, 0, NULL, &args);
}
