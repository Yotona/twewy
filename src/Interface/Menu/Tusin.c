#include "Interface/Menu/Tusin.h"
#include "CriSndMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/HBlank.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Save.h"
#include "SndMgr.h"
#include "SndMgrSeIdx.h"
#include "Util/SysFont.h"
#include "common_data.h"
#include <nitro/fs/overlay.h>
#include <nitro/gx.h>
#include <nitro/mi/cpumem.h>
#include <nitro/reg.h>
#include <nitro/rtc.h>

// A beacon found by the wireless library's scan.
typedef struct {
    /* 0x0 */ char unk_0[0x4];
    /* 0x4 */ u8   macAddress[6];
} TusinBeacon;

typedef struct {
    /* 0x00 */ char         unk_00[0xE];
    /* 0x0E */ u16          count;
    /* 0x10 */ TusinBeacon* beacons[1];
} TusinScanResult;

// A player whose beacon was seen during this session.
typedef struct {
    /* 0x00 */ u8   macAddress[6];
    /* 0x06 */ char unk_06[0x8 - 0x6];
    /* 0x08 */ BOOL used;
    /* 0x0C */ s64  expireTime; // JP only: frames until the player can be counted again
} TusinSeenPlayer;              // Size: 0x14

// What one player sends to another.
typedef struct {
    /* 0x000 */ u8  macAddress[6];
    /* 0x006 */ u16 nickName[11];
    /* 0x01C */ u16 message[27];
    /* 0x052 */ u8  profile[sizeof(TusinProfile)];
} TusinPacket; // Size: 0x2E6

typedef struct {
    /* 0x00000 */ MenuStateBase   base;
    /* 0x21618 */ u8              comm[0x21AC8 - 0x21618]; // Wireless library state (overlay 40)
    /* 0x21AC8 */ s32             commBusy;
    /* 0x21ACC */ TusinSeenPlayer seenPlayers[65];
    /* 0x21FE0 */ s64             seenDuration; // Seconds before the same player counts again (JP only)
    /* 0x21FE8 */ TusinPacket     sendPacket;
    /* 0x222CE */ TusinPacket     recvPackets[10];
    /* 0x23FCA */ char            unk_23FCA[0x23FCC - 0x23FCA];
    /* 0x23FCC */ s32             recvCount;
    /* 0x23FD0 */ u16             recvKinds[10]; // 0: esper, 1: civvy, 2: alien
    /* 0x23FE4 */ s32             taskId_Btn;
    /* 0x23FE8 */ s32             taskId_BtnF;
    /* 0x23FEC */ s32             taskId_BtnLR;
    /* 0x23FF0 */ s32             taskId_Num;
#ifdef REGION_USA
    /* 0x23FF4 */ s32 unk_23FF4;
#endif
    /* 0x23FF8 */ s32               taskId_TextScr;
    /* 0x23FFC */ s32               taskId_BeltU[10];
    /* 0x24024 */ s32               unk_24024;
    /* 0x24028 */ s32               exitReady;
    /* 0x2402C */ s16               timer;
    /* 0x2402E */ u16               unk_2402E;
    /* 0x24030 */ u16               shownCount; // Received players already shown on a belt
    /* 0x24032 */ char              unk_24032[0x24034 - 0x24032];
    /* 0x24034 */ GlobalFriendData* friendData;
    /* 0x24038 */ TusinObject       tusin;
} TusinState; // Size: 0x24624 (JP: 0x244A0)

void GX_LoadBgPltt(void* src, u32 offset, u32 size);
void GX_LoadObjPltt(void* src, u32 offset, u32 size);
void GXs_LoadBgPltt(void* src, u32 offset, u32 size);
void GXs_LoadObjPltt(void* src, u32 offset, u32 size);
void func_0202b878(void);
s32  func_020417e0(RTCDate* date, RTCTime* time);
void func_020415a4(void);
BOOL func_02001b44(s32, s32, void*, s32);
void func_02041060(s32* status);

// The wireless library (USA overlay 40, JP overlay 39) is not decompiled yet.
#ifdef REGION_USA
    #define Comm_Init    func_ov040_0209dfdc
    #define Comm_Exit    func_ov040_0209e028
    #define Comm_IsBusy  func_ov040_0209e03c
    #define Comm_Start   func_ov040_0209e888
    #define Comm_Stop    func_ov040_0209e900
    #define Comm_VBlank  func_ov040_0209e91c
    #define OVERLAY_COMM OVERLAY_40_ID
#else
    #define Comm_Init    func_ov039_0209e95c
    #define Comm_Exit    func_ov039_0209e9a8
    #define Comm_IsBusy  func_ov039_0209e9bc
    #define Comm_Start   func_ov039_0209f208
    #define Comm_Stop    func_ov039_0209f280
    #define Comm_VBlank  func_ov039_0209f29c
    #define OVERLAY_COMM OVERLAY_39_ID
#endif

void Comm_Init(void* comm, u16 (*onScan)(TusinState*, TusinScanResult*, u16), TusinState* scanArg,
               void (*onReceive)(TusinState*, s32, TusinPacket*, u32), TusinState* receiveArg, TusinPacket* sendPacket,
               u32 packetSize);
