#include "Debug/OpenEnd.h"
#include "Display.h"
#include "EasyFade.h"
#include "TouchInput.h"
#include "common_data.h"

/*Nonmatching: Regswaps, Opcode reorder*/
void OpenEnd_InitHardware(void) {
    Interrupts_Init();
    func_0200434c();
    DMA_Init(0x100);
    Interrupts_ForceVBlank();
    GX_Init();
    data_02066aec = 0;
    data_02066eec = 0;
    GXx_SetMasterBrightness(&REG_MASTER_BRIGHT, 0 - 0x10);
    GXx_SetMasterBrightness(&REG_DISP_B_MASTER_BRIGHT, ~0xf);
    GX_DispOn();
    REG_DISPCNT_SUB |= 0x10000;
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForBg(0x1);
    GX_SetBankForObj(0x2);
    GX_SetBankForBgExtPltt(0x10);
    GX_SetBankForSubBg(0x4);
    GX_SetBankForSubObj(0x8);
    GX_SetBankForSubBgExtPltt(0x80);
    g_DisplaySettings.unk_000 = 0;
    REG_POWER_CNT &= ~0x8000;
    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    DisplayBGSettings* mainBg0 = Display_GetBG0Settings(DISPLAY_MAIN);
    mainBg0->bgMode            = 0;
    mainBg0->screenSizeText    = 0;
    mainBg0->screenBase        = 0;
    mainBg0->extPlttSlot       = 0;
    mainBg0->colorMode         = 1;
    mainBg0->charBase          = 2;

    if (g_DisplaySettings.controls[DISPLAY_MAIN].dimension == GX2D3D_MODE_2D) {
        REG_BG0CNT = (REG_BG0CNT & 0x43) | 0x88;
    }

    DisplayBGSettings* mainBg1 = Display_GetBG1Settings(DISPLAY_MAIN);
    mainBg1->colorMode         = 1;
    mainBg1->screenBase        = 2;
    mainBg1->bgMode            = 0;
    mainBg1->screenSizeText    = 0;
    mainBg1->extPlttSlot       = 0;
    mainBg1->charBase          = 4;
    REG_BG1CNT                 = (REG_BG1CNT & 0x43) | 0x290;
    // Display_InitMainBG1(0, 0, 1, 2, 4, 0, 0x290); reduces matching %

    g_DisplaySettings.controls[DISPLAY_MAIN].layers = LAYER_BG0 | LAYER_OBJ;
    g_DisplaySettings.controls[DISPLAY_SUB].bgMode  = GX_BGMODE_0;

    GXs_SetGraphicsMode(0);

    DisplayBGSettings* subBg0 = Display_GetBG0Settings(DISPLAY_SUB);
    subBg0->bgMode            = 0;
    subBg0->screenSizeText    = 0;
    subBg0->colorMode         = 1;
    subBg0->screenBase        = 0;
    subBg0->charBase          = 2;
    subBg0->extPlttSlot       = 0;
    REG_BG0CNT_SUB            = (REG_BG0CNT_SUB & 0x43) | 0x88;
    // Display_InitSubBG0(0, 0, 0, 0, 2, 0, 0x88); reduces matching %

    DisplayBGSettings* subBg1 = Display_GetBG1Settings(DISPLAY_SUB);
    subBg1->charBase          = 4;
    subBg1->bgMode            = 0;
    subBg1->screenSizeText    = 0;
    subBg1->colorMode         = 1;
    subBg1->screenBase        = 2;
    subBg1->extPlttSlot       = 0;
    REG_BG1CNT_SUB            = (REG_BG1CNT_SUB & 0x43) | 0x290;
    // Display_InitSubBG1(4, 0, 0, 1, 2, 0, 0x290); reduces matching %

    g_DisplaySettings.controls[DISPLAY_SUB].layers       = 17;
    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_128K;

    func_0200270c(0, 0);
    func_0200270c(1, 0);
    MI_CpuFill(0x0, 0x6800000, 0xa4000);
    MI_CpuFill(0x0, 0x6000000, 0x80000);
    MI_CpuFill(0x0, 0x6200000, 0x20000);
    MI_CpuFill(0x0, 0x6400000, 0x40000);
    MI_CpuFill(0x0, 0x6600000, 0x20000);
    func_0200270c(0, 0);
    func_0200283c(0, &data_020676ec, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200270c(0, 1);
    func_0200283c(0, &data_02068778, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_02001c34(&data_02066aec, &data_0205a128, 0x0, 0x200, 1);
    func_02001c34(&data_02066eec, &data_0205a128, 0x0, 0x200, 1);
}

void OpenEnd_VBlankHandler(void) {
    if (SystemStatusFlags.vblank != FALSE) {
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(&data_0206770c, 0x400);
        GX_LoadOam(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 0x400);
        GXs_LoadOam(&data_02068798, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
    }
}

void OpenEnd_InitWithVBlank(void) {
    OpenEnd_InitHardware();
    Interrupts_RegisterVBlankCallback(OpenEnd_VBlankHandler, TRUE);
}

void OpenEnd_ReleaseVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

// Load/Use .nbfs file
void OpenEnd_LoadScreenSpriteData(s32 r0, s32 r1, void* buffer, s32 r3, s32 dataSize) {
    if (r0 == 0) {
        if (r1 == 0) {
            func_020373c0(buffer, r3, dataSize);
        }
        if (r1 == 1) {
            func_02037480(buffer, r3, dataSize);
        }
        if (r1 == 2) {
            func_02037540(buffer, r3, dataSize);
        }
        if (r1 == 3) {
            func_02037600(buffer, r3, dataSize);
        }
    }
    if (r0 != 1) {
        return;
    }
    if (r1 == 0) {
        func_02037420(buffer, r3, dataSize);
    }
    if (r1 == 1) {
        func_020374e0(buffer, r3, dataSize);
    }
    if (r1 == 2) {
        func_020375a0(buffer, r3, dataSize);
    }
    if (r1 != 3) {
        return;
    }
    func_02037660(buffer, r3, dataSize);
}

// Load/Use .nbfc file
void OpenEnd_LoadScreenCharacterData(s32 r0, s32 r1, void* buffer, s32 r3, s32 dataSize) {
    if (r0 == 0) {
        if (r1 == 0) {
            func_020376c0(buffer, r3, dataSize);
        }
        if (r1 == 1) {
            func_02037780(buffer, r3, dataSize);
        }
        if (r1 == 2) {
            func_02037840(buffer, r3, dataSize);
        }
        if (r1 == 3) {
            func_02037900(buffer, r3, dataSize);
        }
    }
    if (r0 != 1) {
        return;
    }
    if (r1 == 0) {
        func_02037720(buffer, r3, dataSize);
    }
    if (r1 == 1) {
        func_020377e0(buffer, r3, dataSize);
    }
    if (r1 == 2) {
        func_020378a0(buffer, r3, dataSize);
    }
    if (r1 != 3) {
        return;
    }
    func_02037960(buffer, r3, dataSize);
}

// Load/Use .nbfp file
void OpenEnd_LoadScreenPaletteData(s32 r0, void* buffer, s32 r2, s32 dataSize) {
    if (r0 == 0) {
        GX_BeginLoadBgExtPltt();
        GX_LoadBgExtPltt(buffer, r2, dataSize);
        GX_EndLoadBgExtPltt();
    }
    if (r0 != 1) {
        return;
    }
    func_02037c10();                     // GX_BeginLoadBgExtPltt() but for sub display?
    func_02037c28(buffer, r2, dataSize); // GX_LoadBgExtPltt() but for sub display?
    func_02037c8c();                     // GX_EndLoadBgExtPltt() but for sub display?
    return;
}

void OpenEnd_LoadScreenAssets(OpenEndState* state, s32 r1, s32 r2, s32 screenIndex) {
    for (s32 idx = 0; idx < 3; idx++) {
        state->dataList[idx] = DatMgr_LoadRawData(state->dataType, 0, 0, &OpenEnd_FileList[(screenIndex * 3) + 1 + idx]);
    }
    DC_PurgeAll();
    OpenEnd_LoadScreenSpriteData(r1, r2, state->dataList[2]->buffer, 0, state->dataList[2]->dataSize);
    OpenEnd_LoadScreenCharacterData(r1, r2, state->dataList[0]->buffer, 0, state->dataList[0]->dataSize);
    OpenEnd_LoadScreenPaletteData(r1, state->dataList[1]->buffer, r2 << 0xd, state->dataList[1]->dataSize);
    for (s32 idx = 0; idx < 3; idx++) {
        DatMgr_ReleaseData(state->dataList[idx]);
    }
}

void OpenEnd_FadeToBlack(OpenEndState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, state->fadeRate);
    if (EasyFade_IsFading() == 0) {
        DebugOvlDisp_Pop();
    }
}

void OpenEnd_FadeFromBlack(OpenEndState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, state->fadeBrightness, state->fadeRate);
    if (EasyFade_IsFading() == 0) {
        DebugOvlDisp_Pop();
    }
}

void OpenEnd_ValidateSaveData(OpenEndState* state) {
    if (state->isSaveValid != 0)
        return;
    s32 saveStatus     = func_02024aa4();
    state->unk_11A38   = saveStatus;
    state->isSaveValid = 1;
    state->unk_11A40   = 0;
    if (saveStatus == 0) {
        if ((data_02073710.unk_B6 & 1) != FALSE) {
            state->isThereExistingSaveData = 1;
        }
        return;
    } else if (saveStatus != 1) {
        state->unk_11A48 = 1;
        return;
    }
    func_02024d04();
    state->unk_11A40 = 1;
}

void OpenEnd_IntroScreen_Init(OpenEndState* state) {
    OpenEnd_ValidateSaveData(state);
    OpenEnd_LoadScreenAssets(state, 1, 0, 0); // RG_SE
    OpenEnd_LoadScreenAssets(state, 0, 0, 1); // UG_JP
    OpenEnd_LoadScreenAssets(state, 1, 1, 2); // RG_NIN
    OpenEnd_LoadScreenAssets(state, 0, 1, 3); // UG_CRI
    state->introFrameCount = 0;
    state->fadeBrightness  = data_ov037_02083a7c[data_02074d10.unk_410];
    state->fadeRate        = data_ov037_02083a74[data_02074d10.unk_410];
    DebugOvlDisp_Pop();
}

void OpenEnd_IntroScreen_Update(OpenEndState* state) {
    switch (state->sequenceStep) {
        case 0:
            if (state->introFrameCount > 120) {
                state->introFrameCount = 0;
                state->sequenceStep++;
                break;
            }
            state->introFrameCount++;
            break;
        case 1:
            EasyFade_FadeMainDisplay(FADER_LINEAR, -16, 0x333);
            EasyFade_FadeSubDisplay(FADER_LINEAR, -16, 0x333);
            if (EasyFade_IsFading() != 0)
                break;
            state->introFrameCount = 0;
            state->sequenceStep++;
            break;
        case 2:
            if (state->introFrameCount == 0) {
                g_DisplaySettings.controls[DISPLAY_MAIN].layers = g_DisplaySettings.controls[DISPLAY_MAIN].layers & ~0x1 | 0x2;
                g_DisplaySettings.controls[DISPLAY_SUB].layers  = g_DisplaySettings.controls[DISPLAY_SUB].layers & ~0x1 | 0x2;
            }
            EasyFade_FadeMainDisplay(FADER_LINEAR, 0, 0x333);
            EasyFade_FadeSubDisplay(FADER_LINEAR, 0, 0x333);
            if (EasyFade_IsFading() == 0) {
                state->introFrameCount = 0;
                state->sequenceStep++;
                break;
            }
            state->introFrameCount++;
            break;
        case 3:
            if (state->introFrameCount > 120) {
                DebugOvlDisp_Pop();
                break;
            }
            state->introFrameCount++;
            break;
    }
}

extern void func_ov002_02086acc(void* r0);
void        OpenEnd_IntroScreen_Destroy(OpenEndState* state) {
    OverlayTag tag;
    data_02074d10.unk_410 = 1;
    if (state->unk_11A40 != 0) {
        MainOvlDisp_ReplaceTop(&tag, &OVERLAY_2_ID, func_ov002_02086acc, NULL, 0);
    } else {
        OpenEnd_PlayScreenSequence(state);
    }
}

/*Nonmatching: regswap */
extern void ProcessOverlay_OtosuMenu_DataCorrupted(void* state);
extern void ProcessOverlay_OtosuMenu_DataLoadFailure(void* state);
void        OpenEnd_TitleScreen_Init(OpenEndState* state) {
    OverlayTag tag, tag2;
    OpenEnd_ValidateSaveData(state);
    if (state->unk_11A48 != 0) {
        if (state->unk_11A38 == 2) {
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_2_ID, ProcessOverlay_OtosuMenu_DataLoadFailure, 0, 0);
        } else {
            MainOvlDisp_ReplaceTop(&tag2, &OVERLAY_2_ID, ProcessOverlay_OtosuMenu_DataCorrupted, 0, 0);
        }
        DebugOvlDisp_Init();
        return;
    }
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = (g_DisplaySettings.controls[DISPLAY_MAIN].layers | 0x1) & ~0x2;
    g_DisplaySettings.controls[DISPLAY_SUB].layers  = (g_DisplaySettings.controls[DISPLAY_SUB].layers | 0x1) & ~0x2;
    OpenEnd_LoadScreenAssets(state, 0, 0, 5); // OpeningBG_UGstart
    OpenEnd_LoadScreenAssets(state, 1, 0, 4); // OpeningBG_RG
    OpenEnd_CreateBadgeTask(BADGE_NEWGAME);
    if (state->isThereExistingSaveData != FALSE) {
        OpenEnd_CreateBadgeTask(BADGE_CONTINUE);
    }
    state->sequenceStep    = 0;
    state->titleFrameCount = 0;
    state->fadeBrightness  = data_ov037_02083a7c[data_02074d10.unk_410]; // Regswap here
    state->fadeRate        = data_ov037_02083a74[data_02074d10.unk_410]; // and here
    state->alreadySelected = 0;
    func_0202733c();
    DebugOvlDisp_Pop();
}

