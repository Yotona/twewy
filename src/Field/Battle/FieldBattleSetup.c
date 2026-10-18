#include "Field/FieldRegion.h"

#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Resources/BgResMgr.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Battle/FieldBattleTransition.h"
#include "Field/Battle/FieldEncounter.h"
#include "Field/Battle/FieldNoiseEncounter.h"
#include "Field/Event/EventManager.h"
#include "Field/Event/EventTables.h"
#include "Field/Event/Events.h"
#include "Field/FieldTypes.h"
#include "Field/Scene/FieldBg.h"
#include "Field/Scene/FieldPaths.h"
#include "Field/Scene/FieldSeq.h"
#include "Field/Tasks/Fld_Badge.h"
#include "Field/Tasks/Fld_LargeGingo.h"
#include "Field/Tasks/Fld_Luck.h"
#include "Field/Tasks/Fld_Partner.h"
#include "Field/Tasks/Fld_Piranha.h"
#include "Field/Tasks/Fld_Player.h"
#include "Field/Tasks/Fld_Raster.h"
#include "Field/Tasks/Fld_ScanArea.h"
#include "Field/Tasks/Fld_Shinigami.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "SndMgr.h"
#include <nitro/fx.h>

extern BgResource* BgResMgr_AllocScreen(BgResMgr* mgr, void* source, s32 screenBase, s32 screenSize);

extern void      func_02026590(s32* value, s32 target, u16 frames);
extern const u32 data_0205a124[];

u8 Field_GetAreaZone(u32 area) {
    if (area >= 44) {
        return 35;
    }
    return FieldAreaZones[area];
}

u8 Field_GetCurrentZone(void) {
    return Field_GetAreaZone(gSaveData.fieldArea);
}

s32 Field_GetCurrentTrendZone(void) {
    s32 value = Field_GetCurrentZone();
    if (value >= 21) {
        value = -1;
    }
    return value;
}

void FieldBattle_SetupScripted(s32 selector) {
    data_ov030_02109d60->unk_21ABC = 1;
    gSaveData.unk_3130             = 0;
    gSaveData.unk_3134             = selector;
    gSaveData.unk_3142             = 0;
    gSaveData.battleDifficulty     = gSaveData.playerStats.difficulty;
    gSaveData.unk_3138             = func_ov030_020af2fc();
    gSaveData.unk_3140             = 1;
    if (selector == 2) {
        gSaveData.unk_3140 = 0;
        gSaveData.unk_3130 = 1;
    } else if (selector == 3) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 4) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 5) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 6) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 9) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 18) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 33) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 31) {
        gSaveData.unk_3130 = 1;
        gSaveData.unk_3140 = 0;
    } else if ((u32)(selector - 47) <= 2) {
        gSaveData.unk_3130 = 2;
    }
    if (selector == -1) {
        gSaveData.unk_3134            = 1;
        gSaveData.battleCount         = 1;
        gSaveData.battleEncounters[0] = FieldNoise_RollEncounter(0);
    }
    if (selector == -2) {
        gSaveData.unk_3134            = 1;
        gSaveData.battleCount         = 1;
        gSaveData.battleEncounters[0] = FieldNoise_RollEncounter(2);
    }
    gSaveData.battleDropRate = gSaveData.playerStats.dropRate;
}

void Field_BeginScript(FieldEvent event, s32 param) {
    FldEvent_BeginScript(data_ov030_02109d60, event, param);
}

void Field_CountDrawnHuman(void) {
    data_ov030_02109d60->drawnHumanCount += 1;
}

void func_ov030_020af51c(void) {
    data_ov030_02109d60->unk_21B18 += 1;
}

u16 Field_GetLightColor(void) {
    s32 level = data_ov030_02109d60->lightLevel;
    return (u16)(level | (level << 5) | (level << 10));
}

