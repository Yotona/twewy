#include "Field/FieldRegion.h"

#include "Display.h"
#include "EasyFade.h"
#include "Engine/Color.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Engine/Math/Sqrt.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/ObjResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Battle/FieldEncounter.h"
#include "Field/Event/EventFlags.h"
#include "Field/Event/EventManager.h"
#include "Field/Event/EventSpawnTemplates.h"
#include "Field/Event/EventTables.h"
#include "Field/FieldTypes.h"
#include "Field/Flash/FO3DViewer.h"
#include "Field/Scene/FieldAmbientSound.h"
#include "Field/Scene/FieldArea.h"
#include "Field/Scene/FieldBg.h"
#include "Field/Scene/FieldMain.h"
#include "Field/Scene/FieldMusic.h"
#include "Field/Scene/FieldSceneLoad.h"
#include "Field/Scene/FieldSceneRender.h"
#include "Field/Scene/FieldSeq.h"
#include "Field/Tasks/FldSpriteFrame.h"
#include "Field/Tasks/Fld_Badge.h"
#include "Field/Tasks/Fld_Brush.h"
#include "Field/Tasks/Fld_BustShot.h"
#include "Field/Tasks/Fld_CockPit.h"
#include "Field/Tasks/Fld_EventMail.h"
#include "Field/Tasks/Fld_Human.h"
#include "Field/Tasks/Fld_Luck.h"
#include "Field/Tasks/Fld_MapJumpArrow.h"
#include "Field/Tasks/Fld_NSymbolUGMgr.h"
#include "Field/Tasks/Fld_OnePicturCell.h"
#include "Field/Tasks/Fld_Partner.h"
#include "Field/Tasks/Fld_Phone.h"
#include "Field/Tasks/Fld_Piranha.h"
#include "Field/Tasks/Fld_Player.h"
#include "Field/Tasks/Fld_ScanArea.h"
#include "Field/Tasks/Fld_ScanFukidasi.h"
#include "Field/Tasks/Fld_Shinigami.h"
#include "Field/Tasks/Fld_TalkEventTitle.h"
#include "Field/Tasks/Fld_Touch.h"
#include "Field/Tasks/Fld_TouchCommand.h"
#include "Field/Tasks/Fld_TouchShine.h"
#include "Field/Tasks/Fld_Watch.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "SndMgr.h"
#include "common_data.h"
#include <nitro/fx.h>
#include <nitro/types.h>

extern s32  func_0202599c(s32 flagIndex);
extern void Field_SpawnMapJumpArrows(FieldEventManager* mgr);
extern void Field_DeleteMapJumpArrows(FieldEventManager* mgr);
extern void func_02001b44(s32 arg0, s32 arg1, void* arg2, s32 arg3);
extern s32 (*data_02065fd4)(s32 arg0, s32 arg1, s32 arg2);

extern BgResource* BgResMgr_AllocScreen(BgResMgr* mgr, void* source, s32 screenBase, s32 screenSize);
extern BgResource* BgResMgr_AllocChar32(BgResMgr* mgr, void* source, u32 charBase, u32 offset, u32 size);
extern BgResource* BgResMgr_AllocCharExtended(BgResMgr* mgr, void* source, s32 charBase, u32 arg3, u32 size);

void Field_ScriptUpdate(FieldEventManager* mgr);
void Field_ScanUpdate(FieldEventManager* mgr);
void Field_SpawnHuman(FieldEventManager* mgr, FieldPos pos, s32 level, FldSpawnTemplate tmpl, s32 routes, s16 routeCount,
                      s16 segment, s32 angle, FieldPos target);
void Field_SpawnStandingHuman(FieldEventManager* mgr, FieldPos pos, s32 level, FldSpawnTemplate tmpl, s32 angle);

void func_ov030_020aac84(FieldEventManager* mgr) {
    if (gSaveData.unk_24A0[gSaveData.unk_249C] == 72) {
        func_ov030_020a7b50(mgr);
    }
}

// Nonmatching: regalloc
BOOL Field_HasBustShotsOnBothSides(void) {
    FieldEventManager* mgr     = data_ov030_02109d60;
    BOOL               foundLo = FALSE;
    BOOL               foundHi = FALSE;
    s32                i;

    for (i = 0; i < 35; i++) {
        if (mgr->unk_21BBC[i] != -1) {
            if (Fld_BustShot_GetPosX(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_21BBC[i])) < 128) {
                foundLo = TRUE;
            } else {
                foundHi = TRUE;
            }
        }
    }
    if (!foundLo || !foundHi) {
        return FALSE;
    }
    return TRUE;
}

s32 func_ov030_020aad28(void) {
    return data_ov030_02109d60->unk_21CEC;
}

s32 func_ov030_020aad40(void) {
    FieldEventManager* mgr = data_ov030_02109d60;

    if (gSaveData.chapter >= 44) {
        gSaveData.chapter = 0;
    }
    return data_ov030_020da6c8[gSaveData.chapter](mgr);
}

u16 Field_GetMailEvent(void) {
    return gSaveData.mailEvent;
}

BOOL func_ov030_020aad94(FieldEventManager* mgr, s32* outY) {
    if (func_0202599c(1) && gSaveData.chapter != 21) {
        s32 group = Field_GetChapterGroup();

        if (group == 3) {
            return FALSE;
        }
        if (Fld_Partner_IsTalkTouched(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Partner))) {
            mgr->unk_22E14 = 30;
            if (RNG_Next(128) < 32U) {
                mgr->unk_22E18 = 1028;
            } else if (RNG_Next(128) < 64U) {
                mgr->unk_22E18 = 1029;
            } else if (RNG_Next(128) < 96U) {
                mgr->unk_22E18 = data_ov030_020d98ec[group][0];
            } else {
                mgr->unk_22E18 = data_ov030_020d98ec[group][1];
            }
            *outY = Fld_Partner_GetPosY(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Partner));
            return TRUE;
        }
    } else if (Fld_Player_IsTalkTouched(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Player))) {
        *outY = gSaveData.fieldPos.y;
        return TRUE;
    }
    return FALSE;
}

BOOL Field_CheckMailIconTouch(FieldEventManager* mgr) {
    TouchCoord coord;

    if (mgr->taskId_EventMail == -1) {
        return FALSE;
    }
    if (!TouchInput_WasTouchPressed()) {
        return FALSE;
    }
    TouchInput_GetCoord(&coord);
    if (coord.x >= 6 && coord.x <= 22 && coord.y >= 2 && coord.y <= 18) {
        Fld_EventMail_Press(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_EventMail));
        return TRUE;
    }
    return FALSE;
}

