#ifndef FIELD_TASKS_FLD_LUCK_H
#define FIELD_TASKS_FLD_LUCK_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

s32  Fld_Luck_TrySelectSymbol(TaskPool* pool, u32 id1, u32 id2);
s32  Fld_Luck_RemoveActiveSymbols(TaskPool* pool, u32 id);
void Fld_Luck_SpawnSymbols(TaskPool* pool, u32 id, FieldPos* anchor);
s16  Fld_Luck_GetDropRate(void);
s32  Fld_Luck_AddSymbol(TaskPool* pool, s16 x, s16 y, s16 kind, s16 mode, s32 mask);
void Fld_Luck_GenerateSymbols(void);
s32  Fld_Luck_IsScanEventAvailable(void);
s16  Fld_Luck_GetNormalSymbolPercent(void);
s32  Fld_Luck_CreateTask(TaskPool* pool, s32 dataType, s32 value);
void Fld_Luck_SetTarget(TaskPool* pool, u32 id, s32 arg2);

#endif // FIELD_TASKS_FLD_LUCK_H
