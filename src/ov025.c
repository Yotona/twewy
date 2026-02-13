#include "BgResMgr.h"
#include "CriSndMgr.h"
#include "DMA.h"
#include "DatMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "EasyList.h"
#include "EasyTask.h"
#include "Input.h"
#include "Interrupts.h"
#include "Memory.h"
#include "OverlayManager.h"
#include "SndMgr.h"
#include "SpriteMgr.h"
#include "Text.h"
#include "TouchInput.h"
#include "common_data.h"
#include "types.h"

#undef data_02074d10
#undef data_02074e10

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
    /* 0xB4 */ s32  unkB4;
} UnkStruct_02074110_local;

typedef struct {
    /* 0x00000 */ char  pad0[0x60];
    /* 0x00060 */ s16   unk60;
    /* 0x00062 */ s16   unk62;
    /* 0x00064 */ s16   unk64;
    /* 0x00066 */ char  pad66[0x11DA6];
    /* 0x11E0C */ Data* unk11E0C;
    /* 0x11E10 */ char  pad11E10[0x2C0];
    /* 0x120D0 */ u16   unk120D0;
    /* 0x120D2 */ char  pad120D2[0x2];
    /* 0x120D4 */ s32   unk120D4;
    /* 0x120D8 */ s32   unk120D8;
    /* 0x120DC */ s16   unk120DC;
    /* 0x120DE */ s16   unk120DE;
} Ov025MainStruct;

typedef struct {
    /* 0x00 */ char pad0[0x8];
    /* 0x08 */ u32  unk8;
    /* 0x0C */ char padC[0x4];
    /* 0x10 */ u32  unk10;
    /* 0x14 */ char pad14[0x4];
    /* 0x18 */ u32  unk18;
    /* 0x1C */ char pad1C[0x4];
    /* 0x20 */ u32  unk20;
    /* 0x24 */ char pad24[0x4];
    /* 0x28 */ u32  unk28;
    /* 0x2C */ char pad2C[0x4];
    /* 0x30 */ u32  unk30;
} DataHeader;

void func_ov030_020ae92c(void*, s32);                                                                  /* extern */
void BgResMgr_AllocChar32(s32, void*, u32, void*, s32);                                                /* extern */
void BgResMgr_AllocScreen(s32, void*, u32, s32);                                                       /* extern */
void DC_PurgeRange(s32*, void*);                                                                       /* extern */
void DMA_Flush();                                                                                      /* extern */
void DMA_Init(s32 size);                                                                               /* extern */
void DebugOvlDisp_Init();                                                                              /* extern */
s32  DebugOvlDisp_IsStackAtBase();                                                                     /* extern */
void DebugOvlDisp_Pop();                                                                               /* extern */
void DebugOvlDisp_Push(void (*)(void*), void*, void*);                                                 /* extern */
void DebugOvlDisp_ReplaceTop(void (*)(void*), void*);                                                  /* extern */
void DebugOvlDisp_Run();                                                                               /* extern */
void EasyFade_FadeBothDisplays(FaderMode mode, s32 brightness, s32 rate);                              /* extern */
s32  EasyFade_IsFading();                                                                              /* extern *
                                                   void GX_LoadBgPltt(s16 *, void *, void *);                       /* extern */
