#include "Field/Tasks/Fld_ScanFukidasi.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Battle/FieldBattleTransition.h"
#include "Field/FieldTypes.h"
#include "Field/Scene/FieldSceneRender.h"
#include "Field/Scene/FieldSeq.h"
#include "Field/Tasks/FldMesWindow.h"
#include "Field/Tasks/Fld_Luck.h"
#include "Field/Tasks/Fld_NpcRG.h"
#include "Field/Tasks/Fld_ScanMoya.h"
#include "Field/Tasks/Task_Window.h"
#include "SndMgr.h"

// Follow-window spawn block: the window's screen position, then its params.
typedef struct {
    /* 0x00 */ s16             x;
    /* 0x02 */ s16             y;
    /* 0x04 */ FldMesTaskParam msg;
} FldScanWinParam; // Size: 0x2C

// A scannable spot (count at FldScanData.unk600).
typedef struct {
    /* 0x00 */ s32          unk0; // backing task id
    /* 0x04 */ FldScreenPos pos;
    /* 0x08 */ s32          unk8;
    /* 0x0C */ s32          unkC;  // message number, or 0x10000 + Noise encounter
    /* 0x10 */ s32          unk10;
    /* 0x14 */ Task*        human; // NPC the spot belongs to
} FldScanEntry;                    // Size: 0x18

// Scan task state: 64 entries followed by the counters.
typedef struct {
    /* 0x000 */ FldScanEntry entries[64];
    /* 0x600 */ s32          unk600; // live entry count
    /* 0x604 */ s32          unk604;
    /* 0x608 */ s32          unk608; // spawned window task id
    /* 0x60C */ s32          unk60C;
    /* 0x610 */ s32          unk610; // current selection
} FldScanData;                       // Size: 0x614

extern const TaskHandle Task_Window;
static FldScanData*     Fld_ScanFukidasi_Instance = NULL;
static s32              Fld_ScanFukidasi_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static void             Fld_ScanFukidasi_OpenWindow(TaskPool* pool, FldScanData* data, FldScanEntry* entry);

const TaskHandle Tsk_Fld_ScanFukidasi = {"Tsk_Fld_ScanFukidasi", Fld_ScanFukidasi_RunTask, sizeof(FldScanData)};

static s32 Fld_ScanFukidasi_Init(TaskPool* pool, Task* task, void* args) {
    FldScanData* data = task->data;

    Fld_ScanFukidasi_Instance = data;
    data->unk600              = 0;
    data->unk610              = -1;
    return 1;
}

static s32 Fld_ScanFukidasi_HitTest(FldScanEntry* entry, TouchCoord* coord) {
    s32 x = (s16)entry->pos.x;
    s32 y = (s16)entry->pos.y;

    if (coord->x < x - 0x1E) {
        return 0;
    }
    if (coord->x > x + 0x1E) {
        return 0;
    }
    if (coord->y < y - 0xA) {
        return 0;
    }
    if (coord->y <= y + 0xA) {
        return 1;
    }
    return 0;
}

static void Fld_ScanFukidasi_Select(TaskPool* pool, FldScanData* data, s32 sel) {
    s32 i;
    s32 a;
    s32 b;

    for (i = 0; i < data->unk600; i++) {
        if (i != sel) {
            a = 1;
            b = 0;
        } else {
            a = 0;
            b = 1;
        }
        if (data->entries[i].unkC < 0x10000) {
            FldMesWindow_SetPriority(EasyTask_GetTaskById(pool, data->entries[i].unk0), a);
        }
        func_ov030_020b5d08(data->entries[i].human, b);
    }
}

// Nonmatching: loop-exit branch and epilogue placement (USA 95.55%; JP unvalidated).
// Retail keeps a separate early-return epilogue for the aee10()==0 path
// (`b 0x214` + add sp/#8/mov r0,#1/ldmia laid out between the loop and the
// post-loop tail); ours shares a single epilogue with the final return.
static s32 Fld_ScanFukidasi_Update(TaskPool* pool, Task* task, void* args) {
    FldScanData* data = task->data;
    TouchCoord   coord;
    s32          i;

    if (FieldScan_IsTouchActive() != 0) {
        TouchInput_GetCoord(&coord);
        for (i = 0; i < data->unk600; i++) {
            if (Fld_ScanFukidasi_HitTest(&data->entries[i], &coord) != 0) {
                Task* t;

                Fld_ScanFukidasi_Select(pool, data, i);
                t = EasyTask_GetTaskById(pool, data->entries[i].unk0);
                if (data->entries[i].unkC < 0x10000) {
                    FldMesWindow_Press(t);
                }
                if (i != data->unk610) {
                    if (data->unk610 >= 0) {
                        EasyTask_DeleteTask(pool, data->unk608);
                        EasyTask_DeleteTask(pool, data->unk60C);
                    }
                    SndMgr_StartPlayingSE(0xC1);
                    Fld_ScanFukidasi_OpenWindow(pool, data, &data->entries[i]);
                    data->unk60C = Fld_NpcRG_CreateTask(pool, data->unk604, data->entries[i].unk10);
                    data->unk610 = i;
                    func_ov030_020af6d4(5);
                }
                break;
            }
        }
        if (data->entries[i].unkC < 0x10000) {
            s32 sel = data->unk610;

            if (sel >= 0) {
                Task* t = EasyTask_GetTaskById(pool, data->entries[sel].unk0);

                if (FldMesWindow_IsAnimating(t) == 0) {
                    FldMesWindow_Highlight(t);
                }
            }
        }
    }
    return 1;
}