/* Nonmatching: Opcode reordering*/
void OpenEnd_TitleScreen_Update(OpenEndState* state) {
    TouchCoord coords;

    TouchInput_GetCoord(&coords);
    switch (state->sequenceStep) {
        case 0:
            if (state->titleFrameCount <= 648000) {
                state->titleFrameCount++;
            } else {
                CriSndMgr_Stop(0);
                state->titleFrameCount = 0;
                state->sequenceStep++;
            }
            break;
        case 1:
            if (state->titleFrameCount <= 10) {
                state->titleFrameCount++;
            } else {
                func_0202733c(0);
                state->sequenceStep    = 0;
                state->titleFrameCount = 0;
            }
            break;
    }
    if (state->alreadySelected == FALSE)
        return;
    state->fadeBrightness = -16;
    state->fadeRate       = 0x800;
    DebugOvlDisp_Pop();
}

extern void ProcessOverlay_OtosuMenu_DataDeletion(void* r0);
extern void func_ov030_020b0fe8(void* r0);
void        OpenEnd_StartNewGame(OpenEndState* state) {
    OverlayTag tag, tag2;
    if (state->isThereExistingSaveData != FALSE) {
        MainOvlDisp_ReplaceTop(&tag, &OVERLAY_2_ID, ProcessOverlay_OtosuMenu_DataDeletion, NULL, 0);
    } else {
        MainOvlDisp_ReplaceTop(&tag2, &OVERLAY_30_ID, func_ov030_020b0fe8, NULL, 0); // Load game ?
    }
}

