#include "Debug/Takami/Depart.h"
#include "EasyFade.h"
#include "Interrupts.h"
#include "OverlayDispatcher.h"
#include "SpriteMgr.h"
#include "System.h"
#include "TouchInput.h"
#include "common_data.h"

extern void func_ov030_020ae92c();
extern void func_ov043_020aeee0();

struct Position {
    s16 x;
    s16 y;
} data_ov043_020ccd00[6] = {
    {  0,  0},
    {130, 98},
    {125, 79},
    {121, 61},
    {115, 42},
    {110, 24}
};

DepartState* data_ov043_020cd28c = NULL;

void func_ov043_020bdb2c(void* arg0);
void func_ov043_020bd8cc(void);
void func_ov043_020bd8e8(void);
void func_ov043_020be328(DepartUnk*);
void func_ov043_020bdc6c(DepartUnk*);
void func_ov043_020be254(DepartUnk*);
void func_ov043_020be32c(DepartUnk*);

s32 DepartExit_CreateTask(TaskPool* pool, s32 arg1, void* arg2);
s32 DepartPanel_CreateTask(TaskPool* pool, s32 arg1, u16 arg2, void* arg3);
s32 DepartBoard_CreateTask(TaskPool* pool, s32 arg1, u16 arg2, s32 arg3);
s32 DepartCur_CreateTask(TaskPool* pool, s32 arg1, void* arg2);
s32 DepartTextScr_CreateTask(TaskPool* pool, s32 arg1, void* arg2);
s32 DepartTextScrU_CreateTask(TaskPool* pool, s32 arg1, s32 arg2);

void func_ov043_020bcdb0(DepartState* state) {
    DepartUnk* temp = &state->unk_2165C;

    state->unk_21650 = 0;
    state->unk_21654 = 0;
    state->unk_21656 = 0;
    temp->unk_00     = 0;
    temp->unk_02     = 0;
    temp->unk_03     = 0;
    temp->unk_30     = 0;
    temp->unk_31     = 0;
    func_ov043_020bdb2c(temp);
}

void func_ov043_020bcdfc(DepartState* state) {
    DepartUnk* temp_r5 = &state->unk_2165C;

    EasyTask_CreateTask(&state->taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -16, 0x1000);

    state->taskId_Exit = DepartExit_CreateTask(&state->taskPool, state->unk_11588, temp_r5);

    for (u16 i = 0; i < temp_r5->unk_0C; i++) {
        state->taskIds_Panel[i] = DepartPanel_CreateTask(&state->taskPool, state->unk_11588, i, temp_r5);
        state->taskIds_Board[i] = DepartBoard_CreateTask(&state->taskPool, state->unk_11588, i, temp_r5);
    }

    state->taskId_Cur      = DepartCur_CreateTask(&state->taskPool, state->unk_11588, temp_r5);
    state->taskId_TextScr  = DepartTextScr_CreateTask(&state->taskPool, state->unk_11588, temp_r5);
    state->taskId_TextScrU = DepartTextScrU_CreateTask(&state->taskPool, state->unk_11588, temp_r5);
}

void func_ov043_020bcf3c(DepartState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void func_ov043_020bcf64(DepartState* state) {
    // Not yet implemented
}

void func_ov043_020bd084(DepartState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 16, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

const char* data_ov043_020ccc48 = "Seq_Depart()";

void func_ov043_020bd0ac(DepartState* state) {
    if (state == NULL) {
        char* sequence = data_ov043_020ccc48;
        state          = Mem_AllocHeapTail(&gDebugHeap, sizeof(DepartState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        data_ov043_020cd28c = state;
        MainOvlDisp_SetCbArg(state);
    }

    DepartUnk* temp = &state->unk_2165C;

    state->unk_11584 = DatMgr_AllocateSlot();
    state->unk_11588 = DatMgr_AllocateSlot();
    func_ov043_020bd8cc();
    state->unk_11580 = func_0200cef0(state);
    TouchInput_Init();
    Mem_InitializeHeap(&state->memPool, state->memPoolBuffer, 0x10000U);
    FS_LoadOverlay(0, &OVERLAY_31_ID);
    EasyTask_InitializePool(&state->taskPool, &state->memPool, 0x200U, NULL, NULL);
    data_02066aec = 0;
    data_02066eec = 0;
    func_ov043_020bcdb0(state);
    func_ov043_020be254(temp);
    func_ov043_020bcdfc(state);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)func_ov043_020bd084, state, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov043_020bcf64, state, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov043_020bcf3c, state, 0);
    MainOvlDisp_NextProcessStage();
    func_0202733c((temp->unk_08 % 3) + 31);
}

void func_ov043_020bd224(DepartState* state) {
    DepartUnk* temp_r4 = &state->unk_2165C;

    TouchInput_Update();
    func_02003c4c();
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02002890(&data_02069804, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_ov043_020be328(temp_r4);
    DebugOvlDisp_Run();
    EasyTask_UpdatePool(&state->taskPool);
    if (DebugOvlDisp_IsStackAtBase() == 1) {
        state->unk_21650 = 1;
    }
    func_02003c68();
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc[0], 0);
    func_0200bf60(data_0206b3cc[1], 0);
    func_0200bf60(data_0206b3cc[2], 0);
    if (state->unk_21650 == 0) {
        return;
    }
    switch (temp_r4->unk_03) {
        case 0: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_30_ID, func_ov030_020ae92c, NULL, 0);
        } break;
        case 1: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_43_ID, func_ov043_020aeee0, NULL, 0);
        } break;
        default: {
            OverlayTag tag;
            MainOvlDisp_Pop(&tag);
        } break;
    }
}

