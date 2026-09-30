#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/FriendList.h"
#include "SndMgr.h"
#include "SndMgrSeIdx.h"
#include <nitro/fx/fx_division.h>

typedef struct {
    /* 0x00 */ Sprite            sprites[3];
    /* 0xC0 */ char              unk_C0[0xC4 - 0xC0];
    /* 0xC4 */ FriendListObject* friendList;
    /* 0xC8 */ u16               lastScroll;
} FriendList_sbar; // Size: 0xCC

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
} FriendList_sbar_Args;

static SpriteFrameInfo* FriendList_sbar_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_sbar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_sbar = {"Tsk_FriendList_sbar", FriendList_sbar_RunTask, sizeof(FriendList_sbar)};

static const SpriteAnimation FriendList_sbar_Anim = {
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
    .frameInfoCallback = FriendList_sbar_GetFrameInfo,
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

static u32 FriendList_sbar_PosYToScroll(u16 posY) {
#ifdef REGION_USA
    u32 scroll;
#else
    u16 scroll;
#endif

    if (posY < 75) {
        posY = 75;
    }

#ifdef REGION_USA
    scroll = (u32)(FX_Divide((posY - 75) << 12, 0x1BAA) * 0x10) >> 16;
#else
    scroll = (u16)(posY - 75) * 47 / 83;
#endif
    if (scroll > 47) {
        scroll = 47;
    }
    return scroll;
}

static u16 FriendList_sbar_ScrollToPosY(u16 scroll) {
    u16 posY = scroll * 83 / 47 + 75;

    if (posY > 158) {
        posY = 158;
    }
    return posY;
}

static SpriteFrameInfo* FriendList_sbar_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void FriendList_sbar_Load(Sprite* sprites, FriendList_sbar_Args* args) {
    SpriteAnimation anim = FriendList_sbar_Anim;

    anim.dataType = args->dataType;

    anim.animIndex = 16;
    anim.posX      = 248;
    anim.posY      = 75;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 14;
    anim.posX      = 248;
    anim.posY      = 31;
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 15;
    anim.posX      = 248;
    anim.posY      = 165;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 FriendList_sbar_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_sbar_Args* sbarArgs   = args;
    FriendListObject*     friendList = sbarArgs->friendList;

    FriendList_sbar* sbar = task->data;

    sbar->friendList = friendList;
    sbar->lastScroll = friendList->lastScroll;
    FriendList_sbar_Load(sbar->sprites, sbarArgs);
    return 1;
}

static s32 FriendList_sbar_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_sbar*  sbar       = task->data;
    FriendListObject* friendList = sbar->friendList;
    TouchCoord        coord;

    if (friendList->mode != 0) {
        return 1;
    }

    if (friendList->flags & 8) {
        TouchInput_GetCoord(&coord);
        friendList->scroll = FriendList_sbar_PosYToScroll(coord.y + 19);
        friendList->flags &= ~8;
        friendList->flags |= 0x10;
        sbar->sprites[0].posY = FriendList_sbar_ScrollToPosY(friendList->scroll);
    }

    if (friendList->flags & 0x10) {
        s16 targetY;
        s16 step;

        TouchInput_GetCoord(&coord);
        targetY = coord.y + 19;
        step    = (targetY - sbar->sprites[0].posY) >> 2;
        if (step != 0) {
            sbar->sprites[0].posY += step;
        } else {
            sbar->sprites[0].posY = targetY;
        }

        if (sbar->sprites[0].posY < 75) {
            sbar->sprites[0].posY = 75;
        } else if (sbar->sprites[0].posY > 158) {
            sbar->sprites[0].posY = 158;
        }

        friendList->scroll = FriendList_sbar_PosYToScroll(sbar->sprites[0].posY);
        if (TouchInput_WasTouchReleased()) {
            friendList->flags &= ~0x10;
        }
    } else {
        if (TouchInput_WasTouchPressed()) {
            TouchInput_GetCoord(&coord);
            if (FriendList_IsPointOnSbar(coord.x, coord.y) == 1) {
                if (FriendList_IsPointOnSbarKnob(coord.x, coord.y, sbar->sprites[0].posX, sbar->sprites[0].posY) == 1) {
                    friendList->flags |= 0x10;
                } else {
                    friendList->flags |= 8;
                }
            } else {
                s32 arrow = FriendList_GetSbarArrowAtPoint(coord.x, coord.y);

                if (arrow == 0) {
                    if (friendList->scroll >= 1) {
                        friendList->scroll -= 1;
                    }
                } else if (arrow == 1) {
                    if (friendList->scroll <= 46) {
                        friendList->scroll += 1;
                    }
                }
            }
        }
        sbar->sprites[0].posY = FriendList_sbar_ScrollToPosY(friendList->scroll);
    }

    if (sbar->lastScroll != friendList->scroll) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
    }
    sbar->lastScroll = friendList->scroll;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&sbar->sprites[i]);
    }
    return 1;
}

static s32 FriendList_sbar_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_sbar* sbar = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_RenderFrame(&sbar->sprites[i]);
    }
    return 1;
}

static s32 FriendList_sbar_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_sbar* sbar = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&sbar->sprites[i]);
    }
    return 1;
}

static s32 FriendList_sbar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_sbar_Init,
        .update     = FriendList_sbar_Update,
        .render     = FriendList_sbar_Render,
        .cleanup    = FriendList_sbar_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_sbar_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_sbar_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_sbar, NULL, 0, NULL, &args);
}
