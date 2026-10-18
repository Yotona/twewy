#include "Engine/EasyTask.h"
#include "Field/NSymbol.h"
#include "Save.h"

// USA 0x020c08ec-0x020c0bdc: the manager task that spawns and retires the scan
// Noise symbols. Its task data (NsymUGMgr) holds up to 0x80 symbol task ids
// followed by the chain-battle bookkeeping.

typedef TaskStages NsymUGMgrFns;

extern TaskHandle   data_ov030_020e6f98;
extern NsymUGMgrFns data_ov030_020e6fa4;

extern u32  func_ov030_020c2458(TaskPool* pool, s32 encounter, s32 counter);
extern s32  Field_GetMaxChainedEncounters(void);
extern void FieldScan_AddTime(s32 frames);
extern s32  FieldScan_IsClosing(void);
extern s32  FieldScan_IsBattleStarting(void);
extern s32  FieldScan_IsEnteringBattle(void);

s32 func_ov030_020c08ec(TaskPool* pool, Task* task, s32* args) {
    NsymUGMgr* data = task->data;

    data_ov030_02109d78   = data;
    data->dataType        = *args;
    data->dropRate        = gSaveData.playerStats.dropRate;
    data->chainCount      = 1;
    data->chainTaskId     = -1;
    gSaveData.battleCount = 1;
    data->count           = 0;
    data->unk_218         = 0;
    data->luckTaskId      = func_ov030_020c2010(pool, data->dataType, data->dropRate);
    return 1;
}

s32 func_ov030_020c0958(TaskPool* pool, Task* task) {
    NsymUGMgr* data = task->data;
    s32        i;
    s32        limit;
    s32        scaled;

    if (FieldScan_IsClosing() != 0) {
        return 1;
    }
    i = 0;
    if (data->count > 0) {
        do {
            if (data->ids[i] != -1 && func_ov030_020bbc38(pool, data->ids[i]) != 0) {
                if (gSaveData.battleCount < Field_GetMaxChainedEncounters() && FieldScan_IsBattleStarting() == 0) {
                    if (FieldScan_IsEnteringBattle() == 0) {
                        if (data->unk_218 == 0) {
                            func_ov030_020bbdc4(pool, data->ids[i]);
                            data->unk_218 = data->unk_218 + 1;
                        }
                        break;
                    }
                    func_ov030_020bbdc4(pool, data->ids[i]);
                    EasyTask_DeleteTask(pool, data->ids[i]);
                    data->ids[i]                                      = -1;
                    gSaveData.battleEncounters[gSaveData.battleCount] = gSaveData.scanSymbols[i].encounter;
                    gSaveData.battleCount                             = gSaveData.battleCount + 1;
                    limit                                             = gSaveData.battleCount;
                    if (limit >= 0x10) {
                        limit = 0x10;
                    }
                    scaled         = gSaveData.playerStats.dropRate * limit;
                    data->dropRate = scaled;
                    if (scaled >= 0x270F) {
                        data->dropRate = 0x270F;
                    }
                    func_ov030_020c2070(pool, data->luckTaskId, data->dropRate);
                    FieldScan_AddTime(0x14);
                    data->chainCount = data->chainCount + 1;
                    if (data->chainTaskId != -1) {
                        EasyTask_DeleteTask(pool, data->chainTaskId);
                    }
                    data->chainTaskId = func_ov030_020c2458(pool, data->dataType, data->chainCount);
                } else {
                    break;
                }
            }
            i++;
        } while (i < data->count);
    }
    return 1;
}

s32 func_ov030_020c0adc(void) {
    return 1;
}

s32 func_ov030_020c0ae4(TaskPool* pool, Task* task) {
    NsymUGMgr* data = task->data;
    s32        i;

    i = 0;
    if (data->count > 0) {
        do {
            if (data->ids[i] != -1) {
                EasyTask_DeleteTask(pool, data->ids[i]);
            }
            i++;
        } while (i < data->count);
    }
    EasyTask_DeleteTask(pool, data->luckTaskId);
    if (data->chainTaskId != -1) {
        EasyTask_DeleteTask(pool, data->chainTaskId);
    }
    data_ov030_02109d78 = NULL;
    return 1;
}

s32 func_ov030_020c0b64(TaskPool* pool, Task* task, void* args, s32 stage) {
    NsymUGMgrFns funcs = data_ov030_020e6fa4;

    return funcs.iter[stage](pool, task, args);
}

s32 func_ov030_020c0bac(TaskPool* pool, s32 arg1) {
    s32 param;

    param = arg1;
    return EasyTask_CreateTask(pool, &data_ov030_020e6f98, NULL, 0, NULL, &param);
}
