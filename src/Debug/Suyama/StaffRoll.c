#include "Debug/Suyama/StaffRoll.h"
#include "BgResMgr.h"
#include "Interrupts.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "System.h"
#include "common_data.h"
#include <NitroSDK/fx.h>
#include <types.h>

#define round(value) ((value) > 0 ? (f32)(value << 0xC) + 0.5f : (f32)(value << 0xC) - 0.5f)

const BinIdentifier data_ov042_020847c0[] = {
    {0x2A, "Apl_Suy/Grp_Staff_PartA_Pack.bin"},
    {0x2A, "Apl_Suy/Grp_Staff_PartB_Pack.bin"},
    {0x2A, "Apl_Suy/Grp_Staff_PartC_Pack.bin"},
    {0x2A, "Apl_Suy/Grp_Staff_PartD_Pack.bin"},
};

extern void func_ov030_020d4f74(void*);

void func_ov042_020846e4(void);
void func_ov042_020842e4(void);
void func_ov042_020846d0(void);

void func_ov042_02082f94(StaffRoll_CallbackStruct* state);
void func_ov042_02082ed8(StaffRoll_CallbackStruct* state);
void func_ov042_02082e9c(StaffRoll_CallbackStruct* state);

void func_ov042_020824e0(StaffRollState* state);
void func_ov042_02082620(StaffRollState* state);
void func_ov042_02082700(StaffRollState* state);

void func_ov042_020828c4(StaffRoll_CallbackStruct* pool, CommandUnk04* args);
void func_ov042_0208293c(StaffRoll_CallbackStruct* pool, CommandUnk04* args);
void func_ov042_02082a30(StaffRoll_CallbackStruct* pool, CommandUnk04* args);
void func_ov042_02082b20(StaffRoll_CallbackStruct* pool, CommandUnk04* args);
void func_ov042_02082bf4(StaffRoll_CallbackStruct* pool, CommandUnk04* args);
void func_ov042_02082c98(StaffRoll_CallbackStruct* pool, CommandUnk04* args);
void func_ov042_02082cf4(StaffRoll_CallbackStruct* pool, CommandUnk04* args);
void func_ov042_02082d50(StaffRoll_CallbackStruct* pool, CommandUnk04* args);
void StaffRoll_CreateStaffBgTask(StaffRoll_CallbackStruct* pool, CommandUnk04* args);
void StaffRoll_CreateBgScrollTask(StaffRoll_CallbackStruct* pool, CommandUnk04* args);
void StaffRoll_CreateFontRollTask(StaffRoll_CallbackStruct* pool, CommandUnk04* args);
void StaffRoll_CreateBgBrightTask(StaffRoll_CallbackStruct* pool, CommandUnk04* args);

s32 func_ov042_020840ac(FontRoll* fontRoll, FontRollArgs* args);
s32 func_ov042_020841cc(FontRoll* fontRoll);
s32 func_ov042_0208429c(FontRoll* fontRoll);

void StaffBg_CreateTask(TaskPool* pool, u32 arg1, u32 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7);
void BgScroll_CreateTask(TaskPool* pool, u32 arg1, u32 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7);
s32  FontRoll_CreateTask(TaskPool* taskPool, s32 arg1, s32 arg2, s32 arg3, u16 arg4, s16 arg5);
void BgBright_CreateTask(TaskPool* pool, DisplayEngine engine, s16 startBrightness, s16 endBrightness, s16 duration);

s32 func_ov042_020836d0(TaskPool* pool, Task* task, void* args, s32 stage);
s32 func_ov042_02084064(TaskPool* pool, Task* task, void* args, s32 stage);
s32 StaffBg_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const OverlayProcess OvlProc_StaffRoll = {
    .init = func_ov042_020824e0,
    .main = func_ov042_02082620,
    .exit = func_ov042_02082700,
};

void func_ov042_020824a0(StaffRollState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        func_ov042_02082700(state);
    } else {
        OvlProc_StaffRoll.funcs[stage](state);
    }
}

void func_ov042_020824e0(StaffRollState* state) {
    if (state == NULL) {
        state = Mem_AllocHeapTail(&gDebugHeap, sizeof(StaffRollState));
        Mem_SetSequence(&gDebugHeap, state, "Seq_Staff()");
        MainOvlDisp_SetCbArg(state);
        MI_CpuSet(state, 0, sizeof(StaffRollState));
    }
    FS_LoadOverlay(0, &OVERLAY_31_ID);
    func_ov031_0210aa94(&state->unk_2171C);
    func_ov042_020842e4();
    state->unk_11580 = func_0200cef0(state);
    Mem_InitializeHeap(&state->memPool, &state->memPoolBuffer, sizeof(state->memPoolBuffer));
    EasyTask_InitializePool(&state->taskPool, &state->memPool, 0x200, NULL, NULL);
    state->unk_21614.pool = &state->taskPool;
    data_02066aec         = 0;
    data_02066eec         = 0;
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)func_ov042_02082f94, &state->unk_21614, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov042_02082ed8, &state->unk_21614, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov042_02082e9c, &state->unk_21614, 0);
    MainOvlDisp_NextProcessStage();
}

void func_ov042_02082620(StaffRollState* state) {
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    DebugOvlDisp_Run();
    EasyTask_ProcessPendingTasks(&state->taskPool);
    EasyTask_UpdateActiveTasks(&state->taskPool);
    if (DebugOvlDisp_IsStackAtBase() != FALSE) {
        OverlayTag tag;
        data_02074e10.unk_28 = 2;
        MainOvlDisp_ReplaceTop(&tag, &OVERLAY_30_ID, func_ov030_020d4f74, NULL, 0);
        return;
    }
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc[0], 0);
    func_0200bf60(data_0206b3cc[1], 0);
}

