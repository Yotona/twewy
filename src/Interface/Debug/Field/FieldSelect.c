#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Str.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Field/FieldDebug.h"
#include "Save.h"
#include "common_data.h"

extern void func_02010b84(TextObject* textObj, s32 x, s32 y, s32 width, s32 height);
extern void func_02025b68(void* textObj, UnkStruct_usedby_02025b68* params);
extern void func_02025e30(void* textObj);

extern s32  func_ov030_020af350(void);
extern s32  func_ov030_020b7b00(void);
extern s32  func_ov030_020b7b40(void);
extern s32  func_ov030_020b7b60(void);
extern void func_ov030_020b7c58(s32 area, s32* arg1, s32* arg2, s32* arg3);
extern void func_ov030_020c268c(s32 arg0);
extern s16  func_ov030_020c5930(void);
extern void func_ov030_020ae96c(s32 arg0);
extern void func_ov030_020d4f74(void);

// Per chapter setup routines, indexed by MainData::unk_1AB0.
extern void (*const data_ov030_020dbe30[44])(void);

const char* data_ov030_0210033c[48] = {
    [7]  = "no004 東急ハンズ",
    [1]  = "ce001 スクランブル交差点",
    [2]  = "no001 HWV前",
    [3]  = "ce002 109",
    [37] = "mk014 マークシティ屋上",
    [14] = "ea001 宮下公園",
    [47] = "ce004 ハチ公居ない",
    [22] = "we004 東急本店前",
    [19] = "we001 道玄坂",
    [9]  = "no006 宇田川町路地裏",
    [39] = "ex002 渋谷川川前",
    [11] = "no008 西武",
    [12] = "no009 丸井",
    [13] = "no010 タワレコ",
    [5]  = "ce004 ハチ公前",
    [6]  = "no002 ちとせ会館前",
    [15] = "ea002 ガード下（東）",
    [16] = "ea003 キャットストリート",
    [17] = "so001 渋谷駅西口",
    [18] = "so002 駅南ガード下",
    [35] = "mk012 マークシティ12階",
    [10] = "no007 代々木公園",
    [23] = "we005 オンエアイースト内部",
    [24] = "mk001 マークシティ1階",
    [25] = "mk002 マークシティ2階",
    [26] = "mk003 マークシティ3階",
    [8]  = "no005 パルコ",
    [27] = "mk004 マークシティ4階",
    [28] = "mk005 マークシティ5階",
    [29] = "mk006 マークシティ6階",
    [30] = "mk007 マークシティ7階",
    [31] = "mk008 マークシティ8階",
    [32] = "mk009 マークシティ9階",
    [4]  = "ce003 センター街入り口",
    [33] = "mk010 マークシティ10階",
    [34] = "mk011 マークシティ11階",
    [21] = "we003 オンエアイースト",
    [36] = "mk013 マークシティ13階",
    [38] = "ex001 渋谷川通路A",
    [0]  = "no003 スペイン坂",
    [40] = "ex003 渋谷川通路B",
    [41] = "ex004 渋谷川上級①の部屋",
    [42] = "ex005 渋谷川通路C",
    [43] = "ex006 渋谷川コンポーザーの部屋",
    [44] = "tmp",
    [45] = "tmp",
    [46] = "tmp",
    [20] = "we002 マークシティ",
};

static const char* data_ov030_02100338 = "Seq_FldSel()";

void func_ov030_020b04ac(FieldSelectObject* object) {
    UnkStruct_usedby_02025b68 params;

    params.unk_00 = 1;
    params.unk_04 = 0;
    params.unk_08 = object->unk_11584;
    params.unk_0C = &data_0205c9b0;
    params.unk_10 = 0;
    params.unk_14 = NULL;
    params.unk_18 = 0;
    params.unk_1A = 0;
    params.unk_1C = 0x20;
    params.unk_1E = 0x18;
    func_02025b68(&object->unk_21594, &params);
    Text_RenderToScreen(&object->text, 0, 0, "field select");
}

const char* func_ov030_020b0534(s32 area) {
    return data_ov030_0210033c[area];
}

