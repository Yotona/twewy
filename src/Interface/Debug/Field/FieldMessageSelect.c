#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Str.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Engine/Text.h"
#include "Field/FieldDebug.h"
#include "Util/MsgXls.h"
#include "common_data.h"

typedef struct {
    /* 0x00000 */ ResourceManager  resources;
    /* 0x11580 */ ResourceManager* prevResources;
    /* 0x11584 */ s32              datSlot;
    /* 0x11588 */ TaskPool         taskPool;
    /* 0x11608 */ Heap             heap;
    /* 0x11614 */ u8               memBuffer[0x10000];
    /* 0x21614 */ s32              unk_21614;
    /* 0x21618 */ char             unk_21618[0x8];
    /* 0x21620 */ TextObject       text;
    /* 0x21734 */ char             unk_21734[0x2FC];
    /* 0x21A30 */ UnkOv31Struct    unk_21A30;
    /* 0x21AAC */ s32              messTask; // Task id of the live message window, or -1
    /* 0x21AB0 */ u16              mesNo;    // Message index, global across every .xls file
    /* 0x21AB2 */ char             unk_21AB2[0x2];
    /* 0x21AB4 */ s32              xlsIndex; // Index of the .xls file mesNo falls in
    /* 0x21AB8 */ u8               winKind;
    /* 0x21AB9 */ char             unk_21AB9[0x3];
    /* 0x21ABC */ s32              cursor;
} FldMesSelObject; // Size: 0x21AC0

// One selectable message window style.
typedef struct {
    /* 0x0 */ u8          kind;
    /* 0x4 */ const char* name;
    /* 0x8 */ BOOL        hasBalloon;
} FldMesWinKind; // Size: 0xC

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
} FldMesSize;

typedef struct {
    /* 0x00 */ s32        datSlot;
    /* 0x04 */ FldMesSize unk_04;
    /* 0x08 */ FldMesSize unk_08;
    /* 0x0C */ s32        unk_0C;
    /* 0x10 */ s32        mesNo;
    /* 0x14 */ s16        unk_14;
    /* 0x16 */ s16        unk_16;
    /* 0x18 */ u8         winKind;
    /* 0x19 */ u8         unk_19;
    /* 0x1A */ s8         unk_1A;
    /* 0x1B */ s8         unk_1B;
    /* 0x1C */ s32        unk_1C;
    /* 0x20 */ s32        unk_20;
    /* 0x24 */ s16        unk_24;
} FldMesTaskParam; // Size: 0x28

// Total number of messages across every .xls file.
#define FLD_MES_TOTAL 25233

extern void func_02010b84(TextObject* textObj, s32 x, s32 y, s32 width, s32 height);
extern void func_02025b68(void* textObj, UnkStruct_usedby_02025b68* params);
extern void func_02025e30(void* textObj);

extern u8 func_ov030_020ca778(UnkOv31Struct* arg0, u16 mesNo, s32 arg2, const u8* winKind);

extern const TaskHandle Task_Window;

// The selectable message window styles
static const FldMesWinKind FldMesWinKindTable[12] = {
    [5]  = {0x0B,           "叫び", FALSE}, // "scream"
    [1]  = {0x04,           "wide",  TRUE},
    [4]  = {0x05,           "携帯", FALSE}, // "cell phone"
    [3]  = {0x09,           "普通", FALSE}, // "ordinary"
    [0]  = {0x15,          "debug",  TRUE},
    [8]  = {0x0F,         "ごにょ", FALSE}, // "mumble"
    [9]  = {0x0D,         "心の中", FALSE}, // "in one's heart"
    [10] = {0x11,         "堅物者", FALSE}, // "rigid"
    [2]  = {0x06,        "detaile",  TRUE},
    [6]  = {0x07,     "通常うわさ", FALSE}, // "common rumors"
    [11] = {0x13,   "人でないもの", FALSE}, // "non-human entity"
    [7]  = {0x02, "チュートリアル", FALSE}, // "tutorial"
};

static const char* FldMesSelSequence = "Seq_FldMesSel()";

void FldMesSel_CreateTextView(FldMesSelObject* selObj) {
    UnkStruct_usedby_02025b68 stack;
    stack.unk_00 = 1;
    stack.unk_04 = 0;
    stack.unk_08 = selObj->datSlot;
    stack.unk_0C = &data_0205c9b0;
    stack.unk_10 = 0;
    stack.unk_14 = 0;
    stack.unk_18 = 0;
    stack.unk_1A = 0;
    stack.unk_1C = 0x20;
    stack.unk_1E = 0x18;

    func_02025b68(&selObj->unk_21614, &stack);
    Text_RenderToScreen(&selObj->text, 0, 0, "message select");
}