extern void func_ov044_02084a88();
extern void func_ov030_020ae92c();
void        OpenEnd_ContinueGame(OpenEndState* state) {
    OverlayTag tag, tag2, tag3, tag4;
    if (func_020256bc() == 0) {
        if (func_02023010(0x2AB) != 0) {
            data_02073710.unk_B4 |= 0x10;
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_44_ID, func_ov044_02084a88, 0, 0); //<-- Overlay44 -> Shutdown PP Gain screen
            return;
        }
        if ((data_02073710.unk_B4 & 0x2) != 0) {
            data_02073710.unk_B4 &= ~0x2;
            MainOvlDisp_ReplaceTop(&tag2, &OVERLAY_30_ID, func_ov030_020b0fe8, 0, 0); // Load game ?
            return;
        }
        MainOvlDisp_ReplaceTop(&tag3, &OVERLAY_30_ID, func_ov030_020ae92c, 0, 0); // Load game ?
        return;
    }
    MainOvlDisp_ReplaceTop(&tag4, &OVERLAY_2_ID, ProcessOverlay_OtosuMenu_DataCorrupted, 0, 0);
}

void OpenEnd_TitleScreen_Destroy(OpenEndState* state) {
    OverlayTag tag, tag2;
    state->isSaveValid = 0;
    OpenEnd_ValidateSaveData(state);
    if (state->unk_11A48 != 0) {
        if (state->unk_11A38 == 2) {
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_2_ID, ProcessOverlay_OtosuMenu_DataLoadFailure, 0, 0);
        } else {
            MainOvlDisp_ReplaceTop(&tag2, &OVERLAY_2_ID, ProcessOverlay_OtosuMenu_DataCorrupted, 0, 0);
        }
        DebugOvlDisp_Init();
        return;
    }
    CriSndMgr_Stop(0);
    switch (state->selectedOption) {
        case BADGE_NEWGAME:
            OpenEnd_StartNewGame(state);
            break;
        case BADGE_CONTINUE:
            OpenEnd_ContinueGame(state);
            break;
    }
    DebugOvlDisp_Pop();
}

