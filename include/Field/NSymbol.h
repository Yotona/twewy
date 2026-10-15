#ifndef FIELD_NSYMBOL_H
#define FIELD_NSYMBOL_H

// Scan-mode Noise symbols: the symbol tasks (Fld_NSymbolUG), their manager
// (Fld_NSymbolUGMgr) and the saved records helpers (Fld_Luck). The records
// themselves are MainData::scanSymbols.

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

// Task data of the symbol manager; data_ov030_02109d78 points at it while it runs.
typedef struct {
    /* 0x000 */ s32 ids[0x80];   // symbol task per scan record, -1 when none
    /* 0x200 */ s32 luckTaskId;  // luck counter popup (func_ov030_020c2010)
    /* 0x204 */ s32 chainTaskId; // chain counter popup (func_ov030_020c2458)
    /* 0x208 */ s32 dataType;    // sprite data slot handed to every spawned task
    /* 0x20C */ s32 chainCount;
    /* 0x210 */ s32 count;       // entries of ids[] in use
    /* 0x214 */ s32 dropRate;
    /* 0x218 */ s32 unk_218;
} NsymUGMgr; // Size: 0x21C

extern NsymUGMgr* data_ov030_02109d78;

// Fld_NSymbolUG
s32  func_ov030_020bbbb4(TaskPool* pool, s32 spawnType, FldScreenPos pos, s32 category, s16 mode, s32 progressFlagMask,
                         FieldPos* anchor, s16 recIdx);
s32  func_ov030_020bbc24(TaskPool* pool, u32 id);
s32  func_ov030_020bbc38(TaskPool* pool, u32 id);
void func_ov030_020bbcb4(TaskPool* pool, u32 id);
void func_ov030_020bbdc4(TaskPool* pool, u32 id);

// Fld_Luck
s32  func_ov030_020c2010(TaskPool* pool, s32 dataType, s32 value);
void func_ov030_020c2044(TaskPool* pool, u32 id, s32 visible);
void func_ov030_020c2070(TaskPool* pool, u32 id, s32 value);

#endif // FIELD_NSYMBOL_H