void                            GX_LoadOam(s32*, void*, void*);                                        /* extern */
void                            GX_LoadObjPltt(s32*, void*, void*);                                    /* extern */
void                            GXs_LoadBgPltt(s16*, void*, void*);                                    /* extern */
void                            GXs_LoadOam(s32*, void*, void*);                                       /* extern */
u32                             GXs_LoadObjPltt(s32*, void*, void*);                                   /* extern */
void                            Input_Init(InputState* input, s32 delayInit, s32 delayMin, s32 step);  /* extern */
void                            Interrupts_Init();                                                     /* extern */
IRQCallback                     Interrupts_RegisterVBlankCallback(IRQCallback callback, s32 param_2);  /* extern */
s32                             MainOvlDisp_GetRepeatCount();                                          /* extern */
void                            MainOvlDisp_IncrementRepeatCount();                                    /* extern */
void                            MainOvlDisp_ReplaceTop(void*, s32, void*, s32);                        /* extern */
void                            MainOvlDisp_Run();                                                     /* extern */
void                            MainOvlDisp_SetCbArg(void*);                                           /* extern */
u32                             OvlMgr_LoadOverlay(s32 slot, u32 newOverlay);                          /* extern */
void                            OvlMgr_UnloadOverlay(s32 slot);                                        /* extern */
void                            TouchInput_GetCoord(TouchCoord* out);                                  /* extern */
void                            TouchInput_Init();                                                     /* extern */
u16*                            TouchInput_Update(void);                                               /* extern */
s32                             TouchInput_WasTouchPressed();                                          /* extern */
void                            func_02001c34(s16*, s32*, void*, void*, s32);                          /* extern */
void                            func_0200270c(void*, void*);                                           /* extern */
void                            func_0200283c(s32*, void*, void*);                                     /* extern */
void                            func_02003440(s32*);                                                   /* extern */
void                            func_020034b0(s32*);                                                   /* extern */
void                            func_0200434c();                                                       /* extern */
void                            func_0200adf8(s32, void*, void*, void*, s32);                          /* extern */
void                            func_0200bf60(s32, void*);                                             /* extern */
void                            func_0200cef0(void*);                                                  /* extern */
void                            func_0200d120(s32);                                                    /* extern */
void                            func_020265d4(void*, void*, u16);                                      /* extern */
void                            SndMgr_StartPlayingSE(s32 seIdx);                                      /* extern */
void                            MI_CpuFill(void*, void*, s32);                                         /* extern */
void                            func_ov003_020825b8(s32, void*, void*);                                /* extern */
void                            func_ov003_02082724(void*, s16, s16);                                  /* extern */
void                            func_ov003_02082a90(void*, void*, BinIdentifier*, s32, s32, s32, s32); /* extern */
void                            func_ov003_02082b0c(void*);                                            /* extern */
void                            func_ov003_02082b64(void*);                                            /* extern */
void                            func_ov003_02082cc4(s32);                                              /* extern */
void                            func_ov003_02082d04(void*);                                            /* extern */
u32                             func_ov025_020e75ac();                                                 /* static */
void                            func_ov025_020e7f28(void* arg0);                                       /* static */
void                            func_ov025_020e7f90(void* arg0);                                       /* static */
void                            func_ov025_020e7fdc(void* arg0);                                       /* static */
void                            func_ov025_020e8078(void* arg0);                                       /* static */
void                            func_ov025_020e81b8(void* arg0);                                       /* static */
void                            func_ov025_020e826c(void* arg0);                                       /* static */
extern InputState               InputStatus;
extern s32                      SystemStatusFlags;
extern const TaskHandle         Task_EasyFade;
extern UnkStruct_02071d10_local data_02071d10;
extern UnkStruct_02074110_local data_02074110;
extern void*                    func_ov003_0208ec74;
extern void*                    func_ov027_020e860c;
extern void*                    func_ov037_0208370c;
extern BgResMgr*                g_BgResourceManagers[2];
static s8                       data_ov025_020e8324[0x10] = "Seq_Continue( )";
static s8*                      data_ov025_020e8320       = data_ov025_020e8324;
static s8                       data_ov025_020e8334[0x2C] = "Apl_Fur/Grp_Continue.bin";
static BinIdentifier            data_ov025_020e8308       = {0x19, data_ov025_020e8334};                        /* const */
static void (*data_ov025_020e8310[3])(void*) = {func_ov025_020e8078, func_ov025_020e81b8, func_ov025_020e826c}; /* const */

