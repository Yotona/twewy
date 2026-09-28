#include "Interface/Menu/Depart.h"
#include "CriSndMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Player/Inventory.h"
#include "SpriteMgr.h"
#include "common_data.h"

extern void func_ov030_020ae92c();
extern void func_ov043_020aeee0();

Point data_ov043_020ccd00[6] = {
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
void Depart_UpdateBackgrounds(DepartObject*);
void func_ov043_020bdc6c(DepartObject*);
void Depart_LoadBackgrounds(DepartObject*);
void Depart_ReleaseBackgrounds(DepartObject*);

void func_ov043_020bcdb0(DepartState* state) {
    DepartObject* temp = &state->unk_2165C;

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
    DepartObject* temp_r5 = &state->unk_2165C;

    EasyTask_CreateTask(&state->base.taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -16, 0x1000);

    state->taskId_Exit = DepartExit_CreateTask(&state->base.taskPool, state->base.dataType, temp_r5);

    for (u16 i = 0; i < temp_r5->unk_0C; i++) {
        state->taskIds_Panel[i] = DepartPanel_CreateTask(&state->base.taskPool, state->base.dataType, i, temp_r5);
        state->taskIds_Board[i] = DepartBoard_CreateTask(&state->base.taskPool, state->base.dataType, i, temp_r5);
    }

    state->taskId_Cur      = Depart_cur_CreateTask(&state->base.taskPool, state->base.dataType, temp_r5);
    state->taskId_TextScr  = DepartTextScr_CreateTask(&state->base.taskPool, state->base.dataType, temp_r5);
    state->taskId_TextScrU = Depart_textScrU_CreateTask(&state->base.taskPool, state->base.dataType, temp_r5);
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

void Depart_Init(DepartState* state) {
    if (state == NULL) {
        char* sequence = data_ov043_020ccc48;
        state          = Mem_AllocHeapTail(&gDebugHeap, sizeof(DepartState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        data_ov043_020cd28c = state;
        MainOvlDisp_SetCbArg(state);
    }

    DepartObject* temp = &state->unk_2165C;

    state->base.spareDataType = DatMgr_AllocateSlot();
    state->base.dataType      = DatMgr_AllocateSlot();
    func_ov043_020bd8cc();
    state->base.prevResMgr = ResourceMgr_ReinitManagers(&state->base.resMgr);
    TouchInput_Init();
    Mem_InitializeHeap(&state->base.heap, state->base.heapBuffer, sizeof(state->base.heapBuffer));
    FS_LoadOverlay(0, &OVERLAY_31_ID);
    EasyTask_InitializePool(&state->base.taskPool, &state->base.heap, 0x200, NULL, NULL);
    data_02066aec = 0;
    data_02066eec = 0;
    func_ov043_020bcdb0(state);
    Depart_LoadBackgrounds(temp);
    func_ov043_020bcdfc(state);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)func_ov043_020bd084, state, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov043_020bcf64, state, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov043_020bcf3c, state, 0);
    MainOvlDisp_NextProcessStage();
    CriSndMgr_PlayFile((temp->unk_08 % 3) + ADX_S01);
}

void Depart_Update(DepartState* state) {
    DepartObject* temp_r4 = &state->unk_2165C;

    TouchInput_Update();
    OamMgr_Reset3DState();
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    Depart_UpdateBackgrounds(temp_r4);
    DebugOvlDisp_Run();
    EasyTask_UpdatePool(&state->base.taskPool);
    if (DebugOvlDisp_IsStackAtBase() == TRUE) {
        state->unk_21650 = 1;
    }
    OamMgr_Swap3DBuffers();
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_EXTENDED], NULL);
    if (state->unk_21650 != 0) {
        switch (temp_r4->unk_03) {
            case 0: {
                OverlayTag tag;
                MainOvlDisp_ReplaceTop(&tag, &OVERLAY_30_ID, func_ov030_020ae92c, NULL, PROCESS_STAGE_INIT);
            } break;
            case 1: {
                OverlayTag tag;
                MainOvlDisp_ReplaceTop(&tag, &OVERLAY_43_ID, func_ov043_020aeee0, NULL, PROCESS_STAGE_INIT);
            } break;
            default: {
                OverlayTag tag;
                MainOvlDisp_Pop(&tag);
            } break;
        }
    }
}

void Depart_Destroy(DepartState* state) {
    DepartObject* temp = &state->unk_2165C;

    CriSndMgr_Stop(ADX_TITLE);
    func_ov043_020bdc6c(temp);
    Depart_ReleaseBackgrounds(temp);
    EasyTask_DestroyPool(&state->base.taskPool);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->base.spareDataType);
    DatMgr_ClearSlot(state->base.dataType);
    func_ov043_020bd8e8();
    FS_UnloadOverlay(0, &OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
}

const OverlayProcess OvlProc_Depart = {
    .init = Depart_Init,
    .main = Depart_Update,
    .exit = Depart_Destroy,
};

void func_ov043_020bd414(void* object) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        Depart_Destroy(object);
    } else {
        OvlProc_Depart.funcs[stage](object);
    }
}