void Comm_Exit(void* comm);
s32  Comm_IsBusy(void* comm);
void Comm_Start(void* comm);
void Comm_Stop(void* comm);
void Comm_VBlank(void);

// JP has no counterpart to USA's ov036, so every later overlay number is one lower there.
#ifdef REGION_USA
    #define TUSIN_OVL_MENU         OVERLAY_43_ID
    #define TUSIN_OVL_MENU_ENTRY   ((void*)0x02084040) /* ProcessOverlay_MenuTop */
    #define TUSIN_OVL_SET          OVERLAY_45_ID
    #define TUSIN_OVL_SET_ENTRY    ProcessOverlay_TusinSet
    #define TUSIN_OVL_RESULT       OVERLAY_44_ID
    #define TUSIN_OVL_RESULT_ENTRY ((void*)0x02084A88)
    #define TUSIN_OVL2_ENTRY_3     ((void*)0x02086B0C)
    #define TUSIN_OVL2_ENTRY_4     ((void*)0x02086B4C)
    #define TUSIN_OVL2_ENTRY_5     ((void*)0x02086A8C)
#else
    #define TUSIN_OVL_MENU         OVERLAY_42_ID
    #define TUSIN_OVL_MENU_ENTRY   ((void*)0x020849C4)
    #define TUSIN_OVL_SET          OVERLAY_44_ID
    #define TUSIN_OVL_SET_ENTRY    ProcessOverlay_TusinSet
    #define TUSIN_OVL_RESULT       OVERLAY_43_ID
    #define TUSIN_OVL_RESULT_ENTRY ((void*)0x020853C4)
    #define TUSIN_OVL2_ENTRY_3     ((void*)0x020873AC)
    #define TUSIN_OVL2_ENTRY_4     ((void*)0x020873EC)
    #define TUSIN_OVL2_ENTRY_5     ((void*)0x0208736C)
#endif
extern u32 OVERLAY_42_ID;

extern void TUSIN_OVL_SET_ENTRY(void* state);

void Tusin_StoreFriend(TusinState* state, u16 index, u8 timesMet, u8 lastShopId);
void Tusin_ProcessEsper(TusinState* state);
void Tusin_StageFadeIn(TusinState* state);
void Tusin_StageStartComm(TusinState* state);
void Tusin_StageMain(TusinState* state);
void Tusin_StageBeginSave(TusinState* state);
void Tusin_StageSave(TusinState* state);
void Tusin_StageSaving(TusinState* state);
void Tusin_StageSaveDone(TusinState* state);
void Tusin_StageFadeOut(TusinState* state);
void Tusin_Init(TusinState* state);
void Tusin_Update(TusinState* state);
void Tusin_Destroy(TusinState* state);
void Tusin_RegisterVBlank(void);
void Tusin_DeregisterVBlank(void);

static const char* Tusin_SequenceName     = "Seq_Tusin()";
static char        Tusin_FriendDataName[] = "GlobalFriendData";

static const OverlayProcess OvlProc_Tusin = {
    .init = (OverlayCB)Tusin_Init,
    .main = (OverlayCB)Tusin_Update,
    .exit = (OverlayCB)Tusin_Destroy,
};

// clang-format off
const BinIdentifier Tusin_BinIdentifiers[16] = {
    [0]  = {0x2D, "Apl_Tak/Grp_Tusin_BGD00.bin"},
    [1]  = {0x2D, "Apl_Tak/Grp_Menu_fontSCR.bin"},
    [2]  = {0x2D, "Apl_Tak/Grp_Tusin_OBD00.bin"},
    [3]  = {0x2D, "Apl_Tak/Grp_Menu_BGU.bin"},
    [4]  = {0x2D, "Apl_Tak/Grp_Menu_fontSCR.bin"},
    [5]  = {0x2D, "Apl_Tak/Grp_Tusin_OBU00.bin"},
    [6]  = {0x2D, "Apl_Tak/Grp_Tusin_OBU01.bin"},
    [7]  = {0x2D, "Apl_Tak/Grp_Badge.bin"},
    [8]  = {0x2D, "Apl_Tak/Grp_Item.bin"},
    [9]  = {0x2D, "Apl_Tak/Grp_DummyBadge.bin"},
    [10] = {0x2D, "Apl_Tak/Grp_DummyItem.bin"},
    [11] = {0x2D, "Data/BadgeData.bin"},
    [12] = {0x2D, "Apl_Tak/ItemData.bin"},
    [13] = {0x2D, "Apl_Tak/FoodData.bin"},
    [14] = {0x2D, "Apl_Tak/TreasureData.bin"},
    [15] = {0x2D, "Apl_Fuk/Grp_OtosuMenuObj.bin"},
};
// clang-format on

TusinState* Tusin_State;

static inline BOOL Tusin_IsMacByteEqual(u8* macAddress, s32 index, u8 value) {
    return macAddress[index] == value;
}

