#include "Interface/Menu/Result.h"
#include "CriSndMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/HBlank.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Player/Inventory.h"
#include "Player/Inventory/Items.h"
#include "Player/Inventory/Pins.h"
#include "Player/Stats.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <nitro/fs/overlay.h>
#include <nitro/mi/cpumem.h>
#include <nitro/reg.h>

typedef struct {
    /* 0x00000 */ ResourceManager  unk_00000;
    /* 0x11580 */ ResourceManager* unk_11580;
    /* 0x11584 */ s32              unk_11584;
    /* 0x11588 */ s32              unk_11588;
    /* 0x1158C */ Heap             heap;
    /* 0x11598 */ u8               heapBuffer[0x10000];
    /* 0x21598 */ TaskPool         taskPool;
    /* 0x21618 */ s32              taskId_NumTime;
    /* 0x2161C */ s32              taskId_NumMaxHit;
    /* 0x21620 */ s32              taskId_NumParam;
    /* 0x21624 */ s32              taskId_Bdg[6];
    /* 0x2163C */ s32              taskId_BdgBP[6];
    /* 0x21654 */ s32              taskId_BdgLV[6];
    /* 0x2166C */ s32              taskId_BdgPRI[6];
    /* 0x21684 */ s32              taskId_BdgSC[6];
    /* 0x2169C */ s32              taskId_SlotCover[6];
    /* 0x216B4 */ s32              taskId_Lvup[6];
    /* 0x216CC */ s32              taskId_Evo[6];
    /* 0x216E4 */ s32              taskId_Flash[6];
    /* 0x216FC */ s32              taskId_TextScr;
    /* 0x21700 */ s32              taskId_Star[6][15];
    /* 0x21868 */ s32              taskId_BdgU[32];
    /* 0x218E8 */ s32              taskId_BeltU[50];
    /* 0x219B0 */ s32              taskId_RankU;
    /* 0x219B4 */ s32              exitReady;
    /* 0x219B8 */ s16              timer;
    /* 0x219BA */ s16              entryIndex;
    /* 0x219BC */ u16              rewardStep;
    /* 0x219BE */ char             unk_219BE[0x219C0 - 0x219BE];
    /* 0x219C0 */ ResultObject     result;
} ResultState; // Size: 0x227C8

// Explicitly initialized so mwcc creates it here rather than at its first use; the later creation
// reorders the equal-size local-array templates in .rodata.
static ResultState* g_ResultState = NULL;

void       GX_LoadBgPltt(void* src, u32 offset, u32 size);
void       GX_LoadObjPltt(void* src, u32 offset, u32 size);
void       GXs_LoadBgPltt(void* src, u32 offset, u32 size);
void       GXs_LoadObjPltt(void* src, u32 offset, u32 size);
BOOL       func_02001b44(s32, s32, void*, s32);
void       func_0202b878(void);
extern u32 OVERLAY_42_ID;
extern u32 OVERLAY_45_ID;

// JP has no counterpart to USA's ov036, so every later overlay number is one lower there.
#ifdef REGION_USA
    #define RESULT_OVL_MENU       OVERLAY_43_ID
    #define RESULT_OVL_MENU_ENTRY ((void*)0x02084040) /* func_ov043_02084040 */
    #define RESULT_OVL_NEXT       OVERLAY_45_ID
    #define RESULT_OVL_NEXT_ENTRY ((void*)0x02088700) /* func_ov045_02088700 */
#else
    #define RESULT_OVL_MENU       OVERLAY_42_ID
    #define RESULT_OVL_MENU_ENTRY ((void*)0x020849C4) /* func_ov042_020849c4 */
    #define RESULT_OVL_NEXT       OVERLAY_44_ID
    #define RESULT_OVL_NEXT_ENTRY ((void*)0x020885C8) /* func_ov044_020885c8 */
#endif
extern void func_ov030_020ae92c(void);
extern void func_ov030_020b0fe8(void);
extern void func_ov027_020e860c(void);

void Result_UpdateEvoFlash(ResultState* state);
void Result_StageFadeIn(ResultState* state);
void Result_StageStart(ResultState* state);
void Result_StageBattleRewards(ResultState* state);
void Result_StageBattleCountPP(ResultState* state);
void Result_StageBattleEvolve(ResultState* state);
void Result_StageBattleVoice(ResultState* state);
void Result_StageBattleWaitVoice(ResultState* state);
void Result_StageBattleWaitExit(ResultState* state);
void Result_StageSureStart(ResultState* state);
void Result_StageSureCountPP(ResultState* state);
void Result_StageSureEvolve(ResultState* state);
void Result_StageSureWaitExit(ResultState* state);
void Result_StageSleepStart(ResultState* state);
void Result_StageSleepCountPP(ResultState* state);
void Result_StageSleepEvolve(ResultState* state);
void Result_StageSleepWaitExit(ResultState* state);
void Result_StageMabsStart(ResultState* state);
void Result_StageMabsCountPP(ResultState* state);
void Result_StageMabsEvolve(ResultState* state);
void Result_StageMabsWaitExit(ResultState* state);
void Result_StageFadeOut(ResultState* state);
void Result_Init(ResultState* state);
void Result_Update(ResultState* state);
void Result_Destroy(ResultState* state);
void Result_RegisterVBlank(void);
void Result_DeregisterVBlank(void);

static const OverlayProcess OvlProc_Result = {
    .init = Result_Init,
    .main = Result_Update,
    .exit = Result_Destroy,
};

// BinIdentifier archive id: this overlay's own number (USA ov044, JP ov043).
#ifdef REGION_USA
    #define RESULT_OVL_SELF 44
#else
    #define RESULT_OVL_SELF 43
#endif

