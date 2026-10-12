#include "Otosu/OtosuGame.h"

static void OtuScene_GuestBoard_Enter(TinPinSlammer_Scene* scene);
static void OtuScene_GuestBoard_Exit(TinPinSlammer_Scene* scene);

static u8 OtuBoard_DeckRecvBuf[0x10];

s32 OtuPhys_SpinDecay     = 10;
s32 OtuPhys_SpinPower     = 1000;
s32 OtuPhys_StunnedImpact = 0x3000;
s32 OtuPhys_Knockback     = 0x6000;
s32 OtuPhys_HandDashSpeed = -0x4000;

s32 OtuPhys_Friction[4] = {0x1800, 0xB33, 0x3000, 0x4CD};

OtuSceneStage OtuScene_HostBoard = {
    OtuScene_HostBoard_Enter,
    OtuScene_HostBoard_Exit,
    OtuScene_HostBoardSteps,
    sizeof(OtuBoardStage),
};

OtuSceneStage OtuScene_GuestBoard = {
    OtuScene_GuestBoard_Enter,
    OtuScene_GuestBoard_Exit,
    OtuScene_GuestBoardSteps,
    sizeof(OtuBoardStage),
};

OtuSceneStage OtuScene_SinglePlayerBoard = {
    OtuScene_SinglePlayerBoard_Enter,
    OtuScene_SinglePlayerBoard_Exit,
    OtuScene_SinglePlayerBoardSteps,
    sizeof(OtuBoardStage),
};

OtuPinTune OtuPinTunes[12] = {
    {0x8000, 0x148, 0x99A, 0x14CD},
    {0x7AB8, 0x171, 0x948, 0x1614},
    {0x7548, 0x19A, 0x91F, 0x175C},
    {0x7000, 0x19A, 0x8CD, 0x187B},
    {0x6AB8, 0x1C3, 0x87B, 0x19C3},
    {0x6548, 0x1EC, 0x829, 0x1B0A},
    {0x6000, 0x214, 0x800, 0x1C52},
    {0x5AB8, 0x214, 0x7AE, 0x1D9A},
    {0x5548, 0x23D, 0x75C, 0x1EB8},
    {0x5000, 0x266, 0x733, 0x2000},
    {0x5000, 0x4CD, 0x4CD, 0x3000},
    {0x2800, 0x266, 0x4CD, 0x7000},
};

void func_ov040_0209edc0();
void func_ov040_0209eef4();
void func_ov040_0209ef48();
void func_ov040_0209ef6c();

