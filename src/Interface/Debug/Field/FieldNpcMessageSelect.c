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
#include "Util/SysFont.h"
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
    /* 0x21A30 */ SysFont          font;
    /* 0x21AAC */ s32              womanTask; // Task id of the message window previewing the woman's line, or -1
    /* 0x21AB0 */ s32              manTask;   // Task id of the message window previewing the man's line, or -1
    /* 0x21AB4 */ s32              lineIndex; // Offset into the current message range, in steps of 2
    /* 0x21AB8 */ s32              npcIndex;  // Index into FldNpcMesSelTable
    /* 0x21ABC */ s32              isMan;     // Selects the man's (1) or woman's (0) message range
} FldNpcMesSelObject;                         // Size: 0x21AC0

// Message ranges of one NPC: [0], [1] are the man's first and last message, [2], [3] the woman's.
typedef struct {
    /* 0x0 */ u16 manFirst;
    /* 0x2 */ u16 manLast;
    /* 0x4 */ u16 womanFirst;
    /* 0x6 */ u16 womanLast;
} FldNpcMesRange; // Size: 0x8

typedef struct {
    /* 0x0 */ const char*           name;
    /* 0x4 */ const FldNpcMesRange* range;
} FldNpcMesEntry; // Size: 0x8

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
} FldNpcMesSize;

// Parameter block of Tsk_Task_Window, the same one FieldMessageSelect.c fills in.
typedef struct {
    /* 0x00 */ s32           datSlot;
    /* 0x04 */ FldNpcMesSize unk_04;
    /* 0x08 */ FldNpcMesSize unk_08;
    /* 0x0C */ s32           unk_0C;
    /* 0x10 */ s32           mesNo;
    /* 0x14 */ s16           unk_14;
    /* 0x16 */ s16           unk_16;
    /* 0x18 */ u8            winKind;
    /* 0x19 */ u8            unk_19;
    /* 0x1A */ s8            unk_1A;
    /* 0x1B */ s8            unk_1B;
    /* 0x1C */ s32           unk_1C;
    /* 0x20 */ s32           unk_20;
    /* 0x24 */ s16           unk_24;
} FldNpcMesTaskParam; // Size: 0x28

#define FLD_NPC_MES_COUNT 38

extern void func_02010b84(TextObject* textObj, s32 x, s32 y, s32 width, s32 height);
extern void func_02025b68(void* textObj, UnkStruct_usedby_02025b68* params);
extern void func_02025e30(void* textObj);

extern u8 func_ov030_020ca778(SysFont* arg0, s32 mesNo, s32 arg2, const u8* winKind);

extern const TaskHandle Tsk_Task_Window;

// Message range tables, defined in the field data blobs.
extern const FldNpcMesRange FieldEncounterRangesByChapterGroup;
extern const FldNpcMesRange data_ov030_020d990c;
extern const FldNpcMesRange data_ov030_020d9914;
extern const FldNpcMesRange data_ov030_020d991c;
extern const FldNpcMesRange data_ov030_020da8e6;
extern const FldNpcMesRange data_ov030_020da8f6;
extern const FldNpcMesRange data_ov030_020da8fe;
extern const FldNpcMesRange data_ov030_020da906;
extern const FldNpcMesRange data_ov030_020da8ee;
extern const FldNpcMesRange data_ov030_020da90e;
extern const FldNpcMesRange FieldEncounterRangesByArea;
extern const FldNpcMesRange data_ov030_020da916;
extern const FldNpcMesRange data_ov030_020da91e;
extern const FldNpcMesRange data_ov030_020da926;
extern const FldNpcMesRange data_ov030_020da936;
extern const FldNpcMesRange data_ov030_020da93e;
extern const FldNpcMesRange data_ov030_020da946;
extern const FldNpcMesRange data_ov030_020da94e;
extern const FldNpcMesRange data_ov030_020da956;
extern const FldNpcMesRange data_ov030_020da95e;
extern const FldNpcMesRange data_ov030_020da966;
extern const FldNpcMesRange data_ov030_020da96e;
extern const FldNpcMesRange data_ov030_020da976;
extern const FldNpcMesRange data_ov030_020da986;
extern const FldNpcMesRange data_ov030_020da98e;
extern const FldNpcMesRange FieldEncounterRangesByRank;
extern const FldNpcMesRange data_ov030_020d9f32;
extern const FldNpcMesRange data_ov030_020d9f3a;
extern const FldNpcMesRange data_ov030_020d9f42;
extern const FldNpcMesRange data_ov030_020d9f4a;
extern const FldNpcMesRange data_ov030_020d9f52;
extern const FldNpcMesRange data_ov030_020d9f5a;
extern const FldNpcMesRange data_ov030_020d9f62;
extern const FldNpcMesRange data_ov030_020d9f6a;
extern const FldNpcMesRange data_ov030_020d9f72;
extern const FldNpcMesRange data_ov030_020d9f7a;
extern const FldNpcMesRange data_ov030_020d9f82;
extern const FldNpcMesRange data_ov030_020d9f8a;