BOOL Tusin_IsPlayerSeen(TusinState* state, u8* macAddress) {
    u16 i;

    for (i = 0; i < 65; i++) {
        if (state->seenPlayers[i].used == TRUE && Tusin_IsMacByteEqual(macAddress, 0, state->seenPlayers[i].macAddress[0]) &&
            Tusin_IsMacByteEqual(macAddress, 1, state->seenPlayers[i].macAddress[1]) &&
            Tusin_IsMacByteEqual(macAddress, 2, state->seenPlayers[i].macAddress[2]) &&
            Tusin_IsMacByteEqual(macAddress, 3, state->seenPlayers[i].macAddress[3]) &&
            Tusin_IsMacByteEqual(macAddress, 4, state->seenPlayers[i].macAddress[4]) &&
            Tusin_IsMacByteEqual(macAddress, 5, state->seenPlayers[i].macAddress[5]))
        {
            return TRUE;
        }
    }
    return FALSE;
}

s16 Tusin_FindFreeSeenSlot(TusinState* state) {
    s16 i;

    for (i = 0; i < 65; i++) {
        if (state->seenPlayers[i].used == FALSE) {
            return i;
        }
    }
    return -1;
}

u16 Tusin_OnScan(TusinState* state, TusinScanResult* scan, u16 skipMask) {
    TusinObject* tusin     = &state->tusin;
    u16          knownMask = 0;
    u16          i;

#ifdef REGION_USA
    GX_GetVCount(); // Leftover profiling
#endif
    for (i = 0; i < scan->count; i++) {
        if (Tusin_IsPlayerSeen(state, scan->beacons[i]->macAddress) == TRUE) {
            knownMask |= 1 << i;
        } else {
#ifdef REGION_USA
            s16 slot = Tusin_FindFreeSeenSlot(state);
#else
            u16 slot = Tusin_FindFreeSeenSlot(state);
#endif

            if (slot != -1 && !(skipMask & (1 << i))) {
                TusinBeacon* beacon = scan->beacons[i];

                state->seenPlayers[slot].macAddress[0] = beacon->macAddress[0];
                state->seenPlayers[slot].macAddress[1] = beacon->macAddress[1];
                state->seenPlayers[slot].macAddress[2] = beacon->macAddress[2];
                state->seenPlayers[slot].macAddress[3] = beacon->macAddress[3];
                state->seenPlayers[slot].macAddress[4] = beacon->macAddress[4];
                state->seenPlayers[slot].macAddress[5] = beacon->macAddress[5];
                state->seenPlayers[slot].used          = TRUE;
                tusin->civviesMet++;
                tusin->totalMet++;
                if (tusin->mingleRemaining != 0) {
                    tusin->mingleRemaining--;
                }
                state->recvKinds[state->recvCount] = 1;
                state->recvCount++;
            }
        }
    }
#ifdef REGION_USA
    GX_GetVCount(); // Leftover profiling
#endif
    return knownMask;
}

void Tusin_OnReceive(TusinState* state, s32 unused, TusinPacket* packet, u32 size) {
    TusinObject* tusin = &state->tusin;
#ifdef REGION_USA
    s16 slot;
#else
    u16 slot;
#endif

#ifdef REGION_USA
    GX_GetVCount(); // Leftover profiling
#endif
    if (state->recvCount >= 10) {
        return;
    }

    MI_CpuCopyU8(packet, &state->recvPackets[state->recvCount], size);
    state->recvKinds[state->recvCount] = 0;
    state->recvCount++;
    tusin->espersMet++;
    tusin->totalMet++;
    if (tusin->mingleRemaining != 0) {
        tusin->mingleRemaining--;
    }

    slot = Tusin_FindFreeSeenSlot(state);
    if (slot != -1) {
        state->seenPlayers[slot].macAddress[0] = packet->macAddress[0];
        state->seenPlayers[slot].macAddress[1] = packet->macAddress[1];
        state->seenPlayers[slot].macAddress[2] = packet->macAddress[2];
        state->seenPlayers[slot].macAddress[3] = packet->macAddress[3];
        state->seenPlayers[slot].macAddress[4] = packet->macAddress[4];
        state->seenPlayers[slot].macAddress[5] = packet->macAddress[5];
        state->seenPlayers[slot].used          = TRUE;
#ifndef REGION_USA
        state->seenPlayers[slot].expireTime = state->seenDuration * 60;
#endif
    }
#ifdef REGION_USA
    GX_GetVCount(); // Leftover profiling
#endif
}

#ifndef REGION_USA
s16 func_ov044_020831f0(u8* macAddress) {
    u8 emptyMac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    return (macAddress[0] == emptyMac[0] && macAddress[1] == emptyMac[1] && macAddress[2] == emptyMac[2] &&
            macAddress[3] == emptyMac[3] && macAddress[4] == emptyMac[4] && macAddress[5] == emptyMac[5])
               ? TRUE
               : FALSE;
}
#endif

