#ifndef FIELD_TASKS_FLD_TOUCHSEAL_H
#define FIELD_TASKS_FLD_TOUCHSEAL_H

#include "Engine/EasyTask.h"

s32  Fld_TouchSeal_CreateTask(TaskPool* pool, s32 arg1, s32 arg2, s16 arg3, s16 arg4);
void Fld_TouchSeal_SetAnimation(Task* task, s16 arg1);
s32  Fld_TouchSeal_IsTouched(Task* task);

#endif // FIELD_TASKS_FLD_TOUCHSEAL_H