static s32 Fld_ScanFukidasi_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Fld_ScanFukidasi_Destroy(TaskPool* pool, Task* task, void* args) {
    FldScanData* data = task->data;
    s32          i;

    for (i = 0; i < data->unk600; i++) {
        EasyTask_DeleteTask(pool, data->entries[i].unk0);
        func_ov030_020b5d08(data->entries[i].human, 0);
    }
    if (data->unk610 >= 0) {
        EasyTask_DeleteTask(pool, data->unk608);
        EasyTask_DeleteTask(pool, data->unk60C);
    }
    Fld_ScanFukidasi_Instance = NULL;
    return 1;
}

static s32 Fld_ScanFukidasi_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_ScanFukidasi_Init,
        .update     = Fld_ScanFukidasi_Update,
        .render     = Fld_ScanFukidasi_Render,
        .cleanup    = Fld_ScanFukidasi_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

// Nonmatching: register allocation in the window-param block (USA 90.50%; JP unvalidated).
// Retail batches the unk4/unk6 reloads into r8/r9 and orders datSlot before mesNo;
// ours swaps r8/r9 and reorders the store pair
void Fld_ScanFukidasi_AddSpot(TaskPool* pool, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, Task* task) {
    FldScanData*    data;
    FldScanEntry*   entry;
    FldMesTaskParam param;
    s32             x;
    s32             y;

    param.unk_14 = 0;
    x            = arg2;
    y            = arg3;
    if (Fld_ScanFukidasi_Instance == NULL) {
        return;
    }
    data         = Fld_ScanFukidasi_Instance;
    entry        = &data->entries[data->unk600];
    data->unk600 = data->unk600 + 1;
    if (x < 0x20) {
        x = 0x20;
    }
    if (x > 0xE0) {
        x = 0xE0;
    }
    if (y < 0x10) {
        y = 0x10;
    }
    data->unk604 = arg1;
    if (y > 0xB0) {
        y = 0xB0;
    }
    entry->pos.x = x;
    entry->pos.y = y;
    entry->unk8  = arg5;
    entry->unkC  = arg4;
    entry->unk10 = arg6;
    entry->human = task;
    if (arg4 >= 0x10000) {
        entry->unk0 = Fld_ScanMoya_CreateTask(pool, data->unk604, entry->pos);
        Fld_Luck_AddSymbol(pool, (s16)entry->pos.x, (s16)entry->pos.y, (s16)(arg4 - 0x10000), 1, func_ov030_020b5d58(task));
    } else {
        s32 id;
        u16 px;
        u16 py;

        param.winKind   = 4;
        param.datSlot   = arg1;
        param.mesNo     = arg4;
        param.unk_04.sx = entry->pos.x;
        param.unk_04.sy = entry->pos.y;
        px              = entry->pos.x;
        py              = entry->pos.y;
        param.unk_08.sx = px;
        param.unk_08.sy = py;
        param.unk_1A    = 0;
        param.unk_0C    = -1;
        param.unk_16    = 8;
        param.unk_1B    = 0;
        param.unk_19    = func_ov030_020ca778((void*)data_ov030_02109d64, arg4, 0, &param.winKind);
        param.unk_24    = 0;
        param.unk_20    = 0;
        param.unk_1C    = 0;
        id              = EasyTask_CreateTask(pool, &Task_Window, NULL, 0, NULL, &param);
        entry->unk0     = id;
        FldMesWindow_SetPriority(EasyTask_GetTaskById(pool, id), 1);
    }
    SndMgr_PlaySEWithPan(0xC0, x);
}

// Nonmatching: constant materialisation order (USA 98.50%; JP unvalidated).
// Retail derives -1 with `sub r1,r2,#1` after `mov r2,#0` (the zero constant
// register is allocated before 0x80/0x8c, so the -1 peephole fires); ours emits
// `mvn r1,#0` because our zero is materialised last.
static void Fld_ScanFukidasi_OpenWindow(TaskPool* pool, FldScanData* data, FldScanEntry* entry) {
    FldScanWinParam param;

    param.msg.unk_14  = 0;
    param.msg.datSlot = data->unk604;
    param.msg.mesNo   = entry->unk8;
    param.msg.winKind = 6;
    FieldScan_MarkSeen(entry->unk8);
    param.msg.unk_0C    = -1;
    param.msg.unk_16    = 8;
    param.x             = 0x80;
    param.y             = 0x8C;
    param.msg.unk_04.sx = 0x80;
    param.msg.unk_04.sy = 0x8C;
    param.msg.unk_08.sx = 0x80;
    param.msg.unk_08.sy = 0x8C;
    param.msg.unk_1A    = 0;
    param.msg.unk_1B    = 0;
    param.msg.unk_20    = 0;
    param.msg.unk_19    = func_ov030_020ca778((void*)data_ov030_02109d64, param.msg.mesNo, 0, &param.msg.winKind);
    param.msg.unk_24    = 1;
    param.msg.unk_1C    = 0;
    data->unk608        = EasyTask_CreateTask(pool, &Task_Window, NULL, 0, NULL, &param.msg);
}