void func_ov025_020e7360(void) {
    Interrupts_Init();
    func_0200434c();
    GX_Init();
    DMA_Init(0x10);
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
    MI_CpuFill(0, (void*)0x0680000, 0xA4000);
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
    g_DisplaySettings.controls[0].brightness = 0x10;
    g_DisplaySettings.controls[1].brightness = 0x10;
    Display_CommitSynced();
    g_DisplaySettings.unk_000 = 0;
    *(u16*)0x04000304 &= ~0x8000;
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);
    g_DisplaySettings.engineState[0].bgSettings[1].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    g_DisplaySettings.engineState[0].bgSettings[1].colorMode      = GX_BG_COLORS_16;
    g_DisplaySettings.engineState[0].bgSettings[1].screenBase     = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].charBase       = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].extPlttSlot    = 0;
    *(u16*)0x0400000A                                             = (*(u16*)0x0400000A & 0x43) | 0x104;
    g_DisplaySettings.engineState[0].bgSettings[1].priority       = 2;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic         = 0;
    g_DisplaySettings.controls[0].layers                          = 0x12;
    g_DisplaySettings.controls[1].bgMode                          = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);
    g_DisplaySettings.engineState[1].bgSettings[1].screenBase     = 1;
    g_DisplaySettings.engineState[1].bgSettings[1].charBase       = 4;
    g_DisplaySettings.engineState[1].bgSettings[1].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    g_DisplaySettings.engineState[1].bgSettings[1].colorMode      = GX_BG_COLORS_16;
    g_DisplaySettings.engineState[1].bgSettings[1].extPlttSlot    = 0;
    *(u16*)0x0400100A                                             = (*(u16*)0x0400100A & 0x43) | 0x110;
    g_DisplaySettings.engineState[1].bgSettings[1].priority       = 2;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic         = 0;
    g_DisplaySettings.controls[1].layers                          = 2;
}

u32 func_ov025_020e75ac(void) {
    u32 temp_r0;

    temp_r0 = (u32)(SystemStatusFlags << 0x1F) >> 0x1F;
    if (temp_r0 == 0) {
        return temp_r0;
    }
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
    return GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
}

void func_ov025_020e767c(void) {
    func_ov025_020e7360();
    func_ov025_020e749c();
    Interrupts_RegisterVBlankCallback((void (*)(void))func_ov025_020e75ac, 1);
}

void func_ov025_020e769c(void) {
    func_ov025_020e7360();
    Interrupts_RegisterVBlankCallback(NULL, 1);
}

void func_ov025_020e76b4(Ov025MainStruct* arg0) {
    Data*       temp_r0;
    Data*       temp_r3_2;
    DataHeader* temp_r1;
    DataHeader* temp_r2;
    DataHeader* temp_r2_2;
    DataHeader* temp_r2_3;
    DataHeader* temp_r2_4;
    DataHeader* temp_r3;
    void*       var_r1;
    void*       var_r1_2;
    void*       var_r4;
    void*       var_r4_2;
    void*       var_r5;
    void*       var_r5_2;

    temp_r0        = DatMgr_LoadRawData(1, NULL, 0, &data_ov025_020e8308);
    arg0->unk11E0C = temp_r0;
    if (temp_r0 == NULL) {
        var_r1 = NULL;
    } else {
        temp_r2 = (DataHeader*)(temp_r0->buffer + 0x20);
        var_r1  = (u8*)temp_r2 + temp_r2->unk8;
    }
    if (temp_r0 == NULL) {
        var_r4 = NULL;
    } else {
        temp_r3 = (DataHeader*)(temp_r0->buffer + 0x20);
        var_r4  = (u8*)temp_r3 + temp_r3->unk10;
    }
    if (temp_r0 == NULL) {
        var_r5 = NULL;
    } else {
        temp_r2_2 = (DataHeader*)(temp_r0->buffer + 0x20);
        var_r5    = (u8*)temp_r2_2 + temp_r2_2->unk18;
    }
    BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r1, g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, 0x6000);
    BgResMgr_AllocScreen(g_BgResourceManagers[1], var_r4, g_DisplaySettings.engineState[1].bgSettings[1].screenBase,
                         g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText);
    func_0200adf8(data_0206b3cc[1], var_r5, 0, 0, 7);
    temp_r3_2 = arg0->unk11E0C;
    if (temp_r3_2 == NULL) {
        var_r1_2 = NULL;
    } else {
        temp_r1  = (DataHeader*)(temp_r3_2->buffer + 0x20);
        var_r1_2 = (u8*)temp_r1 + temp_r1->unk20;
    }
    if (temp_r3_2 == NULL) {
        var_r5_2 = NULL;
    } else {
        temp_r2_3 = (DataHeader*)(temp_r3_2->buffer + 0x20);
        var_r5_2  = (u8*)temp_r2_3 + temp_r2_3->unk28;
    }
    if (temp_r3_2 == NULL) {
        var_r4_2 = NULL;
    } else {
        temp_r2_4 = (DataHeader*)(temp_r3_2->buffer + 0x20);
        var_r4_2  = (u8*)temp_r2_4 + temp_r2_4->unk30;
    }
    BgResMgr_AllocChar32(g_BgResourceManagers[0], var_r1_2, g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0,
                         0x6000);
    BgResMgr_AllocScreen(g_BgResourceManagers[0], var_r5_2, g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                         g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);
    func_0200adf8(data_0206b3cc[0], var_r4_2, 0, 0, 7);
}