s32 Field_CheckChapterEvent(FieldEventManager* mgr) {
    if (gSaveData.chapter >= 44) {
        gSaveData.chapter = 0;
    }
    return FieldChapterEventChecks[gSaveData.chapter](mgr);
}

BOOL Field_CheckEventTriggers(FieldEventManager* mgr) {
    s32  hookY;
    s32  npcIndex;
    BOOL wantHook = FALSE;

    if (func_ov030_020aad94(mgr, &hookY)) {
        FldEvent_BeginScript(mgr, func_ov030_020aad40(), 0);
        wantHook = TRUE;
    }
    npcIndex = FieldBg_GetTouchedHuman(mgr);
    if (npcIndex != -1) {
        if (wantHook && func_ov030_020b5d4c(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Humans[npcIndex])) < hookY) {
            return TRUE;
        }
        mgr->unk_22E14     = 0;
        gSaveData.unk_2498 = 0;
        FldEvent_BeginScript(mgr, func_ov030_020b5cd8(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Humans[npcIndex])),
                             npcIndex);
        return TRUE;
    }
    if (wantHook) {
        return TRUE;
    }
    if (Field_CheckMailIconTouch(mgr)) {
        FldEvent_BeginScript(mgr, Field_GetMailEvent(), 0);
        func_ov030_020aec38(10);
        return TRUE;
    }
    return FALSE;
}

BOOL Field_CheckExit(FieldEventManager* mgr) {
    s32 blocked;

    mgr->activeExit = func_ov030_020b8358(gSaveData.fieldArea, mgr->origin, mgr->unk_21AC0, &blocked);
    if (mgr->activeExit != NULL) {
        if (func_0202599c(7)) {
            return TRUE;
        }
        mgr->unk_21AC0 = 0;
        return TRUE;
    }
    if (blocked == 0) {
        mgr->unk_21AC0 = 1;
    }
    return FALSE;
}

void Field_PlayNekuVoice(void) {
    u16 r = RNG_Next(1024);

    if (r < 128U) {
        SndMgr_StartPlayingSE(SEIDX_NEKU_FLD_01);
    } else if (r < 256U) {
        SndMgr_StartPlayingSE(SEIDX_NEKU_FLD_02);
    }
}

void Field_BeginExitTransition(FieldEventManager* mgr) {
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepLeaveArea, mgr, 0);
    mgr->blendTimer = 30;
    mgr->blendEva   = 0;
}

void Field_PlayElevatorAnim(FieldEventManager* mgr) {
    s16  area   = gSaveData.fieldArea;
    BOOL played = FALSE;

    if (area >= 24 && area < 37) {
        if (gSaveData.fieldPos.x < I2F(244)) {
            FO3DViewer_RestartAnim("mk001_b00a4");
            played = TRUE;
        } else if (gSaveData.fieldPos.x < I2F(455)) {
            FO3DViewer_RestartAnim("mk001_c00a4");
            played = TRUE;
        }
    }
    if (gSaveData.fieldArea >= 26 && gSaveData.fieldArea < 37 && gSaveData.fieldPos.x >= I2F(455)) {
        FO3DViewer_RestartAnim("mk001_d00a4");
        played = TRUE;
    }
    if (played) {
        SndMgr_PlaySEWithPan(SEIDX_BG_CITY_ELE_OPEN, F2I(gSaveData.fieldPos.x - mgr->origin.x));
        mgr->unk_21AAC = 81;
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepWaitThenBlend, mgr, 0);
    }
}

BOOL Field_CheckPhoneMenuInput(FieldEventManager* mgr) {
    TouchCoord coord;

    if (!Inventory_HasRequiredQuantity(ITEM_STICKER_PHONE_MENU, 1, 0)) {
        return FALSE;
    }
    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_START) {
        return TRUE;
    }
    if (!TouchInput_WasTouchPressed()) {
        return FALSE;
    }
    TouchInput_GetCoord(&coord);
    if (coord.x >= 238 && coord.y <= 26) {
        Fld_Phone_Press(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Phone));
        SndMgr_StartPlayingSE(SEIDX_MENU_MENTER);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov030_020ab3b8(s32 arg0) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (arg0 == data_ov030_020d95c4[i][0]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov030_020ab3ec(s32 arg0) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (arg0 == data_ov030_020d95c4[i][1]) {
            return TRUE;
        }
    }
    return FALSE;
}

u16 func_ov030_020ab420(s32 arg0) {
    s32 i = 0;

    while (TRUE) {
        if (arg0 == data_ov030_020d95c4[i][0]) {
            return data_ov030_020d95c4[i][1];
        }
        i++;
    }
}

u16 func_ov030_020ab454(void) {
    u8  chapter = gSaveData.chapter;
    s32 index;

    if (chapter == 15) {
        index = 0;
    } else if (chapter == 16 || chapter == 17) {
        index = 1;
    } else {
        index = 2;
    }
    return data_ov030_020d95c4[index][0];
}

BOOL func_ov030_020ab494(FieldEventManager* mgr) {
    s16 r = RNG_Next(100);

    if (func_0202599c(7)) {
        return FALSE;
    }
    s16 area = gSaveData.fieldArea;
    if (area == 23) {
        return FALSE;
    }
    if (area == 41) {
        return FALSE;
    }
    if (area == 42) {
        return FALSE;
    }
    if (area == 43) {
        return FALSE;
    }
    if (area == 37) {
        return FALSE;
    }
    if (r >= gSaveData.unk_3120) {
        return FALSE;
    }
    func_ov030_020aec1c(7);
    gSaveData.unk_2498 = 0;
    FldEvent_BeginScript(mgr, func_ov030_020ab454(), 0);
    return TRUE;
}

