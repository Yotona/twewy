#include "Otosu/OtosuMenu.h"

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
    };
    u16 data[10];
} Ov002_U16_10;

static const Ov002_U16_10 OtosuMenu_Ranking_HostWaitTexts;

static const OtosuMenuRectList2 OtosuMenu_Ranking_SinglePlayerButtons = {
    {
     {0x0000, 0x0068, 0x005C, 0x00B0, 0x0068},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     }
};

static void          OtosuMenu_Ranking_Load(PrcCtx* ctx, OtosuMenuObj* menuObj);
static void          OtosuMenu_Ranking_Destroy(PrcCtx* ctx, OtosuMenuObj* menuObj);
static void          OtosuMenu_Ranking_Update(void);
static void          OtosuMenu_Ranking_Render(void);
static PrcStepResult OtosuMenu_Ranking_Step_RecordResult(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_HandleChoice(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_Begin(PrcCtx* ctx, void* unused);
static PrcStepResult OtosuMenu_Ranking_Step_SetupDisconnectScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_WaitDisconnect(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_ReturnToGame(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_ExitSinglePlayer(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_InitHost(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_StartHost(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_SetupHostWaitScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_HostWaitGuests(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_SetupHostScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_HostChoose(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_SetupGuestWaitScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_ConnectToHost(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_InitGuestWait(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_GuestWait(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_SetupGuestScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_WaitHostChoice(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_SetupSinglePlayerNames(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_SetupSinglePlayerScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_SinglePlayerWaitExit(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_ShutdownWireless(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_LeaveSession(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_CheckGuests(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_SendExit(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_ShutdownHost(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Ranking_Step_StopHost(PrcCtx* ctx, void* object);
static PrcStepFn     OtosuMenu_Ranking_GuestStepTable[] = {
    OtosuMenu_Ranking_Step_Begin,
    OtosuPrcStep_LoadPlayers,
    OtosuMenu_Ranking_Step_SetupGuestWaitScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_Ranking_Step_ConnectToHost,
    OtosuMenu_Ranking_Step_InitGuestWait,
    OtosuMenu_Ranking_Step_GuestWait,
    PrcStep_StartDelay,
    PrcStep_TickDelay,
    PrcStep_StartDelay,
    PrcStep_TickDelay,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_Ranking_Step_SetupGuestScreen,
    OtosuMenu_Ranking_Step_RecordResult,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_Ranking_Step_WaitHostChoice,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_Ranking_Step_LeaveSession,
    OtosuPrcStep_SavePlayers,
    OtosuMenu_Ranking_Step_HandleChoice,
    OtosuMenu_Ranking_Step_SetupDisconnectScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_Ranking_Step_ShutdownWireless,
    OtosuMenu_Ranking_Step_WaitDisconnect,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_Ranking_Step_ReturnToGame,
    PrcStep_PopFrame,
};

static PrcStepFn OtosuMenu_Ranking_HostStepTable[] = {
    OtosuMenu_Ranking_Step_Begin,
    OtosuPrcStep_LoadPlayers,
    OtosuMenu_Ranking_Step_SetupHostWaitScreen,
    OtosuMenu_Ranking_Step_InitHost,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_Ranking_Step_StartHost,
    OtosuMenu_Ranking_Step_HostWaitGuests,
    PrcStep_StartDelay,
    PrcStep_TickDelay,
    PrcStep_StartDelay,
    PrcStep_TickDelay,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_Ranking_Step_SetupHostScreen,
    OtosuMenu_Ranking_Step_RecordResult,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_Ranking_Step_HostChoose,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_Ranking_Step_CheckGuests,
    OtosuMenu_Ranking_Step_SendExit,
    OtosuMenu_Ranking_Step_StopHost,
    OtosuPrcStep_SavePlayers,
    OtosuMenu_Ranking_Step_HandleChoice,
    OtosuMenu_Ranking_Step_SetupDisconnectScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_Ranking_Step_ShutdownHost,
    OtosuMenu_Ranking_Step_WaitDisconnect,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_Ranking_Step_ReturnToGame,
    PrcStep_PopFrame,
};
static const OtosuMenuRectList3 OtosuMenu_Ranking_HostChoiceTexts = {
    {
     {0x2407, 0x68, 0x46, 0xB0, 0x56},
     {0x2408, 0x68, 0x6E, 0xB0, 0x7E},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList2 OtosuMenu_Ranking_GuestWaitTexts = {
    {
     {0x23F8, 0x0008, 0x0008, 0x00F8, 0x0040},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList2 OtosuMenu_Ranking_DisconnectTexts = {
    {
     {0x240B, 0x0008, 0x0008, 0x00F8, 0x0040},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};

static PrcStepFn OtosuMenu_Ranking_SinglePlayerStepTable[] = {
    OtosuMenu_Ranking_Step_Begin,
    OtosuPrcStep_LoadPlayers,
    OtosuMenu_Ranking_Step_SetupSinglePlayerNames,
    OtosuMenu_Ranking_Step_SetupSinglePlayerScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_Ranking_Step_SinglePlayerWaitExit,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_Ranking_Step_RecordResult,
    OtosuPrcStep_SavePlayers,
    OtosuMenu_Ranking_Step_ExitSinglePlayer,
    PrcStep_PopFrame,
};
static const OtosuMenuRectList2 OtosuMenu_Ranking_SinglePlayerTexts = {
    {
     {0x240A, 0x0068, 0x005C, 0x00B0, 0x0068},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};

PrcFrameDesc OtosuMenu_Ranking_GuestFrameDesc = {
    .enter     = OtosuMenu_Ranking_Load,
    .stepTable = OtosuMenu_Ranking_GuestStepTable,
    .update    = OtosuMenu_Ranking_Update,
    .render    = OtosuMenu_Ranking_Render,
    .exit      = OtosuMenu_Ranking_Destroy,
};
static const OtosuMenuRectList3 OtosuMenu_Ranking_HostChoiceButtons = {
    {
     {0, 0x50, 0x40, 0xB0, 0x58},
     {1, 0x50, 0x68, 0xB0, 0x80},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     }
};
static const Ov002_U16_4 OtosuMenu_OpponentNameSets[0x2C] = {
    {0x240C, 0x2421, 0xFFFF, 0xFFFF},
    {0x240C, 0x2423, 0xFFFF, 0xFFFF},
    {0x240C, 0x240D, 0xFFFF, 0xFFFF},
    {0x240C, 0x240E, 0xFFFF, 0xFFFF},
    {0x240C, 0x241F, 0xFFFF, 0xFFFF},
    {0x240C, 0x241B, 0xFFFF, 0xFFFF},
    {0x240C, 0x2428, 0x2429, 0x242A},
    {0x240C, 0x242B, 0x242C, 0x242D},
    {0x240C, 0x2418, 0xFFFF, 0xFFFF},
    {0x240E, 0x2419, 0xFFFF, 0xFFFF},
    {0x240C, 0x2420, 0xFFFF, 0xFFFF},
    {0x240C, 0x241A, 0xFFFF, 0xFFFF},
    {0x240C, 0x241B, 0xFFFF, 0xFFFF},
    {0x240C, 0x241E, 0x241E, 0xFFFF},
    {0x2423, 0x2422, 0x242E, 0x242F},
    {0x2423, 0x241D, 0xFFFF, 0xFFFF},
    {0x2423, 0x2417, 0x2417, 0xFFFF},
    {0x240C, 0x2416, 0x2416, 0x2416},
    {0x240C, 0x241F, 0xFFFF, 0xFFFF},
    {0x2423, 0x2421, 0xFFFF, 0xFFFF},
    {0x240C, 0x241C, 0xFFFF, 0xFFFF},
    {0x2423, 0x2425, 0x2425, 0x2425},
    {0x240C, 0x2411, 0xFFFF, 0xFFFF},
    {0x2426, 0x2412, 0xFFFF, 0xFFFF},
    {0x240E, 0x2413, 0xFFFF, 0xFFFF},
    {0x240D, 0x2414, 0xFFFF, 0xFFFF},
    {0x240C, 0x2415, 0xFFFF, 0xFFFF},
    {0x2423, 0x2411, 0xFFFF, 0xFFFF},
    {0x2423, 0x2411, 0xFFFF, 0xFFFF},
    {0x240C, 0x2423, 0xFFFF, 0xFFFF},
    {0x240C, 0x2424, 0xFFFF, 0xFFFF},
    {0x240C, 0x2421, 0xFFFF, 0xFFFF},
    {0x240C, 0x2421, 0xFFFF, 0xFFFF},
    {0x240C, 0x2423, 0xFFFF, 0xFFFF},
    {0x240C, 0x2421, 0xFFFF, 0xFFFF},
    {0x240C, 0x2423, 0xFFFF, 0xFFFF},
    {0x240C, 0x2423, 0x2421, 0xFFFF},
    {0x240C, 0x2427, 0x2425, 0x241C},
    {0x240C, 0x2418, 0xFFFF, 0xFFFF},
    {0x240C, 0x2419, 0xFFFF, 0xFFFF},
    {0x240C, 0x2420, 0xFFFF, 0xFFFF},
    {0x240C, 0x241A, 0xFFFF, 0xFFFF},
    {0x240C, 0x241E, 0xFFFF, 0xFFFF},
    {0x240C, 0x242E, 0xFFFF, 0xFFFF}
}; /* const */
PrcFrameDesc OtosuMenu_Ranking_SinglePlayerFrameDesc = {
    .enter     = OtosuMenu_Ranking_Load,
    .stepTable = OtosuMenu_Ranking_SinglePlayerStepTable,
    .update    = OtosuMenu_Ranking_Update,
    .render    = OtosuMenu_Ranking_Render,
    .exit      = OtosuMenu_Ranking_Destroy,
};
static const OtosuMenuRectList2 OtosuMenu_Ranking_GuestTexts = {
    {
     {0x2409, 0x0008, 0x0008, 0x00F8, 0x0040},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
PrcFrameDesc OtosuMenu_Ranking_HostFrameDesc = {
    .enter     = OtosuMenu_Ranking_Load,
    .stepTable = OtosuMenu_Ranking_HostStepTable,
    .update    = OtosuMenu_Ranking_Update,
    .render    = OtosuMenu_Ranking_Render,
    .exit      = OtosuMenu_Ranking_Destroy,
};

// Nonmatching: control flow differs
static void OtosuMenu_Ranking_CreateRankBoards(OtosuMenuObj* menuObj) {
    OtosuMenu_RankBoardArg arg;
    PrcCtx*                boards[4] = {&menuObj->objCtx[0], &menuObj->objCtx[1], &menuObj->objCtx[2], &menuObj->objCtx[3]};
    u16                    place;
    u8                     player;
    PrcCtx*                board;

    for (place = 0; place < gSaveData.otosuPlayerCount; place++) {
        player     = menuObj->placePlayers[place];
        arg.winner = (menuObj->firstPlaceUnique != 0 && place == 0) ? TRUE : FALSE;
        board      = boards[place];
        PrcCtx_Init(board, "OtosuMenu_RankBoardObj", 0x220);
        arg.place  = place;
        arg.rank   = menuObj->placeRanks[place];
        arg.player = player;
        arg.score  = gSaveData.otosuScores[player];
        PrcCtx_ReplaceFrame(board, &OtosuMenu_RankBoard_FrameDesc, &arg);
        PrcMaster_RegisterContext(&menuObj->prcMaster, board);
        if (menuObj->firstPlaceUnique != 0 && menuObj->playerRanks[gSaveData.otosuGameKey] == 0) {
            menuObj->outcome = 2;
        } else if (menuObj->playerRanks[gSaveData.otosuGameKey] == 0) {
            if (menuObj->isSinglePlayer != 0) {
                menuObj->outcome = 2;
            } else {
                menuObj->outcome = 0;
            }
        } else {
            menuObj->outcome = 1;
        }
    }
}

// Nonmatching: scheduling and register allocation
static void OtosuMenu_Ranking_Load(PrcCtx* ctx, OtosuMenuObj* menuObj) {
    u16 i;

    i                        = 0;
    menuObj->cursor.posX     = 0;
    menuObj->cursor.posY     = 0xC8;
    menuObj->commErrorScreen = 1;
    do {
        u16* score = &gSaveData.otosuScores[i];
        if (*score > 0x270FU) {
            *score = 0x270FU;
        }
        i += 1;
    } while ((u32)i < 4U);
}

static void OtosuMenu_Ranking_Destroy(PrcCtx* ctx, OtosuMenuObj* menuObj) {
    OtosuMenu_ReleaseScreens(menuObj);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
}

static void OtosuMenu_Ranking_Update(void) {}

static void OtosuMenu_Ranking_Render(void) {}

static PrcStepResult OtosuMenu_Ranking_Step_RecordResult(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    u32           total;

    if ((menuObj->firstPlaceUnique != 0) && (menuObj->playerRanks[gSaveData.otosuGameKey] == 0)) {
        gSaveData.unk_1D84 = (u8)(gSaveData.unk_1D84 + 1);
    }
    total                = gSaveData.mabsBasePP + gSaveData.otosuScores[gSaveData.otosuGameKey];
    gSaveData.mabsBasePP = total;
    if (total > 0x270FU) {
        gSaveData.mabsBasePP = 0x270FU;
    }
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_REPEAT;
}

static PrcStepResult OtosuMenu_Ranking_Step_HandleChoice(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    u16           state;

    state = menuObj->syncState;
    switch (state) { /* irregular */
        case 7:
            OtosuMenu_DestroyResultObjects(menuObj);
            OtosuMenu_DestroyCursorAndLinkLevel(menuObj);
            OtosuMenu_ReleaseScreens(menuObj);
            OtosuMenu_InitDisplay();
            OtosuMenu_LoadCursorAndLinkLevel(menuObj);
            if (gSaveData.otosuGameKey == 0) {
                PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_StageSelect_HostFrameDesc, NULL);
            } else {
                PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_StageSelect_GuestFrameDesc, NULL);
            }
            break;
        case 8:
            PrcCtx_AdvanceStep(ctx);
            break;
        default:
            OS_WaitForever();
            break;
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Ranking_Step_Begin(PrcCtx* ctx, void* unused) {
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Ranking_Step_SetupDisconnectScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj = (OtosuMenuObj*)object;
    OtosuMenuRectList2 texts   = OtosuMenu_Ranking_DisconnectTexts;

    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
    OtosuMenu_DestroyResultObjects(menuObj);
    OtosuMenu_SetupResultMessageScreens(menuObj, texts.rects);
    PrcCtx_Init(&menuObj->objCtx[4], "OtosuMenu_Icon2", 0x40);
    PrcCtx_ReplaceFrame(&menuObj->objCtx[4], &OtosuMenu_Icon2_FrameDesc, NULL);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
    menuObj->unk_474C8 = 60;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Ranking_Step_WaitDisconnect(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    menuObj->unk_474C8--;
    if (menuObj->unk_474C8 == 0) {
        PrcMaster_UnregisterContextOrPanic(&menuObj->prcMaster, &menuObj->objCtx[4]);
        PrcCtx_AdvanceStep(ctx);
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Ranking_Step_ReturnToGame(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    menuObj->nextScene    = 3;
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Ranking_Step_ExitSinglePlayer(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    menuObj->nextScene    = 4;
    return PRC_STEP_CONTINUE;
}

static void OtosuMenu_Ranking_OnSendDone(void* arg, OtosuMenuObj* menuObj) {
    menuObj->sendBusy = 0;
}

static s32 OtosuMenu_Ranking_OnGuestConnected(OtosuMenuChildInfo* guest, OtosuMenuObj* menuObj) {
    if (guest->aid >= 4) {
        return FALSE;
    }
    MI_CpuCopyU8(guest->name, menuObj->players[guest->player].name, 0x16);
    MI_CpuCopyU8(guest->bssid, menuObj->players[guest->aid].bssid, 6);
    menuObj->playerMask |= 1 << guest->aid;
    menuObj->playerDirtyMask |= 1 << guest->aid;
    menuObj->guestAckMask[guest->aid] = 0;
    return TRUE;
}

static void OtosuMenu_Ranking_OnGuestDisconnected(OtosuMenuChildInfo* guest, OtosuMenuObj* menuObj) {
    if (guest->aid < 4) {
        menuObj->playerMask &= ~(1 << guest->aid);
        menuObj->playerDirtyMask |= 1 << guest->aid;
    }
}

static void OtosuMenu_Ranking_OnBeaconUpdated(void* arg, OtosuMenuObj* menuObj) {
    menuObj->beaconUpdateBusy = 0;
}

static void OtosuMenu_Ranking_OnGuestPacket(s32 guest, void* data, void* size, OtosuMenuObj* menuObj) {
    if (data == NULL) {
        return;
    }
    if (size != (void*)0xA) {
        return;
    }
    if (menuObj == 0) {
        return;
    }
    menuObj->guestAckMask[guest]    = *(u16*)data;
    menuObj->guestAckVersion[guest] = *(u16*)((u8*)data + 2);
}

static void OtosuMenu_Ranking_RefreshRoster(OtosuMenuObj* menuObj) {
    menuObj->beaconUpdatePending = 1;
    menuObj->rosterChanged       = 1;
    u32 irqState                 = OS_DisableIRQ();
    menuObj->playerDirtyMask     = 0;
    OS_RestoreIRQ(irqState);
}

static void OtosuMenu_Ranking_UpdateBeacon(OtosuMenuObj* menuObj) {
    s32 acceptNew;

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
    if (func_ov040_0209d788(OtosuMenu_Ranking_OnBeaconUpdated, menuObj, &menuObj->beacon, 0x1E, acceptNew) == 0) {
        menuObj->beaconUpdateBusy = 0;
    }
}

static PrcStepResult OtosuMenu_Ranking_Step_InitHost(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

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
    PrcCtx_Init(&menuObj->objCtx[4], "OtosuMenu_Icon2", 0x40);
    PrcCtx_ReplaceFrame(&menuObj->objCtx[4], &OtosuMenu_Icon2_FrameDesc, NULL);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: instruction scheduling
static PrcStepResult OtosuMenu_Ranking_Step_StartHost(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    s32           temp_r4;
    s32           temp_r5;
    u32           state;

    state = func_ov040_0209cb78();
    switch (state) {
        default:
            break;
        case 0:
            func_ov040_0209caac(0x400548);
            break;
        case 1:
            func_ov040_0209cb9c();
            break;
        case 7:
            temp_r5 = func_ov040_0209cde4();
            temp_r4 = func_020442f8();
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(2);
            func_ov040_0209cb08(&menuObj->beacon, 0x1E);
            func_ov040_0209d40c(OtosuMenu_Ranking_OnGuestConnected, menuObj);
            func_ov040_0209d420(OtosuMenu_Ranking_OnGuestDisconnected, menuObj);
            func_ov040_0209d434(OtosuMenu_Ranking_OnGuestPacket, menuObj);
            func_ov040_0209d0a8(0, temp_r4, temp_r5);
            break;
        case 4:
            PrcCtx_AdvanceStep(ctx);
            break;
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Ranking_Step_SetupHostWaitScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    Ov002_U16_10  texts   = OtosuMenu_Ranking_HostWaitTexts;
    u32           statusFlag;

    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
    OtosuMenu_SetupResultMessageScreens(menuObj, (const OtosuMenuRect*)texts.data);
    statusFlag               = SystemStatusFlags.unk_06;
    SystemStatusFlags.unk_06 = 0;
    menuObj->unk_46070       = statusFlag;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 0;
    menuObj->unk_474C8       = 600;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: control flow differs
static PrcStepResult OtosuMenu_Ranking_Step_HostWaitGuests(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    s32           irq;
    void*         step;
    s32           sendPacket;
    s32           i;
    s32           j;
    u16           mask;
    u32           n;

    step       = ctx;
    sendPacket = 0;
    func_ov002_020824a0();
    if (menuObj->playerDirtyMask != 0) {
        OtosuMenu_Ranking_RefreshRoster(menuObj);
    }
    if ((menuObj->beaconUpdatePending != 0) && (menuObj->beaconUpdateBusy == 0)) {
        OtosuMenu_Ranking_UpdateBeacon(menuObj);
    }
    if ((menuObj->playerMask != 1) && (menuObj->sendBusy == 0)) {
        if (menuObj->rosterChanged != 0) {
            menuObj->rosterChanged = 0;
            sendPacket             = 1;
            menuObj->rosterVersion = (u16)(menuObj->rosterVersion + 1);
        } else {
            i = 1;
        loop_16:
            if (i < 4) {
                mask = menuObj->playerMask;
                if (mask & (1 << i)) {
                    if (mask != menuObj->guestAckMask[i]) {
                        sendPacket = 1;
                    } else if (menuObj->rosterVersion != menuObj->guestAckVersion[i]) {
                        sendPacket = 1;
                    } else {
                        goto block_15;
                    }
                } else {
                block_15:
                    i += 1;
                    goto loop_16;
                }
            }
            if ((sendPacket == 0) && (menuObj->beacon.playerCount == gSaveData.otosuPlayerCount)) {
                menuObj->syncState = 4U;
                sendPacket         = 1;
            }
        }
    }
    if (sendPacket != 0) {
        irq                            = OS_DisableIRQ();
        OtosuMenu_HostPacket.syncState = menuObj->syncState;
        if (menuObj->syncState != 0) {

        } else {
            j                                  = 0;
            OtosuMenu_HostPacket.playerMask    = menuObj->playerMask;
            OtosuMenu_HostPacket.rosterVersion = menuObj->rosterVersion;
        loop_31:
            if (j < 4) {
                if (menuObj->playerMask & (1 << j)) {
                    if (j == 0) {
                        n = 0;
                    loop_28:
                        if (n < 0xBU) {
                            n = (u32)(u16)(n + 1);
                            goto loop_28;
                        }
                    }
                    MI_CpuCopyU8(menuObj->players[j].name, OtosuMenu_HostPacket.names[j], 0x16);
                }
                j += 1;
                goto loop_31;
            }
        }
        OS_RestoreIRQ(irq);
        menuObj->sendBusy = 1;
        if (func_ov040_0209d48c(&OtosuMenu_HostPacket, 0x78, OtosuMenu_Ranking_OnSendDone, menuObj) == 0) {
            menuObj->sendBusy = 0;
        }
    }
    menuObj->unk_474C8--;
    if (menuObj->unk_474C8 == 0) {
        PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_CommError_ConnectionLostFrameDesc, NULL);
        return PRC_STEP_CONTINUE;
    }
    if (menuObj->syncState == 4) {
        PrcCtx_AdvanceStep(step);
    }
    return PRC_STEP_CONTINUE;
}

// Nonmatching: instruction selection differs
static PrcStepResult OtosuMenu_Ranking_Step_SetupHostScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj  = (OtosuMenuObj*)object;
    OtosuMenuRectList3 texts    = OtosuMenu_Ranking_HostChoiceTexts;
    s32                names[5] = {0, 0, 0, 0, 0};
    u16                i;

    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
    OtosuMenu_ComputeRanks(menuObj);
    for (i = 0; i < gSaveData.otosuPlayerCount; i++) {
        names[i] = OtosuMenu_GetPlaceName(menuObj, i);
    }
    names[i] = 0;
    OtosuMenu_SetupResultRankingScreens(menuObj, 0xA, 0xC, names, texts.rects);
    OtosuMenu_Ranking_CreateRankBoards(menuObj);
    menuObj->board     = 0xFFFF;
    menuObj->unk_474CA = menuObj->playerMask;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: control flow differs
static PrcStepResult OtosuMenu_Ranking_Step_HostChoose(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj    = (OtosuMenuObj*)object;
    u16                cursors[6] = {5, 0x47, 0x3C, 5, 0x47, 0x64};
    OtosuMenuRectList3 buttons    = OtosuMenu_Ranking_HostChoiceButtons;
    s32                irq;
    u16                state;
    u16                touched;
    u16*               cursor;

    touched = OtosuMenu_GetTouchedButton(buttons.rects);
    if (menuObj->unk_474CA != func_ov040_0209cb68()) {
        PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_CommError_ConnectionLostFrameDesc, NULL);
        return PRC_STEP_CONTINUE;
    }
    if (touched == 0xFFFF) {
        return PRC_STEP_CONTINUE;
    }
    if (touched == 0xFFFE) {
        menuObj->board       = 0xFFFFU;
        menuObj->cursor.posX = 0U;
        menuObj->cursor.posY = 0xD2U;
        return PRC_STEP_CONTINUE;
    }
    if (menuObj->board == touched) {
        switch (touched) { /* switch 1; irregular */
            case 0:        /* switch 1 */
                SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
                menuObj->syncState = 7U;
                break;
            case 1: /* switch 1 */
                SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
                menuObj->syncState = 8U;
                break;
        }
    } else {
        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CURSOR);
        menuObj->board       = touched;
        cursor               = &cursors[touched * 3];
        menuObj->cursor.posX = cursor[1];
        menuObj->cursor.posY = cursor[2];
        Sprite_ChangeAnimation(&menuObj->cursor, menuObj->cursor.animData, (s16)cursor[0], menuObj->cursor.cellTable);
    }
    irq                            = OS_DisableIRQ();
    OtosuMenu_HostPacket.syncState = menuObj->syncState;
    state                          = menuObj->syncState;
    switch (state) { /* switch 2; irregular */
        case 7:      /* switch 2 */
            OtosuMenu_HostPacket.board = menuObj->board;
            PrcCtx_AdvanceStep(ctx);
            break;
        case 8: /* switch 2 */
            OtosuMenu_HostPacket.board = menuObj->board;
            PrcCtx_AdvanceStep(ctx);
            break;
    }
    OS_RestoreIRQ(irq);
    func_ov040_0209d48c(&OtosuMenu_HostPacket, 0x78, OtosuMenu_Ranking_OnSendDone, menuObj);
    return PRC_STEP_CONTINUE;
}

static const Ov002_U16_10 OtosuMenu_Ranking_HostWaitTexts = {0x23F8, 0x0008, 0x0008, 0x00F8, 0x0040,
                                                             0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000}; /* const */
// Nonmatching (JP): JP's host packet carries a single bssid (0x66 bytes)
static void OtosuMenu_Ranking_OnHostPacket(s32 sender, void* data, void* size, OtosuMenuObj* menuObj) {
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
        case 3:
        case 4:
            break;
        case 0:
            for (i = 0; i < 4; i++) {
                if (packet->playerMask & (1 << i)) {
                    MI_CpuCopyU8(packet->names[i], menuObj->players[i].name, 0x16);
                }
            }
            menuObj->playerMask      = packet->playerMask;
            menuObj->playerDirtyMask = 0xF;
            menuObj->rosterVersion   = packet->rosterVersion;
            break;
    }
}

static PrcStepResult OtosuMenu_Ranking_Step_SetupGuestWaitScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj = (OtosuMenuObj*)object;
    OtosuMenuRectList2 texts   = OtosuMenu_Ranking_GuestWaitTexts;
    u32                statusFlag;

    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
    OtosuMenu_SetupResultMessageScreens(menuObj, texts.rects);
    statusFlag               = SystemStatusFlags.unk_06;
    SystemStatusFlags.unk_06 = 0;
    menuObj->unk_46070       = statusFlag;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 0;
    PrcCtx_Init(&menuObj->objCtx[4], "OtosuMenu_Icon2", 0x40);
    PrcCtx_ReplaceFrame(&menuObj->objCtx[4], &OtosuMenu_Icon2_FrameDesc, NULL);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
    menuObj->unk_474C8 = 600;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: instruction scheduling
static PrcStepResult OtosuMenu_Ranking_Step_ConnectToHost(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    u32           state;

    state = func_ov040_0209cb78();
    switch (state) {
        case 2:
        case 3:
            break;
        default:
            break;
        case 0:
            func_ov040_0209caac(0x400548);
            break;
        case 1:
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(2);
            func_ov040_0209cabc(menuObj->ownName, 0x18);
            func_ov040_0209b8b8(1, menuObj->players[0].bssid, 0);
            func_ov040_0209d434(OtosuMenu_Ranking_OnHostPacket, menuObj);
            break;
        case 4:
            PrcCtx_AdvanceStep(ctx);
            break;
    }
    menuObj->unk_474C8--;
    if (menuObj->unk_474C8 != 0) {
        return 0;
    }
    PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_CommError_ConnectionLostFrameDesc, NULL);
    return 0;
}

static void OtosuMenu_Ranking_OnGuestSendDone(void* arg, OtosuMenuObj* menuObj) {
    menuObj->sendBusy = 0;
}

static PrcStepResult OtosuMenu_Ranking_Step_InitGuestWait(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    menuObj->unk_474C8 = 0xFFFF;
    menuObj->unk_474CA = 0;
    menuObj->unk_474CC = 0;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: control flow differs
static PrcStepResult OtosuMenu_Ranking_Step_GuestWait(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    if (menuObj->playerDirtyMask != 0) {
        menuObj->unk_474CC = 1U;
    }
    if ((menuObj->sendBusy == 0) && (menuObj->unk_474CC != 0)) {
        u32 irqState1                       = OS_DisableIRQ();
        OtosuMenu_GuestPacket.playerMask    = menuObj->playerMask;
        OtosuMenu_GuestPacket.rosterVersion = menuObj->rosterVersion;
        OS_RestoreIRQ(irqState1);
        menuObj->unk_474CC = 0U;
        menuObj->sendBusy  = 1;
        if (func_ov040_0209d48c(&OtosuMenu_GuestPacket, 0xA, OtosuMenu_Ranking_OnGuestSendDone, menuObj) == 0) {
            menuObj->sendBusy = 0;
        }
    }
    if (menuObj->syncState == 4) {
        PrcCtx_AdvanceStep(ctx);
    }
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP's host packet carries a single bssid (0x66 bytes)
static void OtosuMenu_Ranking_OnHostChoice(s32 sender, void* data, void* size, OtosuMenuObj* menuObj) {
    u16 state;

    if (data == NULL) {
        return;
    }
    if (size != (void*)0x78) {
        return;
    }
    if (menuObj == NULL) {
        return;
    }
    state = ((OtosuMenuHostPacket*)data)->syncState;
    switch (state) {
        case 3:
        case 8:
            menuObj->syncState = 8;
            menuObj->board     = ((OtosuMenuHostPacket*)data)->board;
            return;
        case 7:
            menuObj->syncState = state;
            menuObj->board     = ((OtosuMenuHostPacket*)data)->board;
            return;
    }
}

// Nonmatching: instruction selection differs
static PrcStepResult OtosuMenu_Ranking_Step_SetupGuestScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj  = (OtosuMenuObj*)object;
    OtosuMenuRectList2 texts    = OtosuMenu_Ranking_GuestTexts;
    s32                names[5] = {0, 0, 0, 0, 0};
    u16                i;

    OtosuMenu_ComputeRanks(menuObj);
    for (i = 0; i < gSaveData.otosuPlayerCount; i++) {
        names[i] = OtosuMenu_GetPlaceName(menuObj, i);
    }
    names[i] = 0;
    OtosuMenu_SetupResultRankingScreens(menuObj, 0xFFFF, 0xB, names, texts.rects);
    OtosuMenu_Ranking_CreateRankBoards(menuObj);
    menuObj->unk_474C8 = 0xFFFF;
    func_ov040_0209d434(OtosuMenu_Ranking_OnHostChoice, menuObj);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Ranking_Step_WaitHostChoice(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    u16           state;

    state = menuObj->syncState;
    switch (state) { /* irregular */
        case 3:
        case 8:
            PrcCtx_AdvanceStep(ctx);
            break;
        case 7:
            PrcCtx_AdvanceStep(ctx);
            break;
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Ranking_Step_SetupSinglePlayerNames(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    const u16*    msgIds;
    SysCode*      name;
    u16           i;

    menuObj->matchPlayerCount = 0;
    for (i = 0; i < 4; i++) {
        msgIds = (const u16*)&OtosuMenu_OpponentNameSets[(u8)gSaveData.unk_341C];
        if (msgIds[i] == 0xFFFF) {
            break;
        }
        name = SysFont_GetMsgBuf(&menuObj->font, msgIds[i]);
        MI_CpuCopyU16(name, menuObj->players[i].name, sizeof(menuObj->players[i].name));
        Mem_Free(&gDebugHeap, name);
        menuObj->matchPlayerCount++;
    }
    gSaveData.otosuPlayerCount = i;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: instruction selection differs
static PrcStepResult OtosuMenu_Ranking_Step_SetupSinglePlayerScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj  = (OtosuMenuObj*)object;
    OtosuMenuRectList2 texts    = OtosuMenu_Ranking_SinglePlayerTexts;
    s32                names[5] = {0, 0, 0, 0, 0};
    u16                i;

    OtosuMenu_ComputeSinglePlayerRanks(menuObj);
    for (i = 0; i < menuObj->matchPlayerCount; i++) {
        names[i] = OtosuMenu_GetPlaceName(menuObj, i);
    }
    names[i] = 0;
    OtosuMenu_SetupResultRankingScreens(menuObj, 0xE, 0xD, names, texts.rects);
    OtosuMenu_Ranking_CreateRankBoards(menuObj);
    menuObj->unk_474C8 = 0xFFFF;
    menuObj->board     = 0xFFFF;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: instruction selection differs
static PrcStepResult OtosuMenu_Ranking_Step_SinglePlayerWaitExit(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    u16                cursors[3] = {5, 0x48, 0x52};
    OtosuMenuRectList2 buttons    = OtosuMenu_Ranking_SinglePlayerButtons;
    s32                cursor;
    u16                touched;

    touched = OtosuMenu_GetTouchedButton(buttons.rects);
    if (touched == 0xFFFF) {
        return 0;
    }
    if (touched == 0xFFFE) {
        menuObj->board       = 0xFFFFU;
        menuObj->cursor.posX = 0U;
        menuObj->cursor.posY = 0xD2U;
        return PRC_STEP_CONTINUE;
    }
    if (menuObj->board == touched) {
        if (touched == 0) {
            SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
            PrcCtx_AdvanceStep(ctx);
        }
    } else {
        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CURSOR);
        cursor               = touched * 3;
        menuObj->board       = touched;
        menuObj->cursor.posX = cursors[cursor + 1];
        menuObj->cursor.posY = cursors[cursor + 2];
        Sprite_ChangeAnimation(&menuObj->cursor, menuObj->cursor.animData, (s16)cursors[cursor], menuObj->cursor.cellTable);
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Ranking_Step_ShutdownWireless(PrcCtx* ctx, void* object) {
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
    return 0;
}

static PrcStepResult OtosuMenu_Ranking_Step_LeaveSession(PrcCtx* ctx, void* object) {
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

static PrcStepResult OtosuMenu_Ranking_Step_CheckGuests(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    if (menuObj->unk_474CA != func_ov040_0209cb68()) {
        PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_CommError_ConnectionLostFrameDesc, NULL);
        return 0;
    }
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_REPEAT;
}

// Nonmatching: scheduling and register allocation
static PrcStepResult OtosuMenu_Ranking_Step_SendExit(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    if (func_ov040_0209cb68() == 1) {
        PrcCtx_AdvanceStep(ctx);
        return PRC_STEP_CONTINUE;
    }
    u32 irqState2                  = OS_DisableIRQ();
    OtosuMenu_HostPacket.syncState = 3;
    OS_RestoreIRQ(irqState2);
    if (func_ov040_0209d48c(&OtosuMenu_HostPacket, 0xA, OtosuMenu_Ranking_OnGuestSendDone, menuObj) != 0) {
        return PRC_STEP_CONTINUE;
    }
    PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_CommError_ConnectionLostFrameDesc, NULL);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Ranking_Step_ShutdownHost(PrcCtx* ctx, void* object) {
    u32 state;

    state = func_ov040_0209cb78();
    switch (state) {
        case 0:
            PrcCtx_AdvanceStep(ctx);
            break;
        case 1:
            func_ov040_0209d6cc();
            break;
        case 3:
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

static PrcStepResult OtosuMenu_Ranking_Step_StopHost(PrcCtx* ctx, void* object) {
    u32 state;

    state = func_ov040_0209cb78();
    switch (state) {
        case 0:
        case 3:
            break;
        case 1:
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