void func_ov025_020e785c(Ov025MainStruct* arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4) {
    arg0->unk60 = arg1;
    arg0->unk62 = arg3;
    arg0->unk64 = arg4;
    func_ov003_02082a90(0, arg0, &data_ov025_020e8308, 7, 9, 8, arg2);
    func_ov003_02082724(arg0, arg3, arg4);
}

void func_ov025_020e78c8(Ov025MainStruct* arg0, u16 arg1) {
    u16 temp_r1;

    arg0->unk120D0 = arg1;
    temp_r1        = arg0->unk120D0;
    switch ((s32)temp_r1) { /* irregular */
        case 0:
            func_ov003_02082724(arg0 + 0x11FB0, 0x80, 0x56);
            break;
        case 1:
            func_ov003_02082724(arg0 + 0x11FB0, 0x80, 0x6B);
            break;
        case 2:
            func_ov003_02082724(arg0 + 0x11FB0, 0x80, 0x80);
            break;
        case 3:
            func_ov003_02082724(arg0 + 0x11FB0, 0x80, 0x95);
            break;
    }
    func_ov003_02082d04(arg0 + 0x11FB0);
}

s32 func_ov025_020e797c(Ov025MainStruct* arg0) {
    TouchCoord       touchCoord;
    s32              sp0;
    s32              sp4;
    s16              temp_r1;
    s16              temp_r1_2;
    s32              var_r2;
    u16              var_r4;
    Ov025MainStruct* var_r3;

    var_r4 = 0xFFFF;
    TouchInput_GetCoord(&touchCoord);
    sp0    = touchCoord.x;
    sp4    = touchCoord.y;
    var_r3 = (Ov025MainStruct*)((u8*)arg0 + 0x11E10);
    var_r2 = 0;
loop_1:
    temp_r1 = var_r3->unk62;
    if ((sp0 >= (s32)(temp_r1 - 0x48)) && (sp0 <= (s32)(temp_r1 + 0x48)) &&
        (temp_r1_2 = var_r3->unk64, (sp4 >= (s32)(temp_r1_2 - 0xA))) && (sp4 <= (s32)(temp_r1_2 + 0xA)))
    {
        var_r4 = var_r3->unk60;
    } else {
        var_r2 += 1;
        var_r3 = (Ov025MainStruct*)((u8*)var_r3 + 0x68);
        if (var_r2 < 4) {
            goto loop_1;
        }
    }
    if ((var_r4 == 2) && (((UnkStruct_020740d10_local*)&data_02074d10)->unk140 & 1)) {
        var_r4 = 0xFFFF;
    }
    return (s32)var_r4;
}