// Listed out of index order on purpose: mwcc derives the .data order of these literals from the
// initializer listing, and this is the listing that reproduces the ROM layout.
const BinIdentifier Result_BinIdentifiers[35] = {
    [0]  = {RESULT_OVL_SELF,    "Apl_Tak/Grp_Result_BGD00.bin"},
    [19] = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGUy01.bin"},
    [2]  = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGDn01.bin"},
    [4]  = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGDs01.bin"},
    [20] = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGUb00.bin"},
    [5]  = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGDy00.bin"},
    [8]  = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGDb01.bin"},
    [7]  = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGDb00.bin"},
    [6]  = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGDy01.bin"},
    [22] = {RESULT_OVL_SELF,  "Apl_Tak/Grp_Result_BGUsure.bin"},
    [3]  = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGDs00.bin"},
    [24] = {RESULT_OVL_SELF, "Apl_Tak/Grp_Result_BGUsleep.bin"},
    [12] = {RESULT_OVL_SELF,    "Apl_Tak/Grp_Result_OBD00.bin"},
    [26] = {RESULT_OVL_SELF,    "Apl_Tak/Grp_Result_OBU01.bin"},
    [23] = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGUbay.bin"},
    [18] = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGUy00.bin"},
    [10] = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGDbay.bin"},
    [1]  = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGDn00.bin"},
    [14] = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGUn00.bin"},
    [15] = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGUn01.bin"},
    [16] = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGUs00.bin"},
    [17] = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGUs01.bin"},
    [9]  = {RESULT_OVL_SELF,  "Apl_Tak/Grp_Result_BGDsure.bin"},
    [21] = {RESULT_OVL_SELF,   "Apl_Tak/Grp_Result_BGUb01.bin"},
    [11] = {RESULT_OVL_SELF, "Apl_Tak/Grp_Result_BGDsleep.bin"},
    [25] = {RESULT_OVL_SELF,    "Apl_Tak/Grp_Result_OBU00.bin"},
    [13] = {RESULT_OVL_SELF,    "Apl_Tak/Grp_Result_OBD01.bin"},
    [27] = {RESULT_OVL_SELF,           "Apl_Tak/Grp_Badge.bin"},
    [28] = {RESULT_OVL_SELF,      "Apl_Tak/Grp_DummyBadge.bin"},
    [29] = {RESULT_OVL_SELF,        "Apl_Tak/Grp_MenuIcon.bin"},
    [30] = {RESULT_OVL_SELF,              "Data/BadgeData.bin"},
    [32] = {RESULT_OVL_SELF,            "Apl_Tak/FoodData.bin"},
    [31] = {RESULT_OVL_SELF,            "Apl_Tak/ItemData.bin"},
    [33] = {RESULT_OVL_SELF,        "Apl_Tak/TreasureData.bin"},
    [34] = {RESULT_OVL_SELF,    "Apl_Tak/Grp_Menu_fontSCR.bin"},
};

static const char* Result_SequenceName = "Seq_Result()";

ResultBadge Result_EmptyBadge = {
    .pinID         = 0xFFFF,
    .brand         = 14,
    .levelPP       = 1,
    .nextLevelPP   = 100,
    .ppToNextLevel = 100,
    .slot          = 1,
    .evolveChoice  = 2,
    .evolveLevel   = {5, 5},
};

// The screen runs in one of four modes picked by gSaveData.unk_1AB4: 0x08 "sure" (mingle PP), 0x10 "sleep"
// (shutdown PP), 0x60 "mabs" (Tin Pin Slammer), otherwise the regular post-battle result.
void Result_InitState(ResultState* state) {
    ResultObject* result;

    state->exitReady   = 0;
    result             = &state->result;
    state->timer       = 0;
    state->entryIndex  = 0;
    state->rewardStep  = 0;
    result->stageFlags = 0;
    result->unk_29E    = 0;
    result->exitTarget = 0;
#ifdef REGION_USA
    if (gSaveData.unk_1AB4 & 8) {
        Result_ImportSurePins(result);
        Result_LoadEquipment(result);
        Result_InitSureMode(result);
    } else if (gSaveData.unk_1AB4 & 0x10) {
        Result_LoadEquipment(result);
        Result_InitSleepMode(result);
    } else if (gSaveData.unk_1AB4 & 0x60) {
        Result_LoadEquipment(result);
        Result_InitMabsMode(result);
    } else {
        Result_LoadEquipment(result);
        Result_InitBattleMode(result);
    }
#else
    Result_LoadEquipment(result);
    if (gSaveData.unk_1AB4 & 8) {
        Result_InitSureMode(result);
    } else if (gSaveData.unk_1AB4 & 0x10) {
        Result_InitSleepMode(result);
    } else if (gSaveData.unk_1AB4 & 0x60) {
        Result_InitMabsMode(result);
    } else {
        Result_InitBattleMode(result);
    }
#endif
}

void Result_CreateBadgeTasks(ResultObject* result, s32 index) {
    ResultState* state = g_ResultState;
    u16          i     = index;

    state->taskId_Bdg[index]    = Result_bdg_CreateTask(&state->taskPool, state->unk_11588, i, result);
    state->taskId_BdgBP[index]  = Result_bdgBP_CreateTask(&state->taskPool, state->unk_11588, i, result);
    state->taskId_BdgLV[index]  = Result_bdgLV_CreateTask(&state->taskPool, state->unk_11588, i, result);
    state->taskId_BdgPRI[index] = Result_bdgPRI_CreateTask(&state->taskPool, state->unk_11588, i, result);
    state->taskId_BdgSC[index]  = Result_bdgSC_CreateTask(&state->taskPool, state->unk_11588, i, result);
}

void Result_DestroyBadgeTasks(ResultObject* result, s32 index) {
    ResultState* state = g_ResultState;

    EasyTask_DeleteTask(&state->taskPool, state->taskId_Bdg[index]);
    EasyTask_DeleteTask(&state->taskPool, state->taskId_BdgBP[index]);
    EasyTask_DeleteTask(&state->taskPool, state->taskId_BdgLV[index]);
    EasyTask_DeleteTask(&state->taskPool, state->taskId_BdgPRI[index]);
    EasyTask_DeleteTask(&state->taskPool, state->taskId_BdgSC[index]);
}