void Tusin_InitState(TusinState* state) {
    TusinObject* tusin = &state->tusin;
    s32          i;
#ifndef REGION_USA
    u16 count = 0;
    s64 now;

    func_020415a4();
#endif

    state->exitReady       = 0;
    state->timer           = 0;
    state->unk_2402E       = 0;
    state->shownCount      = 0;
    state->recvCount       = 0;
    state->commBusy        = 0;
    tusin->flags           = 0;
    tusin->unk_46C         = 0;
    tusin->nextProcess     = 0;
    tusin->unk_468         = 6;
    tusin->btnPressed      = 0;
    tusin->btnFState       = 0;
    tusin->btnLRPressed[0] = 0;
    tusin->btnLRPressed[1] = 0;
    tusin->btnLRTimer      = 0;
    tusin->civviesMet      = 0;
    tusin->espersMet       = 0;
    tusin->aliensMet       = 0;
    tusin->totalMet        = 0;
    tusin->mingleRemaining = 10;
    tusin->flags |= 4;
    tusin->lastRakedMoney  = 0;
    tusin->giftItemId      = 0xFFFF;
    tusin->alienKind       = 0;
    tusin->alienTimer      = 3600;
    state->seenDuration    = 3600;
    tusin->totalRakedMoney = 0;
    for (i = 0; i < 50; i++) {
        tusin->unk_490[i] = 0;
        tusin->unk_4C2[i] = 0;
    }

    Tusin_InitProfile(tusin);
    Comm_Init(state->comm, Tusin_OnScan, state, Tusin_OnReceive, state, &state->sendPacket, sizeof(TusinPacket));
#ifndef REGION_USA
    now = func_ov044_02085060();
#endif
    MI_CpuCopyU8(tusin->macAddress, state->sendPacket.macAddress, sizeof(tusin->macAddress));
    MI_CpuCopyU8(tusin->nickName, state->sendPacket.nickName, sizeof(tusin->nickName));
    MI_CpuCopyU8(tusin->message, state->sendPacket.message, sizeof(tusin->message));
    MI_CpuCopyU8(&tusin->profile, state->sendPacket.profile, sizeof(tusin->profile));

    for (i = 0; i < 65; i++) {
        MI_CpuSet(state->seenPlayers[i].macAddress, 0xFF, sizeof(state->seenPlayers[i].macAddress));
#ifndef REGION_USA
        state->seenPlayers[i].expireTime = 0;
#endif
        state->seenPlayers[i].used = FALSE;
    }

#ifndef REGION_USA
    // Friends met within the last seenDuration seconds do not count again yet.
    for (i = 0; i < 50; i++) {
        if (func_ov044_020831f0(state->friendData->unk_0000[i].unk_00) != TRUE) {
            s64 elapsed = func_ov044_02085104(now, func_ov044_02085088(&state->friendData->unk_0000[i].lastMet));

            if (elapsed < state->seenDuration) {
                state->seenPlayers[count].expireTime = (state->seenDuration - elapsed) * 60;
                MI_CpuCopyU8(state->friendData->unk_0000[i].unk_00, state->seenPlayers[count].macAddress, 6);
                state->seenPlayers[count].used = TRUE;
                count++;
            }
        }
    }
#endif
    MI_CpuSet(state->recvPackets, 0, sizeof(TusinPacket));
    for (i = 0; i < 10; i++) {
        state->recvKinds[i] = 0;
    }
}

void Tusin_CreateTasks(TusinState* state) {
    TusinObject* tusin = &state->tusin;

    state->taskId_Btn     = Tusin_btn_CreateTask(&state->base.taskPool, state->base.dataType, tusin);
    state->taskId_BtnF    = Tusin_btnF_CreateTask(&state->base.taskPool, state->base.dataType, tusin);
    state->taskId_BtnLR   = Tusin_btnLR_CreateTask(&state->base.taskPool, state->base.dataType, tusin);
    state->taskId_Num     = Tusin_num_CreateTask(&state->base.taskPool, state->base.dataType, tusin);
    state->taskId_TextScr = Tusin_textScr_CreateTask(&state->base.taskPool, state->base.dataType, tusin);
}

void Tusin_ShiftFriends(TusinState* state, s32 count) {
    TusinObject* tusin = &state->tusin;

    for (u16 i = count; i != 0; i--) {
        state->friendData->unk_0000[i]          = state->friendData->unk_0000[i - 1];
        state->friendData->unk_1DB0[i].unk_0[0] = state->friendData->unk_1DB0[i - 1].unk_0[0];
        gSaveData.mingleFriends[i]              = gSaveData.mingleFriends[i - 1];
        tusin->unk_4C2[i]                       = tusin->unk_4C2[i - 1];
        tusin->unk_490[i]                       = tusin->unk_490[i - 1];
    }
}

s16 Tusin_FindFriend(TusinState* state) {
    UnkLargeFriendStruct* friends = state->friendData->unk_0000;
    s16                   i;

    for (i = 0; i < 50; i++) {
        u8* mac = state->recvPackets[state->shownCount].macAddress;

        if (mac[0] == friends->unk_00[0] && mac[1] == friends->unk_00[1] && mac[2] == friends->unk_00[2] &&
            mac[3] == friends->unk_00[3] && mac[4] == friends->unk_00[4] && mac[5] == friends->unk_00[5])
        {
            return i;
        }
        friends++;
    }
    return -1;
}