// Nonmatching
void func_ov043_020bd388(DepartState* state) {
    func_02027388(0);
    func_ov043_020bdc6c(&state->unk_2165C);
    func_ov043_020be32c(&state->unk_2165C);
    EasyTask_DestroyPool(&state->taskPool);
    func_0200cef0(NULL);
    DatMgr_ClearSlot(state->unk_11584);
    DatMgr_ClearSlot(state->unk_11588);
    func_ov043_020bd8e8();
    FS_UnloadOverlay(0, &OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
    func_02004a68(&gMainHeap);
    func_02004a68(&gDebugHeap);
}

const OverlayProcess data_ov043_020cad64 = {
    .init = func_ov043_020bd0ac,
    .main = func_ov043_020bd224,
    .exit = func_ov043_020bd388,
};

void func_ov043_020bd414(void* arg0) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        func_ov043_020bd388(arg0);
    } else {
        data_ov043_020cad64.funcs[stage](arg0);
    }
}

// Nonmatching
void func_ov043_020bd454(void) {
    Interrupts_Init();
    func_0200434c();
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
    GX_SetBankForSubObjExtPltt(GX_VRAM_NONE);
    MI_CpuFill(0, 0x06800000, 0xA4000);
    MI_CpuFill(0, 0x06000000, 0x80000);
    MI_CpuFill(0, 0x06200000, 0x20000);
    MI_CpuFill(0, 0x06400000, 0x40000);
    MI_CpuFill(0, 0x06600000, 0x20000);
    *(u16*)0x04000304 &= ~0x8000;
    Display_CommitSynced();
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    DisplayBGSettings* mainBg1 = &g_DisplaySettings.engineState[0].bgSettings[1];
    mainBg1->bgMode            = DISPLAY_BGMODE_TEXT;
    mainBg1->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    mainBg1->colorMode         = GX_BG_COLORS_16;
    mainBg1->screenBase        = 0;
    mainBg1->charBase          = 1;
    mainBg1->extPlttSlot       = 0;
    REG_BG1CNT                 = ((REG_BG1CNT & 0x43) | 4);

    DisplayBGSettings* mainBg2 = &g_DisplaySettings.engineState[0].bgSettings[2];
    mainBg2->bgMode            = DISPLAY_BGMODE_TEXT;
    mainBg2->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    mainBg2->colorMode         = GX_BG_COLORS_16;
    mainBg2->screenBase        = 1;
    mainBg2->charBase          = 3;
    mainBg2->extPlttSlot       = 1;
    REG_BG2CNT                 = ((REG_BG2CNT & 0x43) | 0x10C);

    DisplayBGSettings* mainBg3 = &g_DisplaySettings.engineState[0].bgSettings[3];
    mainBg3->bgMode            = DISPLAY_BGMODE_TEXT;
    mainBg3->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    mainBg3->colorMode         = GX_BG_COLORS_16;
    mainBg3->screenBase        = 2;
    mainBg3->charBase          = 5;
    mainBg3->extPlttSlot       = 1;
    REG_BG3CNT                 = ((REG_BG3CNT & 0x43) | 0x214);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[0].objTileMode = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.controls[0].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[1].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objBmpMode  = GX_OBJBMPMODE_1D_128K;

    data_0206aa78 = 0x300010;
    data_0206aa7c = 0x400040;

    g_DisplaySettings.controls[0].layers     = 0x1D;
    g_DisplaySettings.controls[0].brightness = 0x10;
    g_DisplaySettings.controls[1].bgMode     = GX_BGMODE_0;

    GXs_SetGraphicsMode(GX_BGMODE_0);

    DisplayBGSettings* subBg2 = &g_DisplaySettings.engineState[1].bgSettings[2];
    subBg2->bgMode            = DISPLAY_BGMODE_TEXT;
    subBg2->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    subBg2->colorMode         = GX_BG_COLORS_16;
    subBg2->screenBase        = 0;
    subBg2->charBase          = 1;
    subBg2->extPlttSlot       = 1;
    REG_BG2CNT_SUB            = ((REG_BG2CNT_SUB & 0x43) | 4);

    DisplayBGSettings* subBg3 = &g_DisplaySettings.engineState[1].bgSettings[3];
    subBg3->bgMode            = DISPLAY_BGMODE_TEXT;
    subBg3->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    subBg3->colorMode         = GX_BG_COLORS_16;
    subBg3->screenBase        = 1;
    subBg3->charBase          = 3;
    subBg3->extPlttSlot       = 1;
    REG_BG3CNT_SUB            = ((REG_BG3CNT_SUB & 0x43) | 0x10C);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[1].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objBmpMode  = GX_OBJBMPMODE_1D_128K;

    g_DisplaySettings.controls[1].layers     = 0x1C;
    g_DisplaySettings.controls[1].brightness = 0x10;

    func_02003ad0();
    func_02003c68();
    func_0200270c(0, 0);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200270c(0, 1);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    func_0200270c(0, 2);
    func_02002890(&data_02069804, 0);
}