void Field_MainUpdate(FieldEventManager* mgr) {
    func_ov030_020c2d50(gSaveData.areaPopulation[gSaveData.fieldArea]);
    if (mgr->taskId_EventMail == -1 && Field_GetMailEvent() != 0 && gSaveData.fieldArea != 37) {
        mgr->taskId_EventMail = Fld_EventMail_CreateTask(&mgr->taskPool, mgr->dataType);
    }
    if (mgr->unk_21AF4 != 0) {
        mgr->unk_21AAC = mgr->unk_21AF4;
        mgr->unk_21AF4 = 0;
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepWait, mgr, 0);
        return;
    }
    if (gSaveData.fieldArea != 23 && gSaveData.fieldArea != 37) {
        func_ov030_020aeaa0(gSaveData.fieldArea, 1);
    }
    mgr->unk_21AF0 = 0;
    mgr->unk_21AC8 = 0;
    if (Field_CheckChapterEvent(mgr)) {
        DebugOvlDisp_Pop();
        DebugOvlDisp_Push((OverlayCB)Field_ScriptUpdate, mgr, 0);
    } else if (mgr->exitRequest != 4) {
        if (mgr->unk_21AD0 != 0) {
            mgr->unk_21AD0  = 0;
            mgr->activeExit = &mgr->unk_21AF8;
            Field_BeginExitTransition(mgr);
        } else if (FieldBg_IsBadgeTapped(mgr)) {
            func_ov030_020c3360();
            mgr->scanActive = 1;
            mgr->scanPhase  = 0;
            mgr->unk_21B18  = 0;
            mgr->scanTimer  = 25;
            mgr->unk_21B20  = -1;
            SndMgr_StartPlayingSE(SEIDX_SE_SCANL_OPEN);
            mgr->unk_21B24           = EasyTask_CreateTask(&mgr->taskPool, &Tsk_Fld_ScanFukidasi, NULL, 0, NULL, NULL);
            mgr->taskId_NSymbolUGMgr = Fld_NSymbolUGMgr_CreateTask(&mgr->taskPool, mgr->dataType);
            mgr->chainedEncounters   = 0;
            Fld_Luck_SpawnSymbols(&mgr->taskPool, mgr->taskId_NSymbolUGMgr, &mgr->origin);
            if (mgr->unk_21700 != -1) {
                EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21700);
                mgr->unk_21700 = -1;
            }
            Fld_ScanArea_Open(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_ScanArea));
            EasyFade_FadeSubDisplay(FADER_SMOOTH, 16, 0);
            g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG0;
            mgr->scanSlideY = I2F(64);
            Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG0, 0, mgr->scanSlideY);
            DebugOvlDisp_Pop();
            DebugOvlDisp_Push((OverlayCB)Field_ScanUpdate, mgr, 0);
        } else if (Field_CheckEventTriggers(mgr)) {
            DebugOvlDisp_Pop();
            DebugOvlDisp_Push((OverlayCB)Field_ScriptUpdate, mgr, 0);
        } else if (Field_CheckExit(mgr)) {
            if (mgr->activeExit->destArea >= 70) {
                DebugOvlDisp_Pop();
                mgr->exitRequest = 5;
            } else if (func_ov030_020ab494(mgr)) {
                DebugOvlDisp_Pop();
                DebugOvlDisp_Push((OverlayCB)Field_ScriptUpdate, mgr, 0);
            } else {
                func_ov030_020aec38(7);
                Field_BeginExitTransition(mgr);
                Field_PlayElevatorAnim(mgr);
            }
        } else if (Field_CheckPhoneMenuInput(mgr)) {
            DebugOvlDisp_Pop();
            mgr->exitRequest = 1;
        } else {
            FieldBg_ResetScanSeen(mgr);
            mgr->unk_21AD4 = 1;
        }
    }
}

void Field_ScriptUpdate(FieldEventManager* mgr) {
    func_ov030_020a7b50(mgr);
    if (mgr->unk_21ABC != 0) {
        mgr->exitRequest = 2;
        SndMgr_StartPlayingSE(SEIDX_SE_BATTLE_IN);
        DebugOvlDisp_Pop();
    } else if (gSaveData.unk_2498 == 0) {
        if (mgr->unk_21AD0 != 0) {
            mgr->unk_21AD0  = 0;
            mgr->activeExit = &mgr->unk_21AF8;
            Field_BeginExitTransition(mgr);
        } else {
            DebugOvlDisp_Pop();
            DebugOvlDisp_Push((OverlayCB)Field_MainUpdate, mgr, 0);
            func_ov030_020c2de8();
            Field_SpawnMapJumpArrows(mgr);
            mgr->unk_21AC0 = 0;
        }
    }
}

void Field_ScanUpdate(FieldEventManager* mgr) {
    FieldScan_Update(mgr);
    if (mgr->scanActive == 0) {
        DebugOvlDisp_Pop();
        DebugOvlDisp_Push((OverlayCB)Field_MainUpdate, mgr, 0);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepWaitFade, mgr, 0);
        func_ov030_020c2de8();
    }
}

void Field_InitState(FieldEventManager* mgr) {
    mgr->unk_2162C        = 0;
    mgr->unk_21618        = 0;
    mgr->unk_21AC0        = 0;
    mgr->unk_21AD4        = 0;
    gSaveData.unk_245A    = 1;
    mgr->scanPhase        = 5;
    mgr->unk_21AC8        = 1;
    mgr->unk_21A5C        = 0;
    mgr->talkEventActive  = 0;
    mgr->unk_21AD0        = 0;
    mgr->unk_21ACC        = 1;
    mgr->unk_216C8        = -1;
    mgr->unk_21A90        = -1;
    mgr->unk_21A94        = -1;
    mgr->unk_21A6C        = 0;
    mgr->unk_21A78        = 0;
    mgr->eventPicScreen   = 0;
    mgr->taskId_UGFade    = -1;
    mgr->unk_21700        = -1;
    mgr->unk_216E0        = -1;
    mgr->unk_216E8        = -1;
    mgr->taskId_Raster    = -1;
    mgr->taskId_EventMail = -1;
    mgr->unk_21A24        = -1;
    mgr->taskId_Choice    = -1;
    Fld_TouchShine_SetActiveCount(0);
    mgr->unk_21AF0         = 0;
    mgr->unk_21AF4         = 0;
    mgr->exitRequest       = 0;
    mgr->touchEventKind    = 0;
    mgr->blendTimer        = 0;
    mgr->unk_21AAC         = 0;
    mgr->blendEva          = 0;
    mgr->unk_21620         = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldDownScreen, 6, FALSE);
    mgr->unk_22E34         = 0;
    mgr->scanActive        = 0;
    mgr->mapJumpArrowCount = 0;
    mgr->eventPictureId    = 0;
    mgr->unk_22E14         = 0;
    mgr->touchEventResult  = 0;
    mgr->unk_21ABC         = 0;
    mgr->drawnHumanCount   = 0;
    mgr->unk_22E2C         = 3;
    mgr->lightLevel        = 31;
    FldEvent_InitState(mgr);
    mgr->unk_21AC4 = 0;
    mgr->unk_21A28 = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_NpcCeparate, 74, FALSE);
    FieldBg_ResetScanSeen(mgr);
    func_ov030_020aec1c(3);
    func_ov030_020aec1c(2);
}

