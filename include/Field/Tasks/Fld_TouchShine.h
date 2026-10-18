#ifndef FIELD_TASKS_FLD_TOUCHSHINE_H
#define FIELD_TASKS_FLD_TOUCHSHINE_H

#include "Engine/EasyTask.h"

s32  Fld_TouchShine_CreateTask(TaskPool* pool, s32 arg1);
void Fld_TouchShine_SetActiveCount(s16 arg0);
s16  Fld_TouchShine_GetActiveCount(void);

#endif // FIELD_TASKS_FLD_TOUCHSHINE_H
