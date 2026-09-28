#include "Interface/Menu/Top.h"
#include "CriSndMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/HBlank.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Input.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <nitro/fs/overlay.h>
#include <nitro/mi/cpumem.h>
#include <nitro/reg.h>

typedef struct {
    /* 0x00000 */ ResourceManager  resMgr;
    /* 0x11580 */ ResourceManager* prevResMgr;
    /* 0x11584 */ s32              spareDataType;
    /* 0x11588 */ s32              dataType;
    /* 0x1158C */ Heap             heap;
    /* 0x11598 */ u8               heapBuffer[0x10000];
    /* 0x21598 */ TaskPool         taskPool;
    /* 0x21618 */ s32              unk_21618;
    /* 0x2161C */ s32              taskId_LuckNum;
    /* 0x21620 */ s32              taskId_LuckStar;
    /* 0x21624 */ s32              taskId_LvGauge;
    /* 0x21628 */ s32              taskId_Select[8];
    /* 0x21648 */ s32              taskId_Icon;
    /* 0x2164C */ s32              taskId_NameD;
    /* 0x21650 */ s32              taskId_NumLV;
    /* 0x21654 */ s32              taskId_NumExpNext;
    /* 0x21658 */ s32              taskId_NumHP;
    /* 0x2165C */ s32              taskId_NumMoney;
    /* 0x21660 */ s32              unk_21660;
    /* 0x21664 */ s32              taskId_DrawDiff;
    /* 0x21668 */ s32              taskId_DrawPtrAI;
    /* 0x2166C */ s32              taskId_SelDiff;
    /* 0x21670 */ s32              taskId_SelPtrAI;
    /* 0x21674 */ s32              unk_21674;
    /* 0x21678 */ s32              taskId_TextScr;
    /* 0x2167C */ s32              taskId_HelpCur;
    /* 0x21680 */ s32              taskId_Pointer;
    /* 0x21684 */ s32              taskId_NameU;
    /* 0x21688 */ s32              taskId_RankNumU[4];
    /* 0x21698 */ s32              taskId_MapTipU[21]; // 23 are created; the last two land in taskId_MapTipBrdU[0..1]
    /* 0x216EC */ s32              taskId_MapTipBrdU[21];
    /* 0x21740 */ s32              taskId_BrdLogoU;
    /* 0x21744 */ s32              taskId_NekuU;
    /* 0x21748 */ s32              taskId_IconU[46];
    /* 0x21800 */ s32              taskId_TextScrU;
    /* 0x21804 */ s32              taskId_HelpCurU;
    /* 0x21808 */ s16              timer;
    /* 0x2180A */ char             unk_2180A[0x2180C - 0x2180A];
    /* 0x2180C */ s32              exitReady;
    /* 0x21810 */ MenuTopObject    topMenu;
} MenuTopState; // Size: 0x21958

void GX_LoadBgPltt(void* src, u32 offset, u32 size);
void GX_LoadObjPltt(void* src, u32 offset, u32 size);
void GXs_LoadBgPltt(void* src, u32 offset, u32 size);
void GXs_LoadObjPltt(void* src, u32 offset, u32 size);
void func_02023d00(s32);
s32  func_02023d1c(s32);
void func_0202b878(void);

extern void ProcessOverlay_MenuEquip(void* state);
extern void func_ov028_020e82d0(void* state);
extern void func_ov043_0208f44c(void* state);
extern void func_ov043_0209bce4(void* state);
extern void func_ov043_020c04f0(void* state);

void MenuTop_InitState(MenuTopState* state);
void MenuTop_CreateTasks(MenuTopState* state);
void MenuTop_StageFadeIn(MenuTopState* state);
void MenuTop_StageOpenHelp(MenuTopState* state);
void MenuTop_StageMain(MenuTopState* state);
void MenuTop_StageHelp(MenuTopState* state);
void MenuTop_StageCloseHelp(MenuTopState* state);
void MenuTop_StageFadeOut(MenuTopState* state);
void MenuTop_Init(MenuTopState* state);
void MenuTop_Update(MenuTopState* state);
void MenuTop_Destroy(MenuTopState* state);
void MenuTop_RegisterVBlank(void);
void MenuTop_DeregisterVBlank(void);