void Field_ReleasePacks(FieldEventManager* mgr) {
    DatMgr_ReleaseData(mgr->unk_21A28);
    DatMgr_ReleaseData(mgr->unk_21620);
}

void func_ov030_020abbcc(FieldEventManager* mgr) {
    if (mgr->unk_21AC4 != 0) {
        func_ov030_020b7aac(&mgr->unk_2191C);
        ObjResMgr_ReleaseResource(g_ObjResourceManagers[2], mgr->unk_21A9C);
        mgr->unk_21AC4 = 0;
    }
}

void Field_DestroyHumans(FieldEventManager* mgr) {
    s32 i;

    EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_ScanArea);
    for (i = 0; i < mgr->humanCount; i++) {
        EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_Humans[i]);
    }
    mgr->humanCount = 0;
    if (gSaveData.fieldArea == 41) {
        for (i = 0; i < 64; i++) {
            EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21924[i]);
        }
    }
    func_ov030_020c6300();
    Field_DeleteMapJumpArrows(mgr);
    func_ov030_020abbcc(mgr);
}

s16 FieldEncounter_GetRankBaseLevel(s32 rank) {
    s32 i;

    for (i = 0; i < 12; i++) {
        if (rank == FieldRankLevelRanges[i][0]) {
            break;
        }
    }
    return FieldRankLevelRanges[i][1];
}

s16 FieldEncounter_GetRankForLevel(s32 level) {
    s32 i;

    for (i = 0; i < 12; i++) {
        s16 lo = FieldRankLevelRanges[i][1];

        if (lo <= level && level < lo + 12) {
            return FieldRankLevelRanges[i][0];
        }
    }
    return FieldRankLevelRanges[i][0];
}

s32 Field_RollHumanLevel(s32 index) {
    s32 place = 14;
    s32 brand = Field_GetCurrentTrendZone();

    if (brand >= 0) {
        u16 roll = RNG_Next(1000) / 10;

        if (roll < 70) {
            place = gSaveData.brandTrends[brand].ranking[0];
        } else if (roll < 90) {
            place = gSaveData.brandTrends[brand].ranking[1];
        }
    }
    if (place < 13) {
        return FieldEncounter_GetRankBaseLevel(place) + RNG_Next(12000) / 1000;
    }
    s32 level = RNG_Next(168);
    if (level >= 162) {
        level -= 150;
    }
    return level;
}

BOOL FieldEncounter_UsesFirstRange(s32 level) {
    if (level < 6) {
        return TRUE;
    }
    if (level < 18) {
        return FALSE;
    }
    if (level < 162) {
        return (((level - 18) / 6) & 1) == 0;
    }
    return TRUE;
}

BOOL func_ov030_020abe64(u32 arg0) {
    if (arg0 >= 22894 && arg0 <= 23203) {
        return TRUE;
    }
    if (arg0 >= 23480) {
        return TRUE;
    }
    return FALSE;
}

u16 FieldEncounter_NextByArea(s32 level) {
    s16 area = gSaveData.fieldArea;
    u16 lo1  = FieldEncounterRangesByArea[area].lo1;
    u16 hi1  = FieldEncounterRangesByArea[area].hi1;
    u16 lo2  = FieldEncounterRangesByArea[area].lo2;
    u16 hi2  = FieldEncounterRangesByArea[area].hi2;

    if (FieldEncounter_UsesFirstRange(level)) {
        if (lo1 + gSaveData.encounterCursors[4] > hi1) {
            gSaveData.encounterCursors[4] = 0;
        }
        s16 v                         = gSaveData.encounterCursors[4];
        gSaveData.encounterCursors[4] = v + 2;
        return lo1 + v;
    }
    if (lo2 + gSaveData.encounterCursors[5] > hi2) {
        gSaveData.encounterCursors[5] = 0;
    }
    s16 v                         = gSaveData.encounterCursors[5];
    gSaveData.encounterCursors[5] = v + 2;
    return lo2 + v;
}

u16 FieldEncounter_NextByRank(s32 level) {
    s16 group = FieldEncounter_GetRankForLevel(level);
    u16 lo1   = FieldEncounterRangesByRank[group].lo1;
    u16 hi1   = FieldEncounterRangesByRank[group].hi1;
    u16 lo2   = FieldEncounterRangesByRank[group].lo2;
    u16 hi2   = FieldEncounterRangesByRank[group].hi2;

    if (FieldEncounter_UsesFirstRange(level)) {
        if (lo1 + gSaveData.encounterCursors[2] > hi1) {
            gSaveData.encounterCursors[2] = 0;
        }
        s16 v                         = gSaveData.encounterCursors[2];
        gSaveData.encounterCursors[2] = v + 2;
        return lo1 + v;
    }
    if (lo2 + gSaveData.encounterCursors[3] > hi2) {
        gSaveData.encounterCursors[3] = 0;
    }
    s16 v                         = gSaveData.encounterCursors[3];
    gSaveData.encounterCursors[3] = v + 2;
    return lo2 + v;
}

s32 Field_GetChapterGroup(void) {
    if (gSaveData.chapter <= 6) {
        return 0;
    }
    if (gSaveData.chapter <= 13) {
        return 1;
    }
    if (gSaveData.chapter <= 20) {
        return 2;
    }
    if (gSaveData.chapter == 21) {
        return 3;
    }
    if (gSaveData.chapter <= 28) {
        return 0;
    }
    if (gSaveData.chapter <= 35) {
        return 1;
    }
    return 2;
}

u16 FieldEncounter_NextByChapter(s32 level) {
    s32 group = Field_GetChapterGroup();
    u16 lo1   = FieldEncounterRangesByChapterGroup[group].lo1;
    u16 hi1   = FieldEncounterRangesByChapterGroup[group].hi1;
    u16 lo2   = FieldEncounterRangesByChapterGroup[group].lo2;
    u16 hi2   = FieldEncounterRangesByChapterGroup[group].hi2;

    if (FieldEncounter_UsesFirstRange(level)) {
        if (lo1 + gSaveData.encounterCursors[0] > hi1) {
            gSaveData.encounterCursors[0] = 0;
        }
        s16 v                         = gSaveData.encounterCursors[0];
        gSaveData.encounterCursors[0] = v + 2;
        return lo1 + v;
    }
    if (lo2 + gSaveData.encounterCursors[1] > hi2) {
        gSaveData.encounterCursors[1] = 0;
    }
    s16 v                         = gSaveData.encounterCursors[1];
    gSaveData.encounterCursors[1] = v + 2;
    return lo2 + v;
}

