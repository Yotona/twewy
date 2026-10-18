// USA 0x020be85c-0x020bee24: the scan "fukidasi" (speech-bubble) manager. It
// keeps a table of hit-test entries (each backed by a scan marker task) and a
// message-window task that follows the currently selected entry. The manager
// pointer lives in data_ov030_02109d70.
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Ov030Data.h"
#include "SndMgr.h"

// Follow-window spawn block: the window's screen position, then its params.
typedef struct {
    /* 0x00 */ s16             x;
    /* 0x02 */ s16             y;
    /* 0x04 */ FldMesTaskParam msg;
} FldScanWinParam; // Size: 0x2C

// A scannable spot (stride 0x18, count at FldScanData.unk600).
typedef struct {
    /* 0x00 */ s32          unk0; // backing task id
    /* 0x04 */ FldScreenPos pos;
    /* 0x08 */ s32          unk8;
    /* 0x0C */ s32          unkC;  // message number, or 0x10000 + Noise encounter
    /* 0x10 */ s32          unk10;
    /* 0x14 */ Task*        human; // NPC the spot belongs to
} FldScanEntry;                    // Size: 0x18

// Scan task state: 64 entries followed by the counters. Size is 0x614, which
// the TaskHandle below depends on.
typedef struct {
    /* 0x000 */ FldScanEntry entries[64];
    /* 0x600 */ s32          unk600; // live entry count
    /* 0x604 */ s32          unk604;
    /* 0x608 */ s32          unk608; // spawned window task id
    /* 0x60C */ s32          unk60C;
    /* 0x610 */ s32          unk610; // current selection
} FldScanData;                       // Size: 0x614

extern const char       data_ov030_0210740c[];
extern const TaskHandle Task_Window;
extern s32              data_ov030_02109d64;
extern FldScanData*     data_ov030_02109d70;

extern s32  FieldScan_IsTouchActive(void);
extern void FieldScan_MarkSeen(s32 id);
extern void func_ov030_020af6d4(s32 mode);
extern void func_ov030_020b400c(Task* task, s32 mode);
extern s32  func_ov030_020b40e8(Task* task);
extern void func_ov030_020b4144(Task* task);
extern void func_ov030_020b416c(Task* task);
extern void func_ov030_020b5d08(Task* task, s32 arg1);
extern s32  func_ov030_020b5d58(Task* task);
extern s32  func_ov030_020bf050(TaskPool* pool, s32 arg1, s32 arg2);
extern s32  func_ov030_020c2640(TaskPool* pool, s32 dataType, FldScreenPos pos);
extern s32  func_ov030_020c1088(TaskPool* pool, s16 x, s16 y, s16 kind, s16 mode, s32 mask);
extern u8   func_ov030_020ca778(void* font, s32 mesNo, s32 arg2, const u8* winKind);

s32  func_ov030_020be85c(TaskPool* pool, Task* task, void* args);
s32  func_ov030_020be884(FldScanEntry* entry, TouchCoord* coord);
void func_ov030_020be8d8(TaskPool* pool, FldScanData* data, s32 sel);
s32  func_ov030_020be958(TaskPool* pool, Task* task, void* args);
s32  func_ov030_020beac4(TaskPool* pool, Task* task, void* args);
s32  func_ov030_020beacc(TaskPool* pool, Task* task, void* args);
s32  func_ov030_020beb58(TaskPool* pool, Task* task, void* args, s32 stage);
void func_ov030_020beba0(TaskPool* pool, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, Task* arg7);
void func_ov030_020bed54(TaskPool* pool, FldScanData* data, FldScanEntry* entry);

const TaskHandle data_ov030_020e6360 = {data_ov030_0210740c, func_ov030_020beb58, sizeof(FldScanData)};

const TaskStages data_ov030_020e636c = {
    {func_ov030_020be85c, func_ov030_020be958, func_ov030_020beac4, func_ov030_020beacc}
};