static const char* data_ov043_020cb5f4 = "Seq_MenuTop()";

static const OverlayProcess OvlProc_MenuTop = {
    .init = (OverlayCB)MenuTop_Init,
    .main = (OverlayCB)MenuTop_Update,
    .exit = (OverlayCB)MenuTop_Destroy,
};

// Map position of each area, indexed by MenuTopObject.currentArea. Areas 22-34 and 35+ share the last two.
const Point MenuTop_AreaMapPos[23] = {
    {0xD7, 0x78},
    {0xC4, 0x76},
    {0xD2, 0x6D},
    {0xE4, 0x81},
    {0xC9, 0x64},
    {0xBC, 0x5D},
    {0xC1, 0x51},
    {0xAF, 0x49},
    {0xC8, 0x43},
    {0xA8, 0x3A},
    {0xD7, 0x5D},
    {0xDF, 0x51},
    {0xDB, 0x43},
    {0xF0, 0x52},
    {0xEA, 0x46},
    {0xF8, 0x36},
    {0xD7, 0x8F},
    {0xF2, 0x94},
    {0xAC, 0x87},
    {0x92, 0x7F},
    {0x9B, 0x5F},
    {0xB4, 0xA4},
    {0xBE, 0xB2},
};

const BinIdentifier MenuTop_BinIdentifiers[14] = {
    [1] = {0x2B,      "Apl_Tak/Grp_Menu_BGD.bin"},
      [6] = {0x2B,      "Apl_Tak/Grp_Menu_BGU.bin"},
    [12] = {0x2B,      "Apl_Tak/Grp_MenuIcon.bin"},
      [11] = {0x2B,      "Apl_Tak/Grp_MenuLuck.bin"},
    [13] = {0x2B,     "Apl_Tak/Grp_BrandLogo.bin"},
      [9] = {0x2B,  "Apl_Tak/Grp_Menu_fontSCR.bin"},
    [10] = {0x2B,  "Apl_Tak/Grp_Menu_fontSCR.bin"},
      [4] = {0x2B, "Apl_Tak/Grp_MenuTop_OBD02.bin"},
    [5] = {0x2B, "Apl_Tak/Grp_MenuTop_BGU00.bin"},
      [2] = {0x2B, "Apl_Tak/Grp_MenuTop_OBD00.bin"},
    [3] = {0x2B, "Apl_Tak/Grp_MenuTop_OBD01.bin"},
      [7] = {0x2B, "Apl_Tak/Grp_MenuTop_OBU00.bin"},
    [8] = {0x2B, "Apl_Tak/Grp_MenuTop_OBU01.bin"},
      [0] = {0x2B, "Apl_Tak/Grp_MenuTop_BGD00.bin"},
};

void MenuTop_InitState(MenuTopState* state) {
    MenuTopObject* topMenu = &state->topMenu;

    state->exitReady        = 0;
    state->timer            = 0;
    topMenu->flags          = 0;
    topMenu->iconPressed[0] = 0;
    topMenu->iconPressed[1] = 0;
    topMenu->pressTimer     = 0;
    topMenu->selectedEntry  = 0;
    topMenu->popupOpen      = 0;
    topMenu->upperPage      = 0;
    topMenu->unk_5A         = 0;
    topMenu->unk_5B         = 0;
    topMenu->blinkTimer     = 0;
    topMenu->blinkPhase     = 0;
    topMenu->blinkOn        = 0;
    for (u16 i = 0; i < 3; i++) {
        topMenu->helpPressed[i] = 0;
    }
    topMenu->helpPage = 0;
    MenuTop_LoadFromSave(topMenu);
}

