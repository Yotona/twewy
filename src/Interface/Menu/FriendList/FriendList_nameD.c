#include "Interface/Menu/FriendList.h"

typedef struct {
    /* 0x00 */ Sprite            sprites[3];
    /* 0xC0 */ BOOL              visible;
    /* 0xC4 */ FriendListObject* friendList;
} FriendList_nameD; // Size: 0xC8

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
} FriendList_nameD_Args;

static SpriteFrameInfo* FriendList_nameD_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation FriendList_nameD_Anim = {
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
    .frameInfoCallback = FriendList_nameD_GetFrameInfo,
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

static SpriteFrameInfo* FriendList_nameD_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void FriendList_nameD_Load(FriendList_nameD* nameD, FriendList_nameD_Args* args) {
    SpriteAnimation anim         = FriendList_nameD_Anim;
    Point           positions[3] = {
        {41,  40},
        {41,  88},
        {41, 136},
    };

    anim.dataType = args->dataType;
    for (u16 i = 0; i < 3; i++) {
        anim.animIndex = 2;
        anim.posX      = positions[i].x;
        anim.posY      = positions[i].y;
        _Sprite_Load(&nameD->sprites[i], &anim);
    }
}

static s32 FriendList_nameD_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_nameD*      nameD    = task->data;
    FriendList_nameD_Args* initArgs = args;

    nameD->visible    = TRUE;
    nameD->friendList = initArgs->friendList;
    FriendList_nameD_Load(nameD, initArgs);
    return 1;
}

static s32 FriendList_nameD_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_nameD* nameD = task->data;

    for (s16 i = 0; i < 3; i++) {
        Sprite_Update(&nameD->sprites[i]);
    }
    return 1;
}

static s32 FriendList_nameD_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_nameD* nameD = task->data;

    for (s16 i = 0; i < 3; i++) {
        Sprite_RenderFrame(&nameD->sprites[i]);
    }
    return 1;
}

static s32 FriendList_nameD_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_nameD* nameD = task->data;

    for (s16 i = 0; i < 3; i++) {
        Sprite_Release(&nameD->sprites[i]);
    }
    return 1;
}

static s32 FriendList_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_nameD_Init,
        .update     = FriendList_nameD_Update,
        .render     = FriendList_nameD_Render,
        .cleanup    = FriendList_nameD_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_FriendList_nameD = {"Tsk_FriendList_nameD", FriendList_nameD_RunTask, sizeof(FriendList_nameD)};

s32 FriendList_nameD_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_nameD_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_nameD, NULL, 0, NULL, &args);
}