void func_ov043_020bd7e4(void) {
    if (SystemStatusFlags.vblank) {
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
        func_02001b44(2, 0, &data_020672ec, 0x400);
    }
}

void func_ov043_020bd8cc(void) {
    func_ov043_020bd454();
    Interrupts_RegisterVBlankCallback(func_ov043_020bd7e4, 1);
}

void func_ov043_020bd8e8(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}

s32 func_ov043_020bd8fc(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5) {
    if ((arg0 >= arg2) && (arg0 <= (s32)(arg2 + arg4)) && (arg1 >= arg3) && (arg1 <= (s32)(arg3 + arg5))) {
        return 1;
    }
    return 0;
}

void func_ov043_020bd938(Sprite* sprite, s16 arg1) {
    Data* resourceData = sprite->resourceData;

    u8* param0;
    if (resourceData == NULL) {
        param0 = NULL;
    } else {
        param0 = (u32*)resourceData->buffer;
        param0 += 0x20;
        param0 = param0 + *((u32*)(param0 + 0x18));
    }

    u8* param1;
    if (resourceData == NULL) {
        param1 = NULL;
    } else {
        param1 = (u32*)resourceData->buffer;
        param1 += 0x20;
        param1 = param1 + *((u32*)(param1 + 0x10));
    }

    Sprite_ChangeAnimation(sprite, param0, arg1, param1);
}

const BinIdentifier data_ov043_020cadc0 = {43, "Apl_Tak/ItemData.bin"};

void func_ov043_020bd984(u16* arg0, s32 arg1) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, arg0, 0x18, &data_ov043_020cadc0, arg1 * 0x18));
}

const BinIdentifier data_ov043_020cadc8 = {43, "Apl_Tak/FoodData.bin"};

void func_ov043_020bd9b0(u16* arg0, s32 arg1) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, arg0, 0x14, &data_ov043_020cadc8, arg1 * 0x14));
}

const BinIdentifier data_ov043_020cadd0 = {43, "Apl_Tak/TreasureData.bin"};

void func_ov043_020bd9dc(u16* arg0, s32 arg1) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, arg0, 8, &data_ov043_020cadd0, arg1 * 8));
}

const BinIdentifier data_ov043_020cadd8 = {43, "Apl_Tak/ShopData.bin"};

void func_ov043_020bda08(u16* arg0, u16 arg1) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, arg0, 0xC, &data_ov043_020cadd8, arg1 * 0xC));
}

u16 func_ov043_020bda34(s32 arg0) {
    u16 var_r5;
    u16 i;
    u16 var_r7 = 0;
    switch (arg0) {
        case 0:
            var_r5 = 4;
            break;
        case 5:
            var_r5 = 3;
            break;
        case 13:
            var_r5 = 4;
            break;
        case 18:
            var_r5 = 4;
            break;
        case 25:
            var_r5 = 3;
            break;
        case 30:
            var_r5 = 3;
            break;
        case 42:
            var_r5 = 3;
            break;
        default:
            var_r5 = 4;
            break;
    }

    for (i = 1; i <= var_r5; i++) {
        if (func_0202353c((u8)(arg0 + i)) == 1) {
            var_r7 += 1;
        }
    }
    return var_r7;
}

s32 func_ov043_020bdb18(u8 arg0) {
    // return data_ov043_020ccdd0[arg0] - 1;
}

void func_ov043_020bdb2c(void* arg0) {
    // ? spC;
    // s32 temp_hi;
    // s32 temp_r3;
    // s32 temp_r8;
    // u16 temp_r0;
    // u16 var_r0;
    // u16 var_r1;
    // u16 var_r7;
    // u16 var_r9;

    // var_r7     = 0;
    // arg0->unkA = (u16)data_02072d10.unkAB2;
    // arg0->unk4 = (s16)data_02072d10.unkAB3;
    // func_ov043_020bda08(&spC, arg0->unkA);
    // var_r0 = spE;
    // if (var_r0 == 3) {
    //     func_02023010(0x2AE);
    //     var_r0 = func_ov043_020bdb18(data_02072d10.unk712) + 0x6B;
    // }
    // arg0->unk6 = var_r0;
    // arg0->unk8 = (s16)(sp10 + 0x1F);
    // var_r1     = 0;
    // arg0->unkC = func_ov043_020bda34((s32)arg0->unkA);
    // var_r9     = 0;
    // do {
    //     temp_r3        = (var_r1 * 6) + (s32)arg0;
    //     temp_r3->unkE  = 0xFFFF;
    //     temp_r3->unk10 = 0;
    //     var_r1 += 1;
    //     temp_r3->unk12 = 0;
    // } while ((u32)var_r1 < 5U);
    // if ((u32)arg0->unkC <= 0U) {
    //     return;
    // }
    // do {
    //     if (func_0202353c((u8)(arg0->unkA + var_r9 + 1)) == 1) {
    //         temp_r8       = (var_r7 * 6) + (s32)arg0;
    //         temp_r8->unkE = (u16)(arg0->unkA + var_r9 + 1);
    //         func_ov043_020bda08(&subroutine_arg0, temp_r8->unkE);
    //         temp_r8->unk10 = sp6;
    //         temp_r8->unk12 = 0;
    //         var_r7 += 1;
    //     }
    //     temp_r0 = var_r9 + 1;
    //     temp_hi = (u32)arg0->unkC > (u32)temp_r0;
    //     var_r9  = temp_r0;
    // } while (temp_hi);
}