void MenuTop_CreateTasks(MenuTopState* state) {
    MenuTopObject* topMenu = &state->topMenu;
    u16            area;
    u16            iconCount;

    EasyTask_CreateTask(&state->taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -0x10, 0x1000);
    state->taskId_Pointer  = MenuTop_pointer_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_LuckNum  = MenuTop_luckNum_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_LuckStar = MenuTop_luckStar_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_LvGauge  = MenuTop_lvGauge_CreateTask(&state->taskPool, state->dataType, topMenu);
    for (u16 i = 0; i < 8; i++) {
        state->taskId_Select[i] = MenuTop_select_CreateTask(&state->taskPool, state->dataType, (s32)topMenu, i);
    }
    state->taskId_Icon       = MenuTop_icon_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_NameD      = MenuTop_nameD_CreateTask(&state->taskPool, state->dataType, (s32)topMenu);
    state->taskId_NumLV      = MenuTop_numLV_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_NumExpNext = MenuTop_numExpNext_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_NumHP      = MenuTop_numHP_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_NumMoney   = MenuTop_numMoney_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_DrawDiff   = MenuTop_drawDiff_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_DrawPtrAI  = MenuTop_drawPtrAI_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_TextScr    = MenuTop_textScr_CreateTask(&state->taskPool, state->dataType, topMenu);
    if (topMenu->currentArea <= 20) {
        state->taskId_NameU       = MenuTop_nameU_CreateTask(&state->taskPool, state->dataType, (s32)topMenu);
        state->taskId_RankNumU[0] = MenuTop_rankNumU_CreateTask(&state->taskPool, state->dataType, 0, 1, (s32)topMenu);
        state->taskId_RankNumU[1] = MenuTop_rankNumU_CreateTask(&state->taskPool, state->dataType, 1, 2, (s32)topMenu);
        state->taskId_RankNumU[2] = MenuTop_rankNumU_CreateTask(&state->taskPool, state->dataType, 2, 3, (s32)topMenu);
        state->taskId_RankNumU[3] = MenuTop_rankNumU_CreateTask(&state->taskPool, state->dataType, 3, 13, (s32)topMenu);
    }
    for (u16 i = 0; i < 21; i++) {
        state->taskId_MapTipU[i]    = MenuTop_mapTipU_CreateTask(&state->taskPool, state->dataType, i, topMenu);
        state->taskId_MapTipBrdU[i] = MenuTop_mapTipBrdU_CreateTask(&state->taskPool, state->dataType, i, topMenu);
    }
    state->taskId_MapTipU[21] = MenuTop_mapTipU_CreateTask(&state->taskPool, state->dataType, 21, topMenu);
    state->taskId_MapTipU[22] = MenuTop_mapTipU_CreateTask(&state->taskPool, state->dataType, 22, topMenu);
    state->taskId_BrdLogoU    = MenuTop_brdLogoU_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_TextScrU    = MenuTop_textScrU_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_NekuU       = MenuTop_nekuU_CreateTask(&state->taskPool, state->dataType, topMenu);
    iconCount                 = 0;
    for (area = 0; area < 41; area++) {
        if (((s32(*)(u8, s32))func_0202366c)(area, 4) == 1) {
            state->taskId_IconU[iconCount] = MenuTop_iconU_CreateTask(&state->taskPool, state->dataType, topMenu, area, 15);
            iconCount++;
        } else if (((s32(*)(u8, s32))func_0202366c)(area, 2) == 1) {
            state->taskId_IconU[iconCount] = MenuTop_iconU_CreateTask(&state->taskPool, state->dataType, topMenu, area, 14);
            iconCount++;
        }
    }
}

void MenuTop_StageFadeIn(MenuTopState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        if (func_02023d1c(0) == 0) {
            state->timer = 30;
            DebugOvlDisp_ReplaceTop((OverlayCB)MenuTop_StageOpenHelp, state, PROCESS_STAGE_INIT);
        } else {
            DebugOvlDisp_ReplaceTop((OverlayCB)MenuTop_StageMain, state, PROCESS_STAGE_INIT);
        }
    }
}

void MenuTop_StageOpenHelp(MenuTopState* state) {
    MenuTopObject* topMenu = &state->topMenu;

    if (state->timer > 0) {
        state->timer--;
        return;
    }

    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1 | LAYER_BG2;
    g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG0;
    state->taskId_HelpCur  = MenuTop_helpCur_CreateTask(&state->taskPool, state->dataType, topMenu);
    state->taskId_HelpCurU = MenuTop_helpCurU_CreateTask(&state->taskPool, state->dataType, (s32)topMenu);
    topMenu->upperPage     = 2;
    topMenu->flags |= 2;
    DebugOvlDisp_ReplaceTop((OverlayCB)MenuTop_StageHelp, state, PROCESS_STAGE_INIT);
}

