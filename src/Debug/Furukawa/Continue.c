#include "Debug/Furukawa/Continue.h"
#include "BgResMgr.h"
#include "CriSndMgr.h"
#include "DMA.h"
#include "Display.h"
#include "EasyFade.h"
#include "EasyList.h"
#include "Input.h"
#include "Interrupts.h"
#include "OverlayDispatcher.h"
#include "SndMgr.h"
#include "SpriteMgr.h"
#include "System.h"
#include "Text.h"
#include "TouchInput.h"
#include "common_data.h"

#include <NitroSDK/mi/cpumem.h>

typedef struct {
    /* 0x000 */ char pad0[0x130];
    /* 0x130 */ s32  unk130;        /* inferred */
    /* 0x134 */ char pad134[0xC];   /* maybe part of unk130[4]? */
    /* 0x140 */ u8   unk140;        /* inferred */
    /* 0x141 */ s8   unk141;        /* inferred */
    /* 0x142 */ char pad142[0x2CE]; /* maybe part of unk141[0x2CF]? */
    /* 0x410 */ s32  unk_410;
} UnkStruct_020740d10_local;        /* size = 0x414 */

typedef struct {
    /* 0x00 */ char pad_00[0x28];
    /* 0x28 */ u16  unk_28;
    /* 0x2A */ char pad_2A[0x18];
    /* 0x42 */ u16  unk42;
} UnkStruct_02074e10_local;

typedef struct {
    /* 0x00 */ char pad_00[0x19];
    /* 0x19 */ u8   unk19;
} UnkStruct_02071d10_local;

typedef struct {
    /* 0x00 */ char pad_00[0xB4];
    /* 0xB4 */ s16  unkB4;
} UnkStruct_02074110_local;

extern void func_02001c34(s16*, s32*, void*, void*, s32);
extern void func_0200270c(void*, void*);
extern void func_0200283c(s32*, void*, void*);
extern void func_02003440(s32*);
extern void func_020034b0(s32*);
extern void func_0200434c();
extern void func_0200adf8(s32, void*, void*, void*, s32);
extern void func_0200bf60(s32, void*);
extern void func_0200cef0(void*);
extern void func_0200d120(s32);
extern void func_020265d4(void*, void*, u16);
extern void func_ov003_020825b8(s32, void*, void*);
extern void func_ov003_02082724(void*, s32, s16);
extern void func_ov003_02082b0c(void*);
extern void func_ov003_02082b64(void*);
extern void func_ov003_02082cc4(s32);
extern void func_ov003_02082d04(void*);
extern void func_ov003_0208ec74(void);
extern void func_ov027_020e860c(void);
extern void func_ov030_020ae92c(void*, s32);
extern void func_ov037_0208370c(void);

void func_ov025_020e7f90(ContinueObject* contObj);
void func_ov025_020e7fdc(ContinueObject* contObj);

extern UnkStruct_02071d10_local data_02071d10;
extern UnkStruct_02074110_local data_02074110;

static const char* data_ov025_020e8320 = "Seq_Continue( )";

static const BinIdentifier data_ov025_020e8308 = {25, "Apl_Fur/Grp_Continue.bin"};

void func_ov025_020e7360(void) {
    Interrupts_Init();
    func_0200434c();
    GX_Init();
    DMA_Init(0x100);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(GX_VRAM_ALL);
    GX_SetBankForBg(GX_VRAM_A);
    GX_SetBankForObj(GX_VRAM_B);
    GX_SetBankForSubBg(GX_VRAM_C);
    GX_SetBankForSubObj(GX_VRAM_D);
    g_DisplaySettings.controls[0].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objTileMode = GX_OBJTILEMODE_1D_128K;
    func_0200270c(0, 0);
    func_0200270c(0, 1);
    MI_CpuFill(0, 0x06800000, 0xA4000);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    func_02001c34(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    func_02001c34(&data_02066eec, &data_0205a128, 0, 0x200, 1);
}

void func_ov025_020e749c(void) {
    g_DisplaySettings.controls[0].brightness = 16;
    g_DisplaySettings.controls[1].brightness = 16;
    Display_CommitSynced();
    g_DisplaySettings.unk_000 = 0;

    REG_POWER_CNT &= ~0x8000;

    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 1, 0, 0x104);

    g_DisplaySettings.engineState[0].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic   = 0;

    g_DisplaySettings.controls[0].layers = 0x12;
    g_DisplaySettings.controls[1].bgMode = GX_BGMODE_0;

    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 4, 0, 0x110);

    g_DisplaySettings.engineState[1].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic   = 0;

    g_DisplaySettings.controls[1].layers = 2;
}

