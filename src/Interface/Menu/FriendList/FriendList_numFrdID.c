#include "Interface/Menu/FriendList.h"

#ifdef REGION_USA
    #define FRIENDLIST_NUMFRDID_X 21
#else
    #define FRIENDLIST_NUMFRDID_X 20
#endif

typedef struct {
    /* 0x000 */ Sprite            sprites[12]; // [0]-[2]: row frames, [3]-[11]: three digits per row
    /* 0x300 */ char              unk_300[0x304 - 0x300];
    /* 0x304 */ FriendListObject* friendList;
} FriendList_numFrdID; // Size: 0x308

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
    /* 0x8 */ s16               number;
} FriendList_numFrdID_Args;

static SpriteFrameInfo* FriendList_numFrdID_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_numFrdID_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_numFrdID = {"Tsk_FriendList_numFrdID", FriendList_numFrdID_RunTask,
                                                   sizeof(FriendList_numFrdID)};

static const SpriteAnimation FriendList_numFrdID_Anim = {
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
    .frameInfoCallback = FriendList_numFrdID_GetFrameInfo,
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

static SpriteFrameInfo* FriendList_numFrdID_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void FriendList_numFrdID_SetDigits(Sprite* sprites, u16 scroll) {
    s16 digits[3];
    s16 i;
    s16 row;

    for (row = 0; row < 3; row++) {
        s16 number = scroll + row + 1;
        s16 rest;

        digits[0] = number / 100;
        rest      = number % 100;
        digits[1] = rest / 10;
        digits[2] = rest % 10;
        for (i = 0; i < 3; i++) {
            FriendList_SetSpriteFrame(&sprites[(row + 1) * 3 + i], digits[i] + 4);
        }
    }
}

static void FriendList_numFrdID_Load(Sprite* sprites, FriendList_numFrdID_Args* args) {
    SpriteAnimation anim = FriendList_numFrdID_Anim;
    s16             digits[3];
    s16             row;
    s16             i;
    s32             x;
    s32             y;

    anim.dataType = args->dataType;
    for (i = 0, y = 38; i < 3; y += 48, i++) {
        anim.animIndex = 3;
        anim.posX      = 7;
        anim.posY      = y;
        _Sprite_Load(&sprites[i], &anim);
    }

    for (row = 0, y = 38; row < 3; row++, y += 48) {
        s16 number = args->number + row;
        s16 rest;

        digits[0] = number / 100;
        rest      = number % 100;
        digits[1] = rest / 10;
        digits[2] = rest % 10;
        for (i = 0, x = FRIENDLIST_NUMFRDID_X; i < 3; x += 4, i++) {
            anim.animIndex = digits[i] + 4;
            anim.posX      = x;
            anim.posY      = y;
            _Sprite_Load(&sprites[(row + 1) * 3 + i], &anim);
        }
    }
}

static s32 FriendList_numFrdID_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_numFrdID_Args* numArgs  = args;
    FriendList_numFrdID*      numFrdID = task->data;

    numFrdID->friendList = numArgs->friendList;
    FriendList_numFrdID_Load(numFrdID->sprites, numArgs);
    return 1;
}

static s32 FriendList_numFrdID_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_numFrdID* numFrdID = task->data;

    FriendList_numFrdID_SetDigits(numFrdID->sprites, numFrdID->friendList->scroll);
    for (s32 i = 0; i < 12; i++) {
        Sprite_Update(&numFrdID->sprites[i]);
    }
    return 1;
}

static s32 FriendList_numFrdID_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_numFrdID* numFrdID = task->data;

    for (s32 i = 0; i < 12; i++) {
        Sprite_RenderFrame(&numFrdID->sprites[i]);
    }
    return 1;
}

static s32 FriendList_numFrdID_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_numFrdID* numFrdID = task->data;

    for (s32 i = 0; i < 12; i++) {
        Sprite_Release(&numFrdID->sprites[i]);
    }
    return 1;
}

static s32 FriendList_numFrdID_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_numFrdID_Init,
        .update     = FriendList_numFrdID_Update,
        .render     = FriendList_numFrdID_Render,
        .cleanup    = FriendList_numFrdID_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_numFrdID_CreateTask(TaskPool* pool, s32 dataType, s32 index, FriendListObject* friendList) {
    FriendList_numFrdID_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;
    args.number     = index + 1;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_numFrdID, NULL, 0, NULL, &args);
}
