#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/FriendList.h"
#include "SndMgr.h"
#include "SndMgrSeIdx.h"

typedef struct {
    /* 0x000 */ Sprite            sprites[5];
    /* 0x140 */ BOOL              visible[5];
    /* 0x154 */ FriendListObject* friendList;
} FriendList_partner; // Size: 0x158

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
} FriendList_partner_Args;

static SpriteFrameInfo* FriendList_partner_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_partner_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_partner = {"Tsk_FriendList_partner", FriendList_partner_RunTask,
                                                  sizeof(FriendList_partner)};

static const SpriteAnimation FriendList_partner_Anim = {
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
    .frameInfoCallback = FriendList_partner_GetFrameInfo,
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

static SpriteFrameInfo* FriendList_partner_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void FriendList_partner_Load(FriendList_partner* partner, Sprite* sprites, FriendList_partner_Args* args) {
    FriendListObject* friendList = partner->friendList;
    SpriteAnimation   anim       = FriendList_partner_Anim;

    anim.dataType = args->dataType;
    for (s16 i = 0; i < 3; i++) {
        anim.animIndex = i + 19;
        anim.posX      = i * 18 + 133;
        anim.posY      = 12;
        _Sprite_Load(&sprites[i], &anim);
    }

    anim.animIndex = 17;
    anim.posX      = friendList->partner * 18 + 133;
    anim.posY      = 12;
    _Sprite_Load(&sprites[3], &anim);

    anim.animIndex = 1;
    anim.posX      = 81;
    anim.posY      = 12;
    _Sprite_Load(&sprites[4], &anim);
}

static s32 FriendList_partner_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_partner*      partner  = task->data;
    FriendList_partner_Args* initArgs = args;

    partner->friendList = initArgs->friendList;
    for (u16 i = 0; i < 5; i++) {
        partner->visible[i] = TRUE;
    }
    FriendList_partner_Load(partner, partner->sprites, initArgs);
    return 1;
}

static s32 FriendList_partner_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_partner* partner    = task->data;
    FriendListObject*   friendList = partner->friendList;
    TouchCoord          touch;

    if (friendList->mode != 0) {
        partner->visible[3] = FALSE;
        return 1;
    }

    partner->visible[3] = TRUE;
    if (friendList->flags & 0x4) {
        friendList->flags &= ~0x4;
    }

    if (TouchInput_WasTouchPressed()) {
        s16 index;

        TouchInput_GetCoord(&touch);
        index = FriendList_GetPartnerAtPoint(touch.x, touch.y);
        if (index != -1 && friendList->partner != index) {
            s32 screenIndex;

            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            partner->sprites[3].posX = index * 18 + 133;
            friendList->partner      = index;
            FriendList_ChangePartner(friendList, index);
            if (friendList->partner == 0) {
                screenIndex = 0;
            } else if (friendList->partner == 1) {
                screenIndex = 4;
            } else {
                screenIndex = 5;
            }
            FriendList_ReleaseBgResource(&friendList->resources[3], DISPLAY_SUB);
            FriendList_LoadBgResourceIndexed(&friendList->resources[3], DISPLAY_SUB, 3, 5, 1, 13, screenIndex,
                                             friendList->partner);
        }
    }

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&partner->sprites[i]);
    }
    return 1;
}

static s32 FriendList_partner_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_partner* partner = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (partner->visible[i]) {
            Sprite_RenderFrame(&partner->sprites[i]);
        }
    }
    return 1;
}

static s32 FriendList_partner_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_partner* partner = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&partner->sprites[i]);
    }
    return 1;
}

static s32 FriendList_partner_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_partner_Init,
        .update     = FriendList_partner_Update,
        .render     = FriendList_partner_Render,
        .cleanup    = FriendList_partner_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_partner_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_partner_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_partner, NULL, 0, NULL, &args);
}