void Result_CreateTasks(ResultState* state) {
    ResultObject* result = &state->result;

#ifdef REGION_JP
    // USA starts the fade-in from Result_Init instead.
    EasyTask_CreateTask(&state->taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -0x10, 0x1000);
#endif
    if (gSaveData.unk_1AB4 & 8) {
        state->taskId_TextScr  = Result_textScr_sure_CreateTask(&state->taskPool, state->unk_11588, result);
        state->taskId_NumParam = Result_num_Param_sure_CreateTask(&state->taskPool, state->unk_11588, result);
    } else if (gSaveData.unk_1AB4 & 0x10) {
        state->taskId_TextScr  = Result_textScr_sleep_CreateTask(&state->taskPool, state->unk_11588, result);
        state->taskId_NumParam = Result_num_Param_sleep_CreateTask(&state->taskPool, state->unk_11588, result);
    } else if (gSaveData.unk_1AB4 & 0x60) {
        state->taskId_NumParam = Result_num_Param_mabs_CreateTask(&state->taskPool, state->unk_11588, result);
    } else {
        state->taskId_NumTime   = Result_numTime_CreateTask(&state->taskPool, state->unk_11588, result);
        state->taskId_NumMaxHit = Result_numMaxHit_CreateTask(&state->taskPool, state->unk_11588, result);
        state->taskId_NumParam  = Result_num_Param_CreateTask(&state->taskPool, state->unk_11588, result);
        state->taskId_RankU     = Result_rankU_CreateTask(&state->taskPool, state->unk_11588, result->overallRank, result);
    }

    for (s16 i = 0; i < 6; i++) {
        Result_CreateBadgeTasks(result, i);
    }
    for (s16 i = 0; i < 6; i++) {
        if (i >= result->badgeCount) {
            state->taskId_SlotCover[i] = Result_slotCover_CreateTask(&state->taskPool, state->unk_11588, i, result);
        }
    }
}

BOOL Result_IsAnyBadgeEvolving(ResultObject* result) {
    for (u16 i = 0; i < result->badgeCount; i++) {
        if (result->badges[i].flags & 1) {
            return TRUE;
        }
    }
    return FALSE;
}

void Result_UpdateEvoFlash(ResultState* state) {
    ResultObject* result        = &state->result;
    u16           durations[40] = {20, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 6,
                                   8,  6, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    u16           levels[40]    = {0,   5,  10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95,
                                   100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5};

    if (result->flashTimer > 0) {
        result->flashTimer--;
    } else {
        result->flashStep++;
        if (result->flashStep > 39) {
            result->flashStep = 0;
        }
        result->flashTimer = durations[result->flashStep] - 1;
        result->flashLevel = (levels[result->flashStep] * 30) / 100 + 1;
    }

    if (Result_IsAnyBadgeEvolving(result) == TRUE && result->flashStep == 0 && result->flashTimer == 0) {
        SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_EVO_FLASH);
    }
}

void Result_StageFadeIn(ResultState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        state->timer = 60;
        DebugOvlDisp_Pop();
    }
}

void Result_StageStart(ResultState* state) {
    ResultObject* result = &state->result;

    if (state->timer > 0) {
        state->timer--;
        return;
    }

    state->entryIndex = 0;
    state->timer      = 0;
    if (gSaveData.unk_1AB4 & 8) {
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageSureStart, state, PROCESS_STAGE_INIT);
    } else if (gSaveData.unk_1AB4 & 0x10) {
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageSleepStart, state, PROCESS_STAGE_INIT);
    } else if (gSaveData.unk_1AB4 & 0x60) {
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageMabsStart, state, PROCESS_STAGE_INIT);
    } else {
        result->stageFlags |= 2;
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageBattleRewards, state, PROCESS_STAGE_INIT);
    }
}

// Nonmatching: the 0x219BC offset is loaded early into r3 instead of at the switch in r0
void Result_StageBattleRewards(ResultState* state) {
    ResultObject* result = &state->result;
    s16           index;

    if (result->stageFlags & 1) {
        result->stageFlags &= ~1;
    }
    if (result->stageFlags & 4) {
        result->stageFlags &= ~4;
    }

    switch (state->rewardStep) {
        case 0: {
            if (result->dropCount == 0) {
                state->entryIndex = 0;
                state->rewardStep = 1;
            } else if (result->stageFlags & 2) {
                result->stageFlags &= ~2;
                result->stageFlags |= 1;
                if (state->entryIndex > 5) {
                    result->stageFlags |= 4;
                }
                state->taskId_BeltU[state->entryIndex] =
                    Result_beltU_CreateTask(&state->taskPool, state->unk_11588, 0, state->entryIndex, result);
                state->taskId_BdgU[state->entryIndex] =
                    Result_bdgU_CreateTask(&state->taskPool, state->unk_11588, 0, state->entryIndex, result);
                state->entryIndex++;
                if (state->entryIndex >= result->dropCount) {
                    state->entryIndex = 0;
                    state->rewardStep = 1;
                }
            }
        } break;

        case 1: {
            if (result->specialBonusCount == 0) {
                state->entryIndex = 0;
                state->rewardStep = 2;
            } else if (result->stageFlags & 2) {
                result->stageFlags &= ~2;
                result->stageFlags |= 1;
                index = state->entryIndex;
                state->taskId_BeltU[(u16)(index + 32)] =
                    Result_beltU_CreateTask(&state->taskPool, state->unk_11588, 2, index, result);
                state->entryIndex++;
                if (state->entryIndex >= result->specialBonusCount) {
                    state->entryIndex = 0;
                    state->rewardStep = 2;
                }
            }
        } break;

        case 2: {
            if (result->foodFinishedCount == 0) {
                state->entryIndex = 0;
                state->rewardStep = 4;
            } else if (result->foodFinished[0] == 1) {
                if (result->stageFlags & 2) {
                    result->stageFlags &= ~2;
                    result->stageFlags |= 0x21;
                    state->taskId_BeltU[48] = Result_beltU_CreateTask(&state->taskPool, state->unk_11588, 1, 0, result);
                    state->rewardStep       = 3;
                }
            } else {
                state->rewardStep = 3;
            }
        } break;

        case 3: {
            if (result->foodFinished[1] == 1) {
                if (result->stageFlags & 2) {
                    result->stageFlags &= ~2;
                    u16 slot = state->entryIndex + 48;

                    result->stageFlags |= 0x21;
                    state->taskId_BeltU[slot] = Result_beltU_CreateTask(&state->taskPool, state->unk_11588, 1, 1, result);
                    state->rewardStep         = 4;
                }
            } else {
                state->rewardStep = 4;
            }
        } break;

        case 4: {
            if (result->displayedPP < result->earnedPP) {
                SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_COUNTERLOOP);
            }
            DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageBattleCountPP, state, PROCESS_STAGE_INIT);
        } break;
    }
}

