#ifndef FIELD_TASKS_FLD_WALL_H
#define FIELD_TASKS_FLD_WALL_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

void Fld_Wall_SetAllExitStates(s32 arg0);
void Fld_Wall_SetAllWallStates(s32 arg0);
void Field_ResetDayProgress(void);
s32  Fld_Wall_CreateTask(TaskPool* pool, s32 dataType, FieldPos* origin, s32 dir);

#endif // FIELD_TASKS_FLD_WALL_H