void Tusin_StoreFriend(TusinState* state, u16 index, u8 timesMet, u8 lastShopId) {
    TusinObject* tusin = &state->tusin;
    TusinProfile profile;
    RTCDate      date;
    RTCTime      time;
    u16          i;

    func_020417e0(&date, &time);
    MI_CpuCopyU8(state->recvPackets[state->shownCount].macAddress, state->friendData->unk_0000[index].unk_00, 6);
    MI_CpuCopyU8(state->recvPackets[state->shownCount].nickName, state->friendData->unk_0000[index].nickName, 0x16);
    MI_CpuCopyU8(state->recvPackets[state->shownCount].message, state->friendData->unk_0000[index].message, 0x36);
    MI_CpuCopyU8(state->recvPackets[state->shownCount].profile, &profile, sizeof(TusinProfile));

    timesMet++;
    if (timesMet > 99) {
        timesMet = 99;
    }
    state->friendData->unk_0000[index].timesMet       = timesMet;
    state->friendData->unk_0000[index].lastMet.year   = date.year;
    state->friendData->unk_0000[index].lastMet.month  = date.month;
    state->friendData->unk_0000[index].lastMet.day    = date.day;
    state->friendData->unk_0000[index].lastMet.hour   = time.hour;
    state->friendData->unk_0000[index].lastMet.minute = time.minute;
    state->friendData->unk_0000[index].lastMet.second = time.second;
    state->friendData->unk_0000[index].experience     = profile.experience;
    state->friendData->unk_0000[index].shop           = profile.shop;
    MI_CpuCopyU8(state->friendData->unk_0000[index].unk_00, gSaveData.mingleFriends[index].macAddress, 6);

    for (i = 0; i < 50; i++) {
        BOOL match = tusin->macAddress[0] == profile.friends[i].macAddress[0] &&
                     tusin->macAddress[1] == profile.friends[i].macAddress[1] &&
                     tusin->macAddress[2] == profile.friends[i].macAddress[2] &&
                     tusin->macAddress[3] == profile.friends[i].macAddress[3] &&
                     tusin->macAddress[4] == profile.friends[i].macAddress[4] &&
                     tusin->macAddress[5] == profile.friends[i].macAddress[5];

        if (match == TRUE) {
            u32 money  = profile.friends[i].unk_8 & ~0xFF000000;
            u32 shopId = (profile.friends[i].unk_8 & 0xFF000000) >> 24;

            if (money != 0 && shopId != lastShopId) {
                tusin->lastRakedMoney = money >> 1;
                tusin->totalRakedMoney += tusin->lastRakedMoney;
                if (tusin->totalRakedMoney > 9999999) {
                    tusin->totalRakedMoney = 9999999;
                }
                state->friendData->unk_1DB0[index].unk_0[0] = shopId;
            } else {
                tusin->lastRakedMoney = 0;
            }
        }
    }

    if (gSaveData.mingleFriends[index].unk_8 & ~0xFF000000) {
        if (tusin->unk_4C2[index] == 0) {
            tusin->unk_4C2[index] = 1;
        }
    }
    tusin->giftItemId = profile.shop.giftItemId;
}

void Tusin_ProcessEsper(TusinState* state) {
    TusinObject* tusin = &state->tusin;
    s16          index;

    tusin->lastRakedMoney = 0;
    index                 = Tusin_FindFriend(state);
    if (index != -1) {
        u8 timesMet   = state->friendData->unk_0000[index].timesMet;
        u8 lastShopId = state->friendData->unk_1DB0[index].unk_0[0];

        Tusin_ShiftFriends(state, index);
        Tusin_StoreFriend(state, 0, timesMet, lastShopId);
    } else {
        Tusin_ShiftFriends(state, 49);
        Tusin_StoreFriend(state, 0, 0, 0);
    }
}

void Tusin_StageFadeIn(TusinState* state) {
    TusinObject* tusin = &state->tusin;

    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (EasyFade_IsFading()) {
        return;
    }
    if (tusin->nextProcess == 3 || tusin->nextProcess == 5) {
        DebugOvlDisp_Pop();
    } else {
        DebugOvlDisp_ReplaceTop((OverlayCB)Tusin_StageStartComm, state, PROCESS_STAGE_INIT);
    }
}

void Tusin_StageStartComm(TusinState* state) {
    if (state->commBusy != 0) {
        return;
    }
    Comm_Start(state->comm);
    DebugOvlDisp_ReplaceTop((OverlayCB)Tusin_StageMain, state, PROCESS_STAGE_INIT);
}

