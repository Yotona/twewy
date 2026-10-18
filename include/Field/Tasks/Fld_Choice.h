#ifndef FIELD_TASKS_FLD_CHOICE_H
#define FIELD_TASKS_FLD_CHOICE_H

#include "Engine/EasyTask.h"

s32 Fld_Choice_CreateTask(TaskPool* pool, s32 arg1, s16 arg2, s16* arg3);
s16 Fld_Choice_GetResult(TaskPool* pool, Task* task);

#endif // FIELD_TASKS_FLD_CHOICE_H