// Nonmatching: Regswaps
void func_ov042_02082700(StaffRollState* state) {
    EasyTask_DestroyPool(&state->taskPool);
    func_ov031_0210aabc(&state->unk_2171C);
    func_0200cef0(0);

    for (s32 engine = 0; engine < 2; engine++) {
        StaffRollUnkC* unkC = &state->unk_21614.unk_10[engine];
        for (s32 i = 0; i < 4; i++) {
            DatMgr_ReleaseData(unkC->group1[i].data);
            DatMgr_ReleaseData(unkC->group2[i].data);
            BgResMgr_ReleaseChar(g_BgResourceManagers[engine], unkC->group1[i].unk_04);
            BgResMgr_ReleaseScreen(g_BgResourceManagers[engine], unkC->group2[i].unk_04);
        }

        for (s32 i = 0; i < 3; i++) {
            DatMgr_ReleaseData(unkC->group3[i].data);
            func_0200afec(data_0206b3cc[i], unkC->group3[engine].unk_04);
        }
    }
    func_ov042_020846d0();
    FS_UnloadOverlay(0, &OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
}

void func_ov042_020827ec(StaffRoll_CallbackStruct* cbArg, s32 r1, CommandUnk04* args) {
    switch (r1) {
        case 0:
        case 1:
            break;
        case 3:
            func_ov042_0208293c(cbArg, args);
            return;
        case 2:
            func_ov042_020828c4(cbArg, args);
            return;
        case 4:
            func_ov042_02082a30(cbArg, args);
            return;
        case 5:
            func_ov042_02082b20(cbArg, args);
            return;
        case 6:
            func_ov042_02082bf4(cbArg, args);
            return;
        case 7:
            func_ov042_02082c98(cbArg, args);
            return;
        case 8:
            func_ov042_02082cf4(cbArg, args);
            return;
        case 9:
            func_ov042_02082d50(cbArg, args);
            return;
        case 10:
            StaffRoll_CreateStaffBgTask(cbArg, args);
            return;
        case 11:
            StaffRoll_CreateBgScrollTask(cbArg, args);
            return;
        case 12:
            StaffRoll_CreateFontRollTask(cbArg, args);
            return;
        case 13:
            StaffRoll_CreateBgBrightTask(cbArg, args);
            return;
    }
}

void func_ov042_020828c4(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    CommandUnk04* r2 = (CommandUnk04*)args;

    s16 temp_ip = r2->unk_00;
    u32 idx     = temp_ip;
    s16 temp_r3 = r2->unk_02;

    if (r2->unk_04 != 0) {
        DisplayControlSettings* temp_r2 = &g_DisplaySettings.controls[idx];
        s32                     temp_r1 = 1 << temp_r3;
        if (temp_r1 < 0) {
            temp_r2->layers = 0x1F;
        } else {
            temp_r2->layers |= temp_r1;
        }
    } else {
        DisplayControlSettings* temp_r2 = &g_DisplaySettings.controls[idx];
        s32                     temp_r1 = 1 << temp_r3;
        if (temp_r1 < 0) {
            temp_r2->layers = 0;
        } else {
            temp_r2->layers &= ~temp_r1;
        }
    }
}

void func_ov042_0208293c(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    s16 tmp  = args->unk_00;
    s16 tmp2 = args->unk_02;
    u16 tmp3 = args->unk_04;
    u16 tmp4 = args->unk_06;
    u16 tmp5 = args->unk_08;

    StaffRollUnkA* var = &cbArg->unk_10[tmp].group1[tmp2];

    var->unk_08 = tmp3;
    var->unk_0A = tmp4;
    var->unk_0C = tmp5;

    Data* data = var->data;
    if (data != NULL) {
        DatMgr_ReleaseData(data);
        var->data = NULL;
    }

    if (var->unk_04 != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[tmp], var->unk_04);
        var->unk_04 = NULL;
    }

    Data* var2 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov042_020847c0[tmp3], tmp4, FALSE);
    u8*   param;
    if (var2 != NULL) {
        param = (u8*)var2->buffer;
        param += 0x20;
        param += ((u32*)param)[2];
    } else {
        param = NULL;
    }

    void* temp_04 = BgResMgr_AllocChar32(g_BgResourceManagers[tmp], param,
                                         g_DisplaySettings.engineState[tmp].bgSettings[tmp2].charBase, 0, tmp5);
    var->data     = var2;
    var->unk_04   = temp_04;
}

void func_ov042_02082a30(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    s16 tmp  = args->unk_00;
    s16 tmp2 = args->unk_02;
    u16 tmp3 = args->unk_04;
    u16 tmp4 = args->unk_06;

    StaffRollUnkB* var  = &cbArg->unk_10[tmp].group2[tmp2];
    Data*          data = var->data;

    if (data != NULL) {
        DatMgr_ReleaseData(data);
        var->data = NULL;
    }

    if (var->unk_04 != NULL) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[tmp], var->unk_04);
        var->unk_04 = NULL;
    }

    Data* var2 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov042_020847c0[tmp3], tmp4, FALSE);
    u8*   param;
    if (var2 != NULL) {
        param = (u8*)var2->buffer;
        param += 0x20;
        param += ((u32*)param)[2];
    } else {
        param = NULL;
    }

    void* temp_04 =
        BgResMgr_AllocScreen(g_BgResourceManagers[tmp], param, g_DisplaySettings.engineState[tmp].bgSettings[tmp2].screenBase,
                             g_DisplaySettings.engineState[tmp].bgSettings[tmp2].screenSizeText);
    var->data   = var2;
    var->unk_04 = temp_04;
}