void Result_StageBattleCountPP(ResultState* state) {
    ResultObject* result = &state->result;
    s32           step;

    if (TouchInput_IsTouchActive() || (InputStatus.buttonState.currButtons & 1)) {
        step = 5;
        result->stageFlags |= 0x10;
    } else {
        step = 1;
        result->stageFlags &= ~0x10;
    }

    if (result->displayedPP >= result->earnedPP) {
        u16  i;
        BOOL evolving;

        result->displayedPP = result->earnedPP;
        SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_COUNTERSTOP);
        evolving          = FALSE;
        state->entryIndex = 0;
        for (i = 0; i < result->badgeCount; i++) {
            if (result->badges[i].flags & 1) {
                evolving = TRUE;
            }
        }
        if (evolving) {
            state->timer = 60;
            DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageBattleEvolve, state, PROCESS_STAGE_INIT);
        } else {
            state->timer = 10;
            result->stageFlags |= 8;
            DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageBattleVoice, state, PROCESS_STAGE_INIT);
        }
        return;
    }

    result->displayedPP += step;
    for (u16 i = 0; i < result->badgeCount; i++) {
        if (result->badges[i].pinID != 0xFFFF) {
            ResultBadge* badge = &result->badges[i];

            if (badge->level != badge->maxLevel && !(badge->flags & 1)) {
                badge->pp[0] += step;
                if (badge->pp[0] >= result->earnedPP + badge->startPP[0]) {
                    badge->pp[0] = result->earnedPP + badge->startPP[0];
                }
                result->badges[i].totalPP = Result_GetBadgeTotalPP(result, i);
                if (result->badges[i].totalPP >= result->badges[i].nextLevelPP) {
                    result->badges[i].evolveChoice = Result_CheckBadgeEvolution(result, i, 0);
                    if (result->badges[i].evolveChoice != 2) {
                        result->badges[i].totalPP = result->badges[i].nextLevelPP;
                        result->badges[i].flags |= 1;
                        state->taskId_Flash[i] = Result_flash_CreateTask(&state->taskPool, state->unk_11588, i, result);
                    } else {
                        result->badges[i].level++;
                        result->badges[i].levelPP     = Result_GetBadgeLevelPP(result, i, result->badges[i].ppCurve);
                        result->badges[i].nextLevelPP = Result_GetBadgeNextLevelPP(result, i, result->badges[i].ppCurve);
#ifdef REGION_USA
                        if (result->badges[i].level == result->badges[i].maxLevel) {
                            if (result->badges[i].pp[0] >= result->badges[i].startPP[0] + result->badges[i].ppToNextLevel) {
                                result->badges[i].pp[0] = result->badges[i].startPP[0] + result->badges[i].ppToNextLevel;
                            }
                        }
#endif
                        state->taskId_Lvup[i] = Result_lvup_CreateTask(&state->taskPool, state->unk_11588, i, result);
                        for (u16 j = 0; j < 15; j++) {
                            state->taskId_Star[i][j] =
                                Result_star_CreateTask(&state->taskPool, state->unk_11588, i, 0, result);
                        }
                    }
                }
            }
        }
    }
    Result_UpdateEvoFlash(state);
}

void Result_StageBattleEvolve(ResultState* state) {
    ResultObject* result = &state->result;

    if (state->timer > 0) {
        state->timer--;
    } else if (result->badges[state->entryIndex].evolveChoice != 2) {
        if (result->flashStep == 20 && result->flashTimer == 0) {
            Result_DestroyBadgeTasks(result, state->entryIndex);
            Result_EvolveBadge(result, state->entryIndex,
                               result->badges[state->entryIndex].evolvePinID[result->badges[state->entryIndex].evolveChoice]);
            Result_CreateBadgeTasks(result, state->entryIndex);
            state->taskId_Evo[state->entryIndex] =
                Result_evo_CreateTask(&state->taskPool, state->unk_11588, state->entryIndex, result);
            for (u16 j = 0; j < 15; j++) {
                state->taskId_Star[state->entryIndex][j] =
                    Result_star_CreateTask(&state->taskPool, state->unk_11588, state->entryIndex, 1, result);
            }
            EasyTask_DeleteTask(&state->taskPool, state->taskId_Flash[state->entryIndex]);
            state->timer = 30;
            state->entryIndex++;
        }
    } else {
        state->entryIndex++;
    }

    Result_UpdateEvoFlash(state);
    if (state->entryIndex >= result->badgeCount) {
        state->timer = 60;
        result->stageFlags |= 8;
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageBattleVoice, state, PROCESS_STAGE_INIT);
    }
}