void func_ov043_020bdc6c(DepartUnk*) {
    return;
}

s32 func_ov043_020bdc70(s32 arg0, s32 arg1) {
    // u16 spE;
    // u16 spC;
    // u16 spA;
    // u16 sp8;

    // sp8 = data_ov043_020cade0.unk8;
    // spC = data_ov043_020cade0.unk4;
    // spA = data_ov043_020cade0.unkA;
    // spE = data_ov043_020cade0.unk6;
    // if (func_ov043_020bd8fc(arg0, arg1, (s32)(s16)spC, (s32)(s16)spE, (s16)(s32)(s16)sp8, (s16)(s32)(s16)spA) == 1) {
    //     return 1;
    // }
    // return 0;
}

s16 func_ov043_020bdcd0(s32 arg0, s32 arg1, s32 arg2) {
    // u16 sp12;
    // u16 sp10;
    // s32 spC;
    // s32 sp8;
    // s16 temp_r0;
    // s16 var_r6;
    // s32 temp_gt;
    // s32 var_r7;
    // s32 var_r8;

    // sp10   = data_ov043_020cade0.unk0;
    // sp8    = arg0;
    // spC    = arg1;
    // sp12   = data_ov043_020cade0.unk2;
    // var_r6 = 0;
    // if (arg2 > 0) {
    //     var_r7 = 0;
    //     var_r8 = 0;
    // loop_2:
    //     if (func_ov043_020bd8fc(sp8, spC, (s32)(s16)((*(&data_ov043_020ccd00 + (arg2 * 4)) + var_r8) - 0x58),
    //                             (s32)(s16)(((&data_ov043_020ccd00 + (arg2 * 4))->unk2 + var_r7) - 0x11),
    //                             (s16)(s32)(s16)sp10, (s16)(s32)(s16)sp12) == 1)
    //     {
    //         return var_r6;
    //     }
    //     temp_r0 = var_r6 + 1;
    //     temp_gt = arg2 > (s32)temp_r0;
    //     var_r7 += 0x25;
    //     var_r8 += 0xA;
    //     var_r6 = temp_r0;
    //     if (!temp_gt) {
    //         /* Duplicate return node #5. Try simplifying control flow for better match */
    //         return -1;
    //     }
    //     goto loop_2;
    // }
    // return -1;
}

const BinIdentifier data_ov043_020cad70[3] = {
    {43,     "Apl_Tak/Grp_Menu_BGD.bin"},
    {43, "Apl_Tak/Grp_Depart_BGD00.bin"},
    {43, "Apl_Tak/Grp_Menu_fontSCR.bin"},
};

// Nonmatching
void func_ov043_020bdd9c(DepartResources* resources, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    resources->data = DatMgr_LoadRawData(1, NULL, 0, &data_ov043_020cad70[arg3]);

    u32* param;
    if (resources->data == NULL) {
        param = NULL;
    } else {
        param = (u32*)resources->data->buffer;
        param = (u32*)((u8*)param + 0x20);
        param = (u32*)((u8*)param + param[2]);
    }
    resources->unk_10 = param;

    if (resources->data == NULL) {
        param = NULL;
    } else {
        param = (u32*)resources->data->buffer;
        param = (u32*)((u8*)param + 0x20);
        param = (u32*)((u8*)param + param[4]);
    }
    resources->unk_14 = (u8*)param;

    if (resources->data == NULL) {
        param = NULL;
    } else {
        param = (u32*)resources->data->buffer;
        param = (u32*)((u8*)param + 0x20);
        param = (u32*)((u8*)param + param[6]);
    }
    resources->unk_18 = (u8*)param;

    if (arg1 == 0) {
        u32 var_r2 = (*resources->unk_10 & ~0xFF) >> 8;
        if ((*resources->unk_10 & 0xF0) == 0) {
            var_r2 -= 4;
        }
        resources->charResource   = BgResMgr_AllocChar32(g_BgResourceManagers[0], (u8*)resources->unk_10,
                                                         g_DisplaySettings.engineState[0].bgSettings[arg2].charBase, var_r2);
        resources->screenResource = BgResMgr_AllocScreen(g_BgResourceManagers[0], resources->unk_14,
                                                         g_DisplaySettings.engineState[0].bgSettings[arg2].screenBase,
                                                         g_DisplaySettings.engineState[0].bgSettings[arg2].screenSizeText);
        resources->unk_0C         = func_0200adf8(data_0206b3cc[0], resources->unk_18, 0, arg4, arg5);
        func_0200bf60(data_0206b3cc[0], resources->unk_0C);
    } else {
        u32 var_r2 = (*resources->unk_10 & ~0xFF) >> 8;
        if ((*resources->unk_10 & 0xF0) == 0) {
            var_r2 -= 4;
        }
        resources->charResource   = BgResMgr_AllocChar32(g_BgResourceManagers[1], (u8*)resources->unk_10,
                                                         g_DisplaySettings.engineState[1].bgSettings[arg2].charBase, var_r2);
        resources->screenResource = BgResMgr_AllocScreen(g_BgResourceManagers[1], resources->unk_14,
                                                         g_DisplaySettings.engineState[1].bgSettings[arg2].screenBase,
                                                         g_DisplaySettings.engineState[1].bgSettings[arg2].screenSizeText);
        resources->unk_0C         = func_0200adf8(data_0206b3cc[1], resources->unk_18, 0, arg4, arg5);
        func_0200bf60(data_0206b3cc[1], resources->unk_0C);
    }
}