void func_ov042_02082b20(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    s16 tmp  = args->unk_00;
    u16 tmp2 = args->unk_02;
    u16 tmp3 = args->unk_04;
    u16 tmp4 = args->unk_06;
    u16 tmp5 = args->unk_08;
    u16 tmp6 = args->unk_0A;

    StaffRollUnkB* var  = &cbArg->unk_10[tmp].group3[tmp2];
    Data*          data = var->data;

    if (data != NULL) {
        DatMgr_ReleaseData(data);
        var->data = NULL;
    }

    if (var->unk_04 != NULL) {
        func_0200afec(data_0206b3cc[tmp], var->unk_04);
        var->unk_04 = NULL;
    }

    Data* var2 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov042_020847c0[tmp5], tmp6, FALSE);
    u8*   param;
    if (var2 != NULL) {
        param = (u8*)var2->buffer;
        param += 0x20;
        param += ((u32*)param)[2];
    } else
        param = NULL;

    void* temp_04 = func_0200adf8(data_0206b3cc[tmp], param, 0, tmp3, tmp4);
    var->data     = var2;
    var->unk_04   = temp_04;
}

void func_ov042_02082bf4(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    CommandUnk04* r2 = (CommandUnk04*)args;

    s16            tmp  = r2->unk_00;
    s16            tmp2 = r2->unk_02;
    StaffRollUnkA* var  = &cbArg->unk_10[tmp].group1[tmp2];
    if (var->unk_04 != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[tmp], var->unk_04);
        var->unk_04 = NULL;
    }
    u8*   param;
    Data* var2 = var->data;
    if (var2 != NULL) {
        param = (u8*)var2->buffer;
        param += 0x20;
        param += ((u32*)param)[2];
    } else {
        param = NULL;
    }

    var->unk_04 = BgResMgr_AllocChar32(g_BgResourceManagers[tmp], param,
                                       g_DisplaySettings.engineState[tmp].bgSettings[tmp2].charBase, 0, var->unk_0C);
}

void func_ov042_02082c98(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    s16            tmp = args->unk_00;
    StaffRollUnkA* var = &cbArg->unk_10[tmp].group1[args->unk_02];
    if (var->data != NULL) {
        DatMgr_ReleaseData(var->data);
        var->data = NULL;
    }
    if (var->unk_04 != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[tmp], var->unk_04);
        var->unk_04 = NULL;
    }
}

void func_ov042_02082cf4(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    s16            tmp = args->unk_00;
    StaffRollUnkB* var = &cbArg->unk_10[tmp].group2[args->unk_02];
    if (var->data != NULL) {
        DatMgr_ReleaseData(var->data);
        var->data = NULL;
    }
    if (var->unk_04 != NULL) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[tmp], var->unk_04);
        var->unk_04 = NULL;
    }
}

void func_ov042_02082d50(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    s16            tmp = args->unk_00;
    StaffRollUnkB* var = &cbArg->unk_10[tmp].group3[(u16)args->unk_02];
    if (var->data != NULL) {
        DatMgr_ReleaseData(var->data);
        var->data = NULL;
    }
    if (var->unk_04 != NULL) {
        func_0200afec(data_0206b3cc[tmp], var->unk_04);
        var->unk_04 = NULL;
    }
}

void StaffRoll_CreateStaffBgTask(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    StaffBg_CreateTask(cbArg->pool, args->unk_00, args->unk_02, args->unk_04, args->unk_06, args->unk_08, args->unk_0A,
                       args->unk_0C);
}

void StaffRoll_CreateBgScrollTask(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    BgScroll_CreateTask(cbArg->pool, args->unk_00, args->unk_02, args->unk_04, args->unk_06, args->unk_08, args->unk_0A,
                        args->unk_0C);
}

void StaffRoll_CreateFontRollTask(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    FontRoll_CreateTask(cbArg->pool, cbArg, args->unk_00, args->unk_02, args->unk_04, args->unk_06);
}

void StaffRoll_CreateBgBrightTask(StaffRoll_CallbackStruct* cbArg, CommandUnk04* args) {
    BgBright_CreateTask(cbArg->pool, args->unk_00, args->unk_02, args->unk_04, args->unk_06);
}

const BinIdentifier data_ov042_020847e0 = {42, "Apl_Suy/staff_cmdlist.bin"};

void func_ov042_02082e9c(StaffRoll_CallbackStruct* state) {
    CriSndMgr_SetLpFlg(FALSE);
    func_0202733c(0x22);
    state->commandlist = DatMgr_LoadRawData(1, NULL, 0, &data_ov042_020847e0);
    DebugOvlDisp_Pop();
}

void func_ov042_02082ed8(StaffRoll_CallbackStruct* cbArg) {
    s32 cmdIndex;

    Command* cmdlist = (Command*)cbArg->commandlist->buffer;

    cbArg->unk_100_2 = 0;
    cmdIndex         = cbArg->commandIndex;

    Command* cmd = &cmdlist[cmdIndex];

    if (cmdlist[cmdIndex].group1Index != 0) {
        while (cmdlist[cmdIndex].group1Index != 0) {
            if (cmd->StaffRollUnkCIndex > cbArg->unk_04) {
                break;
            }

            func_ov042_020827ec(cbArg, cmd->group1Index, &cmd->unk_04);

            cmd++;
            cmdIndex++;
        }
    }

    cbArg->commandIndex = cmdIndex;

    if (cmdlist[cmdIndex].group1Index == 0 && cmdlist[cmdIndex].StaffRollUnkCIndex <= cbArg->unk_04) {
        DebugOvlDisp_Pop();
    }

    if (cbArg->unk_100_1 == 0) {
        cbArg->unk_04++;
    }
}

void func_ov042_02082f94(StaffRoll_CallbackStruct* state) {
    DatMgr_ReleaseData(state->commandlist);
    func_02027388(0x22);
    CriSndMgr_SetLpFlg(TRUE);
    if (DebugOvlDisp_IsStackAtBase() == FALSE)
        DebugOvlDisp_Pop();
}

BOOL func_ov042_02082fc4(UnkOv31Struct* arg0) {
    return arg0->unk_14 != NULL;
}