void Field_ApplyVertexColorAt(FieldPos* pos) {
    u16 color;

    if (gSaveData.fieldArea == 8) {
        if (Fld_Shinigami_GetFloorAttr(pos, &color)) {
            if (color != GX_RGB(19, 19, 19)) {
                REG_GFX_FIFO_VERTEX_COLOR = color;
            }
        } else {
            REG_GFX_FIFO_VERTEX_COLOR = GX_RGB(19, 19, 19);
        }
    } else {
        if (Fld_Shinigami_GetFloorAttr(pos, &color)) {
            REG_GFX_FIFO_VERTEX_COLOR = color;
        }
    }
}

PaletteResource* Field_GetHumanPalette(void) {
    return data_ov030_02109d60->humanPalette;
}

s32 Field_GetPlayerHeight(void) {
    return Fld_Player_GetHeight(EasyTask_GetTaskById(&data_ov030_02109d60->taskPool, data_ov030_02109d60->taskId_Player));
}

s32 func_ov030_020af618(void) {
    return Fld_Player_IsWalking(EasyTask_GetTaskById(&data_ov030_02109d60->taskPool, data_ov030_02109d60->taskId_Player));
}

void Field_GetPlayerScreenPos(s16* out0, s16* out1) {
    FieldEventManager* mgr = data_ov030_02109d60;

    *out0 = F2I(gSaveData.fieldPos.x - mgr->origin.x);
    *out1 = F2I(gSaveData.fieldPos.y - mgr->origin.y - Field_GetPlayerHeight());
}

s32 Field_ResolvePresetCoord(s32 x) {
    if (x >= -1021 && x < -1010) {
        return FieldPresetCoords[x + 1021];
    }
    return x;
}

void func_ov030_020af6d4(s32 mode) {
    FieldBg_SetTopBgMode(data_ov030_02109d60, mode);
}

void Field_SetTouchEventResult(s32 result) {
    data_ov030_02109d60->touchEventResult = result;
}

void Field_SetTouchEventTarget(s16 target) {
    data_ov030_02109d60->touchEventTarget = target;
    SndMgr_StartPlayingSE(SEIDX_SE_DIALOG_NEXT);
}

s32 func_ov030_020af730(void) {
    return data_ov030_02109d60->unk_21AF0;
}

s32 func_ov030_020af748(void) {
    return data_ov030_02109d60->unk_21AD4;
}

void FieldScan_AddTime(s32 frames) {
    data_ov030_02109d60->scanTimer += frames;
}

void FieldScan_RaiseBgLevel(s32 category) {
    FieldEventManager* mgr = data_ov030_02109d60;

    if (category >= 23 && mgr->scanBgLevel < 23) {
        mgr->scanBgLevel = category;
        FieldBg_UpdateTopBgPalette(mgr);
        FieldBg_UpdateScanPicturePalette(mgr);
    } else if (category >= 9 && mgr->scanBgLevel < 9) {
        mgr->scanBgLevel = category;
        FieldBg_UpdateTopBgPalette(mgr);
        FieldBg_UpdateScanPicturePalette(mgr);
    }
}

void func_ov030_020af7f0(void) {
    FieldEventManager* mgr = data_ov030_02109d60;

    FieldBg_ShowEventPicture(mgr);
    if (mgr->taskId_Badge != -1) {
        Fld_Badge_SetPriority(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Badge), 3);
    }
}

BOOL func_ov030_020af834(s32 arg0, s32 arg1) {
    FieldEventManager* mgr = data_ov030_02109d60;
    s32                start;
    s32                end;

    if (arg0 < 32) {
        start = 0;
        end   = 32;
    } else {
        start = 32;
        end   = 64;
    }
    for (s32 i = start; i < end; i++) {
        if (i != arg0 && Fld_Piranha_IsNear(&mgr->taskPool, mgr->unk_21924[i], arg1)) {
            return TRUE;
        }
    }
    return FALSE;
}