void func_ov025_020e7a20(Ov025MainStruct* arg0) {
    s32 temp_r0;
    u16 temp_r1;

    if (TouchInput_WasTouchPressed() == 0) {
        return;
    }
    temp_r0 = func_ov025_020e797c(arg0);
    temp_r1 = arg0->unk120D0;
    if (temp_r0 != temp_r1) {
        if (temp_r0 != 0xFFFF) {
            SndMgr_StartPlayingSE(0x2A);
        }
        func_ov025_020e78c8(arg0, (u16)temp_r0);
        return;
    }
    if (temp_r1 == 0xFFFF) {
        return;
    }
    SndMgr_StartPlayingSE(0x2B);
    arg0->unk120DC = 0;
    arg0->unk120DE = 0;
    DebugOvlDisp_ReplaceTop(func_ov025_020e7f90, (void*)arg0);
}

void func_ov025_020e7aac(Ov025MainStruct* arg0) {
    Ov025MainStruct* temp_r0;

    func_ov025_020e76b4(arg0);
    func_ov025_020e785c((Ov025MainStruct*)((u8*)arg0 + 0x11E10), 0, 1, 0x80, 0x56);
    func_ov025_020e785c((Ov025MainStruct*)((u8*)arg0 + 0x11E78), 1, 2, 0x80, 0x6B);
    func_ov025_020e785c((Ov025MainStruct*)((u8*)arg0 + 0x11F48), 3, 3, 0x80, 0x95);
    temp_r0 = (Ov025MainStruct*)((u8*)arg0 + 0x11EE0);
    if (((UnkStruct_020740d10_local*)&data_02074d10)->unk140 & 1) {
        func_ov025_020e785c(temp_r0, 2, 5, 0x80, 0x80);
    } else {
        func_ov025_020e785c(temp_r0, 2, 4, 0x80, 0x80);
    }
    func_ov003_02082a90(0, (u8*)arg0 + 0x11FB0, &data_ov025_020e8308, 7, 9, 8, 0);
    func_ov003_020825b8((s32)((u8*)arg0 + 0x11FB0), 0, 1);
    func_ov025_020e78c8(arg0, 0xFFFFU);
    func_ov003_02082a90(0, (u8*)arg0 + 0x12010, &data_ov025_020e8308, 7, 9, 8, 6);
    func_ov003_02082a90(0, (u8*)arg0 + 0x12070, &data_ov025_020e8308, 9, 8, 0xA, 7);
    arg0->unk120D4 = 0x148000;
    arg0->unk120D8 = -0x48000;
}

void func_ov025_020e7c40(Ov025MainStruct* arg0) {
    if (arg0->unk120D0 != 0xFFFF) {
        func_ov003_02082b0c((u8*)arg0 + 0x11FB0);
    }
    func_ov003_02082b0c((u8*)arg0 + 0x11E10);
    func_ov003_02082b0c((u8*)arg0 + 0x11E78);
    func_ov003_02082b0c((u8*)arg0 + 0x11EE0);
    func_ov003_02082b0c((u8*)arg0 + 0x11F48);
    func_ov003_02082b0c((u8*)arg0 + 0x12010);
    func_ov003_02082b0c((u8*)arg0 + 0x12070);
}

void func_ov025_020e7cb8(Ov025MainStruct* arg0) {
    if (arg0->unk120D0 != 0xFFFF) {
        func_ov003_02082b64((u8*)arg0 + 0x11FB0);
    }
    func_ov003_02082b64((u8*)arg0 + 0x11E10);
    func_ov003_02082b64((u8*)arg0 + 0x11E78);
    func_ov003_02082b64((u8*)arg0 + 0x11EE0);
    func_ov003_02082b64((u8*)arg0 + 0x11F48);
    func_ov003_02082724((u8*)arg0 + 0x12010, (s16)((s32)(arg0->unk120D4 * 0x10) >> 0x10), 0x60);
    func_ov003_02082724((u8*)arg0 + 0x12070, (s16)((s32)(arg0->unk120D8 * 0x10) >> 0x10), 0x60);
    func_ov003_02082b64((u8*)arg0 + 0x12010);
    func_ov003_02082b64((u8*)arg0 + 0x12070);
}