// Nonmatching
void func_ov042_02082fd8(UnkOv31Struct* arg0, s32 r1) {
    u16* in_lr;

    if (arg0 != NULL) {
        in_lr = arg0->unk_14;
    }

    if (in_lr != NULL) {
        u16 uVar1 = *in_lr;

        while (uVar1 != 0xffff) {
            if ((uVar1 & 0xfff0) == 0xffb0) {
                *in_lr = r1 & 0xf | 0xffb0;
            }
            in_lr++;
            uVar1 = *in_lr;
        }
        return;
    }
}

static s32 BgBright_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static s32 BgBright_Init(BgBright* bright, BgBrightArgs* args);
static s32 BgBright_Update(BgBright* bright);

static const TaskHandle data_ov042_020847e8 = {"Tsk_BgBright", BgBright_RunTask, 0x18};

void BgBright_CreateTask(TaskPool* pool, DisplayEngine engine, s16 startBrightness, s16 endBrightness, s16 duration) {
    BgBrightArgs args;
    args.duration        = duration;
    args.engine          = engine;
    args.startBrightness = startBrightness;
    args.endBrightness   = endBrightness;
    EasyTask_CreateTask(pool, &data_ov042_020847e8, 0, 0, 0, &args);
}

static s32 BgBright_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    BgBright* bright = (BgBright*)task->data;

    switch (stage) {
        case 0:
            return BgBright_Init(bright, (BgBrightArgs*)args);
        case 1:
            return BgBright_Update(bright);
        default:
            return TRUE;
    }
}

// Nonmatching: Minor instruction reordering
static BOOL BgBright_Init(BgBright* bright, BgBrightArgs* args) {
    bright->engine          = args->engine;
    bright->startBrightness = args->startBrightness;
    bright->endBrightness   = args->endBrightness;
    bright->duration        = args->duration;

    bright->currentBrightness = round(args->startBrightness);

    bright->step = FX_Divide((s32)round(args->endBrightness) - (s32)round(args->startBrightness), round(args->duration));

    bright->elapsedFrames = 0;
    bright->maxFrames     = args->duration;

    s32 brightness = args->startBrightness;
    if (brightness > 16) {
        brightness = 16;
    } else if (brightness < -16) {
        brightness = -16;
    }

    g_DisplaySettings.controls[args->engine].brightness = brightness;
    return TRUE;
}

// Nonmatching: Minor instruction reordering
static BOOL BgBright_Update(BgBright* bright) {
    BOOL isAnimating = TRUE;

    bright->currentBrightness += bright->step;

    if (bright->elapsedFrames >= bright->maxFrames) {
        bright->currentBrightness = round(bright->endBrightness);
        isAnimating               = FALSE;
    }

    s32 brightness = bright->currentBrightness >> 0xC;
    if (brightness < -16) {
        brightness = -16;
    } else if (brightness > 16) {
        brightness = 16;
    }
    if (brightness > 16) {
        brightness = 16;
    } else if (brightness < -16) {
        brightness = -16;
    }
    g_DisplaySettings.controls[bright->engine].brightness = brightness;
    bright->elapsedFrames++;
    return isAnimating;
}

s32 StaffBg_Init(StaffBg* staffBg, StaffBgArgs* args);
s32 StaffBg_Update(StaffBg* staffBg);

static const TaskHandle data_ov042_020847f4 = {"Tsk_StaffBg", StaffBg_RunTask, 0x20};

void StaffBg_CreateTask(TaskPool* pool, u32 arg1, u32 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7) {
    StaffBgArgs args;
    args.unk_00 = arg1;
    args.unk_04 = arg2;
    args.unk_08 = arg3;
    args.unk_0C = arg4;
    args.unk_0E = arg5;
    args.unk_10 = arg6;
    args.unk_12 = arg7;
    EasyTask_CreateTask(pool, &data_ov042_020847f4, NULL, 0, NULL, &args);
}

s32 func_ov042_02083344(StaffBg* staffBg) {
    switch (staffBg->unk_12) {
        case 32:
            return ((0x1F000 - staffBg->unk_14) >> 0xC);
        case 33:
            return (staffBg->unk_14 >> 0xC);
        default:
            return staffBg->unk_12;
    }
}

s32 StaffBg_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    StaffBg* staffBg = (StaffBg*)task->data;

    switch (stage) {
        case 0:
            return StaffBg_Init(staffBg, (StaffBgArgs*)args);
        case 1:
            return StaffBg_Update(staffBg);
        default:
            return 1;
    }
}

// Nonmatching: Regswaps
s32 StaffBg_Init(StaffBg* staffBg, StaffBgArgs* args) {
    g_DisplaySettings.engineState[args->unk_00].blendMode = 1;

    s32 engine = args->unk_00;
    s32 layer0 = args->unk_04;
    s32 layer1 = args->unk_08;

    if (args->unk_08 == 0) {
        g_DisplaySettings.engineState[engine].blendLayer0 = layer0;
        g_DisplaySettings.engineState[engine].blendLayer1 = 32;
    } else {
        g_DisplaySettings.engineState[engine].blendLayer0 = layer0;
        g_DisplaySettings.engineState[engine].blendLayer1 = layer1;
    }

    staffBg->unk_00 = args->unk_00;
    staffBg->unk_04 = args->unk_04;
    staffBg->unk_08 = args->unk_08;
    staffBg->unk_0C = args->unk_0C;
    staffBg->unk_0E = args->unk_0E;
    staffBg->unk_10 = args->unk_10;
    staffBg->unk_12 = args->unk_12;

    staffBg->unk_14 = round(args->unk_0C);

    f32 var_r0_2 = round(args->unk_10);
    f32 var_r0_3 = round(args->unk_0C);
    f32 var_r0_4 = round(args->unk_0E);

    staffBg->unk_18 = FX_Divide((s32)var_r0_4 - (s32)var_r0_3, (s32)var_r0_2);
    staffBg->unk_1C = 0;
    staffBg->unk_1E = (s16)args->unk_10;

    s32 temp_r0_9 = func_ov042_02083344(staffBg);

    engine = args->unk_00;

    g_DisplaySettings.engineState[engine].blendCoeff0 = args->unk_0C;
    g_DisplaySettings.engineState[engine].blendCoeff1 = temp_r0_9;
    return 1;
}