static const char* FldNpcMesSelSequence = "Seq_FldNpcMesSel()";

static const FldNpcMesEntry FldNpcMesSelTable[FLD_NPC_MES_COUNT] = {
    {        "SHIKI_FREE", &FieldEncounterRangesByChapterGroup},
    {       "JOSHUA_FREE",                &data_ov030_020d990c},
    {         "BEAT_FREE",                &data_ov030_020d9914},
    {     "KAIKETSU_FREE",                &data_ov030_020d991c},
    {         "MAP_ce001",                &data_ov030_020da8e6},
    {         "MAP_ce002",                &data_ov030_020da8f6},
    {         "MAP_ce003",                &data_ov030_020da8fe},
    {         "MAP_ce004",                &data_ov030_020da906},
    {         "MAP_no001",                &data_ov030_020da8ee},
    {         "MAP_no002",                &data_ov030_020da90e},
    {         "MAP_no003",         &FieldEncounterRangesByArea},
    {         "MAP_no004",                &data_ov030_020da916},
    {         "MAP_no005",                &data_ov030_020da91e},
    {         "MAP_no006",                &data_ov030_020da926},
    {         "MAP_no008",                &data_ov030_020da936},
    {         "MAP_no009",                &data_ov030_020da93e},
    {         "MAP_no010",                &data_ov030_020da946},
    {         "MAP_ea001",                &data_ov030_020da94e},
    {         "MAP_ea002",                &data_ov030_020da956},
    {         "MAP_ea003",                &data_ov030_020da95e},
    {         "MAP_so001",                &data_ov030_020da966},
    {         "MAP_so002",                &data_ov030_020da96e},
    {         "MAP_we001",                &data_ov030_020da976},
    {         "MAP_we003",                &data_ov030_020da986},
    {         "MAP_we004",                &data_ov030_020da98e},
    {         "MUSRATTUS",         &FieldEncounterRangesByRank},
    {                "DB",                &data_ov030_020d9f32},
    {        "TIGREPUNKS",                &data_ov030_020d9f3a},
    {    "LAPINANGELIQUE",                &data_ov030_020d9f42},
    {     "DRAGONCOUTURE",                &data_ov030_020d9f4a},
    {          "HIPSNAKE",                &data_ov030_020d9f52},
    {            "PEGASO",                &data_ov030_020d9f5a},
    {     "SHEEPHEAVENLY",                &data_ov030_020d9f62},
    {"JUPITEROFTHEMONKEY",                &data_ov030_020d9f6a},
    {          "PAVOREAL",                &data_ov030_020d9f72},
    {      "NATURALPUPPY",                &data_ov030_020d9f7a},
    {          "WILDBOAR",                &data_ov030_020d9f82},
    {            "GATITO",                &data_ov030_020d9f8a},
};

void FldNpcMesSel_CreateTextView(FldNpcMesSelObject* selObj) {
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
    Text_RenderToScreen(&selObj->text, 0, 0, "NPC message select");
}

void FldNpcMesSel_DrawMenu(FldNpcMesSelObject* selObj) {
    const char* sex = selObj->isMan != 0 ? "MAN" : "WOMAN";

    func_02010b84(&selObj->text, 0, 0x10, 0x100, 0x60);
    Text_RenderToScreen(&selObj->text, 8, 0x10,
                        Str_SPrintf("%s_%s_%03d", FldNpcMesSelTable[selObj->npcIndex].name, sex, selObj->lineIndex / 2 + 1));
}

