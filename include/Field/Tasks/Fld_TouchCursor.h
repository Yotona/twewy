#ifndef FIELD_TASKS_FLD_TOUCHCURSOR_H
#define FIELD_TASKS_FLD_TOUCHCURSOR_H

#include "Engine/EasyTask.h"

s32  Fld_TouchCursor_CreateTask(TaskPool* pool, s32 arg1, s16 arg2);
void Fld_TouchCursor_SetAnimation(Task* task, s16 arg1);

#endif // FIELD_TASKS_FLD_TOUCHCURSOR_H