u16  func_ov040_0209cb68(void);
s32  func_ov040_0209cde4(void);
void func_ov040_0209caac(s32 arg0);
void func_ov040_0209cb9c(void);
void func_ov040_0209cb08(void* state, s32 arg1);
void func_ov040_0209cabc(void* state, s32 arg1);
void func_ov040_0209c158(void);
void func_ov040_0209ba04(void (*cb)(WMBssDesc*, TinPinSlammer_Scene*), void* scene, void* bssid, s32 arg3);
void func_ov040_0209d0a8(s32 arg0, s32 arg1, s32 arg2);
void func_ov040_0209d290(s32 arg0, WMBssDesc* parent);
void func_ov040_0209d40c(void (*cb)(), void* arg);
void func_ov040_0209d420(void (*cb)(), void* arg);
void func_ov040_0209d818(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void func_ov040_0209d848(s32 arg0);
void func_ov040_0209ece8(void* buf, s32 len);
void func_ov040_0209ec5c(void (*cb)(TinPinSlammer_Scene*, s32, const void*, s32), void* scene, void* buf, s32 len);
void func_ov040_0209ed58(s32 arg0);
void func_ov040_0209ef88(void);
void func_ov040_0209efc0(void);
s32  func_020442f8(void);
void func_020471ec(s32 a0, s32 a1, void* arg);
void func_020472a8(void* arg);
void func_0204737c(void);

void OtuScene_HostBoard_StartHost(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    if (stage->timer > 0) {
        stage->timer = stage->timer - 1;
    } else {
        scene->linkTimeout = 1;
    }

    switch (func_ov040_0209cb78()) {
        case 1: {
            func_ov040_0209d990();
            func_ov040_0209caac(0x400548);
            func_ov040_0209cb9c();
        } break;

        case 7: {
            s32 a = func_ov040_0209cde4();
            s32 b = func_020442f8();

            func_ov040_0209d818(3, 6, 0x10, 2, 0x10, 2);
            func_ov040_0209d848(3);

            func_ov040_0209d40c((void (*)())OtuBoard_OnGuestConnected, scene);
            func_ov040_0209d420((void (*)())OtuBoard_OnGuestDisconnected, scene);

            stage->packetKind = 1;
            func_ov040_0209cb08(&stage->packetKind, 2);
            func_ov040_0209d0a8(4, b, a);
        } break;

        case 5: {
            if (stage->badgeCount == func_02047e84(func_ov040_0209cb68())) {
                stage->step = 0;
                OtuStageDispatch_NextStep(&scene->stage);
            }
        } break;
    }
}

// Nonmatching: control flow differs
void OtuScene_HostBoard_SyncDecks(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    s32            i;

    switch (stage->step) {
        case 0: {
            OtuBoard_SetSyncStep(scene, 1);
        } break;

        case 1: {
            OtuScene_SyncStep = 1;

            i = 0;
            while (i < stage->badgeCount) {
                if (OtuScene_GetPlayerInput(i)->syncStep != 1) {
                    break;
                }
                i++;
            }
            if (i == stage->badgeCount) {
                OtuBoard_SetSyncStep(scene, 2);
            }
        } break;

        case 2: {
            stage->readyMask = 1;
            func_020471ec(0x10, 0x10, func_ov040_0209edc0);
            func_ov040_0209d970(func_ov040_0209eef4);
            func_ov040_0209d434(func_ov040_0209ef48, scene);
            func_ov040_0209ec5c(OtuBoard_ReceiveDeck, scene, OtuBoard_DeckRecvBuf, 0x10);
            scene->linkOpen = 1;
            func_ov040_0209ef88();

            u16 mask = 0;
            for (i = 1; i < stage->badgeCount; i++) {
                mask |= 1 << i;
            }
            func_ov040_0209ed58(mask);
            OtuBoard_SetSyncStep(scene, 3);
        } break;

        case 3: {
            if (stage->badgeCount == func_02047e84(stage->readyMask)) {
                func_ov040_0209ece8(scene->decks, 0x38);
                OtuBoard_SetSyncStep(scene, 4);
            }
        } break;

        case 4: {
            OtuScene_SyncStep = 2;
            for (i = 0; i < stage->badgeCount; i++) {
                if (OtuScene_GetPlayerInput(i)->syncStep != 2) {
                    break;
                }
            }
            if (i == stage->badgeCount) {
                OtuBoard_SetSyncStep(scene, 6);
            }
        } break;

        case 6: {
            OtuScene_SyncStep = 3;
            for (i = 0; i < stage->badgeCount; i++) {
                if (OtuScene_GetPlayerInput(i)->syncStep != 3) {
                    break;
                }
            }
            if (i == stage->badgeCount) {
                OtuBoard_SetSyncStep(scene, 7);
            }
        } break;

        case 7: {
            EasyFade_FadeMainDisplay(FADER_SMOOTH, 0x10, 0x1000);
            EasyFade_FadeSubDisplay(FADER_SMOOTH, 0x10, 0x1000);
            OtuBoard_SetSyncStep(scene, 8);
        } break;

        case 8: {
            func_ov040_0209ed30();
            func_ov040_0209ece4();
            func_ov040_0209ed20();
            func_02047338();
            func_ov040_0209d970(0);
            func_ov040_0209d434(0, 0);
            scene->linkOpen = 0;
            EasyTask_DeleteTask(&scene->pool1, stage->wrwaitId);
            OtuStageDispatch_NextStep(&scene->stage);
        } break;
    }
}

void OtuScene_HostBoard_SetupBoard(TinPinSlammer_Scene* scene) {
    OtuStageDispatch_GetBlock(&scene->stage);
    OtuBoard_SetupMultiplayer(scene);
    OtuStageDispatch_NextStep(&scene->stage);
}

void OtuScene_HostBoard_WaitStart(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = (OtuBoardStage*)OtuStageDispatch_GetBlock(&scene->stage);

    OtuScene_SyncStep = 4;

    s32 i;
    for (i = 0; i < stage->badgeCount; i++) {
        if (OtuScene_GetPlayerInput(i)->syncStep != 4) {
            break;
        }
    }

    if (i == stage->badgeCount) {
        scene->playing = 1;
        CriSndMgr_PlayFile(0x18);
        EasyFade_FadeBothDisplays(3, 0, 0x1E);
        OtuStageDispatch_NextStep(&scene->stage);
    }
}

void OtuScene_HostBoard_Play(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    if (scene->playing == 0) {
        if (OtosuGame_gameover_IsActive(EasyTask_GetTaskData(&scene->pool1, stage->gameoverId)) == 0) {
            func_ov040_0209d420(NULL, NULL);
            OtuStageDispatch_NextStep(&scene->stage);
        }
    }
    OtuBoard_Update(scene);
}

void OtuScene_HostBoard_Disconnect(TinPinSlammer_Scene* scene) {
    switch (func_ov040_0209cb78()) {
        case 5: {
            if (func_ov040_0209cb68() == 1) {
                func_ov040_0209d588();
            }
        } break;

        case 1: {
            OtuStageDispatch_SetNext(&scene->stage, NULL);
        } break;
    }
}

static void OtuScene_GuestBoard_Enter(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    stage->gotPacket  = 0;
    stage->timer      = 600;
    stage->badgeCount = scene->playerCount;

    stage->wriconId = OtosuGame_wricon_CreateTask(&scene->pool1, scene->spareDataType);
    stage->wrwaitId = OtosuGame_wrwait_CreateTask(&scene->pool1, scene->spareDataType);
}

static void OtuScene_GuestBoard_Exit(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    EasyTask_DeleteTask(&scene->pool1, stage->wriconId);
    OtuBoard_Teardown(scene);
}

void OtuScene_GuestBoard_ConnectToHost(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    if (stage->timer > 0) {
        stage->timer--;
    } else {
        scene->linkTimeout = 1;
    }

    switch (func_ov040_0209cb78()) {
        case 1: {
            if (stage->gotPacket == 0) {
                func_ov040_0209d990();
                func_ov040_0209caac(0x400548);
                func_ov040_0209d848(3);
                stage->packetKind = gSaveData.otosuGameKey;
                func_ov040_0209ba04(OtuBoard_OnScanResult, scene, scene->parentBssid, 0);
            } else {
                func_ov040_0209d818(3, 6, 0x10, 2, 0x10, 2);
                func_ov040_0209cabc(&stage->packetKind, 2);
                func_ov040_0209d290(5, &stage->parent);
            }
        } break;

        case 2: {
            if (stage->gotPacket) {
                func_ov040_0209c158();
            }
        } break;

        case 5: {
            stage->step = 0;
            OtuStageDispatch_NextStep(&scene->stage);
        } break;
    }
}

// Nonmatching: control flow differs
void OtuScene_GuestBoard_SyncDecks(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    switch (stage->step) {
        case 0: {
            OtuBoard_SetSyncStep(scene, 1);
        } break;

        case 1: {
            OtuScene_SyncStep = 1;
            s32 i;
            for (i = 0; i < stage->badgeCount; i++) {
                if (OtuScene_GetPlayerInput(i)->syncStep != 1) {
                    break;
                }
            }
            OtuBoard_SetSyncStep(scene, 2);
        } break;

        case 2: {
            stage->readyMask = 0;
            func_020472a8(func_ov040_0209edc0);
            func_ov040_0209d728();
            func_0204737c();
            func_ov040_0209d970(func_ov040_0209eef4);
            func_ov040_0209d434(func_ov040_0209ef6c, scene);
            func_ov040_0209ec5c(OtuBoard_ReceiveDeck, scene, OtuBoard_DeckRecvBuf, 0x10);
            func_ov040_0209ece8(scene->decks, 14);
            scene->linkOpen = 1;
            func_ov040_0209efc0();
            OtuBoard_SetSyncStep(scene, 4);
        } break;

        case 4: {
            OtuScene_SyncStep = 2;
            s32 i;
            for (i = 0; i < stage->badgeCount; i++) {
                if (OtuScene_GetPlayerInput(i)->syncStep != 2) {
                    break;
                }
            }
            if (i == stage->badgeCount) {
                func_ov040_0209ed58(1);
                OtuBoard_SetSyncStep(scene, 5);
            }
        } break;

        case 5: {
            if (stage->readyMask != 0) {
                OtuBoard_SetSyncStep(scene, 6);
            }
        } break;

        case 6: {
            OtuScene_SyncStep = 3;
            s32 i;
            for (i = 0; i < stage->badgeCount; i++) {
                if (OtuScene_GetPlayerInput(i)->syncStep != 3) {
                    break;
                }
            }
            if (i == stage->badgeCount) {
                OtuBoard_SetSyncStep(scene, 7);
            }
        } break;

        case 7:
            EasyFade_FadeMainDisplay(FADER_SMOOTH, 0x10, 0x1000);
            EasyFade_FadeSubDisplay(FADER_SMOOTH, 0x10, 0x1000);
            OtuBoard_SetSyncStep(scene, 8);
            break;

        case 8:
            func_ov040_0209ed30();
            func_ov040_0209ece4();
            func_ov040_0209ed20();
            func_02047338();
            func_ov040_0209d970(0);
            func_ov040_0209d434(0, 0);
            scene->linkOpen = 0;
            EasyTask_DeleteTask(&scene->pool1, stage->wrwaitId);
            OtuStageDispatch_NextStep(&scene->stage);
            break;
    }
}

void OtuScene_GuestBoard_SetupBoard(TinPinSlammer_Scene* scene) {
    OtuStageDispatch_GetBlock(&scene->stage);
    OtuBoard_SetupMultiplayer(scene);
    OtuStageDispatch_NextStep(&scene->stage);
}

void OtuScene_GuestBoard_WaitStart(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    OtuScene_SyncStep = 4;

    s32 i;
    for (i = 0; i < stage->badgeCount; i++) {
        if (OtuScene_GetPlayerInput(i)->syncStep != 4) {
            break;
        }
    }

    if (i == stage->badgeCount) {
        scene->playing = 1;
        CriSndMgr_PlayFile(0x18);
        EasyFade_FadeBothDisplays(3, 0, 30);
        OtuStageDispatch_NextStep(&scene->stage);
    }
}

void OtuScene_GuestBoard_Play(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    if (scene->playing == FALSE) {
        if (OtosuGame_gameover_IsActive(EasyTask_GetTaskData(&scene->pool1, stage->gameoverId)) == FALSE) {
            OtuStageDispatch_NextStep(&scene->stage);
        }
    }
    OtuBoard_Update(scene);
}

void OtuScene_GuestBoard_Disconnect(TinPinSlammer_Scene* scene) {
    switch (func_ov040_0209cb78()) {
        case 5:
            func_ov040_0209d588();
            return;
        case 1:
            OtuStageDispatch_SetNext(&scene->stage, NULL);
            return;
    }
}