void OpenEnd_PlayScreenSequence(OpenEndState* state) {
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)screenFunctions[data_02074d10.unk_410][2], state, 0);
    DebugOvlDisp_Push((OverlayCB)OpenEnd_FadeFromBlack, state, 0);
    DebugOvlDisp_Push((OverlayCB)screenFunctions[data_02074d10.unk_410][1], state, 0);
    DebugOvlDisp_Push((OverlayCB)OpenEnd_FadeToBlack, state, 0);
    DebugOvlDisp_Push((OverlayCB)screenFunctions[data_02074d10.unk_410][0], state, 0);
}

extern vu32        data_02066a58;
static const char* seq_OpenEnd = "Seq_OpenEnd(void *)";
/*Nomatching: regswaps*/
void OpenEnd_Init(OpenEndState* state) {
    if (state == NULL) {
        char* SeqName = seq_OpenEnd;
        void* unk     = Mem_AllocHeapTail(&gDebugHeap, sizeof(OpenEndState));
        Mem_SetSequence(sizeof(OpenEndState), unk, SeqName);
        g_OpenEndstate = unk;
        MainOvlDisp_SetCbArg(unk);
    }
    MI_CpuFill(0, state, Mem_GetBlockSize(&gDebugHeap, state));
    OpenEnd_InitWithVBlank();
    u32 brightness = data_ov037_02083a7c[data_02074d10.unk_410];
    if (data_ov037_02083a7c[data_02074d10.unk_410] > 0x10) {
        brightness = 0x10;
    } else if (data_ov037_02083a7c[data_02074d10.unk_410] < ~0xf) {
        brightness = ~0xF;
    }
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = brightness;
    if (data_ov037_02083a7c[data_02074d10.unk_410] > 0x10) {
        brightness = 0x10;
    } else if (data_ov037_02083a7c[data_02074d10.unk_410] < ~0xf) {
        brightness = ~0xF;
    }
    g_DisplaySettings.controls[DISPLAY_SUB].brightness = brightness;
    func_0200cef0(&(state->unk_10));
    Input_Init(&InputStatus, 8, 1, 2);
    TouchInput_Init();
    TouchInput_Update();
    func_02025b1c();
    data_02066a58 &= ~0x8; // data_02066a58.bit_3 = 0
    state->dataType  = DatMgr_AllocateSlot();
    state->unk_11A38 = 0;
    Mem_InitializeHeap(&state->memPool, &state->unk_1161C, 0x400);
    EasyTask_InitializePool(&state->taskPool, &state->memPool, 0x10, 0, 0);
    g_taskPool = &state->taskPool;
    EasyTask_CreateTask(&state->taskPool, &Task_EasyFade, 0, 0, 0, 0);
    OpenEnd_PlayScreenSequence(state);
    MainOvlDisp_NextProcessStage();
}