s32 Field_GetMaxChainedEncounters(void) {
    s32 maxChainedEncounters = 0;

    if (Inventory_HasRequiredQuantity(ITEM_STICKER_CHAIN_4, 1, 0)) {
        maxChainedEncounters = 4;
    }
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_CHAIN_16, 1, 0)) {
        maxChainedEncounters = 16;
    }
    return maxChainedEncounters;
}

BOOL FieldScan_CanChainEncounter(void) {
    FieldEventManager* mgr = data_ov030_02109d60;

    return mgr->chainedEncounters < Field_GetMaxChainedEncounters();
}

void FieldScan_AddChainedEncounter(void) {
    data_ov030_02109d60->chainedEncounters++;
}

s16 Field_AddEncounterHumanCount(s32 delta) {
    FieldEventManager* mgr = data_ov030_02109d60;

    mgr->encounterHumanCount += delta;
    if (mgr->encounterHumanCount < 0) {
        mgr->encounterHumanCount = 0;
    }
    return mgr->encounterHumanCount;
}

void Field_SetEncounterHumanCount(s16 count) {
    data_ov030_02109d60->encounterHumanCount = count;
}

FieldPos* Field_GetCameraOrigin(void) {
    return &data_ov030_02109d60->origin;
}

BOOL func_ov030_020af9b4(void) {
    if (gSaveData.battleCount != 1) {
        return FALSE;
    }
    for (u32 i = 0; i < 17; i++) {
        if (gSaveData.battleEncounters[0] == data_ov030_020d9964[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov030_020afa04(void) {
    for (s32 i = 0; i < gSaveData.scanSymbolCount; i++) {
        if (gSaveData.scanSymbols[i].state == 2 && gSaveData.scanSymbols[i].category == 8) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov030_020afa58(void) {
    for (s32 i = 0; i < gSaveData.scanSymbolCount; i++) {
        if (gSaveData.scanSymbols[i].state == 2) {
            s16 category = gSaveData.scanSymbols[i].category;
            if (category >= 23 && category < 31) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void FieldScan_Update(FieldEventManager* mgr) {
    switch (mgr->scanPhase) {
        case 0: {
            if (mgr->scanTimer != 0) {
                if (mgr->scanTimer == 25) {
                    FieldBg_LoadScanSubBg0(mgr);
                    FieldScan_MarkSeen(-2);
                }
                if (mgr->scanTimer < 23) {
                    EasyFade_FadeSubDisplay(FADER_LINEAR, 0, 2621);
                }
                func_02026590(&mgr->scanSlideY, 0, (u16)mgr->scanTimer);
                Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG0, 0, mgr->scanSlideY);
                mgr->scanTimer--;
            } else {
                EasyFade_FadeSubDisplay(FADER_SMOOTH, 16, 0);
                mgr->scanTimer = 1;
                mgr->scanPhase++;
            }
        } break;

        case 1: {
            if (mgr->scanTimer != 0) {
                mgr->scanTimer--;
                g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG0 | LAYER_BG1;
                g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].priority = 1;
                g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].priority = 1;
                FieldBg_LoadScanFrameSubBg0(mgr);
                FieldBg_LoadScanSubBg1(mgr);
                Field_PlayNekuVoice();
                Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG0, 0, I2F(64));
                Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG1, 0, I2F(64));
            } else {
                mgr->taskId_LargeGingo = Fld_LargeGingo_CreateTask(&mgr->taskPool, mgr->dataType);
                FieldBg_SetTopBgMode(mgr, 4);
                FieldBg_LoadScanPicture(mgr);
                mgr->scanPhase++;
                mgr->blendEva                                         = 0;
                mgr->blendEvb                                         = I2F(16);
                mgr->scanTimer                                        = 75;
                g_DisplaySettings.engineState[DISPLAY_MAIN].blendMode = 1;
                Display_SetBlendLayers(DISPLAY_MAIN, 0x2, 0x33);
                Fld_ScanArea_FadeOut(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_ScanArea));
            }
        } break;

        case 2: {
            EasyFade_FadeSubDisplay(FADER_LINEAR, 0, 873);
            g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1;
            if (mgr->scanTimer != 0) {
                func_02026590(&mgr->blendEva, I2F(16), (u16)mgr->scanTimer);
                func_02026590(&mgr->blendEvb, I2F(6), (u16)mgr->scanTimer);
                mgr->scanTimer--;
                Display_SetBlendCoeffs(DISPLAY_MAIN, F2I(mgr->blendEva), F2I(mgr->blendEvb));
                if (mgr->scanTimer == 0) {
                    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG0;
                    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG1;
                    EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_LargeGingo);
                }
            } else {
                mgr->scanPhase++;
            }
        } break;

        case 3: {
            if (FieldBg_IsBadgeTapped(mgr)) {
                SndMgr_StartPlayingSE(SEIDX_SE_SCANS_CLOSE);
                mgr->scanPhase++;
                EasyFade_FadeBothDisplays(FADER_LINEAR, 16, FX32_ONE);
            } else if (Fld_Luck_TrySelectSymbol(&mgr->taskPool, mgr->taskId_NSymbolUGMgr, mgr->taskId_Badge)) {
                mgr->scanBgLevel = gSaveData.scanSymbols[gSaveData.scanSymbolSelection].category;
                if (mgr->scanBgLevel == 22) {
                    mgr->exitRequest = 7;
                    DebugOvlDisp_Pop();
                } else {
                    mgr->taskId_Raster = EasyTask_CreateTask(&mgr->taskPool, &Tsk_Fld_Raster, NULL, 0, NULL, NULL);
                    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG2;
                    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG3;
                    FieldBg_LoadScanLevelSubBg1(mgr);
                    FieldBg_LoadScanSubBg3(mgr);
                    FieldBg_LoadScanLevelTopBg(mgr);
                    FieldBg_UpdateScanPicturePalette(mgr);
                    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].priority = 2;
                    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].priority = 1;
                    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].priority = 3;
                    g_DisplaySettings.engineState[DISPLAY_SUB].blendMode              = 1;
                    Display_SetBlendLayers(DISPLAY_SUB, 0xC, 0x22);
                    mgr->blendEva = 0;
                    mgr->blendEvb = I2F(16);
                    Display_SetBlendCoeffs(DISPLAY_SUB, F2I(mgr->blendEva), F2I(mgr->blendEvb));
                    g_DisplaySettings.engineState[DISPLAY_MAIN].blendMode              = 0;
                    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 3;
                    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21B24);
                    Field_DestroyHud(mgr);
                    Fld_Badge_SetVisible(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Badge), 0);
                    for (s32 i = 0; i < mgr->humanCount; i++) {
                        EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_Humans[i]);
                    }
                    mgr->humanCount = 0;
                    mgr->scanPhase  = 6;
                    mgr->scanTimer  = 90;
                }
            }
        } break;

        case 4: {
            if (!EasyFade_IsFading()) {
                if (mgr->taskId_Raster != -1) {
                    EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_Raster);
                    mgr->taskId_Raster = -1;
                }
                g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG1;
                FieldBg_SetTopBgMode(mgr, 1);
                EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21B24);
                Fld_ScanArea_SetTargetScale(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_ScanArea), 0);
                g_DisplaySettings.engineState[DISPLAY_MAIN].blendMode = 1;
                Display_SetBlendLayers(DISPLAY_MAIN, 0x1, 0x3F);
                Display_SetBlendCoeffs(DISPLAY_MAIN, 10, 6);
                FieldBg_ReleaseSubBg0(mgr);
                FieldBg_ReleaseSubBg1(mgr);
                Field_ReleaseEventPicture(mgr);
                mgr->unk_21A6C = BgResMgr_AllocScreen(g_BgResourceManagers[1], data_0205a124,
                                                      g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].screenBase, 0);
                mgr->unk_21A78 = BgResMgr_AllocScreen(g_BgResourceManagers[1], data_0205a124,
                                                      g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].screenBase, 0);
                BgResMgr_ReleaseScreen(g_BgResourceManagers[1], (BgResource*)mgr->unk_21A6C);
                BgResMgr_ReleaseScreen(g_BgResourceManagers[1], (BgResource*)mgr->unk_21A78);
                mgr->unk_21A6C                                                    = 0;
                mgr->unk_21A78                                                    = 0;
                g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].priority = 2;
                g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].priority = 2;
                mgr->scanPhase++;
                EasyFade_FadeBothDisplays(FADER_LINEAR, 0, FX32_ONE);
                EasyTask_DeleteTask(&mgr->taskPool, mgr->taskId_NSymbolUGMgr);
            }
        } break;

        case 5: {
            mgr->scanActive = 0;
        } break;

        case 6: {
            g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2 | LAYER_BG3;
            if (mgr->scanTimer != 0) {
                func_02026590(&mgr->blendEva, I2F(16), (u16)mgr->scanTimer);
                func_02026590(&mgr->blendEvb, 0, (u16)mgr->scanTimer);
                mgr->scanTimer--;
                Display_SetBlendCoeffs(DISPLAY_SUB, F2I(mgr->blendEva), F2I(mgr->blendEvb));
            } else {
                mgr->scanPhase = 7;
                mgr->scanTimer = 30;
                Fld_Player_StartBattleIn(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Player));
                Fld_Partner_StartBattleIn(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Partner));
                Fld_Luck_RemoveActiveSymbols(&mgr->taskPool, mgr->taskId_NSymbolUGMgr);
            }
        } break;

        case 7: {
            if (mgr->scanTimer != 0) {
                mgr->scanTimer--;
            } else {
                mgr->exitRequest              = 2;
                gSaveData.unk_3130            = 0;
                gSaveData.unk_3134            = 1;
                gSaveData.unk_3138            = func_ov030_020af2fc();
                gSaveData.unk_3142            = 0;
                gSaveData.battleDifficulty    = gSaveData.playerStats.difficulty;
                gSaveData.battleEncounters[0] = gSaveData.scanSymbols[gSaveData.scanSymbolSelection].encounter;
                gSaveData.unk_3140            = 0;
                if (gSaveData.scanSymbols[gSaveData.scanSymbolSelection].category >= 31) {
                    gSaveData.unk_3140 |= 1;
                }

                s16 encounter = gSaveData.scanSymbols[gSaveData.scanSymbolSelection].encounter;
                if (encounter == 88) {
                    gSaveData.unk_3130 = 1;
                    gSaveData.unk_3134 = 51;
                } else if (encounter == 89) {
                    gSaveData.unk_3130 = 1;
                    gSaveData.unk_3134 = 52;
                } else if (encounter == 90) {
                    gSaveData.unk_3130 = 1;
                    gSaveData.unk_3134 = 50;
                }

                if (func_ov030_020af9b4()) {
                    gSaveData.unk_3130 = 2;
                }
                if (func_ov030_020afa04()) {
                    gSaveData.battleDifficulty = 1;
                }
                if (func_ov030_020afa58()) {
                    gSaveData.unk_3140 |= 1;
                }
                gSaveData.battleDropRate = Fld_Luck_GetDropRate();
                SndMgr_StartPlayingSE(SEIDX_SE_BATTLE_IN);
                DebugOvlDisp_Pop();
            }
        } break;
    }
}

s16 FieldTalk_GetKeywordCount(void) {
    s16 i;

    for (i = 0; i < 16; i++) {
        if (gSaveData.unk_24C8[i] == -1) {
            return i;
        }
    }
    return i;
}

BOOL FieldEvent_IsDebugSkipHeld(void) {
    if (gSaveData.unk_2498 == 0) {
        return FALSE;
    }
    if (!Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        return FALSE;
    }
    if (SysControl.buttonState.holdButtons & (INPUT_BUTTON_L | INPUT_BUTTON_R)) {
        return TRUE;
    }
    return FALSE;
}