void func_ov025_020e7d70(Ov025MainStruct* arg0) {
    func_ov003_02082cc4((s32)((u8*)arg0 + 0x11FB0));
    func_ov003_02082cc4((s32)((u8*)arg0 + 0x11E10));
    func_ov003_02082cc4((s32)((u8*)arg0 + 0x11E78));
    func_ov003_02082cc4((s32)((u8*)arg0 + 0x11EE0));
    func_ov003_02082cc4((s32)((u8*)arg0 + 0x11F48));
    func_ov003_02082cc4((s32)((u8*)arg0 + 0x12010));
    func_ov003_02082cc4((s32)((u8*)arg0 + 0x12070));
}

void func_ov025_020e7dd0(Ov025MainStruct* arg0) {
    void* sp24;
    void* sp1C;
    void* sp14;
    void* spC;
    void* sp4;
    u16   temp_r0;

    temp_r0 = arg0->unk120D0;
    switch (temp_r0) {
        default:
        case 3:
            if (((UnkStruct_020740d10_local*)&data_02074d10)->unk130 == 3) {
                MainOvlDisp_ReplaceTop(&spC, 0x1B, &func_ov027_020e860c, 0);
                return;
            }
            MainOvlDisp_ReplaceTop(&sp4, 0x25, &func_ov037_0208370c, 0);
            return;
        case 0:
            ((UnkStruct_02074e10_local*)&data_02074e10)->unk42 = 0;
            ((UnkStruct_020740d10_local*)&data_02074d10)->unk141 =
                (s8)((u32)(((UnkStruct_02071d10_local*)&data_02071d10)->unk19 << 0x1E) >> 0x1E);
            MainOvlDisp_ReplaceTop(&sp24, 3, &func_ov003_0208ec74, 0);
            return;
        case 1:
            ((UnkStruct_02074e10_local*)&data_02074e10)->unk42   = 0;
            ((UnkStruct_020740d10_local*)&data_02074d10)->unk141 = 0;
            MainOvlDisp_ReplaceTop(&sp1C, 3, &func_ov003_0208ec74, 0);
            return;
        case 2:
            if (((UnkStruct_020740d10_local*)&data_02074d10)->unk140 & 2) {
                func_ov030_020ae92c(&data_02074110, 5);
                return;
            }
            data_02074110.unkB4 = 3;
            MainOvlDisp_ReplaceTop(&sp14, 0x1E, NULL, 0);
            return;
    }
}

void func_ov025_020e7f28(void* arg0) {
    Ov025MainStruct* mainStruct = (Ov025MainStruct*)arg0;
    if (mainStruct->unk120DC == 0) {
        EasyFade_FadeBothDisplays(0, 0, 0x1000);
        mainStruct->unk120DC = (s16)(mainStruct->unk120DC + 1);
    }
    if (EasyFade_IsFading() != 0) {
        return;
    }
    mainStruct->unk120DC = 0;
    mainStruct->unk120DE = 0;
    DebugOvlDisp_ReplaceTop(func_ov025_020e7fdc, arg0);
}

void func_ov025_020e7f90(void* arg0) {
    Ov025MainStruct* mainStruct = (Ov025MainStruct*)arg0;
    if (mainStruct->unk120DC == 0) {
        EasyFade_FadeBothDisplays(0, 0x10, 0x1000);
        mainStruct->unk120DC = (s16)(mainStruct->unk120DC + 1);
    }
    if (EasyFade_IsFading() != 0) {
        return;
    }
    DebugOvlDisp_Pop();
}