// Nonmatching
void func_ov043_020bdf9c(DepartResources* resources, s32 arg1, s32 arg2, s32 arg3, u16 arg4, s32 arg5, s32 arg6) {
    resources->data = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov043_020cad70[arg3], arg4, FALSE);

    u32* param;
    if (resources->data == NULL) {
        param = NULL;
    } else {
        param = (u32*)resources->data->buffer;
        param = (u32*)((u8*)param + 0x20);
        param = (u32*)((u8*)param + param[2]);
    }
    resources->unk_10 = param;

    if (resources->data == NULL) {
        param = NULL;
    } else {
        param = (u32*)resources->data->buffer;
        param = (u32*)((u8*)param + 0x20);
        param = (u32*)((u8*)param + param[4]);
    }
    resources->unk_14 = (u8*)param;

    if (resources->data == NULL) {
        param = NULL;
    } else {
        param = (u32*)resources->data->buffer;
        param = (u32*)((u8*)param + 0x20);
        param = (u32*)((u8*)param + param[6]);
    }
    resources->unk_18 = (u8*)param;

    if (arg1 == 0) {
        u32 var_r2 = (*resources->unk_10 & ~0xFF) >> 8;
        if ((*resources->unk_10 & 0xF0) == 0) {
            var_r2 -= 4;
        }
        resources->charResource   = BgResMgr_AllocChar32(g_BgResourceManagers[0], (u8*)resources->unk_10,
                                                         g_DisplaySettings.engineState[0].bgSettings[arg2].charBase, var_r2);
        resources->screenResource = BgResMgr_AllocScreen(g_BgResourceManagers[0], resources->unk_14,
                                                         g_DisplaySettings.engineState[0].bgSettings[arg2].screenBase,
                                                         g_DisplaySettings.engineState[0].bgSettings[arg2].screenSizeText);
        resources->unk_0C         = func_0200adf8(data_0206b3cc[0], resources->unk_18, 0, arg4, arg5);
        func_0200bf60(data_0206b3cc[0], resources->unk_0C);
    } else {
        u32 var_r2 = (*resources->unk_10 & ~0xFF) >> 8;
        if ((*resources->unk_10 & 0xF0) == 0) {
            var_r2 -= 4;
        }
        resources->charResource   = BgResMgr_AllocChar32(g_BgResourceManagers[1], (u8*)resources->unk_10,
                                                         g_DisplaySettings.engineState[1].bgSettings[arg2].charBase, var_r2);
        resources->screenResource = BgResMgr_AllocScreen(g_BgResourceManagers[1], resources->unk_14,
                                                         g_DisplaySettings.engineState[1].bgSettings[arg2].screenBase,
                                                         g_DisplaySettings.engineState[1].bgSettings[arg2].screenSizeText);
        resources->unk_0C         = func_0200adf8(data_0206b3cc[1], resources->unk_18, 0, arg4, arg5);
        func_0200bf60(data_0206b3cc[1], resources->unk_0C);
    }
}

void func_ov043_020be1a8(DepartResources* resources, s32 arg1) {
    if (arg1 == 0) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[0], resources->charResource);
        BgResMgr_ReleaseScreen(g_BgResourceManagers[0], resources->screenResource);
        func_0200afec(data_0206b3cc[0], resources->unk_0C);
    } else {
        BgResMgr_ReleaseChar(g_BgResourceManagers[1], resources->charResource);
        BgResMgr_ReleaseScreen(g_BgResourceManagers[1], resources->screenResource);
        func_0200afec(data_0206b3cc[1], resources->unk_0C);
    }
    DatMgr_ReleaseData(resources->data);
}

void func_ov043_020be230(DepartResources* arg0) {
    arg0->data           = NULL;
    arg0->screenResource = NULL;
    arg0->charResource   = NULL;
    arg0->unk_0C         = NULL;
    arg0->unk_14         = NULL;
    arg0->unk_10         = NULL;
    arg0->unk_18         = NULL;
}

void func_ov043_020be254(DepartUnk* arg0) {
    for (s32 i = 0; i < 4; i++) {
        func_ov043_020be230(&arg0->unk_34[i]);
        func_ov043_020be230(&arg0->unk_A4[i]);
    }

    func_ov043_020bdd9c(&arg0->unk_A4[2], 0, 2, 2, 0xF, 1);
    func_ov043_020bdd9c(&arg0->unk_A4[3], 0, 3, 0, 0, 1);
    func_ov043_020bdd9c(&arg0->unk_34[2], 1, 2, 6, 0xF, 1);
    func_ov043_020bdf9c(&arg0->unk_34[3], 1, 3, 5, arg0->unk_06, 0, 0xE);
}