s32 StaffBg_Update(StaffBg* staffBg) {
    s32 var_r4 = 1;
    staffBg->unk_14 += staffBg->unk_18;

    if (staffBg->unk_1C >= staffBg->unk_1E) {
        staffBg->unk_14 = round(staffBg->unk_0E);
        var_r4          = 0;
    }

    s32 coeff0 = staffBg->unk_14 >> 0xC;
    s32 coeff1 = func_ov042_02083344(staffBg);

    if (coeff0 < 0) {
        coeff0 = 0;
    } else if (coeff0 > 0x1F) {
        coeff0 = 0x1F;
    }

    if (coeff1 < 0) {
        coeff1 = 0;
    } else if (coeff1 > 0x1F) {
        coeff1 = 0x1F;
    }

    s32 engine = staffBg->unk_00;

    g_DisplaySettings.engineState[engine].blendCoeff0 = coeff0;
    g_DisplaySettings.engineState[engine].blendCoeff1 = coeff1;
    staffBg->unk_1C++;
    return var_r4;
}

s32 func_ov042_02083708(BgScroll* scroll, BgScrollArgs* args);
s32 func_ov042_02083838(BgScroll* scroll);

const TaskHandle data_ov042_02084800 = {"Tsk_BgScroll", func_ov042_020836d0, 0x28};

void BgScroll_CreateTask(TaskPool* pool, u32 arg1, u32 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7) {
    BgScrollArgs args;
    args.unk_00 = arg1;
    args.unk_04 = arg2;
    args.unk_08 = arg3;
    args.unk_0A = arg4;
    args.unk_0C = arg5;
    args.unk_0E = arg6;
    args.unk_10 = arg7;
    EasyTask_CreateTask(pool, &data_ov042_02084800, NULL, 0, NULL, &args);
}

s32 func_ov042_020836d0(TaskPool* pool, Task* task, void* args, s32 stage) {
    BgScroll* scroll = (BgScroll*)task->data;

    switch (stage) {
        case 0:
            return func_ov042_02083708(scroll, args);
        case 1:
            return func_ov042_02083838(scroll);
        default:
            return 1;
    }
}

// Nonmatching: Regswaps, incorrect instructions for DisplayEngineState access
s32 func_ov042_02083708(BgScroll* scroll, BgScrollArgs* args) {
    MI_CpuSet(scroll, 0, sizeof(BgScroll));
    scroll->unk_00 = args->unk_00;
    scroll->unk_04 = args->unk_04;
    scroll->unk_08 = args->unk_08;
    scroll->unk_0A = args->unk_0A;
    scroll->unk_0C = args->unk_0C;
    scroll->unk_0E = args->unk_0E;
    scroll->unk_10 = args->unk_10;

    scroll->unk_24 = 0;

    s32 vOffset = round(scroll->unk_0A);
    s32 hOffset = round(scroll->unk_08);

    s32 engine  = scroll->unk_00;
    s32 bgLayer = scroll->unk_04;

    DisplayEngineState* temp_r5 = &g_DisplaySettings.engineState[engine];

    switch (temp_r5->bgSettings[bgLayer].bgMode) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            temp_r5->bgAffines[bgLayer].unk_14 = 1;
            break;
    }

    temp_r5->bgOffsets[bgLayer].hOffset = hOffset;
    temp_r5->bgOffsets[bgLayer].vOffset = vOffset;
    return 1;
}

// Nonmatching: Incorrect instructions for DisplayEngineState access
s32 func_ov042_02083838(BgScroll* scroll) {
    s32 var_r4 = 1;

    if (scroll->unk_24 == 0) {
        scroll->unk_26 = scroll->unk_10;
        scroll->unk_14 = round(scroll->unk_08);
        scroll->unk_18 = round(scroll->unk_0A);

        f32 var_r0_3   = round(scroll->unk_10);
        f32 var_r0_4   = round(scroll->unk_0C);
        scroll->unk_1C = FX_Divide((s32)var_r0_4 - scroll->unk_14, (s32)var_r0_3);

        f32 var_r0_5   = round(scroll->unk_10);
        f32 var_r0_6   = round(scroll->unk_0E);
        scroll->unk_20 = FX_Divide((s32)var_r0_6 - scroll->unk_18, (s32)var_r0_5);
    }

    if (scroll->unk_24 >= scroll->unk_26) {
        scroll->unk_14 = round(scroll->unk_0C);
        scroll->unk_18 = round(scroll->unk_0E);
        var_r4         = 0;
    } else {
        scroll->unk_14 += scroll->unk_1C;
        scroll->unk_18 += scroll->unk_20;
    }

    s32 bgLayer = scroll->unk_04;
    s32 vOffset = scroll->unk_18;
    s32 hOffset = scroll->unk_14;

    DisplayEngineState* temp_lr = &g_DisplaySettings.engineState[scroll->unk_00];

    switch (temp_lr->bgSettings[bgLayer].bgMode) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            temp_lr->bgAffines[bgLayer].unk_14 = 1;
            break;
    }

    temp_lr->bgOffsets[bgLayer].hOffset = hOffset;
    temp_lr->bgOffsets[bgLayer].vOffset = vOffset;

    scroll->unk_24++;
    return var_r4;
}

const TaskHandle data_ov042_0208481c = {"Tsk_FontRoll", func_ov042_02084064, 0xEB4};