u16 FieldEncounter_RollForHuman(s32 level, s32 limit) {
    s32 count = (u16)(limit / 10);
    u16 v;

    if (count == 0) {
        count = 1;
    }
    if (gSaveData.unk_311E < count && RNG_Next(1000) < 100) {
        gSaveData.unk_311E++;
        if (RNG_Next(256) < 128) {
            v = FieldEncounter_NextByRank(level);
        } else {
            v = FieldEncounter_NextByArea(level);
        }
    } else {
        v = FieldEncounter_NextByChapter(level);
    }
    Field_AddEncounterHumanCount(1);
    return v;
}

void FieldEncounter_AssignToTemplate(s32 level, FldSpawnTemplate* tpl, s32 limit) {
    if (!func_0202599c(9) && Field_AddEncounterHumanCount(0) < limit && RNG_Next(256) < 64) {
        tpl->unk_10 = FieldEncounter_RollForHuman(level, limit);
        tpl->unk_0E = tpl->unk_10 + 1;
    } else {
        tpl->unk_10 = -2;
        tpl->unk_0E = -2;
    }
}

s32 func_ov030_020ac254(void) {
    if (gSaveData.chapter == 17) {
        return 20;
    }
    if (gSaveData.chapter == 18) {
        return 50;
    }
    if (gSaveData.chapter == 19) {
        return 80;
    }
    if (gSaveData.chapter == 20) {
        return 100;
    }
    return 0;
}

BOOL func_ov030_020ac294(void) {
    s32 chance = func_ov030_020ac254();

    if (chance == 0) {
        return FALSE;
    }
    return RNG_Next(100) < chance;
}

void Field_SpawnArea5Humans(FieldEventManager* mgr) {
    FldSpawnTemplate template = FieldDefaultSpawnTemplate;

    if (gSaveData.fieldArea == 5) {
        s32 count = 14;
        if (gSaveData.areaPopulation[gSaveData.fieldArea] <= 14) {
            count = gSaveData.areaPopulation[gSaveData.fieldArea];
        }
        count -= mgr->humanCount;

        for (s32 i = 0; i < count; i++) {
            template.pos.x  = I2F(FieldArea5HumanPlacements[i][0]);
            template.pos.y  = I2F(FieldArea5HumanPlacements[i][1]);
            template.unk_08 = FieldArea5HumanPlacements[i][2];
            if (template.unk_08 == -1) {
                template.unk_08 = FieldEncounter_GetRankBaseLevel(gSaveData.brandTrends[1].ranking[0]);
            }
            template.unk_08 += RNG_Next(300) / 100;
            template.unk_0A = FieldArea5HumanPlacements[i][3];
            if (func_ov030_020ac294()) {
                template.unk_0A = 3;
                template.unk_10 = -2;
                template.unk_0E = -2;
            } else {
                FieldEncounter_AssignToTemplate(template.unk_08, &template, 15);
            }
            Field_SpawnStandingHuman(mgr, template.pos, template.unk_08, template, 0);
        }
    }
}

// Nonmatching
void Field_SpawnRouteHumans(FieldEventManager* mgr, s32 percent) {
    FldHumanSpawnEntry* entry;
    FldSpawnTemplate    tpl = FieldDefaultSpawnTemplate;
    FldHumanSpawnEntry* e;
    FieldPos            pos;
    FieldPos            target;
    s16                 seg;
    s32                 sx;
    s32                 sy;
    s32                 i;
    s32                 dx;
    int                 none;
    s32                 dy;
    s32                 r;
    s32                 n = gSaveData.areaPopulation[gSaveData.fieldArea] * percent / 100 - mgr->humanCount;
    if (n < 0 || (u32)gSaveData.fieldArea >= 24) {
        return;
    }
    n                          = gSaveData.fieldArea;
    FldHumanSpawnEntry* routes = FieldAreaRoutes[gSaveData.fieldArea].routes;
    if (routes == NULL) {
        if (1) {
        }
        return;
    }
    s16 count = FieldAreaRoutes[n].count;
    n -= mgr->humanCount;
    for (i = 0; i < (n & 0xFF); i++) {
        seg = i % (count * 2);
        if (seg >= count) {
            seg = seg - (count * 2);
        }
        if (seg < 0) {
            e        = &routes[-1 - seg];
            sx       = e->x1 + RNG_Next(e->w1);
            sy       = e->y1 + RNG_Next(e->h1);
            target.x = e->x0 + RNG_Next(e->w0);
            target.y = e->y0 + RNG_Next(e->h0);
        } else {
            entry    = &routes[seg & 0xFFFFFFFFu];
            e        = entry;
            sx       = e->x0 + RNG_Next(e->w0);
            sy       = e->y0 + RNG_Next(e->h0);
            target.x = e->x1 + RNG_Next(e->w1);
            target.y = e->y1 + RNG_Next(e->h1);
        }
        dy = target.y - sy;
        dx = target.x - sx;
        target.y <<= FIXED_SHIFT;
        target.x <<= FIXED_SHIFT;
        r          = RNG_Next(Sqrti(dx * dx + dy * dy));
        s32 angle  = FX_Atan2Idx(I2F(dy), I2F(dx));
        pos.x      = r * FX_SinCosTable_[(angle >> 4) * 2 + 1] + I2F(sx);
        pos.y      = r * FX_SinCosTable_[(angle >> 4) * 2] + I2F(sy);
        s32 level  = Field_RollHumanLevel(i);
        tpl.unk_0A = 9;
        if (func_ov030_020ac294()) {
            none       = -2;
            tpl.unk_0A = 3;
            tpl.unk_10 = none;
            tpl.unk_0E = none;
            if (seg) {
            }
        } else {
            FieldEncounter_AssignToTemplate(level, &tpl, 15);
        }
        Field_SpawnHuman(mgr, pos, level, tpl, (s32)routes, count, seg, angle, target);
    }
}

