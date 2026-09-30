#include "Interface/Menu/FriendList.h"

typedef struct {
    /* 0x000 */ Sprite            sprites[11];
    /* 0x2C0 */ BOOL              visible;
    /* 0x2C4 */ FriendListObject* friendList;
} FriendList_myShop; // Size: 0x2C8

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
} FriendList_myShop_Args;

static SpriteFrameInfo* FriendList_myShop_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_myShop_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_myShop = {"Tsk_FriendList_myShop", FriendList_myShop_RunTask,
                                                 sizeof(FriendList_myShop)};

static const SpriteAnimation FriendList_myShop_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = FriendList_myShop_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &FriendList_BinIdentifiers[4],
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

static SpriteFrameInfo* FriendList_myShop_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void FriendList_myShop_Load(Sprite* sprites, FriendList_myShop_Args* args) {
    SpriteAnimation anim          = FriendList_myShop_Anim;
    Point           positions[11] = {
        { 49,  46},
        { 49,  86},
        { 49, 126},
        {217,  50},
        {217,  90},
        {217, 130},
        {217,  69},
        {217, 109},
        {217, 149},
        { 63, 163},
        {128,  96},
    };
    u16 i;

    anim.dataType = args->dataType;
    for (i = 0; i < 3; i++) {
        anim.animIndex = i + 1;
        anim.posX      = positions[i].x;
        anim.posY      = positions[i].y;
        _Sprite_Load(&sprites[i], &anim);
    }
    for (i = 0; i < 3; i++) {
        anim.animIndex = 4;
        anim.posX      = positions[i + 3].x;
        anim.posY      = positions[i + 3].y;
        _Sprite_Load(&sprites[i + 3], &anim);
    }
    for (i = 0; i < 3; i++) {
        anim.animIndex = 6;
        anim.posX      = positions[i + 6].x;
        anim.posY      = positions[i + 6].y;
        _Sprite_Load(&sprites[i + 6], &anim);
    }

    anim.animIndex = 8;
    anim.posX      = positions[9].x;
    anim.posY      = positions[9].y;
    _Sprite_Load(&sprites[9], &anim);

    anim.animIndex = 10;
    anim.posX      = positions[10].x;
    anim.posY      = positions[10].y;
    _Sprite_Load(&sprites[10], &anim);
}

static s32 FriendList_myShop_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_myShop_Args* myShopArgs = args;
    FriendList_myShop*      myShop     = task->data;

    myShop->visible    = FALSE;
    myShop->friendList = myShopArgs->friendList;
    FriendList_myShop_Load(myShop->sprites, myShopArgs);
    return 1;
}

static s32 FriendList_myShop_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_myShop* myShop     = task->data;
    FriendListObject*  friendList = myShop->friendList;
    s16                i;

    if (friendList->mode == 1) {
        myShop->visible = TRUE;
    } else {
        myShop->visible = FALSE;
    }

    if (friendList->btnPressed == 1) {
        FriendList_SetSpriteFrame(&myShop->sprites[9], 9);
        if (friendList->iconTimer != 0) {
            friendList->iconTimer--;
        } else {
            friendList->btnPressed = 0;
        }
    } else {
        FriendList_SetSpriteFrame(&myShop->sprites[9], 8);
    }

    for (i = 0; i < 3; i++) {
        if (friendList->arrowPressed[i] == 1) {
            FriendList_SetSpriteFrame(&myShop->sprites[i + 3], 5);
            if (friendList->iconTimer != 0) {
                friendList->iconTimer--;
            } else {
                friendList->arrowPressed[i] = 0;
            }
        } else {
            FriendList_SetSpriteFrame(&myShop->sprites[i + 3], 4);
        }
    }

    for (i = 0; i < 3; i++) {
        if (friendList->arrowPressed[i + 3] == 1) {
            FriendList_SetSpriteFrame(&myShop->sprites[i + 6], 7);
            if (friendList->iconTimer != 0) {
                friendList->iconTimer--;
            } else {
                friendList->arrowPressed[i + 3] = 0;
            }
        } else {
            FriendList_SetSpriteFrame(&myShop->sprites[i + 6], 6);
        }
    }

    for (i = 0; i < 11; i++) {
        Sprite_Update(&myShop->sprites[i]);
    }
    return 1;
}

static s32 FriendList_myShop_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_myShop* myShop = task->data;

    for (s16 i = 0; i < 11; i++) {
        if (myShop->visible) {
            Sprite_RenderFrame(&myShop->sprites[i]);
        }
    }
    return 1;
}

static s32 FriendList_myShop_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_myShop* myShop = task->data;

    for (s16 i = 0; i < 11; i++) {
        Sprite_Release(&myShop->sprites[i]);
    }
    return 1;
}

static s32 FriendList_myShop_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_myShop_Init,
        .update     = FriendList_myShop_Update,
        .render     = FriendList_myShop_Render,
        .cleanup    = FriendList_myShop_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_myShop_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_myShop_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_myShop, NULL, 0, NULL, &args);
}
