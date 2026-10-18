#ifndef FIELD_TASKS_FLD_PIRANHA_H
#define FIELD_TASKS_FLD_PIRANHA_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

s32 Fld_Piranha_CreateTask(TaskPool* pool, s32 dataType, FieldPos* origin, s32 index);
s32 Fld_Piranha_IsNear(TaskPool* pool, u32 id, s32 arg);
u16 Field_GetEncounterCategory(s32 encounter);

#endif // FIELD_TASKS_FLD_PIRANHA_H