s32 FontRoll_CreateTask(TaskPool* taskPool, s32 arg1, s32 arg2, s32 arg3, u16 arg4, s16 arg5) {
    FontRollArgs args;
    args.unk_00 = arg1;
    args.unk_04 = arg2;
    args.unk_08 = arg3;
    args.unk_0C = arg4;
    args.unk_0E = arg5;
    return EasyTask_CreateTask(taskPool, &data_ov042_0208481c, NULL, 0, NULL, &args);
}

u8* func_ov042_02083b48(FontRoll* fontRoll, s32 arg1, s32 arg2) {
    u8* temp_r1 = fontRoll->unk_EA8->buffer;
    if (arg1 == 0) {
        return temp_r1 + (arg2 * 2);
    }
    return temp_r1 + ((arg2 + 0x6A) * 2);
}

struct unk2084924 {
    s16 unk_00;
    s16 unk_02;
} data_ov042_02084924[2];

s32 func_ov042_02083b64(FontRoll* fontRoll) {
    s32 var_r5 = 0xC0;

    struct unk2084924* temp_r4 = &data_ov042_02084924[fontRoll->unk_00C];

    u8* temp_r0 = func_ov042_02083b48(fontRoll, fontRoll->unk_00C, 0);

    for (s32 i = 0; i < temp_r4->unk_02; i++) {
        var_r5 += temp_r0[i] + 0xA;
    }
    return var_r5;
}

void func_ov042_02083bb8(FontRoll* fontRoll) {
    CommandUnk04 commandUnk;
    commandUnk.unk_00 = fontRoll->engine;
    commandUnk.unk_02 = fontRoll->bgLayer;
    func_ov042_020827ec(fontRoll->unk_000, 6, &commandUnk);
}

// Nonmatching: Regswaps
void func_ov042_02083be8(FontRoll* fontRoll) {
    fontRoll->unk_EA0 += fontRoll->unk_EA4;
    if (fontRoll->unk_EA0 >= 0x100000) {
        fontRoll->unk_EA0 -= 0x100000;
    }

    s32 engine  = fontRoll->engine;
    s32 bgLayer = fontRoll->bgLayer;

    DisplayEngineState* engineState = &g_DisplaySettings.engineState[engine];

    s32 vOffset = fontRoll->unk_EA0;

    switch (engineState->bgSettings[bgLayer].bgMode) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            engineState->bgAffines[bgLayer].unk_14 = 1;
            break;
    }

    engineState->bgOffsets[bgLayer].hOffset = 0;
    engineState->bgOffsets[bgLayer].vOffset = vOffset;
}

s32 func_ov042_02083c78(FontRoll* fontRoll) {
    if (fontRoll->unk_E98 == -1) {
        return 0;
    }
    UnkOv31Struct* temp_r4 = &fontRoll->unk_010[fontRoll->unk_E98];
    if (func_ov042_02082fc4(temp_r4) == 0) {
        return 0;
    }

    u8  temp_r4_2 = temp_r4->unk_70 - ((s32)fontRoll->unk_EA0 >> 0xC);
    s32 temp_r1   = 0x100 - func_ov031_0210c5b4(temp_r4);
    if (((s32)temp_r4_2 >= (s32)(temp_r1 - 0x10)) && ((s32)temp_r4_2 <= temp_r1)) {
        return 1;
    }
    return 0;
}

// Nonmatching: Regswaps, minor instruction reordering
void func_ov042_02083cf0(FontRoll* fontRoll) {
    s32 temp_r3 = fontRoll->unk_E98;
    if (temp_r3 == -1) {
        return;
    }

    StaffRollUnkA* temp_r4 = &fontRoll->unk_000->unk_10[fontRoll->engine].group1[fontRoll->bgLayer];
    StaffRollUnkB* temp_r9 = &fontRoll->unk_000->unk_10[fontRoll->engine].group2[fontRoll->bgLayer];

    if (func_ov042_02082fc4(&fontRoll->unk_010[temp_r3]) == 0) {
        return;
    }
    func_ov031_0210ab34(&fontRoll->unk_010[temp_r3], 8);
    func_ov042_02082fd8(&fontRoll->unk_010[temp_r3], 8);

    Data* grp2data = temp_r9->data;
    u8*   var_r2;
    if (grp2data == NULL) {
        var_r2 = NULL;
    } else {
        var_r2 = (u8*)grp2data->buffer;
        var_r2 += 0x20;
        var_r2 += ((u32*)var_r2)[2];
    }

    Data* grp1data = temp_r4->data;
    u8*   var_r3;
    if (grp1data == NULL) {
        var_r3 = NULL;
    } else {
        var_r3 = (u8*)grp1data->buffer;
        var_r3 += 0x20;
        var_r3 += ((u32*)var_r3)[2];
    }

    func_ov031_0210be18(&fontRoll->unk_010[temp_r3], var_r2 + 4, var_r3 + 4, 0);
    func_ov031_0210aabc(&fontRoll->unk_010[temp_r3]);
    func_ov042_02083bb8(fontRoll);
    fontRoll->unk_E98 = (fontRoll->unk_E98 + 1) % 30;
}

// Nonmatching
s32 func_ov042_02083df8(FontRoll* fontRoll) {
    s32 temp_r5 = fontRoll->unk_E9C;
    if (temp_r5 == -1) {
        return 1;
    }
    if (fontRoll->unk_EAC >= data_ov042_02084924[fontRoll->unk_00C].unk_00) {
        return 0;
    }

    UnkOv31Struct* temp_r4 = &fontRoll->unk_010[temp_r5];
    s32            temp    = temp_r4->unk_70;

    u8 var_r3 = ((s32)fontRoll->unk_EA0 >> 0xC) + 0xC0;
    if (var_r3 < temp) {
        var_r3 += 0x100;
    }

    u8* temp_r5_3 = func_ov042_02083b48(fontRoll, fontRoll->unk_00C, fontRoll->unk_EAC);
    if ((s32)(var_r3 - temp) >= func_ov031_0210c5b4(temp_r4) + *temp_r5_3) {
        return 1;
    }
    return 0;
}