void OpenEnd_Update(OpenEndState* state) {
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    TouchInput_Update();
    if (TouchInput_WasTouchPressed() != 0) {
        flag_screenTouched = TRUE;
    }
    EasyTask_UpdatePool(&state->taskPool);
    DebugOvlDisp_Run();
    if (DebugOvlDisp_IsStackAtBase() != 0)
        return;
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc[0], 0);
    func_0200bf60(data_0206b3cc[1], 0);
}

void OpenEnd_Destroy(OpenEndState* state) {
    if (state == NULL) {
        return;
    }
    data_02074d10.unk_410 = 1;
    OpenEnd_ReleaseVBlank();
    DatMgr_ClearSlot(state->dataType);
    EasyTask_DestroyPool(&state->taskPool);
    Mem_Free(&gDebugHeap, state);
    MainOvlDisp_SetCbArg(NULL);
    CriSndMgr_Stop(0);
}

static const OverlayProcess OvlProc_OpenEnd = {
    .init = OpenEnd_Init,
    .main = OpenEnd_Update,
    .exit = OpenEnd_Destroy,
};

void ProcessOverlay_OpenEnd(OpenEndState* state) {
    u32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        OpenEnd_Destroy(state);
    } else {
        OvlProc_OpenEnd.funcs[stage](state);
    }
}