// Nonmatching
void func_ov043_020bd454(void) {
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
    GX_SetBankForSubObjExtPltt(GX_VRAM_NONE);
    MI_CpuFill(0, 0x06800000, 0xA4000);
    MI_CpuFill(0, 0x06000000, 0x80000);
    MI_CpuFill(0, 0x06200000, 0x20000);
    MI_CpuFill(0, 0x06400000, 0x40000);
    MI_CpuFill(0, 0x06600000, 0x20000);
    REG_POWER_CNT &= ~0x8000;
    Display_CommitSynced();
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 4);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 3, 1, 0x10C);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 5, 1, 0x214);

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

    Display_SetMainLayers(LAYER_BG0 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);
    g_DisplaySettings.controls[0].brightness = 0x10;
    g_DisplaySettings.controls[1].bgMode     = GX_BGMODE_0;

    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 1, 4);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 3, 1, 0x10C);

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

    Display_SetSubLayers(LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);
    g_DisplaySettings.controls[1].brightness = 0x10;

    OamMgr_Init3DSpritePipeline();
    OamMgr_Swap3DBuffers();
    OamMgr_InitExtended();
}

void func_ov043_020bd7e4(void) {
    if (SystemStatusFlags.vblank) {
        Display_Commit();
        DMA_Flush();
        OamMgr_Commit();
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

void Depart_SetSpriteFrame(Sprite* sprite, s16 frame) {
    void* anim   = Data_GetPackEntryData(sprite->resourceData, 3);
    void* frames = Data_GetPackEntryData(sprite->resourceData, 2);

    Sprite_ChangeAnimation(sprite, anim, frame, frames);
}

const BinIdentifier data_ov043_020cadc0 = {43, "Apl_Tak/ItemData.bin"};

void func_ov043_020bd984(RawItemData* itemData, u16 itemID) {
    Data_LoadToBuffer(1, *itemData, &data_ov043_020cadc0, itemID);
}

const BinIdentifier data_ov043_020cadc8 = {43, "Apl_Tak/FoodData.bin"};

void func_ov043_020bd9b0(RawFoodData* foodData, u16 itemID) {
    Data_LoadToBuffer(1, *foodData, &data_ov043_020cadc8, itemID);
}

const BinIdentifier data_ov043_020cadd0 = {43, "Apl_Tak/TreasureData.bin"};

void func_ov043_020bd9dc(RawTreasureData* treasureData, u16 itemID) {
    Data_LoadToBuffer(1, *treasureData, &data_ov043_020cadd0, itemID);
}

const BinIdentifier data_ov043_020cadd8 = {43, "Apl_Tak/ShopData.bin"};

void func_ov043_020bda08(RawShopData* shopData, u16 arg0) {
    Data_LoadToBuffer(1, *shopData, &data_ov043_020cadd8, arg0);
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

void func_ov043_020bdc6c(DepartObject*) {
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

void Depart_LoadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &data_ov043_020cad70[binIndex]);
    res->charData    = Data_GetPackEntryData(res->data, 1);
    res->screenMap   = Data_GetPackEntryData(res->data, 2);
    res->paletteData = Data_GetPackEntryData(res->data, 3);
    if (engine == DISPLAY_MAIN) {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_MAIN], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], res->paletteResource);
    } else {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_SUB], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_SUB], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], res->paletteResource);
    }
}

// Same as Depart_LoadBgResource, but the BG pack is entry `packIndex` of the archive.
void Depart_LoadBgResourcePacked(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, u16 packIndex, s32 palStart,
                                 u32 palCount) {
    res->data        = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov043_020cad70[binIndex], packIndex, FALSE);
    res->charData    = Data_GetPackEntryData(res->data, 1);
    res->screenMap   = Data_GetPackEntryData(res->data, 2);
    res->paletteData = Data_GetPackEntryData(res->data, 3);
    if (engine == DISPLAY_MAIN) {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_MAIN], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], res->paletteResource);
    } else {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_SUB], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_SUB], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], res->paletteResource);
    }
}

void Depart_ReleaseBgResource(MenuBgResource* res, s32 engine) {
    if (engine == DISPLAY_MAIN) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_MAIN], res->charResource);
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenResource);
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_MAIN], res->paletteResource);
    } else {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], res->charResource);
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenResource);
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], res->paletteResource);
    }
    DatMgr_ReleaseData(res->data);
}

void Depart_ClearBgResource(MenuBgResource* res) {
    res->data            = NULL;
    res->screenResource  = NULL;
    res->charResource    = NULL;
    res->paletteResource = NULL;
    res->screenMap       = NULL;
    res->charData        = NULL;
    res->paletteData     = NULL;
}

void Depart_LoadBackgrounds(DepartObject* depart) {
    s32             i;
    MenuBgResource* subRes  = &depart->resources[0];
    MenuBgResource* mainRes = &depart->resources[4];

    for (i = 0; i < 4; i++) {
        Depart_ClearBgResource(subRes);
        Depart_ClearBgResource(mainRes);
        subRes += 1;
        mainRes += 1;
    }

    Depart_LoadBgResource(&depart->resources[6], DISPLAY_MAIN, 2, 2, 15, 1);
    Depart_LoadBgResource(&depart->resources[7], DISPLAY_MAIN, 3, 0, 0, 1);
    Depart_LoadBgResource(&depart->resources[2], DISPLAY_SUB, 2, 6, 15, 1);
    Depart_LoadBgResourcePacked(&depart->resources[3], DISPLAY_SUB, 3, 5, depart->unk_06, 0, 14);
}

void Depart_UpdateBackgrounds(DepartObject* depart) {
    return;
}

void Depart_ReleaseBackgrounds(DepartObject* depart) {
    Depart_ReleaseBgResource(&depart->resources[6], DISPLAY_MAIN);
    Depart_ReleaseBgResource(&depart->resources[7], DISPLAY_MAIN);
    Depart_ReleaseBgResource(&depart->resources[2], DISPLAY_SUB);
    Depart_ReleaseBgResource(&depart->resources[3], DISPLAY_SUB);
}