void FldMesSel_DrawMenu(FldMesSelObject* selObj) {
    func_02010b84(&selObj->text, 0, 0x10, 0x100, 0x60);
    Text_RenderToScreen(&selObj->text, 8, 0x10, Str_SPrintf("%s", MsgXls_GetXlsName(selObj->xlsIndex)));
    Text_RenderToScreen(&selObj->text, 8, 0x18,
                        Str_SPrintf("mesNum:%d total:%d", selObj->mesNo - MsgXls_GetMesNum(selObj->xlsIndex), selObj->mesNo));
    Text_RenderToScreen(&selObj->text, 8, 0x20, Str_SPrintf("win_kind:%s", FldMesWinKindTable[selObj->winKind].name));
    Text_RenderToScreen(&selObj->text, 0, (selObj->cursor * 8) + 0x10, ">");
}

void FldMesSel_DestroyTextView(FldMesSelObject* selObj) {
    func_02025e30(&selObj->unk_21614);
}

// Spawns the message window task previewing the current message in the given window style.
s32 FldMesSel_CreateMessageWindow(FldMesSelObject* selObj, u8 winKind) {
    FldMesSize      size;
    FldMesTaskParam param;
    u16             mesNo;

    param.unk_14  = 0;
    param.datSlot = selObj->datSlot;
    size.x        = 0x80;
    size.y        = 0x60;
    mesNo         = selObj->mesNo;
    param.mesNo   = mesNo;
    param.winKind = FldMesWinKindTable[winKind].kind;
    param.unk_24  = 0;
    param.unk_04  = size;
    param.unk_08  = size;
    param.unk_1A  = 0;
    param.unk_0C  = -1;
    param.unk_16  = 0;
    param.unk_1B  = 0;

    if (FldMesWinKindTable[winKind].hasBalloon) {
        param.unk_19 = func_ov030_020ca778(&selObj->unk_21A30, mesNo, 0, &param.winKind);
    } else {
        param.unk_19 = func_ov030_020ca778(NULL, mesNo, 0, &param.winKind);
    }

    param.unk_20 = 0;
    param.unk_1C = 0;
    return EasyTask_CreateTask(&selObj->taskPool, &Task_Window, NULL, 0, NULL, &param);
}

void FldMesSel_Open(FldMesSelObject* selObj) {
    FldMesSel_CreateTextView(selObj);
    FldMesSel_DrawMenu(selObj);
    selObj->messTask = FldMesSel_CreateMessageWindow(selObj, selObj->winKind);
}

void FldMesSel_Close(FldMesSelObject* selObj) {
    FldMesSel_DestroyTextView(selObj);
    EasyTask_DeleteTask(&selObj->taskPool, selObj->messTask);
}

// Steps the message index, rolling over into the neighbouring .xls file when it leaves this one.
void FldMesSel_MoveMessage(FldMesSelObject* selObj) {
    if (SysControl.buttonState.holdButtons == INPUT_BUTTON_LEFT) {
        if (selObj->mesNo != 0) {
            selObj->mesNo--;
            if ((s32)selObj->mesNo < MsgXls_GetMesNum(selObj->xlsIndex)) {
                selObj->xlsIndex--;
            }
        } else {
            selObj->mesNo    = FLD_MES_TOTAL - 1;
            selObj->xlsIndex = MsgXls_GetXlsFileCount() - 1;
        }
    } else if (SysControl.buttonState.holdButtons == INPUT_BUTTON_RIGHT) {
        selObj->mesNo++;
        if (selObj->mesNo >= FLD_MES_TOTAL) {
            selObj->mesNo    = 0;
            selObj->xlsIndex = 0;
        } else if ((s32)selObj->mesNo >= MsgXls_GetMesNumEnd(selObj->xlsIndex)) {
            selObj->xlsIndex++;
        }
    }
}

// Steps the selected .xls file and jumps the message index to that file's first message.
void FldMesSel_MoveXlsFile(FldMesSelObject* selObj) {
    if (SysControl.buttonState.holdButtons == INPUT_BUTTON_LEFT) {
        if (selObj->xlsIndex > 0) {
            selObj->xlsIndex--;
        } else {
            selObj->xlsIndex = MsgXls_GetXlsFileCount() - 1;
        }
        selObj->mesNo = MsgXls_GetMesNum(selObj->xlsIndex);
    } else if (SysControl.buttonState.holdButtons == INPUT_BUTTON_RIGHT) {
        selObj->xlsIndex++;
        if (selObj->xlsIndex >= MsgXls_GetXlsFileCount()) {
            selObj->xlsIndex = 0;
        }
        selObj->mesNo = MsgXls_GetMesNum(selObj->xlsIndex);
    }
}