void FldNpcMesSel_DestroyTextView(FldNpcMesSelObject* selObj) {
    func_02025e30(&selObj->unk_21614);
}

s32 FldNpcMesSel_CreateMessageWindow(FldNpcMesSelObject* selObj, u8 winKind) {
    FldNpcMesTaskParam param;

    u16 firstMes;
    if (selObj->isMan != 0) {
        firstMes = FldNpcMesSelTable[selObj->npcIndex].range->manFirst;
    } else {
        firstMes = FldNpcMesSelTable[selObj->npcIndex].range->womanFirst;
    }

    FldNpcMesSize size;
    size.x = 0x80;
    size.y = 0x60;

    param.unk_14  = 0;
    param.datSlot = selObj->datSlot;
    param.mesNo   = selObj->lineIndex + firstMes;
    param.winKind = winKind;
    param.unk_24  = 0;

    if (winKind == 6) {
        size.y       = 0x8C;
        param.unk_24 = 1;
        param.mesNo  = param.mesNo + 1;
    }

    param.unk_04 = size;
    param.unk_08 = size;
    param.unk_1A = 0;
    param.unk_0C = -1;
    param.unk_16 = 0;
    param.unk_1B = 0;
    param.unk_19 = func_ov030_020ca778(&selObj->font, param.mesNo, 0, &param.winKind);
    param.unk_20 = 0;
    param.unk_1C = 0;
    return EasyTask_CreateTask(&selObj->taskPool, &Tsk_Task_Window, 0, 0, 0, &param);
}

void FldNpcMesSel_Open(FldNpcMesSelObject* selObj) {
    FldNpcMesSel_CreateTextView(selObj);
    FldNpcMesSel_DrawMenu(selObj);
    selObj->womanTask = FldNpcMesSel_CreateMessageWindow(selObj, 4);
    selObj->manTask   = FldNpcMesSel_CreateMessageWindow(selObj, 6);
}

void FldNpcMesSel_Close(FldNpcMesSelObject* selObj) {
    FldNpcMesSel_DestroyTextView(selObj);
    EasyTask_DeleteTask(&selObj->taskPool, selObj->womanTask);
    EasyTask_DeleteTask(&selObj->taskPool, selObj->manTask);
}

void FldNpcMesSel_HandleInput(FldNpcMesSelObject* selObj) {
    s32 lineIndex = selObj->lineIndex;
    s32 npcIndex  = selObj->npcIndex;
    s32 isMan     = selObj->isMan;

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_SELECT) {
        OverlayTag tag;
        MainOvlDisp_Pop(&tag);
        return;
    }

    if (SysControl.buttonState.holdButtons == INPUT_BUTTON_LEFT) {
        selObj->lineIndex = lineIndex - 2;
        if (selObj->lineIndex < 0) {
            if (selObj->isMan != 0) {
                selObj->isMan = 0;
                selObj->npcIndex--;
                if (selObj->npcIndex < 0) {
                    selObj->npcIndex = FLD_NPC_MES_COUNT - 1;
                }
                selObj->lineIndex = FldNpcMesSelTable[selObj->npcIndex].range->womanLast -
                                    FldNpcMesSelTable[selObj->npcIndex].range->womanFirst;
            } else {
                selObj->isMan = 1;
                selObj->lineIndex =
                    FldNpcMesSelTable[selObj->npcIndex].range->manLast - FldNpcMesSelTable[selObj->npcIndex].range->manFirst;
            }
        }
    } else if (SysControl.buttonState.holdButtons == INPUT_BUTTON_RIGHT) {
        selObj->lineIndex = lineIndex + 2;
        if (selObj->isMan != 0) {
            if (selObj->lineIndex >
                FldNpcMesSelTable[selObj->npcIndex].range->manLast - FldNpcMesSelTable[selObj->npcIndex].range->manFirst)
            {
                selObj->lineIndex = 0;
                selObj->isMan     = 0;
            }
        } else if (selObj->lineIndex > FldNpcMesSelTable[selObj->npcIndex].range->womanLast -
                                           FldNpcMesSelTable[selObj->npcIndex].range->womanFirst)
        {
            selObj->lineIndex = 0;
            selObj->isMan     = 1;
            selObj->npcIndex++;
            if ((u32)selObj->npcIndex >= 38) {
                selObj->npcIndex = 0;
            }
        }
    } else if (SysControl.buttonState.holdButtons == INPUT_BUTTON_UP) {
        if (isMan != 0) {
            selObj->isMan = 0;
            selObj->npcIndex--;
            if (selObj->npcIndex < 0) {
                selObj->npcIndex = FLD_NPC_MES_COUNT - 1;
            }
            selObj->lineIndex =
                FldNpcMesSelTable[selObj->npcIndex].range->womanLast - FldNpcMesSelTable[selObj->npcIndex].range->womanFirst;
        } else {
            selObj->isMan = 1;
            selObj->lineIndex =
                FldNpcMesSelTable[selObj->npcIndex].range->manLast - FldNpcMesSelTable[selObj->npcIndex].range->manFirst;
        }
    } else if (SysControl.buttonState.holdButtons == INPUT_BUTTON_DOWN) {
        if (isMan != 0) {
            selObj->isMan     = 0;
            selObj->lineIndex = 0;
        } else {
            selObj->isMan     = 1;
            selObj->lineIndex = 0;
            selObj->npcIndex++;
            if ((u32)selObj->npcIndex >= 38) {
                selObj->npcIndex = 0;
            }
        }
    }

    if (lineIndex != selObj->lineIndex || npcIndex != selObj->npcIndex || isMan != selObj->isMan) {
        FldNpcMesSel_DrawMenu(selObj);
        EasyTask_DeleteTask(&selObj->taskPool, selObj->womanTask);
        EasyTask_DeleteTask(&selObj->taskPool, selObj->manTask);
        selObj->womanTask = FldNpcMesSel_CreateMessageWindow(selObj, 4);
        selObj->manTask   = FldNpcMesSel_CreateMessageWindow(selObj, 6);
    }
}