void OpenEnd_OnButtonSelect(u32 r0) {

    if (g_OpenEndstate->alreadySelected != 0)
        return;
    g_OpenEndstate->selectedOption  = r0;
    g_OpenEndstate->alreadySelected = 1;

    SndMgr_StartPlayingSE(2); // SE_pause
}

BOOL OpenEnd_IsInCircle(s32* coords, s32 x, s32 y) {
    s32 xx   = (x - coords[0]) * (x - coords[0]);
    s32 yy   = (y - coords[1]) * (y - coords[1]);
    s32 dSqr = xx + yy;
    s32 maxD = coords[2] * coords[2];
    return dSqr <= maxD;
}

void OpenEnd_InitBadgeAnim(SpriteAnimation* anim, s16 r1, s16 r2, s16 r3, s16 s1) {
    *anim         = g_BadgeAnim;
    anim->binIden = OpenEnd_FileList;
    anim->unk_1C  = r1;
    anim->unk_26  = r2;
    anim->unk_28  = r3;
    anim->unk_2A  = s1;
}

typedef struct {
    /* 0x00 */ Sprite badge;
    /* 0x40 */ Sprite badgeShadow;
    /* 0x80 */ u32    badgeIndex;
} TaskBadge_Data;

s32 OpenEnd_TaskBadge_Init(struct TaskPool* unused_r0, struct Task* r1, void* taskParam) {
    SpriteAnimation anim;
    u32*            r2 = (u32*)taskParam;

    TaskBadge_Data* badgeData = r1->data;
    MI_CpuSet(badgeData, 0, sizeof(TaskBadge_Data));

    u32 badgeIndex        = *r2;
    badgeData->badgeIndex = badgeIndex;
    OpenEnd_InitBadgeAnim(&anim, (badgeIndex * 3) + 2, (badgeIndex * 3) + 3, (badgeIndex * 3) + 4, 1);

    Sprite_Load(&badgeData->badgeShadow, &anim);

    anim.unk_2A = 2;
    Sprite_Load(&badgeData->badge, &anim);

    return 1;
}

