#include "Interface/Menu/FriendList.h"

typedef struct {
    /* 0x00 */ Sprite            sprites[2];
    /* 0x80 */ BOOL              visible[2];
    /* 0x88 */ FriendListObject* friendList;
} FriendList_helpCurU; // Size: 0x8C

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
} FriendList_helpCurU_Args;

static SpriteFrameInfo* FriendList_helpCurU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_helpCurU = {"Tsk_FriendList_helpCurU", FriendList_helpCurU_RunTask,
                                                   sizeof(FriendList_helpCurU)};

static const SpriteAnimation FriendList_helpCurU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = FriendList_helpCurU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &FriendList_BinIdentifiers[7],
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

static SpriteFrameInfo* FriendList_helpCurU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void FriendList_helpCurU_Load(Sprite* sprites, FriendList_helpCurU_Args* args) {
    SpriteAnimation anim = FriendList_helpCurU_Anim;

    anim.dataType = args->dataType;

    anim.animIndex = 33;
    anim.posX      = 7;
    anim.posY      = 96;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 34;
    anim.posX      = 249;
    anim.posY      = 96;
    _Sprite_Load(&sprites[1], &anim);
}

static s32 FriendList_helpCurU_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_helpCurU_Args* helpCurUArgs = args;
    FriendList_helpCurU*      helpCurU     = task->data;

    helpCurU->friendList = helpCurUArgs->friendList;
    FriendList_helpCurU_Load(helpCurU->sprites, helpCurUArgs);
    return 1;
}

static s32 FriendList_helpCurU_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_helpCurU* helpCurU = task->data;

    if (helpCurU->friendList->helpPage == 0) {
        helpCurU->visible[0] = FALSE;
        helpCurU->visible[1] = TRUE;
    } else if (helpCurU->friendList->helpPage == 2) {
        helpCurU->visible[0] = TRUE;
        helpCurU->visible[1] = FALSE;
    } else {
        helpCurU->visible[0] = TRUE;
        helpCurU->visible[1] = TRUE;
    }

    for (u16 i = 0; i < 2; i++) {
        Sprite_Update(&helpCurU->sprites[i]);
    }
    return 1;
}

static s32 FriendList_helpCurU_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_helpCurU* helpCurU = task->data;

    for (u16 i = 0; i < 2; i++) {
        if (helpCurU->visible[i]) {
            Sprite_RenderFrame(&helpCurU->sprites[i]);
        }
    }
    return 1;
}

static s32 FriendList_helpCurU_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_helpCurU* helpCurU = task->data;

    for (u16 i = 0; i < 2; i++) {
        Sprite_Release(&helpCurU->sprites[i]);
    }
    return 1;
}

static s32 FriendList_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_helpCurU_Init,
        .update     = FriendList_helpCurU_Update,
        .render     = FriendList_helpCurU_Render,
        .cleanup    = FriendList_helpCurU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_helpCurU_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_helpCurU_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_helpCurU, NULL, 0, NULL, &args);
}