void FldNpcMesSel_Init(void* object) {
    const char*         sequence = FldNpcMesSelSequence;
    FldNpcMesSelObject* selObj   = Mem_AllocHeapTail(&gDebugHeap, sizeof(FldNpcMesSelObject));

    Mem_SetSequence(&gDebugHeap, selObj, sequence);
    MainOvlDisp_SetCbArg(selObj);
    selObj->datSlot   = DatMgr_AllocateSlot();
    selObj->lineIndex = 0;
    selObj->npcIndex  = 0;
    selObj->isMan     = 1;
    func_ov030_020b20c0();
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = 0;
    selObj->prevResources                           = ResourceMgr_ReinitManagers(&selObj->resources);
    Mem_InitializeHeap(&selObj->heap, selObj->memBuffer, sizeof(selObj->memBuffer));
    EasyTask_InitializePool(&selObj->taskPool, &selObj->heap, 0x100, NULL, NULL);
    OvlMgr_LoadOverlay(3, &OVERLAY_31_ID);
    SysFont_Init(&selObj->font);
    data_02066aec                                       = 0;
    data_02066eec                                       = 0;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 0;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 0;
    FldNpcMesSel_Open(selObj);
    MainOvlDisp_NextProcessStage();
}

void FldNpcMesSel_Update(void* object) {
    FldNpcMesSelObject* selObj = object;

    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    FldNpcMesSel_HandleInput(selObj);
    EasyTask_UpdatePool(&selObj->taskPool);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
}

void FldNpcMesSel_Destroy(void* object) {
    FldNpcMesSelObject* selObj = object;

    FldNpcMesSel_Close(selObj);
    EasyTask_DestroyPool(&selObj->taskPool);
    SysFont_Destroy(&selObj->font);
    OvlMgr_UnloadOverlay(3);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(selObj->datSlot);
    Mem_Free(&gDebugHeap, selObj);
}

static const OverlayProcess OvlProc_FldNpcMesSel = {
    .init = FldNpcMesSel_Init,
    .main = FldNpcMesSel_Update,
    .exit = FldNpcMesSel_Destroy,
};

void ProcessOverlay_FldNpcMesSel(void* object) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        FldNpcMesSel_Destroy(object);
    } else {
        OvlProc_FldNpcMesSel.funcs[stage](object);
    }
}