void Result_StageBattleVoice(ResultState* state) {
    ResultObject* result       = &state->result;
    SndMgrSeIdx   voices[2][4] = {
        {0x376, 0x39E, 0x3C6, 0x3EE},
        {0x377, 0x39F, 0x3C7, 0x3EF},
    };

    if (state->timer > 0) {
        state->timer--;
    } else {
        if (result->overallRank == 0) {
            if (result->battlePartner == 0xFF) {
                SndMgr_StartPlayingSE(voices[0][0]);
            } else {
                SndMgr_StartPlayingSE(voices[0][result->battlePartner + 1]);
            }
        } else if (result->overallRank == 5) {
            if (result->battlePartner == 0xFF) {
                SndMgr_StartPlayingSE(voices[1][0]);
            } else {
                SndMgr_StartPlayingSE(voices[1][result->battlePartner + 1]);
            }
        }
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageBattleWaitVoice, state, PROCESS_STAGE_INIT);
        return;
    }

    if (TouchInput_WasTouchPressed() || (InputStatus.buttonState.pressedButtons & 1)) {
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageBattleWaitExit, state, PROCESS_STAGE_INIT);
    }
}

void Result_StageBattleWaitVoice(ResultState* state) {
    if (SndMgr_IsSEPlaying(SEIDX_NEKU_BTL_RIZALT01) == FALSE) {
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageBattleWaitExit, state, PROCESS_STAGE_INIT);
    }
}

// Nonmatching: ReplaceTop argument loads scheduled before the two stores
void Result_StageBattleWaitExit(ResultState* state) {
    ResultObject* result = &state->result;

    if (TouchInput_WasTouchPressed() || (InputStatus.buttonState.pressedButtons & 1)) {
        SndMgr_StartPlayingSE(SEIDX_SE_BATTLE_OUT);
        gSaveData.unk_24B4 = 2;
        result->exitTarget = 0;
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageFadeOut, state, PROCESS_STAGE_INIT);
    }
}

void Result_StageSureStart(ResultState* state) {
    ResultObject* result = &state->result;

    if (TouchInput_WasTouchPressed() || (InputStatus.buttonState.pressedButtons & 1)) {
        result->exitTarget = 4;
        if (result->displayedPP < result->earnedPP) {
            SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_COUNTERLOOP);
        }
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageSureCountPP, state, PROCESS_STAGE_INIT);
    }
}

void Result_StageSureCountPP(ResultState* state) {
    ResultObject* result = &state->result;
    s32           step;

    if (TouchInput_IsTouchActive() || (InputStatus.buttonState.currButtons & 1)) {
        step = 5;
        result->stageFlags |= 0x10;
    } else {
        step = 1;
        result->stageFlags &= ~0x10;
    }

    if (result->displayedPP >= result->earnedPP) {
        u16  i;
        BOOL evolving;

        result->displayedPP = result->earnedPP;
        SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_COUNTERSTOP);
        evolving          = FALSE;
        state->entryIndex = 0;
        for (i = 0; i < result->badgeCount; i++) {
            if (result->badges[i].flags & 1) {
                evolving = TRUE;
            }
        }
        if (evolving) {
            state->timer = 60;
            DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageSureEvolve, state, PROCESS_STAGE_INIT);
        } else {
            state->timer = 0;
            DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageSureWaitExit, state, PROCESS_STAGE_INIT);
        }
        return;
    }

    result->displayedPP += step;
    for (u16 i = 0; i < result->badgeCount; i++) {
        if (result->badges[i].pinID != 0xFFFF) {
            ResultBadge* badge = &result->badges[i];

            if (badge->level != badge->maxLevel && !(badge->flags & 1)) {
                badge->pp[1] += step;
                if (badge->pp[1] >= result->earnedPP + badge->startPP[1]) {
                    badge->pp[1] = result->earnedPP + badge->startPP[1];
                }
                result->badges[i].totalPP = Result_GetBadgeTotalPP(result, i);
                if (result->badges[i].totalPP >= result->badges[i].nextLevelPP) {
                    result->badges[i].evolveChoice = Result_CheckBadgeEvolution(result, i, 1);
                    if (result->badges[i].evolveChoice != 2) {
                        result->badges[i].totalPP = result->badges[i].nextLevelPP;
                        result->badges[i].flags |= 1;
                        state->taskId_Flash[i] = Result_flash_CreateTask(&state->taskPool, state->unk_11588, i, result);
                    } else {
                        result->badges[i].level++;
                        result->badges[i].levelPP     = Result_GetBadgeLevelPP(result, i, result->badges[i].ppCurve);
                        result->badges[i].nextLevelPP = Result_GetBadgeNextLevelPP(result, i, result->badges[i].ppCurve);
#ifdef REGION_USA
                        if (result->badges[i].level == result->badges[i].maxLevel) {
                            if (result->badges[i].pp[1] >= result->badges[i].startPP[1] + result->badges[i].ppToNextLevel) {
                                result->badges[i].pp[1] = result->badges[i].startPP[1] + result->badges[i].ppToNextLevel;
                            }
                        }
#endif
                        state->taskId_Lvup[i] = Result_lvup_CreateTask(&state->taskPool, state->unk_11588, i, result);
                        for (u16 j = 0; j < 15; j++) {
                            state->taskId_Star[i][j] =
                                Result_star_CreateTask(&state->taskPool, state->unk_11588, i, 0, result);
                        }
                    }
                }
            }
        }
    }
    Result_UpdateEvoFlash(state);
}

