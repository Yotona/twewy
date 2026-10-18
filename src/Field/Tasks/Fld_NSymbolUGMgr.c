#include "Field/Tasks/Fld_NSymbolUGMgr.h"
#include "Engine/EasyTask.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Battle/FieldBattleTransition.h"
#include "Field/Tasks/Fld_Chain.h"
#include "Field/Tasks/Fld_Luck.h"
#include "Field/Tasks/Fld_NSymbolUG.h"
#include "Save.h"
static s32 Fld_NSymbolUGMgr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

NsymUGMgr* Fld_NSymbolUGMgr_Instance;

static const TaskHandle Tsk_Fld_NSymbolUGMgr = {"Tsk_Fld_NSymbolUGMgr", Fld_NSymbolUGMgr_RunTask, sizeof(NsymUGMgr)};

static s32 Fld_NSymbolUGMgr_Init(TaskPool* pool, Task* task, void* args) {
    s32*       params = args;
    NsymUGMgr* data   = task->data;

    Fld_NSymbolUGMgr_Instance = data;
    data->dataType            = *params;
    data->dropRate            = gSaveData.playerStats.dropRate;
    data->chainCount          = 1;
    data->chainTaskId         = -1;
    gSaveData.battleCount     = 1;
    data->count               = 0;
    data->unk_218             = 0;
    data->luckTaskId          = Fld_Luck_CreateTask(pool, data->dataType, data->dropRate);
    return 1;
}

static s32 Fld_NSymbolUGMgr_Update(TaskPool* pool, Task* task, void* args) {
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
            if (data->ids[i] != -1 && Fld_NSymbolUG_IsNearPlayer(pool, data->ids[i]) != 0) {
                if (gSaveData.battleCount < Field_GetMaxChainedEncounters() && FieldScan_IsBattleStarting() == 0) {
                    if (FieldScan_IsEnteringBattle() == 0) {
                        if (data->unk_218 == 0) {
                            Fld_NSymbolUG_MarkDefeated(pool, data->ids[i]);
                            data->unk_218 = data->unk_218 + 1;
                        }
                        break;
                    }
                    Fld_NSymbolUG_MarkDefeated(pool, data->ids[i]);
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
                    Fld_Luck_SetTarget(pool, data->luckTaskId, data->dropRate);
                    FieldScan_AddTime(0x14);
                    data->chainCount = data->chainCount + 1;
                    if (data->chainTaskId != -1) {
                        EasyTask_DeleteTask(pool, data->chainTaskId);
                    }
                    data->chainTaskId = Fld_Chain_CreateTask(pool, data->dataType, data->chainCount);
                } else {
                    break;
                }
            }
            i++;
        } while (i < data->count);
    }
    return 1;
}

static s32 Fld_NSymbolUGMgr_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Fld_NSymbolUGMgr_Destroy(TaskPool* pool, Task* task, void* args) {
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
    Fld_NSymbolUGMgr_Instance = NULL;
    return 1;
}

static s32 Fld_NSymbolUGMgr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_NSymbolUGMgr_Init,
        .update     = Fld_NSymbolUGMgr_Update,
        .render     = Fld_NSymbolUGMgr_Render,
        .cleanup    = Fld_NSymbolUGMgr_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_NSymbolUGMgr_CreateTask(TaskPool* pool, s32 arg1) {
    s32 param;

    param = arg1;
    return EasyTask_CreateTask(pool, &Tsk_Fld_NSymbolUGMgr, NULL, 0, NULL, &param);
}