// Nonmatching: two independent address computations are scheduled in the other order
void Tusin_StageMain(TusinState* state) {
    TusinObject* tusin = &state->tusin;
    TouchCoord   touch;
    s32          lidClosed;

    func_02041060(&lidClosed);
    if (tusin->btnPressed == 1 && lidClosed == 1) {
        if (!(tusin->flags & 0x20)) {
            Comm_Stop(state->comm);
            tusin->statusMsg = SYSMSG_MINGLE_POWER_LIGHT_RED;
            tusin->flags |= 0x21;
        }
    }

    if (tusin->mingleRemaining == 0 && !(tusin->flags & 0x20)) {
        Comm_Stop(state->comm);
        tusin->statusMsg = SYSMSG_MINGLE_QUOTA_MET;
        tusin->flags |= 0x21;
    }

    if (state->shownCount < state->recvCount && (tusin->flags & 4)) {
        tusin->flags &= ~4;
        tusin->flags |= 3;
        MI_CpuCopyU8(state->recvPackets[state->shownCount].nickName, tusin->friendName, sizeof(tusin->friendName));
        if (state->recvKinds[state->shownCount] == 0) {
            Tusin_ProcessEsper(state);
        } else if (state->recvKinds[state->shownCount] == 1) {
            tusin->giftItemId = Tusin_GetRandomCivvyGift();
        } else {
            tusin->alienKind  = RNG_Next(0xFFFF) % 30;
            tusin->giftItemId = Tusin_GetAlienGift(tusin->alienKind);
        }
#ifdef REGION_USA
        state->taskId_BeltU[state->shownCount] = Tusin_beltU_CreateTask(
            &state->base.taskPool, state->base.dataType, state->recvKinds[state->shownCount], tusin, state->shownCount);
#else
        state->taskId_BeltU[state->shownCount] =
            Tusin_beltU_CreateTask(&state->base.taskPool, state->base.dataType, state->recvKinds[state->shownCount], tusin);
#endif
        state->shownCount++;
    }

    state->commBusy = Comm_IsBusy(state->comm);

#ifndef REGION_USA
    for (u16 i = 0; i < 65; i++) {
        if (state->seenPlayers[i].used == TRUE && state->seenPlayers[i].expireTime > 0) {
            state->seenPlayers[i].expireTime--;
            if (state->seenPlayers[i].expireTime == 0) {
                MI_CpuSet(state->seenPlayers[i].macAddress, 0xFF, sizeof(state->seenPlayers[i].macAddress));
                state->seenPlayers[i].used = FALSE;
            }
        }
    }
#endif

    if (tusin->alienTimer > 0) {
        tusin->alienTimer--;
    } else {
        s32 odds;

        tusin->alienTimer = 3600;
        if (tusin->totalMet == 0) {
            odds = 60;
        } else {
            odds = 300;
        }
        if (tusin->mingleRemaining != 0 && RNG_Next(0xFFFF) % odds == 0) {
            state->recvKinds[state->recvCount] = 2;
            state->recvCount++;
            tusin->aliensMet++;
            tusin->totalMet++;
            if (tusin->mingleRemaining != 0) {
                tusin->mingleRemaining--;
            }
        }
    }

    if (TouchInput_WasTouchPressed()) {
        s32 lr;

        TouchInput_GetCoord(&touch);
        if (Tusin_IsPointOnBtnF(touch.x, touch.y) == 1) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            tusin->btnFState = 1;
            if (!(tusin->flags & 0x20)) {
                Comm_Stop(state->comm);
                tusin->statusMsg = SYSMSG_MINGLE_ENDING;
                tusin->flags |= 0x21;
            }
            tusin->flags |= 0x1000;
        }

        if (Tusin_IsPointOnBtn(touch.x, touch.y) == 1) {
            if (tusin->btnPressed == 1 && lidClosed == 1) {
                return;
            }
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            tusin->btnPressed = 1 - tusin->btnPressed;
        }

        lr = Tusin_GetBtnLRAtPoint(touch.x, touch.y);
        if (lr != -1 && !(tusin->flags & 0x20)) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
            tusin->btnLRPressed[lr] = 1;
            tusin->btnLRTimer       = 12;
            if (lr == 0) {
                if (tusin->mingleRemaining > 1) {
                    tusin->mingleRemaining--;
                }
            } else {
                if (tusin->mingleRemaining < 10 - tusin->totalMet) {
                    tusin->mingleRemaining++;
                }
            }
            tusin->flags |= 1;
        }
    }

#ifdef REGION_USA
    if ((tusin->flags & 0x20) && state->commBusy == 0) {
        SystemStatusFlags;
        SystemStatusFlags.unk_06 = TRUE;
    }
#endif
    if (!(tusin->flags & 0x20)) {
        return;
    }
    if (!(tusin->flags & 0x1000)) {
        return;
    }
    if (state->commBusy != 0) {
        return;
    }
#ifndef REGION_USA
    SystemStatusFlags;
    SystemStatusFlags.unk_06 = TRUE;
#endif
    SystemStatusFlags;
    SystemStatusFlags.unk_05 = FALSE;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = FALSE;
    DebugOvlDisp_ReplaceTop((OverlayCB)Tusin_StageBeginSave, state, PROCESS_STAGE_INIT);
}

void Tusin_StageBeginSave(TusinState* state) {
    TusinObject* tusin = &state->tusin;

    tusin->statusMsg = SYSMSG_MINGLE_SAVING;
    tusin->flags |= 1;
    DebugOvlDisp_ReplaceTop((OverlayCB)Tusin_StageSave, state, PROCESS_STAGE_INIT);
}

void Tusin_StageSave(TusinState* state) {
    gSaveData.unk_1AB6 |= 1;
    Tusin_WriteLastProfile();
    Tusin_CommitResults(&state->tusin);
    Savefile_ResetIOPipeline();
    FriendData_Set(state->friendData);
    DebugOvlDisp_ReplaceTop((OverlayCB)Tusin_StageSaving, state, PROCESS_STAGE_INIT);
}