void func_ov030_020b0544(FieldSelectObject* object) {
    func_02010b84(&object->text, 0, 0x10, 0x100, 0x60);
    Text_RenderToScreen(&object->text, 8, 0x10, func_ov030_020b0534(gSaveData.unk_2458));
    Text_RenderToScreen(&object->text, 8, 0x18, Str_SPrintf("width:%d", func_ov030_020b7b00()));

    s32 building = func_ov030_020b7b40();
    if (building < 0) {
        building = 0;
    }
    Text_RenderToScreen(&object->text, 8, 0x20, Str_SPrintf("Building:%d", building));
    Text_RenderToScreen(&object->text, 8, 0x28, Str_SPrintf("Road    :%d", func_ov030_020b7b60()));

    s32 whole = gSaveData.unk_248C >> 12;
    Text_RenderToScreen(&object->text, 8, 0x30,
                        Str_SPrintf("topScale  :%03d.%02d", whole, ((gSaveData.unk_248C * 100) >> 12) - (whole * 100)));

    s32 whole2 = gSaveData.unk_2490 >> 12;
    Text_RenderToScreen(&object->text, 8, 0x38,
                        Str_SPrintf("botomScale:%03d.%02d", whole2, ((gSaveData.unk_2490 * 100) >> 12) - (whole2 * 100)));

    Text_RenderToScreen(&object->text, 8, 0x40, Str_SPrintf("NPC:%3d", gSaveData.unk_2548[gSaveData.unk_2458]));

    if (gSaveData.unk_26F0 != 0) {
        Text_RenderToScreen(&object->text, 8, 0x48, "DEBUG WINDOW ON ");
    } else {
        Text_RenderToScreen(&object->text, 8, 0x48, "DEBUG WINDOW OFF");
    }

    if (gSaveData.unk_26EC != 0) {
        Text_RenderToScreen(&object->text, 8, 0x50, "MapJump ON ");
    } else {
        Text_RenderToScreen(&object->text, 8, 0x50, "MapJump OFF");
    }

    Text_RenderToScreen(&object->text, 8, 0x58, Str_SPrintf("partner:%d", gSaveData.playerStats.activePartner));
    Text_RenderToScreen(&object->text, 8, 0x60, Str_SPrintf("noize symbol:%d", gSaveData.unk_3110));
    Text_RenderToScreen(&object->text, 8, 0x68, Str_SPrintf("top brand:%d", object->unk_219B8));
    Text_RenderToScreen(&object->text, 0, (object->unk_219B4 * 8) + 0x10, ">");
}

void func_ov030_020b0820(FieldSelectObject* object) {
    func_02025e30(&object->unk_21594);
}

void func_ov030_020b0834(FieldSelectObject* object) {
    s32 index = func_ov030_020af350();
    if (index < 0) {
        object->unk_219B8 = -1;
        return;
    }
    object->unk_219B8 = gSaveData.brandTrends[index].ranking[0];
}

void func_ov030_020b0878(FieldSelectObject* object) {
    s32 index = func_ov030_020af350();
    if (index < 0) {
        return;
    }
    gSaveData.brandTrends[index].ranking[0] = object->unk_219B8;
}

void func_ov030_020b08b0(FieldSelectObject* object) {
    func_ov030_020b0834(object);
    if (gSaveData.unk_2484 != 0) {
        gSaveData.unk_3110 = func_ov030_020c5930();
    }
    func_ov030_020c26ec();
    func_ov030_020b04ac(object);
}

void func_ov030_020b08f0(FieldSelectObject* object) {
    func_ov030_020b0820(object);
}

// Cycles the active friend with the D-Pad, clamped to the three party members.
void func_ov030_020b08fc(void) {
    if (SysControl.buttonState.holdButtons & INPUT_BUTTON_RIGHT) {
        if (gSaveData.playerStats.activePartner < 2) {
            gSaveData.playerStats.activePartner++;
        }
        return;
    }
    if ((SysControl.buttonState.holdButtons & INPUT_BUTTON_LEFT) && gSaveData.playerStats.activePartner != 0) {
        gSaveData.playerStats.activePartner--;
    }
}