// Steps the selected message window style.
void FldMesSel_MoveWindowKind(FldMesSelObject* selObj) {
    if (SysControl.buttonState.holdButtons == INPUT_BUTTON_LEFT) {
        if (selObj->winKind != 0) {
            selObj->winKind--;
        } else {
            selObj->winKind = 11;
        }
    } else if (SysControl.buttonState.holdButtons == INPUT_BUTTON_RIGHT) {
        if (selObj->winKind < 10) {
            selObj->winKind++;
        } else {
            selObj->winKind = 0;
        }
    }
}

void FldMesSel_HandleInput(FldMesSelObject* selObj) {
    OverlayTag tag;
    u16        mesNo = selObj->mesNo;
    u8         winKind;
    s32        cursor = selObj->cursor;

    winKind = selObj->winKind;

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_SELECT) {
        MainOvlDisp_Pop(&tag);
        return;
    }

    if (SysControl.buttonState.holdButtons == INPUT_BUTTON_UP) {
        selObj->cursor = cursor - 1;
        if (selObj->cursor < 0) {
            selObj->cursor = 2;
        }
    } else if (SysControl.buttonState.holdButtons == INPUT_BUTTON_DOWN) {
        selObj->cursor = cursor + 1;
        if (selObj->cursor > 2) {
            selObj->cursor = 0;
        }
    } else if (cursor == 0) {
        FldMesSel_MoveXlsFile(selObj);
    } else if (cursor == 1) {
        FldMesSel_MoveMessage(selObj);
    } else if (cursor == 2) {
        FldMesSel_MoveWindowKind(selObj);
    }

    if (mesNo == selObj->mesNo && cursor == selObj->cursor && winKind == selObj->winKind) {
        return;
    }

    FldMesSel_DrawMenu(selObj);
    if (selObj->messTask == -1) {
        return;
    }

    EasyTask_DeleteTask(&selObj->taskPool, selObj->messTask);
    selObj->messTask = FldMesSel_CreateMessageWindow(selObj, selObj->winKind);
}

void FldMesSel_Init(void* object) {
    const char*      sequence = FldMesSelSequence;
    FldMesSelObject* selObj   = Mem_AllocHeapTail(&gDebugHeap, sizeof(FldMesSelObject));

    Mem_SetSequence(&gDebugHeap, selObj, sequence);
    MainOvlDisp_SetCbArg(selObj);
    selObj->datSlot  = DatMgr_AllocateSlot();
    selObj->mesNo    = 0;
    selObj->xlsIndex = 0;
    selObj->winKind  = 0;
    selObj->cursor   = 0;
    func_ov030_020b20c0();
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = 0;
    selObj->prevResources                           = ResourceMgr_ReinitManagers(&selObj->resources);
    Mem_InitializeHeap(&selObj->heap, selObj->memBuffer, sizeof(selObj->memBuffer));
    EasyTask_InitializePool(&selObj->taskPool, &selObj->heap, 0x100, NULL, NULL);
    OvlMgr_LoadOverlay(3, &OVERLAY_31_ID);
    OvlMgr_LoadOverlay(4, &OVERLAY_36_ID);
    func_ov031_0210aa94(&selObj->unk_21A30);
    data_02066aec                                       = 0;
    data_02066eec                                       = 0;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 0;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 0;
    FldMesSel_Open(selObj);
    MainOvlDisp_NextProcessStage();
}

void FldMesSel_Update(void* object) {
    FldMesSelObject* selObj = object;

    OamMgr_Reset(g_OamMgr, 0, 0);
    OamMgr_Reset(&g_OamMgr[1], 0, 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    FldMesSel_HandleInput(selObj);
    EasyTask_UpdatePool(&selObj->taskPool);
    OamMgr_FlushCommands(g_OamMgr);
    OamMgr_FlushCommands(&g_OamMgr[1]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
}

void FldMesSel_Destroy(void* object) {
    FldMesSelObject* selObj = object;

    FldMesSel_Close(selObj);
    EasyTask_DestroyPool(&selObj->taskPool);
    func_ov031_0210aabc(&selObj->unk_21A30);
    OvlMgr_UnloadOverlay(4);
    OvlMgr_UnloadOverlay(3);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(selObj->datSlot);
    Mem_Free(&gDebugHeap, selObj);
}

static const OverlayProcess OvlProc_FldMesSel = {
    .init = FldMesSel_Init,
    .main = FldMesSel_Update,
    .exit = FldMesSel_Destroy,
};

void ProcessOverlay_FldMesSel(void* object) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        FldMesSel_Destroy(object);
    } else {
        OvlProc_FldMesSel.funcs[stage](object);
    }
}
