#include "Otosu/OtosuMenu.h"

typedef union {
    struct {
        u16 unk0;
        u16 unk2;
        u16 unk4;
        u16 unk6;
        u16 unk8;
        u16 unkA;
    };
    u16 data[6];
} Ov002_U16_6;

typedef struct {
    u16 data[15];
} Ov002_U16_15;

typedef union {
    struct {
        u16 unk0;
        u16 unk2;
        u16 unk4;
        u16 unk6;
        u16 unk8;
        u16 unkA;
        u16 unkC;
        u16 unkE;
        u16 unk10;
        u16 unk12;
        u16 unk14;
        u16 unk16;
    };
    u16 data[12];
} Ov002_U16_12;

static void          OtosuMenu_Lobby_Load(PrcCtx* ctx, OtosuMenuObj* menuObj);
static void          OtosuMenu_Lobby_Destroy(void* arg0, OtosuMenuObj* menuObj);
static void          OtosuMenu_Lobby_Update(void);
static void          OtosuMenu_Lobby_Render(void);
static PrcStepResult OtosuMenu_Lobby_Step_Begin(PrcCtx* ctx, void* unused);
static PrcStepResult OtosuMenu_Lobby_Step_InitHost(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_StartHost(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_DrawPlayerNames(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_SetupHostScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_HostWait(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_GoToHostStageSelect(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_GoToGuestStageSelect(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_StartScan(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_SetupJoinScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_ChooseHost(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_EndScan(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_SetupGuestScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_ConnectToHost(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_InitGuestWait(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_GuestWait(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Lobby_Step_ShutdownWireless(PrcCtx* ctx, void* unused);
static PrcStepResult OtosuMenu_Lobby_Step_LeaveSession(PrcCtx* ctx, void* unused);
static PrcStepResult OtosuMenu_Lobby_Step_SendCancel(PrcCtx* ctx, void* arg1);
static PrcStepResult OtosuMenu_Lobby_Step_SendStart(PrcCtx* ctx, void* arg1);
static PrcStepResult OtosuMenu_Lobby_Step_ShutdownHost(PrcCtx* ctx, void* arg0);
static PrcStepResult OtosuMenu_Lobby_Step_StopHost(PrcCtx* ctx, void* unused);

static PrcStepFn OtosuMenu_Lobby_ShutdownSteps[] = {OtosuMenu_Lobby_Step_ShutdownWireless, PrcStep_PopFrame};

static PrcStepFn OtosuMenu_Lobby_JoinStepTable[] = {
    OtosuMenu_Lobby_Step_Begin,
    OtosuMenu_Lobby_Step_StartScan,
    OtosuMenu_Lobby_Step_SetupJoinScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_Lobby_Step_ChooseHost,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_Lobby_Step_EndScan,
    OtosuMenu_Lobby_Step_SetupGuestScreen,
    OtosuMenu_Lobby_Step_ConnectToHost,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_Lobby_Step_InitGuestWait,
    OtosuMenu_Lobby_Step_GuestWait,
    OtosuPrcStep_SaveOwnPlayer,
    OtosuPrcStep_SavePlayers,
    OtosuMenu_Lobby_Step_LeaveSession,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_Lobby_Step_GoToGuestStageSelect,
    PrcStep_PopFrame,
};

static PrcStepFn OtosuMenu_Lobby_CancelSteps[] = {OtosuMenu_Lobby_Step_SendCancel, OtosuMenu_Lobby_Step_ShutdownHost,
                                                  PrcStep_PopFrame};

PrcFrameDesc OtosuMenu_Lobby_JoinFrameDesc = {
    .enter     = OtosuMenu_Lobby_Load,
    .stepTable = OtosuMenu_Lobby_JoinStepTable,
    .update    = OtosuMenu_Lobby_Update,
    .render    = OtosuMenu_Lobby_Render,
    .exit      = OtosuMenu_Lobby_Destroy,
};

static PrcStepFn OtosuMenu_Lobby_HostStepTable[] = {
    OtosuMenu_Lobby_Step_Begin,
    OtosuMenu_Lobby_Step_InitHost,
    OtosuMenu_Lobby_Step_StartHost,
    OtosuMenu_Lobby_Step_SetupHostScreen,
    OtosuMenu_Lobby_Step_DrawPlayerNames,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_Lobby_Step_HostWait,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuPrcStep_SaveOwnPlayer,
    OtosuPrcStep_SavePlayers,
    OtosuMenu_Lobby_Step_SendStart,
    OtosuMenu_Lobby_Step_StopHost,
    OtosuMenu_Lobby_Step_GoToHostStageSelect,
    PrcStep_PopFrame,
};

static const Ov002_U16_6 OtosuMenu_Lobby_HostCursors = {0x0002, 0x0002, 0x009D, 0x0002, 0x00A4, 0x009D}; /* const */
BOOL                     OtosuMenu_TitleSkipped;
static s16               OtosuMenu_Lobby_HostNameBuf[0xE];
static const Ov002_U16_6 OtosuMenu_Lobby_GuestCursors  = {0x0002, 0x0002, 0x009D, 0x0002, 0x002A, 0x0054}; /* const */
PrcFrameDesc             OtosuMenu_Lobby_HostFrameDesc = {
                .enter     = OtosuMenu_Lobby_Load,
                .stepTable = OtosuMenu_Lobby_HostStepTable,
                .update    = OtosuMenu_Lobby_Update,
                .render    = OtosuMenu_Lobby_Render,
                .exit      = OtosuMenu_Lobby_Destroy,
};
static const OtosuMenuRectList2 OtosuMenu_Lobby_GuestButtons = {
    {
     {0x0000, 0x0008, 0x00A0, 0x0058, 0x00B8},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     }
};
static const OtosuMenuRectList5 OtosuMenu_Lobby_JoinButtons = {
    {
     {0, 8, 0xA0, 0x58, 0xB8},
     {1, 0x48, 0x40, 0xB6, 0x58},
     {2, 0x48, 0x60, 0xB6, 0x78},
     {3, 0x48, 0x80, 0xB6, 0x98},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     }
};
static const Ov002_U16_12       OtosuMenu_Lobby_JoinCursors = {0x0002, 0x0002, 0x009D, 0x0004, 0x0040, 0x003E,
                                                               0x0004, 0x0040, 0x005E, 0x0004, 0x0040, 0x007E}; /* const */
static const OtosuMenuRectList3 OtosuMenu_Lobby_JoinTexts   = {
    {
     {0x23F4, 8, 8, 0xF8, 0x40},
     {0x23EB, 0x18, 0xA6, 0x58, 0xB6},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList3 OtosuMenu_Lobby_HostButtons = {
    {
     {0, 8, 0xA0, 0x58, 0xB8},
     {1, 0xA5, 0xA0, 0xF5, 0xB8},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     }
};
static const OtosuMenuRectList4 OtosuMenu_Lobby_HostTexts = {
    {
     {0x23F6, 0x0008, 0x0008, 0x00F8, 0x0040},
     {0x23F7, 0x00C5, 0x00A6, 0x00F5, 0x00B6},
     {0x23EB, 0x0018, 0x00A6, 0x0058, 0x00B6},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList3 OtosuMenu_Lobby_GuestTexts = {
    {
     {0x23F5, 8, 8, 0xF8, 0x40},
     {0x23EB, 0x18, 0xA6, 0x58, 0xB6},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};

static void OtosuMenu_Lobby_Load(PrcCtx* ctx, OtosuMenuObj* menuObj) {
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
}

static void OtosuMenu_Lobby_ClearWindow(void) {
    g_DisplaySettings.engineState[0].window0              = 31;
    g_DisplaySettings.engineState[0].window0Effects       = TRUE;
    g_DisplaySettings.engineState[0].windowOutside        = 31;
    g_DisplaySettings.engineState[0].windowOutsideEffects = TRUE;
    Display_Commit();
}

static void OtosuMenu_Lobby_Destroy(void* ctx, OtosuMenuObj* menuObj) {
    OtosuMenu_ReleaseScreens(menuObj);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
}

static void OtosuMenu_Lobby_Update(void) {}

static void OtosuMenu_Lobby_Render(void) {}

static PrcStepResult OtosuMenu_Lobby_Step_Begin(PrcCtx* ctx, void* unused) {
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static void OtosuMenu_Lobby_OnSendDone(void* arg, void* object) {
    ((OtosuMenuObj*)object)->sendBusy = 0;
}

static s32 OtosuMenu_Lobby_RejectGuest(void) {
    return 0;
}

static s32 OtosuMenu_Lobby_OnGuestConnected(OtosuMenuChildInfo* guest, OtosuMenuObj* menuObj) {
    u16 aid;

    aid = guest->aid;
    if ((u32)aid >= 4U) {
        return 0;
    }
    MI_CpuCopyU8(guest->name, menuObj->players[aid].name, 0x16);
    MI_CpuCopyU8(guest->bssid, menuObj->players[guest->aid].bssid, 6);

    menuObj->playerMask      = (u16)(menuObj->playerMask | (1 << guest->aid));
    menuObj->playerDirtyMask = (u16)(menuObj->playerDirtyMask | (1 << guest->aid));

    menuObj->guestAckMask[guest->aid] = 0;
    return 1;
}

static void OtosuMenu_Lobby_OnGuestDisconnected(OtosuMenuChildInfo* guest, OtosuMenuObj* menuObj) {
    u16 aid;

    aid = guest->aid;
    if ((u32)aid >= 4U) {
        return;
    }
    menuObj->playerMask      = (u16)(menuObj->playerMask & ~(1 << aid));
    menuObj->playerDirtyMask = (u16)(menuObj->playerDirtyMask | (1 << guest->aid));
}

static void OtosuMenu_Lobby_OnBeaconUpdated(void* arg, OtosuMenuObj* menuObj) {
    menuObj->beaconUpdateBusy = 0;
}

static void OtosuMenu_Lobby_OnGuestPacket(s32 guest, void* data, void* size, OtosuMenuObj* menuObj) {
    OtosuMenuGuestPacket* packet = data;

    if (data == NULL) {
        return;
    }
    if (size != (void*)0xA) {
        return;
    }
    if (menuObj == NULL) {
        return;
    }
    menuObj->guestAckMask[guest]    = packet->playerMask;
    menuObj->guestAckVersion[guest] = packet->rosterVersion;
    MI_CpuCopyU8(packet->bssid, menuObj->connectedBssids[guest], 6);
}

static PrcStepResult OtosuMenu_Lobby_Step_InitHost(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    s32 ownerName;

    menuObj->playerMask          = 0;
    menuObj->playerDirtyMask     = 0;
    menuObj->beaconUpdateBusy    = 0;
    menuObj->beaconUpdatePending = 0;
    menuObj->acceptingGuests     = 1;
    menuObj->sendBusy            = 0;
    menuObj->rosterChanged       = 0;
    menuObj->rosterVersion       = 0;
    MI_CpuSet(menuObj->guestAckMask, 0, 8);
    MI_CpuSet(menuObj->guestAckVersion, 0, 8);
    menuObj->syncState          = 0;
    menuObj->beacon.playerCount = 1;
    menuObj->playerMask         = 1;
    ownerName                   = SysFont_GetOwnerName();
    MI_CpuCopyU8(ownerName, menuObj->beacon.name, 0x16);
    MI_CpuCopyU8(ownerName, menuObj->players[0].name, 0x16);
    Mem_Free(&gDebugHeap, ownerName);
    func_0203a96c(menuObj->players[0].bssid);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: instruction scheduling
static PrcStepResult OtosuMenu_Lobby_Step_StartHost(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    s32 temp_r4;
    s32 temp_r5;
    u32 state;

    state = func_ov040_0209cb78();
    switch (state) {
        default:
            break;
        case 0:
            func_ov040_0209cf20();
            func_ov040_0209caac(0x400548);
            break;
        case 1:
            func_ov040_0209cb9c();
            break;
        case 7:
            temp_r5 = func_ov040_0209cde4();
            temp_r4 = func_020442f8();
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(0);
            func_ov040_0209cb08(&menuObj->beacon, 0x1E);
            func_ov040_0209d40c(OtosuMenu_Lobby_OnGuestConnected, menuObj);
            func_ov040_0209d420(OtosuMenu_Lobby_OnGuestDisconnected, menuObj);
            func_ov040_0209d434(OtosuMenu_Lobby_OnGuestPacket, menuObj);
            func_ov040_0209d0a8(0, temp_r4, temp_r5);
            break;
        case 4:
            PrcCtx_AdvanceStep(ctx);
            break;
    }
    return PRC_STEP_CONTINUE;
}

static void OtosuMenu_Lobby_SetStartEnabled(OtosuMenuObj* menuObj, s32 enabled) {
    u8* pack;
    u8* screen;
    u8* screen2;

    pack = (u8*)menuObj->packs[0];
    if (enabled != 0) {
        screen                                                = Data_GetPackEntryData((Data*)pack, 23);
        menuObj->mainScreens[1][0]                            = (void*)(screen + 4);
        g_DisplaySettings.engineState[0].window0              = 0x1F;
        g_DisplaySettings.engineState[0].window0Effects       = 1;
        g_DisplaySettings.engineState[0].windowOutside        = 0x1F;
        g_DisplaySettings.engineState[0].windowOutsideEffects = 1;
    } else {
        screen2                                               = Data_GetPackEntryData((Data*)pack, 24);
        menuObj->mainScreens[1][0]                            = (void*)(screen2 + 4);
        g_DisplaySettings.engineState[0].window0Left          = 688128;
        g_DisplaySettings.engineState[0].window0Top           = 655360;
        g_DisplaySettings.engineState[0].window0Right         = 1015808;
        g_DisplaySettings.engineState[0].window0Bottom        = 753664;
        g_DisplaySettings.engineState[0].window0              = 0x1E;
        g_DisplaySettings.engineState[0].window0Effects       = 1;
        g_DisplaySettings.engineState[0].windowOutside        = 0x1F;
        g_DisplaySettings.engineState[0].windowOutsideEffects = 1;
        g_DisplaySettings.controls[0].windows |= 1;
    }
    Display_Commit();
    func_0200d1d8(&menuObj->mainMaps[1], 0, 1, 0, menuObj->mainScreens[1], 1, 1);
}

static PrcStepResult OtosuMenu_Lobby_Step_DrawPlayerNames(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    OtosuMenu_DrawPlayerNames(menuObj);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Lobby_Step_SetupHostScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    OtosuMenuRectList4 texts = OtosuMenu_Lobby_HostTexts;

    OtosuMenu_SetupScreens(menuObj, 0x18, 0x16, texts.rects);
    OtosuMenu_Lobby_SetStartEnabled(menuObj, 0);
    menuObj->unk_474C8 = 0xFFFF;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: the names cursor and the roster mask swap registers
static void OtosuMenu_Lobby_RefreshRoster(OtosuMenuObj* menuObj) {
    u8  names[4][sizeof(OtosuMenuPlayer)];
    s32 i;
    u32 mask;
    u32 irq;
    u8* src;

    menuObj->beaconUpdatePending = 1;
    menuObj->rosterChanged       = 1;
    irq                          = OS_DisableIRQ();
    mask                         = menuObj->playerMask & menuObj->playerDirtyMask;
    menuObj->playerDirtyMask     = 0;
    src                          = (u8*)menuObj->players;
    for (i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            MI_CpuCopyU8(src, names[i], 0x16);
        }
        src += sizeof(OtosuMenuPlayer);
    }
    OS_RestoreIRQ(irq);
    OtosuMenu_DrawPlayerNames(menuObj);
}

static void OtosuMenu_Lobby_UpdateBeacon(OtosuMenuObj* menuObj) {
    s32 acceptNew;
    u16 playerCount;

    menuObj->beaconUpdatePending = 0;
    menuObj->beacon.playerCount  = func_02047e84(menuObj->playerMask);
    acceptNew                    = 1;
    menuObj->beaconUpdateBusy    = 1;
    if (menuObj->acceptingGuests != 0) {
        if ((u32)menuObj->beacon.playerCount >= 4U) {
            acceptNew = 0;
        }
    } else {
        acceptNew = 0;
    }
    if (func_ov040_0209d788(OtosuMenu_Lobby_OnBeaconUpdated, menuObj, &menuObj->beacon, 0x1E, acceptNew) == 0) {
        menuObj->beaconUpdateBusy = 0;
    }
    playerCount = menuObj->beacon.playerCount;
    OtosuMenu_Lobby_SetStartEnabled(menuObj, (playerCount > 1U) ? 1 : 0);
}

// Nonmatching: control flow differs
static PrcStepResult OtosuMenu_Lobby_Step_HostWait(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    OtosuMenuRectList3 buttons = OtosuMenu_Lobby_HostButtons;
    Ov002_U16_6        cursors = OtosuMenu_Lobby_HostCursors;
    s32                irq2;
    s32                irq;
    s32                i;
    s32                sendPacket;
    s32                j;
    s32                k;
    u16                mask2;
    u16                mask;
    u16                touched;
    void*              cursor;

    sendPacket = 0;
    touched    = OtosuMenu_GetTouchedButton(buttons.rects);
    if ((touched == 1) && (menuObj->beacon.playerCount == 1)) {
        touched = 0xFFFF;
    }
    if (touched == 0xFFFE) {
        touched              = 0xFFFF;
        menuObj->unk_474C8   = 0xFFFF;
        menuObj->cursor.posX = 0U;
        menuObj->cursor.posY = 0xC8U;
    }
    if ((menuObj->unk_474C8 == 1) && (menuObj->beacon.playerCount == 1)) {
        menuObj->unk_474C8   = 0xFFFF;
        menuObj->cursor.posX = 0U;
        menuObj->cursor.posY = 0xC8U;
    }
    if (touched != 0xFFFF) {
        if (menuObj->unk_474C8 == touched) {
            switch (touched) { /* irregular */
                case 0:
                    SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CANCEL);
                    PrcCtx_ReplaceFrame(ctx, &OtosuMenu_RoleSelect_FrameDesc, NULL);
                    menuObj->linkLevelIcon.posX = 0;
                    menuObj->linkLevelIcon.posY = 0xC8;
                    PrcCtx_PushStepTable(ctx, OtosuMenu_Lobby_CancelSteps);
                    PrcCtx_PushStepTable(ctx, PrcSteps_FadeBrightImmediate);
                    return 0;
                case 1:
                    irq = OS_DisableIRQ();
                    SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
                    if (((u32)func_02047e84(menuObj->playerMask) >= 2U) && (menuObj->sendBusy == 0)) {
                        i = 1;
                    loop_25:
                        if (i < 4) {
                            mask = menuObj->playerMask;
                            if (!(mask & (1 << i)) || ((mask == menuObj->guestAckMask[i]) &&
                                                       (menuObj->rosterVersion == menuObj->guestAckVersion[i])))
                            {
                                i += 1;
                                goto loop_25;
                            }
                        }
                        if (i == 4) {
                            func_ov040_0209d40c(OtosuMenu_Lobby_RejectGuest, menuObj);
                            func_ov040_0209d420(NULL, menuObj);
                            menuObj->syncState           = 2U;
                            menuObj->acceptingGuests     = 0;
                            sendPacket                   = 1;
                            menuObj->beaconUpdatePending = 1;
                        }
                    }
                    OS_RestoreIRQ(irq);
                    goto block_33;
            }
        } else {
            menuObj->unk_474C8 = touched;
            SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CURSOR);
            if ((touched != 0) && (touched != 1)) {

            } else {
                cursor               = &cursors.data[touched * 3];
                menuObj->cursor.posX = ((u16*)cursor)[1];
                menuObj->cursor.posY = ((u16*)cursor)[2];
                Sprite_ChangeAnimation(&menuObj->cursor, menuObj->cursor.animData, (s16)((u16*)cursor)[0],
                                       menuObj->cursor.cellTable);
            }
            goto block_33;
        }
    } else {
    block_33:
        if (menuObj->playerDirtyMask != 0) {
            OtosuMenu_Lobby_RefreshRoster(menuObj);
        }
        if ((menuObj->beaconUpdatePending != 0) && (menuObj->beaconUpdateBusy == 0)) {
            OtosuMenu_Lobby_UpdateBeacon(menuObj);
        }
        if ((menuObj->playerMask != 1) && (menuObj->sendBusy == 0)) {
            if (menuObj->rosterChanged != 0) {
                menuObj->rosterChanged = 0;
                sendPacket             = 1;
                menuObj->rosterVersion = (u16)(menuObj->rosterVersion + 1);
            } else {
                j = 1;
            loop_49:
                if (j < 4) {
                    mask2 = menuObj->playerMask;
                    if (mask2 & (1 << j)) {
                        if (mask2 != menuObj->guestAckMask[j]) {
                            sendPacket = 1;
                        } else if (menuObj->rosterVersion != menuObj->guestAckVersion[j]) {
                            sendPacket = 1;
                        } else {
                            goto block_48;
                        }
                    } else {
                    block_48:
                        j += 1;
                        goto loop_49;
                    }
                }
            }
        }
        if (sendPacket != 0) {
            irq2                           = OS_DisableIRQ();
            OtosuMenu_HostPacket.syncState = menuObj->syncState;
            if (menuObj->syncState == 0) {
                k                                  = 0;
                OtosuMenu_HostPacket.playerMask    = menuObj->playerMask;
                OtosuMenu_HostPacket.rosterVersion = menuObj->rosterVersion;
            loop_56:
                if (k < 4) {
                    if (menuObj->playerMask & (1 << k)) {
                        MI_CpuCopyU8(menuObj->players[k].name, OtosuMenu_HostPacket.names[k], 0x16);
                        MI_CpuCopyU8(menuObj->players[k].bssid, OtosuMenu_HostPacket.bssids[k], 6);
                    }
                    k += 1;
                    goto loop_56;
                }
            }
            OS_RestoreIRQ(irq2);
            menuObj->sendBusy = 1;
            if (func_ov040_0209d48c(&OtosuMenu_HostPacket, 0x78, OtosuMenu_Lobby_OnSendDone, menuObj) == 0) {
                menuObj->sendBusy = 0;
            }
        }
        if (menuObj->syncState == 2) {
            menuObj->matchPlayerCount = (s32)func_02047e84(menuObj->playerMask);
            PrcCtx_AdvanceStep(ctx);
        }
        return 0;
    }
}

static PrcStepResult OtosuMenu_Lobby_Step_GoToHostStageSelect(PrcCtx* ctx, void* object) {
    PrcCtx_ReplaceFrame(ctx, &OtosuMenu_StageSelect_HostFrameDesc, NULL);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Lobby_Step_GoToGuestStageSelect(PrcCtx* ctx, void* object) {
    PrcCtx_ReplaceFrame(ctx, &OtosuMenu_StageSelect_GuestFrameDesc, NULL);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: register allocation of the loop constants and the chars pointer
static void OtosuMenu_Lobby_DrawHostList(OtosuMenuObj* menuObj) {
    s32 dest;
    u16 j;
    u16 i;
    u32 offset;

    MI_CpuFillU16(0, (s32)menuObj->nameChars, 0x3000);
    MI_CpuFillU16(0, (s32)OtosuMenu_Lobby_HostNameBuf, 0x16);
    i = 0;
    do {
        if (!(menuObj->foundHostMask & (1 << i))) {
            *OtosuMenu_Lobby_HostNameBuf = 0xFFFF;
        } else {
            MI_CpuCopyU8(menuObj->foundHostBeacons[i].name, OtosuMenu_Lobby_HostNameBuf, 0x16);
        }
        SysFont_SetMsgPtr(&menuObj->font, OtosuMenu_Lobby_HostNameBuf);
        SysFont_SetPos(&menuObj->font, 0, 5);
        SysFont_SetHAlign(&menuObj->font, 0, 256);
        SysFont_SetVAlign(&menuObj->font, 3, 32);
        SysFont_DrawCurrentToChar(&menuObj->font, menuObj->nameChars + (i << 12), 32, 4);
        i += 1;
    } while ((u32)i < 3U);
    j = 0;
    do {
        offset = ((j << 5) + 0x48) << 8;
        dest   = G2_GetBG0CharPtr() + ((s32)(offset + (offset >> 0x1F)) >> 1);
        DC_PurgeAll();
        func_0203abec(3, (s32)menuObj->nameChars + (j << 0xC), dest, 0x1000);
        j += 1;
    } while ((u32)j < 3U);
}

// Nonmatching: instruction selection differs
static void OtosuMenu_Lobby_OnScanResult(WMBssDesc* bss, OtosuMenuObj* menuObj) {
    s32        slot;
    s32        i;
    u16        count;
    WMBssDesc* host;

    slot = 0;
    if (bss->gameInfoLength == 0) {
        return;
    }
    if (bss->gameInfo.userGameInfoLength != 0x70) {
        return;
    }
    for (i = 5; i >= 0; i--) {
        if (menuObj->foundHostMask & (1 << i)) {
            host = &menuObj->foundHosts[i].bss;
            if (host->bssid[0] == bss->bssid[0] && host->bssid[1] == bss->bssid[1] && host->bssid[2] == bss->bssid[2] &&
                host->bssid[3] == bss->bssid[3] && host->bssid[4] == bss->bssid[4] && host->bssid[5] == bss->bssid[5])
            {
                if (!(bss->gameInfo.gameNameCount_attribute & 1)) {
                    menuObj->foundHostMask        = (u16)(menuObj->foundHostMask & ~(1 << i));
                    menuObj->foundHostChangedMask = (u16)(menuObj->foundHostChangedMask | (1 << i));
                    return;
                }
                break;
            }
        } else {
            slot = i;
        }
    }
    if (!(bss->gameInfo.gameNameCount_attribute & 1)) {
        return;
    }
    count = func_02047e84(menuObj->foundHostMask);
    if ((u32)func_02047e84(menuObj->foundHostMask) >= 6U) {
        return;
    }
    menuObj->foundHostCount = count;
    if (i >= 0) {
        slot = i;
    }
    menuObj->foundHosts[slot].bss     = *bss;
    menuObj->foundHosts[slot].timeout = 180;
    menuObj->foundHostMask            = (u16)(menuObj->foundHostMask | (1 << slot));
    menuObj->foundHostChangedMask     = (u16)(menuObj->foundHostChangedMask | (1 << slot));
}

// Nonmatching (JP): JP uses a different wireless GGID
static PrcStepResult OtosuMenu_Lobby_Step_StartScan(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    switch (func_ov040_0209cb78()) {
        case 0:
            func_ov040_0209cf20();
            func_ov040_0209caac(0x400548);
            break;
        case 1:
            func_ov040_0209ba04(OtosuMenu_Lobby_OnScanResult, menuObj, &OtosuMenu_BroadcastBssid, 0);
            break;
        case 2:
            PrcCtx_AdvanceStep(ctx);
            break;
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Lobby_Step_SetupJoinScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj = (OtosuMenuObj*)object;
    OtosuMenuRectList3 texts   = OtosuMenu_Lobby_JoinTexts;

    OtosuMenu_SetupScreens(menuObj, 0x1A, 0x19, texts.rects);
    menuObj->unk_474C8     = 0xFFFF;
    menuObj->selectedHost  = 0xFFFF;
    menuObj->foundHostMask = 0;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: control flow differs
static PrcStepResult OtosuMenu_Lobby_Step_ChooseHost(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    OtosuMenuRectList5 buttons = OtosuMenu_Lobby_JoinButtons;
    Ov002_U16_12       cursors = OtosuMenu_Lobby_JoinCursors;
    s32                irq;
    s32                irq2;
    s32                selected;
    s32                cursor;
    s32                timeout;
    s32                i;
    s32                j;
    u16                touched;
    u16                changed;

    touched = OtosuMenu_GetTouchedButton(buttons.rects);
    if (touched == 0xFFFE) {
        menuObj->unk_474C8   = 0xFFFF;
        menuObj->cursor.posX = 0U;
        menuObj->cursor.posY = 0xC8U;
        return 0;
    }
    if ((touched != 0xFFFF) && (touched != 0)) {
        if ((menuObj->foundHostMask & (1 << (touched - 1))) == 0) {
            touched = 0xFFFF;
        }
    }
    if (touched != 0xFFFF) {
        if (menuObj->unk_474C8 == touched) {
            switch (touched) {
                default:
                    goto refresh;
                case 0:
                    SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CANCEL);
                    PrcCtx_ReplaceFrame(ctx, &OtosuMenu_RoleSelect_FrameDesc, NULL);
                    menuObj->linkLevelIcon.posX = 0;
                    menuObj->linkLevelIcon.posY = 0xC8;
                    PrcCtx_PushStepTable(ctx, OtosuMenu_Lobby_ShutdownSteps);
                    PrcCtx_PushStepTable(ctx, PrcSteps_FadeBrightImmediate);
                    return PRC_STEP_CONTINUE;
                case 1:
                case 2:
                case 3:
                    SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
                    if (menuObj->foundHostMask & (1 << menuObj->selectedHost)) {
                        irq               = OS_DisableIRQ();
                        menuObj->joinHost = menuObj->foundHosts[menuObj->selectedHost].bss;
                        OS_RestoreIRQ(irq);
                        menuObj->unk_41810 = 1;
                        PrcCtx_AdvanceStep(ctx);
                    }
                    goto refresh;
            }
        } else {
            SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CURSOR);
            menuObj->unk_474C8 = touched;
            if (touched != 0) {
                menuObj->selectedHost = (u16)(touched - 1);
            }
            switch (touched) {
                default:
                    break;
                case 0:
                case 1:
                case 2:
                case 3:
                    cursor               = touched * 3;
                    menuObj->cursor.posX = cursors.data[cursor + 1];
                    menuObj->cursor.posY = cursors.data[cursor + 2];
                    Sprite_ChangeAnimation(&menuObj->cursor, menuObj->cursor.animData, (s16)cursors.data[cursor],
                                           menuObj->cursor.cellTable);
                    break;
            }
            goto refresh;
        }
    } else {
        goto refresh;
    }
refresh:
    irq2 = OS_DisableIRQ();
    for (i = 0; i < 6; i++) {
        if (menuObj->foundHostMask & (1 << i)) {
            timeout                        = menuObj->foundHosts[i].timeout - 1;
            menuObj->foundHosts[i].timeout = timeout;
            if (timeout <= 0) {
                menuObj->foundHostMask        = (u16)(menuObj->foundHostMask & ~(1 << i));
                menuObj->foundHostChangedMask = (u16)(menuObj->foundHostChangedMask | (1 << i));
            }
        }
    }
    if (menuObj->foundHostChangedMask != 0) {
        menuObj->foundHostCount = func_02047e84(menuObj->foundHostMask);
    }
    OS_RestoreIRQ(irq2);
    if (menuObj->foundHostChangedMask != 0) {
        u32 irqState                  = OS_DisableIRQ();
        changed                       = menuObj->foundHostChangedMask;
        menuObj->foundHostChangedMask = 0U;
        for (j = 0; j < 6; j++) {
            if (menuObj->foundHostMask & changed & (1 << j)) {
                menuObj->foundHostBeacons[j] = *(OtosuMenuBeacon*)menuObj->foundHosts[j].bss.gameInfo.userGameInfo;
            }
        }
        OS_RestoreIRQ(irqState);
        selected = menuObj->selectedHost;
        if ((selected != 0xFFFF) && !(menuObj->foundHostMask & (1 << selected))) {
            menuObj->selectedHost = 0xFFFF;
            menuObj->unk_474C8    = 0xFFFF;
            menuObj->cursor.posX  = 0U;
            menuObj->cursor.posY  = 0xC8U;
        }
        OtosuMenu_Lobby_DrawHostList(menuObj);
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Lobby_Step_EndScan(PrcCtx* ctx, void* object) {
    u32 state;

    state = func_ov040_0209cb78();
    switch (state) { /* irregular */
        case 0:
            PrcCtx_AdvanceStep(ctx);
            break;
        case 1:
            PrcCtx_AdvanceStep(ctx);
            break;
        case 2:
            func_ov040_0209c158();
            break;
    }
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP's host packet carries a single bssid (0x66 bytes)
static void OtosuMenu_Lobby_OnHostPacket(s32 sender, void* data, void* size, OtosuMenuObj* menuObj) {
    OtosuMenuHostPacket* packet = data;
    s32                  i;

    if (data == NULL) {
        return;
    }
    if (size != (void*)0x78) {
        return;
    }
    if (menuObj == NULL) {
        return;
    }
    menuObj->syncState = packet->syncState;
    switch (packet->syncState) {
        case 2:
        case 3:
            return;
        case 0:
            for (i = 0; i < 4; i++) {
                if (packet->playerMask & (1 << i)) {
                    MI_CpuCopyU8(packet->names[i], menuObj->players[i].name, 0x16);
                    MI_CpuCopyU8(packet->bssids[i], menuObj->players[i].bssid, 6);
                }
            }
            menuObj->playerMask      = packet->playerMask;
            menuObj->playerDirtyMask = 0xF;
            menuObj->rosterVersion   = packet->rosterVersion;
            return;
    }
}

static PrcStepResult OtosuMenu_Lobby_Step_SetupGuestScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj = (OtosuMenuObj*)object;
    OtosuMenuRectList3 texts   = OtosuMenu_Lobby_GuestTexts;

    s32 ownerName;
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
    OtosuMenu_SetupScreens(object, 0x18, 0x16, texts.rects);
    ownerName = SysFont_GetOwnerName();
    MI_CpuCopyU8(ownerName, menuObj->ownName, 0x16);
    Mem_Free(&gDebugHeap, ownerName);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_REPEAT;
}

// Nonmatching: instruction scheduling
static PrcStepResult OtosuMenu_Lobby_Step_ConnectToHost(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    u32           state;

    state = func_ov040_0209cb78();
    switch (state) { /* irregular */
        case 0:
            break;
        case 1:
            MI_CpuCopyU8(menuObj->joinHost.bssid, menuObj->hostBssid, 6);
            MI_CpuCopyU8(menuObj->joinHost.bssid, menuObj->players[0].bssid, 6);
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(0);
            func_ov040_0209cabc(menuObj->ownName, 0x18);
            func_ov040_0209d434(OtosuMenu_Lobby_OnHostPacket, menuObj);
            func_ov040_0209d290(1, &menuObj->joinHost);
            break;
        case 4:
            PrcCtx_AdvanceStep(ctx);
            break;
    }
    return PRC_STEP_CONTINUE;
}

static void OtosuMenu_Lobby_OnGuestSendDone(void* arg, OtosuMenuObj* menuObj) {
    menuObj->sendBusy = 0;
}

static PrcStepResult OtosuMenu_Lobby_Step_InitGuestWait(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    menuObj->unk_474C8 = 0xFFFF;
    menuObj->sendBusy  = 0;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_REPEAT;
}

// Nonmatching: control flow differs
static PrcStepResult OtosuMenu_Lobby_Step_GuestWait(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    u8                 names[4 * sizeof(OtosuMenuPlayer)];
    u16                cursorY;
    u16                cursorX;
    u16                cursorAnim;
    s32                irq;
    s32                offset;
    s32                cursor;
    s32                i;
    u16                state;
    u16                mask;
    u16                dirty;
    u16                touched;
    OtosuMenuRectList2 buttons = OtosuMenu_Lobby_GuestButtons;

    cursorAnim = OtosuMenu_Lobby_GuestCursors.unk0;
    cursorX    = OtosuMenu_Lobby_GuestCursors.unk2;
    cursorY    = OtosuMenu_Lobby_GuestCursors.unk4;
    touched    = OtosuMenu_GetTouchedButton(buttons.rects);
    if (touched == 0xFFFE) {
        touched              = 0xFFFF;
        menuObj->unk_474C8   = 0xFFFF;
        menuObj->cursor.posX = 0U;
        menuObj->cursor.posY = 0xC8U;
    }
    if (touched != 0xFFFF) {
        if (menuObj->unk_474C8 == touched) {
            if (touched == 0) {
                SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CANCEL);
                PrcCtx_ReplaceFrame(ctx, &OtosuMenu_Lobby_JoinFrameDesc, NULL);
                PrcCtx_PushStepTable(ctx, OtosuMenu_Lobby_ShutdownSteps);
                PrcCtx_PushStepTable(ctx, PrcSteps_FadeBrightImmediate);
                return PRC_STEP_CONTINUE;
            }
            goto block_10;
        }
        menuObj->unk_474C8 = touched;
        if (touched == 0) {
            SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CANCEL);
            cursor = touched * 6;
            (void)cursor;
            menuObj->cursor.posX = cursorX;
            menuObj->cursor.posY = cursorY;
            Sprite_ChangeAnimation(&menuObj->cursor, menuObj->cursor.animData, (s16)cursorAnim, menuObj->cursor.cellTable);
        }
        goto block_10;
    }
block_10:
    if (menuObj->playerDirtyMask != 0) {
        irq                         = OS_DisableIRQ();
        mask                        = menuObj->playerMask;
        dirty                       = menuObj->playerDirtyMask;
        menuObj->beacon.playerCount = func_02047e84(mask);
        i                           = 0;
        menuObj->playerDirtyMask    = 0U;
    loop_15:
        if (i < 4) {
            if (mask & dirty & (1 << i)) {
                offset = i * (s32)sizeof(OtosuMenuPlayer);
                MI_CpuCopyU8((u8*)menuObj->players + offset, names + offset, 0x16);
            }
            i += 1;
            goto loop_15;
        }
        OS_RestoreIRQ(irq);
        OtosuMenu_DrawPlayerNames(menuObj);
    }
    if (menuObj->sendBusy == 0) {
        u32 irqState1                       = OS_DisableIRQ();
        OtosuMenu_GuestPacket.playerMask    = menuObj->playerMask;
        OtosuMenu_GuestPacket.rosterVersion = menuObj->rosterVersion;
        OS_RestoreIRQ(irqState1);
        func_0203a96c(OtosuMenu_GuestPacket.bssid);
        menuObj->sendBusy = 1;
        if (func_ov040_0209d48c(&OtosuMenu_GuestPacket, 0xA, OtosuMenu_Lobby_OnGuestSendDone, menuObj) == 0) {
            menuObj->sendBusy = 0;
        }
    }
    state = menuObj->syncState;
    if (state != 2) {
        if (state == 3) {
            PrcCtx_ReplaceFrame(ctx, &OtosuMenu_RoleSelect_FrameDesc, NULL);
            PrcCtx_PushStepTable(ctx, OtosuMenu_CommError_JoinFailedPopStepTable);
            PrcCtx_PushStepTable(ctx, OtosuMenu_Lobby_ShutdownSteps);
        }
    } else {
        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
        menuObj->matchPlayerCount = (s32)func_02047e84(menuObj->playerMask);
        PrcCtx_AdvanceStep(ctx);
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Lobby_Step_ShutdownWireless(PrcCtx* ctx, void* unused) {
    u32 state;

    state = func_ov040_0209cb78();
    switch (state) {
        default:
            break;
        case 0:
            PrcCtx_AdvanceStep(ctx);
            break;
        case 1:
            func_ov040_0209d6cc();
            break;
        case 4:
            func_ov040_0209d588();
            break;
        case 2:
            func_ov040_0209c158();
            break;
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Lobby_Step_LeaveSession(PrcCtx* ctx, void* unused) {
    u32 state;

    state = func_ov040_0209cb78();
    switch (state) {
        case 0:
        case 3:
            break;
        default:
            break;
        case 1:
            PrcCtx_AdvanceStep(ctx);
            break;
        case 4:
            func_ov040_0209d588();
            break;
        case 2:
            func_ov040_0209c158();
            break;
    }
    return PRC_STEP_CONTINUE;
}

// Nonmatching: scheduling and register allocation
static PrcStepResult OtosuMenu_Lobby_Step_SendCancel(PrcCtx* ctx, void* object) {
    if (func_ov040_0209cb68() == 1) {
        PrcCtx_AdvanceStep(ctx);
        return PRC_STEP_CONTINUE;
    }
    u32 irqState2                  = OS_DisableIRQ();
    OtosuMenu_HostPacket.syncState = 3;
    OS_RestoreIRQ(irqState2);
    func_ov040_0209d48c(&OtosuMenu_HostPacket, 0x78, OtosuMenu_Lobby_OnSendDone, object);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: scheduling and register allocation
static PrcStepResult OtosuMenu_Lobby_Step_SendStart(PrcCtx* ctx, void* object) {
    if (func_ov040_0209cb68() == 1) {
        PrcCtx_AdvanceStep(ctx);
        return PRC_STEP_CONTINUE;
    }
    u32 irqState3                  = OS_DisableIRQ();
    OtosuMenu_HostPacket.syncState = 2;
    OS_RestoreIRQ(irqState3);
    func_ov040_0209d48c(&OtosuMenu_HostPacket, 0x78, OtosuMenu_Lobby_OnSendDone, object);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: control flow differs
static PrcStepResult OtosuMenu_Lobby_Step_ShutdownHost(PrcCtx* ctx, void* object) {
    u32 state;

    state = func_ov040_0209cb78();
    switch (state) {
        default:
        case 2:
            func_ov040_0209d588();
            break;
        case 0:
            OtosuMenu_Lobby_ClearWindow();
            PrcCtx_AdvanceStep(ctx);
            break;
        case 1:
            func_ov040_0209d6cc();
            break;
        case 4:
            if (func_ov040_0209cb68() == 1) {
                func_ov040_0209d588();
            }
            break;
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Lobby_Step_StopHost(PrcCtx* ctx, void* unused) {
    u32 state;

    state = func_ov040_0209cb78();
    switch (state) {
        case 0:
        case 3:
            break;
        case 1:
            OtosuMenu_Lobby_ClearWindow();
            PrcCtx_AdvanceStep(ctx);
            break;
        case 4:
            if (func_ov040_0209cb68() == 1) {
                func_ov040_0209d588();
            }
            break;
        case 2:
        default:
            func_ov040_0209d588();
            break;
    }
    return PRC_STEP_CONTINUE;
}

static void OtosuMenu_VBlankCallback(void) {
    if (SystemStatusFlags.vblank) {
        Display_Commit();
        DMA_Flush();
        OamMgr_Commit();
        DC_PurgeRange(g_PaletteBuffers[DISPLAY_MAIN].bg, 0x400);
        GX_LoadBgPltt(g_PaletteBuffers[DISPLAY_MAIN].bg, 0, 0x200);
        GX_LoadObjPltt(g_PaletteBuffers[DISPLAY_MAIN].obj, 0, 0x200);
        DC_PurgeRange(g_PaletteBuffers[DISPLAY_SUB].bg, 0x400);
        GXs_LoadBgPltt(g_PaletteBuffers[DISPLAY_SUB].bg, 0, 0x200);
        GXs_LoadObjPltt(g_PaletteBuffers[DISPLAY_SUB].obj, 0, 0x200);
    }
}

// Nonmatching: instruction selection differs
void OtosuMenu_InitDisplayForEntry(void) {
    Interrupts_ForceVBlank();
    g_DisplaySettings.controls[0].brightness = 16;
    g_DisplaySettings.controls[1].brightness = 16;
    Interrupts_Init();
    HBlank_Init();
    GX_Init();
    DMA_Init(0x100);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForBg(1);
    GX_SetBankForObj(2);
    GX_SetBankForSubBg(4);
    GX_SetBankForSubObj(8);
    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    MI_CpuFill(0, (void*)0x06000000, 0x80000);
    Interrupts_ForceVBlank();
    MI_CpuFill(0, (void*)0x06200000, 0x20000);
    MI_CpuFill(0, (void*)0x06400000, 0x40000);
    MI_CpuFill(0, (void*)0x06600000, 0x20000);
    Interrupts_ForceVBlank();
    g_DisplaySettings.controls[0].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[0].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[1].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.unk_000                 = 0;
    REG_POWER_CNT &= ~0x8000;
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    Display_InitMainBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 0, 6, 0x4018);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 0, 5, 0x4214);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 4, 3, 1, 0x440c);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 6, 1, 1, 0x4604);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    Display_SetMainLayers(LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);

    g_DisplaySettings.controls[1].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 0, 7, 0, 0x401C);
    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 2, 5, 0, 0x4214);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 4, 3, 1, 0x440c);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 6, 1, 1, 0x4604);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    Display_SetSubLayers(LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);

    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 1;
    g_DisplaySettings.engineState[0].blendLayer1 = 62;
    g_DisplaySettings.engineState[0].blendCoeff0 = 6;
    g_DisplaySettings.engineState[0].blendCoeff1 = 10;
    g_DisplaySettings.engineState[1].blendMode   = 1;
    g_DisplaySettings.engineState[1].blendLayer0 = 1;
    g_DisplaySettings.engineState[1].blendLayer1 = 62;
    g_DisplaySettings.engineState[1].blendCoeff0 = 6;
    g_DisplaySettings.engineState[1].blendCoeff1 = 10;
    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.engineState[0].windowOutside        = 31;
    g_DisplaySettings.engineState[0].windowOutsideEffects = FALSE;
    g_DisplaySettings.controls[1].windows |= 4;
    g_DisplaySettings.engineState[1].windowOutside        = 31;
    g_DisplaySettings.engineState[1].windowOutsideEffects = FALSE;
    Interrupts_RegisterVBlankCallback(OtosuMenu_VBlankCallback, TRUE);
    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.engineState[0].windowObj            = 12;
    g_DisplaySettings.engineState[0].windowObjEffects     = TRUE;
    g_DisplaySettings.engineState[0].windowOutside        = 31;
    g_DisplaySettings.engineState[0].windowOutsideEffects = TRUE;
    g_DisplaySettings.engineState[0].blendMode            = 1;
    g_DisplaySettings.engineState[0].blendLayer0          = 4;
    g_DisplaySettings.engineState[0].blendLayer1          = 8;
    g_DisplaySettings.engineState[0].blendCoeff0          = 12;
    g_DisplaySettings.engineState[0].blendCoeff1          = 4;
    g_DisplaySettings.engineState[1].blendMode            = 1;
    g_DisplaySettings.engineState[1].windowObj            = 12;
    g_DisplaySettings.engineState[1].windowObjEffects     = TRUE;
    g_DisplaySettings.engineState[1].blendLayer0          = 4;
    g_DisplaySettings.engineState[1].blendLayer1          = 8;
    g_DisplaySettings.engineState[1].windowOutside        = 31;
    g_DisplaySettings.engineState[1].windowOutsideEffects = FALSE;
    g_DisplaySettings.engineState[1].blendCoeff0          = 12;
    g_DisplaySettings.engineState[1].blendCoeff1          = 4;
    Display_Commit();
    OamMgr_Init();
}

// Nonmatching: scheduling and register allocation
void OtosuMenu_InitDisplay(void) {
    Interrupts_ForceVBlank();
    Interrupts_Init();
    HBlank_Init();
    GX_Init();
    DMA_Init(0x100);
    Display_Init();
    g_DisplaySettings.engineState[0].window0              = 31;
    g_DisplaySettings.engineState[0].window0Effects       = TRUE;
    g_DisplaySettings.engineState[0].windowOutside        = 31;
    g_DisplaySettings.engineState[0].windowOutsideEffects = TRUE;
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForBg(1);
    GX_SetBankForObj(2);
    GX_SetBankForSubBg(4);
    GX_SetBankForSubObj(8);
    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    MI_CpuFill(0, (void*)0x06000000, 0x80000);
    Interrupts_ForceVBlank();
    MI_CpuFill(0, (void*)0x06200000, 0x20000);
    MI_CpuFill(0, (void*)0x06400000, 0x40000);
    MI_CpuFill(0, (void*)0x06600000, 0x20000);
    Interrupts_ForceVBlank();
    g_DisplaySettings.controls[0].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[0].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[1].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.unk_000                 = 0;
    REG_POWER_CNT &= ~0x8000;
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);
    g_DisplaySettings.engineState[0].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;
    g_DisplaySettings.engineState[0].bgSettings[0].mosaic   = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic   = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic   = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic   = 0;
    Display_SetMainLayers(LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);
    g_DisplaySettings.controls[1].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 0, 7, 0, 0x401C);
    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 2, 5, 0, 0x4214);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 4, 3, 1, 0x440C);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 6, 1, 1, 0x4604);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    Display_SetSubLayers(LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);

    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 1;
    g_DisplaySettings.engineState[0].blendLayer1 = 62;
    g_DisplaySettings.engineState[0].blendCoeff0 = 6;
    g_DisplaySettings.engineState[0].blendCoeff1 = 10;

    g_DisplaySettings.engineState[1].blendMode   = 1;
    g_DisplaySettings.engineState[1].blendLayer0 = 1;
    g_DisplaySettings.engineState[1].blendLayer1 = 62;
    g_DisplaySettings.engineState[1].blendCoeff0 = 6;
    g_DisplaySettings.engineState[1].blendCoeff1 = 10;

    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.engineState[0].windowOutside        = 31;
    g_DisplaySettings.engineState[0].windowOutsideEffects = FALSE;
    g_DisplaySettings.controls[1].windows |= 4;
    g_DisplaySettings.engineState[1].windowOutside        = 31;
    g_DisplaySettings.engineState[1].windowOutsideEffects = FALSE;
    Interrupts_RegisterVBlankCallback(OtosuMenu_VBlankCallback, TRUE);
    g_DisplaySettings.controls[0].windows |= 4;

    g_DisplaySettings.engineState[0].windowObj            = 12;
    g_DisplaySettings.engineState[0].windowObjEffects     = TRUE;
    g_DisplaySettings.engineState[0].windowOutside        = 31;
    g_DisplaySettings.engineState[0].windowOutsideEffects = TRUE;
    g_DisplaySettings.engineState[0].blendMode            = 1;
    g_DisplaySettings.engineState[0].blendLayer0          = 4;
    g_DisplaySettings.engineState[0].blendLayer1          = 8;
    g_DisplaySettings.engineState[0].blendCoeff0          = 12;
    g_DisplaySettings.engineState[0].blendCoeff1          = 4;

    g_DisplaySettings.engineState[1].blendMode            = 1;
    g_DisplaySettings.engineState[1].windowObj            = 12;
    g_DisplaySettings.engineState[1].windowObjEffects     = TRUE;
    g_DisplaySettings.engineState[1].blendLayer0          = 4;
    g_DisplaySettings.engineState[1].blendLayer1          = 8;
    g_DisplaySettings.engineState[1].windowOutside        = 31;
    g_DisplaySettings.engineState[1].windowOutsideEffects = FALSE;
    g_DisplaySettings.engineState[1].blendCoeff0          = 12;
    g_DisplaySettings.engineState[1].blendCoeff1          = 4;

    Display_InitMainBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 5, 0, 0x4014);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 1, 0, 0x4204);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 4, 1, 1, 0x4404);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 6, 3, 1, 0x460C);

    Display_Commit();
    OamMgr_Init();
}

// Nonmatching: instruction selection differs
void OtosuMenu_InitDisplayForSaveData(void) {
    Interrupts_ForceVBlank();
    Display_Init();
    Interrupts_Init();
    HBlank_Init();
    DMA_Init(0x100);
    Interrupts_RegisterHBlankCallback(0, 1);
    Interrupts_RegisterVBlankCallback(OtosuMenu_VBlankCallback, 1);
    g_DisplaySettings.controls[0].brightness = -16;
    g_DisplaySettings.controls[1].brightness = -16;
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForBg(1);
    GX_SetBankForObj(2);
    GX_SetBankForSubBg(4);
    GX_SetBankForSubObj(8);
    MI_CpuFill(0, 0x06800000, 0xA4000);
    MI_CpuFill(0, 0x06000000, 0x80000);
    Interrupts_ForceVBlank();
    MI_CpuFill(0, 0x06200000, 0x20000);
    MI_CpuFill(0, 0x06400000, 0x40000);
    MI_CpuFill(0, 0x06600000, 0x20000);
    Interrupts_ForceVBlank();
    g_DisplaySettings.controls[0].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[0].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[1].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.unk_000                 = 0;
    REG_POWER_CNT &= ~0x8000;
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    Display_SetMainLayers(LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);
    g_DisplaySettings.controls[1].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 0, 7, 0, 0x401C);
    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 2, 5, 0, 0x4214);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 4, 3, 1, 0x440c);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 6, 1, 1, 0x4604);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    Display_SetSubLayers(LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);
    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.controls[1].windows |= 4;

    g_DisplaySettings.engineState[0].windowObj            = 12;
    g_DisplaySettings.engineState[0].windowObjEffects     = TRUE;
    g_DisplaySettings.engineState[0].windowOutside        = 31;
    g_DisplaySettings.engineState[0].windowOutsideEffects = TRUE;
    g_DisplaySettings.engineState[0].blendMode            = 1;
    g_DisplaySettings.engineState[0].blendLayer0          = 4;
    g_DisplaySettings.engineState[0].blendLayer1          = 8;
    g_DisplaySettings.engineState[0].blendCoeff0          = 12;
    g_DisplaySettings.engineState[0].blendCoeff1          = 4;
    g_DisplaySettings.engineState[1].blendMode            = 1;
    g_DisplaySettings.engineState[1].windowObj            = 12;
    g_DisplaySettings.engineState[1].windowObjEffects     = TRUE;
    g_DisplaySettings.engineState[1].blendLayer0          = 4;
    g_DisplaySettings.engineState[1].blendLayer1          = 8;
    g_DisplaySettings.engineState[1].windowOutside        = 31;
    g_DisplaySettings.engineState[1].windowOutsideEffects = FALSE;
    g_DisplaySettings.engineState[1].blendCoeff0          = 12;
    g_DisplaySettings.engineState[1].blendCoeff1          = 4;

    Display_InitMainBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 5, 0, 0x4014);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 1, 0, 0x4204);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 4, 1, 1, 0x4404);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 6, 3, 1, 0x460c);

    Display_Commit();
    OamMgr_Init();
}