void func_ov043_020be328(DepartUnk*) {
    return;
}

void func_ov043_020be32c(DepartUnk* arg0) {
    func_ov043_020be1a8(&arg0->unk_A4[2], 0);
    func_ov043_020be1a8(&arg0->unk_A4[3], 0);
    func_ov043_020be1a8(&arg0->unk_34[2], 1);
    func_ov043_020be1a8(&arg0->unk_34[3], 1);
}

/// MARK: DepartExit

typedef struct {
    /* 0x00 */ char unk_00[0x30];
    /* 0x30 */ u8   unk_30;
    /* 0x31 */ u8   unk_31;
} DepartExitUnk;

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ s32            unk_40;
    /* 0x44 */ DepartExitUnk* unk_44;
} DepartExit; // Size: 0x48

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
} DepartExitArgs;

SpriteFrameInfo* func_ov043_020be368(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020be404(void* arg1, void* arg2, void* arg3) {
    // Not yet implemented
}

s32 DepartExit_Init(TaskPool* pool, Task* task, void* args) {
    DepartExit*     departExit     = task->data;
    DepartExitArgs* departExitArgs = args;

    departExit->unk_40 = 1;
    departExit->unk_44 = departExitArgs->unk_4;
    func_ov043_020be404(departExit, departExit, departExitArgs);
    return 1;
}

s32 DepartExit_Update(TaskPool* pool, Task* task, void* args) {
    DepartExit*    departExit = task->data;
    DepartExitUnk* iVar1      = departExit->unk_44;

    if (iVar1->unk_30 == 1) {
        func_ov043_020bd938(&departExit->sprite, 2);
        if (iVar1->unk_31 != 0) {
            iVar1->unk_31--;
        } else {
            iVar1->unk_30 = 0;
        }
    } else {
        func_ov043_020bd938(&departExit->sprite, 1);
    }
    Sprite_Update(&departExit->sprite);
    return 1;
}

s32 DepartExit_Render(TaskPool* pool, Task* task, void* args) {
    DepartExit* departExit = task->data;

    Sprite_RenderFrame(&departExit->sprite);
    return 1;
}

s32 DepartExit_Release(TaskPool* pool, Task* task, void* args) {
    DepartExit* departExit = task->data;

    Sprite_Release(&departExit->sprite);
    return 1;
}

s32 DepartExit_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov043_020cadf8 = {
        .initialize = DepartExit_Init,
        .update     = DepartExit_Update,
        .render     = DepartExit_Render,
        .cleanup    = DepartExit_Release,
    };
    return data_ov043_020cadf8.iter[stage](pool, task, args);
}

const TaskHandle data_ov043_020cadec = {"Tsk_Depart_exit", DepartExit_RunTask, 0x48};

s32 DepartExit_CreateTask(TaskPool* pool, s32 arg1, void* arg2) {
    DepartExitArgs args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    EasyTask_CreateTask(pool, &data_ov043_020cadec, NULL, 0, NULL, &args);
}

/// MARK: DepartPanel

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk_40;
    /* 0x44 */ s32    unk_44;
} DepartPanel; // Size: 0x48

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
    /* 0x8 */ u16 unk_8;
} DepartPanelArgs;

SpriteFrameInfo* func_ov043_020be59c(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020be638(void* arg0, void* arg1, void* arg2) {
    // Not yet implemented
}

s32 DepartPanel_Init(TaskPool* pool, Task* task, void* args) {
    DepartPanel*     panel     = task->data;
    DepartPanelArgs* panelArgs = args;

    panel->unk_40 = 1;
    panel->unk_44 = panelArgs->unk_4;
    func_ov043_020be638(panel, panel, panelArgs);
    return 1;
}

s32 DepartPanel_Update(TaskPool* pool, Task* task, void* args) {
    DepartPanel* panel = task->data;

    Sprite_Update(&panel->sprite);
    return 1;
}

s32 DepartPanel_Render(TaskPool* pool, Task* task, void* args) {
    DepartPanel* panel = task->data;

    Sprite_RenderFrame(&panel->sprite);
    return 1;
}

s32 DepartPanel_Release(TaskPool* pool, Task* task, void* args) {
    DepartPanel* panel = task->data;

    Sprite_Release(&panel->sprite);
    return 1;
}

s32 DepartPanel_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov043_020cae88 = {
        .initialize = DepartPanel_Init,
        .update     = DepartPanel_Update,
        .render     = DepartPanel_Render,
        .cleanup    = DepartPanel_Release,
    };
    return data_ov043_020cae88.iter[stage](pool, task, args);
}

const TaskHandle data_ov043_020cae34 = {"Tsk_Depart_panel", DepartPanel_RunTask, sizeof(DepartPanel)};

s32 DepartPanel_CreateTask(TaskPool* pool, s32 arg1, u16 arg2, void* arg3) {
    DepartPanelArgs args;
    args.unk_0 = arg1;
    args.unk_8 = arg2;
    args.unk_4 = arg3;
    return EasyTask_CreateTask(pool, &data_ov043_020cae34, NULL, 0, NULL, &args);
}