const u32 OpenEnd_TitleScreen_BadgeInfo[][5] = {
    {0x22, 0x1F, 0x19, 0x00, 0x00},
    {0xD7, 0x1F, 0x19, 0x01, 0x01}
};

// Nonmatching
s32 OpenEnd_TaskBadge_Update(struct TaskPool* unused_r0, struct Task* r1, void* taskParam) {
    TouchCoord      coords, coords2;
    TaskBadge_Data* badgeData = r1->data;
    TouchInput_GetCoord(&coords2);

    coords = coords2;

    if (flag_screenTouched != 0 && TouchInput_IsTouchActive() != FALSE &&
        OpenEnd_IsInCircle(&OpenEnd_TitleScreen_BadgeInfo[badgeData->badgeIndex][0], coords.x, coords.y) != FALSE)
    {
        badgeData->badge.posX = 130;
        badgeData->badge.posY = 98;
    } else {
        badgeData->badge.posX = 128;
        badgeData->badge.posY = 96;
    }

    if (flag_screenTouched != 0 && TouchInput_WasTouchReleased() != FALSE) {
        if (OpenEnd_IsInCircle(&OpenEnd_TitleScreen_BadgeInfo[badgeData->badgeIndex][0], coords.x, coords.y) != FALSE) {
            OpenEnd_OnButtonSelect(OpenEnd_TitleScreen_BadgeInfo[badgeData->badgeIndex][1]);
        }
    }
    Sprite_Update(&badgeData->badge);
    Sprite_Update(&badgeData->badgeShadow);
    return 1;
}

s32 OpenEnd_TaskBadge_Render(struct TaskPool* unused_r0, struct Task* r1, void* taskParam) {
    TaskBadge_Data* badgeData = r1->data;
    Sprite_RenderFrame(&badgeData->badgeShadow);
    Sprite_RenderFrame(&badgeData->badge);
    return 1;
}

s32 OpenEnd_TaskBadge_CleanUp(struct TaskPool* unused_r0, struct Task* r1, void* taskParam) {
    TaskBadge_Data* badgeData = r1->data;
    Sprite_Release(&badgeData->badge);
    Sprite_Release(&badgeData->badgeShadow);
    return 1;
}

s32 OpenEnd_TaskBadge_RunTask(struct TaskPool* pool, struct Task* task, void* taskParam, s32 index) {
    const TaskStages data_ov037_02083b4c = {
        .initialize = OpenEnd_TaskBadge_Init,
        .update     = OpenEnd_TaskBadge_Update,
        .render     = OpenEnd_TaskBadge_Render,
        .cleanup    = OpenEnd_TaskBadge_CleanUp,
    };
    return data_ov037_02083b4c.iter[index](pool, task, taskParam);
}

s32 OpenEnd_CreateBadgeTask(u32 idx) {
    static const TaskHandle TaskHandle_OpenEnd_Badge = {"Tsk_OpenEnd_Badge", OpenEnd_TaskBadge_RunTask, 0x84};
    u32                     badge                    = idx;
    return EasyTask_CreateTask(g_taskPool, &TaskHandle_OpenEnd_Badge, NULL, 0, 0, &badge);
}