void MenuTop_StageMain(MenuTopState* state) {
    MenuTopObject* topMenu = &state->topMenu;
    TouchCoord     coord;

    if (topMenu->flags & 0x1000) {
        return;
    }

    if (topMenu->popupOpen == 0) {
        if (TouchInput_IsTouchActive()) {
            TouchInput_GetCoord(&coord);
            if (!(topMenu->flags & 1)) {
                if (MenuTop_IsPointOnDifficulty(coord.x, coord.y) == 1) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    topMenu->flags |= 8;
                    state->taskId_SelDiff = MenuTop_selDiff_CreateTask(&state->taskPool, state->dataType, topMenu);
                    g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG0;
                    topMenu->popupOpen = 1;
                    return;
                }
                if (MenuTop_IsPointOnPartnerAI(coord.x, coord.y) == 1) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    topMenu->flags |= 8;
                    state->taskId_SelPtrAI = MenuTop_selPtrAI_CreateTask(&state->taskPool, state->dataType, topMenu);
                    g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG0;
                    topMenu->popupOpen = 1;
                    return;
                }
            }
        }

        if (TouchInput_WasTouchPressed()) {
            s16 entry;
            s16 button;

            TouchInput_GetCoord(&coord);
            entry = MenuTop_GetEntryAtPoint(coord.x, coord.y);
            if (entry != -1 && MenuTop_IsEntryAvailable(entry) == 1) {
                topMenu->selectedEntry = entry;
                topMenu->flags |= 0x1000;
                SndMgr_StartPlayingSE(SEIDX_MENU_MSCRATCH);
                if (entry == 5) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MEXIT);
                }
                DebugOvlDisp_Pop();
                return;
            }

            button = MenuTop_GetIconButtonAtPoint(coord.x, coord.y);
            if (button != -1) {
                topMenu->iconPressed[button] = 1;
                topMenu->pressTimer          = 12;
                if (button == 0) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
                    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1 | LAYER_BG2;
                    g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG0;
                    state->taskId_HelpCur  = MenuTop_helpCur_CreateTask(&state->taskPool, state->dataType, topMenu);
                    state->taskId_HelpCurU = MenuTop_helpCurU_CreateTask(&state->taskPool, state->dataType, (s32)topMenu);
                    topMenu->upperPage     = 2;
                    topMenu->flags |= 2;
                    DebugOvlDisp_ReplaceTop((OverlayCB)MenuTop_StageHelp, state, PROCESS_STAGE_INIT);
                } else {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
                    SndMgr_StartPlayingSE(SEIDX_MENU_MEXIT);
                    topMenu->flags |= 0x1000;
                    topMenu->selectedEntry = 9;
                    DebugOvlDisp_Pop();
                }
            }
        } else if (InputStatus.buttonState.pressedButtons & 8) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
            SndMgr_StartPlayingSE(SEIDX_MENU_MEXIT);
            topMenu->flags |= 0x1000;
            topMenu->selectedEntry = 9;
            DebugOvlDisp_Pop();
        }
    }

    if (topMenu->blinkTimer > 0) {
        topMenu->blinkTimer--;
        return;
    }
    if (topMenu->blinkOn != 0) {
        topMenu->blinkOn = 0;
    } else {
        topMenu->blinkOn = 1;
    }
    topMenu->blinkTimer = 40;
    topMenu->blinkPhase = 1 - topMenu->blinkPhase;
}

void MenuTop_StageHelp(MenuTopState* state) {
    MenuTopObject* topMenu = &state->topMenu;
    TouchCoord     coord;

    if (TouchInput_WasTouchPressed()) {
        s16 button;

        TouchInput_GetCoord(&coord);
        button = MenuTop_GetHelpButtonAtPoint(coord.x, coord.y);
        if (button != -1) {
            if (topMenu->pressTimer != 0) {
                return;
            }
            topMenu->helpPressed[button] = 1;
            topMenu->pressTimer          = 12;
            if (button == 0) {
                if (topMenu->helpPage != 0) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    topMenu->helpPage--;
                    topMenu->flags |= 2;
                }
            } else if (button == 1) {
                if (topMenu->helpPage < 6) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    topMenu->helpPage++;
                    topMenu->flags |= 2;
                }
            } else if (func_02023d1c(0) == 1) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
                DebugOvlDisp_ReplaceTop((OverlayCB)MenuTop_StageCloseHelp, state, PROCESS_STAGE_INIT);
            }
        }
    }
    if (func_02023d1c(0) == 0) {
        func_02023d00(0);
    }
}