s32 func_ov030_020be85c(TaskPool* pool, Task* task, void* args) {
    FldScanData* data = task->data;

    data_ov030_02109d70 = data;
    data->unk600        = 0;
    data->unk610        = -1;
    return 1;
}

s32 func_ov030_020be884(FldScanEntry* entry, TouchCoord* coord) {
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

void func_ov030_020be8d8(TaskPool* pool, FldScanData* data, s32 sel) {
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
            func_ov030_020b400c(EasyTask_GetTaskById(pool, data->entries[i].unk0), a);
        }
        func_ov030_020b5d08(data->entries[i].human, b);
    }
}

// Nonmatching: loop-exit branch and epilogue placement (USA 95.55%; JP unvalidated).
// Retail keeps a separate early-return epilogue for the aee10()==0 path
// (`b 0x214` + add sp/#8/mov r0,#1/ldmia laid out between the loop and the
// post-loop tail); ours shares a single epilogue with the final return.
s32 func_ov030_020be958(TaskPool* pool, Task* task, void* args) {
    FldScanData* data = task->data;
    TouchCoord   coord;
    s32          i;

    if (FieldScan_IsTouchActive() != 0) {
        TouchInput_GetCoord(&coord);
        for (i = 0; i < data->unk600; i++) {
            if (func_ov030_020be884(&data->entries[i], &coord) != 0) {
                Task* t;

                func_ov030_020be8d8(pool, data, i);
                t = EasyTask_GetTaskById(pool, data->entries[i].unk0);
                if (data->entries[i].unkC < 0x10000) {
                    func_ov030_020b416c(t);
                }
                if (i != data->unk610) {
                    if (data->unk610 >= 0) {
                        EasyTask_DeleteTask(pool, data->unk608);
                        EasyTask_DeleteTask(pool, data->unk60C);
                    }
                    SndMgr_StartPlayingSE(0xC1);
                    func_ov030_020bed54(pool, data, &data->entries[i]);
                    data->unk60C = func_ov030_020bf050(pool, data->unk604, data->entries[i].unk10);
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

                if (func_ov030_020b40e8(t) == 0) {
                    func_ov030_020b4144(t);
                }
            }
        }
    }
    return 1;
}

s32 func_ov030_020beac4(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov030_020beacc(TaskPool* pool, Task* task, void* args) {
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
    data_ov030_02109d70 = NULL;
    return 1;
}

s32 func_ov030_020beb58(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov030_020e636c;

    return stages.iter[stage](pool, task, args);
}

// Nonmatching: register allocation in the window-param block (USA 90.50%; JP unvalidated).
// Retail batches the unk4/unk6 reloads into r8/r9 and orders datSlot before mesNo;
// ours swaps r8/r9 and reorders the store pair
void func_ov030_020beba0(TaskPool* pool, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, Task* arg7) {
    FldScanData*    data;
    FldScanEntry*   entry;
    FldMesTaskParam param;
    s32             x;
    s32             y;

    param.unk_14 = 0;
    x            = arg2;
    y            = arg3;
    if (data_ov030_02109d70 == NULL) {
        return;
    }
    data         = data_ov030_02109d70;
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
    entry->human = arg7;
    if (arg4 >= 0x10000) {
        entry->unk0 = func_ov030_020c2640(pool, data->unk604, entry->pos);
        func_ov030_020c1088(pool, (s16)entry->pos.x, (s16)entry->pos.y, (s16)(arg4 - 0x10000), 1, func_ov030_020b5d58(arg7));
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
        func_ov030_020b400c(EasyTask_GetTaskById(pool, id), 1);
    }
    SndMgr_PlaySEWithPan(0xC0, x);
}

// Nonmatching: constant materialisation order (USA 98.50%; JP unvalidated).
// Retail derives -1 with `sub r1,r2,#1` after `mov r2,#0` (the zero constant
// register is allocated before 0x80/0x8c, so the -1 peephole fires); ours emits
// `mvn r1,#0` because our zero is materialised last.
void func_ov030_020bed54(TaskPool* pool, FldScanData* data, FldScanEntry* entry) {
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