// Nonmatching: regswaps
void Field_SpawnRandomHumans(FieldEventManager* mgr, s32 percent) {
    FldSpawnTemplate tpl = FieldDefaultSpawnTemplate;
    FieldPos         pos;
    s32              i;
    s32              j;
    s32              k = 0;
    u16              rx;
    func_ov030_020b7b40();
    s32 n = gSaveData.areaPopulation[gSaveData.fieldArea] * percent / 100 - mgr->humanCount;
    if (n < 0) {
        n = k;
    }
    for (i = 0; i < n; i++) {
        if (func_ov030_020ac294()) {
            if (++k >= 3) {
                n++;
                if (1) {
                }
                k = 0;
            }
            tpl.unk_0A = 3;
            tpl.unk_10 = -2;
            tpl.unk_0E = -2;
            pos.x      = I2F(RNG_Next(func_ov030_020b7bc4()));
            pos.y      = I2F(RNG_Next(func_ov030_020b7b20()));
            s32 lim;
            j = 0;
            while (TRUE) {
                lim = func_ov030_020b7c9c(pos.x, pos.y);
                if (pos.y >= lim) {
                    break;
                }
                pos.y = gSaveData.fieldPos.y + I2F(RNG_Next(192) - 96);
                if (++j >= 99) {
                    pos.y = lim + I2F(RNG_Next(192));
                    break;
                }
            }

        } else {
            tpl.unk_0A = 10;
            pos.x      = gSaveData.fieldPos.x + I2F((rx = RNG_Next(256)) - 128);
            pos.y      = gSaveData.fieldPos.y + I2F(RNG_Next(192) - 96);
            while (TRUE) {
                n = pos.y;
                if (n >= func_ov030_020b7c9c(pos.x, pos.y)) {
                    break;
                }
                pos.y = gSaveData.fieldPos.y + I2F(RNG_Next(192) - 96);
            }
        }
        s32 level = Field_RollHumanLevel(i);
        if (tpl.unk_0A != 3) {
            FieldEncounter_AssignToTemplate(level, &tpl, 6);
        }
        Field_SpawnStandingHuman(mgr, pos, level, tpl, RNG_Next(65536));
    }
}

void Field_SpawnHumans(FieldEventManager* mgr) {
    s32 count = 0;

    mgr->humanCount = 0;
    mgr->unk_21920  = 0;
    mgr->unk_21922  = 0;
    Field_SetEncounterHumanCount(0);

    const FldSpawnTemplate* tpl = func_ov030_020baa2c(&count);
    for (s32 i = 0; i < count; i++) {
        if (tpl[i].unk_18 == func_ov030_020aec08(tpl[i].unk_14)) {
            Field_SpawnStandingHuman(mgr, tpl[i].pos, tpl[i].unk_08, tpl[i], 0);
        }
    }
    Field_SpawnMapJumpArrows(mgr);

    u32 area = gSaveData.fieldArea;
    if (area < 24) {
        Field_SpawnArea5Humans(mgr);
        Field_SpawnRandomHumans(mgr, FieldHumanSpawnPercents[area][0]);
        Field_SpawnRouteHumans(mgr, FieldHumanSpawnPercents[area][1]);
    }
}

void Field_SpawnHuman(FieldEventManager* mgr, FieldPos pos, s32 level, FldSpawnTemplate tmpl, s32 routes, s16 routeCount,
                      s16 segment, s32 angle, FieldPos target) {
    if (mgr->canSpawnHumans != 0) {
        mgr->taskId_Humans[mgr->humanCount] =
            Fld_Human_CreateTask(&mgr->taskPool, &mgr->origin, pos, mgr->humanCount, mgr->dataType, tmpl, level,
                                 RNG_Next(819) + 819, routes, angle, routeCount, segment, target);
        if (mgr->taskId_Humans[mgr->humanCount] == -1) {
            mgr->canSpawnHumans = 0;
        }
        mgr->humanCount++;
        if (mgr->humanCount >= 100) {
            mgr->canSpawnHumans = 0;
        }
    }
}

void Field_SpawnStandingHuman(FieldEventManager* mgr, FieldPos pos, s32 level, FldSpawnTemplate tmpl, s32 angle) {
    FieldPos target = {0, 0};

    Field_SpawnHuman(mgr, pos, level, tmpl, 0, 0, 0, angle, target);
}

void Field_SpawnMapJumpArrows(FieldEventManager* mgr) {
    FldSpawnTemplate template = FieldDefaultSpawnTemplate;
    FieldPos pos;
    s32      kind;
    s32      ret;

    Field_DeleteMapJumpArrows(mgr);

    while ((ret = func_ov030_020b86bc(&pos, &template.unk_0A, &kind, mgr->mapJumpArrowCount)) != 0) {
        if (ret != 1) {
            if (kind == 37 && gSaveData.fieldArea == 24) {
                kind = func_ov030_020aeaf4();
            }
            mgr->taskId_MapJumpArrows[mgr->mapJumpArrowCount] =
                Fld_MapJumpArrow_CreateTask(&mgr->taskPool, &mgr->origin, pos, mgr->dataType, template, kind);
        } else {
            mgr->taskId_MapJumpArrows[mgr->mapJumpArrowCount] = -1;
        }
        mgr->mapJumpArrowCount++;
    }
}

void Field_DeleteMapJumpArrows(FieldEventManager* mgr) {
    for (s32 i = 0; i < mgr->mapJumpArrowCount; i++) {
        if (mgr->taskId_MapJumpArrows[i] != -1) {
            EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_MapJumpArrows[i]);
        }
    }
    mgr->mapJumpArrowCount = 0;
}

void func_ov030_020acd98(FieldEventManager* mgr, u32* arg1) {
    ObjResource* res = ObjResMgr_AllocResource(g_ObjResourceManagers[2], NULL, 1024, 0);

    mgr->unk_21A9C = res;
    mgr->unk_21AA0 = (u32)(res->bitmapIndex << 5);
    func_02001b44(1, mgr->unk_21AA0, Data_GetPackEntryData(mgr->unk_21D2C, 4), 1024);
    mgr->unk_21AA4 = *arg1;
    func_02001b44(2, *arg1, Data_GetPackEntryData(mgr->unk_21D2C, 3), 16);
    *arg1 += 16;
    mgr->unk_21AA8 = *arg1;
    func_02001b44(2, *arg1, Data_GetPackEntryData(mgr->unk_21D2C, 5), 16);
}