void func_ov025_020e7fdc(void* arg0) {
    Ov025MainStruct* mainStruct = (Ov025MainStruct*)arg0;
    s16              temp_r1;

    if (mainStruct->unk120DC == 0) {
        if (mainStruct->unk120DE == 0) {
            mainStruct->unk120DE = 0x10;
        }
        temp_r1 = mainStruct->unk120DE;
        if ((s32)temp_r1 > 0) {
            func_020265d4(&mainStruct->unk120D4, 0x80000, (u16)temp_r1);
            func_020265d4(&mainStruct->unk120D8, 0x80000, (u16)mainStruct->unk120DE);
            mainStruct->unk120DE = (s16)(mainStruct->unk120DE - 1);
        }
        if ((s32)mainStruct->unk120DE <= 0) {
            mainStruct->unk120DC = 0;
            mainStruct->unk120DE = 0;
        }
    }
    func_ov025_020e7a20(mainStruct);
}

void func_ov025_020e8078(void* arg0) {
    s8*   temp_r5;
    void* var_r4;

    var_r4 = arg0;
    if (var_r4 == NULL) {
        temp_r5 = data_ov025_020e8320;
        var_r4  = Mem_AllocHeapTail(&gDebugHeap, 0x120E0);
        Mem_SetSequence(&gDebugHeap, var_r4, temp_r5);
        MainOvlDisp_SetCbArg(var_r4);
    }
    MI_CpuFill(0, var_r4, Mem_GetBlockSize(&gDebugHeap, var_r4));
    OvlMgr_LoadOverlay(3, 3);
    Mem_InitializeHeap(var_r4 + 0x800, var_r4, 0x800U);
    EasyTask_InitializePool(var_r4 + 0x80C, var_r4 + 0x800, 8U, NULL, NULL);
    func_ov025_020e767c();
    func_0200cef0(var_r4 + 0x88C);
    data_02066aec = 0;
    data_02066eec = 0;
    Input_Init(&InputStatus, 0xC, 2, 1);
    TouchInput_Init();
    EasyTask_CreateTask(var_r4 + 0x80C, &Task_EasyFade, NULL, 0, NULL, NULL);
    func_ov025_020e7aac((Ov025MainStruct*)var_r4);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push(func_ov025_020e7f28, var_r4, 0);
    MainOvlDisp_IncrementRepeatCount();
    MainOvlDisp_Run();
}

void func_ov025_020e81b8(void* arg0) {
    Ov025MainStruct* mainStruct = (Ov025MainStruct*)arg0;
    TouchInput_Update();
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_ov025_020e7c40(mainStruct);
    func_ov025_020e7cb8(mainStruct);
    DebugOvlDisp_Run();
    if (DebugOvlDisp_IsStackAtBase() != 0) {
        func_ov025_020e7dd0(mainStruct);
        return;
    }
    EasyTask_UpdatePool((TaskPool*)((u8*)arg0 + 0x80C));
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc[0], 0);
    func_0200bf60(data_0206b3cc[1], 0);
}

void func_ov025_020e826c(void* arg0) {
    Ov025MainStruct* mainStruct = (Ov025MainStruct*)arg0;
    if (arg0 == NULL) {
        return;
    }
    func_ov025_020e7d70(mainStruct);
    func_0200d120((s32)((u8*)arg0 + 0x88C));
    DatMgr_ReleaseData(mainStruct->unk11E0C);
    func_ov025_020e769c();
    EasyTask_DestroyPool((TaskPool*)((u8*)arg0 + 0x80C));
    Mem_Free(&gDebugHeap, arg0);
    MainOvlDisp_SetCbArg(NULL);
    OvlMgr_UnloadOverlay(3);
}

void func_ov025_020e82c8(void* arg0) {
    s32 temp_r0;

    temp_r0 = MainOvlDisp_GetRepeatCount();
    if (temp_r0 == 0x7FFFFFFF) {
        func_ov025_020e826c(arg0);
        return;
    }
    data_ov025_020e8310[temp_r0](arg0);
}