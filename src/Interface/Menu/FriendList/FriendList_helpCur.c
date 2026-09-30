#include "Interface/Menu/FriendList.h"

s32 Inventory_IsHelpSeen(s32);

typedef struct {
    /* 0x00 */ Sprite            sprites[3];
    /* 0xC0 */ BOOL              visible[3];
    /* 0xCC */ FriendListObject* friendList;
} FriendList_helpCur; // Size: 0xD0

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
} FriendList_helpCur_Args;

static SpriteFrameInfo* FriendList_helpCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_helpCur = {"Tsk_FriendList_helpCur", FriendList_helpCur_RunTask,
                                                  sizeof(FriendList_helpCur)};

static const SpriteAnimation FriendList_helpCur_Anim = {
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
    .frameInfoCallback = FriendList_helpCur_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &FriendList_BinIdentifiers[3],
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

static SpriteFrameInfo* FriendList_helpCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void FriendList_helpCur_Load(FriendList_helpCur* helpCur, Sprite* sprites, FriendList_helpCur_Args* args) {
    SpriteAnimation anim = FriendList_helpCur_Anim;

    for (u16 i = 0; i < 3; i++) {
        helpCur->visible[i] = TRUE;
    }

    anim.dataType = args->dataType;

    anim.animIndex = 1;
    anim.posX      = 48;
    anim.posY      = 88;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 3;
    anim.posX      = 208;
    anim.posY      = 88;
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 5;
    anim.posX      = 128;
    anim.posY      = 125;
    _Sprite_Load(&sprites[2], &anim);

    if (Inventory_IsHelpSeen(1) == 0) {
        helpCur->visible[2] = FALSE;
    }
}

static s32 FriendList_helpCur_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_helpCur_Args* helpCurArgs = args;
    FriendList_helpCur*      helpCur     = task->data;

    helpCur->friendList = helpCurArgs->friendList;
    FriendList_helpCur_Load(helpCur, helpCur->sprites, helpCurArgs);
    return 1;
}

static s32 FriendList_helpCur_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_helpCur* helpCur    = task->data;
    FriendListObject*   friendList = helpCur->friendList;
    s32                 i;

    for (i = 0; i < 3; i++) {
        if (friendList->helpButtonPressed[i] == 1) {
            FriendList_SetSpriteFrame(&helpCur->sprites[i], i * 2 + 2);
            if (friendList->iconTimer != 0) {
                friendList->iconTimer--;
            } else {
                friendList->helpButtonPressed[i] = 0;
            }
        } else {
            FriendList_SetSpriteFrame(&helpCur->sprites[i], i * 2 + 1);
        }
    }

    if (friendList->helpPage == 0) {
        helpCur->visible[0] = FALSE;
        helpCur->visible[1] = TRUE;
    } else if (friendList->helpPage == 2) {
        helpCur->visible[0] = TRUE;
        helpCur->visible[1] = FALSE;
    } else {
        helpCur->visible[0] = TRUE;
        helpCur->visible[1] = TRUE;
    }

    if (Inventory_IsHelpSeen(1) == 1) {
        helpCur->visible[2] = TRUE;
    }

    for (i = 0; i < 3; i++) {
        Sprite_Update(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 FriendList_helpCur_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        if (helpCur->visible[i]) {
            Sprite_RenderFrame(&helpCur->sprites[i]);
        }
    }
    return 1;
}

static s32 FriendList_helpCur_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 FriendList_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_helpCur_Init,
        .update     = FriendList_helpCur_Update,
        .render     = FriendList_helpCur_Render,
        .cleanup    = FriendList_helpCur_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_helpCur_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_helpCur_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_helpCur, NULL, 0, NULL, &args);
}