void MenuTop_StageCloseHelp(MenuTopState* state) {
    MenuTopObject* topMenu = &state->topMenu;

    if (topMenu->helpPressed[2] != 0) {
        return;
    }
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG1;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG2;
    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG0;
    EasyTask_DeleteTask(&state->taskPool, state->taskId_HelpCur);
    EasyTask_DeleteTask(&state->taskPool, state->taskId_HelpCurU);
    topMenu->helpPage  = 0;
    topMenu->upperPage = 0;
    topMenu->flags |= 2;
    DebugOvlDisp_ReplaceTop((OverlayCB)MenuTop_StageMain, state, PROCESS_STAGE_INIT);
}

void MenuTop_StageFadeOut(MenuTopState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0x10, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void MenuTop_Init(MenuTopState* state) {
    if (state == NULL) {
        const char* sequence = data_ov043_020cb5f4;
        state                = Mem_AllocHeapTail(&gDebugHeap, sizeof(MenuTopState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        MainOvlDisp_SetCbArg(state);
    }
    state->spareDataType = DatMgr_AllocateSlot();
    state->dataType      = DatMgr_AllocateSlot();
    MenuTop_RegisterVBlank();
    state->prevResMgr = ResourceMgr_ReinitManagers(&state->resMgr);
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
    TouchInput_Init();
    Mem_InitializeHeap(&state->heap, state->heapBuffer, sizeof(state->heapBuffer));
    EasyTask_InitializePool(&state->taskPool, &state->heap, 0x80, NULL, NULL);
    FS_LoadOverlay(0, (u32)&OVERLAY_31_ID);
    data_02066aec = 0;
    data_02066eec = 0;
    MenuTop_InitState(state);
    MenuTop_LoadBackgrounds(&state->topMenu);
    MenuTop_CreateTasks(state);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)MenuTop_StageFadeOut, state, PROCESS_STAGE_INIT);
    DebugOvlDisp_Push((OverlayCB)MenuTop_StageFadeIn, state, PROCESS_STAGE_INIT);
    MainOvlDisp_NextProcessStage();
    if (gSaveData.unk_1AB4 & 1) {
        CriSndMgr_PlayFile(gSaveData.bgmFile);
        gSaveData.unk_1AB4 &= ~1;
    }
    gSaveData.unk_24B4 = 1;
}

void MenuTop_Update(MenuTopState* state) {
    MenuTopObject* topMenu = &state->topMenu;

    TouchInput_Update();
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    MenuTop_UpdateBackgrounds(topMenu);
    DebugOvlDisp_Run();
    EasyTask_UpdatePool(&state->taskPool);
    if (DebugOvlDisp_IsStackAtBase() == TRUE) {
        state->exitReady = 1;
    }
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);

    if (state->exitReady == 0) {
        return;
    }

    switch (topMenu->selectedEntry) {
        case 0: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_45_ID, (void*)0x02091034, NULL, PROCESS_STAGE_INIT);
        } break;
        case 1: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_28_ID, func_ov028_020e82d0, NULL, PROCESS_STAGE_INIT);
        } break;
        case 2: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_43_ID, ProcessOverlay_MenuEquip, NULL, PROCESS_STAGE_INIT);
        } break;
        case 3: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_43_ID, func_ov043_0208f44c, NULL, PROCESS_STAGE_INIT);
        } break;
        case 4: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_43_ID, func_ov043_0209bce4, NULL, PROCESS_STAGE_INIT);
        } break;
        case 5: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_2_ID, (void*)0x020868CC, NULL, PROCESS_STAGE_INIT);
        } break;
        case 6: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_45_ID, (void*)0x02088700, NULL, PROCESS_STAGE_INIT);
        } break;
        case 7: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_43_ID, func_ov043_020c04f0, NULL, PROCESS_STAGE_INIT);
        } break;
        case 9: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_30_ID, (void*)0x020AE92C, NULL, PROCESS_STAGE_INIT);
        } break;
        default: {
            OverlayTag tag;
            MainOvlDisp_Pop(&tag);
        } break;
    }
}