void Tusin_StageSaving(TusinState* state) {
    TusinObject* tusin = &state->tusin;
    s32          errorFlags;

    if (Savefile_RunAlternatePipelineStep() != 1) {
        return;
    }

    SystemStatusFlags;
    SystemStatusFlags.unk_06 = TRUE;
    tusin->btnFState         = 0;
    errorFlags               = Savefile_GetWriteErrorFlags();
    if (errorFlags == 0) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
        state->timer     = 120;
        tusin->statusMsg = SYSMSG_SAVE_COMPLETE;
        tusin->flags |= 1;
        gSaveData.unk_1AB4 |= 8;
        tusin->nextProcess = 2;
        DebugOvlDisp_ReplaceTop((OverlayCB)Tusin_StageSaveDone, state, PROCESS_STAGE_INIT);
    } else if (errorFlags & 2) {
        tusin->nextProcess = 3;
        DebugOvlDisp_Pop();
    } else if (errorFlags & 4) {
        tusin->nextProcess = 4;
        DebugOvlDisp_Pop();
    } else {
        tusin->nextProcess = 5;
        DebugOvlDisp_Pop();
    }
}

void Tusin_StageSaveDone(TusinState* state) {
    if (state->timer > 0) {
        state->timer--;
        return;
    }
    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
    SndMgr_StartPlayingSE(SEIDX_MENU_MEXIT);
    DebugOvlDisp_Pop();
}

void Tusin_StageFadeOut(TusinState* state) {
    TusinObject* tusin = &state->tusin;

    if (tusin->nextProcess == 3 || tusin->nextProcess == 4 || tusin->nextProcess == 5) {
        EasyFade_FadeBothDisplays(FADER_LINEAR, -16, 0x1000);
    } else {
        EasyFade_FadeBothDisplays(FADER_LINEAR, 16, 0x1000);
    }
    if (EasyFade_IsFading()) {
        return;
    }
    DebugOvlDisp_Pop();
}

void Tusin_Init(TusinState* state) {
    TusinObject*      tusin;
    GlobalFriendData* friendData;
    s32               result;

    if (state == NULL) {
        const char* sequence = Tusin_SequenceName;
        state                = Mem_AllocHeapTail(&gDebugHeap, sizeof(TusinState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        Tusin_State = state;
        MainOvlDisp_SetCbArg(state);
    }
    tusin                     = &state->tusin;
    state->base.spareDataType = DatMgr_AllocateSlot();
    state->base.dataType      = DatMgr_AllocateSlot();
    Tusin_RegisterVBlank();
    state->base.prevResMgr = ResourceMgr_ReinitManagers(&state->base.resMgr);
    TouchInput_Init();
    Mem_InitializeHeap(&state->base.heap, state->base.heapBuffer, sizeof(state->base.heapBuffer));
    FS_LoadOverlay(0, (u32)&OVERLAY_31_ID);
    OvlMgr_LoadOverlay(3, (u32)&OVERLAY_COMM);
    EasyTask_InitializePool(&state->base.taskPool, &state->base.heap, 0x200, NULL, NULL);
    data_02066aec = 0;
    data_02066eec = 0;
    EasyTask_CreateTask(&state->base.taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -16, 0x1000);

    friendData = Mem_AllocHeapTail(&gMainHeap, sizeof(GlobalFriendData));
    Mem_SetSequence(&gMainHeap, friendData, Tusin_FriendDataName);
    state->friendData = friendData;
    result            = Savefile_LoadFriendImage(friendData);
    if (result == 0) {
        Tusin_InitState(state);
        Tusin_LoadBackgrounds(tusin);
        Tusin_CreateTasks(state);
    } else if (result == 2) {
        tusin->nextProcess = 3;
    } else {
        tusin->nextProcess = 5;
    }

    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)Tusin_StageFadeOut, state, PROCESS_STAGE_INIT);
    DebugOvlDisp_Push((OverlayCB)Tusin_StageFadeIn, state, PROCESS_STAGE_INIT);
    MainOvlDisp_NextProcessStage();
    CriSndMgr_Stop(0);
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
}

void Tusin_Update(TusinState* state) {
    TusinObject* tusin = &state->tusin;

    TouchInput_Update();
#ifndef REGION_USA
    OamMgr_Reset3DState();
#endif
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
#ifndef REGION_USA
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
#endif
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    Tusin_UpdateBackgrounds(tusin);
    DebugOvlDisp_Run();
    EasyTask_UpdatePool(&state->base.taskPool);
    if (tusin->flags & 2) {
        tusin->flags &= ~2;
    }
    if (DebugOvlDisp_IsStackAtBase() == TRUE) {
        state->exitReady = 1;
    }
#ifndef REGION_USA
    OamMgr_Swap3DBuffers();
#endif
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
#ifndef REGION_USA
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_EXTENDED], NULL);
#endif

    if (state->exitReady == 0) {
        return;
    }

    switch (tusin->nextProcess) {
        case 0: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&TUSIN_OVL_MENU, TUSIN_OVL_MENU_ENTRY, NULL, PROCESS_STAGE_INIT);
        } break;
        case 1: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&TUSIN_OVL_SET, TUSIN_OVL_SET_ENTRY, NULL, PROCESS_STAGE_INIT);
        } break;
        case 2: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&TUSIN_OVL_RESULT, TUSIN_OVL_RESULT_ENTRY, NULL, PROCESS_STAGE_INIT);
        } break;
        case 3: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_2_ID, TUSIN_OVL2_ENTRY_3, NULL, PROCESS_STAGE_INIT);
        } break;
        case 4: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_2_ID, TUSIN_OVL2_ENTRY_4, NULL, PROCESS_STAGE_INIT);
        } break;
        case 5: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_2_ID, TUSIN_OVL2_ENTRY_5, NULL, PROCESS_STAGE_INIT);
        } break;
        default: {
            OverlayTag tag;
            MainOvlDisp_Pop(&tag);
        } break;
    }
}