/// MARK: DepartBoard

typedef struct {
    /* 0x00 */ Sprite sprites[2];
    /* 0x80 */ s32    unk_80[2];
    /* 0x88 */ s32    unk_88;
} DepartBoard; // Size: 0x8C

typedef struct {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ s32 unk_04;
} DepartBoardArgs;

SpriteFrameInfo* func_ov043_020be7dc(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020be888(DepartBoard* arg0, DepartBoard* arg1, void* arg2) {
    // Not yet implemented
}

s32 DepartBoard_Init(TaskPool* pool, Task* task, void* args) {
    DepartBoard*     board     = task->data;
    DepartBoardArgs* boardArgs = args;

    board->unk_88 = boardArgs->unk_04;
    func_ov043_020be888(board, board, args);
    return 1;
}

s32 DepartBoard_Update(TaskPool* pool, Task* task, void* args) {
    DepartBoard* board = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Update(&board->sprites[i]);
    }
    return 1;
}

s32 DepartBoard_Render(TaskPool* pool, Task* task, void* args) {
    DepartBoard* board = task->data;

    for (s32 i = 0; i < 2; i++) {
        if (board->unk_80[i] != 0) {
            Sprite_RenderFrame(&board->sprites[i]);
        }
    }
    return 1;
}

s32 DepartBoard_Release(TaskPool* pool, Task* task, void* args) {
    DepartBoard* board = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&board->sprites[i]);
    }
    return 1;
}

s32 DepartBoard_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov043_020cae88 = {
        .initialize = DepartBoard_Init,
        .update     = DepartBoard_Update,
        .render     = DepartBoard_Render,
        .cleanup    = DepartBoard_Release,
    };
    return data_ov043_020cae88.iter[stage](pool, task, args);
}

const TaskHandle data_ov043_020cae7c = {"Tsk_Depart_board", DepartBoard_RunTask, sizeof(DepartBoard)};

s32 DepartBoard_CreateTask(TaskPool* pool, s32 arg1, u16 arg2, s32 arg3) {
    // Not yet implemented
}

/// MARK: DepartTextScr

typedef struct {
    /* 0x000 */ s32           unk_000;
    /* 0x004 */ UnkOv31Struct unk_004[5];
    /* 0x270 */ UnkOv31Struct unk_270[5];
} DepartTextScr; // Size: 0x4DC

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
} DepartTextScrArgs;

void func_ov043_020beb44(DepartTextScr* textScr) {
    s32 i;
    for (i = 0; i < 5; i++) {
        func_ov031_0210aa94(&textScr->unk_004[i]);
        func_ov031_0210aa94(&textScr->unk_270[i]);
    }

    for (i = 0; i < 5; i++) {
        func_ov031_0210ab34(&textScr->unk_004[i], 14);
        func_ov031_0210ab34(&textScr->unk_270[i], 14);
    }
}

void func_ov043_020bebc0(DepartTextScr* textScr) {
    // Not yet implemented
}

s32 DepartTextScr_Init(TaskPool* pool, Task* task, void* args) {
    DepartTextScr*     textScr     = task->data;
    DepartTextScrArgs* textScrArgs = args;

    textScr->unk_000 = textScrArgs->unk_4;
    func_ov043_020beb44(textScr);
    func_ov043_020bebc0(textScr);
    return 1;
}

s32 DepartTextScr_Update(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 DepartTextScr_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 DepartTextScr_Release(TaskPool* pool, Task* task, void* args) {
    DepartTextScr* textScr = task->data;

    for (s32 i = 0; i < 5; i++) {
        func_ov031_0210aabc(&textScr->unk_004[i]);
        func_ov031_0210aabc(&textScr->unk_270[i]);
    }
    return 1;
}

s32 DepartTextScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov043_020caed0 = {
        .initialize = DepartTextScr_Init,
        .update     = DepartTextScr_Update,
        .render     = DepartTextScr_Render,
        .cleanup    = DepartTextScr_Release,
    };
    return data_ov043_020caed0.iter[stage](pool, task, args);
}

const TaskHandle data_ov043_020caec4 = {"Tsk_Depart_textScr", DepartTextScr_RunTask, sizeof(DepartTextScr)};

s32 DepartTextScr_CreateTask(TaskPool* pool, s32 arg1, void* arg2) {
    DepartTextScrArgs args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    return EasyTask_CreateTask(pool, &data_ov043_020caec4, NULL, 0, NULL, &args);
}

/// MARK: DepartTextScrU

typedef struct {
    /* 0x00 */ char unk_00[0xA];
    /* 0x0A */ u16  unk_0A;
    /* 0x0C */ char unk_0C[0x70];
    /* 0x7C */ s32  unk_7C;
    /* 0x80 */ s32  unk_80;
} DepartTextScrUUnk;

typedef struct {
    /* 0x00 */ DepartTextScrUUnk* unk_00;
    /* 0x04 */ UnkOv31Struct      unk_04;
} DepartTextScrU; // Size: 0x80

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
} DepartTextScrUArgs;