// Nonmatching
void func_ov042_02083e8c(FontRoll* fontRoll) {
    s32 var_r4 = (fontRoll->unk_E9C == -1) ? 0 : (fontRoll->unk_E9C + 1) % 30;

    struct unk2084924* temp_r8 = &data_ov042_02084924[fontRoll->unk_00C];
    u8*                sp0     = func_ov042_02083b48(fontRoll, fontRoll->unk_00C, fontRoll->unk_EAC);

    if (fontRoll->unk_E98 == -1) {
        fontRoll->unk_E98 = var_r4;
    }
    fontRoll->unk_E9C = var_r4;

    UnkOv31Struct* temp_r7 = fontRoll->unk_010;
    u32            temp_r9 = ((s32)fontRoll->unk_EA0 >> 0xC) + 0xC0;

    StaffRollUnkA* temp_r11 = &fontRoll->unk_000->unk_10[fontRoll->engine].group1[fontRoll->bgLayer];
    StaffRollUnkB* temp_r4  = &fontRoll->unk_000->unk_10[fontRoll->engine].group2[fontRoll->bgLayer];

    s32 temp_r5 = fontRoll->bgLayer;

    func_ov031_0210aa94(&temp_r7[var_r4]);
    func_ov031_0210b630(&temp_r7[var_r4], &temp_r8[fontRoll->unk_EAC]);
    func_ov031_0210ab34(&temp_r7[var_r4], 0xE);

    switch (sp0[1]) {
        case 0:
            func_ov031_0210ab3c(&temp_r7[var_r4], 1, 0x9C);
            break;
        case 1:
            func_ov031_0210ab3c(&temp_r7[var_r4], 0, 0x9C);
            break;
        case 2:
            func_ov031_0210ab3c(&temp_r7[var_r4], 2, 0x9C);
            break;
    }

    func_ov031_0210ab54(&temp_r7[var_r4], 1, 0);
    func_ov031_0210ab28(&temp_r7[var_r4], 0x64, (u8)temp_r9);

    Data* temp_r0_3 = temp_r4->data;
    u8*   var_r2;
    if (temp_r0_3 == NULL) {
        var_r2 = NULL;
    } else {
        var_r2 = (u8*)temp_r0_3->buffer;
        var_r2 += 0x20;
        var_r2 += ((u32*)var_r2)[2];
    }

    Data* temp_r0_4 = temp_r11->data;
    u8*   var_r3;
    if (temp_r0_4 == NULL) {
        var_r3 = NULL;
    } else {
        var_r3 = (u8*)temp_r0_4->buffer;
        var_r3 += 0x20;
        var_r3 += ((u32*)var_r3)[2];
    }

    func_ov031_0210be18(&temp_r7[var_r4], var_r2 + 4, var_r3 + 4, 0);
    func_ov042_02083bb8(fontRoll);
    fontRoll->unk_EAC++;
}

s32 func_ov042_02084064(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    FontRoll* fontRoll = (FontRoll*)task->data;

    switch (arg3) {
        case 0:
            return func_ov042_020840ac(fontRoll, arg2);
        case 1:
            return func_ov042_020841cc(fontRoll);
        case 3:
            return func_ov042_0208429c(fontRoll);
        default:
            return 1;
    }
}

const BinIdentifier data_ov042_02084814 = {42, "Apl_Suy/staff_font.bin"};

s32 func_ov042_020840ac(FontRoll* fontRoll, FontRollArgs* args) {
    MI_CpuSet(fontRoll, 0, sizeof(FontRoll));
    fontRoll->unk_000 = args->unk_00;
    fontRoll->engine  = args->unk_04;
    fontRoll->bgLayer = args->unk_08;
    fontRoll->unk_00C = args->unk_0C;
    fontRoll->unk_00E = args->unk_0E;
    fontRoll->unk_EA8 = DatMgr_LoadRawData(1, NULL, 0, &data_ov042_02084814);
    fontRoll->unk_EA0 = 0x20000;

    f32 denom = round(args->unk_0E);

    f32 numer = round(func_ov042_02083b64(fontRoll));

    fontRoll->unk_EA4 = FX_Divide(numer, denom);
    fontRoll->unk_EB0 = fontRoll->unk_00E;
    fontRoll->unk_E98 = -1;
    fontRoll->unk_E9C = -1;
    return 1;
}

// Nonmatching: Instruction differences in Data access
s32 func_ov042_020841cc(FontRoll* fontRoll) {
    s32 engine  = fontRoll->engine;
    s32 bgLayer = fontRoll->bgLayer;

    Data* grp1data = fontRoll->unk_000->unk_10[engine].group1[bgLayer].data;
    Data* grp2data = fontRoll->unk_000->unk_10[engine].group2[bgLayer].data;

    if (grp1data == NULL || grp2data == NULL) {
        return FALSE;
    }

    if (func_ov042_02083df8(fontRoll) == 1) {
        func_ov042_02083e8c(fontRoll);
        fontRoll->unk_000->unk_100_2 = 1;
    }

    if ((fontRoll->unk_000->unk_100_2 == 0) && (func_ov042_02083c78(fontRoll) == 1)) {
        func_ov042_02083cf0(fontRoll);
        fontRoll->unk_000->unk_100_2 = 1;
    }
    func_ov042_02083be8(fontRoll);

    if (fontRoll->unk_EAE >= fontRoll->unk_EB0) {
        return 0;
    }
    fontRoll->unk_EAE++;
    return 1;
}

