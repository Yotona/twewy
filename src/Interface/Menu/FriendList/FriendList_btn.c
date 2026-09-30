#include "Interface/Menu/FriendList.h"

typedef struct {
    /* 0x00 */ Sprite            sprites[3];
    /* 0xC0 */ BOOL              visible[3];
    /* 0xCC */ FriendListObject* friendList;
    /* 0xD0 */ s16               unk_D0;
    /* 0xD2 */ char              unk_D2[0xD4 - 0xD2];
} FriendList_btn; // Size: 0xD4

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
} FriendList_btn_Args;

static SpriteFrameInfo* FriendList_btn_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_btn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation FriendList_btn_Anim = {
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
    .frameInfoCallback = FriendList_btn_GetFrameInfo,
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

static SpriteFrameInfo* FriendList_btn_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void FriendList_btn_Load(FriendList_btn* btn, FriendList_btn_Args* args) {
    SpriteAnimation anim         = FriendList_btn_Anim;
    Point           positions[3] = {
        {214,  39},
        {214,  87},
        {214, 135},
    };

    anim.dataType = args->dataType;
    for (u16 i = 0; i < 3; i++) {
        anim.animIndex = 22;
        anim.posX      = positions[i].x;
        anim.posY      = positions[i].y;
        _Sprite_Load(&btn->sprites[i], &anim);
    }
}

static s32 FriendList_btn_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_btn*      btn      = task->data;
    FriendList_btn_Args* initArgs = args;

    btn->friendList = initArgs->friendList;
    btn->unk_D0     = 0;
    for (u16 i = 0; i < 3; i++) {
        btn->visible[i] = TRUE;
    }
    FriendList_btn_Load(btn, initArgs);
    return 1;
}

static s32 FriendList_btn_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_btn*   btn        = task->data;
    FriendListObject* friendList = btn->friendList;
    u16               i;

    for (i = 0; i < 3; i++) {
        if (friendList->rowPressed[i] == 1) {
            FriendList_SetSpriteFrame(&btn->sprites[i], 23);
            if (friendList->iconTimer != 0) {
                friendList->iconTimer--;
            } else {
                friendList->rowPressed[i] = 0;
            }
        } else {
            FriendList_SetSpriteFrame(&btn->sprites[i], 22);
        }
        if (friendList->friends[friendList->scroll + i].hasSellable == 1) {
            btn->visible[i] = TRUE;
        } else {
            btn->visible[i] = FALSE;
        }
    }

    for (i = 0; i < 3; i++) {
        Sprite_Update(&btn->sprites[i]);
    }
    return 1;
}

static s32 FriendList_btn_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_btn* btn = task->data;

    for (u16 i = 0; i < 3; i++) {
        if (btn->visible[i]) {
            Sprite_RenderFrame(&btn->sprites[i]);
        }
    }
    return 1;
}

static s32 FriendList_btn_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_btn* btn = task->data;

    for (u16 i = 0; i < 3; i++) {
        Sprite_Release(&btn->sprites[i]);
    }
    return 1;
}

static s32 FriendList_btn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_btn_Init,
        .update     = FriendList_btn_Update,
        .render     = FriendList_btn_Render,
        .cleanup    = FriendList_btn_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_FriendList_btn = {"Tsk_FriendList_btn", FriendList_btn_RunTask, sizeof(FriendList_btn)};

s32 FriendList_btn_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_btn_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_btn, NULL, 0, NULL, &args);
}