static void Continue_VBlank(void) {
    if (SystemStatusFlags.vblank) {
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(&data_0206770c, 0x400);
        GX_LoadOam(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 0x400);
        GXs_LoadOam(&data_02068798, 0, 0x400);
        DC_PurgeRange((s32*)&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange((s32*)&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
    }
}

static void Continue_RegisterVBlank(void) {
    func_ov025_020e7360();
    func_ov025_020e749c();
    Interrupts_RegisterVBlankCallback(Continue_VBlank, TRUE);
}

static void Continue_ReleaseVBlank(void) {
    func_ov025_020e7360();
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

void func_ov025_020e76b4(ContinueObject* arg0) {
    arg0->unk_11E0C = DatMgr_LoadRawData(1, NULL, 0, &data_ov025_020e8308);

    void* var_r1 = Data_GetPackEntryData(arg0->unk_11E0C, 1);
    void* var_r4 = Data_GetPackEntryData(arg0->unk_11E0C, 2);
    void* var_r5 = Data_GetPackEntryData(arg0->unk_11E0C, 3);

    BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r1, g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, 0x6000);
    BgResMgr_AllocScreen(g_BgResourceManagers[1], var_r4, g_DisplaySettings.engineState[1].bgSettings[1].screenBase,
                         g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText);
    func_0200adf8(data_0206b3cc[1], var_r5, 0, 0, 7);

    void* var_r1_2 = Data_GetPackEntryData(arg0->unk_11E0C, 4);
    void* var_r5_2 = Data_GetPackEntryData(arg0->unk_11E0C, 5);
    void* var_r4_2 = Data_GetPackEntryData(arg0->unk_11E0C, 6);

    BgResMgr_AllocChar32(g_BgResourceManagers[0], var_r1_2, g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0,
                         0x6000);
    BgResMgr_AllocScreen(g_BgResourceManagers[0], var_r5_2, g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                         g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);
    func_0200adf8(data_0206b3cc[0], var_r4_2, 0, 0, 7);
}

void func_ov025_020e785c(ContinueUnkStruct* arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4) {
    arg0->unk_60 = arg1;
    arg0->unk_62 = arg3;
    arg0->unk_64 = arg4;
    func_ov003_02082a90(0, arg0, &data_ov025_020e8308, 7, 9, 8, 10, arg2);
    func_ov003_02082724(arg0, arg3, arg4);
}

// Nonmatching: Unreachable branch?
// Scratch: heKBA
static void func_ov025_020e78c8(ContinueObject* contObj, u32 arg1) {
    contObj->unk_120D0 = arg1;

    if ((s32)contObj->unk_120D0 <= 3) {
        if ((s32)contObj->unk_120D0 < 0 || (s32)contObj->unk_120D0 == 0xFFFF) {
            return;
        }
        switch (contObj->unk_120D0) {
            case 0:
                func_ov003_02082724(&contObj->unk_11FB0, 0x80, 0x56);
                break;
            case 1:
                func_ov003_02082724(&contObj->unk_11FB0, 0x80, 0x6B);
                break;
            case 2:
                func_ov003_02082724(&contObj->unk_11FB0, 0x80, 0x80);
                break;
            case 3:
                func_ov003_02082724(&contObj->unk_11FB0, 0x80, 0x95);
                break;
        }
        func_ov003_02082d04(&contObj->unk_11FB0);
    }
}

s32 func_ov025_020e797c(ContinueObject* arg0) {
    u16 result = 0xFFFF;

    TouchCoord touchCoord;
    TouchInput_GetCoord(&touchCoord);

    for (s32 index = 0; index < 4; index++) {
        ContinueUnkStruct* blob = &arg0->unk_11E10[index];

        if ((touchCoord.x >= (blob->unk_62 - 72)) && (touchCoord.x <= (blob->unk_62 + 72)) &&
            (touchCoord.y >= (blob->unk_64 - 10)) && (touchCoord.y <= (blob->unk_64 + 10)))
        {
            result = blob->unk_60;
            break;
        }
    }

    if ((result == 2) && (((UnkStruct_020740d10_local*)&data_02074d10)->unk140 & 1)) {
        result = 0xFFFF;
    }

    return result;
}

void func_ov025_020e7a20(ContinueObject* arg0) {
    if (TouchInput_WasTouchPressed() == FALSE) {
        return;
    }

    s32 temp_r0 = func_ov025_020e797c(arg0);

    if (temp_r0 != arg0->unk_120D0) {
        if (temp_r0 != 0xFFFF) {
            SndMgr_StartPlayingSE(0x2A);
        }
        func_ov025_020e78c8(arg0, temp_r0);
    } else if (arg0->unk_120D0 != 0xFFFF) {
        SndMgr_StartPlayingSE(0x2B);
        arg0->unk_120DC = 0;
        arg0->unk_120DE = 0;
        DebugOvlDisp_ReplaceTop((OverlayCB)func_ov025_020e7f90, arg0, PROCESS_STAGE_INIT);
    }
}

void func_ov025_020e7aac(ContinueObject* contObj) {
    func_ov025_020e76b4(contObj);
    func_ov025_020e785c(&contObj->unk_11E10[0], 0, 1, 0x80, 0x56);
    func_ov025_020e785c(&contObj->unk_11E10[1], 1, 2, 0x80, 0x6B);
    func_ov025_020e785c(&contObj->unk_11E10[3], 3, 3, 0x80, 0x95);
    if (((UnkStruct_020740d10_local*)&data_02074d10)->unk140 & 1) {
        func_ov025_020e785c(&contObj->unk_11E10[2], 2, 5, 0x80, 0x80);
    } else {
        func_ov025_020e785c(&contObj->unk_11E10[2], 2, 4, 0x80, 0x80);
    }
    func_ov003_02082a90(0, &contObj->unk_11FB0, &data_ov025_020e8308, 7, 9, 8, 10, 0);
    func_ov003_020825b8(&contObj->unk_11FB0, 0, 1);
    func_ov025_020e78c8(contObj, 0xFFFF);
    func_ov003_02082a90(0, &contObj->unk_12010, &data_ov025_020e8308, 7, 9, 8, 10, 6);
    func_ov003_02082a90(0, &contObj->unk_12070, &data_ov025_020e8308, 7, 9, 8, 10, 7);
    contObj->unk_120D4 = 0x148000;
    contObj->unk_120D8 = -0x48000;
}

void func_ov025_020e7c40(ContinueObject* contObj) {
    if (contObj->unk_120D0 != 0xFFFF) {
        func_ov003_02082b0c(&contObj->unk_11FB0);
    }
    func_ov003_02082b0c(&contObj->unk_11E10[0]);
    func_ov003_02082b0c(&contObj->unk_11E10[1]);
    func_ov003_02082b0c(&contObj->unk_11E10[2]);
    func_ov003_02082b0c(&contObj->unk_11E10[3]);
    func_ov003_02082b0c(&contObj->unk_12010);
    func_ov003_02082b0c(&contObj->unk_12070);
}

void func_ov025_020e7cb8(ContinueObject* contObj) {
    if (contObj->unk_120D0 != 0xFFFF) {
        func_ov003_02082b64(&contObj->unk_11FB0);
    }
    func_ov003_02082b64(&contObj->unk_11E10[0]);
    func_ov003_02082b64(&contObj->unk_11E10[1]);
    func_ov003_02082b64(&contObj->unk_11E10[2]);
    func_ov003_02082b64(&contObj->unk_11E10[3]);
    func_ov003_02082724(&contObj->unk_12010, ((contObj->unk_120D4 * 16) >> 0x10), 0x60);
    func_ov003_02082724(&contObj->unk_12070, ((contObj->unk_120D8 * 16) >> 0x10), 0x60);
    func_ov003_02082b64(&contObj->unk_12010);
    func_ov003_02082b64(&contObj->unk_12070);
}

void func_ov025_020e7d70(ContinueObject* contObj) {
    func_ov003_02082cc4(&contObj->unk_11FB0);
    func_ov003_02082cc4(&contObj->unk_11E10[0]);
    func_ov003_02082cc4(&contObj->unk_11E10[1]);
    func_ov003_02082cc4(&contObj->unk_11E10[2]);
    func_ov003_02082cc4(&contObj->unk_11E10[3]);
    func_ov003_02082cc4(&contObj->unk_12010);
    func_ov003_02082cc4(&contObj->unk_12070);
}

void func_ov025_020e7dd0(ContinueObject* contObj) {
    switch (contObj->unk_120D0) {
        case 0: {
            ((UnkStruct_02074e10_local*)&data_02074e10)->unk42 = 0;
            ((UnkStruct_020740d10_local*)&data_02074d10)->unk141 =
                (s8)((u32)(((UnkStruct_02071d10_local*)&data_02071d10)->unk19 << 0x1E) >> 0x1E);

            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_3_ID, &func_ov003_0208ec74, NULL, PROCESS_STAGE_INIT);
        } break;

        case 1: {
            ((UnkStruct_02074e10_local*)&data_02074e10)->unk42   = 0;
            ((UnkStruct_020740d10_local*)&data_02074d10)->unk141 = 0;

            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_3_ID, &func_ov003_0208ec74, NULL, PROCESS_STAGE_INIT);
        } break;

        case 2: {
            if (((UnkStruct_020740d10_local*)&data_02074d10)->unk140 & 2) {
                data_02074110.unkB4 = 5;
            } else {
                data_02074110.unkB4 = 3;
            }
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_30_ID, &func_ov030_020ae92c, NULL, PROCESS_STAGE_INIT);
        } break;

        default:
        case 3: {

            if (((UnkStruct_020740d10_local*)&data_02074d10)->unk130 == 3) {
                OverlayTag tag;
                MainOvlDisp_ReplaceTop(&tag, &OVERLAY_27_ID, &func_ov027_020e860c, NULL, PROCESS_STAGE_INIT);
            } else {
                OverlayTag tag;
                MainOvlDisp_ReplaceTop(&tag, &OVERLAY_37_ID, &func_ov037_0208370c, NULL, PROCESS_STAGE_INIT);
            }
        } break;
    }
}