s32 func_ov042_0208429c(FontRoll* fontRoll) {
    DatMgr_ReleaseData(fontRoll->unk_EA8);

    for (s32 i = 0; i < 30; i++) {
        if (func_ov042_02082fc4(&fontRoll->unk_010[i]) != 0) {
            func_ov031_0210aabc(&fontRoll->unk_010[i]);
        }
    }
    return 1;
}

void func_ov042_020842e4(void) {
    Interrupts_Init();
    func_0200434c();
    do {
    } while (REG_VCOUNT < (s16)0xC0);
    GX_Init();
    DMA_Init(0x100);
    Display_Init();
    g_DisplaySettings.controls[0].brightness = -16;
    g_DisplaySettings.controls[1].brightness = -16;
    Display_Commit();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(GX_VRAM_ALL);
    GX_SetBankForBg(GX_VRAM_A);
    GX_SetBankForSubBg(GX_VRAM_C);
    g_DisplaySettings.unk_000 = 0;
    *(u16*)0x04000304 &= ~0x8000;
    Display_CommitSynced();
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    DisplayBGSettings* mainBg0 = &g_DisplaySettings.engineState[0].bgSettings[0];
    mainBg0->bgMode            = DISPLAY_BGMODE_TEXT;
    mainBg0->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    mainBg0->colorMode         = GX_BG_COLORS_16;
    mainBg0->extPlttSlot       = 0;
    mainBg0->screenBase        = 3;
    mainBg0->charBase          = 6;
    if (g_DisplaySettings.controls[0].dimension == GX2D3D_MODE_2D) {
        REG_BG0CNT = (REG_BG0CNT & 0x43) | 0x318;
    }

    DisplayBGSettings* mainBg1 = &g_DisplaySettings.engineState[0].bgSettings[1];
    mainBg1->bgMode            = DISPLAY_BGMODE_TEXT;
    mainBg1->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    mainBg1->colorMode         = GX_BG_COLORS_16;
    mainBg1->screenBase        = 1;
    mainBg1->charBase          = 2;
    mainBg1->extPlttSlot       = 0;
    REG_BG1CNT                 = (REG_BG1CNT & 0x43) | 0x108;

    DisplayBGSettings* mainBg2 = &g_DisplaySettings.engineState[0].bgSettings[2];
    mainBg2->bgMode            = DISPLAY_BGMODE_TEXT;
    mainBg2->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    mainBg2->colorMode         = GX_BG_COLORS_256;
    mainBg2->screenBase        = 2;
    mainBg2->extPlttSlot       = 1;
    mainBg2->charBase          = 4;
    REG_BG2CNT                 = (REG_BG2CNT & 0x43) | 0x290;

    DisplayBGSettings* mainBg3 = &g_DisplaySettings.engineState[0].bgSettings[3];
    mainBg3->bgMode            = DISPLAY_BGMODE_TEXT;
    mainBg3->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    mainBg3->colorMode         = GX_BG_COLORS_256;
    mainBg3->screenBase        = 3;
    mainBg3->charBase          = 6;
    mainBg3->extPlttSlot       = 1;
    REG_BG3CNT                 = (REG_BG3CNT & 0x43) | 0x398;

    g_DisplaySettings.controls[0].layers = 0;

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[1].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    DisplayBGSettings* subBg0 = &g_DisplaySettings.engineState[1].bgSettings[0];
    subBg0->bgMode            = DISPLAY_BGMODE_TEXT;
    subBg0->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    subBg0->colorMode         = GX_BG_COLORS_16;
    subBg0->screenBase        = 3;
    subBg0->charBase          = 6;
    subBg0->extPlttSlot       = 0;
    REG_BG0CNT_SUB            = (REG_BG0CNT_SUB & 0x43) | 0x318;

    DisplayBGSettings* subBg1 = &g_DisplaySettings.engineState[1].bgSettings[1];
    subBg1->bgMode            = DISPLAY_BGMODE_TEXT;
    subBg1->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    subBg1->colorMode         = GX_BG_COLORS_16;
    subBg1->screenBase        = 1;
    subBg1->charBase          = 2;
    subBg1->extPlttSlot       = 0;
    REG_BG1CNT_SUB            = (REG_BG1CNT_SUB & 0x43) | 0x108;

    DisplayBGSettings* subBg2 = &g_DisplaySettings.engineState[1].bgSettings[2];
    subBg2->bgMode            = DISPLAY_BGMODE_TEXT;
    subBg2->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    subBg2->colorMode         = GX_BG_COLORS_256;
    subBg2->screenBase        = 2;
    subBg2->charBase          = 4;
    subBg2->extPlttSlot       = 1;
    REG_BG2CNT_SUB            = (REG_BG2CNT_SUB & 0x43) | 0x290;

    DisplayBGSettings* subBg3 = &g_DisplaySettings.engineState[1].bgSettings[3];
    subBg3->bgMode            = DISPLAY_BGMODE_TEXT;
    subBg3->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    subBg3->colorMode         = GX_BG_COLORS_256;
    subBg3->screenBase        = 3;
    subBg3->charBase          = 6;
    subBg3->extPlttSlot       = 1;
    REG_BG3CNT_SUB            = (REG_BG3CNT_SUB & 0x43) | 0x398;

    g_DisplaySettings.controls[1].layers = 0;

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    MI_CpuFill(0, 0x06800000, 0xA4000);
    MI_CpuFill(0, 0x06000000, 0x80000);
    MI_CpuFill(0, 0x06200000, 0x20000);
    func_0200270c(0, 0);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200270c(0, 1);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    func_02001c34(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    func_02001c34(&data_02066eec, &data_0205a128, 0, 0x200, 1);
    Interrupts_RegisterVBlankCallback(func_ov042_020846e4, 1);
}

void func_ov042_020846d0(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}

void func_ov042_020846e4(void) {
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
    }
}