void Tusin_Destroy(TusinState* state) {
    TusinObject* tusin = &state->tusin;

    Comm_Exit(state->comm);
    if (tusin->nextProcess != 3 && tusin->nextProcess != 5) {
        Tusin_ReleaseBackgrounds(tusin);
    }
    Mem_Free(&gMainHeap, state->friendData);
    EasyTask_DestroyPool(&state->base.taskPool);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->base.spareDataType);
    DatMgr_ClearSlot(state->base.dataType);
    Tusin_DeregisterVBlank();
    OvlMgr_UnloadOverlay(3);
    FS_UnloadOverlay(0, (u32)&OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
}

void ProcessOverlay_Tusin(TusinState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        Tusin_Destroy(state);
    } else {
        OvlProc_Tusin.funcs[stage](state);
    }
}

// Nonmatching: register allocation (target uses lr where this uses r4)
void Tusin_InitDisplay(void) {
    Interrupts_Init();
    HBlank_Init();
    do {
    } while (REG_VCOUNT < (s16)0xC0);
    GX_Init();
    func_0202b878();
    DMA_Init(0x100);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(GX_VRAM_ALL);
    GX_SetBankForTex(GX_VRAM_A);
    GX_SetBankForTexPltt(GX_VRAM_G);
    GX_SetBankForBg(GX_VRAM_E);
    GX_SetBankForObj(GX_VRAM_B);
    GX_SetBankForBgExtPltt(GX_VRAM_NONE);
    GX_SetBankForObjExtPltt(GX_VRAM_NONE);
    GX_SetBankForSubBg(GX_VRAM_C);
    GX_SetBankForSubObj(GX_VRAM_D);
    GX_SetBankForSubBgExtPltt(GX_VRAM_NONE);
    GX_SetBankForSubObjExtPltt(GX_VRAM_NONE);
    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    MI_CpuFill(0, (void*)0x06000000, 0x80000);
    MI_CpuFill(0, (void*)0x06200000, 0x20000);
    MI_CpuFill(0, (void*)0x06400000, 0x40000);
    MI_CpuFill(0, (void*)0x06600000, 0x20000);
    REG_POWER_CNT |= 0x8000;
    Display_CommitSynced();
    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 1, 0x4);

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode   = GX_OBJBMPMODE_1D_128K;
    Display_SetMainLayers(LAYER_BG0 | LAYER_BG3 | LAYER_OBJ);
    data_0206aa78 = 0x300010;
    data_0206aa7c = 0x400040;

    g_DisplaySettings.controls[DISPLAY_SUB].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 1, 0x4);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 3, 1, 0x10C);

    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    Display_SetSubLayers(LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);
    OamMgr_Init3DSpritePipeline();
    OamMgr_Swap3DBuffers();

    OamMgr_InitEngine(0, DISPLAY_MAIN);
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    DC_PurgeRange(g_OamMgr[DISPLAY_MAIN].oam, 0x400);
    GX_LoadOam(g_OamMgr[DISPLAY_MAIN].oam, 0, 0x400);
    OamMgr_InitEngine(0, DISPLAY_SUB);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    DC_PurgeRange(g_OamMgr[DISPLAY_SUB].oam, 0x400);
    GXs_LoadOam(g_OamMgr[DISPLAY_SUB].oam, 0, 0x400);
#ifndef REGION_USA
    OamMgr_InitEngine(0, DISPLAY_EXTENDED);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
#endif
}

void Tusin_VBlank(void) {
    if (SystemStatusFlags.vblank) {
        Comm_VBlank();
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(g_OamMgr[DISPLAY_MAIN].oam, 0x400);
        GX_LoadOam(g_OamMgr[DISPLAY_MAIN].oam, 0, 0x400);
        DC_PurgeRange(g_OamMgr[DISPLAY_SUB].oam, 0x400);
        GXs_LoadOam(g_OamMgr[DISPLAY_SUB].oam, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
#ifndef REGION_USA
        func_02001b44(2, 0, &data_020672ec, 0x400);
#endif
    }
}

void Tusin_RegisterVBlank(void) {
    Tusin_InitDisplay();
    Interrupts_RegisterVBlankCallback(Tusin_VBlank, 1);
}

void Tusin_DeregisterVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}
