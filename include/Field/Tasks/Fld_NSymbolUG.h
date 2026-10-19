#ifndef FIELD_TASKS_FLD_NSYMBOLUG_H
#define FIELD_TASKS_FLD_NSYMBOLUG_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

s32  Fld_NSymbolUG_CreateTask(TaskPool* pool, s32 spawnType, FldScreenPos pos, s32 category, s16 mode, s32 progressFlagMask,
                              FieldPos* anchor, s16 recIdx);
s32  Fld_NSymbolUG_IsTaskTouched(TaskPool* pool, u32 id);
s32  Fld_NSymbolUG_IsNearPlayer(TaskPool* pool, u32 id);
void Fld_NSymbolUG_Capture(TaskPool* pool, u32 id);
void Fld_NSymbolUG_MarkDefeated(TaskPool* pool, u32 id);

#endif // FIELD_TASKS_FLD_NSYMBOLUG_H