void MenuTop_Destroy(MenuTopState* state) {
    MenuTopObject* topMenu = &state->topMenu;

    switch (topMenu->selectedEntry) {
        case 0:
        case 1:
        case 5:
        case 6:
        case 7:
        case 9:
            CriSndMgr_Stop(ADX_TITLE);
            gSaveData.unk_1AB4 |= 1;
            break;
    }
    MenuTop_WriteBackToSave(topMenu);
    MenuTop_ReleaseBackgrounds(topMenu);
    EasyTask_DestroyPool(&state->taskPool);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->spareDataType);
    DatMgr_ClearSlot(state->dataType);
    MenuTop_DeregisterVBlank();
    FS_UnloadOverlay(0, (u32)&OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
}

void ProcessOverlay_MenuTop(MenuTopState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        MenuTop_Destroy(state);
    } else {
        OvlProc_MenuTop.funcs[stage](state);
    }
}

void MenuTop_InitDisplay(void) {
    Interrupts_Init();
    HBlank_Init();
    GX_Init();
    func_0202b878();
    DMA_Init(0x100);
    Display_Init();
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
    GX_SetBankForSubObjExtPltt(GX_VRAM_I);
    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    MI_CpuFill(0, (void*)0x06000000, 0x80000);
    MI_CpuFill(0, (void*)0x06200000, 0x20000);
    MI_CpuFill(0, (void*)0x06400000, 0x40000);
    MI_CpuFill(0, (void*)0x06600000, 0x20000);
    REG_POWER_CNT |= 0x8000;
    Display_CommitSynced();
    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    Display_InitMainBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 3, 0, 0xC);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 1, 0, 0x104);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 1, 1, 0x204);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 3, 1, 1, 0x304);

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode   = GX_OBJBMPMODE_1D_128K;
    data_0206aa78                                        = 0x300010;
    data_0206aa7c                                        = 0x400040;
    Display_SetMainLayers(LAYER_BG0 | LAYER_BG3 | LAYER_OBJ);
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 16;

    g_DisplaySettings.controls[DISPLAY_SUB].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);
    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 3, 0, 0x10C);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 1, 1, 0x204);
    // Not Display_InitSubBG3: extPlttSlot is written before charBase here.
    {
        DisplayBGSettings* bg3Settings = Display_GetBG3Settings(DISPLAY_SUB);
        bg3Settings->bgMode            = DISPLAY_BGMODE_TEXT;
        bg3Settings->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
        bg3Settings->colorMode         = GX_BG_COLORS_16;
        bg3Settings->screenBase        = 3;
        bg3Settings->extPlttSlot       = 1;
        bg3Settings->charBase          = 5;
        REG_BG3CNT_SUB                 = (REG_BG3CNT_SUB & 0x43) | 0x314;
    }

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
    Display_SetSubLayers(LAYER_BG1 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);
    g_DisplaySettings.controls[DISPLAY_SUB].brightness = 16;

    g_DisplaySettings.engineState[DISPLAY_MAIN].blendMode   = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0 = 4;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1 = 0x38;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = 6;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = 10;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendMode    = 1;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0  = 1;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1  = 0x3E;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0  = 6;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1  = 10;

    OamMgr_InitEngine(0, DISPLAY_MAIN);
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    DC_PurgeRange(g_OamMgr[DISPLAY_MAIN].oam, 0x400);
    GX_LoadOam(g_OamMgr[DISPLAY_MAIN].oam, 0, 0x400);
    OamMgr_InitEngine(0, DISPLAY_SUB);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    DC_PurgeRange(g_OamMgr[DISPLAY_SUB].oam, 0x400);
    GXs_LoadOam(g_OamMgr[DISPLAY_SUB].oam, 0, 0x400);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
}

void MenuTop_VBlank(void) {
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
    }
}

void MenuTop_RegisterVBlank(void) {
    MenuTop_InitDisplay();
    Interrupts_RegisterVBlankCallback(MenuTop_VBlank, 1);
}

void MenuTop_DeregisterVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}