void Result_StageSureEvolve(ResultState* state) {
    ResultObject* result = &state->result;

    if (state->timer > 0) {
        state->timer--;
    } else if (result->badges[state->entryIndex].evolveChoice != 2) {
        if (result->flashStep == 20 && result->flashTimer == 0) {
            Result_DestroyBadgeTasks(result, state->entryIndex);
            Result_EvolveBadge(result, state->entryIndex,
                               result->badges[state->entryIndex].evolvePinID[result->badges[state->entryIndex].evolveChoice]);
            Result_CreateBadgeTasks(result, state->entryIndex);
            state->taskId_Evo[state->entryIndex] =
                Result_evo_CreateTask(&state->taskPool, state->unk_11588, state->entryIndex, result);
            for (u16 j = 0; j < 15; j++) {
                state->taskId_Star[state->entryIndex][j] =
                    Result_star_CreateTask(&state->taskPool, state->unk_11588, state->entryIndex, 1, result);
            }
            EasyTask_DeleteTask(&state->taskPool, state->taskId_Flash[state->entryIndex]);
            state->timer = 30;
            state->entryIndex++;
        }
    } else {
        state->entryIndex++;
    }

    Result_UpdateEvoFlash(state);
    if (state->entryIndex >= result->badgeCount) {
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageSureWaitExit, state, PROCESS_STAGE_INIT);
    }
}

void Result_StageSureWaitExit(ResultState* state) {
    if (TouchInput_WasTouchPressed() || (InputStatus.buttonState.pressedButtons & 1)) {
        SndMgr_StartPlayingSE(SEIDX_SE_BATTLE_OUT);
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageFadeOut, state, PROCESS_STAGE_INIT);
    }
}

void Result_StageSleepStart(ResultState* state) {
    ResultObject* result = &state->result;

    result->exitTarget = 5;
    if (result->displayedPP < result->earnedPP) {
        SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_COUNTERLOOP);
    }
    DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageSleepCountPP, state, PROCESS_STAGE_INIT);
}

void Result_StageSleepCountPP(ResultState* state) {
    ResultObject* result = &state->result;
    s32           step;

    if (TouchInput_IsTouchActive() || (InputStatus.buttonState.currButtons & 1)) {
        step = 5;
        result->stageFlags |= 0x10;
    } else {
        step = 1;
        result->stageFlags &= ~0x10;
    }

    if (result->displayedPP >= result->earnedPP) {
        u16  i;
        BOOL evolving;

        result->displayedPP = result->earnedPP;
        SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_COUNTERSTOP);
        evolving          = FALSE;
        state->entryIndex = 0;
        for (i = 0; i < result->badgeCount; i++) {
            if (result->badges[i].flags & 1) {
                evolving = TRUE;
            }
        }
        if (evolving) {
            state->timer = 60;
            DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageSleepEvolve, state, PROCESS_STAGE_INIT);
        } else {
            state->timer = 0;
            DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageSleepWaitExit, state, PROCESS_STAGE_INIT);
        }
        return;
    }

    result->displayedPP += step;
    for (u16 i = 0; i < result->badgeCount; i++) {
        if (result->badges[i].pinID != 0xFFFF) {
            ResultBadge* badge = &result->badges[i];

            if (badge->level != badge->maxLevel && !(badge->flags & 1)) {
                badge->pp[2] += step;
                if (badge->pp[2] >= result->earnedPP + badge->startPP[2]) {
                    badge->pp[2] = result->earnedPP + badge->startPP[2];
                }
                result->badges[i].totalPP = Result_GetBadgeTotalPP(result, i);
                if (result->badges[i].totalPP >= result->badges[i].nextLevelPP) {
                    result->badges[i].evolveChoice = Result_CheckBadgeEvolution(result, i, 2);
                    if (result->badges[i].evolveChoice != 2) {
                        result->badges[i].totalPP = result->badges[i].nextLevelPP;
                        result->badges[i].flags |= 1;
                        state->taskId_Flash[i] = Result_flash_CreateTask(&state->taskPool, state->unk_11588, i, result);
                    } else {
                        result->badges[i].level++;
                        result->badges[i].levelPP     = Result_GetBadgeLevelPP(result, i, result->badges[i].ppCurve);
                        result->badges[i].nextLevelPP = Result_GetBadgeNextLevelPP(result, i, result->badges[i].ppCurve);
#ifdef REGION_USA
                        if (result->badges[i].level == result->badges[i].maxLevel) {
                            if (result->badges[i].pp[2] >= result->badges[i].startPP[2] + result->badges[i].ppToNextLevel) {
                                result->badges[i].pp[2] = result->badges[i].startPP[2] + result->badges[i].ppToNextLevel;
                            }
                        }
#endif
                        state->taskId_Lvup[i] = Result_lvup_CreateTask(&state->taskPool, state->unk_11588, i, result);
                        for (u16 j = 0; j < 15; j++) {
                            state->taskId_Star[i][j] =
                                Result_star_CreateTask(&state->taskPool, state->unk_11588, i, 0, result);
                        }
                    }
                }
            }
        }
    }
    Result_UpdateEvoFlash(state);
}

void Result_StageSleepEvolve(ResultState* state) {
    ResultObject* result = &state->result;

    if (state->timer > 0) {
        state->timer--;
    } else if (result->badges[state->entryIndex].evolveChoice != 2) {
        if (result->flashStep == 20 && result->flashTimer == 0) {
            Result_DestroyBadgeTasks(result, state->entryIndex);
            Result_EvolveBadge(result, state->entryIndex,
                               result->badges[state->entryIndex].evolvePinID[result->badges[state->entryIndex].evolveChoice]);
            Result_CreateBadgeTasks(result, state->entryIndex);
            state->taskId_Evo[state->entryIndex] =
                Result_evo_CreateTask(&state->taskPool, state->unk_11588, state->entryIndex, result);
            for (u16 j = 0; j < 15; j++) {
                state->taskId_Star[state->entryIndex][j] =
                    Result_star_CreateTask(&state->taskPool, state->unk_11588, state->entryIndex, 1, result);
            }
            EasyTask_DeleteTask(&state->taskPool, state->taskId_Flash[state->entryIndex]);
            state->timer = 30;
            state->entryIndex++;
        }
    } else {
        state->entryIndex++;
    }

    Result_UpdateEvoFlash(state);
    if (state->entryIndex >= result->badgeCount) {
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageSleepWaitExit, state, PROCESS_STAGE_INIT);
    }
}

