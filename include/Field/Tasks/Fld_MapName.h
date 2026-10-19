#ifndef FIELD_TASKS_FLD_MAPNAME_H
#define FIELD_TASKS_FLD_MAPNAME_H

#include "Engine/EasyTask.h"

s32  Fld_MapName_CreateTask(TaskPool* pool, s32 dataType, s32 messageId);
s32  Fld_MapName_CreateTaskOnMain(TaskPool* pool, s32 dataType, s32 messageId);
void Fld_MapName_SetPosition(Task* task, s16 x, s16 y);

#endif // FIELD_TASKS_FLD_MAPNAME_H