void func_ov025_020e7f28(ContinueObject* contObj) {
    if (contObj->unk_120DC == 0) {
        EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
        contObj->unk_120DC++;
    }
    if (EasyFade_IsFading() == FALSE) {
        contObj->unk_120DC = 0;
        contObj->unk_120DE = 0;
        DebugOvlDisp_ReplaceTop((OverlayCB)func_ov025_020e7fdc, contObj, PROCESS_STAGE_INIT);
    }
}

void func_ov025_020e7f90(ContinueObject* contObj) {
    if (contObj->unk_120DC == 0) {
        EasyFade_FadeBothDisplays(FADER_LINEAR, 16, 0x1000);
        contObj->unk_120DC++;
    }
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void func_ov025_020e7fdc(ContinueObject* contObj) {
    if (contObj->unk_120DC == 0) {
        if (contObj->unk_120DE == 0) {
            contObj->unk_120DE = 16;
        }
        if (contObj->unk_120DE > 0) {
            func_020265d4(&contObj->unk_120D4, 0x80000, contObj->unk_120DE);
            func_020265d4(&contObj->unk_120D8, 0x80000, contObj->unk_120DE);
            contObj->unk_120DE--;
        }
        if (contObj->unk_120DE <= 0) {
            contObj->unk_120DC = 0;
            contObj->unk_120DE = 0;
        }
    }
    func_ov025_020e7a20(contObj);
}

void Continue_Init(ContinueObject* contObj) {
    if (contObj == NULL) {
        const char* sequence = data_ov025_020e8320;
        contObj              = Mem_AllocHeapTail(&gDebugHeap, sizeof(ContinueObject));
        Mem_SetSequence(&gDebugHeap, contObj, sequence);
        MainOvlDisp_SetCbArg(contObj);
    }
    MI_CpuFill(0, contObj, Mem_GetBlockSize(&gDebugHeap, contObj));
    OvlMgr_LoadOverlay(3, &OVERLAY_3_ID);
    Mem_InitializeHeap(&contObj->memPool, contObj->memBuffer, sizeof(contObj->memBuffer));
    EasyTask_InitializePool(&contObj->taskPool, &contObj->memPool, 8, NULL, NULL);
    Continue_RegisterVBlank();
    func_0200cef0(&contObj->unk_0088C);
    data_02066aec = 0;
    data_02066eec = 0;
    Input_Init(&InputStatus, 12, 2, 1);
    TouchInput_Init();
    EasyTask_CreateTask(&contObj->taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    func_ov025_020e7aac(contObj);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)func_ov025_020e7f28, contObj, 0);
    MainOvlDisp_NextProcessStage();
    MainOvlDisp_Run();
}