void Result_StageSleepWaitExit(ResultState* state) {
    if (TouchInput_WasTouchPressed() || (InputStatus.buttonState.pressedButtons & 1)) {
        SndMgr_StartPlayingSE(SEIDX_SE_BATTLE_OUT);
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageFadeOut, state, PROCESS_STAGE_INIT);
    }
}

void Result_StageMabsStart(ResultState* state) {
    ResultObject* result = &state->result;

    if (gSaveData.unk_1AB4 & 0x20) {
        result->exitTarget = 0;
    } else {
        result->exitTarget = 4;
    }
    if (result->displayedPP < result->earnedPP) {
        SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_COUNTERLOOP);
    }
    DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageMabsCountPP, state, PROCESS_STAGE_INIT);
}

void Result_StageMabsCountPP(ResultState* state) {
    ResultObject* result = &state->result;
    s32           step;

    if (TouchInput_IsTouchActive() || (InputStatus.buttonState.currButtons & 1)) {
        step = 5;
        result->stageFlags |= 0x10;
    } else {
        step = 1;
        result->stageFlags &= ~0x10;
    }

    if (result->displayedPP >= result->earnedPP) {
        u16  i;
        BOOL evolving;

        result->displayedPP = result->earnedPP;
        SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_COUNTERSTOP);
        evolving          = FALSE;
        state->entryIndex = 0;
        for (i = 0; i < result->badgeCount; i++) {
            if (result->badges[i].flags & 1) {
                evolving = TRUE;
            }
        }
        if (evolving) {
            state->timer = 60;
            DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageMabsEvolve, state, PROCESS_STAGE_INIT);
        } else {
            state->timer = 0;
            DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageMabsWaitExit, state, PROCESS_STAGE_INIT);
        }
        return;
    }

    result->displayedPP += step;
    for (u16 i = 0; i < result->badgeCount; i++) {
        if (result->badges[i].pinID != 0xFFFF) {
            ResultBadge* badge = &result->badges[i];

            if (badge->level != badge->maxLevel && !(badge->flags & 1)) {
                badge->pp[1] += step;
                if (badge->pp[1] >= result->earnedPP + badge->startPP[1]) {
                    badge->pp[1] = result->earnedPP + badge->startPP[1];
                }
                result->badges[i].totalPP = Result_GetBadgeTotalPP(result, i);
                if (result->badges[i].totalPP >= result->badges[i].nextLevelPP) {
                    result->badges[i].evolveChoice = Result_CheckBadgeEvolution(result, i, 1);
                    if (result->badges[i].evolveChoice != 2) {
                        result->badges[i].totalPP = result->badges[i].nextLevelPP;
                        result->badges[i].flags |= 1;
                        state->taskId_Flash[i] = Result_flash_CreateTask(&state->taskPool, state->unk_11588, i, result);
                    } else {
                        result->badges[i].level++;
                        result->badges[i].levelPP     = Result_GetBadgeLevelPP(result, i, result->badges[i].ppCurve);
                        result->badges[i].nextLevelPP = Result_GetBadgeNextLevelPP(result, i, result->badges[i].ppCurve);
                        state->taskId_Lvup[i]         = Result_lvup_CreateTask(&state->taskPool, state->unk_11588, i, result);
                        for (u16 j = 0; j < 15; j++) {
                            state->taskId_Star[i][j] =
                                Result_star_CreateTask(&state->taskPool, state->unk_11588, i, 0, result);
                        }
                    }
                }
            }
        }
    }
    Result_UpdateEvoFlash(state);
}

void Result_StageMabsEvolve(ResultState* state) {
    ResultObject* result = &state->result;

    if (state->timer > 0) {
        state->timer--;
    } else if (result->badges[state->entryIndex].evolveChoice != 2) {
        if (result->flashStep == 20 && result->flashTimer == 0) {
            Result_DestroyBadgeTasks(result, state->entryIndex);
            Result_EvolveBadge(result, state->entryIndex,
                               result->badges[state->entryIndex].evolvePinID[result->badges[state->entryIndex].evolveChoice]);
            Result_CreateBadgeTasks(result, state->entryIndex);
            state->taskId_Evo[state->entryIndex] =
                Result_evo_CreateTask(&state->taskPool, state->unk_11588, state->entryIndex, result);
            for (u16 j = 0; j < 15; j++) {
                state->taskId_Star[state->entryIndex][j] =
                    Result_star_CreateTask(&state->taskPool, state->unk_11588, state->entryIndex, 1, result);
            }
            EasyTask_DeleteTask(&state->taskPool, state->taskId_Flash[state->entryIndex]);
            state->timer = 30;
            state->entryIndex++;
        }
    } else {
        state->entryIndex++;
    }

    Result_UpdateEvoFlash(state);
    if (state->entryIndex >= result->badgeCount) {
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageMabsWaitExit, state, PROCESS_STAGE_INIT);
    }
}

void Result_StageMabsWaitExit(ResultState* state) {
    if (TouchInput_WasTouchPressed() || (InputStatus.buttonState.pressedButtons & 1)) {
        SndMgr_StartPlayingSE(SEIDX_SE_BATTLE_OUT);
        DebugOvlDisp_ReplaceTop((OverlayCB)Result_StageFadeOut, state, PROCESS_STAGE_INIT);
    }
}