void func_ov043_020bee2c(DepartTextScrU* textScrU) {
    func_ov031_0210aa94(&textScrU->unk_04);
    func_ov031_0210ab34(&textScrU->unk_04, 8);
}

void func_ov043_020bee4c(DepartTextScrU* textScrU) {
    DepartTextScrUUnk* temp_r7 = textScrU->unk_00;
    s32                temp_r4 = temp_r7->unk_80;
    s32                temp_r5 = temp_r7->unk_7C;

    if ((temp_r4 == 0) || (temp_r5 == 0)) {
        OS_WaitForever();
    }

    func_ov031_0210ab28(&textScrU->unk_04, 2, 6);
    func_ov031_0210ab54(&textScrU->unk_04, 1, 0);
    func_ov031_0210b630(&textScrU->unk_04, (u16)(temp_r7->unk_0A + 0x2FB8));
    func_ov031_0210be18(&textScrU->unk_04, temp_r4 + 4, temp_r5 + 4, 0);
}

s32 DepartTextScrU_Init(TaskPool* pool, Task* task, void* args) {
    DepartTextScrU*     textScrU     = task->data;
    DepartTextScrUArgs* textScrUArgs = args;

    textScrU->unk_00 = textScrUArgs->unk_4;
    func_ov043_020bee2c(textScrU);
    func_ov043_020bee4c(textScrU);
    return 1;
}

s32 DepartTextScrU_Update(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 DepartTextScrU_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 DepartTextScrU_Release(TaskPool* pool, Task* task, void* args) {
    DepartTextScrU* textScrU = task->data;

    func_ov031_0210aabc(&textScrU->unk_04);
    return 1;
}

s32 DepartTextScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov043_020caeec = {
        .initialize = DepartTextScrU_Init,
        .update     = DepartTextScrU_Update,
        .render     = DepartTextScrU_Render,
        .cleanup    = DepartTextScrU_Release,
    };
    return data_ov043_020caeec.iter[stage](pool, task, args);
}

const TaskHandle data_ov043_020caee0 = {"Tsk_Depart_textScrU", DepartTextScrU_RunTask, sizeof(DepartTextScrU)};

s32 DepartTextScrU_CreateTask(TaskPool* pool, s32 arg1, s32 arg2) {
    DepartTextScrUArgs args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    return EasyTask_CreateTask(pool, &data_ov043_020caee0, NULL, 0, NULL, &args);
}

/// MARK: DepartCur

typedef struct {
    /* 0x0 */ char unk_0[0x4];
    /* 0x4 */ s16  unk_4;
    /* 0x6 */ char unk_6[0x6];
    /* 0xC */ u16  unk_C;
} DepartCurUnk;

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ s32           unk_40;
    /* 0x44 */ DepartCurUnk* unk_44;
    /* 0x48 */ s16           unk_48;
} DepartCur; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32   unk_0;
    /* 0x4 */ void* unk_4;
} DepartCurArgs;

SpriteFrameInfo* func_ov043_020bef90(Sprite* sprite, s32 arg1, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020bf038(DepartCur* arg0, DepartCur* arg1, s32* arg2) {
    // Not yet implemented
}

s32 DepartCur_Init(TaskPool* pool, Task* task, void* args) {
    DepartCur*     cur     = task->data;
    DepartCurArgs* curArgs = args;

    cur->unk_44 = curArgs->unk_4;
    func_ov043_020bf038(cur, cur, args);
    return 1;
}

// Nonmatching: Regalloc
s32 DepartCur_Update(TaskPool* pool, Task* task, void* args) {
    DepartCur*    cur    = task->data;
    DepartCurUnk* pDVar1 = cur->unk_44;

    if (pDVar1->unk_4 != cur->unk_48) {
        cur->sprite.posX = pDVar1->unk_4 * 10 + data_ov043_020ccd00[pDVar1->unk_C].x;
        cur->sprite.posY = pDVar1->unk_4 * 37 + data_ov043_020ccd00[pDVar1->unk_C].y;
        cur->unk_48      = pDVar1->unk_4;
    }
    Sprite_Update(&cur->sprite);
    return 1;
}

s32 DepartCur_Render(TaskPool* pool, Task* task, void* args) {
    DepartCur* cur = task->data;

    if (cur->unk_40 != 0) {
        Sprite_RenderFrame(&cur->sprite);
    }
    return 1;
}

s32 DepartCur_Release(TaskPool* pool, Task* task, void* args) {
    DepartCur* cur = task->data;

    Sprite_Release(&cur->sprite);
    return 1;
}

s32 DepartCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov043_020caf08 = {
        .initialize = DepartCur_Init,
        .update     = DepartCur_Update,
        .render     = DepartCur_Render,
        .cleanup    = DepartCur_Release,
    };
    return data_ov043_020caf08.iter[stage](pool, task, args);
}

const TaskHandle data_ov043_020caefc = {"Tsk_Depart_cur", DepartCur_RunTask, sizeof(DepartCur)};

s32 DepartCur_CreateTask(TaskPool* pool, s32 arg1, void* arg2) {
    DepartCurArgs args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    return EasyTask_CreateTask(pool, &data_ov043_020caefc, NULL, 0, NULL, &args);
}