void Field_InitHumans(FieldEventManager* mgr) {
    u32 offset = 2048;

    func_ov030_020b7744(&mgr->unk_2191C, mgr->unk_21610);
    offset = (u32)(data_02065fd4(2048, 0, 1) << 16) >> 13;
    func_ov030_020acd98(mgr, &offset);
    mgr->canSpawnHumans = 1;
    mgr->unk_21AC4      = 1;
    mgr->unk_21AE8      = 0;
    mgr->unk_21AE4      = 0;
    mgr->unk_21AEC      = 0;
    mgr->taskId_ScanArea =
        Fld_ScanArea_CreateTask(&mgr->taskPool, &mgr->origin, &gSaveData.fieldPos, mgr->unk_21AA0, mgr->unk_21AA4);
    if (gSaveData.fieldArea == 41) {
        for (s32 i = 0; i < 64; i++) {
            mgr->unk_21924[i] = Fld_Piranha_CreateTask(&mgr->taskPool, mgr->unk_21610, &mgr->origin, i);
        }
    }
    gSaveData.unk_311E = 0;
    func_ov030_020c6150(&mgr->unk_21D30);
    mgr->humanCount = 0;
    Field_SpawnHumans(mgr);
}

void func_ov030_020acfc4(FieldEventManager* mgr) {
    s32 v = Fld_Luck_GetNormalSymbolPercent();
    s16 id;

    if (mgr->unk_216E0 != -1) {
        if (mgr->unk_216E8 != -1) {
            EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216E8);
        }
        if (v < 0) {
            id = 10;
        } else if (v < 30) {
            id = 7;
        } else if (v < 60) {
            id = 8;
        } else {
            id = 9;
        }
        mgr->unk_216E8 = Fld_CockPit_CreateTask(&mgr->taskPool, mgr->dataType, id);
    }
}

void Field_CreateHud(FieldEventManager* mgr) {
    if (mgr->unk_216E0 == -1 && gSaveData.fieldArea != 37) {
        s16 v1 = 6;
        if (Fld_Luck_IsScanEventAvailable()) {
            v1 = 5;
        }
        mgr->unk_216E0 = Fld_CockPit_CreateTask(&mgr->taskPool, mgr->dataType, v1);

        s16 v2 = 4;
        if (func_0202599c(10)) {
            v2 = 3;
        }
        mgr->unk_216E4 = Fld_CockPit_CreateTask(&mgr->taskPool, mgr->dataType, v2);

        func_ov030_020acfc4(mgr);
        mgr->unk_216EC = Fld_CockPit_CreateTask(&mgr->taskPool, mgr->dataType, (s16)(gSaveData.unk_2470 + 11));

        s16 v3 = 2;
        if (func_020236a0()) {
            v3 = 1;
        }
        mgr->unk_216F8 = Fld_CockPit_CreateTask(&mgr->taskPool, mgr->dataType, v3);

        mgr->taskId_Watch = Fld_Watch_CreateTask(&mgr->taskPool, mgr->dataType);
        mgr->taskId_Phone = Fld_Phone_CreateTask(&mgr->taskPool, mgr->dataType);
        if (mgr->taskId_EventMail == -1 && Field_GetMailEvent() != 0) {
            mgr->taskId_EventMail = Fld_EventMail_CreateTask(&mgr->taskPool, mgr->dataType);
        }
    }
}

void Field_DestroyHud(FieldEventManager* mgr) {
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216E0);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216E4);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216E8);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216EC);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216F8);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_Watch);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_Phone);
    mgr->unk_216E0 = -1;
    mgr->unk_216E8 = -1;
    if (mgr->taskId_EventMail != -1) {
        EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_EventMail);
        mgr->taskId_EventMail = -1;
    }
}

void Field_CreateObjects(FieldEventManager* mgr) {
    mgr->taskId_Player  = Fld_Player_CreateTask(&mgr->taskPool, &mgr->origin, &gSaveData.fieldPos);
    mgr->taskId_Partner = Fld_Partner_CreateTask(&mgr->taskPool, &mgr->origin, &gSaveData.fieldPos);
    FieldBg_UpdateWalkSpeed(mgr);

    mgr->unk_21A2C    = Data_GetPackEntryData(mgr->unk_21A28, 1);
    mgr->humanPalette = PaletteMgr_AcquireContiguous(g_PaletteManagers[2], mgr->unk_21A2C, 5, 1);
    Field_CreateHud(mgr);
    mgr->taskId_Badge       = Fld_Badge_CreateTask(&mgr->taskPool, mgr->dataType, 240, 176, 0, 0);
    mgr->taskId_KeywordMenu = -1;
    mgr->unk_216BC          = -1;
    mgr->unk_216C0          = -1;
    mgr->unk_216CC          = -1;
    if (mgr->unk_216C8 == -1 && mgr->talkEventActive != 0) {
        mgr->unk_216C8 = Fld_TalkEventTitle_CreateTask(&mgr->taskPool, mgr->dataType);
    }
    Field_InitHumans(mgr);
}

void Field_DestroyObjects(FieldEventManager* mgr) {
    Field_DestroyHumans(mgr);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_Player);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_Partner);
    PaletteMgr_ReleaseResource(g_PaletteManagers[2], mgr->humanPalette);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_Badge);
    mgr->taskId_Badge = -1;
    Field_DestroyHud(mgr);
    Field_ReleaseEventPicture(mgr);
}

void Field_ReleaseEventPicture(FieldEventManager* mgr) {
    if (mgr->eventPicScreen != NULL) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[0], mgr->eventPicScreen);
        BgResMgr_ReleaseChar(g_BgResourceManagers[0], mgr->eventPicChar);
        PaletteMgr_ReleaseResource(g_PaletteManagers[0], mgr->eventPicPalette);
        mgr->eventPicScreen = NULL;
    }
}

void Field_CloseEventPicture(FieldEventManager* mgr) {
    Field_ReleaseEventPicture(mgr);
    if (mgr->unk_21A90 != -1) {
        EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21A90);
        mgr->unk_21A90 = -1;
    }
    if (mgr->unk_21A94 != -1) {
        EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21A94);
        mgr->unk_21A94 = -1;
    }
}