// Steps the selected area with the D-Pad and refreshes everything derived from it.
void func_ov030_020b0948(FieldSelectObject* object) {
    if (SysControl.buttonState.holdButtons & INPUT_BUTTON_RIGHT) {
        gSaveData.unk_2458++;
    } else if (SysControl.buttonState.holdButtons & INPUT_BUTTON_LEFT) {
        gSaveData.unk_2458--;
    }

    if (gSaveData.unk_2458 >= 0x2C) {
        gSaveData.unk_2458 = 0x2B;
    }
    if (gSaveData.unk_2458 < 0) {
        gSaveData.unk_2458 = 0;
    }

    func_ov030_020b7c58(gSaveData.unk_2458, &gSaveData.unk_248C, &gSaveData.unk_2490, &gSaveData.unk_2494);
    func_ov030_020b0834(object);

    if (!(SysControl.buttonState.holdButtons & INPUT_BUTTON_RIGHT) &&
        !(SysControl.buttonState.holdButtons & INPUT_BUTTON_LEFT))
    {
        return;
    }
    gSaveData.unk_3110 = func_ov030_020c5930();
}

// Toggles a flag with the D-Pad.
void func_ov030_020b0a14(s32* flag) {
    if (SysControl.buttonState.holdButtons & INPUT_BUTTON_RIGHT) {
        *flag = 1;
        return;
    }
    if (SysControl.buttonState.holdButtons & INPUT_BUTTON_LEFT) {
        *flag = 0;
    }
}

// Steps a free running counter with the D-Pad.
void func_ov030_020b0a40(s16* value) {
    if (SysControl.buttonState.holdButtons & INPUT_BUTTON_RIGHT) {
        *value += 1;
        return;
    }
    if (SysControl.buttonState.holdButtons & INPUT_BUTTON_LEFT) {
        *value -= 1;
    }
}

void func_ov030_020b0a78(FieldSelectObject* object) {
    OverlayTag tag;
    OverlayTag tag2;

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_A) {
        func_ov030_020ae96c(gSaveData.unk_2458);
        gSaveData.unk_24B4 = 0;
        if (gSaveData.unk_26EC != 0) {
            func_ov030_020c268c(4);
        } else {
            func_ov030_020c268c(0);
        }
        func_ov030_020b0878(object);
        MainOvlDisp_ReplaceTop(&tag, &OVERLAY_30_ID, func_ov030_020ae92c, NULL, PROCESS_STAGE_INIT);
        return;
    }

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_SELECT) {
        MainOvlDisp_Pop(&tag2);
    } else if (SysControl.buttonState.holdButtons == INPUT_BUTTON_UP) {
        object->unk_219B4--;
    } else if (SysControl.buttonState.holdButtons == INPUT_BUTTON_DOWN) {
        object->unk_219B4++;
    }

    if (object->unk_219B4 < 0) {
        object->unk_219B4 = 0xB;
    } else if (object->unk_219B4 >= 0xC) {
        object->unk_219B4 = 0;
    }

    switch (object->unk_219B4) {
        case 0:
            func_ov030_020b0948(object);
            break;
        case 6:
            func_ov030_020b0a40(&gSaveData.unk_2548[gSaveData.unk_2458]);
            break;
        case 7:
            func_ov030_020b0a14(&gSaveData.unk_26F0);
            break;
        case 8:
            func_ov030_020b0a14(&gSaveData.unk_26EC);
            break;
        case 9:
            func_ov030_020b08fc();
            break;
        case 10:
            func_ov030_020b0a40(&gSaveData.unk_3110);
            if (gSaveData.unk_3110 < 0) {
                gSaveData.unk_3110 = 0;
            } else if (gSaveData.unk_3110 >= 0x80) {
                gSaveData.unk_3110 = 0x7F;
            }
            break;
        case 11:
            if (object->unk_219B8 >= 0) {
                func_ov030_020b0a40(&object->unk_219B8);
                if (object->unk_219B8 < 0) {
                    object->unk_219B8 = 0;
                } else if (object->unk_219B8 >= 0xD) {
                    object->unk_219B8 = 0xC;
                }
            }
            break;
        default:
            break;
    }

    func_ov030_020ae96c(gSaveData.unk_2458);
    func_ov030_020b0544(object);
}