void Result_StageFadeOut(ResultState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0x10, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void Result_Init(ResultState* state) {
    if (state == NULL) {
        const char* sequence = Result_SequenceName;
        state                = Mem_AllocHeapTail(&gDebugHeap, sizeof(ResultState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        g_ResultState = state;
        MainOvlDisp_SetCbArg(state);
    }
    state->unk_11584 = DatMgr_AllocateSlot();
    state->unk_11588 = DatMgr_AllocateSlot();
    Result_RegisterVBlank();
    state->unk_11580 = ResourceMgr_ReinitManagers(&state->unk_00000);
#ifdef REGION_USA
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
#endif
    TouchInput_Init();
    Mem_InitializeHeap(&state->heap, state->heapBuffer, sizeof(state->heapBuffer));
    FS_LoadOverlay(0, (u32)&OVERLAY_31_ID);
    EasyTask_InitializePool(&state->taskPool, &state->heap, 0x200, NULL, NULL);
    data_02066aec = 0;
    data_02066eec = 0;
#ifdef REGION_USA
    EasyTask_CreateTask(&state->taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -0x10, 0x1000);
#endif
    Result_InitState(state);
    Result_LoadBackgrounds(&state->result);
    Result_CreateTasks(state);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)Result_StageStart, state, PROCESS_STAGE_INIT);
    DebugOvlDisp_Push((OverlayCB)Result_StageFadeIn, state, PROCESS_STAGE_INIT);
    MainOvlDisp_NextProcessStage();
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
}

void Result_Update(ResultState* state) {
    ResultObject* result = &state->result;

    TouchInput_Update();
    OamMgr_Reset3DState();
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    Result_UpdateBackgrounds(result);
    DebugOvlDisp_Run();
    EasyTask_UpdatePool(&state->taskPool);
    if (DebugOvlDisp_IsStackAtBase() == TRUE) {
        state->exitReady = 1;
    }
    OamMgr_Swap3DBuffers();
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_EXTENDED], NULL);

    if (state->exitReady != 0) {
        switch (result->exitTarget) {
            case 0: {
                OverlayTag tag;
                MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_30_ID, func_ov030_020ae92c, NULL, PROCESS_STAGE_INIT);
            } break;

            case 2: {
                OverlayTag tag;
                MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_27_ID, func_ov027_020e860c, NULL, PROCESS_STAGE_INIT);
            } break;

            case 3: {
                OverlayTag tag;
                MainOvlDisp_ReplaceTop(&tag, (s32)&RESULT_OVL_NEXT, RESULT_OVL_NEXT_ENTRY, NULL, PROCESS_STAGE_INIT);
            } break;

            case 4: {
                OverlayTag tag;
                MainOvlDisp_ReplaceTop(&tag, (s32)&RESULT_OVL_MENU, RESULT_OVL_MENU_ENTRY, NULL, PROCESS_STAGE_INIT);
            } break;

            case 5: {
                if (gSaveData.unk_1AB4 & 2) {
                    OverlayTag tag;
                    gSaveData.unk_1AB4 &= ~2;
                    MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_30_ID, func_ov030_020b0fe8, NULL, PROCESS_STAGE_INIT);
                } else {
                    OverlayTag tag;
                    MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_30_ID, func_ov030_020ae92c, NULL, PROCESS_STAGE_INIT);
                }
            } break;

            case 1:
            default: {
                OverlayTag tag;
                MainOvlDisp_Pop(&tag);
            } break;
        }
    }
}

void Result_Destroy(ResultState* state) {
    ResultObject* result = &state->result;

    Result_SaveEquippedPins(result);
    if (gSaveData.unk_1AB4 & 8) {
        gSaveData.unk_1AB4 &= ~8;
        Result_CommitSure(result);
        gSaveData.unk_1AB4 |= 1;
    } else if (gSaveData.unk_1AB4 & 0x10) {
        gSaveData.unk_1AB4 &= ~0x10;
        Result_CommitSleep(result);
    } else if (gSaveData.unk_1AB4 & 0x60) {
        gSaveData.unk_1AB4 &= ~0x60;
        Result_CommitMabs(result);
    } else {
        Result_CommitBattle(result);
    }
    CriSndMgr_Stop(0);
    Result_ReleaseBackgrounds(result);
    EasyTask_DestroyPool(&state->taskPool);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->unk_11584);
    DatMgr_ClearSlot(state->unk_11588);
    Result_DeregisterVBlank();
    FS_UnloadOverlay(0, (u32)&OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
}

void ProcessOverlay_Result(ResultState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        Result_Destroy(state);
    } else {
        OvlProc_Result.funcs[stage](state);
    }
}

// Nonmatching: BG-setting constants materialised in a different order
void Result_InitDisplay(void) {
    Interrupts_Init();
    HBlank_Init();
    do {
    } while (REG_VCOUNT < (s16)0xC0);
    GX_Init();
    func_0202b878();
    DMA_Init(0x100);
    Display_Init();
    if (gSaveData.unk_1AB4 & 0x10) {
        g_DisplaySettings.controls[DISPLAY_MAIN].brightness = -16;
        g_DisplaySettings.controls[DISPLAY_SUB].brightness  = -16;
        Display_Commit();
    }
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(GX_VRAM_ALL);
    GX_SetBankForTex(GX_VRAM_A);
    GX_SetBankForTexPltt(GX_VRAM_G);
    GX_SetBankForBg(GX_VRAM_B);
    GX_SetBankForObj(GX_VRAM_E);
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
    REG_POWER_CNT &= ~0x8000;
    Display_CommitSynced();
    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 5, 0, 0x14);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 1, 1, 0x104);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 3, 1, 0x20C);

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_64K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode   = GX_OBJBMPMODE_1D_128K;
    Display_SetMainLayers(LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);
    data_0206aa78 = 0x300010;
    data_0206aa7c = 0x400040;

    g_DisplaySettings.controls[DISPLAY_SUB].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

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
    Display_SetSubLayers(LAYER_BG3 | LAYER_OBJ);
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
    OamMgr_InitEngine(0, DISPLAY_EXTENDED);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
}

void Result_VBlank(void) {
    if (SystemStatusFlags.vblank) {
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
        func_02001b44(2, 0, &data_020672ec, 0x400);
    }
}

void Result_RegisterVBlank(void) {
    Result_InitDisplay();
    Interrupts_RegisterVBlankCallback(Result_VBlank, 1);
}

void Result_DeregisterVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}