void Continue_Update(ContinueObject* contObj) {
    TouchInput_Update();
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_ov025_020e7c40(contObj);
    func_ov025_020e7cb8(contObj);
    DebugOvlDisp_Run();

    if (DebugOvlDisp_IsStackAtBase() != 0) {
        func_ov025_020e7dd0(contObj);
    } else {
        EasyTask_UpdatePool(&contObj->taskPool);
        func_020034b0(&data_020676ec);
        func_020034b0(&data_02068778);
        func_0200bf60(data_0206b3cc[0], 0);
        func_0200bf60(data_0206b3cc[1], 0);
    }
}

void Continue_Destroy(ContinueObject* contObj) {
    if (contObj != NULL) {
        func_ov025_020e7d70(contObj);
        func_0200d120(&contObj->unk_0088C);
        DatMgr_ReleaseData(contObj->unk_11E0C);
        Continue_ReleaseVBlank();
        EasyTask_DestroyPool(&contObj->taskPool);
        Mem_Free(&gDebugHeap, contObj);
        MainOvlDisp_SetCbArg(NULL);
        OvlMgr_UnloadOverlay(3);
    }
}

static const OverlayProcess OvlProc_Continue = {
    .init = Continue_Init,
    .main = Continue_Update,
    .exit = Continue_Destroy,
};

void ProcessOverlay_Continue(void* object) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        Continue_Destroy(object);
    } else {
        OvlProc_Continue.funcs[stage](object);
    }
}