#ifndef FIELD_TASKS_FLD_NSYMBOLUGMGR_H
#define FIELD_TASKS_FLD_NSYMBOLUGMGR_H

#include "Engine/EasyTask.h"

// Task data of the symbol manager; Fld_NSymbolUGMgr_Instance points at it while it runs.
typedef struct {
    /* 0x000 */ s32 ids[0x80];   // symbol task per scan record, -1 when none
    /* 0x200 */ s32 luckTaskId;  // luck counter popup (Fld_Luck_CreateTask)
    /* 0x204 */ s32 chainTaskId; // chain counter popup (Fld_Chain_CreateTask)
    /* 0x208 */ s32 dataType;    // sprite data slot handed to every spawned task
    /* 0x20C */ s32 chainCount;
    /* 0x210 */ s32 count;       // entries of ids[] in use
    /* 0x214 */ s32 dropRate;
    /* 0x218 */ s32 unk_218;
} NsymUGMgr; // Size: 0x21C

extern NsymUGMgr* Fld_NSymbolUGMgr_Instance;
s32               Fld_NSymbolUGMgr_CreateTask(TaskPool* pool, s32 arg1);

#endif // FIELD_TASKS_FLD_NSYMBOLUGMGR_H