void Field_LoadEventPicture(FieldEventManager* mgr) {
    s32 id = mgr->eventPictureId;
    s32 entryIndex;
    s32 binIndex;

    if (id == 0) {
        SndMgr_StartPlayingSE(SEIDX_EV_SE_BADGE);
    }
    if (id == 3 || id == 8) {
        SndMgr_StartPlayingSE(SEIDX_EV_SE_TIMER);
    }
    if (id == 8) {
        binIndex   = 0;
        entryIndex = 6;
    } else if (id < 5) {
        entryIndex = id + 3;
        binIndex   = 0;
    } else if (id < 9) {
        entryIndex = id - 4;
        binIndex   = 1;
    } else {
        entryIndex = id - 8;
        binIndex   = 2;
    }

    Data* pack;
    if (id == 148) {
        pack = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldTouch, 9, FALSE);
    } else {
        pack = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldEventPacks[binIndex], entryIndex, FALSE);
    }

    void* charData   = Data_GetPackEntryData(pack, 1);
    void* screenData = Data_GetPackEntryData(pack, 2);
    void* palData    = Data_GetPackEntryData(pack, 3);

    Field_ReleaseEventPicture(mgr);
    mgr->eventPicScreen = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], screenData,
                                               g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].screenBase,
                                               g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].screenSizeText);

    u32 charSize = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->eventPicChar    = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], charData,
                                                g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].charBase, 0, charSize);
    mgr->eventPicPalette = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], palData, 0, 0, 16);
    if (id == 3) {
        mgr->unk_21A90 = Fld_OnePicturCell_CreateTask(&mgr->taskPool, mgr->dataType, 0);
    } else if (id == 8) {
        mgr->unk_21A90 = Fld_OnePicturCell_CreateTask(&mgr->taskPool, mgr->dataType, 1);
    } else {
        mgr->unk_21A90 = -1;
    }
    DatMgr_ReleaseData(pack);
    Display_SetBlendLayers(DISPLAY_MAIN, 0x1, 0x3F);
    Display_SetBlendCoeffs(DISPLAY_MAIN, 10, 6);
}

void Field_LoadTouchEventBg(FieldEventManager* mgr) {
    s32 entryIndex = 8;
    if (mgr->touchEventKind == 3) {
        entryIndex = 9;
    } else if ((u16)(s16)(mgr->touchEventKind - 4) <= 1) {
        entryIndex = 10;
    }

    Data* pack          = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldTouch, entryIndex, FALSE);
    void* charData      = Data_GetPackEntryData(pack, 1);
    void* screenData    = Data_GetPackEntryData(pack, 2);
    void* palData       = Data_GetPackEntryData(pack, 3);
    mgr->eventPicScreen = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], screenData,
                                               g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].screenBase,
                                               g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].screenSizeText);

    u32 charSize = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->eventPicChar    = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], charData,
                                                g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].charBase, 0, charSize);
    mgr->eventPicPalette = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], palData, 0, 0, 16);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], mgr->eventPicPalette);
    DatMgr_ReleaseData(pack);
    Fld_TouchShine_SetActiveCount(0);

    if (mgr->touchEventKind == 3) {
        mgr->unk_21A90 = Fld_Shinigami_CreateTask(&mgr->taskPool, mgr->dataType);
        mgr->unk_21A94 = Fld_TouchCommand_CreateTaskWithMsg(&mgr->taskPool, mgr->dataType, mgr->unk_21D28, 0);
    } else if ((u16)(s16)(mgr->touchEventKind - 4) <= 1) {
        mgr->unk_21A90 = Fld_Brush_CreateTask(&mgr->taskPool, mgr->dataType);
    } else {
        mgr->unk_21A90 = Fld_Touch_CreateTask(&mgr->taskPool, mgr->dataType);
    }
}

// Nonmatching
void Field_SetupDisplay2D(FieldEventManager* mgr, s32 use256Colors) {
    GX_DisableBankForTex();
    GX_DisableBankForTexPltt();
    GX_DisableBankForBg();
    GX_DisableBankForObj();
    GX_DisableBankForObjExtPltt();
    GX_SetBankForBg(GX_VRAM_A);
    GX_SetBankForObj(GX_VRAM_B);
    GX_SetBankForObjExtPltt(GX_VRAM_F);
    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    Display_CommitSynced();
    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    s32 colorMode = use256Colors != 0 ? GX_BG_COLORS_256 : GX_BG_COLORS_16;
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, colorMode, 0, 1, 0, (colorMode << 7) | 4);
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendMode = 1;
    Display_SetBlendLayers(DISPLAY_MAIN, 0x1, 0x3F);
    OamMgr_InitEngine(0, 0);
    OamMgr_Reset(&g_OamMgr[0], 0, 0);
    Color_CopyRange(&data_02066aec, &data_0205a128, 0, 512, 1);
    OamMgr_InitEngine(0, 2);
    OamMgr_SetAffineCount(&g_OamMgr[2], 0);
    Color_CopyRange((u16*)&data_020672ec, &data_0205a128, 0, 512, 1);
    OamMgr_ResetCommandQueues(&g_OamMgr[0]);
    BgResMgr_Init(&mgr->resources.mainBgMgr, 0);
    ObjResMgr_Init(&mgr->resources.objResMgr0, 0);
    ObjResMgr_Init(&mgr->resources.objResMgr2, 2);
    PaletteMgr_Init(&mgr->resources.palMgr0, 0);
    PaletteMgr_Init(&mgr->resources.palMgr2, 2);
}

void Field_SetupDisplay3D(FieldEventManager* mgr) {
    GX_DisableBankForTex();
    GX_DisableBankForTexPltt();
    GX_DisableBankForBg();
    GX_DisableBankForObj();
    GX_DisableBankForObjExtPltt();
    GX_SetBankForTex(GX_VRAM_AB);
    GX_SetBankForTexPltt(GX_VRAM_G);
    GX_SetBankForBg(GX_VRAM_F);
    GX_SetBankForObj(GX_VRAM_E);
    Display_CommitSynced();
    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 6, 0, 0, 0x600);
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendMode = 1;
    Display_SetBlendLayers(DISPLAY_MAIN, 0x2, 0x33);
    OamMgr_InitEngine(0, 0);
    OamMgr_Reset(&g_OamMgr[0], 0, 0);
    Color_CopyRange(&data_02066aec, &data_0205a128, 0, 512, 1);
    OamMgr_InitEngine(0, 2);
    OamMgr_SetAffineCount(&g_OamMgr[2], 0);
    Color_CopyRange((u16*)&data_020672ec, &data_0205a128, 0, 512, 1);
    BgResMgr_Init(&mgr->resources.mainBgMgr, 0);
    ObjResMgr_Init(&mgr->resources.objResMgr0, 0);
    ObjResMgr_Init(&mgr->resources.objResMgr2, 2);
    PaletteMgr_Init(&mgr->resources.palMgr0, 0);
    PaletteMgr_Init(&mgr->resources.palMgr2, 2);
}

s16 func_ov030_020adc84(void) {
    s32 group = Field_GetChapterGroup();
    s32 offset;

    if (group == 0) {
        offset = 0;
        if (gSaveData.chapter == 5) {
            offset = 12;
        }
    } else if (group == 1) {
        offset = 4;
    } else if (group == 2) {
        offset = 8;
    } else {
        return 0;
    }
    return data_ov030_020d9924[offset + gSaveData.unk_2470];
}