// Nonmatching: r1/r2 swapped for the g_DisplaySettings / data_02066eec pool loads
void func_ov030_020b0ca4(void) {
    const char*        sequenceName = data_ov030_02100338;
    FieldSelectObject* object       = Mem_AllocHeapTail(&gDebugHeap, 0x219BC);

    Mem_SetSequence(&gDebugHeap, object, sequenceName);
    MainOvlDisp_SetCbArg(object);
    object->unk_11584 = DatMgr_AllocateSlot();
    object->unk_219B4 = 0;
    func_ov030_020b20c0();
    Display_SetMainLayers(LAYER_NONE);
    object->prevResources = ResourceMgr_ReinitManagers(&object->resources);

    g_DisplaySettings.controls[DISPLAY_SUB].brightness = data_02066eec = g_DisplaySettings.controls[DISPLAY_MAIN].brightness =
        data_02066aec                                                  = 0;

    func_ov030_020b08b0(object);
    MainOvlDisp_NextProcessStage();
}

void func_ov030_020b0d58(FieldSelectObject* object) {
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    func_ov030_020b0a78(object);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], 0);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], 0);
}

void func_ov030_020b0dd8(FieldSelectObject* object) {
    func_ov030_020b08f0(object);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(object->unk_11584);
    Mem_Free(&gDebugHeap, object);
}

static const OverlayProcess data_ov030_020dbee0 = {
    .init = func_ov030_020b0ca4,
    .main = func_ov030_020b0d58,
    .exit = func_ov030_020b0dd8,
};

void func_ov030_020b0e0c(void* object) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        func_ov030_020b0dd8(object);
    } else {
        data_ov030_020dbee0.funcs[stage](object);
    }
}

void func_ov030_020b0e4c(void) {
    MainOvlDisp_NextProcessStage();
}

void func_ov030_020b0e58(void) {
    gSaveData.unk_2484 = 1;
    func_ov030_020c26ec();
    gSaveData.unk_2488 = 1;
    gSaveData.unk_30FC = 0;
    gSaveData.unk_30FE = -1;
    gSaveData.unk_3100 = -1;
    gSaveData.unk_24BE = 0;

    if (gSaveData.unk_1AB0 <= 6) {
        gSaveData.playerStats.activePartner = PARTNER_SHIKI;
    } else if (gSaveData.unk_1AB0 <= 13) {
        gSaveData.playerStats.activePartner = PARTNER_JOSHUA;
    } else if (gSaveData.unk_1AB0 <= 20) {
        gSaveData.playerStats.activePartner = PARTNER_BEAT;
    } else if (gSaveData.unk_1AB0 <= 28) {
        gSaveData.playerStats.activePartner = PARTNER_SHIKI;
    } else if (gSaveData.unk_1AB0 <= 35) {
        gSaveData.playerStats.activePartner = PARTNER_JOSHUA;
    } else if (gSaveData.unk_1AB0 <= 42) {
        gSaveData.playerStats.activePartner = PARTNER_BEAT;
    }

    func_ov030_020aec38(6);
    func_ov030_020aec38(9);
    gSaveData.unk_3104 = 0;
    gSaveData.unk_310C = NULL;
    gSaveData.unk_30FE = 39;
    func_ov030_020aead0(2);
    func_ov030_020aead0(4);
    func_ov030_020aec38(10);
    func_ov030_020b7ac8();
    data_ov030_020dbe30[gSaveData.unk_1AB0]();

    if (gSaveData.unk_3124 == -1) {
        OverlayTag tag;
        MainOvlDisp_ReplaceTop(&tag, &OVERLAY_30_ID, func_ov030_020ae92c, NULL, PROCESS_STAGE_INIT);
    } else {
        OverlayTag tag;
        MainOvlDisp_ReplaceTop(&tag, &OVERLAY_30_ID, func_ov030_020d4f74, NULL, PROCESS_STAGE_INIT);
    }
}

void func_ov030_020b0fe4(void* object) {}

static const OverlayProcess data_ov030_020dbeec = {
    .init = func_ov030_020b0e4c,
    .main = func_ov030_020b0e58,
    .exit = func_ov030_020b0fe4,
};

void func_ov030_020b0fe8(void* object) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        func_ov030_020b0fe4(object);
    } else {
        data_ov030_020dbeec.funcs[stage](object);
    }
}
